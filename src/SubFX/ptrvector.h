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

#include <stddef.h>

#include "include/internal/ptrvector.h"

#ifdef __cplusplus
extern "C"
{
#endif

subfx_ptrVector *subfx_ptrVector_init();

subfx_handle subfx_ptrVector_create(subfx_freeFunc);

subfx_exitstate subfx_ptrVector_destroy(subfx_handle);

void *subfx_ptrVector_at(subfx_handle, size_t);

subfx_exitstate subfx_ptrVector_setValue(subfx_handle, size_t, void *);

subfx_exitstate subfx_ptrVector_clear(subfx_handle);

subfx_exitstate subfx_ptrVector_size(subfx_handle, size_t *);

subfx_exitstate subfx_ptrVector_capacity(subfx_handle, size_t *);

subfx_exitstate subfx_ptrVector_reserve(subfx_handle, size_t);

subfx_exitstate subfx_ptrVector_pushback(subfx_handle, void *);

subfx_exitstate subfx_ptrVector_resize(subfx_handle,
                                       size_t,
                                       void *,
                                       void *(*)(void *));

#ifdef __cplusplus
}
#endif

