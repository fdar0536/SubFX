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

#include "include/internal/utils/vector.h"

#ifdef __cplusplus
extern "C"
{
#endif

subfx_vector *subfx_vector_init();

subfx_handle subfx_vector_create(size_t sizeOfData);

subfx_exitstate subfx_vector_destroy(subfx_handle vector);

subfx_exitstate subfx_vector_at(subfx_handle vector, size_t index, void *dst);

subfx_exitstate subfx_vector_setValue(subfx_handle vector, size_t, const void *);

subfx_exitstate subfx_vector_clear(subfx_handle vector);

subfx_exitstate subfx_vector_size(subfx_handle vector, size_t *);

subfx_exitstate subfx_vector_capacity(subfx_handle vector, size_t *);

subfx_exitstate subfx_vector_reserve(subfx_handle vector, size_t);

subfx_exitstate subfx_vector_pushback(subfx_handle vector, const void *);

subfx_exitstate subfx_vector_resize(subfx_handle vector, size_t, const void *);

#ifdef __cplusplus
}
#endif
