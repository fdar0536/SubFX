#include <vector>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <new>

#ifdef _WIN32
#include <cstring>

#include "boost/locale.hpp"
#endif

#include "YutilsCpp"

using namespace Yutils;

std::shared_ptr<FontHandle>
FontHandle::create(std::string &family,
                   bool bold,
                   bool italic,
                   bool underline,
                   bool strikeout,
                   int size,
                   double xscale,
                   double yscale,
                   double hspace) THROW
{
    if (size <= 0)
    {
        throw std::invalid_argument("FontHandle::create: size cannot lower than 0");
    }

    FontHandle *ret(new (std::nothrow) FontHandle(xscale, yscale, hspace));
    if (!ret)
    {
        return nullptr;
    }

#ifndef _WIN32
    int upscale(FONT_PRECISION);
#else
    ret->downscale = (1. / static_cast<double>(ret->upscale));
#endif

#ifdef _WIN32
    std::wstring family_dst(boost::locale::conv::utf_to_utf<wchar_t>(family));
    if (wcslen(family_dst.c_str()) > 31)
    {
        delete ret;
        throw std::invalid_argument("family name too long");
    }

    ret->dc = CreateCompatibleDC(nullptr);
    if (!ret->dc)
    {
        delete ret;
        return nullptr;
    }

    int res = SetMapMode(ret->dc, MM_TEXT);
    if (res == 0)
    {
        DeleteDC(ret->dc);
        delete ret;
        return nullptr;
    }

    res = SetBkMode(ret->dc, TRANSPARENT);
    if (res == 0)
    {
        DeleteDC(ret->dc);
        delete ret;
        return nullptr;
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
        delete ret;
        return nullptr;
    }

    ret->old_font = SelectObject(ret->dc, ret->font);
#else
    // This is almost copypasta from Youka/Yutils
    ret->surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 1, 1);
    if (!ret->surface)
    {
        delete ret;
        return nullptr;
    }

    ret->context = cairo_create(ret->surface);
    if (!ret->context)
    {
        cairo_surface_destroy(ret->surface);
        delete ret;
        return nullptr;
    }

    ret->layout = pango_cairo_create_layout(ret->context);
    if (!ret->layout)
    {
        cairo_destroy(ret->context);
        cairo_surface_destroy(ret->surface);
        delete ret;
        return nullptr;
    }

    //set font to layout
    PangoFontDescription *font_desc(pango_font_description_new());
    if (!font_desc)
    {
        g_object_unref(ret->layout);
        cairo_destroy(ret->context);
        cairo_surface_destroy(ret->surface);
        delete ret;
        return nullptr;
    }

    pango_font_description_set_family(font_desc, family.c_str());
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
        delete ret;
        return nullptr;
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
                              nullptr));
    if (!metrics)
    {
        pango_attr_list_unref(attr);
        pango_font_description_free(font_desc);
        g_object_unref(ret->layout);
        cairo_destroy(ret->context);
        cairo_surface_destroy(ret->surface);
        delete ret;
        return nullptr;
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

    return std::shared_ptr<FontHandle>(ret);
}

FontHandle::~FontHandle()
{
#ifdef _WIN32
    SelectObject(dc, old_font);
    DeleteObject(font);
    DeleteDC(dc);
#else
    g_object_unref(layout);
    cairo_destroy(context);
    cairo_surface_destroy(surface);
#endif
}

