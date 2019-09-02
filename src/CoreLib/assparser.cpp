#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <regex>

#include <cstdio>
#include <cinttypes>

#include "boost/lexical_cast.hpp"

#include "assparser.hpp"

using namespace std;

using boost::lexical_cast;
using boost::bad_lexical_cast;

AssParser::AssParser(const string &fileName) :
    metaData(new AssMeta()),
    styleData(map<string, AssStyle *>())
{
    CoreAss();
    fstream assFile;
    try
    {
        assFile.open(fileName, fstream::in);
    }
    catch (...)
    {
        throw invalid_argument("CANNOT open file");
    }

    dialogData.reserve(1000);
    string tmpString;
    while(!safeGetline(assFile, tmpString).eof())
    {
        parseLine(tmpString);
    }

    assFile.close();

    section = Idle;
}

AssParser::~AssParser()
{
    delete metaData;
    metaData = nullptr;
    for (auto i = styleData.begin(); i != styleData.end(); ++i)
    {
        delete i->second;
        i->second = nullptr;
    }

    for (auto i = dialogData.begin(); i != dialogData.end(); ++i)
    {
        delete *i;
    }
}

AssMeta *AssParser::meta() const
{
    return metaData;
}

map<string, AssStyle *> AssParser::styles() const
{
    return styleData;
}

vector<AssDialog *> AssParser::dialogs() const
{
    return dialogData;
}

// private member function
istream &AssParser::safeGetline(istream &is, string &buf)
{
    buf.clear();
    buf.reserve(5120);

    istream::sentry se(is, true);
    streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(buf.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            buf += (char)c;
        }
    }
}

