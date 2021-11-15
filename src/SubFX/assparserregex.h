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

#define REGEX_COUNT 8

#define REGEX_ASS_SECION_MARK 0
#define REGEX_STR_ASS_SECION_MARK \
    "^\\[.*\\]$"

// for "Script Info"
#define REGEX_WRAP_STYLE 1
#define REGEX_STR_WRAP_STYLE \
    "^WrapStyle: \\d$"

#define REGEX_SCALED_BORDER_AND_SHADOW 2
#define REGEX_STR_SCALED_BORDER_AND_SHADOW \
    "^ScaledBorderAndShadow:" \
    " ([Yy]es|[Nn]o)$"

#define REGEX_PLAY_RES_X 3
#define REGEX_STR_PLAY_RES_X \
    "^PlayResX: \\d+$"

#define REGEX_PLAY_RES_Y 4
#define REGEX_STR_PLAY_RES_Y \
    "^PlayResY: \\d+$"

#define REGEX_YCBCR_MATRIX 5
#define REGEX_STR_YCBCR_MATRIX \
    "^YCbCr Matrix: (.*)"

#define REGEX_V4_STYLES 6
#define REGEX_STR_V4_STYLES \
    "^Style: (.*),(.*),(\\d+),(&H[0-9A-F]{8}),(&H[0-9A-F]{8})," \
    "(&H[0-9A-F]{8}),(&H[0-9A-F]{8}),(-?[01]),(-?[01]),(-?[01])," \
    "(-?[01]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(-?\\d+\\.?\\d*)," \
    "(-?\\d+\\.?\\d*),([13]),(\\d+\\.?\\d*),(\\d+\\.?\\d*),([1-9])," \
    "(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+\\.?\\d*),(\\d+)$"

#define REGEX_EVENTS 7
#define REGEX_STR_EVENTS \
    "^(.+): (\\d+)," \
    "(\\d:\\d\\d:\\d\\d\\.\\d\\d)," \
    "(\\d:\\d\\d:\\d\\d\\.\\d\\d)," \
    "(.+),(.*),(\\d+\\.?\\d*)," \
    "(\\d+\\.?\\d*),(\\d+\\.?\\d*)," \
    "(|.*),(.*)$"