// public member function
std::map<std::string, double> FontHandle::metrics() NOTHROW
{
#ifdef _WIN32
    TEXTMETRICW  *fontMetrics(new (std::nothrow) TEXTMETRICW);
    if (!fontMetrics)
    {
        return std::map<std::string, double>();
    }

    if (GetTextMetricsW(dc, fontMetrics) == 0)
    {
        delete fontMetrics;
        return std::map<std::string, double>();
    }

    std::map<std::string, double> ret;
    ret["height"] = fontMetrics->tmHeight * downscale * yscale;
    ret["ascent"] = fontMetrics->tmAscent * downscale * yscale;
    ret["descent"] = fontMetrics->tmDescent * downscale * yscale;
    ret["internal_leading"] = fontMetrics->tmInternalLeading * downscale * yscale;
    ret["external_leading"] = fontMetrics->tmExternalLeading * downscale * yscale;

    delete fontMetrics;
#else
    PangoFontMetrics *fontMetrics(pango_context_get_metrics(
        pango_layout_get_context(layout),
        pango_layout_get_font_description(layout),
        nullptr
    ));

    if (!fontMetrics)
    {
        return std::map<std::string, double>();
    }

    double ascent(static_cast<double>(pango_font_metrics_get_ascent(fontMetrics)));
    ascent = ascent / PANGO_SCALE * downscale;

    double descent = static_cast<double>(pango_font_metrics_get_descent(fontMetrics));
    descent = descent / PANGO_SCALE * downscale;
    pango_font_metrics_unref(fontMetrics);

    std::map<std::string, double> ret;
    ret["height"] = (ascent + descent) * yscale * fonthack_scale;
    ret["ascent"] = ascent * yscale * fonthack_scale;
    ret["descent"] = descent * yscale * fonthack_scale;
    ret["internal_leading"] = 0.;
    ret["external_leading"] = pango_layout_get_spacing(layout) /
                              PANGO_SCALE * downscale * yscale * fonthack_scale;
#endif

    return ret;
}

std::map<std::string, double>
FontHandle::text_extents(std::string &text) NOTHROW
{
#ifdef _WIN32
    std::wstring textDst(boost::locale::conv::utf_to_utf<wchar_t>(text));
    size_t textLen = wcslen(textDst.c_str());

    SIZE *size(new (std::nothrow) SIZE);
    if (!size)
    {
        return std::map<std::string, double>();
    }

    if (GetTextExtentPoint32W(dc, textDst.c_str(),
                              static_cast<int>(textLen), size) == 0)
    {
        delete size;
        return std::map<std::string, double>();
    }
    std::map<std::string, double> ret;

    ret["width"] = (size->cx * downscale + hspace * textLen) * xscale;
    ret["height"] = size->cy * downscale * yscale;

    delete size;
#else
    pango_layout_set_text(layout, text.c_str(), -1);
    PangoRectangle *rect(new (std::nothrow) PangoRectangle);
    if (!rect)
    {
        return std::map<std::string, double>();
    }

    pango_layout_get_pixel_extents(layout, nullptr, rect);
    std::map<std::string, double> ret;
    ret["width"] = rect->width * downscale * xscale * fonthack_scale;
    ret["height"] = rect->height * downscale * yscale * fonthack_scale;

    delete rect;
#endif

    return ret;
}

