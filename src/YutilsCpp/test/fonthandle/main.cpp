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

#include "YutilsCpp"

int main()
{
    std::string tmpString("Source Code Pro");
    auto handle(PROJ_NAMESPACE::Yutils::FontHandle::create(
        tmpString, // family
                0, // bold
                0, // italic
                0, // underline
                0, // strikeout
               60, // size
               1., // xscale
               1., // yscale
               0. // hspace
               ));

    if (handle == nullptr)
    {
        puts("Fail in initializing.");
        return 1;
    }

    auto map = handle->metrics();
    if (!map.size())
    {
        puts("Fail to get font metrics");
        return 1;
    }

    printf("font's height: %lf\n", map["height"]);
    printf("font's ascent: %lf\n", map["ascent"]);
    printf("font's descent: %lf\n", map["descent"]);
    printf("font's internal leading: %lf\n",
           map["internal_leading"]);
    printf("font's external leading: %lf\n",
           map["external_leading"]);

    tmpString = "testing";
    map = handle->text_extents(tmpString);
    if (!map.size())
    {
        puts("Fail to get text extents");
        return 1;
    }

    printf("text's width is: %lf\n", map["width"]);
    printf("text's height is: %lf\n", map["height"]);

    tmpString = handle->text_to_shape(tmpString);
    if (!tmpString.length())
    {
        puts("Fail to get text's shape");
        return 1;
    }

    printf("text's shape is: %s\n", tmpString.c_str());
    return 0;
}
