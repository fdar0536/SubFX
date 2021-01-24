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

#include "config.h"
#include "subfx.h"
#include "types.h"

// utils
#include "utils/logger.h"
#include "utils/map.h"
#include "utils/vector.h"
#include "utils/ptrvector.h"

subfx_exitstate subfx_getHandleType(subfx_handle in, subfx_types *dst)
{
    if (!in)
    {
        return subfx_failed;
    }

    ID *id = (ID *)in;
    *dst = id->id;

    return subfx_success;
}

subfx_exitstate subfx_closeHandle(subfx_handle in)
{
    if (!in)
    {
        return subfx_failed;
    }

    ID *id = (ID *)in;
    switch(id->id)
    {
    case subfx_types_utils_logger:
    {
        return subfx_utils_logger_destroy(in);
    }
    case subfx_types_vector:
    {
        return subfx_vector_destroy(in);
    }
    case subfx_types_ptrVector:
    {
        return subfx_ptrVector_destroy(in);
    }
    case subfx_type_map:
    {
        return subfx_map_destroy(in);
    }
    default:
    {
        return subfx_failed;
    }
    }
}

const char *subfx_version()
{
    return PROJ_NAME" r"PROJ_VERSION;
}
