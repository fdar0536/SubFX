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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SubFX/types.h"
#include "ptrvector.h"

typedef struct vector
{
    subfx_types id;

    uint8_t **data;

    subfx_freeFunc freeFunc;

    size_t size;

    size_t capacity;
} vector;

subfx_ptrVector *subfx_ptrVector_init()
{
    subfx_ptrVector *ret = calloc(1, sizeof(subfx_ptrVector));
    if (!ret)
    {
        return NULL;
    }

    ret->create = subfx_ptrVector_create;
    ret->at = subfx_ptrVector_at;
    ret->setValue = subfx_ptrVector_setValue;
    ret->clear = subfx_ptrVector_clear;
    ret->size = subfx_ptrVector_size;
    ret->capacity = subfx_ptrVector_capacity;
    ret->reserve = subfx_ptrVector_reserve;
    ret->pushback = subfx_ptrVector_pushback;
    ret->resize = subfx_ptrVector_resize;

    return ret;
}

subfx_handle subfx_ptrVector_create(subfx_freeFunc freeFunc)
{
    if (!freeFunc)
    {
        return NULL;
    }

    vector *ret = calloc(1, sizeof(vector));
    if (!ret)
    {
        return NULL;
    }

    ret->id = subfx_types_ptrVector;
    ret->data = NULL;
    ret->freeFunc = freeFunc;
    ret->size = 0;
    ret->capacity = 0;

    return ret;
}

subfx_exitstate subfx_ptrVector_destroy(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (vec->data)
    {
        size_t i = 0;
        for (i = 0; i < vec->size; ++i)
        {
            vec->freeFunc(vec->data[i]);
        }

        free(vec->data);
    }

    free(in);
    return subfx_success;
}

void *subfx_ptrVector_at(subfx_handle in, size_t index)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return NULL;
    }

    vector *vec = (vector *)in;
    if (index >= vec->size)
    {
        return NULL;
    }

    uint8_t **data = vec->data;
    data += index;

    return data[0];
}

subfx_exitstate subfx_ptrVector_setValue(subfx_handle in,
                                         size_t index,
                                         void *src)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    if (!src)
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (index >= vec->size)
    {
        return subfx_failed;
    }

    vec->freeFunc(vec->data[index]);
    vec->data[index] = src;

    return subfx_success;
}

subfx_exitstate subfx_ptrVector_clear(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;

    size_t i = 0;
    for (i = 0; i < vec->size; ++i)
    {
        vec->freeFunc(vec->data[i]);
        vec->data[i] = NULL;
    }

    vec->size = 0;
    return subfx_success;
}

subfx_exitstate subfx_ptrVector_size(subfx_handle in, size_t *dst)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    if (!dst)
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    *dst = vec->size;

    return subfx_success;
}

subfx_exitstate subfx_ptrVector_capacity(subfx_handle in, size_t *dst)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    if (!dst)
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    *dst = vec->capacity;

    return subfx_success;
}

subfx_exitstate subfx_ptrVector_reserve(subfx_handle in, size_t newSize)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (newSize <= vec->capacity)
    {
        // do nothing
        return subfx_success;
    }

    uint8_t **newData = calloc(newSize, sizeof(uint8_t *));
    if (!newData)
    {
        return subfx_failed;
    }

    if (vec->data)
    {
        size_t i;
        for (i = 0; i < vec->size; ++i)
        {
            newData[i] = vec->data[i];
        }

        free(vec->data);
    }

    vec->data = newData;
    vec->capacity = newSize;

    return subfx_success;
}

subfx_exitstate subfx_ptrVector_pushback(subfx_handle in, void *src)
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    if (!src)
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (vec->size == vec->capacity)
    {
        if (subfx_ptrVector_reserve(vec, vec->capacity + 1) == subfx_failed)
        {
            return subfx_failed;
        }
    }

    uint8_t **data = vec->data;

    // array just contain pointer
    data += vec->size;
    data[0] = src;
    ++vec->size;

    return subfx_success;
}

subfx_exitstate subfx_ptrVector_resize(subfx_handle in,
                                       size_t amount,
                                       void *src,
                                       void *(*deepCopyFunc)(void *))
{
    if (subfx_checkInput(in, subfx_types_ptrVector))
    {
        return subfx_failed;
    }

    if (!src)
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (vec->capacity < amount)
    {
        if (subfx_ptrVector_reserve(in, amount) == subfx_failed)
        {
            return subfx_failed;
        }
    }

    size_t i;
    if (vec->size > amount)
    {
        for (i = amount; i < vec->size; ++i)
        {
            vec->freeFunc(vec->data[i]);
            vec->data[i] = NULL;
        }

        vec->size = amount;
    }
    else if (vec->size < amount)
    {
        // vec->capacity >= amount

        void *toBeInsert = NULL;
        for (i = vec->size; i < amount; ++i)
        {
            toBeInsert = deepCopyFunc(src);
            if (!toBeInsert)
            {
                return subfx_failed;
            }

            if (subfx_ptrVector_pushback(vec, toBeInsert) == subfx_failed)
            {
                return subfx_failed;
            }
        }
    }

    return subfx_success;
}
