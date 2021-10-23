/*
 * This file is part of SubFX,
 * Copyright (c) 2020 fdar0536
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

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "SubFX.h"

int main()
{
    SubFX api;
    if (SubFX_init(&api) == subfx_failed)
    {
        fputs("Fail to create api entry.\n", stderr);
        return 1;
    }

    subfx_misc_api *misc = &api.misc;
    char *string = misc->doubleToString(sqrt(2.));
    if (!string)
    {
        fputs("Fail due to \"doubleToString\"", stderr);
        return 1;
    }

    puts(string);

    char errMsg[1000];
    errMsg[0] = '\0';

    FILE *ass = fopen("in.ass", "r");
    if (!ass)
    {
        fputs("Fail to open file", stderr);
        free(string);
        return 1;
    }

    size_t strSize = 500 * sizeof(double);
    subfx_exitstate state = 0;
    uint8_t flag = 1;
    while (flag)
    {
        state = misc->getLine(string, strSize, ass, errMsg);
        switch (state)
        {
        case subfx_success:
        case subfx_successWithWarning:
        {
            puts(string);
            break;
        }
        case subfx_eof:
        {
            flag = 0;
            break;
        }
        case subfx_failed:
        {
            fputs(errMsg, stderr);
            fclose(ass);
            free(string);
            return 1;
        }
        default:
        {
            fputs("You should NEVER see this line", stderr);
            fclose(ass);
            free(string);
            return 1;
        }
        }

        string[0] = '\0';
    }

    // clean up
    fclose(ass);
    free(string);

    return 0;
}
