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

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)
#pragma warning(disable: 4127)
#endif // _MSC_VER

#pragma once

#include "config.h"

#ifdef _WIN32
#define NOMINMAX
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef __MINGW32__
#define SYMBOL_SHOW __attribute__((dllexport))
#else
#define SYMBOL_SHOW __declspec(dllexport)
#endif // __MINGW32__
#else
#define SYMBOL_SHOW __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
#define THROW noexcept(false)
#define NOTHROW noexcept
#define UNUSED(x) static_cast<void>(x)
#else
#define THROW
#define NOTHROW
#define UNUSED(x) (void)x
#endif

typedef enum subfx_exitstate
{
    subfx_success,
    subfx_successWithWarning,
    subfx_failed
} subfx_exitstate;

#define subfx_pError(buf, msg) \
    if (buf) \
    { \
        sprintf(buf, "%s", msg); \
    }

typedef enum subfx_types
{
    subfx_int8_t,
    subfx_uint8_t,
    subfx_int16_t,
    subfx_uint16_t,
    subfx_int32_t,
    subfx_uint32_t,
    subfx_int64_t,
    suffx_uint64_t,
    subfx_float,
    subfx_double,
    subfx_other
} subfx_types;

typedef int (*subfx_cmpFunc)(const void *, const void *);
typedef void (*subfx_freeFunc)(void *);
