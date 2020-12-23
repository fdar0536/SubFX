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
#include <string.h>

#include "SubFX.h"

int testingLogger(subfx_utils_logger *api, subfx_handle logger)
{
    if (api->writeOut(logger, "Message to stdout.\n") == subfx_failed)
    {
        fputs("Fail to write message to stdout.", stderr);
        return 1;
    }

    if (api->writeErr(logger, "Message to stderr.\n") == subfx_failed)
    {
        fputs("Fail to write message to stderr.", stdout);
        return 1;
    }

    return 0;
}

int main()
{
    SubFX *api = SubFX_init();
    if (!api)
    {
        fputs("Fail to create api entry.\n", stderr);
        return 1;
    }

    subfx_utils_logger *loggerApi = api->utils->logger;

    subfx_handle logger = loggerApi->create(stdout, stderr, subfx_false);
    if (!logger)
    {
        fputs("Fail to create logger via create.", stderr);
        return 1;
    }

    if (testingLogger(loggerApi, logger))
    {
        return 1;
    }

    if (api->closeHandle(logger) == subfx_failed)
    {
        fputs("Fail to close handle.", stderr);
        SubFX_destroy(api);
        return 1;
    }

    char tmpPath[100];
    memset(tmpPath, 0, 100);
#ifdef _WIN32
    memcpy(tmpPath, getenv("TEMP"), strlen(getenv("TEMP")));
    strcat(tmpPath, "\\testingFile");
#else
    memcpy(tmpPath, "/tmp/testingFile", 17);
#endif

    logger = loggerApi->create2(tmpPath, tmpPath);
    if (!logger)
    {
        fputs("Fail to create logger via create2.", stderr);
        return 1;
    }

    if (testingLogger(loggerApi, logger))
    {
        return 1;
    }

    if (api->closeHandle(logger) == subfx_failed)
    {
        fputs("Fail to close handle.", stderr);
        SubFX_destroy(api);
        return 1;
    }

    SubFX_destroy(api);

    return 0;
}
