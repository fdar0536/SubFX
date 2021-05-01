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

#include <new>

#include <cstring>

#ifdef _WIN32
#include <string>

#include "windows.h"
#include "boost/locale.hpp"
#else
#include "pango/pangocairo.h"
#endif

#include "common.h"
#include "fonthandle.h"
#include "global.h"
#include "misc.h"
#include "types.h"
#include "smath.h"

#define FONT_PRECISION 64

extern "C"
{

typedef struct FontHandle
{
    subfx_types id;

#ifdef _WIN32
    HDC dc;

    HFONT font;

    HGDIOBJ old_font;

    double hspace;

    int upscale;
#else
    cairo_surface_t *surface;

    cairo_t *context;

    PangoLayout *layout;

    double fonthack_scale;
#endif

    double xscale;

    double yscale;

    double downscale;
} FontHandle;

subfx_fonthandle *subfx_fonthandle_init()
{
    subfx_fonthandle *ret(reinterpret_cast<subfx_fonthandle *>
                          (calloc(1, sizeof(subfx_fonthandle))));
    if (!ret)
    {
        return NULL;
    }

    ret->create = subfx_fonthandle_create;
    ret->metrics = subfx_fonthandle_metrics;
    ret->text_extents = subfx_fonthandle_text_extents;
    ret->text_to_shape = subfx_fonthandle_text_to_shape;

    return ret;
}

subfx_handle subfx_fonthandle_create(const char *family,
                                     bool bold,
                                     bool italic,
                                     bool underline,
                                     bool strikeout,
                                     int32_t size,
                                     double xscale, // 1.
                                     double yscale, // 1.
                                     double hspace, // 0.
                                     char *errMsg)
{
    if (size <= 0)
    {
        subfx_pError(errMsg,
                     "FontHandle->create: size cannot lower than 0");
        return NULL;
    }

    FontHandle *ret(reinterpret_cast<FontHandle *>
                    (calloc(1, sizeof(FontHandle))));
    if (!ret)
    {
        return NULL;
    }

    ret->id = subfx_types_fonthandle;
    ret->xscale = xscale;
    ret->yscale = yscale;
#ifdef _WIN32
    ret->hspace = hspace;
    ret->upscale = FONT_PRECISION;
    ret->downscale = (1.f / static_cast<double>(ret->upscale));
    ret->dc = NULL;
    ret->font = NULL;
    ret->old_font = NULL;

    std::wstring family_dst(boost::locale::conv::utf_to_utf<wchar_t>(family));
    if (wcslen(family_dst.c_str()) > 31)
    {
        subfx_fonthandle_destroy(ret);
        subfx_pError(errMsg,
                     "FontHandle->create: family name too long");
    }

    ret->dc = CreateCompatibleDC(NULL);
    if (!ret->dc)
    {
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    int res = SetMapMode(ret->dc, MM_TEXT);
    if (res == 0)
    {
        DeleteDC(ret->dc);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    res = SetBkMode(ret->dc, TRANSPARENT);
    if (res == 0)
    {
        DeleteDC(ret->dc);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    ret->font = CreateFontW(
        size * ret->upscale, // nHeight
        0,    // nWidth
        0,    // nEscapement
        0,    // nOrientation
        (bold ? FW_BOLD : FW_NORMAL),    // fnWeight
        (italic ? 1 : 0),    // fdwItalic
        (underline ? 1 : 0),    //fdwUnderline
        (strikeout ? 1 : 0),    // fdwStrikeOut
        DEFAULT_CHARSET,    // fdwCharSet
        OUT_TT_PRECIS,    // fdwOutputPrecision
        CLIP_DEFAULT_PRECIS,    // fdwClipPrecision
        ANTIALIASED_QUALITY,    // fdwQuality
        DEFAULT_PITCH + FF_DONTCARE,    // fdwPitchAndFamily
        family_dst.c_str()
    );

    if (!ret->font)
    {
        DeleteDC(ret->dc);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    ret->old_font = SelectObject(ret->dc, ret->font);
#else
    int upscale(FONT_PRECISION);
    ret->downscale = (1. / static_cast<double>(upscale));
    ret->surface = NULL;
    ret->context = NULL;
    ret->layout = NULL;

    // This is almost copypasta from Youka/Yutils
    ret->surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 1, 1);
    if (!ret->surface)
    {
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    ret->context = cairo_create(ret->surface);
    if (!ret->context)
    {
        cairo_surface_destroy(ret->surface);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    ret->layout = pango_cairo_create_layout(ret->context);
    if (!ret->layout)
    {
        cairo_destroy(ret->context);
        cairo_surface_destroy(ret->surface);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    //set font to layout
    PangoFontDescription *font_desc(pango_font_description_new());
    if (!font_desc)
    {
        g_object_unref(ret->layout);
        cairo_destroy(ret->context);
        cairo_surface_destroy(ret->surface);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    pango_font_description_set_family(font_desc, family);
    pango_font_description_set_weight(font_desc,
                                      bold ? PANGO_WEIGHT_BOLD :
                                      PANGO_WEIGHT_NORMAL);
    pango_font_description_set_style(font_desc,
                                     italic ? PANGO_STYLE_ITALIC :
                                     PANGO_STYLE_NORMAL);
    pango_font_description_set_absolute_size(font_desc,
                                             size * PANGO_SCALE * upscale);
    pango_layout_set_font_description(ret->layout, font_desc);

    PangoAttrList *attr(pango_attr_list_new());
    if (!attr)
    {
        pango_font_description_free(font_desc);
        g_object_unref(ret->layout);
        cairo_destroy(ret->context);
        cairo_surface_destroy(ret->surface);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    pango_attr_list_insert(attr,
                           pango_attr_underline_new(
                           underline ? PANGO_UNDERLINE_SINGLE :
                           PANGO_UNDERLINE_NONE));
    pango_attr_list_insert(attr,
                           pango_attr_strikethrough_new(strikeout));
    pango_attr_list_insert(attr,
                           pango_attr_letter_spacing_new(
                           static_cast<int>(hspace) * PANGO_SCALE * upscale));
    pango_layout_set_attributes(ret->layout, attr);

    PangoFontMetrics *metrics(pango_context_get_metrics(
                              pango_layout_get_context(ret->layout),
                              pango_layout_get_font_description(ret->layout),
                              NULL));
    if (!metrics)
    {
        pango_attr_list_unref(attr);
        pango_font_description_free(font_desc);
        g_object_unref(ret->layout);
        cairo_destroy(ret->context);
        cairo_surface_destroy(ret->surface);
        subfx_fonthandle_destroy(ret);
        return NULL;
    }

    double ascent(static_cast<double>(pango_font_metrics_get_ascent(metrics)));
    double descent(static_cast<double>(pango_font_metrics_get_descent(metrics)));

    ret->fonthack_scale = size /
                         ((ascent + descent) /
                           static_cast<double>(PANGO_SCALE) *
                           ret->downscale);

    pango_font_metrics_unref(metrics);
    pango_attr_list_unref(attr);
    pango_font_description_free(font_desc);
#endif

    return ret;
}

subfx_exitstate subfx_fonthandle_destroy(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_fonthandle))
    {
        return subfx_failed;
    }

    FontHandle *handle(reinterpret_cast<FontHandle *>(in));

#ifdef _WIN32
    SelectObject(handle->dc, handle->old_font);
    DeleteObject(handle->font);
    DeleteDC(handle->dc);
#else
    g_object_unref(handle->layout);
    cairo_destroy(handle->context);
    cairo_surface_destroy(handle->surface);
#endif

    free(in);

    return subfx_success;
}

double *subfx_fonthandle_metrics(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_fonthandle))
    {
        return NULL;
    }

    double *ret(reinterpret_cast<double *>(calloc(5, sizeof(double))));
    if (!ret)
    {
        return NULL;
    }

    FontHandle *handle(reinterpret_cast<FontHandle *>(in));

#ifdef _WIN32
    TEXTMETRICW  *fontMetrics(new (std::nothrow) TEXTMETRICW);
    if (!fontMetrics)
    {
        free(ret);
        return NULL;
    }

    if (GetTextMetricsW(handle->dc, fontMetrics) == 0)
    {
        delete fontMetrics;
        free(ret);
        return NULL;
    }

    ret[subfx_fonthandle_metrics_height] =
            fontMetrics->tmHeight *
            handle->downscale *
            handle->yscale;

    ret[subfx_fonthandle_metrics_ascent] =
            fontMetrics->tmAscent *
            handle->downscale *
            handle->yscale;

    ret[subfx_fonthandle_metrics_descent] =
            fontMetrics->tmDescent *
            handle->downscale *
            handle->yscale;

    ret[subfx_fonthandle_metrics_internal_leading] =
            fontMetrics->tmInternalLeading *
            handle->downscale *
            handle->yscale;

    ret[subfx_fonthandle_metrics_external_leading] =
            fontMetrics->tmExternalLeading *
            handle->downscale *
            handle->yscale;

    delete fontMetrics;
#else
    PangoFontMetrics *fontMetrics(pango_context_get_metrics(
        pango_layout_get_context(handle->layout),
        pango_layout_get_font_description(handle->layout),
        NULL
    ));

    if (!fontMetrics)
    {
        free(ret);
        return NULL;
    }

    double ascent(
                static_cast<double>
                (pango_font_metrics_get_ascent(fontMetrics)));
    ascent = ascent / PANGO_SCALE * handle->downscale;

    double descent =
            static_cast<double>
            (pango_font_metrics_get_descent(fontMetrics));
    descent = descent / PANGO_SCALE * handle->downscale;
    pango_font_metrics_unref(fontMetrics);

    ret[subfx_fonthandle_metrics_height] =
            (ascent + descent) *
            handle->yscale *
            handle->fonthack_scale;

    ret[subfx_fonthandle_metrics_ascent] =
            ascent *
            handle->yscale *
            handle->fonthack_scale;

    ret[subfx_fonthandle_metrics_descent] =
            descent *
            handle->yscale *
            handle->fonthack_scale;

    ret[subfx_fonthandle_metrics_internal_leading] = 0.;

    ret[subfx_fonthandle_metrics_external_leading] =
            pango_layout_get_spacing(handle->layout) /
            PANGO_SCALE *
            handle->downscale *
            handle->yscale *
            handle->fonthack_scale;
#endif

    return ret;
}

double *subfx_fonthandle_text_extents(subfx_handle in,
                                             const char *text)
{
    if (subfx_checkInput(in, subfx_types_fonthandle))
    {
        return NULL;
    }

    double *ret(reinterpret_cast<double *>(calloc(2, sizeof(double))));
    if (!ret)
    {
        return NULL;
    }

    FontHandle *handle(reinterpret_cast<FontHandle *>(in));

#ifdef _WIN32
    std::wstring textDst(boost::locale::conv::utf_to_utf<wchar_t>(text));
    size_t textLen = wcslen(textDst.c_str());

    SIZE *size(new (std::nothrow) SIZE);
    if (!size)
    {
        free(ret);
        return NULL;
    }

    if (GetTextExtentPoint32W(handle->dc, textDst.c_str(),
                              static_cast<int>(textLen), size) == 0)
    {
        delete size;
        free(ret);
        return NULL;
    }

    ret[subfx_fonthandle_text_extents_width] =
            (size->cx * handle->downscale +
             handle->hspace * textLen) *
            handle->xscale;

    ret[subfx_fonthandle_text_extents_height] =
            size->cy *
            handle->downscale *
            handle->yscale;

    delete size;
#else
    pango_layout_set_text(handle->layout, text, -1);
    PangoRectangle *rect(new (std::nothrow) PangoRectangle);
    if (!rect)
    {
        free(ret);
        return NULL;
    }

    pango_layout_get_pixel_extents(handle->layout, NULL, rect);
    ret[subfx_fonthandle_text_extents_width] =
            rect->width *
            handle->downscale *
            handle->xscale *
            handle->fonthack_scale;

    ret[subfx_fonthandle_text_extents_height] =
            rect->height *
            handle->downscale *
            handle->yscale *
            handle->fonthack_scale;

    delete rect;
#endif

    return ret;
}

#ifdef _WIN32
#define cleanUp \
    AbortPath(handle->dc); \
    delete[] points; \
    delete[] types; \
    if (!charWidths) \
    { \
        delete[] charWidths; \
    } \
     \
    if (fdsa->closeHandle(shape) == fdsa_failed) \
    { \
        subfx_pError(errMsg, "text_to_shape: you should" \
                             " never see this message."); \
    }
#else
#define cleanUp \
    cairo_new_path(handle->context); \
    cairo_path_destroy(path); \
    if (fdsa->closeHandle(shape) == fdsa_failed) \
    { \
        subfx_pError(errMsg, "text_to_shape: you should" \
                             " never see this message."); \
    }
#endif

char *subfx_fonthandle_text_to_shape(subfx_handle in,
                                            const char *text, char *errMsg)
{
    fDSA *fdsa(getFDSA());
    if (!fdsa)
    {
        return NULL;
    }

    if (subfx_checkInput(in, subfx_types_fonthandle))
    {
        return NULL;
    }

    FontHandle *handle(reinterpret_cast<FontHandle *>(in));
    char *retStr;
    size_t retSize;
#ifdef _WIN32
    std::wstring textDst(boost::locale::conv::utf_to_utf<wchar_t>(text));
    size_t textLen(wcslen(textDst.c_str()));
    if (textLen > 8192)
    {
        subfx_pError(errMsg, "text_to_shape: text is too long");
        return NULL;
    }

    INT *charWidths(NULL);
    if (handle->hspace != 0)
    {
        charWidths = new (std::nothrow) INT[textLen];
        if (!charWidths)
        {
            return NULL;
        }

        SIZE *size(new (std::nothrow) SIZE);
        if (!size)
        {
            delete[] charWidths;
            return NULL;
        }

        int space(static_cast<int>(handle->hspace * handle->upscale));
        for (size_t i = 0; i <= (textLen - 1); ++i)
        {
            if (GetTextExtentPoint32W(handle->dc, textDst.c_str() + i, 1, size) == 0)
            {
                delete[] charWidths;
                delete size;
                return NULL;
            }

            charWidths[i] = size->cx + space;
        }

        delete size;
    } // end if (hspace != 0)

    if (BeginPath(handle->dc) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(handle->dc);
        return NULL;
    }

    if (ExtTextOutW(handle->dc, 0, 0, 0x0, NULL, textDst.c_str(),
                    static_cast<UINT>(textLen), charWidths) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(handle->dc);
        return NULL;
    }

    if (EndPath(handle->dc) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(handle->dc);
        return NULL;
    }

    int points_n(GetPath(handle->dc, NULL, NULL, 0));
    if (points_n <= 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(handle->dc);
        return NULL;
    }

    POINT *points(new (std::nothrow) POINT[points_n]);
    if (!points)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(handle->dc);
        return NULL;
    }

    BYTE *types(new (std::nothrow) BYTE[points_n]);
    if (!types)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        delete[] points;
        AbortPath(handle->dc);
        return NULL;
    }

    GetPath(handle->dc, points, types, points_n);

    fdsa_handle shape(fdsa->ptrVector->create(free));
    if (!shape)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        delete[] points;
        delete[] types;
        AbortPath(handle->dc);
        return NULL;
    }

    // may be larger or smaller?
    if (fdsa->ptrVector->reserve(shape, 2048) == fdsa_failed)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        delete[] points;
        delete[] types;
        AbortPath(handle->dc);
        return NULL;
    }

    int i(0);
    BYTE last_type(0xff), cur_type(0xff);
    POINT cur_point;
    double tmpDouble(0.f);
    while (i < points_n)
    {
        cur_type = types[i];
        cur_point = points[i];

        switch (cur_type)
        {
        case PT_MOVETO:
            if (last_type != PT_MOVETO)
            {
                retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
                if (!retStr)
                {
                    cleanUp;
                    return NULL;
                }

                memcpy(retStr, "m", 2);
                if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
                {
                    cleanUp;
                    free(retStr);
                    return NULL;
                }

                last_type = cur_type;
            }

            tmpDouble =
                    subfx_math_round(
                        cur_point.x *
                        handle->downscale *
                        handle->xscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpDouble = subfx_math_round(
                        cur_point.y *
                        handle->downscale *
                        handle->yscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            ++i;
            break;
        case PT_LINETO:
        case (PT_LINETO + PT_CLOSEFIGURE):
            if (last_type != PT_LINETO)
            {
                retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
                if (!retStr)
                {
                    cleanUp;
                    return NULL;
                }

                memcpy(retStr, "l", 2);
                if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
                {
                    cleanUp;
                    free(retStr);
                    return NULL;
                }

                last_type = cur_type;
            }

            tmpDouble = subfx_math_round(
                        cur_point.x *
                        handle->downscale *
                        handle->xscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpDouble = subfx_math_round(
                        cur_point.y *
                        handle->downscale *
                        handle->yscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            ++i;
            break;
        case PT_BEZIERTO:
        case (PT_BEZIERTO + PT_CLOSEFIGURE):
            if (last_type != PT_BEZIERTO)
            {
                retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
                if (!retStr)
                {
                    cleanUp;
                    return NULL;
                }

                memcpy(retStr, "b", 2);
                if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
                {
                    cleanUp;
                    free(retStr);
                    return NULL;
                }

                last_type = cur_type;
            }

            tmpDouble = subfx_math_round(
                        cur_point.x *
                        handle->downscale *
                        handle->xscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpDouble = subfx_math_round(
                        cur_point.y *
                        handle->downscale *
                        handle->yscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpDouble = subfx_math_round(
                    points[i + 1].x *
                    handle->downscale *
                    handle->xscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpDouble = subfx_math_round(
                    points[i + 1].y *
                    handle->downscale *
                    handle->yscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpDouble = subfx_math_round(
                    points[i + 2].x *
                    handle->downscale *
                    handle->xscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpDouble = subfx_math_round(
                    points[i + 2].y *
                    handle->downscale *
                    handle->yscale, FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpDouble);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            i += 3;
            break;
        default: // invalid type (should never happen, but let us be safe)
            ++i;
            break;
        }

        if ((cur_type & 0x1) == 1) // odd = PT_CLOSEFIGURE
        {
            retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            memcpy(retStr, "c", 2);
            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }
        }
    } // end while (i < points_n)

    // clean up
    AbortPath(handle->dc);
    delete[] points;
    delete[] types;
    if (!charWidths)
    {
        delete[] charWidths;
    }
#else
    // Set text path to layout
    cairo_save(handle->context);
    cairo_scale(handle->context,
                handle->downscale *
                handle->xscale *
                handle->fonthack_scale,
                handle->downscale *
                handle->yscale *
                handle->fonthack_scale);
    pango_layout_set_text(handle->layout, text, -1);
    pango_cairo_layout_path(handle->context, handle->layout);
    cairo_restore(handle->context);

    // Convert path to shape
    cairo_path_t *path(cairo_copy_path(handle->context));
    if (!path)
    {
        return NULL;
    }

    if (path->status != CAIRO_STATUS_SUCCESS)
    {
        cairo_new_path(handle->context);
        cairo_path_destroy(path);
        return NULL;
    }

    fdsa_handle shape(fdsa->ptrVector->create(free));
    if (!shape)
    {
        cairo_new_path(handle->context);
        cairo_path_destroy(path);
        return NULL;
    }

    // may be larger or smaller?
    if (fdsa->ptrVector->reserve(shape, 2048) == fdsa_failed)
    {
        cairo_new_path(handle->context);
        cairo_path_destroy(path);
        return NULL;
    }

    int i(0), cur_type(0), last_type(99999); // first loop has no last_type
    double tmpValue(0.);
    while (i < path->num_data)
    {
        cur_type = path->data[i].header.type;
        switch (cur_type)
        {
        case CAIRO_PATH_MOVE_TO:
            if (cur_type != last_type)
            {
                retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
                if (!retStr)
                {
                    cleanUp;
                    return NULL;
                }

                memcpy(retStr, "m", 2);
                if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
                {
                    cleanUp;
                    free(retStr);
                    return NULL;
                }
            }

            tmpValue =
                    subfx_math_round(
                        path->data[i + 1].point.x,
                        FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 1].point.y,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            break;
        case CAIRO_PATH_LINE_TO:
            if (cur_type != last_type)
            {
                retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
                if (!retStr)
                {
                    cleanUp;
                    return NULL;
                }

                memcpy(retStr, "l", 2);
                if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
                {
                    cleanUp;
                    free(retStr);
                    return NULL;
                }
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 1].point.x,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 1].point.y,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            break;
        case CAIRO_PATH_CURVE_TO:
            if (cur_type != last_type)
            {
                retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
                if (!retStr)
                {
                    cleanUp;
                    return NULL;
                }

                memcpy(retStr, "b", 2);
                if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
                {
                    cleanUp;
                    free(retStr);
                    return NULL;
                }
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 1].point.x,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 1].point.y,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 2].point.x,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 2].point.y,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 3].point.x,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            tmpValue =
                    subfx_math_round(
                    path->data[i + 3].point.y,
                    FP_PRECISION);

            retStr = subfx_misc_doubleToString(tmpValue);
            if (!retStr)
            {
                cleanUp;
                return NULL;
            }

            if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
            {
                cleanUp;
                free(retStr);
                return NULL;
            }

            break;
        case CAIRO_PATH_CLOSE_PATH:
            if (cur_type != last_type)
            {
                retStr = reinterpret_cast<char *>(calloc(2, sizeof(char)));
                if (!retStr)
                {
                    cleanUp;
                    return NULL;
                }

                memcpy(retStr, "c", 2);
                if (fdsa->ptrVector->pushback(shape, retStr) == fdsa_failed)
                {
                    cleanUp;
                    free(retStr);
                    return NULL;
                }
            }

            break;
        default:
            break;
        }

        last_type = cur_type;
        i += path->data[i].header.length;
    }

    cairo_new_path(handle->context);
    cairo_path_destroy(path);
#endif

    // calculate how many buffer we need
    if (fdsa->ptrVector->size(shape, &retSize) == fdsa_failed)
    {
        subfx_pError(errMsg, "text_to_shape: you should"
                             " never see this message.");
        static_cast<void>(fdsa->closeHandle(shape));
        return NULL;
    }

    size_t length(0);
    for (size_t index = 0; index < retSize; ++index)
    {
        retStr = reinterpret_cast<char *>(fdsa->ptrVector->at(shape, index));
        if (!retStr)
        {
            subfx_pError(errMsg, "text_to_shape: you should"
                                 " never see this message.");
            static_cast<void>(fdsa->closeHandle(shape));
            return NULL;
        }

        length += strlen(retStr);
    }

    length += (retSize + 1); // for ' ' and '\0'

    retStr = reinterpret_cast<char *>(calloc(length, sizeof(char)));
    if (!retStr)
    {
        if (fdsa->closeHandle(shape) == fdsa_failed)
        {
            subfx_pError(errMsg, "text_to_shape: you should"
                                 " never see this message.");
        }

        return NULL;
    }

    char *pStr(retStr);
    char *tmpStr(NULL);
    for (size_t index = 0; index < retSize; ++index)
    {
        tmpStr = reinterpret_cast<char *>(fdsa->ptrVector->at(shape, index));
        if (!tmpStr)
        {
            free(retStr);
            subfx_pError(errMsg, "text_to_shape: you should"
                                 " never see this message.");
            static_cast<void>(fdsa->closeHandle(shape));
            return NULL;
        }

        length = strlen(tmpStr);
        memcpy(pStr, tmpStr, length);
        pStr += length;

        memcpy(pStr, " ", 1);
        ++pStr;
    }

    pStr[0] = '\0';

    if (fdsa->closeHandle(shape) == fdsa_failed)
    {
        subfx_pError(errMsg, "text_to_shape: you should"
                             " never see this message.");
        free(retStr);
        return NULL;
    }

    return retStr;
}

} // end extern "C"
