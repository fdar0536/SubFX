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

#include "defines.h"

#define subfx_fonthandle_metrics_height           0
#define subfx_fonthandle_metrics_ascent           1
#define subfx_fonthandle_metrics_descent          2
#define subfx_fonthandle_metrics_internal_leading 3
#define subfx_fonthandle_metrics_external_leading 4

#define subfx_fonthandle_text_extents_width  0
#define subfx_fonthandle_text_extents_height 1

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_fontHandle subfx_fontHandle;

typedef struct subfx_fontHandle_api
{
    /**
     * @brief Create FontHandle object.
     *
     * @param family the font family.
     * @param bold If true, font has bold weight.
     * @param italic If true, font has italic style.
     * @param underline If true, font has underline decoration.
     * @param strikeout If true, font has strikeout decoration.
     * @param size the font size.
     * @param xscale can define horizontal scale.
     * @param yscale can define vertical scale.
     * @param hspace can define intercharacter space.
     * @param errMsg you can pass buffer if you want to get the error message.
     * @return If failed, it will return NULL.
     */
    subfx_fontHandle *(*create)(const char *family,
                                bool bold,
                                bool italic,
                                bool underline,
                                bool strikeout,
                                int32_t size,
                                double xscale, // 1.
                                double yscale, // 1.
                                double hspace, // 0.
                                char *errMsg);

    subfx_exitstate (*destory)(subfx_fontHandle *fontHandle);

    /**
     * Returns font metrics as array with followings fields:
     * subfx_fonthandle_metrics_height: font maximal height
     * subfx_fonthandle_metrics_ascent: font ascent
     * subfx_fonthandle_metrics_descent: font descent
     * subfx_fonthandle_metrics_internal_leading: font internal leading
     * subfx_fonthandle_metrics_external_leading: font external leading
     *
     * @param fonthandle the fonthandle return from create()
     * @return If fail, it will return NULL.
     */
    double *(*metrics)(subfx_fontHandle *fontHandle);

    /**
     * Returns extents of text with given font as array with followings fields:
     * subfx_fonthandle_text_extents_width: text width
     * subfx_fonthandle_text_extents_height: text height
     *
     * @param fonthandle the fonthandle return from create()
     * @param text input text
     * @return If fail, it will return NULL.
     */
    double *(*text_extents)(subfx_fontHandle *fontHandle,
                            const char *text);

    /**
     * Converts text with given font to an ASS shape.
     *
     * @param fonthandle the fonthandle return from create()
     * @param text input text
     * @param errMsg you can pass buffer if you want to get the error message.
     * @return If fail, it will return NULL.
     */
    char *(*text_to_shape)(subfx_fontHandle *fontHandle,
                           const char *text, char *errMsg);
} subfx_fontHandle_api;

#ifdef __cplusplus
}
#endif
