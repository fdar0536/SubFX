#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include <cstdio>
#include <cinttypes>

#ifndef SCNd8
#define SCNd8 "hhd"
#endif
#ifndef SCNu8
#define SCNu8 "hhu"
#endif

#include "boost/lexical_cast.hpp"
#include "boost/regex.hpp"
#include "boost/regex/icu.hpp"

#include "YutilsCpp"

using boost::lexical_cast;
using boost::bad_lexical_cast;

using namespace PROJ_NAMESPACE::Yutils;

std::shared_ptr<AssParser>
AssParser::create(const std::string &fileName) THROW
{
    std::fstream assFile;
    assFile.open(fileName, std::fstream::in);
    if (assFile.fail())
    {
        throw std::invalid_argument("AssParser::create: CANNOT open file.");
    }

    AssParser *ret(new (std::nothrow) AssParser());
    if (!ret)
    {
        return nullptr;
    }

    ret->dialogData.reserve(1000);
    std::string tmpString;
    uint8_t flag(1);
    uint8_t flags[] = {0, 0, 0, 0};
    while(!ret->safeGetline(assFile, tmpString).eof())
    {
        if (flag)
        {
            // for safety
            if (tmpString.size() == 0)
            {
                delete ret;
                throw std::invalid_argument("AssParser::create: Is input "
                                            "an empty file?");
            }

            flag = 0;
            tmpString = ret->checkBom(tmpString);
        }

        // here may be throw exception
        ret->parseLine(tmpString, flags);
    }

    assFile.close();

    // check ass file is valid or not
    if (ret->dialogData.size() == 0)
    {
        delete ret;
        throw std::invalid_argument("AssParser::create: This ass file has no "
                                    "dialog data.");
    }

    if (ret->metaData->play_res_x == 0 ||
        ret->metaData->play_res_y == 0)
    {
        std::cerr << "Warning: PlayRes is fallback to default." << std::endl;
        ret->metaData->play_res_x = 640;
        ret->metaData->play_res_y = 360;
    }

    if (ret->styleData.size() == 0)
    {
        // set default style
        std::cerr << "Warning: CANNOT find any style data in ass file." << std::endl;
        std::cerr << "Warning: Create default style." << std::endl;

        std::shared_ptr<AssStyle> style(std::make_shared<AssStyle>());
        ret->styleData["Default"] = style;
    }

    ret->section = Idle;
    return std::shared_ptr<AssParser>(ret);
}

std::shared_ptr<AssMeta> AssParser::meta() const NOTHROW
{
    return metaData;
}

std::map<std::string, std::shared_ptr<AssStyle>> AssParser::styles() const NOTHROW
{
    return styleData;
}

std::vector<std::shared_ptr<AssDialog>> AssParser::dialogs() const NOTHROW
{
    return dialogData;
}

void AssParser::extendDialogs() THROW
{
    if (dialogParsed)
    {
        return;
    }

    parseDialogs();
}

bool AssParser::dialogIsExtended() const NOTHROW
{
    return dialogParsed;
}

bool AssParser::isSylAvailable() const NOTHROW
{
    return sylReady;
}

bool AssParser::isWordAvailable() const NOTHROW
{
    return wordReady;
}

bool AssParser::isCharAvailable() const NOTHROW
{
    return charReady;
}

// private member functions
std::istream &AssParser::safeGetline(std::istream &is, std::string &buf) NOTHROW
{
    buf.clear();
    buf.reserve(5120);

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    while(1)
    {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(buf.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            buf += static_cast<char>(c);
        }
    }
}

std::string AssParser::checkBom(std::string &in) NOTHROW
{
    // utf-8 bom
    if (static_cast<uint8_t>(in.at(0)) != 0xef ||
        static_cast<uint8_t>(in.at(1)) != 0xbb ||
        static_cast<uint8_t>(in.at(2)) != 0xbf)
    {
        return in;
    }

    std::cerr << "Warning: Remove utf-8 bom." << std::endl;
    return in.substr(3);
}

