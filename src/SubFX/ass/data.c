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
#include <stdio.h>

#include "data.h"

subfx_exitstate subfx_ass_meta_init(subfx_ass_meta *ret)
{
    if (!ret)
    {
        return subfx_failed;
    }

    ret->wrap_style = 0;
    ret->scaled_border_and_shadow = true;
    ret->play_res_x = 640;
    ret->play_res_y = 360;
    sprintf(ret->colorMatrix, "TV.601");

    return subfx_success;
}

subfx_exitstate subfx_ass_style_init(subfx_ass_style *ret)
{
    if (!ret) return subfx_failed;

    sprintf(ret->fontname, "%s", "Arial");

    ret->fontsize = 20;
    ret->bold = false;
    ret->italic = false;
    ret->underline = false;
    ret->strikeout = false;
    ret->scale_x = 100.;
    ret->scale_y = 100.;
    ret->spaceing = 0.;
    ret->angle = 0.;
    ret->bolder_style = 1;
    ret->outline = 0.;
    ret->shadow = 0.;
    ret->alignment = 2;
    ret->margin_l = 0.;
    ret->margin_r = 0.;
    ret->margin_v = 20.;
    ret->encoding = 0;

    sprintf(ret->color1, "%s", "&HFFFFFF&");
    sprintf(ret->alpha1, "%s", "&H00&");
    sprintf(ret->color2, "%s", "&H000000&");
    sprintf(ret->alpha2, "%s", "&H00&");
    sprintf(ret->color3, "%s", "&HFFFFFF&");
    sprintf(ret->alpha3, "%s", "&H00&");
    sprintf(ret->color4, "%s", "&HFFFFFF&");
    sprintf(ret->alpha4, "%s", "&H00&");

    return subfx_success;
}
