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

#include <cstdio>

#include "config.h"
#include "YutilsCpp"

#define StringToColorAlpha(x, y) \
    x = ass::stringToColorAlpha(y); \
    printf("r: %d, g: %d, b: %d, a: %d\n", \
           std::get<0>(x), std::get<1>(x), \
           std::get<2>(x), std::get<3>(x));

#define ColorAlphaToString(x, y) \
    x = ass::colorAlphaToString(y); \
    puts(x.c_str()); \
    y.clear();

namespace ass = PROJ_NAMESPACE::Yutils::Ass;

int main()
{
    puts("Testing ass.hpp ...");
    std::string tmpString("0:00:51.97");
    uint64_t ms;
    try
    {
        ms = ass::stringToMs(tmpString);
    }
    catch (std::invalid_argument &)
    {
        puts("Fail in testing stringToMs");
        return 1;
    }

#ifdef _WIN32
    printf("case 1's result is: %lld\n", ms);
#else
    printf("case 1's result is: %ld\n", ms);
#endif

    tmpString = ass::msToString(ms);
    printf("case 2's result is: %s\n", tmpString.c_str());

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> res;
    tmpString = "&H22&";
    StringToColorAlpha(res, tmpString)
    tmpString = "&Ha9b8c7&";
    StringToColorAlpha(res, tmpString)
    tmpString = "&HDA3255E6";
    StringToColorAlpha(res, tmpString)

    std::vector<uint8_t> input;
    input.reserve(4);
    input.push_back(49);
    ColorAlphaToString(tmpString, input)

    input.push_back(88);
    input.push_back(254);
    input.push_back(215);
    ColorAlphaToString(tmpString, input)

    input.push_back(143);
    input.push_back(35);
    input.push_back(78);
    input.push_back(236);
    ColorAlphaToString(tmpString, input)

    puts("All done!");
    return 0;
}
