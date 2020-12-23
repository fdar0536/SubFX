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

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum subfx_exitstate
{
    subfx_success,
    subfx_successWithWarning,
    subfx_failed
} subfx_exitstate;

typedef enum subfx_types
{
    subfx_types_utils_logger,
    subfx_type_map,
    subfx_types_pair,
    subfx_types_ptrVector,
    subfx_types_tuple,
    subfx_types_vector
} subfx_types;

typedef enum subfx_bool
{
    subfx_false,
    subfx_true
} subfx_bool;

typedef int (*subfx_cmpFunc)(const void *lhs, const void *rhs);
typedef void (*subfx_freeFunc)(void *toBeFree);
typedef void* subfx_handle;

#ifdef __cplusplus
}
#endif