void AssParser::parseLine(string &line)
{
    if (regex_match(line, regex("^\\[.*\\]$")))
    {
        // ass section mark
        // [XXX]
        string res(line.substr(1, line.length() - 2));
        if (res == "Script Info")
        {
            section = Script_Info;
        }
        else if (res == "V4+ Styles")
        {
            section = V4_Styles;
        }
        else if (res == "Events")
        {
            section = Events;
        }

        return;
    }

    switch (section)
    {
    case Script_Info:
    {
        if (regex_match(line, regex("^WrapStyle: \\d$")))
        {
            try
            {
                metaData->wrap_style = lexical_cast<uint8_t>(line.substr(11));
            }
            catch (const bad_lexical_cast &)
            {
                throw invalid_argument("Syntax error in\n"
                                        "\"Script info\" -> \"WrapStyle\"");
            }
        }
        else if (regex_match(line, regex("^ScaledBorderAndShadow: ([Yy]es|[Nn]o)$")))
        {
            string res(line.substr(23));
            metaData->scaled_border_and_shadow = (res == "Yes" || res == "yes");
        }
        else if (regex_match(line, regex("^PlayResX: \\d+$")))
        {
            try
            {
                metaData->play_res_x = lexical_cast<uint16_t>(line.substr(10));
            }
            catch (const bad_lexical_cast &)
            {
                throw invalid_argument("Syntax error in\n"
                                        "\"Script info\" -> \"PlayResX\"");
            }
        }
        else if (regex_match(line, regex("^PlayResY: \\d+$")))
        {
            try
            {
                metaData->play_res_y = lexical_cast<uint16_t>(line.substr(10));
            }
            catch (const bad_lexical_cast &)
            {
                throw invalid_argument("Syntax error in\n"
                                        "\"Script info\" -> \"PlayResY\"");
            }
        }
        break;
    }
    case V4_Styles:
    {
        smatch match;
        if
        (
            regex_search(line,
                        match,
                        regex(
            "^Style: (.*),(.*),(\\d+),(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(-?[01]),(-?[01]),(-?[01]),(-?[01]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(-?\\d+\\.?\\d*),(-?\\d+\\.?\\d*),([13]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),([1-9]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+)$"
                        ))
        )
        {
            AssStyle *style(new AssStyle());
            try
            {
                style->encoding = lexical_cast<int>(match[match.size() - 1]);
            }
            catch (...)
            {
                delete style;
                throw invalid_argument("Error in parsing style's encoding.");
            }

            if (style->encoding <= 255)
            {
                try
                {
                    style->fontname = match[2];
                    style->fontsize = lexical_cast<int>(match[3]);

                    // match[4~7] are not here

                    // boost will throw error when the number is negative.
                    string tmpString = match[8];
                    sscanf(tmpString.c_str(), "%" SCNd8, &(style->bold));

                    tmpString = match[9];
                    sscanf(tmpString.c_str(), "%" SCNd8, &(style->italic));

                    tmpString = match[10];
                    sscanf(tmpString.c_str(), "%" SCNd8, &(style->underline));

                    tmpString = match[11];
                    sscanf(tmpString.c_str(), "%" SCNd8, &(style->strikeout));

                    tmpString = match[12];
                    sscanf(tmpString.c_str(), "%lf", &(style->scale_x));

                    tmpString = match[13];
                    sscanf(tmpString.c_str(), "%lf", &(style->scale_y));

                    tmpString = match[14];
                    sscanf(tmpString.c_str(), "%lf", &(style->spaceing));

                    tmpString = match[15];
                    sscanf(tmpString.c_str(), "%lf", &(style->angle));

                    style->bolder_style = lexical_cast<uint8_t>(match[16]);

                    tmpString = match[17];
                    sscanf(tmpString.c_str(), "%lf", &(style->outline));

                    tmpString = match[18];
                    sscanf(tmpString.c_str(), "%lf", &(style->shadow));

                    style->alignment = lexical_cast<uint8_t>(match[19]);

                    tmpString = match[20];
                    sscanf(tmpString.c_str(), "%lf", &(style->margin_l));

                    tmpString = match[21];
                    sscanf(tmpString.c_str(), "%lf", &(style->margin_r));

                    tmpString = match[22];
                    sscanf(tmpString.c_str(), "%lf", &(style->margin_v));
                }
                catch (...)
                {
                    delete style;
                    throw invalid_argument("Error when parsing style.");
                }
            }

            tuple<uint8_t, uint8_t, uint8_t, uint8_t> tmpTuple;
            vector<uint8_t> tmpVector;
            string tmpString;

            // color1 and alpha1
            tmpString = match[4];
            tmpTuple = stringToColorAlpha(tmpString);
            tmpVector.reserve(3);
            tmpVector.push_back(get<0>(tmpTuple));
            tmpVector.push_back(get<1>(tmpTuple));
            tmpVector.push_back(get<2>(tmpTuple));
            style->color1 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(get<3>(tmpTuple));
            style->alpha1 = colorAlphaToString(tmpVector);
            tmpVector.clear();

            // color2 and alpha2
            tmpString = match[5];
            tmpTuple = stringToColorAlpha(tmpString);
            tmpVector.reserve(3);
            tmpVector.push_back(get<0>(tmpTuple));
            tmpVector.push_back(get<1>(tmpTuple));
            tmpVector.push_back(get<2>(tmpTuple));
            style->color2 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(get<3>(tmpTuple));
            style->alpha2 = colorAlphaToString(tmpVector);
            tmpVector.clear();

            // color3 and alpha3
            tmpString = match[6];
            tmpTuple = stringToColorAlpha(tmpString);
            tmpVector.reserve(3);
            tmpVector.push_back(get<0>(tmpTuple));
            tmpVector.push_back(get<1>(tmpTuple));
            tmpVector.push_back(get<2>(tmpTuple));
            style->color3 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(get<3>(tmpTuple));
            style->alpha3 = colorAlphaToString(tmpVector);
            tmpVector.clear();

            // color4 and alpha4
            tmpString = match[7];
            tmpTuple = stringToColorAlpha(tmpString);
            tmpVector.reserve(3);
            tmpVector.push_back(get<0>(tmpTuple));
            tmpVector.push_back(get<1>(tmpTuple));
            tmpVector.push_back(get<2>(tmpTuple));
            style->color4 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(get<3>(tmpTuple));
            style->alpha4 = colorAlphaToString(tmpVector);

            styleData[match[1]] = style;
        }
        break;
    }
    case Events:
    {
        smatch match;
        if
        (
            regex_search(line,
                         match,
                         regex(
        "^(.*): (\\d+),(\\d:\\d\\d:\\d\\d\\.\\d\\d),(\\d:\\d\\d:\\d\\d\\.\\d\\d),(.*),(.*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(.*),(.*)$"
                         ))
        )
        {
            AssDialog *dialog(new AssDialog());
            try
            {
                string tmpString = match[1];
                dialog->comment = (tmpString == "Comment");

                tmpString = match[2];
                dialog->layer = lexical_cast<uint32_t>(tmpString);

                tmpString = match[3];
                dialog->start_time = stringToMs(tmpString);

                tmpString = match[4];
                dialog->end_time = stringToMs(tmpString);

                dialog->style = match[5];
                dialog->actor = match[6];

                tmpString = match[7];
                sscanf(tmpString.c_str(), "%lf", &(dialog->margin_l));

                tmpString = match[8];
                sscanf(tmpString.c_str(), "%lf", &(dialog->margin_r));

                tmpString = match[9];
                sscanf(tmpString.c_str(), "%lf", &(dialog->margin_v));

                dialog->effect = match[10];
                dialog->text = match[11];
                dialogData.push_back(dialog);
            }
            catch (...)
            {
                delete dialog;
                throw invalid_argument("Error when parsing dialog");
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
}
