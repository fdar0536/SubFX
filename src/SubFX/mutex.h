/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2021 fdar0536.
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

#include <inttypes.h>

#ifdef _WIN32
#include "windows.h"
#else
#include "pthread.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Mutex
{
#ifdef _WIN32
    HANDLE handle;
#else
    pthread_mutex_t handle;
#endif
} Mutex;

uint8_t Mutex_init(Mutex *);

void Mutex_fin(Mutex *);

uint8_t Mutex_lock(Mutex *);

uint8_t Mutex_unlock(Mutex *);

#ifdef __cplusplus
}
#endif
