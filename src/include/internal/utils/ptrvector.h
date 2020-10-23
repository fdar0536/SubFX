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
#include <inttypes.h>

#include "../basecommon.h"

typedef void subfx_ptrVector;

#ifdef __cplusplus
extern "C"
{
#endif

SYMBOL_SHOW
subfx_ptrVector *subfx_ptrVector_create(subfx_freeFunc dataFreeFunc);

SYMBOL_SHOW
void subfx_ptrVector_free(subfx_ptrVector *);

SYMBOL_SHOW
size_t subfx_ptrVector_size(subfx_ptrVector *, char *errMsg);

SYMBOL_SHOW
size_t subfx_ptrVector_capacity(subfx_ptrVector *, char *errMsg);

SYMBOL_SHOW
subfx_exitstate subfx_ptrVector_reserve(subfx_ptrVector *, size_t);

SYMBOL_SHOW
subfx_exitstate subfx_ptrVector_pushback(subfx_ptrVector *, void *);

SYMBOL_SHOW
const void *subfx_ptrVector_at(subfx_ptrVector *, size_t);

#ifdef __cplusplus
}
#endif
