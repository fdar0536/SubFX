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

#include "include/SubFX.h"

#include "ass.h"
#include "global.h"
#include "logger.h"
#include "misc.h"
#include "smath.h"
#include "subfx.h"
#include "utf8.h"

#include "ass.h"
#include "fonthandle.h"

SUBFX_API subfx_exitstate SubFX_init(SubFX *ret)
{
    if (!ret)
    {
        return subfx_failed;
    }

    if (subfx_logger_init(&ret->logger) == subfx_failed)
    {
        return subfx_failed;
    }

    if (subfx_misc_init(&ret->misc) == subfx_failed)
    {
        return subfx_failed;
    }

    if (subfx_math_init(&ret->math) == subfx_failed)
    {
        return subfx_failed;
    }

    if (subfx_utf8_init(&ret->utf8) == subfx_failed)
    {
        return subfx_failed;
    }

    if (subfx_ass_init(&ret->ass) == subfx_failed)
    {
        return subfx_failed;
    }

    if (subfx_fontHandle_init(&ret->fontHandle) == subfx_failed)
    {
        return subfx_failed;
    }

    if (!globalInit())
    {
        return subfx_failed;
    }

    ret->fdsa = getFDSA();
    ret->version = subfx_version;
    return subfx_success;
}

SUBFX_API void SubFX_fin(SubFX *in)
{
    if (!in) return;

    memset(in, 0, sizeof(SubFX));
    subfx_ass_fin();
}
