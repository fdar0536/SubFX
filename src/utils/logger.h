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

#include "include/internal/defines.h"
#include "include/internal/utils/logger.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Logger
{
    subfx_types id;

    FILE *out;

    FILE *err;

    subfx_bool haveToCloseFiles;
} Logger;

subfx_utils_logger
*subfx_utils_logger_init();

subfx_handle
subfx_utils_logger_create(FILE *,
                          FILE *,
                          subfx_bool);

subfx_handle
subfx_utils_logger_create2(const char *,
                           const char *);

subfx_exitstate subfx_utils_logger_destroy(subfx_handle);

subfx_exitstate subfx_utils_logger_writeOut(subfx_handle, const char *);

subfx_exitstate subfx_utils_logger_writeErr(subfx_handle, const char *);

subfx_handle subfx_utils_logger_createInternal(FILE *,
                                               FILE *,
                                               subfx_bool,
                                               subfx_bool);

void subfx_utils_logger_closeFiles(FILE *, FILE *);

#ifdef __cplusplus
}
#endif
