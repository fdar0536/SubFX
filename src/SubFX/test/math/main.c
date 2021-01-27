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
#include <time.h>

#ifdef _WIN32
#include <inttypes.h>
#endif

#include "SubFX.h"

int main()
{
    SubFX *api = SubFX_init();
    if (!api)
    {
        fputs("Fail to create api entry.\n", stderr);
        return 1;
    }

    subfx_math *math = api->math;

    double *pRet = math->rotate2d(1., 0., 90.);
    if (!pRet)
    {
        fputs("Fail due to \"rotate2d\"", stderr);
        SubFX_destroy(api);
        return 1;
    }

    printf("%lf, %lf\n", pRet[0], pRet[1]);
    free(pRet);
    pRet = NULL;

    double ret = math->rad(180);
    printf("%lf\n", ret);
    printf("%lf\n", math->deg(ret));
#ifdef _WIN32
    srand((uint32_t)time(NULL));
#else
    srand(time(NULL));
#endif
    uint8_t i;
    for (i = 0; i < 5; ++i)
    {
        printf("%lf\n", math->random(-1., 1.));
    }

    SubFX_destroy(api);

    return 0;
}
