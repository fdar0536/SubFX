/*
 * This file is part of SubFX,
 * Copyright (c) 2020-2021 fdar0536
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include "SubFX.h"

#define StringToColorAlpha(x, y) \
    if (ass->stringToColorAlpha(y, x, &size, errMsg) == subfx_failed) \
    { \
        puts("Failed in testing stringToColorAlpha"); \
        SubFX_fin(&api); \
        return 1; \
    } \
    printf("r: %d, g: %d, b: %d, a: %d\n", \
           x[0], x[1], x[2], x[3]);

#define ColorAlphaToString(x, y, z) \
    if (ass->colorAlphaToString(y, z, x, errMsg) == subfx_failed) \
    { \
        puts("Failed in testing colorAlphaToString"); \
        SubFX_fin(&api); \
        return 1; \
    } \
    puts(x);

int main()
{
    puts("Testing ass.h ...");
    SubFX api;
    if (SubFX_init(&api) == subfx_failed)
    {
        puts("Failed in SubFX_init()");
        return 1;
    }

    subfx_ass_api *ass = &api.ass;

    char errMsg[1024];
    char tmpString[1024];
    errMsg[0] = '\0';

    sprintf(tmpString, "0:00:51.97");
    uint64_t ms;

    if (ass->stringToMs(tmpString, &ms, errMsg) == subfx_failed)
    {
        puts("Failed in testing stringToMs");
        SubFX_fin(&api);
        return 1;
    }

#ifdef _WIN32
    printf("case 1's result is: %lld\n", ms);
#elif __APPLE__
    printf("case 1's result is: %llu\n", ms);
#else
    printf("case 1's result is: %ld\n", ms);
#endif

    uint8_t outBuf[128];

    if (ass->msToString(ms, (char *)outBuf) == subfx_failed)
    {
        puts("Failed in testing msToString");
        SubFX_fin(&api);
        return 1;
    }

    printf("case 2's result is: %s\n", outBuf);

    uint8_t size;
    sprintf(tmpString, "&H22&");
    StringToColorAlpha(outBuf, tmpString);
    sprintf(tmpString, "&Ha9b8c7&");
    StringToColorAlpha(outBuf, tmpString);
    sprintf(tmpString, "&HDA3255E6");
    StringToColorAlpha(outBuf, tmpString);

    uint8_t res[4];

    res[0] = 49;
    ColorAlphaToString((char *)outBuf, res, 1);

    res[0] = 88;
    res[1] = 254;
    res[2] = 215;
    ColorAlphaToString((char *)outBuf, res, 3);

    res[0] = 143;
    res[1] = 35;
    res[2] = 78;
    res[3] = 236;
    ColorAlphaToString((char *)outBuf, res, 4);

    SubFX_fin(&api);
    puts("All done!");
    return 0;
}
