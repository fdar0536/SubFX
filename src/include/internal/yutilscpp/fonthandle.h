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

#include <inttypes.h>
#include <stdbool.h>

#include "../defines.h"

#define subfx_yutils_fonthandle_metrics_height           0
#define subfx_yutils_fonthandle_metrics_ascent           1
#define subfx_yutils_fonthandle_metrics_descent          2
#define subfx_yutils_fonthandle_metrics_internal_leading 3
#define subfx_yutils_fonthandle_metrics_external_leading 4

#define subfx_yutils_fonthandle_text_extents_width  0
#define subfx_yutils_fonthandle_text_extents_height 1

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_yutils_fonthandle
{
    subfx_handle (*create)(const char *family,
                           bool bold,
                           bool italic,
                           bool underline,
                           bool strikeout,
                           int32_t size,
                           double xscale, // 1.
                           double yscale, // 1.
                           double hspace, // 0.
                           char *errMsg);

    double *(*metrics)(subfx_handle fonthandle);

    double *(*text_extents)(subfx_handle fonthandle,
                            const char *text);

    char *(*text_to_shape)(subfx_handle fonthandle,
                           const char *text, char *errMsg);
} subfx_yutils_fonthandle;

#ifdef __cplusplus
}
#endif