std::string
FontHandle::text_to_shape(std::string &text) THROW
{
#ifdef _WIN32
    std::wstring textDst(boost::locale::conv::utf_to_utf<wchar_t>(text));
    size_t textLen(wcslen(textDst.c_str()));
    if (textLen > 8192)
    {
        throw std::invalid_argument("text_to_shape: text is too long");
    }

    INT *charWidths(nullptr);
    if (hspace != 0)
    {
        charWidths = new (std::nothrow) INT[textLen];
        if (!charWidths)
        {
            return std::string();
        }

        SIZE *size(new (std::nothrow) SIZE);
        if (!size)
        {
            delete[] charWidths;
            return std::string();
        }

        int space(static_cast<int>(hspace * upscale));
        for (size_t i = 0; i <= (textLen - 1); ++i)
        {
            if (GetTextExtentPoint32W(dc, textDst.c_str() + i, 1, size) == 0)
            {
                delete[] charWidths;
                delete size;
                return std::string();
            }

            charWidths[i] = size->cx + space;
        }

        delete size;
    } // end if (hspace != 0)

    if (BeginPath(dc) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(dc);
        return std::string();
    }

    if (ExtTextOutW(dc, 0, 0, 0x0, nullptr, textDst.c_str(),
                    static_cast<UINT>(textLen), charWidths) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(dc);
        return std::string();
    }

    if (EndPath(dc) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(dc);
        return std::string();
    }

    int points_n(GetPath(dc, nullptr, nullptr, 0));
    if (points_n <= 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(dc);
        return std::string();
    }

    POINT *points(new (std::nothrow) POINT[points_n]);
    if (!points)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(dc);
        return std::string();
    }

    BYTE *types(new (std::nothrow) BYTE[points_n]);
    if (!types)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        delete[] points;
        AbortPath(dc);
        return std::string();
    }

    GetPath(dc, points, types, points_n);

    std::vector<std::string> shape;
    shape.reserve(5000); // may be larger or smaller?

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
                shape.push_back("m");
                last_type = cur_type;
            }

            tmpDouble = round(cur_point.x * downscale * xscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            tmpDouble = round(cur_point.y * downscale * yscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));
            ++i;
            break;
        case PT_LINETO:
        case (PT_LINETO + PT_CLOSEFIGURE):
            if (last_type != PT_LINETO)
            {
                shape.push_back("l");
                last_type = cur_type;
            }

            tmpDouble = round(cur_point.x * downscale * xscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            tmpDouble = round(cur_point.y * downscale * yscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));
            ++i;
            break;
        case PT_BEZIERTO:
        case (PT_BEZIERTO + PT_CLOSEFIGURE):
            if (last_type != PT_BEZIERTO)
            {
                shape.push_back("b");
                last_type = cur_type;
            }

            tmpDouble = round(cur_point.x * downscale * xscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            tmpDouble = round(cur_point.y * downscale * yscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            tmpDouble = round(points[i + 1].x * downscale * xscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            tmpDouble = round(points[i + 1].y * downscale * yscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            tmpDouble = round(points[i + 2].x * downscale * xscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            tmpDouble = round(points[i + 2].y * downscale * yscale, FP_PRECISION);
            shape.push_back(doubleToString(tmpDouble));

            i += 3;
            break;
        default: // invalid type (should never happen, but let us be safe)
            ++i;
            break;
        }

        if ((cur_type & 0x1) == 1) // odd = PT_CLOSEFIGURE
        {
            shape.push_back("c");
        }
    } // end while (i < points_n)

    // clean up
    AbortPath(dc);
    delete[] points;
    delete[] types;
    if (!charWidths)
    {
        delete[] charWidths;
    }
#else
    // Set text path to layout
    cairo_save(context);
    cairo_scale(context,
    downscale * xscale * fonthack_scale, downscale * yscale * fonthack_scale);
    pango_layout_set_text(layout, text.c_str(), -1);
    pango_cairo_layout_path(context, layout);
    cairo_restore(context);

    // Convert path to shape
    cairo_path_t *path(cairo_copy_path(context));
    if (!path)
    {
        return std::string();
    }

    if (path->status != CAIRO_STATUS_SUCCESS)
    {
        cairo_new_path(context);
        cairo_path_destroy(path);
        return std::string();
    }

    std::vector<std::string> shape;
    shape.reserve(5000); // may be larger or smaller?
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
                shape.push_back("m");
            }

            tmpValue = round(path->data[i + 1].point.x, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));

            tmpValue = round(path->data[i + 1].point.y, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));
            break;
        case CAIRO_PATH_LINE_TO:
            if (cur_type != last_type)
            {
                shape.push_back("l");
            }

            tmpValue = round(path->data[i + 1].point.x, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));

            tmpValue = round(path->data[i + 1].point.y, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));
            break;
        case CAIRO_PATH_CURVE_TO:
            if (cur_type != last_type)
            {
                shape.push_back("b");
            }

            tmpValue = round(path->data[i + 1].point.x, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));

            tmpValue = round(path->data[i + 1].point.y, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));

            tmpValue = round(path->data[i + 2].point.x, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));

            tmpValue = round(path->data[i + 2].point.y, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));

            tmpValue = round(path->data[i + 3].point.x, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));

            tmpValue = round(path->data[i + 3].point.y, FP_PRECISION);
            shape.push_back(doubleToString(tmpValue));
            break;
        case CAIRO_PATH_CLOSE_PATH:
            if (cur_type != last_type)
            {
                shape.push_back("c");
            }
            break;
        default:
            break;
        }

        last_type = cur_type;
        i += path->data[i].header.length;
    }

    cairo_new_path(context);
    cairo_path_destroy(path);
#endif

    std::stringstream s;
    std::copy(shape.begin(), shape.end(),
              std::ostream_iterator<std::string>(s, " "));
    return s.str();
}
