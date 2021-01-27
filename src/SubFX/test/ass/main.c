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
    x = ass->stringToColorAlpha(y, errMsg); \
    if (!x) \
    { \
        puts("Failed in testing stringToColorAlpha"); \
        SubFX_destroy(api); \
        return 1; \
    } \
    printf("r: %d, g: %d, b: %d, a: %d\n", \
           x[0], x[1], x[2], x[3]); \
    free(x);

#define ColorAlphaToString(x, y, z) \
    x = ass->colorAlphaToString(y, z, errMsg); \
    if (!x) \
    { \
        puts("Failed in testing colorAlphaToString"); \
        SubFX_destroy(api); \
        return 1; \
    } \
    puts(x); \
    free(x);

int main()
{
    puts("Testing ass.h ...");
    SubFX *api = SubFX_init();
    if (!api)
    {
        puts("Failed in SubFX_init()");
        return 1;
    }

    subfx_ass *ass = api->ass;

    char errMsg[1024];
    char tmpString[1024];
    errMsg[0] = '\0';

    sprintf(tmpString, "0:00:51.97");
    uint64_t ms;

    if (ass->stringToMs(tmpString, &ms, errMsg) == subfx_failed)
    {
        puts("Failed in testing stringToMs");
        SubFX_destroy(api);
        return 1;
    }

#ifdef _WIN32
    printf("case 1's result is: %lld\n", ms);
#elif __APPLE__
    printf("case 1's result is: %llu\n", ms);
#else
    printf("case 1's result is: %ld\n", ms);
#endif

    char *resString = ass->msToString(ms);
    if (!resString)
    {
        puts("Failed in testing msToString");
        SubFX_destroy(api);
        return 1;
    }

    printf("case 2's result is: %s\n", resString);
    free(resString);

    uint8_t *res;
    sprintf(tmpString, "&H22&");
    StringToColorAlpha(res, tmpString);
    sprintf(tmpString, "&Ha9b8c7&");
    StringToColorAlpha(res, tmpString);
    sprintf(tmpString, "&HDA3255E6");
    StringToColorAlpha(res, tmpString);

    res = calloc(4, sizeof(uint8_t));
    if (!res)
    {
        puts("Fail to allocate memory");
        SubFX_destroy(api);
        return 1;
    }

    res[0] = 49;
    ColorAlphaToString(resString, res, 1);

    res[0] = 88;
    res[1] = 254;
    res[2] = 215;
    ColorAlphaToString(resString, res, 3);

    res[0] = 143;
    res[1] = 35;
    res[2] = 78;
    res[3] = 236;
    ColorAlphaToString(resString, res, 4);

    puts("All done!");
    free(res);
    SubFX_destroy(api);
    return 0;
}
