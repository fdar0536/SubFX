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

#include "utils/logger.h"
#include "utils/map.h"
#include "utils/math.h"
#include "utils/misc.h"
#include "utils/pair.h"
#include "utils/ptrvector.h"
#include "utils/tuple.h"
#include "utils/utf8.h"
#include "utils/vector.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_utils
{
    subfx_utils_logger *logger;

    subfx_utils_math *math;

    subfx_utils_misc *misc;

    subfx_utils_utf8 *utf8;
} subfx_utils;

#ifdef __cplusplus
}
#endif
