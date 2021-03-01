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

#define FONT_NAME_LEN 32
#define COLOR_ALPHA_LEN 16
#define ASS_TEXT_LEN (1 << 15) // 32KB

subfx_ass_meta *subfx_ass_meta_create()
{
    subfx_ass_meta *ret = calloc(1, sizeof(subfx_ass_meta));
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

void subfx_ass_meta_destroy(subfx_ass_meta *in)
{
    if (!in)
    {
        return;
    }

    if (in->colorMatrix) free(in->colorMatrix);
    free(in);
}

#define ALLOC_OR_FREE(x, y, z) \
    x = calloc(y, sizeof(char)); \
    if (!x) \
    { \
        subfx_ass_style_destroy(ret); \
        return NULL; \
    } \
    sprintf(x, "%s", z);

subfx_ass_style *subfx_ass_style_create()
{
    subfx_ass_style *ret = calloc(1, sizeof(subfx_ass_style));
    if (!ret) return NULL;

    ret->fontname = NULL;
    ret->color1 = NULL;
    ret->alpha1 = NULL;
    ret->color2 = NULL;
    ret->alpha2 = NULL;
    ret->color3 = NULL;
    ret->alpha3 = NULL;
    ret->color4 = NULL;
    ret->alpha4 = NULL;

    ALLOC_OR_FREE(ret->fontname, FONT_NAME_LEN, "Arial");

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

    ALLOC_OR_FREE(ret->color1, COLOR_ALPHA_LEN, "&HFFFFFF&");
    ALLOC_OR_FREE(ret->alpha1, COLOR_ALPHA_LEN, "&H00&");
    ALLOC_OR_FREE(ret->color2, COLOR_ALPHA_LEN, "&H000000&");
    ALLOC_OR_FREE(ret->alpha2, COLOR_ALPHA_LEN, "&H00&");
    ALLOC_OR_FREE(ret->color3, COLOR_ALPHA_LEN, "&HFFFFFF&");
    ALLOC_OR_FREE(ret->alpha3, COLOR_ALPHA_LEN, "&H00&");
    ALLOC_OR_FREE(ret->color4, COLOR_ALPHA_LEN, "&HFFFFFF&");
    ALLOC_OR_FREE(ret->alpha4, COLOR_ALPHA_LEN, "&H00&");

    return ret;
}

void subfx_ass_style_destroy(subfx_ass_style *in)
{
    if (!in) return;

    if (in->fontname) free(in->fontname);

    if (in->color1) free(in->color1);
    if (in->alpha1) free(in->alpha1);
    if (in->color2) free(in->color2);
    if (in->alpha2) free(in->alpha2);
    if (in->color3) free(in->color3);
    if (in->alpha3) free(in->alpha3);
    if (in->color4) free(in->color4);
    if (in->alpha4) free(in->alpha4);

    free(in);
}
