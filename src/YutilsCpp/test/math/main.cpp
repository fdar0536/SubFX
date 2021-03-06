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

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "testingcase.hpp"

#define TESTING(x) \
    if (x) \
    { \
        return 1; \
    }

int main()
{
    puts("Testing math.hpp");
#ifdef _WIN32
    srand(static_cast<unsigned int>(time(nullptr)));
#else
    srand(time(nullptr));
#endif // _WIN32

    TESTING(testArcCurve())
    TESTING(testBezier())
    TESTING(testDegree())
    TESTING(testDistance())
    TESTING(testLineIntersect())
    TESTING(testOrtho())
    TESTING(testRandomsteps())
    TESTING(testRound())
    TESTING(testStretch())
    TESTING(testTrim())
    TESTING(testEllipse())
    TESTING(testRandomway())
    TESTING(testRotate())

    std::cout << "All done!" << std::endl;
    return 0;
}
