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
#include <stdbool.h>

#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_logger subfx_logger;

typedef struct subfx_logger_api
{
    subfx_logger *(*create)(FILE *out,
                            FILE *err,
                            bool autoCloseFiles);

    subfx_logger *(*create2)(const char *outFile,
                             const char *errFile);

    subfx_exitstate (*destory)(subfx_logger *logger);

    subfx_exitstate (*writeOut)(subfx_logger *logger, const char *msg);

    subfx_exitstate (*writeErr)(subfx_logger *logger, const char *msg);

} subfx_logger_api;

#ifdef __cplusplus
}
#endif
