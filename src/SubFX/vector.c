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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SubFX/types.h"
#include "vector.h"

typedef struct vector
{
    subfx_types id;

    uint8_t *data;

    size_t sizeOfData;

    size_t size;

    size_t capacity;
} vector;

subfx_vector *subfx_vector_init()
{
    subfx_vector *ret = calloc(1, sizeof(subfx_vector));
    if (!ret)
    {
        return NULL;
    }

    ret->create = subfx_vector_create;
    ret->at = subfx_vector_at;
    ret->setValue = subfx_vector_setValue;
    ret->clear = subfx_vector_clear;
    ret->size = subfx_vector_size;
    ret->capacity = subfx_vector_capacity;
    ret->reserve = subfx_vector_reserve;
    ret->pushback = subfx_vector_pushback;
    ret->resize = subfx_vector_resize;

    return ret;
}

subfx_handle subfx_vector_create(size_t sizeOfData)
{
    if (!sizeOfData)
    {
        return NULL;
    }

    vector *vec = calloc(1, sizeof (vector));
    if (!vec)
    {
        return NULL;
    }

    vec->id = subfx_types_vector;
    vec->data = NULL;
    vec->sizeOfData = sizeOfData;
    vec->size = 0;
    vec->capacity = 0;

    return vec;
}

subfx_exitstate subfx_vector_destroy(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_vector))
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (vec->data)
    {
        free(vec->data);
    }

    free(vec);
    return subfx_success;
}

subfx_exitstate subfx_vector_at(subfx_handle in, size_t index, void *dst)
{
    if (subfx_checkInput(in, subfx_types_vector))
    {
        return subfx_failed;
    }

    if (!dst)
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (index >= vec->size)
    {
        return subfx_failed;
    }

    uint8_t *data = vec->data;
    data += (index * vec->sizeOfData);
    memcpy(dst, data, vec->sizeOfData);

    return subfx_success;
}

subfx_exitstate subfx_vector_setValue(subfx_handle in,
                                      size_t index,
                                      const void *src)
{
    if (subfx_checkInput(in, subfx_types_vector))
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

    uint8_t *data = vec->data;
    data += (index * vec->sizeOfData);
    memcpy(data, src, vec->sizeOfData);

    return subfx_success;
}

subfx_exitstate subfx_vector_clear(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_vector))
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    vec->size = 0;
    return subfx_success;
}

subfx_exitstate subfx_vector_size(subfx_handle in, size_t *dst)
{
    if (subfx_checkInput(in, subfx_types_vector))
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

subfx_exitstate subfx_vector_capacity(subfx_handle in, size_t *dst)
{
    if (subfx_checkInput(in, subfx_types_vector))
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

subfx_exitstate subfx_vector_reserve(subfx_handle in, size_t newSize)
{
    if (subfx_checkInput(in, subfx_types_vector))
    {
        return subfx_failed;
    }

    vector *vec = (vector *)in;
    if (newSize <= vec->capacity)
    {
        // do nothing
        return subfx_success;
    }

    uint8_t *newData = calloc(newSize, vec->sizeOfData);
    if (!newData)
    {
        return subfx_failed;
    }

    if (vec->data)
    {
        memcpy(newData, vec->data, vec->sizeOfData * vec->size);
        free(vec->data);
    }

    vec->data = newData;
    vec->capacity = newSize;

    return subfx_success;
}

subfx_exitstate subfx_vector_pushback(subfx_handle in, const void *src)
{
    if (subfx_checkInput(in, subfx_types_vector))
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
        if (subfx_vector_reserve(vec, vec->capacity + 1) == subfx_failed)
        {
            return subfx_failed;
        }
    }

    uint8_t *data = vec->data;
    data += (vec->size * vec->sizeOfData);
    memcpy(data, src, vec->sizeOfData);
    ++vec->size;

    return subfx_success;
}

subfx_exitstate subfx_vector_resize(subfx_handle in,
                                    size_t amount,
                                    const void *src)
{
    if (subfx_checkInput(in, subfx_types_vector))
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
        if (subfx_vector_reserve(in, amount) == subfx_failed)
        {
            return subfx_failed;
        }
    }

    if (vec->size > amount)
    {
        vec->size = amount;
    }
    else if (vec->size < amount)
    {
        // vec->capacity >= amount
        size_t i;
        for (i = vec->size; i < amount; ++i)
        {
            if (subfx_vector_pushback(vec, src) == subfx_failed)
            {
                fputs("subfx_vector_resize: You should never see this line.",
                      stderr);
                return subfx_failed;
            }
        }
    }

    return subfx_success;
}
