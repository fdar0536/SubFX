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

#include <stdio.h>
#include <stdlib.h>
#include "SubFX.h"
#include "testingcase.h"

SubFX *subfx;
subfx_math *math;
char errMsg[1000];

int testInit()
{
    subfx = SubFX_init();
    if (!subfx)
    {
        puts("Fail to initialize");
        return 1;
    }

    math = subfx->math;
    return 0;
}

int testRotate2d()
{
    puts("Testing rotate2d");
    double *pRet = math->rotate2d(1., 0., 90.);
    if (!pRet)
    {
        fputs("Fail due to \"rotate2d\"", stderr);
        SubFX_destroy(subfx);
        return 1;
    }

    printf("%lf, %lf\n", pRet[0], pRet[1]);
    free(pRet);
    puts("rotate2d is pass");
    return 0;
}

int testRadAndDeg()
{
    puts("Testing rad and deg");
    double ret = math->rad(180);
    printf("%lf\n", ret);
    printf("%lf\n", math->deg(ret));
    puts("rad and deg are pass");
    return 0;
}

int testRandom()
{
    puts("Testing random");
    uint8_t i;
    for (i = 0; i < 5; ++i)
    {
        printf("%lf\n", math->random(-1., 1.));
    }

    puts("random is pass");
    return 0;
}

int testArcCurve()
{
    puts("Testing arc_curve");

    double testData[] = { 0., 720., 60. };
    subfx_handle ret;
    subfx_vector *vec = subfx->vector;
    int i;
    size_t j, size;
    double first, second;
    for (i = 0; i < 3; ++i)
    {
        errMsg[0] = '\0';
        if (i != 2)
        {
            ret = math->arc_curve(0., 1.0,
                                  1.5, 2.0,
                                  testData[i],
                                  errMsg);
            if (ret)
            {
                printf("Failed in arc_curve's case %d.\n", i);
                if (subfx->closeHandle(ret) == subfx_failed)
                {
                    puts("Fail to close handle.");
                }

                SubFX_destroy(subfx);
                return 1;
            }
        }
        else
        {
            ret = math->arc_curve(0., 1.0,
                                  1.5, 2.0,
                                  testData[i],
                                  errMsg);

            if (!ret)
            {
                printf("Failed in arc_curve's case %d.\n", i);
                SubFX_destroy(subfx);
                return 1;
            }

            if (vec->size(ret, &size) == subfx_failed)
            {
                printf("Failed in arc_curve's case %d.\n", i);
                if (subfx->closeHandle(ret) == subfx_failed)
                {
                    puts("Fail to close handle.");
                }

                SubFX_destroy(subfx);
                return 1;
            }

            if (!size)
            {
                printf("Failed in arc_curve's case %d.\n", i);
                if (subfx->closeHandle(ret) == subfx_failed)
                {
                    puts("Fail to close handle.");
                }

                SubFX_destroy(subfx);
                return 1;
            }

            for (j = 0; j < size; j += 2)
            {
                if (vec->at(ret, j, &first) == subfx_failed)
                {
                    printf("Failed in arc_curve's case %d.\n", i);
                    if (subfx->closeHandle(ret) == subfx_failed)
                    {
                        puts("Fail to close handle.");
                    }

                    SubFX_destroy(subfx);
                    return 1;
                }

                if (vec->at(ret, j + 1, &second) == subfx_failed)
                {
                    printf("Failed in arc_curve's case %d.\n", i);
                    if (subfx->closeHandle(ret) == subfx_failed)
                    {
                        puts("Fail to close handle.");
                    }

                    SubFX_destroy(subfx);
                    return 1;
                }

                printf("%lf, %lf\n", first, second);
            } // end for (j = 0; j < size; j += 2)

            if (subfx->closeHandle(ret) == subfx_failed)
            {
                puts("Fail to close handle.");
                SubFX_destroy(subfx);
                return 1;
            }
        } // end if (i != 2)
    } // end for (i = 0; i < 3; ++i)

    puts("arc_curve is pass");
    return 0;
} // end testArcCurve

#define TESTBEZIERINTERNAL(x, y) \
    ret = math->bezier(x, input, y, true, errMsg); \
    if (!ret) \
    { \
        puts("Failed in bezier"); \
        SubFX_destroy(subfx); \
        return 1; \
    } \
    free(ret);

int testBezier()
{
    // total 7 cases
    puts("Testing bezier");
    double *ret;
    double input[20] = {
        0.8,  0.6,  2.,   0.,
        0.99, 0.7,  0.3,  0.,
        0.48, 0.46, 0.79, 0.,
        0.1,  0.89, 0.43, 0.,
        0.32, 0.86, 0.15, 0.
    };
    errMsg[0] = '\0';

    // case 1
    ret = math->bezier(1.2, input, 0, true, errMsg);
    if (ret)
    {
        puts("Failed in bezier");
        free(ret);
        SubFX_destroy(subfx);
        return 1;
    }

    puts(errMsg);
    errMsg[0] = '\0';

    // case 2
    ret = math->bezier(0, input, 0, true, errMsg);
    if (ret)
    {
        puts("Failed in bezier");
        free(ret);
        SubFX_destroy(subfx);
        return 1;
    }

    puts(errMsg);
    errMsg[0] = '\0';

    // case 3 to 6
    TESTBEZIERINTERNAL(0.6,  2)
    TESTBEZIERINTERNAL(0.39, 3)
    TESTBEZIERINTERNAL(0.31, 4)
    TESTBEZIERINTERNAL(0.24, 5)

    puts("bezier is pass");
    return 0;
} // end testBezier

