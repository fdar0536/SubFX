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

#include "mutex.h"

uint8_t Mutex_init(Mutex *in)
{
    if (!in) return 1;

#ifdef _WIN32
    in->handle = CreateMutex(
                NULL,
                FALSE,
                NULL);
    if (!in->handle)
    {
        return 1;
    }
#else
    if (pthread_mutex_init (&in->handle, NULL))
    {
        return 1;
    }
#endif

    return 0;
}

void Mutex_fin(Mutex *in)
{
    if (!in) return;
    if (in->handle)
    {
#ifdef _WIN32
        CloseHandle(in->handle);
#else
        pthread_mutex_destroy(&in->handle);
#endif
    }

    memset(in, 0, sizeof(Mutex));
}

uint8_t Mutex_lock(Mutex *in)
{
#ifdef _WIN32
    if (WaitForSingleObject(in->handle, INFINITE) != WAIT_OBJECT_0)
#else
    if (pthread_mutex_lock(&in->handle))
#endif
    {
        return 1;
    }

    return 0;
}

uint8_t Mutex_unlock(Mutex *in)
{
#ifdef _WIN32
    if (!ReleaseMutex(in->handle))
#else
    if (pthread_mutex_unlock(&in->handle))
#endif
    {
        return 1;
    }

    return 0;
}
