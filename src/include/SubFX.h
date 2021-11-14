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

// #include "internal/assparser.h"
#include "internal/defines.h"
#include "internal/logger.h"
#include "internal/misc.h"
#include "internal/smath.h"
#include "internal/utf8.h"

#include "internal/ass.h"
#include "internal/fonthandle.h"

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

/**
 * @struct SubFX
 * This is main object for SubFX.
 * All APIs are included in it.
 */
typedef struct SubFX
{
    fDSA *fdsa;

    subfx_logger_api logger;

    subfx_misc_api misc;

    subfx_math_api math;

    subfx_utf8_api utf8;

    subfx_ass_api ass;

    subfx_fontHandle_api fontHandle;

    /**
     * @return it returns SubFX API's version.
     */
    const char *(*version)();
} SubFX;

/**
 * Initialize SubFX APIs.
 */
SUBFX_API subfx_exitstate SubFX_init(SubFX *);

SUBFX_API void SubFX_fin(SubFX *in);

#ifdef __cplusplus
}
#endif
