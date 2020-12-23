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

#include "init.h"
#include "logger.h"
#include "utils/math.h"
#include "misc.h"
#include "utf8.h"

subfx_utils *subfx_utils_create()
{
    subfx_utils *ret = calloc(1, sizeof(subfx_utils));
    if (!ret)
    {
        return NULL;
    }

    ret->logger = NULL;
    ret->math = NULL;
    ret->misc = NULL;
    ret->utf8 = NULL;

    // logger
    ret->logger = subfx_utils_logger_init();
    if (!ret->logger)
    {
        subfx_utils_destory(ret);
        return NULL;
    }

    // math
    ret->math = subfx_utils_math_init();
    if (!ret)
    {
        subfx_utils_destory(ret);
        return NULL;
    }

    // misc
    ret->misc = subfx_utils_misc_init();
    if (!ret->misc)
    {
        subfx_utils_destory(ret);
        return NULL;
    }

    //utf8
    ret->utf8 = subfx_utils_utf8_init();
    if (!ret->utf8)
    {
        subfx_utils_destory(ret);
        return NULL;
    }

    return ret;
}

void subfx_utils_destory(subfx_utils *utils)
{
    if (!utils)
    {
        return;
    }

    if (utils->logger) free(utils->logger);
    if (utils->math) free(utils->math);
    if (utils->misc) free(utils->misc);
    if (utils->utf8) free(utils->utf8);
    free(utils);
}