void AssParser::parseLine(std::string &line, uint8_t *flags) THROW
{
    if (regex_match(line, boost::regex("^\\[.*\\]$")))
    {
        // ass section mark
        // [XXX]
        std::string res(line.substr(1, line.length() - 2));
        if (res == "Script Info")
        {
            if (flags[Script_Info])
            {
                throw std::invalid_argument("parseLine: Input is not a "
                                            "valid ass file.");
            }

            section = Script_Info;
            ++flags[Script_Info];
        }
        else if (res == "V4+ Styles")
        {
            if (flags[V4_Styles])
            {
                throw std::invalid_argument("parseLine: Input is not a "
                                            "valid ass file.");
            }

            section = V4_Styles;
            ++flags[V4_Styles];
        }
        else if (res == "Events")
        {
            if (flags[Events])
            {
                throw std::invalid_argument("parseLine: Input is not a "
                                            "valid ass file.");
            }

            section = Events;
            ++flags[Events];
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
                throw std::invalid_argument("parseLine: Syntax error in\n"
                       "\"Script info\" -> \"WrapStyle\"");
            }
        }
        else if (regex_match(line, boost::regex("^ScaledBorderAndShadow:"
                                                " ([Yy]es|[Nn]o)$")))
        {
            std::string res(line.substr(23));
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
                throw std::invalid_argument("parseLine: Syntax error in\n"
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
                throw std::invalid_argument("parseLine: Syntax error in\n"
                       "\"Script info\" -> \"PlayResY\"");
            }
        }
        else if (regex_match(line, boost::regex("^YCbCr Matrix: (.*)")))
        {
            std::string res(line.substr(14));
            metaData->colorMatrix = res;
        }
        break;
    }
    case V4_Styles:
    {
        boost::u32regex reg(boost::make_u32regex(
            "^Style: (.*),(.*),(\\d+),(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),"
            "(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(-?[01]),(-?[01]),(-?[01]),"
            "(-?[01]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(-?\\d+\\.?\\d*),"
            "(-?\\d+\\.?\\d*),([13]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),([1-9]),"
            "(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+)$"));
        boost::smatch match;
        if(boost::u32regex_search(line, match, reg))
        {
            std::shared_ptr<AssStyle> style(std::make_shared<AssStyle>());
            try
            {
                style->encoding =
                        lexical_cast<int>(match[static_cast<int>(match.size()) - 1]);
            }
            catch (...)
            {
                throw std::invalid_argument("parseLine: Error in parsing "
                                            "style's encoding.");
            }

            if (style->encoding <= 255)
            {
                try
                {
                    style->fontname = match[2];
                    style->fontsize = lexical_cast<int>(match[3]);

                    // match[4~7] are not here

                    // boost will throw error when the number is negative.
                    std::string tmpString = match[8];
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
                    throw std::invalid_argument("parseLine: Error when parsing"
                                                " style.");
                }
            }

            std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> tmpTuple;
            std::vector<uint8_t> tmpVector;
            std::string tmpString;

            // stringToColorAlpha may throw exception
            // but ignore here
            // color1 and alpha1
            tmpString = match[4];
            tmpTuple = stringToColorAlpha(tmpString);

            tmpVector.reserve(3);
            tmpVector.push_back(std::get<0>(tmpTuple));
            tmpVector.push_back(std::get<1>(tmpTuple));
            tmpVector.push_back(std::get<2>(tmpTuple));
            style->color1 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(std::get<3>(tmpTuple));
            style->alpha1 = colorAlphaToString(tmpVector);
            tmpVector.clear();

            // color2 and alpha2
            tmpString = match[5];
            tmpTuple = stringToColorAlpha(tmpString);
            tmpVector.reserve(3);
            tmpVector.push_back(std::get<0>(tmpTuple));
            tmpVector.push_back(std::get<1>(tmpTuple));
            tmpVector.push_back(std::get<2>(tmpTuple));
            style->color2 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(std::get<3>(tmpTuple));
            style->alpha2 = colorAlphaToString(tmpVector);
            tmpVector.clear();

            // color3 and alpha3
            tmpString = match[6];
            tmpTuple = stringToColorAlpha(tmpString);
            tmpVector.reserve(3);
            tmpVector.push_back(std::get<0>(tmpTuple));
            tmpVector.push_back(std::get<1>(tmpTuple));
            tmpVector.push_back(std::get<2>(tmpTuple));
            style->color3 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(std::get<3>(tmpTuple));
            style->alpha3 = colorAlphaToString(tmpVector);
            tmpVector.clear();

            // color4 and alpha4
            tmpString = match[7];
            tmpTuple = stringToColorAlpha(tmpString);
            tmpVector.reserve(3);
            tmpVector.push_back(std::get<0>(tmpTuple));
            tmpVector.push_back(std::get<1>(tmpTuple));
            tmpVector.push_back(std::get<2>(tmpTuple));
            style->color4 = colorAlphaToString(tmpVector);
            tmpVector.clear();
            tmpVector.reserve(1);
            tmpVector.push_back(std::get<3>(tmpTuple));
            style->alpha4 = colorAlphaToString(tmpVector);

            styleData[match[1]] = style;
        }
        break;
    }
    case Events:
    {
        boost::u32regex reg(boost::make_u32regex("^(.+): (\\d+),"
                                                 "(\\d:\\d\\d:\\d\\d\\.\\d\\d),"
                                                 "(\\d:\\d\\d:\\d\\d\\.\\d\\d),"
                                                 "(.+),(.*),(\\d+\\.?\\d*),"
                                                 "(\\d+\\.?\\d*),(\\d+\\.?\\d*),"
                                                 "(|.*),(.*)$"));
        boost::smatch match;
        if(boost::u32regex_search(line, match, reg))
        {
            std::shared_ptr<AssDialog> dialog(std::make_shared<AssDialog>());

            // stringToMs may throw exception
            // but ignore here
            try
            {
                std::string tmpString = match[1];
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
            catch (const bad_lexical_cast &)
            {
                throw std::invalid_argument("parseLine: Error when parsing dialog");
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

void AssParser::parseDialogs() THROW
{
    double space_width(0.);
    for (size_t i = 0; i < dialogData.size(); ++i)
    {
        std::shared_ptr<AssDialog> dialog(dialogData.at(i));
        dialog->i = static_cast<uint32_t>(i);
        dialog->duration = dialog->end_time - dialog->start_time;
        dialog->mid_time = dialog->start_time + (dialog->duration >> 1);
        dialog->styleref = styleData[dialog->style];
        if (dialog->styleref == nullptr)
        {
            std::cerr << "Waring: dialog " << dialog->i;
            std::cerr << " fallback to default style.";
            std::cerr << std::endl;
            dialog->styleref = std::make_shared<AssStyle>();
        }

        dialog->text_stripped = regex_replace(
            dialog->text,
            boost::regex("\\{[^\\{\\}]+\\}"),
            ""
        );

        auto textsize(textSize(dialog->text_stripped, dialog->styleref));

        dialog->width = textsize->width;
        dialog->height = textsize->height;
        dialog->ascent = textsize->ascent;
        dialog->descent = textsize->descent;
        dialog->internal_leading = textsize->internal_leading;
        dialog->external_leading = textsize->external_leading;

        // Horizontal position
        if (((dialog->styleref->alignment - 1) % 3) == 0)
        {
            dialog->left = (dialog->margin_l != 0. ?
                        dialog->margin_l :
                        dialog->styleref->margin_l);
            dialog->center = dialog->left + (dialog->width / 2.);
            dialog->right = dialog->left + dialog->width;
            dialog->x = dialog->left;
        }
        else if (((dialog->styleref->alignment - 2) % 3) == 0)
        {
            dialog->left = static_cast<double>(metaData->play_res_x >> 1) -
                    (dialog->width / 2.);
            dialog->center = dialog->left + (dialog->width / 2.);
            dialog->right = dialog->left + dialog->width;
            dialog->x = dialog->center;
        }
        else
        {
            dialog->left = metaData->play_res_x - (dialog->margin_r != 0. ?
                        dialog->margin_r :
                        dialog->styleref->margin_r) - dialog->width;
            dialog->center = dialog->left + (dialog->width / 2.);
            dialog->right = dialog->left + dialog->width;
            dialog->x = dialog->right;
        }

        // Vertical position
        if (dialog->styleref->alignment > 6)
        {
            dialog->top = (dialog->margin_v != 0. ?
                        dialog->margin_v :
                        dialog->styleref->margin_v);
            dialog->middle = dialog->top + (dialog->height / 2.);
            dialog->bottom = dialog->top + dialog->height;
            dialog->y = dialog->top;
        }
        else if (dialog->styleref->alignment > 3)
        {
            dialog->top = static_cast<double>(metaData->play_res_y >> 1) -
                    (dialog->height / 2.);
            dialog->middle = dialog->top + (dialog->height / 2.);
            dialog->bottom = dialog->top + dialog->height;
            dialog->y = dialog->middle;
        }
        else
        {
            dialog->top = metaData->play_res_y - (dialog->margin_v != 0. ?
                        dialog->margin_v :
                        dialog->styleref->margin_v) - dialog->height;
            dialog->middle = dialog->top + (dialog->height / 2.);
            dialog->bottom = dialog->top + dialog->height;
            dialog->y = dialog->bottom;
        }

        std::string tmpString(" ");
        textsize = textSize(tmpString, dialog->styleref);

        space_width = textsize->width;
        textsize = nullptr;

        // Add dialog text chunks
        dialog->textChunked.reserve(50);
        tmpString = dialog->text;
        boost::u32regex u32reg(boost::make_u32regex("\\{([^\\{\\}]+)\\}([^\\{]*)"));
        boost::smatch match;
        while (boost::u32regex_search(tmpString, match, u32reg))
        {
            std::shared_ptr<AssTextChunked>
                    textChunked(std::make_shared<AssTextChunked>());

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
            if (boost::u32regex_search(dialog->textChunked.at(index)->tags,
                                       match, u32reg))
            {
                std::shared_ptr<AssSyl> syl(std::make_shared<AssSyl>());
                uint64_t kdur(0);
                tmpString = match[1];
                try
                {
                    kdur = lexical_cast<uint64_t>(tmpString);
                }
                catch (...)
                {
                    dialog->syls.clear();
                    throw std::invalid_argument("parseDialogs: Error when "
                                                "getting syl's duration.");
                }

                syl->i = static_cast<uint32_t>(index);
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

                textsize = textSize(syl->text, dialog->styleref);

                syl->width = textsize->width;
                syl->height = textsize->height;
                syl->ascent = textsize->ascent;
                syl->descent = textsize->descent;
                syl->external_leading = textsize->external_leading;
                syl->internal_leading = textsize->internal_leading;

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
            if (dialog->syls.at(0)->width != 0.)
            {
                if (dialog->styleref->alignment > 6 ||
                    dialog->styleref->alignment < 4)
                {
                    double cur_x(dialog->left);
                    for (size_t index = 0; index < dialog->syls.size(); ++index)
                    {
                        std::shared_ptr<AssSyl> syl(dialog->syls.at(index));

                        // Horizontal position
                        cur_x += (syl->prespace * space_width);
                        syl->left = cur_x;
                        syl->center = syl->left + (syl->width / 2.);
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

                        cur_x += (syl->width + (syl->postspace * space_width));

                        // Vertical position
                        syl->top = dialog->top;
                        syl->middle = dialog->middle;
                        syl->bottom = syl->bottom;
                        syl->y = dialog->y;
                    } //end for index
                }
                else
                {
                    double max_width(0.), sum_height(0.);
                    for (size_t index = 0; index < dialog->syls.size(); ++index)
                    {
                        std::shared_ptr<AssSyl> syl(dialog->syls.at(index));
                        max_width = std::max(max_width, syl->width);
                        sum_height += syl->height;
                    } //end for index

                    double cur_y(static_cast<double>(metaData->play_res_y >> 1) -
                                 (sum_height / 2.));
                    double x_fix(0.);
                    for (size_t index = 0; index < dialog->syls.size(); ++index)
                    {
                        std::shared_ptr<AssSyl> syl(dialog->syls.at(index));
                        x_fix = ((max_width - syl->width) / 2.);
                        if (dialog->styleref->alignment == 4)
                        {
                            syl->left = dialog->left + x_fix;
                            syl->center = syl->left + (syl->width / 2.);
                            syl->right = syl->left + syl->width;
                            syl->x = syl->left;
                        }
                        else if (dialog->styleref->alignment == 5)
                        {
                            syl->left = static_cast<double>(
                                        metaData->play_res_x >> 1) -
                                    (syl->width / 2.);
                            syl->center = syl->left + (syl->width / 2.);
                            syl->right = syl->left + syl->width;
                            syl->x = syl->center;
                        }
                        else
                        {
                            syl->left = dialog->right - syl->width - x_fix;
                            syl->center = syl->left + (syl->width / 2.);
                            syl->right = syl->left + syl->width;
                            syl->x = syl->right;
                        } //end if

                        // Vertical position
                        syl->top = cur_y;
                        syl->middle = syl->top + (syl->height / 2.);
                        syl->bottom = syl->top + syl->height;
                        syl->y = syl->middle;
                        cur_y += syl->height;
                    } //end for index
                }
                // end if (dialog->styleref->alignment > 6 ||
                //         dialog->styleref->alignment < 4)
            } // end if (dialog->syls.at(0)->width != 0.f)
        } // end if (dialog->syls.size() > 0)

        // Add dialog words
        u32reg = boost::make_u32regex("(\\s*)(\\S+)(\\s*)");
        dialog->words.reserve(50);
        tmpString = dialog->text_stripped;
        uint32_t wordIndex(0);
        bool result(false);
        while (true)
        {
#ifdef _MSC_VER
            try
            {
                result = boost::u32regex_search(tmpString, match, u32reg);
            }
            catch (std::out_of_range &)
            {
                break;
            }
#else
            result = boost::u32regex_search(tmpString, match, u32reg);
#endif
            if (!result)
            {
                break;
            }

            std::shared_ptr<AssWord> word(std::make_shared<AssWord>());

            word->text = match[2];
            textsize = textSize(word->text, dialog->styleref);

            tmpString = match[1];
            word->prespace = utf8StringLen(tmpString);

            tmpString = match[3];
            word->postspace = utf8StringLen(tmpString);

            word->i = wordIndex;
            word->start_time = dialog->start_time;
            word->mid_time = dialog->mid_time;
            word->end_time = dialog->end_time;
            word->duration = dialog->duration;

            word->width = textsize->width;
            word->height = textsize->height;
            word->ascent = textsize->ascent;
            word->descent = textsize->descent;
            word->internal_leading = textsize->internal_leading;
            word->external_leading = textsize->external_leading;

            textsize = nullptr;
            dialog->words.push_back(word);
            tmpString = match.suffix();
            ++wordIndex;
        } //end while(true)

        // Calculate word positions with all words data already available
        if (dialog->words.size() > 0)
        {
            if (dialog->words.at(0)->width != 0.)
            {
                if (dialog->styleref->alignment > 6 ||
                    dialog->styleref->alignment < 4)
                {
                    double cur_x(dialog->left);
                    for (size_t index = 0; index < dialog->words.size(); ++index)
                    {
                        std::shared_ptr<AssWord> word(dialog->words.at(index));

                        // Horizontal position
                        cur_x += (word->prespace * space_width);
                        word->left = cur_x;
                        word->center = word->left + (word->width / 2.);
                        word->right = word->left + word->width;

                        if (((dialog->styleref->alignment - 1) % 3) == 0)
                        {
                            word->x = word->left;
                        }
                        else
                        {
                            if (((dialog->styleref->alignment - 2) % 3) == 0)
                            {
                                word->x = word->center;
                            }
                            else
                            {
                                word->x = word->right;
                            }
                        }

                        cur_x += (word->width + (word->postspace * space_width));

                        // Vertical position
                        word->top = dialog->top;
                        word->middle = dialog->middle;
                        word->bottom = dialog->bottom;
                        word->y = dialog->y;
                    } //end for index
                }
                else
                {
                    double max_width(0.), sum_height(0.);
                    for (size_t index = 0; index < dialog->words.size(); ++index)
                    {
                        std::shared_ptr<AssWord> word(dialog->words.at(index));
                        max_width = std::max(max_width, word->width);
                        sum_height += word->height;
                    } //end for index;

                    double cur_y(static_cast<double>(metaData->play_res_y >> 1) -
                                 (sum_height / 2.));
                    double x_fix(0.);
                    for (size_t index = 0; index < dialog->words.size(); ++index)
                    {
                        std::shared_ptr<AssWord> word(dialog->words.at(index));

                        // Horizontal position
                        x_fix = ((max_width - word->width) / 2.);
                        if (dialog->styleref->alignment == 4)
                        {
                            word->left = dialog->left + x_fix;
                            word->center = word->left + (word->width / 2.);
                            word->right = word->left + word->width;
                            word->x = word->left;
                        }
                        else if (dialog->styleref->alignment == 5)
                        {
                            word->left = static_cast<double>(
                                        metaData->play_res_x >> 1) -
                                    (word->width / 2.);
                            word->center = word->left + (word->width / 2.);
                            word->right = word->left + word->width;
                            word->x = word->center;
                        }
                        else // dialog->styleref->alignment == 6
                        {
                            word->left = dialog->right - word->width - x_fix;
                            word->center = word->left + (word->width / 2.);
                            word->right = word->left + word->width;
                            word->x = word->right;
                        }

                        // Vertical position
                        word->top = cur_y;
                        word->middle = word->top + (word->height / 2.);
                        word->bottom = word->top + word->height;
                        cur_y += word->height;
                    } // end for index
                }
                // end if (dialog->styleref->alignment > 6 ||
                //         dialog->styleref->alignment < 4)
            } //end if (dialog->words.at(0)->width != 0.f)
        } // end if (dialog->words.size() > 0)

        // Add dialog characters
        dialog->chars.reserve(50);
        std::vector<std::string> charText(utf8StringSplit(dialog->text_stripped));
        for (size_t cIndex = 0; cIndex < charText.size(); ++cIndex)
        {
            std::shared_ptr<AssChar> assChar(std::make_shared<AssChar>());
            assChar->i = static_cast<uint32_t>(cIndex);
            assChar->start_time = dialog->start_time;
            assChar->mid_time = dialog->mid_time;
            assChar->end_time = dialog->end_time;
            assChar->duration = dialog->duration;
            assChar->text = charText.at(cIndex);

            uint32_t charIndex(0);
            for (size_t index = 0; index < dialog->syls.size(); ++index)
            {
                std::shared_ptr<AssSyl> syl(dialog->syls.at(index));
                uint32_t maxLoop(syl->prespace +
                                 utf8StringLen(syl->text) +
                                 syl->postspace);
                for (size_t sIndex = 0; sIndex < maxLoop; ++sIndex)
                {
                    if (charIndex == assChar->i)
                    {
                        assChar->syl_i = static_cast<int>(syl->i);
                        assChar->start_time = syl->start_time;
                        assChar->mid_time = syl->mid_time;
                        assChar->end_time = syl->end_time;
                        assChar->duration = syl->duration;
                        goto syl_reference_found;
                    }
                    ++charIndex;
                } // end for sIndex
            } // end for index

syl_reference_found:
            charIndex = 0;
            for (size_t index = 0; index < dialog->words.size(); ++index)
            {
                std::shared_ptr<AssWord> word(dialog->words.at(index));
                uint32_t maxLoop(word->prespace +
                                 utf8StringLen(word->text) +
                                 word->postspace);
                for (size_t wIndex = 0; wIndex < maxLoop; ++wIndex)
                {
                    if (charIndex == assChar->i)
                    {
                        assChar->word_i = static_cast<int>(word->i);
                        goto word_reference_found;
                    }
                    ++charIndex;
                } // end for wIndex;
            } // end for index

word_reference_found:
            textsize = textSize(assChar->text, dialog->styleref);

            assChar->width = textsize->width;
            assChar->height = textsize->height;
            assChar->ascent = textsize->ascent;
            assChar->descent = textsize->descent;
            assChar->internal_leading = textsize->internal_leading;
            assChar->external_leading = textsize->external_leading;

            textsize = nullptr;
            dialog->chars.push_back(assChar);
        } //end for cIndex

        // Calculate character positions with all characters data already available
        if (dialog->chars.size() > 0)
        {
            if (dialog->chars.at(0)->width != 0.)
            {
                if (dialog->styleref->alignment > 6 ||
                    dialog->styleref->alignment < 4)
                {
                    double cur_x(dialog->left);
                    for (size_t index = 0; index < dialog->chars.size(); ++index)
                    {
                        std::shared_ptr<AssChar> assChar(dialog->chars.at(index));

                        // Horizontal position
                        assChar->left = cur_x;
                        assChar->center = assChar->left + (assChar->width / 2.);
                        assChar->right = assChar->left + assChar->width;

                        if (((dialog->styleref->alignment - 1) % 3) == 0)
                        {
                            assChar->x = assChar->left;
                        }
                        else
                        {
                            if (((dialog->styleref->alignment - 2) % 3) == 0)
                            {
                                assChar->x = assChar->center;
                            }
                            else
                            {
                                assChar->x = assChar->right;
                            }
                        }

                        cur_x += assChar->width;

                        // Vertical position
                        assChar->top = dialog->top;
                        assChar->middle = dialog->middle;
                        assChar->bottom = dialog->bottom;
                        assChar->y = dialog->y;
                    } // end for index
                }
                else
                {
                    double max_width(0.), sum_height(0.);
                    for (size_t index = 0; index < dialog->chars.size(); ++index)
                    {
                        std::shared_ptr<AssChar> assChar(dialog->chars.at(index));
                        max_width = std::max(max_width, assChar->width);
                        sum_height += assChar->height;
                    } // end for index

                    double cur_y(static_cast<double>(metaData->play_res_y >> 1) -
                                 (sum_height / 2.));
                    double x_fix(0.);
                    for (size_t index = 0; index < dialog->chars.size(); ++index)
                    {
                        std::shared_ptr<AssChar> assChar(dialog->chars.at(index));

                        // Horizontal position
                        x_fix = ((max_width - assChar->width) / 2);
                        if (dialog->styleref->alignment == 4)
                        {
                            assChar->left = dialog->left + x_fix;
                            assChar->center = assChar->left + (assChar->width / 2.);
                            assChar->right = assChar->left + assChar->width;
                            assChar->x = assChar->left;
                        }
                        else if (dialog->styleref->alignment == 5)
                        {
                            assChar->left = static_cast<double>(
                                        metaData->play_res_x >> 1) -
                                    (assChar->width / 2.);
                            assChar->center = assChar->left + (assChar->width / 2.);
                            assChar->right = assChar->left + assChar->width;
                            assChar->x = assChar->center;
                        }
                        else // dialog->styleref->alignment == 6
                        {
                            assChar->left = dialog->right - assChar->width - x_fix;
                            assChar->center = assChar->left + (assChar->width / 2.);
                            assChar->right = assChar->left + assChar->width;
                            assChar->y = assChar->right;
                        }

                        // Vertical position
                        assChar->top = cur_y;
                        assChar->middle = assChar->top + (assChar->height /2.);
                        assChar->bottom = assChar->top + assChar->height;
                        assChar->y = assChar->middle;
                        cur_y += assChar->height;
                    } // end for index
                }
                // end if (dialog->styleref->alignment > 6 ||
                //         dialog->styleref->alignment < 4)
            } //end if (dialog->chars.at(0)->width != 0)
        } //end if (dialog->chars.size() > 0)
    } //end for i

    // sort dialogs
    sort(dialogData.begin(), dialogData.end(),
        [](const std::shared_ptr<AssDialog> &a, const std::shared_ptr<AssDialog> &b)
        {
            return (a->start_time < b->start_time);
        }
    );

    for (size_t i = 0; i < dialogData.size(); ++i)
    {
        std::shared_ptr<AssDialog> dialog(dialogData.at(i));
        dialog->leadin = (i == 0 ?
                          1000.1 :
                          (dialog->start_time - dialogData.at(i - 1)->end_time));
        dialog->leadout = (i == (dialogData.size() - 1) ?
                           1000.1 :
                           (dialogData.at(i + 1)->start_time - dialog->end_time));
    }

    // here maybe have a better solution
    for (size_t i = 0; i < dialogData.size(); ++i)
    {
        auto dialog(dialogData.at(i));
        if (dialog->syls.size())
        {
            sylReady = true;
        }

        if (dialog->words.size())
        {
            wordReady = true;
        }

        if (dialog->chars.size())
        {
            charReady = true;
        }
    }

    dialogParsed = true;
}

std::shared_ptr<AssParser::TEXT_SIZE>
AssParser::textSize(std::string &text,
                    std::shared_ptr<AssStyle> &style)
{
    auto font(FontHandle::create(style->fontname,
                                 style->bold,
                                 style->italic,
                                 style->underline,
                                 style->strikeout,
                                 style->fontsize,
                                 style->scale_x / 100.,
                                 style->scale_y / 100.,
                                 style->spaceing));
    if (font == nullptr)
    {
        throw std::runtime_error("textSize: Fail to create FontHandle");
    }

    auto tmpMap(font->text_extents(text));
    if (tmpMap.empty())
    {
        throw std::runtime_error("textSize: Fail to get text properties");
    }

    TEXT_SIZE *ret(new (std::nothrow) TEXT_SIZE);
    if (!ret)
    {
        throw std::runtime_error("textSize: Fail to allocate memory.");
    }

    ret->width = tmpMap["width"];
    ret->height = tmpMap["height"];

    tmpMap = font->metrics();
    if (tmpMap.empty())
    {
        delete ret;
        throw std::runtime_error("textSize: Fail to get font metrics");
    }

    ret->ascent = tmpMap["ascent"];
    ret->descent = tmpMap["descent"];
    ret->internal_leading = tmpMap["internal_leading"];
    ret->external_leading = tmpMap["external_leading"];
    return std::shared_ptr<AssParser::TEXT_SIZE>(ret);
}
