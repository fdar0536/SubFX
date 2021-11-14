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

#include "fdsa/fdsa.h"

// reference: https://github.com/weizhenye/ASS/wiki/ASS-%E5%AD%97%E5%B9%95%E6%A0%BC%E5%BC%8F%E8%A7%84%E8%8C%83

#ifdef __cplusplus
extern "C"
{
#endif

#define SUBFX_FONT_NAME_LEN 32
#define SUBFX_COLOR_ALPHA_LEN 16
#define SUBFX_ASS_TEXT_LEN (1 << 16) // maybe too large? maybe too small? or dynamic allocate?

typedef struct subfx_ass_meta
{
    uint8_t wrap_style;
    bool scaled_border_and_shadow;
    uint16_t play_res_x;
    uint16_t play_res_y;
    char colorMatrix[32];
} subfx_ass_meta;

typedef struct subfx_ass_style
{
    char fontname[SUBFX_FONT_NAME_LEN];
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
    char color1[SUBFX_COLOR_ALPHA_LEN];
    char alpha1[SUBFX_COLOR_ALPHA_LEN];
    char color2[SUBFX_COLOR_ALPHA_LEN];
    char alpha2[SUBFX_COLOR_ALPHA_LEN];
    char color3[SUBFX_COLOR_ALPHA_LEN];
    char alpha3[SUBFX_COLOR_ALPHA_LEN];
    char color4[SUBFX_COLOR_ALPHA_LEN];
    char alpha4[SUBFX_COLOR_ALPHA_LEN];
} subfx_ass_style;

#define subfx_ass_symbol \
    uint64_t start_time; \
    uint64_t end_time; \
    char text[SUBFX_ASS_TEXT_LEN]; \
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

// maybe too large? maybe too small? or dynamic allocate?
#define SUBFX_ASS_CHUNKED_TEXT_SIZE (1 << 8)

typedef struct subfx_ass_chunked
{
    char tags[SUBFX_ASS_CHUNKED_TEXT_SIZE];
    char text[SUBFX_ASS_CHUNKED_TEXT_SIZE];
} subfx_ass_chunked;

typedef struct subfx_ass_syl
{
    subfx_ass_symbol
    char tags[SUBFX_ASS_CHUNKED_TEXT_SIZE];
    uint32_t prespace;
    uint32_t postspace;
} subfx_ass_syl;

typedef struct subfx_ass_word
{
    subfx_ass_symbol
    uint32_t prespace;
    uint32_t postspace;
} subfx_ass_word;

typedef struct subfx_ass_char
{
    subfx_ass_symbol
    int syl_i;
    int word_i;
} subfx_ass_char;

typedef struct subfx_ass_dialog
{
    subfx_ass_symbol
    subfx_ass_style *styleref;
    char text_stripped[SUBFX_ASS_CHUNKED_TEXT_SIZE];
    bool comment;
    uint32_t layer;
    char style[32];
    char actor[32];
    double margin_l;
    double margin_r;
    double margin_v;
    char effect[32];
    double leadin;
    double leadout;
    fdsa_ptrVector *textChunked;
    fdsa_ptrVector *syls;
    fdsa_ptrVector *words;
    fdsa_ptrVector *chars;
} subfx_ass_dialog;

#ifdef __cplusplus
}
#endif
