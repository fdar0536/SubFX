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

int main()
{
    SubFX subfx;
    if (SubFX_init(&subfx) == subfx_failed)
    {
        puts("Fail in initializing.");
        return 1;
    }

    subfx_fontHandle_api *fontHandle = &subfx.fontHandle;

    char tmpString[100];
    sprintf(tmpString, "Source Code Pro");
    char errMsg[1000];
    errMsg[0] = '\0';
    subfx_fontHandle *handle = fontHandle->create(
            tmpString, // family
            0, // bold
            0, // italic
            0, // underline
            0, // strikeout
           60, // size
           1., // xscale
           1., // yscale
           0., // hspace
           errMsg);

    if (!handle)
    {
        puts("Fail in initializing.");
        return 1;
    }

    int ret = 0;

    double *retDouble = fontHandle->metrics(handle);
    if (!retDouble)
    {
        puts("Fail to get font metrics");
        ret = 1;
        goto error;
    }

    printf("font's height: %lf\n",
           retDouble[subfx_fonthandle_metrics_height]);

    printf("font's ascent: %lf\n",
           retDouble[subfx_fonthandle_metrics_ascent]);

    printf("font's descent: %lf\n",
           retDouble[subfx_fonthandle_metrics_descent]);

    printf("font's internal leading: %lf\n",
           retDouble[subfx_fonthandle_metrics_internal_leading]);

    printf("font's external leading: %lf\n",
           retDouble[subfx_fonthandle_metrics_external_leading]);

    free(retDouble);

    sprintf(tmpString, "testing");
    retDouble = fontHandle->text_extents(handle, tmpString);
    if (!retDouble)
    {
        puts("Fail to get text extents");
        ret = 1;
        goto error;
    }

    printf("text's width is: %lf\n",
           retDouble[subfx_fonthandle_text_extents_width]);

    printf("text's height is: %lf\n",
           retDouble[subfx_fonthandle_text_extents_height]);

    free(retDouble);

    char *retChar = fontHandle->text_to_shape(handle, tmpString, errMsg);
    if (!retChar)
    {
        puts("Fail to get text's shape");
        ret = 1;
        goto error;
    }

    printf("text's shape is: %s\n", retChar);
    free(retChar);

error:
    if (fontHandle->destory(handle) == subfx_failed)
    {
        puts("Fail to close handle");
        ret = 1;
    }

    return ret;
}
