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
#include "subfx.h"
#include "logger.h"
#include "map.h"
#include "misc.h"
#include "ptrvector.h"
#include "smath.h"
#include "utf8.h"
#include "vector.h"
// #include "YutilsCpp/init.h"

SUBFX_API SubFX *SubFX_init()
{
    SubFX *ret = calloc(1, sizeof(SubFX));
    if (!ret)
    {
        return NULL;
    }

    ret->logger = NULL;
    ret->map = NULL;
    ret->misc = NULL;
    ret->ptrVector = NULL;
    ret->math = NULL;
    ret->utf8 = NULL;
    ret->vector = NULL;

    ret->logger = subfx_logger_init();
    if (!ret->logger)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->map = subfx_map_init();
    if (!ret->map)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->misc = subfx_misc_init();
    if (!ret->misc)
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

    ret->math = subfx_math_init();
    if (!ret->math)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->utf8 = subfx_utf8_init();
    if (!ret->utf8)
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

    if (in->logger) free(in->logger);
    if (in->map) free(in->map);
    if (in->misc) free(in->misc);
    if (in->ptrVector) free(in->ptrVector);
    if (in->math) free(in->math);
    if (in->utf8) free(in->utf8);
    if (in->vector) free(in->vector);
    free(in);
}
