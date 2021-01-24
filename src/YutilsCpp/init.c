/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2021 fdar0536.
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

#include "YutilsCpp/init.h"
#include "YutilsCpp/math.h"

subfx_yutils *subfx_yutils_init()
{
    subfx_yutils *ret = calloc(1, sizeof(subfx_yutils));
    if (!ret)
    {
        return NULL;
    }

    ret->math = NULL;

    ret->math = subfx_yutils_math_init();
    if (!ret->math)
    {
        subfx_yutils_destroy(ret);
        return NULL;
    }

    return ret;
}

void subfx_yutils_destroy(subfx_yutils *in)
{
    if (!in)
    {
        return;
    }

    if (in->math) free(in->math);
    free(in);
}
