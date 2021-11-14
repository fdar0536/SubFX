/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "boost/lexical_cast.hpp"
#include "boost/regex.hpp"
#include "boost/regex/icu.hpp"

using boost::lexical_cast;
using boost::bad_lexical_cast;

#include "ass/data.h"
#include "ass.h"
#include "assparser.h"
#include "common.h"
#include "global.h"
#include "misc.h"

extern "C"
{

static void destoryDialogs(void *in)
{
    if (!in) return;

    fDSA *fdsa = getFDSA();
    subfx_ass_dialog *dialog = reinterpret_cast<subfx_ass_dialog *>(in);

    if (dialog->textChunked) fdsa->ptrVector.destory(dialog->textChunked);
    if (dialog->syls) fdsa->ptrVector.destory(dialog->syls);
    if (dialog->words) fdsa->ptrVector.destory(dialog->words);
    if (dialog->chars) fdsa->ptrVector.destory(dialog->chars);
    free(in);
}

static void myFree(void *in)
{
    if (!in) return;
    free(in);
}

static int myStrcmp(const void *lhs, const void *rhs)
{
    if (!lhs && !rhs) return 0;
    else if (lhs && !rhs) return 1;
    else if (!lhs && rhs) return -1;

    return strcmp(reinterpret_cast<const char *>(lhs),
                  reinterpret_cast<const char *>(rhs));
}

subfx_assParser *subfx_assParser_create(const char *fileName,
                                        const char *warningOut,
                                        char *errMsg)
{
    FILE *assFile;
    assFile = fopen(fileName, "r");
    if (!assFile)
    {
        subfx_pError(errMsg, "assParser_create: CANNOT open file.");
        return NULL;
    }

    AssParser *ret(reinterpret_cast<AssParser*>(calloc(1, sizeof(AssParser))));
    if (!ret)
    {
        fclose(assFile);
        return NULL;
    }

    if (!warningOut)
    {
        ret->logger = subfx_logger_create(stdout, stderr, false);
    }
    else
    {
        ret->logger = subfx_logger_create2(warningOut, warningOut);
    }

    if (!ret->logger)
    {
        fclose(assFile);
        subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
        return NULL;
    }

    fDSA *fdsa = getFDSA();
    ret->dialogs = fdsa->ptrVector.create(destoryDialogs);
    if (!ret->dialogs)
    {
        fclose(assFile);
        subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
        return NULL;
    }

    if (fdsa->ptrVector.reserve(ret->dialogs, 1000) == fdsa_failed)
    {
        fclose(assFile);
        subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
        return NULL;
    }

    ret->styles = fdsa->ptrMap.create(myStrcmp, myFree, myFree);
    if (!ret->styles)
    {
        fclose(assFile);
        subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
        return NULL;
    }

    subfx_ass_meta_init(&ret->meta);
    char tmpString[65536];
    uint8_t flag(1);
    uint8_t flags[] = {0, 0, 0, 0};
    subfx_exitstate exitstate;
    while(1)
    {
        exitstate = subfx_misc_getLine(tmpString, 65536, assFile, NULL);
        if (exitstate == subfx_eof) break;
        else if (exitstate == subfx_failed)
        {
            fclose(assFile);
            subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
            return NULL;
        }

        if (flag)
        {
            // for safety
            size_t len = strlen(tmpString);
            if (!len) // len == 0
            {
                subfx_pError(errMsg, "assParser_create: Is input "
                                     "an empty file?");
                fclose(assFile);
                subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
                return NULL;
            }

            flag = 0;
            subfx_assParser_checkBom(ret,
                                     reinterpret_cast<uint8_t *>(tmpString),
                                     &len);
        }

        if (subfx_assParser_parseLine(ret, tmpString, flags, errMsg))
        {
            fclose(assFile);
            subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
            return NULL;
        }
    }

    fclose(assFile);

    size_t size;
    // check ass file is valid or not
    if (fdsa->ptrVector.size(ret->dialogs, &size) == fdsa_failed)
    {
        subfx_pError(errMsg, "AssParser::create: You should never see this line.");
        subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
        return NULL;
    }

    if (!size) // size == 0
    {
        subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
        subfx_pError(errMsg, "AssParser::create: This ass file has no "
                                    "dialog data.");
        return NULL;
    }

    if (ret->meta.play_res_x == 0 ||
        ret->meta.play_res_y == 0)
    {
        subfx_logger_writeErr(ret->logger,
                              "Warning: PlayRes is fallback to default.\n");
        ret->meta.play_res_x = 640;
        ret->meta.play_res_y = 360;
    }

    uint8_t res;
    if (fdsa->ptrMap.isEmpty(ret->styles, &res) == fdsa_failed)
    {
        subfx_pError(errMsg, "AssParser::create: You should never see this line.");
        subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
        return NULL;
    }

    if (!res) // res == 0
    {
        // set default style
        subfx_logger_writeErr(ret->logger,
                              "Warning: CANNOT find any style data "
                               "in ass file.\n");
        subfx_logger_writeErr(ret->logger,
                              "Warning: Create default style.\n");

        subfx_ass_style *style = reinterpret_cast<subfx_ass_style *>
                (calloc(1, sizeof(subfx_ass_style)));
        if (!style)
        {
            subfx_pError(errMsg, "AssParser::create: Fail to create default style.");
            subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
            return NULL;
        }

        char *key = reinterpret_cast<char *>
                (calloc(8, sizeof(char)));

        if (!key)
        {
            free(style);
            subfx_pError(errMsg, "AssParser::create: Fail to create default style.");
            subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
            return NULL;
        }

        subfx_ass_style_init(style);
        memcpy(key, "Default", 8);

        if (fdsa->ptrMap.insertNode(ret->styles, key, style) == fdsa_failed)
        {
            subfx_pError(errMsg, "AssParser::create: You should never see this line.");
            subfx_assParser_destory(reinterpret_cast<subfx_assParser *>(ret));
            return NULL;
        }
    }

    ret->section = Idle;
    return reinterpret_cast<subfx_assParser *>(ret);
}

subfx_exitstate subfx_assParser_destory(subfx_assParser *in)
{
    if (!in) return subfx_failed;

    fDSA *fdsa = getFDSA();
    AssParser *parser = reinterpret_cast<AssParser *>(in);

    if (parser->dialogs) fdsa->ptrVector.destory(parser->dialogs);
    if (parser->logger) subfx_logger_destroy(parser->logger);
    if (parser->styles) fdsa->ptrMap.destory(parser->styles);

    free(parser);
    return subfx_success;
}

subfx_exitstate subfx_assParser_extendDialogs(subfx_assParser *in, char *errMsg)
{
    if (!in) return subfx_failed;
    AssParser *parser = reinterpret_cast<AssParser *>(in);
    if (parser->dialogParsed) return subfx_success;

    return subfx_assParser_parseDialogs(parser, errMsg);
}

subfx_exitstate subfx_assParser_dialogIsExtended(subfx_assParser *in,
                                                 bool *res)
{
    if (!in || !res) return subfx_failed;
    AssParser *parser = reinterpret_cast<AssParser *>(in);
    *res = parser->dialogParsed;
    return subfx_success;
}

void subfx_assParser_checkBom(AssParser *parser, uint8_t *in, size_t *inLen)
{
    if (*inLen < 3) return;

    // utf-8 bom
    if (in[0] != 0xef ||
        in[1] != 0xbb ||
        in[2] != 0xbf)
    {
        return;
    }

    subfx_logger_writeErr(parser->logger, "Warning: Remove utf-8 bom.\n");
    memmove(in, in + 3, *inLen - 2); // one more for '\0'
    *inLen -= 3;
}

uint8_t subfx_assParser_parseLine(AssParser *parser,
                                  const char *line,
                                  uint8_t *flags,
                                  char *errMsg)
{
    fDSA *fdsa = getFDSA();
    if (regex_match(line, boost::regex("^\\[.*\\]$")))
    {
        // ass section mark
        // [XXX]
        // if (res == "Script Info")
        if (!strcmp(line, "[Script Info]"))
        {
            if (flags[Script_Info])
            {
                subfx_pError(errMsg, "parseLine: Input is not a valid ass file.");
                return 1;
            }

            parser->section = Script_Info;
            ++flags[Script_Info];
        }
        else if (!strcmp(line, "[V4+ Styles]"))
        {
            if (flags[V4_Styles])
            {
                subfx_pError(errMsg,
                             "parseLine: Input is not a valid ass file.");
                return 1;
            }

            parser->section = V4_Styles;
            ++flags[V4_Styles];
        }
        else if (!strcmp(line, "[Events]"))
        {
            if (flags[Events])
            {
                subfx_pError(errMsg,
                             "parseLine: Input is not a valid ass file.");
                return 1;
            }

            parser->section = Events;
            ++flags[Events];
        }

        return 0;
    }

    size_t lineLen = strlen(line);
    switch (parser->section)
    {
    case Script_Info:
    {
        if (regex_match(line, boost::regex("^WrapStyle: \\d$")))
        {
            try
            {
                parser->meta.wrap_style = lexical_cast<uint8_t>(line + 11);
            }
            catch (const bad_lexical_cast &)
            {
                subfx_pError(errMsg,
                             "parseLine: Syntax error in\n "
                             "\"Script info\" -> \"WrapStyle\"");
                return 1;
            }
        }
        else if (regex_match(line, boost::regex("^ScaledBorderAndShadow:"
                                                " ([Yy]es|[Nn]o)$")))
        {
            std::string res(line+ 23);
            parser->meta.scaled_border_and_shadow = (res == "Yes" ||
                                                     res == "yes");
        }
        else if (regex_match(line, boost::regex("^PlayResX: \\d+$")))
        {
            try
            {
                parser->meta.play_res_x = lexical_cast<uint16_t>(line + 10);
            }
            catch (const bad_lexical_cast &)
            {
                subfx_pError(errMsg,
                             "parseLine: Syntax error in\n"
                             "\"Script info\" -> \"PlayResX\"");
                return 1;
            }
        }
        else if (regex_match(line, boost::regex("^PlayResY: \\d+$")))
        {
            try
            {
                parser->meta.play_res_y = lexical_cast<uint16_t>(line + 10);
            }
            catch (const bad_lexical_cast &)
            {
                subfx_pError(errMsg,
                             "parseLine: Syntax error in\n"
                             "\"Script info\" -> \"PlayResY\"");
                return 1;
            }
        }
        else if (regex_match(line, boost::regex("^YCbCr Matrix: (.*)")))
        {
            memcpy(parser->meta.colorMatrix, line + 14, lineLen - 14);
            parser->meta.colorMatrix[lineLen - 14] = '\0';
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
            subfx_ass_style *style = reinterpret_cast
                    <subfx_ass_style *>
                    (calloc(1, sizeof(subfx_ass_style)));
            if (!style)
            {
                return 1;
            }

            subfx_ass_style_init(style);
            try
            {
                style->encoding =
                        lexical_cast<int>(match[static_cast<int>(match.size()) - 1]);
            }
            catch (...)
            {
                subfx_pError(errMsg, "parseLine: Error in parsing "
                                            "style's encoding.");
                free(style);
                return 1;
            }

            if (style->encoding <= 255)
            {
                try
                {
                    memcpy(style->fontname,
                           match[2].str().c_str(),
                           match[2].str().length());

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
                    subfx_pError(errMsg, "parseLine: Error when parsing"
                                                " style.");
                    free(style);
                    return 1;
                }
            }

            std::string tmpString;
            char buf1[64], buf2[64];
            uint8_t buf1Size;

            // stringToColorAlpha may throw exception
            // but ignore here
            // color1 and alpha1
            tmpString = match[4];
            if (subfx_ass_stringToColorAlpha(tmpString.c_str(),
                                             buf1,
                                             &buf1Size,
                                             errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        3,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->color1, buf2, strlen(buf2) + 1); // one more for '\0'

            buf1[0] = buf1[3];
            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        1,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->alpha1, buf2, strlen(buf2) + 1); // one more for '\0'

            // color2 and alpha2
            tmpString = match[5];
            if (subfx_ass_stringToColorAlpha(tmpString.c_str(),
                                             buf1,
                                             &buf1Size,
                                             errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        3,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->color2, buf2, strlen(buf2) + 1); // one more for '\0'

            buf1[0] = buf1[3];
            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        1,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->alpha2, buf2, strlen(buf2) + 1); // one more for '\0'

            // color3 and alpha3
            tmpString = match[6];
            if (subfx_ass_stringToColorAlpha(tmpString.c_str(),
                                             buf1,
                                             &buf1Size,
                                             errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        3,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->color3, buf2, strlen(buf2) + 1); // one more for '\0'

            buf1[0] = buf1[3];
            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        1,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->alpha3, buf2, strlen(buf2) + 1); // one more for '\0'

            // color4 and alpha4
            tmpString = match[7];
            if (subfx_ass_stringToColorAlpha(tmpString.c_str(),
                                             buf1,
                                             &buf1Size,
                                             errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        3,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->color4, buf2, strlen(buf2) + 1); // one more for '\0'

            buf1[0] = buf1[3];
            if (subfx_ass_colorAlphaToString(
                        reinterpret_cast<uint8_t *>(buf1),
                        1,
                        buf2,
                        errMsg) == subfx_failed)
            {
                free(style);
                return 1;
            }

            memcpy(style->alpha4, buf2, strlen(buf2) + 1); // one more for '\0'

            tmpString = match[1];
            char *key = reinterpret_cast<char *>
                    (calloc(tmpString.length() + 1, sizeof(char)));
            if (!key)
            {
                free(style);
                return 1;
            }

            memcpy(key, tmpString.c_str(), tmpString.length());
            key[tmpString.length()] = '\0';

            if (fdsa->ptrMap.insertNode(parser->styles, key, style) == fdsa_failed)
            {
                free(key);
                free(style);
                return 1;
            }
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
            subfx_ass_dialog *dialog = reinterpret_cast<subfx_ass_dialog *>
                    (calloc(1, sizeof(subfx_ass_dialog)));
            if (!dialog)
            {
                return 1;
            }

            try
            {
                std::string tmpString = match[1];
                dialog->comment = (tmpString == "Comment");

                tmpString = match[2];
                dialog->layer = lexical_cast<uint32_t>(tmpString);

                tmpString = match[3];
                if (subfx_ass_stringToMs(tmpString.c_str(),
                                         &dialog->start_time,
                                         errMsg) == subfx_failed)
                {
                    free(dialog);
                    return 1;
                }

                tmpString = match[4];
                if (subfx_ass_stringToMs(tmpString.c_str(),
                                         &dialog->end_time,
                                         errMsg) == subfx_failed)
                {
                    free(dialog);
                    return 1;
                }

                tmpString = match[5];
                memcpy(dialog->style, tmpString.c_str(), tmpString.length());
                dialog->style[tmpString.length()] = '\0';

                tmpString = match[6];
                memcpy(dialog->actor, tmpString.c_str(), tmpString.length());
                dialog->actor[tmpString.length()] = '\0';

                tmpString = match[7];
                sscanf(tmpString.c_str(), "%lf", &(dialog->margin_l));

                tmpString = match[8];
                sscanf(tmpString.c_str(), "%lf", &(dialog->margin_r));

                tmpString = match[9];
                sscanf(tmpString.c_str(), "%lf", &(dialog->margin_v));

                tmpString = match[10];
                memcpy(dialog->effect, tmpString.c_str(), tmpString.length());
                dialog->effect[tmpString.length()] = '\0';

                tmpString = match[11];
                memcpy(dialog->text, tmpString.c_str(), tmpString.length());
                dialog->text[tmpString.length()] = '\0';

                if (fdsa->ptrVector.pushBack(parser->dialogs, dialog) == fdsa_failed)
                {
                    free(dialog);
                    return 1;
                }
            }
            catch (const bad_lexical_cast &)
            {
                subfx_pError(errMsg, "parseLine: Error when parsing dialog");
                free(dialog);
                return 1;
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}

subfx_exitstate subfx_assParser_parseDialogs(AssParser *parser,
                                             char *errMsg)
{
    double space_width(0.);
    size_t dialogsSize;
    fDSA *fdsa = getFDSA();
    if (fdsa->ptrVector.size(parser->dialogs, &dialogsSize) == fdsa_failed)
    {
       subfx_pError(errMsg,
                    "AssParser::parseDialogs: You should never see this line.");
       return subfx_failed;
    }

    for (size_t i = 0; i < dialogsSize; ++i)
    {
        subfx_ass_dialog *dialog(
                    reinterpret_cast<subfx_ass_dialog *>
                    (fdsa->ptrVector.at(parser->dialogs, i)));

        if (!dialog)
        {
            subfx_pError(errMsg,
                         "AssParser::parseDialogs: You should never see this line.");
            return subfx_failed;
        }

        dialog->i = static_cast<uint32_t>(i);
        dialog->duration = dialog->end_time - dialog->start_time;
        dialog->mid_time = dialog->start_time + (dialog->duration >> 1);
        dialog->styleref = reinterpret_cast
                <subfx_ass_style *>
                (fdsa->ptrMap.at(parser->styles, dialog->style));
        if (!dialog->styleref)
        {
            std::string out("Waring: dialog " + std::to_string(dialog->i));
            out += " fallback to default style.\n";
            subfx_logger_writeErr(parser->logger, out.c_str());
            dialog->styleref = reinterpret_cast<subfx_ass_style *>
                    (calloc(1, sizeof(subfx_ass_style)));

            if (!dialog->styleref)
            {
                subfx_pError(errMsg,
                             "AssParser::parseDialogs: "
                             "Fail to add default style.");
                return subfx_failed;
            }
        }

        dialog->text_stripped = regex_replace(
            &dialog->text,
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
                    syl->prespace =
                            PROJ_NAMESPACE::Utils::Utf8::stringLen(tmpString);

                    tmpString = match[3];
                    syl->postspace =
                            PROJ_NAMESPACE::Utils::Utf8::stringLen(tmpString);
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
#if (defined _MSC_VER) || (defined __APPLE__)
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
            word->prespace =
                    PROJ_NAMESPACE::Utils::Utf8::stringLen(tmpString);

            tmpString = match[3];
            word->postspace =
                    PROJ_NAMESPACE::Utils::Utf8::stringLen(tmpString);

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
        std::vector<std::string> charText(
                    PROJ_NAMESPACE::Utils::Utf8::stringSplit(
                        dialog->text_stripped));
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
                                 PROJ_NAMESPACE::Utils::Utf8::stringLen(
                                     syl->text) +
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
                                 PROJ_NAMESPACE::Utils::Utf8::stringLen(
                                     word->text) +
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
    return subfx_success;
}

}; // end extern "C"

/*

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

using namespace PROJ_NAMESPACE::Yutils;

void AssParser::extendDialogs() THROW
{
    if (dialogParsed)
    {
        return;
    }

    parseDialogs();
}

// private member functions

void AssParser::parseDialogs() THROW
{

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
*/
