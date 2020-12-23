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
#include <stdarg.h>
#include <stdlib.h>

#include "internal/types.h"
#include "tuple.h"

typedef struct Tuple
{
    subfx_types id;

    uint8_t **data;

    subfx_freeFunc *dataFreeFunc;

    size_t size;
} Tuple;

subfx_tuple *subfx_tuple_init()
{
    subfx_tuple *ret = calloc(1, sizeof(subfx_tuple));
    if (!ret)
    {
        return NULL;
    }

    ret->create = subfx_tuple_create;
    ret->at = subfx_tuple_at;

    return ret;
}

subfx_handle subfx_tuple_create(size_t size, ...)
{
    if (!size)
    {
        return NULL;
    }

    Tuple *ret = calloc(1, sizeof(Tuple));
    if (!ret)
    {
        return NULL;
    }

    ret->data = calloc(size, sizeof(uint8_t *));
    if (!ret->data)
    {
        free(ret);
        return NULL;
    }

    ret->dataFreeFunc = calloc(size, sizeof(subfx_freeFunc));
    if (!ret->dataFreeFunc)
    {
        free(ret->data);
        free(ret);
        return NULL;
    }

    ret->id = subfx_types_tuple;
    ret->size = size;
    va_list list;
    va_start(list, size);
    size <<= 1;
    size_t i;
    for (i = 0; i < size; ++i)
    {
        ret->data[i] = va_arg(list, void *);
        ret->dataFreeFunc[i] = va_arg(list, subfx_freeFunc);
    }

    va_end(list);

    return ret;
}

subfx_exitstate subfx_tuple_destroy(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_tuple))
    {
        return subfx_failed;
    }

    Tuple *tuple = (Tuple *)in;
    size_t i;
    for(i = 0; i < tuple->size; ++i)
    {
        if (tuple->data[i])
        {
            if (tuple->dataFreeFunc[i]) tuple->dataFreeFunc[i](tuple->data[i]);
        }
    }

    free(tuple->data);
    free(tuple->dataFreeFunc);
    free(tuple);

    return subfx_success;
}

void *subfx_tuple_at(subfx_handle in, size_t index)
{
    if (subfx_checkInput(in, subfx_types_tuple))
    {
        return NULL;
    }

    Tuple *tuple = (Tuple *)in;
    if (index >= tuple->size)
    {
        return NULL;
    }

    return tuple->data[index];
}
