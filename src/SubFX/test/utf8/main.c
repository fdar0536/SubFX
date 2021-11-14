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

#include <stdlib.h>
#include <stdio.h>

#include "SubFX.h"

int main()
{
    SubFX api;
    if (SubFX_init(&api))
    {
        fputs("Fail to create api entry.\n", stderr);
        return 1;
    }

    FILE *text = fopen("test.txt", "r");
    if (!text)
    {
        fputs("Fail to open file", stderr);
        SubFX_fin(&api);
        return 1;
    }

    subfx_utf8_api *utf8 = &api.utf8;
    subfx_misc_api *misc = &api.misc;
    fdsa_ptrVector_api *vec = &api.fdsa->ptrVector;
    subfx_exitstate state;
    fdsa_exitstate fstate;
    uint8_t flag = 1;

    char buf[1000];
    buf[0] = '\0';

    char errMsg[1000];
    errMsg[0] = '\0';

    fdsa_ptrVector *handle = NULL;
    size_t i, size;
    char *string = NULL;

    while (flag)
    {
        state = misc->getLine(buf, 1000, text, errMsg);

        switch (state)
        {
        case subfx_success:
        case subfx_successWithWarning:
        {
            printf("%d\n", utf8->stringLen(buf));
            handle = utf8->stringSplit(buf, errMsg);
            if (!handle)
            {
                fputs(errMsg, stderr);
                fclose(text);
                SubFX_fin(&api);
                return 1;
            }

            fstate = vec->size(handle, &size);
            if (fstate == fdsa_failed)
            {
                fputs("Fail to get size", stderr);
                if (vec->destory(handle) == fdsa_failed)
                {
                    fputs("Fail to close handle", stderr);
                }

                fclose(text);
                SubFX_fin(&api);
                return 1;
            }

            for (i = 0; i < size; ++i)
            {
                string = vec->at(handle, i);
                if (!string)
                {
                    fputs("Fail due to access vector", stderr);
                    if (vec->destory(handle) == fdsa_failed)
                    {
                        fputs("Fail to close handle", stderr);
                    }

                    fclose(text);
                    SubFX_fin(&api);
                    return 1;
                }

                puts(string);
            }

            if (vec->destory(handle) == fdsa_failed)
            {
                fputs("Fail to close handle", stderr);
                fclose(text);
                SubFX_fin(&api);
                return 1;
            }

            puts("");
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
            fclose(text);
            SubFX_fin(&api);
            return 1;
        }
        default:
        {
            fputs("You should NEVER see this line", stderr);
            fclose(text);
            SubFX_fin(&api);
            return 1;
        }
        }

        buf[0] = '\0';
    }

    fclose(text);
    SubFX_fin(&api);

    return 0;
}
