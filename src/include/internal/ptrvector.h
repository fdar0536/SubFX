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

#include "inttypes.h"

#include "defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_ptrVector
{
    subfx_handle (*create)(subfx_freeFunc freeFunc);

    void *(*at)(subfx_handle ptrVector, size_t index);

    subfx_exitstate (*setValue)(subfx_handle ptrVector,
                                size_t index,
                                void *src);

    subfx_exitstate (*clear)(subfx_handle ptrVector);

    subfx_exitstate (*size)(subfx_handle ptrVector, size_t *dst);

    subfx_exitstate (*capacity)(subfx_handle ptrVector, size_t *dst);

    subfx_exitstate (*reserve)(subfx_handle ptrVector, size_t newSize);

    subfx_exitstate (*pushback)(subfx_handle ptrVector, void *src);

    subfx_exitstate (*resize)(subfx_handle vector,
                              size_t newSize,
                              void *src,
                              void *(*deepCopyFunc)(void *));
} subfx_ptrVector;

#ifdef __cplusplus
}
#endif

