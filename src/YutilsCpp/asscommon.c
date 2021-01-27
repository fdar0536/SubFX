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

#include "YutilsCpp/asscommon.h"

subfx_yutils_AssMeta *subfx_yutils_AssMeta_create()
{
    subfx_yutils_AssMeta *ret = calloc(1, sizeof(subfx_yutils_AssMeta));
    if (!ret)
    {
        return NULL;
    }

    ret->colorMatrix = calloc(20, sizeof(char));
    if (!ret->colorMatrix)
    {
        free(ret);
        return NULL;
    }

    ret->wrap_style = 0;
    ret->scaled_border_and_shadow = true;
    ret->play_res_x = 640;
    ret->play_res_y = 360;
    sprintf(ret->colorMatrix, "TV.601");

    return ret;
}

void subfx_yutils_AssMeta_destroy(subfx_yutils_AssMeta *in)
{
    if (!in)
    {
        return;
    }

    if (in->colorMatrix) free(in->colorMatrix);
    free(in);
}

subfx_yutils_AssStyle *subfx_yutils_AssStyle_create()
{}

void subfx_yutils_AssStyle_destroy(subfx_yutils_AssStyle *);
