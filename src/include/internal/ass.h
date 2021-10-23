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
#include <stddef.h>

#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @struct subfx_ass
 * namespace "ass"
 */
typedef struct subfx_ass_api
{
    /**
     * Converts time to numeric.
     * @param ass_ms a string in ASS format H:MM:SS.XX
     *        (H=Hours, M=Minutes, S=Seconds, X=Milliseconds*10).
     * @param dst for store result
     * @param errMsg you can pass buffer if you want to get the error message.
     * @return please refer to subfx_exitstate
     */
    subfx_exitstate (*stringToMs)(const char *ass_ms,
                                  uint64_t *dst,
                                  char *errMsg);

    /**
     * It's the reverse function of stringToMs.
     * @return If fail, it will return NULL.
     */
    char *(*msToString)(uint64_t ms_ass);

    /**
     * [r, g, b, a] = stringToColorAlpha(input, errMsg);
     *
     * Converts color, alpha or color+alpha to numeric.
     * @param input a string as ASS color (&HBBGGRR&),
     *        alpha (&HAA&) or both (&HAABBGGRR).
     * @param errMsg you can pass buffer if you want to get the error message.
     * @return If fail, it will return NULL.
     */
    uint8_t *(*stringToColorAlpha)(const char *input, char *errMsg);

    /**
     * Converts color or alpha to ASS presentation.
     * @param input [a] or [r, g, b] or [r, g, b, a]
     * @param inputSize how many elements are there in input array
     * @param errMsg you can pass buffer if you want to get the error message.
     * @return If fail, it will return NULL.
     */
    char *(*colorAlphaToString)(uint8_t *input,
                                size_t inputSize, char *errMsg);
} subfx_ass_api;

#ifdef __cplusplus
}
#endif
