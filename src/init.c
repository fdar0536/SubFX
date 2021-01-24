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

#include "include/SubFX.h"
#include "internal/subfx.h"
#include "utils/init.h"
#include "utils/map.h"
#include "utils/ptrvector.h"
#include "utils/vector.h"
#include "YutilsCpp/init.h"

SUBFX_API SubFX *SubFX_init()
{
    SubFX *ret = calloc(1, sizeof(SubFX));
    if (!ret)
    {
        return NULL;
    }

    ret->map = NULL;
    ret->ptrVector = NULL;
    ret->utils = NULL;
    ret->vector = NULL;
    ret->yutils = NULL;

    ret->map = subfx_map_init();
    if (!ret->map)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->ptrVector = subfx_ptrVector_init();
    if (!ret->ptrVector)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->utils = subfx_utils_create();
    if (!ret->utils)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->vector = subfx_vector_init();
    if (!ret->vector)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->yutils = subfx_yutils_init();
    if (!ret->yutils)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->getHandleType = subfx_getHandleType;
    ret->closeHandle = subfx_closeHandle;
    ret->version = subfx_version;
    return ret;
}

SUBFX_API void SubFX_destroy(SubFX *in)
{
    if (!in)
    {
        return;
    }

    if (in->map) free(in->map);
    if (in->ptrVector) free(in->ptrVector);
    if (in->utils) subfx_utils_destory(in->utils);
    if (in->vector) free(in->vector);
    if (in->yutils) subfx_yutils_destroy(in->yutils);
    free(in);
}