#undef TESTBEZIERINTERNAL

int testDegree()
{
    puts("Testing degree");
    printf("%lf\n", math->degree(5., 6., 7., 8., 9., 10.));
    puts("degree is pass");
    return 0;
} // end testDegree

int testDistance()
{
    puts("Testing distance");
    printf("%lf\n", math->distance(3., 4., 0.));
    puts("distance is pass");
    return 0;
} // end testDistance

int testLineIntersect()
{
    puts("Testing line_intersect");
    double *ret;
    errMsg[0] = '\0';

    // 2 cases
    // case 1
    ret = math->line_intersect(0., 0., 0., 0.,
                               0., 0., 0., 0.,
                               true, errMsg);
    if (ret)
    {
        puts("Failed in line_intersect");
        free(ret);
        SubFX_destroy(subfx);
        return 1;
    }

    puts(errMsg);
    errMsg[0] = '\0';

    // case 2
    ret = math->line_intersect(1., 3., 5., 7.,
                               2., 8., 6., 12.,
                               1, errMsg);
    if (!ret)
    {
        puts("Failed in line_intersect");
        SubFX_destroy(subfx);
        return 1;
    }

    puts("line_intersect is pass");
    free(ret);
    return 0;
} // end testLineIntersect

int testOrtho()
{
    puts("Testing ortho");

    double *ret = math->ortho(5., 10., 15., 2., 4., 6.);
    if (!ret)
    {
        puts("Failed in ortho");
        SubFX_destroy(subfx);
        return 1;
    }

    puts("ortho is pass");
    free(ret);
    return 0;
} // end testOrtho

int testRandomsteps()
{
    puts("Testing randomsteps");
    errMsg[0] = '\0';

    // 2 cases
    // case 1

    (void)math->randomsteps(5., 3., -1., errMsg);
    if (errMsg[0] == '\0')
    {
        puts("Failed in randomsteps");
        SubFX_destroy(subfx);
        return 1;
    }

    puts(errMsg);
    errMsg[0] = '\0';

    // case 2
    printf("%lf\n", math->randomsteps(2., 3., 0.5, NULL));

    puts("randomsteps is pass");
    return 0;
} // end testRandomsteps

int testRound()
{
    puts("Testing Round");

    double ret = math->round(3.86456443435, 5.);
    printf("%lf\n", ret);

    ret = math->round(3.86456443435, 0.5);
    printf("%lf\n", ret);

    puts("Round is pass");
    return 0;
} // end testRound

int testStretch()
{
    puts("Testing stretch");

    double *ret = math->stretch(12., 126., 38., 75.);
    if (!ret)
    {
        puts("Failed in stretch");
        SubFX_destroy(subfx);
        return 1;
    }

    puts("stretch is pass");
    free(ret);
    return 0;
} // end testStretch

int testTrim()
{
    puts("Testing trim");
    errMsg[0] = '\0';

    // 2 cases
    // case 1
    (void)math->trim(3., 5., 2., errMsg);
    if (errMsg[0] == '\0')
    {
        puts("Failed in trim");
        SubFX_destroy(subfx);
        return 1;
    }

    puts(errMsg);
    errMsg[0] = '\0';

    // case 2
    printf("%lf\n", math->trim(3., 0.5, 2., NULL));

    puts("trim is pass");
    return 0;
} // end testTrim

int testEllipse()
{
    puts("Testing ellipse");

    double *ret = math->ellipse(2., 6., 3., 7., 5.);
    if (!ret)
    {
        puts("Failed in ellipse");
        SubFX_destroy(subfx);
        return 1;
    }

    puts("ellipse is pass");
    free(ret);
    return 0;
} // end testEllipse

int testRandomway()
{
    puts("Testing randomway");
    int i;
    for (i = 0; i < 5; ++i)
    {
        printf("%lf\n", math->randomway());
    }

    puts("randomway is pass");
    return 0;
} // end testRandomway

int testRotate()
{
    puts("Testing rotate");
    double input[3] = {3., 4., 5.};
    errMsg[0] = '\0';
    double *ret;

    // 4 cases
    // case 1
    ret = math->rotate(input, "", 2., errMsg);
    if (ret)
    {
        puts("Failed in rotate");
        free(ret);
        SubFX_destroy(subfx);
        return 1;
    }

    puts(errMsg);
    errMsg[0] = '\0';

    // case 2 to 4
    const char strings[3][2] = {"x", "y", "z"};
    int i;
    for (i = 0; i < 3; ++i)
    {
        ret = math->rotate(input, strings[i], 2., NULL);
        if (!ret)
        {
            puts("Failed in rotate");
            SubFX_destroy(subfx);
            return 1;
        }

        free(ret);
    }

    puts("rotate is pass");
    return 0;
} // end testRotate

void testFin()
{
    SubFX_destroy(subfx);
}
