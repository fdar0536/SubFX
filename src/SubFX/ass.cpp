/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2021 fdar0536.
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

#include <regex>
#include <string>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "boost/lexical_cast.hpp"

#include "common.h"
#include "ass.h"

extern "C"
{

subfx_ass *subfx_ass_init()
{
    subfx_ass *ret(reinterpret_cast<subfx_ass *>
                          (calloc(1, sizeof(subfx_ass))));
    if (!ret)
    {
        return NULL;
    }

    ret->stringToMs = subfx_ass_stringToMs;
    ret->msToString = subfx_ass_msToString;
    ret->stringToColorAlpha = subfx_ass_stringToColorAlpha;
    ret->colorAlphaToString = subfx_ass_colorAlphaToString;

    return ret;
}

subfx_exitstate subfx_ass_stringToMs(const char *in,
                                     uint64_t *dst,
                                     char *errMsg)
{
    if (!dst)
    {
        subfx_pError(errMsg, "stringToMs: No output!");
        return subfx_failed;
    }

    std::regex reg("^\\d:\\d\\d:\\d\\d\\.\\d\\d$");
    if (!std::regex_match(in, reg))
    {
        subfx_pError(errMsg, "stringToMs: ASS timestamp expected");
        return subfx_failed;
    }

    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    std::string ass_ms = std::string(in);

    try
    {
        // hour
        *dst = lexical_cast<uint64_t>(ass_ms.substr(0, 1)) * 3600000;

        // minute
        *dst += (lexical_cast<uint64_t>(ass_ms.substr(2, 2)) * 60000);

        //second
        *dst += (lexical_cast<uint64_t>(ass_ms.substr(5, 2)) * 1000);

        // centisecond
        *dst += (lexical_cast<uint64_t>(ass_ms.substr(8, 2)) * 10);
    }
    catch (const bad_lexical_cast &) // for safety
    {
        subfx_pError(errMsg, "stringToMs: Cannot convert!");
        return subfx_failed;
    }

    return subfx_success;
}

char *subfx_ass_msToString(uint64_t ms_ass)
{
    char *buf(reinterpret_cast<char *>(calloc(500, sizeof(char))));
    if (!buf)
    {
        return NULL;
    }

    uint32_t hr(static_cast<int>(floor(ms_ass / 3600000)) % 10); //hour
    uint32_t mins(static_cast<uint32_t>(floor(ms_ass % 3600000 / 60000))); // minutes
    uint32_t sec(static_cast<uint32_t>(floor(ms_ass % 60000 / 1000))); // second
    uint32_t csec(static_cast<uint32_t>(floor(ms_ass % 1000 / 10))); // centisecond

    sprintf(buf, "%d:%02d:%02d.%02d", hr, mins, sec, csec);
    return buf;
}

uint8_t *subfx_ass_stringToColorAlpha(const char *in, char *errMsg)
{
    uint8_t *ret(reinterpret_cast<uint8_t *>(calloc(4, sizeof(uint8_t))));
    if (!ret)
    {
        subfx_pError(errMsg, "stringToColorAlpha: Fail to allocate memory");
        return NULL;
    }

    uint8_t r(0), g(0), b(0), a(0);
    std::string tmpString;
    std::string input = std::string(in);
    if (std::regex_match(input, std::regex("^&[Hh]{1}[0-9a-fA-F]{2}&$")))
    {
        // alpha only &HAA&
        tmpString = input.substr(2, 2);
        a = static_cast<uint8_t>(stoul(tmpString, NULL, 16));
    }
    else if (std::regex_match(input, std::regex("^&[Hh]{1}[0-9a-fA-F]{6}&$")))
    {
        // ass color &HBBGGRR&
        tmpString = input.substr(2, 2);
        b = static_cast<uint8_t>(stoul(tmpString, NULL, 16));

        tmpString = input.substr(4, 2);
        g = static_cast<uint8_t>(stoul(tmpString, NULL, 16));

        tmpString = input.substr(6, 2);
        r = static_cast<uint8_t>(stoul(tmpString, NULL, 16));
    }
    else if (std::regex_match(input, std::regex("^&[Hh]{1}[0-9a-fA-F]{8}$")))
    {
        // both &HAABBGGRR
        tmpString = input.substr(2, 2);
        a = static_cast<uint8_t>(stoul(tmpString, NULL, 16));

        tmpString = input.substr(4, 2);
        b = static_cast<uint8_t>(stoul(tmpString, NULL, 16));

        tmpString = input.substr(6, 2);
        g = static_cast<uint8_t>(stoul(tmpString, NULL, 16));

        tmpString = input.substr(8, 2);
        r = static_cast<uint8_t>(stoul(tmpString, NULL, 16));
    }
    else
    {
        subfx_pError(errMsg, "stringToColorAlpha: Invalid input");
        free(ret);
        return NULL;
    }

    ret[0] = r;
    ret[1] = g;
    ret[2] = b;
    ret[3] = a;

    return ret;
}

char *subfx_ass_colorAlphaToString(uint8_t *input, size_t inputSize,
                                   char *errMsg)
{
    if (inputSize != 1 &&
        inputSize != 3 &&
        inputSize != 4)
    {
        subfx_pError(errMsg, "colorAlphaToString: Invalid input!");
        return NULL;
    }

    char *buf(reinterpret_cast<char *>(calloc(500, sizeof(char))));
    if (!buf)
    {
        subfx_pError(errMsg, "colorAlphaToString: Fail to allocate memory");
        return NULL;
    }

    switch (inputSize)
    {
    case 1:
    {
        // alpha only &HAA&
        sprintf(buf, "&H%02X&", input[0]);
        break;
    }
    case 3:
    {
        // rgb &HBBGGRR&
        sprintf(buf, "&H%02X%02X%02X&",
                input[2],
                input[1],
                input[0]);
        break;
    }
    default:
    {
        // rgba &HAABBGGRR
        sprintf(buf, "&H%02X%02X%02X%02X",
                input[3],
                input[2],
                input[1],
                input[0]);
        break;
    }
    } // end switch

    return buf;
}

} // end extern "C"
