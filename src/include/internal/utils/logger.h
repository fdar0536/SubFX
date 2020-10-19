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

#include "../basecommon.h"

typedef void subfx_utils_logger;

#ifdef __cplusplus
extern "C"
{
#endif

SYMBOL_SHOW
void *subfx_utils_logger_create(FILE *out,
                                FILE *err,
                                int autoCloseFiles);

SYMBOL_SHOW
void *subfx_utils_logger_create2(const char *outFile,
                                 const char *errFile);

SYMBOL_SHOW
void subfx_utils_logger_destory(subfx_utils_logger *);

SYMBOL_SHOW
void subfx_utils_logger_writeOut(subfx_utils_logger *, const char *);

SYMBOL_SHOW
void subfx_utils_logger_writeErr(subfx_utils_logger *, const char *);

#ifdef __cplusplus
}
#endif
