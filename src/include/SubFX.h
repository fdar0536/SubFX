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

    subfx_logger *logger;

    subfx_misc *misc;

    subfx_math *math;

    subfx_utf8 *utf8;

    subfx_ass *ass;

    subfx_fonthandle *fonthandle;

    /**
     * Get object's type id.
     * @param handle the object pointer return from SubFX APIs
     * @param dst the pointer for store result
     * @return please refer to subfx_exitstate
     */
    subfx_exitstate (*getHandleType)(subfx_handle handle, subfx_types *dst);

    /**
     * free the subfx_handle
     * @param handle the object pointer return from SubFX APIs
     * @return please refer to subfx_exitstate
     */
    subfx_exitstate (*closeHandle)(subfx_handle handle);

    /**
     * @return it returns SubFX API's version.
     */
    const char *(*version)();
} SubFX;

/**
 * Initialize SubFX APIs. If fail, it will return NULL.
 */
SUBFX_API SubFX *SubFX_init();

/**
 * Destroy SubFX APIs' entries.
 */
SUBFX_API void SubFX_destroy(SubFX *);

#ifdef __cplusplus
}
#endif
