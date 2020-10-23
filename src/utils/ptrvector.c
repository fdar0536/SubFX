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

#include <stdlib.h>
#include <string.h>

#include "SubFX.h"

typedef struct ptrVector
{
    void **m_data;
    subfx_freeFunc m_dataFreeFunc;

    size_t m_size;
    size_t m_capacity;
} ptrVector;

SYMBOL_SHOW
subfx_ptrVector *subfx_ptrVector_create(subfx_freeFunc dataFreeFunc)
{
    if (!dataFreeFunc)
    {
        return NULL;
    }

    ptrVector *ret = malloc(sizeof(ptrVector));
    if (!ret)
    {
        return NULL;
    }

    ret->m_data = NULL;
    ret->m_dataFreeFunc = dataFreeFunc;
    ret->m_size = 0;
    ret->m_capacity = 0;

    return ret;
}

SYMBOL_SHOW
void subfx_ptrVector_free(subfx_ptrVector *input)
{
    if (!input)
    {
        return;
    }

    ptrVector *vec = (ptrVector *)input;
    if (vec->m_capacity != 0)
    {
        size_t index;
        char **data = (char **)vec->m_data;
        for (index = 0; index < vec->m_size; ++index)
        {
            vec->m_dataFreeFunc(data[index]);
        }

        free(data);
    }

    free(vec);
    input = NULL;
}

SYMBOL_SHOW
size_t subfx_ptrVector_size(subfx_ptrVector *input, char *errMsg)
{
    if (!input)
    {
        subfx_pError(errMsg, "subfx_ptrVector_size: No input.")
        return 0;
    }

    ptrVector *vec = (ptrVector *)input;
    return vec->m_size;
}

SYMBOL_SHOW
size_t subfx_ptrVector_capacity(subfx_ptrVector *input, char *errMsg)
{
    if (!input)
    {
        subfx_pError(errMsg, "subfx_ptrVector_capacity: No input.")
        return 0;
    }

    ptrVector *vec = (ptrVector *)input;
    return vec->m_capacity;
}

SYMBOL_SHOW
subfx_exitstate subfx_ptrVector_reserve(subfx_ptrVector *input, size_t newSize)
{
    if (!input)
    {
        return subfx_failed;
    }

    ptrVector *vec = (ptrVector *)input;
    if (newSize <= vec->m_capacity)
    {
        // do nothing
        return subfx_success;
    }

    // newSize > vec->m_capacity
    char **newData = calloc(newSize, sizeof(char *));
    if (!newData)
    {
        return subfx_failed;
    }

    char **data = (char **)vec->m_data;
    memcpy(newData, data, vec->m_size * sizeof(char *));
    free(data);
    vec->m_data = (void **)newData;
    vec->m_capacity = newSize;

    return subfx_success;
}


SYMBOL_SHOW
subfx_exitstate subfx_ptrVector_pushback(subfx_ptrVector *input, void *value)
{
    if (!input || !value)
    {
        return subfx_failed;
    }

    ptrVector *vec = (ptrVector *)input;
    if (vec->m_size == vec->m_capacity)
    {
        // vector is full
        if (subfx_ptrVector_reserve(vec, vec->m_size + 1) == subfx_failed)
        {
            return subfx_failed;
        }
    }

    char **data = (char **)vec->m_data;
    size_t last = vec->m_size - 1;

    data[last] = calloc(1, sizeof(char *));
    if (!data[last])
    {
        return subfx_failed;
    }

    memcpy(data[last], value, sizeof(char *));

    return subfx_success;
}

SYMBOL_SHOW
const void *subfx_ptrVector_at(subfx_ptrVector *input, size_t pos)
{
    if (!input)
    {
        return NULL;
    }

    ptrVector *vec = (ptrVector *)input;
    if (pos >= vec->m_size)
    {
        // out of range
        return NULL;
    }

    char **data = (char **)vec->m_data;
    return (const void *)data[pos];
}
