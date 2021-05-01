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

#include "global.h"
#include "logger.h"
#include "misc.h"
#include "smath.h"
#include "subfx.h"
#include "utf8.h"

#include "ass.h"
#include "fonthandle.h"

SUBFX_API SubFX *SubFX_init()
{
    SubFX *ret = calloc(1, sizeof(SubFX));
    if (!ret)
    {
        return NULL;
    }

    ret->logger = NULL;
    ret->misc = NULL;
    ret->math = NULL;
    ret->utf8 = NULL;

    ret->ass = NULL;
    ret->fonthandle = NULL;

    ret->logger = subfx_logger_init();
    if (!ret->logger)
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

    ret->ass = subfx_ass_init();
    if (!ret->ass)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->fonthandle = subfx_fonthandle_init();
    if (!ret->fonthandle)
    {
        SubFX_destroy(ret);
        return NULL;
    }

    if (!globalInit())
    {
        SubFX_destroy(ret);
        return NULL;
    }

    ret->fdsa = getFDSA();
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
    if (in->misc) free(in->misc);
    if (in->math) free(in->math);
    if (in->utf8) free(in->utf8);

    if (in->ass) free(in->ass);
    if (in->fonthandle) free(in->fonthandle);
    globalFin();
    free(in);
}
