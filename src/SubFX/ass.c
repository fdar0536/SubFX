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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "regex.h"
#include "ass.h"

// static data for regex
#define REGEX_COUNT           4
#define REGEX_STRING_TO_MS    0
#define REGEX_APLHA_ONLY      1
#define REGEX_ASS_COLOR       2
#define REGEX_ASS_ALPHA_COLOR 3

static RegexData subfx_ass_regex[REGEX_COUNT] = {0};

subfx_exitstate subfx_ass_init(subfx_ass_api *ret)
{
    if (!ret)
    {
        return subfx_failed;
    }

    // fill static arrays
    subfx_ass_fin();

    if (RegexData_init(&subfx_ass_regex[REGEX_STRING_TO_MS],
                       "^\\d:\\d\\d:\\d\\d\\.\\d\\d$"))
    {
        subfx_ass_fin();
        return subfx_failed;
    }

    if (RegexData_init(&subfx_ass_regex[REGEX_APLHA_ONLY],
                       "^&[Hh]{1}[0-9a-fA-F]{2}&$"))
    {
        subfx_ass_fin();
        return subfx_failed;
    }

    if (RegexData_init(&subfx_ass_regex[REGEX_ASS_COLOR],
                       "^&[Hh]{1}[0-9a-fA-F]{6}&$"))
    {
        subfx_ass_fin();
        return subfx_failed;
    }

    if (RegexData_init(&subfx_ass_regex[REGEX_ASS_ALPHA_COLOR],
                       "^&[Hh]{1}[0-9a-fA-F]{8}$"))
    {
        subfx_ass_fin();
        return subfx_failed;
    }

    ret->stringToMs = subfx_ass_stringToMs;
    ret->msToString = subfx_ass_msToString;
    ret->stringToColorAlpha = subfx_ass_stringToColorAlpha;
    ret->colorAlphaToString = subfx_ass_colorAlphaToString;

    return subfx_success;
}

void subfx_ass_fin()
{
    uint8_t index = 0;
    for (index = 0; index < REGEX_COUNT; ++index)
    {
        RegexData_fin(&subfx_ass_regex[index]);
    }

    memset(subfx_ass_regex, 0, REGEX_COUNT * sizeof(RegexData));
}

subfx_exitstate subfx_ass_stringToMs(const char *in,
                                     uint64_t *dst,
                                     char *errMsg)
{
    if (!in || !dst)
    {
        subfx_pError(errMsg, "stringToMs: No output!");
        return subfx_failed;
    }

    int pcre2Ret;
    if (RegexData_match(&subfx_ass_regex[REGEX_STRING_TO_MS],
                        in,
                        &pcre2Ret))
    {
        subfx_pError(errMsg, "stringToMs: ASS timestamp expected");
        return subfx_failed;
    }

    uint64_t hr, min, sec, centisec;
    if (sscanf(in, "%llu:%llu:%llu.%llu", &hr, &min, &sec, &centisec) != 4)
    {
        subfx_pError(errMsg, "stringToMs: Cannot convert!");
        return subfx_failed;
    }

    *dst = 0;
    *dst += hr * 3600000;
    *dst += min * 60000;
    *dst += sec * 1000;
    *dst += centisec * 10;

    return subfx_success;
}

subfx_exitstate subfx_ass_msToString(uint64_t ms_ass, char *buf)
{
    if (!buf)
    {
        return subfx_failed;
    }

    uint32_t hr = (uint32_t)floor((double)(ms_ass / 3600000)) % 10; //hour
    uint32_t mins = (uint32_t)floor((double)(ms_ass % 3600000 / 60000)); // minutes
    uint32_t sec = (uint32_t)floor((double)(ms_ass % 60000 / 1000)); // second
    uint32_t csec = (uint32_t)floor((double)(ms_ass % 1000 / 10)); // centisecond

    sprintf(buf, "%d:%02d:%02d.%02d", hr, mins, sec, csec);
    return subfx_success;
}

subfx_exitstate subfx_ass_stringToColorAlpha(const char *in,
                                             uint8_t *ret,
                                             uint8_t *retLen,
                                             char *errMsg)
{
    if (!in || !ret || !retLen)
    {
        return subfx_failed;
    }

    uint8_t r = 0, g = 0, b = 0, a = 0;
    int pcre2Ret;
    char hexString[8] = {0};
    memcpy(hexString, "0x", 2);
    if (!RegexData_match(&subfx_ass_regex[REGEX_APLHA_ONLY],
                         in,
                         &pcre2Ret))
    {
        // alpha only &HAA&
        memcpy(hexString + 2, in + 2, 2);
        a = (uint8_t)strtoul(hexString, NULL, 0);

        *retLen = 1;
    }
    else if (!RegexData_match(&subfx_ass_regex[REGEX_ASS_COLOR],
                              in,
                              &pcre2Ret))
    {
        // ass color &HBBGGRR&
        memcpy(hexString + 2, in + 2, 2);
        b = (uint8_t)strtoul(hexString, NULL, 16);

        memcpy(hexString + 2, in + 4, 2);
        g = (uint8_t)strtoul(hexString, NULL, 0);

        memcpy(hexString + 2, in + 6, 2);
        r = (uint8_t)strtoul(hexString, NULL, 0);

        *retLen = 3;
    }
    else if (!RegexData_match(&subfx_ass_regex[REGEX_ASS_ALPHA_COLOR],
                              in,
                              &pcre2Ret))
    {
        // both &HAABBGGRR
        memcpy(hexString + 2, in + 2, 2);
        a = (uint8_t)strtoul(hexString, NULL, 16);

        memcpy(hexString + 2, in + 4, 2);
        b = (uint8_t)strtoul(hexString, NULL, 16);

        memcpy(hexString + 2, in + 6, 2);
        g = (uint8_t)strtoul(hexString, NULL, 0);

        memcpy(hexString + 2, in + 8, 2);
        r = (uint8_t)strtoul(hexString, NULL, 0);

        *retLen = 4;
    }
    else
    {
        subfx_pError(errMsg, "stringToColorAlpha: Invalid input");
        *retLen = 0;
        return subfx_failed;
    }

    ret[0] = r;
    ret[1] = g;
    ret[2] = b;
    ret[3] = a;

    return subfx_success;
}

subfx_exitstate subfx_ass_colorAlphaToString(uint8_t *input,
                                             size_t inputSize,
                                             char *buf,
                                             char *errMsg)
{
    if (inputSize != 1 &&
        inputSize != 3 &&
        inputSize != 4)
    {
        subfx_pError(errMsg, "colorAlphaToString: Invalid input!");
        return subfx_failed;
    }

    if (!buf)
    {
        return subfx_failed;
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

    return subfx_success;
}
