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
