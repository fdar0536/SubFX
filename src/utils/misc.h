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

#include <stdio.h>

#include "include/internal/utils/misc.h"
#include "include/internal/defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

subfx_utils_misc *subfx_utils_misc_init();

char *subfx_utils_misc_doubleToString(double input);

subfx_exitstate
subfx_utils_misc_getLine(char *buffer,
                         int bufferSize,
                         FILE *file,
                         char *errMsg);

#ifdef __cplusplus
}
#endif