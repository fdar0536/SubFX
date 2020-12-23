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

#include "internal/types.h"
#include "pair.h"

typedef struct Pair
{
    subfx_types id;

    uint8_t *first;

    subfx_freeFunc firstFreeFunc;

    uint8_t *second;

    subfx_freeFunc secondFreeFunc;
} Pair;

subfx_pair *subfx_pair_init()
{
    subfx_pair *ret = calloc(1, sizeof (subfx_pair));
    if(!ret)
    {
        return NULL;
    }

    ret->create = subfx_pair_create;
    ret->first = subfx_pair_first;
    ret->setFirst = subfx_pair_setFirst;
    ret->second = subfx_pair_second;
    ret->setSecond = subfx_pair_setSecond;

    return ret;
}

subfx_handle subfx_pair_create(void *first, subfx_freeFunc firstFreeFunc,
                               void *second, subfx_freeFunc secondFreeFunc)
{
    Pair *ret = calloc(1, sizeof(Pair));
    if (!ret)
    {
        return NULL;
    }

    ret->first = first;
    ret->firstFreeFunc = firstFreeFunc;
    ret->second = second;
    ret->secondFreeFunc = secondFreeFunc;

    return ret;
}

subfx_exitstate subfx_pair_destroy(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_pair))
    {
        return subfx_failed;
    }

    Pair *pair = (Pair *)in;
    if (pair->first)
    {
        if (pair->firstFreeFunc) pair->firstFreeFunc(pair->first);
    }

    if (pair->second)
    {
        if (pair->secondFreeFunc) pair->secondFreeFunc(pair->second);
    }

    free(pair);

    return subfx_success;
}

void *subfx_pair_first(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_pair))
    {
        return NULL;
    }

    Pair *pair = (Pair *)in;
    return pair->first;
}

subfx_exitstate subfx_pair_setFirst(subfx_handle in,
                                    void *first,
                                    subfx_freeFunc firstFreeFunc)
{
    if (subfx_checkInput(in, subfx_types_pair))
    {
        return subfx_failed;
    }

    Pair *pair = (Pair *)in;
    if (pair->first)
    {
        if (pair->firstFreeFunc) pair->firstFreeFunc(pair->first);
    }

    pair->first = first;
    pair->firstFreeFunc = firstFreeFunc;
    return subfx_success;
}

void *subfx_pair_second(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_pair))
    {
        return NULL;
    }

    Pair *pair = (Pair *)in;
    return pair->second;
}

subfx_exitstate subfx_pair_setSecond(subfx_handle in,
                                     void *second,
                                     subfx_freeFunc secondFreeFunc)
{
    if (subfx_checkInput(in, subfx_types_pair))
    {
        return subfx_failed;
    }

    Pair *pair = (Pair *)in;
    if (pair->second)
    {
        if (pair->secondFreeFunc) pair->secondFreeFunc(pair->second);
    }

    pair->second = second;
    pair->secondFreeFunc = secondFreeFunc;
    return subfx_success;
}

