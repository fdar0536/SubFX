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

#include "../defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_yutils_ass
{
    subfx_exitstate (*stringToMs)(const char *ass_ms,
                                  uint64_t *dst,
                                  char *errMsg);

    char *(*msToString)(uint64_t ms_ass);

    uint8_t *(*stringToColorAlpha)(const char *input, char *errMsg);

    char *(*colorAlphaToString)(uint8_t *input,
                                size_t inputSize, char *errMsg);
} subfx_yutils_ass;

#ifdef __cplusplus
}
#endif
