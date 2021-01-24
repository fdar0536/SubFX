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

#include "internal/defines.h"
#include "internal/utils.h"
// #include "internal/yutils.h"

#if defined _WIN32 || defined __CYGWIN__
#ifdef __MINGW32__
#define SUBFX_API __attribute__((dllexport))
#else
#define SUBFX_API __declspec(dllexport)
#endif // __MINGW32__
#else
#define SUBFX_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct SubFX
{
    subfx_map *map;

    subfx_ptrVector *ptrVector;

    subfx_utils *utils;

    subfx_vector *vector;

    subfx_exitstate (*getHandleType)(subfx_handle handle, subfx_types *dst);

    subfx_exitstate (*closeHandle)(subfx_handle handle);

    const char *(*version)();
} SubFX;

SUBFX_API SubFX *SubFX_init();

SUBFX_API void SubFX_destroy(SubFX *);

#ifdef __cplusplus
}
#endif
