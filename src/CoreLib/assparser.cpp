#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include <cstdio>
#include <cinttypes>

#include "boost/lexical_cast.hpp"
#include "boost/regex.hpp"
#include "boost/regex/icu.hpp"

#include "assparser.hpp"
#include "fonthandle.hpp"

using namespace std;

using boost::lexical_cast;
using boost::bad_lexical_cast;

AssParser::AssParser(const string &fileName) :
    metaData(make_shared<AssMeta>()),
    styleData(map<string, shared_ptr<AssStyle>>()),
    dialogParsed(false)
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

    // check ass file is valid or not
    if (dialogData.size() == 0)
    {
        throw invalid_argument("This ass file has no dialog data.");
    }

    if (metaData->play_res_x == 0 || metaData->play_res_y == 0)
    {
        cerr << "Warning: PlayRes is fallback to default." << endl;
        metaData->play_res_x = 640;
        metaData->play_res_y = 360;
    }

    if (styleData.size() == 0)
    {
        // set default style
        cerr << "Warning: CANNOT find any style data in ass file." << endl;
        cerr << "Warning: Create default style." << endl;
        
        shared_ptr<AssStyle> style(make_shared<AssStyle>());
        styleData["Default"] = style;
    }

    section = Idle;
}

shared_ptr<AssMeta> AssParser::meta() const
{
    return metaData;
}

map<string, shared_ptr<AssStyle>> AssParser::styles() const
{
    return styleData;
}

vector<shared_ptr<AssDialog>> AssParser::dialogs() const
{
    return dialogData;
}

