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
/**
 * @enum subfx_exitstate
 * It defines the return state from SubFX APIs.
 */
typedef enum subfx_exitstate
{
    subfx_success, /**< success */
    subfx_successWithWarning, /**< success but with warning */
    subfx_eof, /**< end of file */
    subfx_failed /**< failed */
} subfx_exitstate;

/**
 * @enum subfx_types
 * The type ID for subfx_handle
 */
typedef enum subfx_types
{
    subfx_types_logger, /**< logger */
    subfx_types_fonthandle /**< font handle */
} subfx_types;

/**
 * @typedef subfx_cmpFunc
 * A function for comparison
 * @param lhs
 * @param rhs
 * @return if the return value is less than 0, indicate lhs is less than rhs;
 *         if the return value is equal to 0, indicate lhs is equal to rhs;
 *         else the return value is greater than 0.
 */
typedef int (*subfx_cmpFunc)(const void *lhs, const void *rhs);

/**
 * @typedef subfx_freeFunc
 * A function for free memory
 * @param toBeFreed the pointer to be freed
 */
typedef void (*subfx_freeFunc)(void *toBeFreed);

/**
 * a pointer for the object return from SubFX API.
 */
typedef void* subfx_handle;

#ifdef __cplusplus
}
#endif
