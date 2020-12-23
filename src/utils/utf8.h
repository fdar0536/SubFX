/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
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

#include "include/internal/utils/utf8.h"

#ifdef __cplusplus
extern "C"
{
#endif

subfx_utils_utf8 *subfx_utils_utf8_init();

uint32_t subfx_utils_utf8_stringLen(const char *);

// https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters
subfx_handle subfx_utils_utf8_stringSplit(const char *, char *);

#ifdef __cplusplus
}
#endif
