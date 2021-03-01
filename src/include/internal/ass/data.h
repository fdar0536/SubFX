/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2021 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../defines.h"

// reference: https://github.com/weizhenye/ASS/wiki/ASS-%E5%AD%97%E5%B9%95%E6%A0%BC%E5%BC%8F%E8%A7%84%E8%8C%83

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_ass_meta
{
    uint8_t wrap_style;
    bool scaled_border_and_shadow;
    uint16_t play_res_x;
    uint16_t play_res_y;
    char *colorMatrix;
} subfx_ass_meta;

typedef struct subfx_ass_style
{
    char *fontname;
    int fontsize;
    int8_t bold;
    int8_t italic;
    int8_t underline;
    int8_t strikeout;
    double scale_x;
    double scale_y;
    double spaceing;
    double angle;
    uint8_t bolder_style;
    double outline;
    double shadow;
    uint8_t alignment;
    double margin_l;
    double margin_r;
    double margin_v;
    int encoding;
    char *color1;
    char *alpha1;
    char *color2;
    char *alpha2;
    char *color3;
    char *alpha3;
    char *color4;
    char *alpha4;
} subfx_ass_style;

#define subfx_ass_symbol \
    uint64_t start_time; \
    uint64_t end_time; \
    char *text; \
    uint32_t i; \
    uint64_t duration; \
    uint64_t mid_time; \
    double width; \
    double height; \
    double ascent; \
    double descent; \
    double internal_leading; \
    double external_leading; \
    double left; \
    double center; \
    double right; \
    double x; \
    double top; \
    double middle; \
    double bottom; \
    double y;

#ifdef __cplusplus
}
#endif

/*
namespace PROJ_NAMESPACE
{

namespace Yutils
{

class AssSymbol
{
public:
    AssSymbol() :
    start_time(0),
    end_time(0),
    text(""),
    i(0),
    duration(0),
    mid_time(0),
    width(0.),
    height(0.),
    ascent(0.),
    descent(0.),
    internal_leading(0.),
    external_leading(0.),
    left(0.),
    center(0.),
    right(0.),
    x(0.),
    top(0.),
    middle(0.),
    bottom(0.),
    y(0.)
    {}
};

class AssTextChunked
{
public:
    AssTextChunked() :
    tags(""),
    text("")
    {}

    std::string tags;
    std::string text;
};

class AssSyl : public AssSymbol
{
public:
    AssSyl() :
    AssSymbol(),
    tags(""),
    prespace(0),
    postspace(0)
    {}

    std::string tags;
    uint32_t prespace;
    uint32_t postspace;
};

class AssWord : public AssSymbol
{
public:
    AssWord() :
    AssSymbol(),
    prespace(0),
    postspace(0)
    {}

    uint32_t prespace;
    uint32_t postspace;
};

class AssChar : public AssSymbol
{
public:
    AssChar() :
    AssSymbol(),
    syl_i(0),
    word_i(0)
    {}

    int syl_i;
    int word_i;
};

class AssDialog : public AssSymbol
{
public:
    AssDialog() :
    AssSymbol(),
    styleref(std::make_shared<AssStyle>()),
    text_stripped(""),
    comment(false),
    layer(0),
    style(""),
    actor(""),
    margin_l(0.),
    margin_r(0.),
    margin_v(0.),
    effect(""),
    leadin(0.),
    leadout(0.),
    textChunked(std::vector<std::shared_ptr<AssTextChunked>>()),
    syls(std::vector<std::shared_ptr<AssSyl>>()),
    words(std::vector<std::shared_ptr<AssWord>>()),
    chars(std::vector<std::shared_ptr<AssChar>>())
    {}

    std::shared_ptr<AssStyle> styleref;
    std::string text_stripped;
    bool comment;
    uint32_t layer;
    std::string style;
    std::string actor;
    double margin_l;
    double margin_r;
    double margin_v;
    std::string effect;
    double leadin;
    double leadout;
    std::vector<std::shared_ptr<AssTextChunked>> textChunked;
    std::vector<std::shared_ptr<AssSyl>> syls;
    std::vector<std::shared_ptr<AssWord>> words;
    std::vector<std::shared_ptr<AssChar>> chars;
};
} // end namespace Yutils

} // end PROJ_NAMESPACE
*/
