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

#include "include/internal/ass.h"

#ifdef __cplusplus
extern "C"
{
#endif

subfx_ass *subfx_ass_init();

subfx_exitstate subfx_ass_stringToMs(const char *ass_ms,
                                     uint64_t *dst,
                                     char *errMsg);

char *subfx_ass_msToString(uint64_t ms_ass);

uint8_t *subfx_ass_stringToColorAlpha(const char *input, char *errMsg);

char *subfx_ass_colorAlphaToString(uint8_t *input, size_t inputSize,
                                   char *errMsg);

#ifdef __cplusplus
}
#endif