void AssParser::upgradeDialogs()
{
    if (dialogParsed)
    {
        return;
    }

    parseDialogs();
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
    if (regex_match(line, boost::regex("^\\[.*\\]$")))
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
        if (regex_match(line, boost::regex("^WrapStyle: \\d$")))
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
        else if (regex_match(line, boost::regex("^ScaledBorderAndShadow: ([Yy]es|[Nn]o)$")))
        {
            string res(line.substr(23));
            metaData->scaled_border_and_shadow = (res == "Yes" || res == "yes");
        }
        else if (regex_match(line, boost::regex("^PlayResX: \\d+$")))
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
        else if (regex_match(line, boost::regex("^PlayResY: \\d+$")))
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
        boost::u32regex reg(boost::make_u32regex(
            "^Style: (.*),(.*),(\\d+),(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(-?[01]),(-?[01]),(-?[01]),(-?[01]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(-?\\d+\\.?\\d*),(-?\\d+\\.?\\d*),([13]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),([1-9]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+)$"));
        boost::smatch match;
        if(boost::u32regex_search(line, match, reg))
        {
            shared_ptr<AssStyle> style(make_shared<AssStyle>());
            try
            {
                style->encoding = lexical_cast<int>(match[match.size() - 1]);
            }
            catch (...)
            {
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
        boost::u32regex reg(boost::make_u32regex("^(.+): (\\d+),(\\d:\\d\\d:\\d\\d\\.\\d\\d),(\\d:\\d\\d:\\d\\d\\.\\d\\d),(.+),(.*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(|.*),(.*)$"));
        boost::smatch match;
        if(boost::u32regex_search(line, match, reg))
        {
            shared_ptr<AssDialog> dialog(make_shared<AssDialog>());
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

void AssParser::parseDialogs()
{
    double space_width(0.f);
    for (size_t i = 0; i < dialogData.size(); ++i)
    {
        shared_ptr<AssDialog> dialog(dialogData.at(i));
        dialog->i = i;
        dialog->duration = dialog->end_time - dialog->start_time;
        dialog->mid_time = dialog->start_time + (dialog->duration >> 1);
        dialog->styleref = styleData[dialog->style];
        if (dialog->styleref == nullptr)
        {
            cerr << "Waring: dialog " << dialog->i << " fallback to default style." << endl;
            dialog->styleref = make_shared<AssStyle>();
        }

        dialog->text_stripped = regex_replace(
            dialog->text,
            boost::regex("\\{[^\\{\\}]+\\}"),
            ""
        );

        TEXT_SIZE *textsize(nullptr);
        try
        {
            textsize = textSize(dialog->text_stripped, dialog->styleref);
        }
        catch (...)
        {
            string tmpString = "Fail to upgrade dialogs in dialog ";
            tmpString += lexical_cast<string>(dialog->i);
            throw runtime_error(tmpString);
        }

        dialog->width = textsize->width;
        dialog->height = textsize->height;
        dialog->ascent = textsize->ascent;
        dialog->descent = textsize->descent;
        dialog->internal_leading = textsize->internal_leading;
        dialog->external_leading = textsize->external_leading;

        // Horizontal position
        if (((dialog->styleref->alignment - 1) % 3) == 0)
        {
            dialog->left = (dialog->margin_l != 0 ? dialog->margin_l : dialog->styleref->margin_l);
            dialog->center = dialog->left + (dialog->width / 2.f);
            dialog->right = dialog->left + dialog->width;
            dialog->x = dialog->left;
        }
        else if (((dialog->styleref->alignment - 2) % 3) == 0)
        {
            dialog->left = static_cast<double>(metaData->play_res_x >> 1) - (dialog->width / 2.f);
            dialog->center = dialog->left + (dialog->width / 2.f);
            dialog->right = dialog->left + dialog->width;
            dialog->x = dialog->center;
        }
        else
        {
            dialog->left = metaData->play_res_x - (dialog->margin_r != 0 ? dialog->margin_r : dialog->styleref->margin_r) - dialog->width;
            dialog->center = dialog->left + (dialog->width / 2.f);
            dialog->right = dialog->left + dialog->width;
            dialog->x = dialog->right;
        }

        // Vertical position
        if (dialog->styleref->alignment > 6)
        {
            dialog->top = (dialog->margin_v != 0 ? dialog->margin_v : dialog->styleref->margin_v);
            dialog->middle = dialog->top + (dialog->height / 2.f);
            dialog->bottom = dialog->top + dialog->height;
            dialog->y = dialog->top;
        }
        else if (dialog->styleref->alignment > 3)
        {
            dialog->top = static_cast<double>(metaData->play_res_y >> 1) - (dialog->height / 2.f);
            dialog->middle = dialog->top + (dialog->height / 2.f);
            dialog->bottom = dialog->top + dialog->height;
            dialog->y = dialog->middle;
        }
        else
        {
            dialog->top = metaData->play_res_y - (dialog->margin_v != 0 ? dialog->margin_v : dialog->styleref->margin_v) - dialog->height;
            dialog->middle = dialog->top + (dialog->height / 2.f);
            dialog->bottom = dialog->top + dialog->height;
            dialog->y = dialog->bottom;
        }

        delete textsize;
        string tmpString(" ");
        try
        {
            textsize = textSize(tmpString, dialog->styleref);
        }
        catch (...)
        {
            tmpString = "Fail to upgrade dialogs in dialog ";
            tmpString += lexical_cast<string>(dialog->i);
            throw runtime_error(tmpString);
        }

        space_width = textsize->width;
        delete textsize;
        textsize = nullptr;

        // Add dialog text chunks
        dialog->textChunked.reserve(50);
        tmpString = dialog->text;
        boost::u32regex u32reg(boost::make_u32regex("\\{([^\\{\\}]+)\\}([^\\{]*)"));
        boost::smatch match;
        while (boost::u32regex_search(tmpString, match, u32reg))
        {
            shared_ptr<AssTextChunked> textChunked(make_shared<AssTextChunked>());
            if (match.prefix() != "")
            {
                textChunked->tags = "";
                textChunked->text = match.prefix();
                tmpString = match.str() + match.suffix();
                dialog->textChunked.push_back(textChunked);
                continue;
            }

            textChunked->tags = match[1];
            textChunked->text = match[2];
            dialog->textChunked.push_back(textChunked);
            tmpString = match.suffix();
        }

        // Add dialog sylables
        dialog->syls.reserve(50);
        uint64_t lastTime(0);
        u32reg = boost::make_u32regex("\\\\[kK][of]?(\\d+)");
        boost::u32regex u32regs(boost::make_u32regex("(\\s*)(\\S*)(\\s*)"));
        for (size_t index = 0; index < dialog->textChunked.size(); ++index)
        {
            if (boost::u32regex_search(dialog->textChunked.at(index)->tags, match, u32reg))
            {
                shared_ptr<AssSyl> syl(make_shared<AssSyl>());
                uint64_t kdur(0);
                tmpString = match[1];
                try
                {
                    kdur = lexical_cast<uint64_t>(tmpString);
                }
                catch (...)
                {
                    dialog->syls.clear();
                    throw invalid_argument("Error when getting syl's duration.");
                }

                syl->i = index;
                syl->start_time = lastTime;
                syl->mid_time = lastTime + kdur * 5; // kdur * 10 / 2
                syl->duration = kdur * 10;
                syl->end_time = lastTime + syl->duration;
                syl->tags = match.prefix() + match.suffix();

                if (boost::u32regex_search(
                        dialog->textChunked.at(index)->text,
                        match,
                        u32regs)
                )
                {
                    syl->text = match[2];
                    
                    tmpString = match[1];
                    syl->prespace = utf8StringLen(tmpString);
                    
                    tmpString = match[3];
                    syl->postspace = utf8StringLen(tmpString);
                }
                else
                {
                    syl->prespace = 0;
                    syl->postspace = 0;
                    syl->text = dialog->textChunked.at(index)->text;
                }

                try
                {
                    textsize = textSize(syl->text, dialog->styleref);
                }
                catch (...)
                {
                    dialog->syls.clear();
                    throw invalid_argument("Error when getting syl's data.");
                }

                syl->width = textsize->width;
                syl->height = textsize->height;
                syl->ascent = textsize->ascent;
                syl->descent = textsize->descent;
                syl->external_leading = textsize->external_leading;
                syl->internal_leading = textsize->internal_leading;

                delete textsize;
                textsize = nullptr;
                lastTime = syl->end_time;
                dialog->syls.push_back(syl);
            }
            else
            {
                dialog->syls.clear();
                break;
            }
        } //end for index

        // Calculate sylable positions with all sylables data already available
        if (dialog->syls.size() > 0)
        {
            if (dialog->syls.at(0)->width != 0.f)
            {
                if (dialog->styleref->alignment > 6 ||
                    dialog->styleref->alignment < 4)
                {
                    double cur_x(dialog->left);
                    for (size_t index = 0; index < dialog->syls.size(); ++index)
                    {
                        shared_ptr<AssSyl> syl(dialog->syls.at(index));

                        // Horizontal position
                        cur_x += (syl->prespace * space_width);
                        syl->left = cur_x;
                        syl->center = syl->left + (syl->width / 2.f);
                        syl->right = syl->left + syl->width;

                        if (((dialog->styleref->alignment - 1) % 3) == 0)
                        {
                            syl->x = syl->left;
                        }
                        else
                        {
                            if (((dialog->styleref->alignment - 2) % 3) == 0)
                            {
                                syl->x = syl->center;
                            }
                            else
                            {
                                syl->x = syl->right;
                            }
                        }

                        cur_x += (syl->postspace * space_width);

                        // Vertical position
                        syl->top = dialog->top;
                        syl->middle = dialog->middle;
                        syl->bottom = syl->bottom;
                        syl->y = dialog->y;
                    } //end for index
                }
                else
                {
                    double max_width(0.f), sum_height(0.f);
                    for (size_t index = 0; index < dialog->syls.size(); ++index)
                    {
                        shared_ptr<AssSyl> syl(dialog->syls.at(index));
                        max_width = max(max_width, syl->width);
                        sum_height += syl->height;
                    } //end for index

                    double cur_y(static_cast<double>(metaData->play_res_y >> 1) - (sum_height / 2.f));
                    double x_fix(0.f);
                    for (size_t index = 0; index < dialog->syls.size(); ++index)
                    {
                        shared_ptr<AssSyl> syl(dialog->syls.at(index));
                        x_fix = ((max_width - syl->width) / 2.f);
                        if (dialog->styleref->alignment == 4)
                        {
                            syl->left = dialog->left + x_fix;
                            syl->center = syl->left + (syl->width / 2.f);
                            syl->right = syl->left + syl->width;
                            syl->x = syl->left;
                        }
                        else if (dialog->styleref->alignment == 5)
                        {
                            syl->left = static_cast<double>(metaData->play_res_x >> 1) - (syl->width / 2.f);
                            syl->center = syl->left + (syl->width / 2.f);
                            syl->right = syl->left + syl->width;
                            syl->x = syl->center; 
                        }
                        else
                        {
                            syl->left = dialog->right - syl->width - x_fix;
                            syl->center = syl->left + (syl->width / 2.f);
                            syl->right = syl->left + syl->width;
                            syl->x = syl->right;
                        } //end if

                        // Vertical position
                        syl->top = cur_y;
                        syl->middle = syl->top + (syl->height / 2.f);
                        syl->bottom = syl->top + syl->height;
                        syl->y = syl->middle;
                        cur_y += syl->height;
                    } //end for index
                }
            }
        } // end if (dialog->syls.size() > 0)
    } //end for i

    dialogParsed = true;
}

AssParser::TEXT_SIZE *AssParser::textSize(string &text, shared_ptr<AssStyle> &style)
{
    FontHandle font(FontHandle(style->fontname,
                    style->bold,
                    style->italic,
                    style->underline,
                    style->strikeout,
                    style->fontsize,
                    style->scale_x / 100.f,
                    style->scale_y / 100.f,
                    style->spaceing)
    );

    map<string, double> tmpMap;
    TEXT_SIZE *ret(new TEXT_SIZE);
    tmpMap = font.text_extents(text);
    ret->width = tmpMap["width"];
    ret->height = tmpMap["height"];

    tmpMap = font.metrics();
    ret->ascent = tmpMap["ascent"];
    ret->descent = tmpMap["descent"];
    ret->internal_leading = tmpMap["internal_leading"];
    ret->external_leading = tmpMap["external_leading"];
    return ret;
}

uint32_t AssParser::utf8StringLen(string &input)
{
    uint32_t len(0);
    for (size_t i = 0; i < input.length(); ++i)
    {
        len += (((uint8_t)input.at(i) & 0xc0) != 0x80);
    }

    return len;
}
