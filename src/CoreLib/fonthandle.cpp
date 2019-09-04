#include <vector>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <sstream>

#ifdef _WIN32
#include <cstring>

#include "boost/locale.hpp"
#endif

#include "fonthandle.hpp"

using namespace std;

FontHandle::FontHandle(string &family, bool bold, bool italic, bool underline,
    bool strikeout, int size,
    double xscale, double yscale, double hspace) :
#ifdef _WIN32
    dc(nullptr),
    font(nullptr),
    old_font(nullptr),
    hspace(hspace),
    upscale(FONT_PRECISION),
#else
    surface(nullptr),
    context(nullptr),
    layout(nullptr),
#endif
    xscale(xscale),
    yscale(yscale)
{
    CoreMath();
    if (size <= 0)
    {
        throw invalid_argument("size cannot lower than 0");
    }

#ifndef _WIN32
    int upscale(FONT_PRECISION);
#endif
    downscale = (1.f / static_cast<double>(upscale));

#ifdef _WIN32
    wstring family_dst(boost::locale::conv::utf_to_utf<wchar_t>(family));
    if (wcslen(family_dst.c_str()) > 31)
    {
        throw invalid_argument("family name to long");
    }

    dc = CreateCompatibleDC(nullptr);
    if (!dc)
    {
        throw runtime_error("CANNOT create DC");
    }

    int ret = SetMapMode(dc, MM_TEXT);
    if (ret == 0)
    {
        throw runtime_error("Fail to SetMapMode");
    }

    ret = SetBkMode(dc, TRANSPARENT);
    if (ret == 0)
    {
        throw runtime_error("Fail to SetBkMode");
    }

    font = CreateFontW(
        size * upscale, // nHeight
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

    if (!font)
    {
        throw runtime_error("CANNOT create font");
    }

    old_font = SelectObject(dc, font);
#else
    // This is almost copypasta from Youka/Yutils
    surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 1, 1);
    if (!surface)
    {
        throw runtime_error("CANNOT create cairo_surface_t");
    }

    context = cairo_create(surface);
    if (!context)
    {
        cairo_surface_destroy(surface);
        throw runtime_error("CANNOT create cairo_t");
    }

    layout = pango_cairo_create_layout(context);
    if (!layout)
    {
        cairo_destroy(context);
        cairo_surface_destroy(surface);
        throw runtime_error("CANNOT create PangoLayout");
    }

    //set font to layout
    PangoFontDescription *font_desc(pango_font_description_new());
    if (!font_desc)
    {
        g_object_unref(layout);
        cairo_destroy(context);
        cairo_surface_destroy(surface);
        throw runtime_error("CANNOT create PangoFontDescription");
    }

    pango_font_description_set_family(font_desc, family.c_str());
    pango_font_description_set_weight(font_desc,
                                      bold ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
    pango_font_description_set_style(font_desc,
                                     italic ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL);
    pango_font_description_set_absolute_size(font_desc,
                                             size * PANGO_SCALE * upscale);
    pango_layout_set_font_description(layout, font_desc);
    
    PangoAttrList *attr(pango_attr_list_new());
    if (!attr)
    {
        pango_font_description_free(font_desc);
        g_object_unref(layout);
        cairo_destroy(context);
        cairo_surface_destroy(surface);
        throw runtime_error("CANNOT create PangoAttrList");
    }

    pango_attr_list_insert(attr,
        pango_attr_underline_new(
            underline ? PANGO_UNDERLINE_SINGLE : PANGO_UNDERLINE_NONE));
    pango_attr_list_insert(attr,
                            pango_attr_strikethrough_new(strikeout));
    pango_attr_list_insert(attr,
                           pango_attr_letter_spacing_new(
                               hspace * PANGO_SCALE * upscale));
    pango_layout_set_attributes(layout, attr);

    PangoFontMetrics *metrics(pango_context_get_metrics(
                              pango_layout_get_context(layout), pango_layout_get_font_description(layout),
                              nullptr));
    if (!metrics)
    {
        pango_attr_list_unref(attr);
        pango_font_description_free(font_desc);
        g_object_unref(layout);
        cairo_destroy(context);
        cairo_surface_destroy(surface);
        throw runtime_error("CANNOT create PangoFontMetrics");
    }

    double ascent(static_cast<double>(pango_font_metrics_get_ascent(metrics)));
    double descent(static_cast<double>(pango_font_metrics_get_descent(metrics)));

    fonthack_scale = size / ((ascent + descent) / (double)PANGO_SCALE * downscale);
    
    pango_font_metrics_unref(metrics);
    pango_attr_list_unref(attr);
    pango_font_description_free(font_desc);
#endif
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
map<string, double> FontHandle::metrics()
{
#ifdef _WIN32
    TEXTMETRICW *fontMetrics(nullptr);
    try
    {
        fontMetrics = new TEXTMETRICW;
    }
    catch (...)
    {
        throw runtime_error("CANNOT create TEXTMETRICW");
    }

    if (GetTextMetricsW(dc, fontMetrics) == 0)
    {
        delete fontMetrics;
        throw runtime_error("Fail to GetTextMetricsW");
    }

    map<string, double> ret;
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
        throw runtime_error("CANNOT create PangoFontMetrics");
    }

    double ascent(static_cast<double>(pango_font_metrics_get_ascent(fontMetrics)));
    ascent = ascent / PANGO_SCALE * downscale;

    double descent = static_cast<double>(pango_font_metrics_get_descent(fontMetrics));
    descent = descent / PANGO_SCALE * downscale;
    pango_font_metrics_unref(fontMetrics);

    map<string, double> ret;
    ret["height"] = (ascent + descent) * yscale * fonthack_scale;
    ret["ascent"] = ascent * yscale * fonthack_scale;
    ret["descent"] = descent * yscale * fonthack_scale;
    ret["internal_leading"] = 0.f;
    ret["external_leading"] = pango_layout_get_spacing(layout) / PANGO_SCALE * downscale * yscale * fonthack_scale;
#endif

    return ret;
}

map<string, double> FontHandle::text_extents(string &text)
{
#ifdef _WIN32
    wstring textDst(boost::locale::conv::utf_to_utf<wchar_t>(text));
    size_t textLen = wcslen(textDst.c_str());

    SIZE *size(nullptr);
    try
    {
        size = new SIZE;
    }
    catch (...)
    {
        throw runtime_error("CANNOT allocate SIZE");
    }

    if (GetTextExtentPoint32W(dc, textDst.c_str(), textLen, size) == 0)
    {
        delete size;
        throw runtime_error("Fail to GetTextExtentPoint32W");
    }
    map<string, double> ret;

    ret["width"] = (size->cx * downscale + hspace * textLen) * xscale;
    ret["height"] = size->cy * downscale * yscale;

    delete size;
#else
    pango_layout_set_text(layout, text.c_str(), -1);
    PangoRectangle *rect(nullptr);
    try
    {
        rect = new PangoRectangle;
    }
    catch(...)
    {
        throw runtime_error("CANNOT create PangoRectangle");
    }

    pango_layout_get_pixel_extents(layout, nullptr, rect);
    map<string, double> ret;
    ret["width"] = rect->width * downscale * xscale * fonthack_scale;
    ret["height"] = rect->height * downscale * yscale * fonthack_scale;

    delete rect;
#endif

    return ret;
}

string FontHandle::text_to_shape(string &text)
{
#ifdef _WIN32
    wstring textDst(boost::locale::conv::utf_to_utf<wchar_t>(text));
    size_t textLen(wcslen(textDst.c_str()));
    if (textLen > 8192)
    {
        throw invalid_argument("text too long");
    }

    INT *charWidths(nullptr);
    if (hspace != 0)
    {
        try
        {
            charWidths = new INT[textLen];
        }
        catch (...)
        {
            throw runtime_error("CANNOT allocate INT array");
        }

        SIZE *size(nullptr);
        try
        {
            size = new SIZE;
        }
        catch (...)
        {
            delete[] charWidths;
            throw runtime_error("CANNOT allocate SIZE");
        }

        int space(hspace * upscale);
        for (size_t i = 0; i <= (textLen - 1); ++i)
        {
            if (GetTextExtentPoint32W(dc, textDst.c_str() + i, 1, size) == 0)
            {
                delete[] charWidths;
                delete size;
                throw runtime_error("Fail to GetTextExtentPoint32W");
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

        throw runtime_error("Fail to BeginPath");
    }


    if (ExtTextOutW(dc, 0, 0, 0x0, nullptr, textDst.c_str(), textLen, charWidths) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        throw runtime_error("Fail to ExtTextOutW");
    }

    if (EndPath(dc) == 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        throw runtime_error("Fail to EndPath");
    }

    int points_n(GetPath(dc, nullptr, nullptr, 0));
    if (points_n <= 0)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }

        AbortPath(dc);
        return string();
    }

    POINT *points(nullptr);
    try
    {
        points = new POINT[points_n];
    }
    catch (...)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }
        throw runtime_error("Fail to allocate POINT array");
    }

    BYTE *types(nullptr);
    try
    {
        types = new BYTE[points_n];
    }
    catch (...)
    {
        if (!charWidths)
        {
            delete[] charWidths;
        }
        throw runtime_error("Fail to allocate BYTE array");
    }

    GetPath(dc, points, types, points_n);

    vector<string> shape;
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
        throw runtime_error("CANNOT create cairo_path_t");
    }

    if (path->status != CAIRO_STATUS_SUCCESS)
    {
        cairo_new_path(context);
        cairo_path_destroy(path);
        return string();
    }

    vector<string> shape;
    shape.reserve(5000); // may be larger or smaller?
    int i(0), cur_type(0), last_type(99999); // first loop has no last_type
    double tmpValue(0.f);
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
        default:
            break;
        }

        last_type = cur_type;
        i += path->data[i].header.length;
    }

    cairo_new_path(context);
    cairo_path_destroy(path);
#endif

    stringstream s;
    copy(shape.begin(), shape.end(), ostream_iterator<string>(s, " "));
    return s.str();
}