/* 
 *  Copyright (C) 2009-2011 milkyjing <milkyjing@gmail.com>
 *                2019 fdar0536
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <algorithm>
#include <cwchar>
#include "utility.hpp"

using namespace std;

namespace util
{
    typedef text TEXT;
};

/**
 * pString->buf need to be allocated before being passed to the function, and it will be reallocated if more space is needed
 * pString->len and pString->capacity should also be specified before being passed to the function, and their value may be modified
 * step is the value that each time the capacity will be increased
 */
static void _tcfxlib_string_append(TCFX_pString pString, const wchar_t *ap, int apLen)
{
    wchar_t *wcs;
    int len, capacity;
    wcs = pString->buf;
    len = pString->len;
    capacity = pString->capacity;
    if (len + apLen + 1 > capacity)
    {  /* we need more space */
        while (len + apLen + 1 > capacity)
            capacity += capacity / 2 + 1;
        wcs = (wchar_t *)realloc(wcs, capacity * sizeof(wchar_t));
        pString->buf = wcs;
        pString->capacity = capacity;
    }

    memcpy(wcs + len, ap, apLen * sizeof(wchar_t));
    len += apLen;
    wcs[len] = L'\0';
    pString->len = len;
}

static int _outline_drawing_move_to(const FT_Vector *to, void *user)
{
    wchar_t buf[256];
    TCFX_pOutlineString pOutline = (TCFX_pOutlineString)user;
    swprintf(buf, 256,
            L"m %d %d ",
            pOutline->offset.x + to->x,
            pOutline->offset.y + pOutline->height - to->y);
    _tcfxlib_string_append(&pOutline->string, buf, wcslen(buf));
    pOutline->last.x = to->x;
    pOutline->last.y = to->y;
    return 0;
}

static int _outline_drawing_line_to(const FT_Vector *to, void *user)
{
    wchar_t buf[256];
    TCFX_pOutlineString pOutline = (TCFX_pOutlineString)user;
    swprintf(buf, 256,
            L"l %d %d ",
            pOutline->offset.x + to->x,
            pOutline->offset.y + pOutline->height - to->y);
    _tcfxlib_string_append(&pOutline->string, buf, wcslen(buf));
    pOutline->last.x = to->x;
    pOutline->last.y = to->y;
    return 0;
}

static int _outline_drawing_conic_to(const FT_Vector *control, const FT_Vector *to, void *user)
{
    wchar_t buf[256];
    int xs, ys, xc, yc, xe, ye, xc1, yc1, xc2, yc2;
    TCFX_pOutlineString pOutline = (TCFX_pOutlineString)user;
    xs = pOutline->last.x;
    ys = pOutline->last.y;
    xc = control->x;
    yc = control->y;
    xe = to->x;
    ye = to->y;
    xc1 = (xs + 2 * xc) / 3;
    yc1 = (ys + 2 * yc) / 3;
    xc2 = (xe + 2 * xc) / 3;
    yc2 = (ye + 2 * yc) / 3;
    swprintf(buf, 256,
            L"b %d %d %d %d %d %d ",
            pOutline->offset.x + xc1,
            pOutline->offset.y + pOutline->height - yc1,
            pOutline->offset.x + xc2,
            pOutline->offset.y + pOutline->height - yc2,
            pOutline->offset.x + xe,
            pOutline->offset.y + pOutline->height - ye);
    _tcfxlib_string_append(&pOutline->string, buf, wcslen(buf));
    pOutline->last.x = to->x;
    pOutline->last.y = to->y;
    return 0;
}

static int _outline_drawing_cubic_to(const FT_Vector *control1, const FT_Vector *control2,
                                     const FT_Vector *to, void *user)
{
    wchar_t buf[256];
    TCFX_pOutlineString pOutline = (TCFX_pOutlineString)user;
    swprintf(buf, 256,
            L"b %d %d %d %d %d %d ",
            pOutline->offset.x + control1->x,
            pOutline->offset.y + pOutline->height - control1->y,
            pOutline->offset.x + control2->x,
            pOutline->offset.y + pOutline->height - control2->y,
            pOutline->offset.x + to->x,
            pOutline->offset.y + pOutline->height - to->y);
    _tcfxlib_string_append(&pOutline->string, buf, wcslen(buf));
    pOutline->last.x = to->x;
    pOutline->last.y = to->y;
    return 0;
}

//TextOutlineDraw(pyFont, text, x, y)
TCFX_PyString utility::get_text_outline_as_string(text &pyFont, const char *text, int x, int y)
{
    TCFX_pFont pFont;
    const wchar_t *texts;
    wchar_t *drawing;
    int len;
    TCFX_PyString pyDrawing;

    pFont = pyFont.get_pFont();
    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();

    if (get_text_outline_drawing_string(pFont, texts[0], x, y, &drawing, &len) != 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "TextOutlineDraw error, failed to get the outline!\n");
        return py::str();
    }

    //convert to utf-8
    std::wstring drawing_src = std::wstring(drawing);
    std::string drawing_dst = boost::locale::conv::utf_to_utf<char>(drawing_src);
    pyDrawing = py::str(drawing_dst.c_str());

    delete[] drawing;
    return pyDrawing;
}

//TextOutlineDraw(font_file, face_id, font_size, text, x, y)
TCFX_PyString utility::get_text_outline_as_string_2(const char *font_file, int face_id, int font_size,
                                                    const char *text, int x, int y)
{
    const wchar_t *texts;
    TCFX_Font font;
    wchar_t *drawing;
    int len;
    TCFX_PyString pyDrawing;

    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();

    char *tmp_str = new char[strlen(font_file) + 1]();
    memcpy(tmp_str, font_file, strlen(font_file));
    tmp_str[strlen(font_file)] = '\0';
    util::TEXT Text;

    if (Text.init_font(&font, tmp_str, face_id, font_size, 0, 0, 0, 0, 0) != 0)
    {
        delete[] font.filename;
        PyErr_SetString(PyExc_RuntimeError, "TextOutlineDraw error, failed to initialize the font!\n");
        return py::str();
    }

    if (get_text_outline_drawing_string(&font, texts[0], x, y, &drawing, &len) != 0)
    {
        Text.fin_font(&font);
        delete[] font.filename;
        PyErr_SetString(PyExc_RuntimeError, "TextOutlineDraw error, failed to get the outline!\n");
        return py::str();
    }

    //convert to utf-8
    std::wstring drawing_src = std::wstring(drawing);
    std::string drawing_dst = boost::locale::conv::utf_to_utf<char>(drawing_src);
    pyDrawing = py::str(drawing_dst.c_str());

    Text.fin_font(&font);
    delete[] font.filename;
    delete[] drawing;
    return pyDrawing;
}

//IsCjk(text)
bool utility::is_c_or_j_or_k(const char *text)
{
    const wchar_t *texts;

    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();
    if (is_text_c_or_j_or_k(texts)) return true;
    else return false;
}

//VertLayout(text)
TCFX_PyString utility::vertical_layout_ass(const char *text)
{
    int count, len;
    const wchar_t *texts;
    wchar_t *textWithAssTags;
    TCFX_PyString pyTextWithAssTags;

    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();
    count = wcslen(texts);
    if (is_text_c_or_j_or_k(texts))
    {    /* Chinese or Japanese or Korea characters needn't be rotated */
        int i, offset;
        len = 1 + (count - 1) * 3;    /* T\\NT\\NT\0 */
        textWithAssTags = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
        offset = 0;
        textWithAssTags[offset] = text[0];
        offset++;
        for (i = 1; i < count; i++) {
            memcpy(textWithAssTags + offset, L"\\N", 2 * sizeof(wchar_t));
            offset += 2;
            textWithAssTags[offset] = text[i];
            offset++;
        }
        textWithAssTags[offset] = L'\0';
    }
    else
    {
        len = 9 + count;
        textWithAssTags = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
        memcpy(textWithAssTags, L"{\\frz270}", 9 * sizeof(wchar_t));
        memcpy(textWithAssTags + 9, text, count * sizeof(wchar_t));
        textWithAssTags[9 + count] = L'\0';
    }

    //convert to utf-8
    std::wstring ass_src = std::wstring(textWithAssTags);
    std::string ass_dst = boost::locale::conv::utf_to_utf<char>(ass_src);

    pyTextWithAssTags = py::str(ass_dst.c_str());
    delete[] textWithAssTags;
    return pyTextWithAssTags;
}

//ShowProgress(total, completed, file_id, file_num)
bool utility::show_progress(int total, int completed, int file_id, int file_num)
{
    printf("Progress: %.2f%c\r", 100 * (file_id + completed / (double)total) / file_num, '%');
    fflush(stdout);
    return true;
}

//Bezier1(nPoints, xs, ys, xe, ye)
TCFX_PyPoints utility::bezier_curve_linear(int points,
                                           double xs, double ys,
                                           double xe, double ye)
{   /* the return value has a pattern of ((x1, y1, a1), (x2, y2, a2), ...) */
    int i;
    double x, y, t;
    py::list pyPoints;
    py::tuple pyPointTemp;

    for (i = 0; i < points; ++i)
    {
        t = i / (double)(points - 1);
        /*x = (1 - t) * xs + t * xe;
        y = (1 - t) * ys + t * ye;*/
        linear_interpolation(&x, &y, xs, ys, xe, ye, t);
        pyPointTemp = py::make_tuple(x, y, 255);
        pyPoints.append(pyPointTemp);
    }
    return py::tuple(pyPoints);
}

//Bezier2(nPoints, xs, ys, xe, ye, xc, yc)
TCFX_PyPoints utility::bezier_curve_quadratic(int points,
                                              double xs, double ys,
                                              double xe, double ye,
                                              double xc, double yc)
{   /* the return value has a pattern of ((x1, y1, a1), (x2, y2, a2), ...) */
    int i;
    double x, y, t;
    py::list pyPoints;
    py::tuple pyPointTemp;

    for (i = 0; i < points; ++i)
    {
        t = i / (double)(points - 1);
        /*x = (1 - t) * (1 - t) * xs + 2 * t * (1 - t) * xc + t * t * xe;
        y = (1 - t) * (1 - t) * ys + 2 * t * (1 - t) * yc + t * t * ye;*/
        conic_bezier(&x, &y, xs, ys, xc, yc, xe, ye, t);
        pyPointTemp = py::make_tuple(x, y, 255);
        pyPoints.append(pyPointTemp);
    }

    return py::tuple(pyPoints);
}

//Bezier3(nPoints, xs, ys, xe, ye, xc1, yc1, xc2, yc2)
TCFX_PyPoints utility::bezier_curve_cubic(int points,
                                          double xs, double ys,
                                          double xe, double ye,
                                          double xc1, double yc1,
                                          double xc2, double yc2)
{   /* the return value has a pattern of ((x1, y1, a1), (x2, y2, a2), ...) */
    int i;
    double x, y, t;
    py::list pyPoints;
    py::tuple pyPointTemp;

    for (i = 0; i < points; ++i)
    {
        t = i / (double)(points - 1);
        /*x = (1 - t) * (1 - t) * (1 - t) * xs + 3 * t * (1 - t) * (1 - t) * xc1 + 3 * t * t * (1 - t) * xc2 + t * t * t * xe;
        y = (1 - t) * (1 - t) * (1 - t) * ys + 3 * t * (1 - t) * (1 - t) * yc1 + 3 * t * t * (1 - t) * yc2 + t * t * t * ye;*/
        cubic_bezier(&x, &y, xs, ys, xc1, yc1, xc2, yc2, xe, ye, t);
        pyPointTemp = py::make_tuple(x, y, 255);
        pyPoints.append(pyPointTemp);
    }
    return py::tuple(pyPoints);
}

//BezierN(nPoints, xs, ys, xe, ye, xl1, yl1, xl2, yl2, order)
TCFX_PyPoints utility::bezier_curve_random(int points,
                                           double xs, double ys,
                                           double xe, double ye,
                                           double xl1, double yl1,
                                           double xl2, double yl2,
                                           int order)
{   /* the return value has a pattern of ((x1, y1, a1), (x2, y2, a2), ...) */
    int i, j;
    double x, y, t;
    int x1, y1, x2, y2;
    double *xptr, *yptr;
    py::list pyPoints;
    py::tuple pyPointTemp;

    x1 = min(xl1, xl2);
    y1 = min(yl1, yl2);
    x2 = max(xl1, xl2);
    y2 = max(yl1, yl2);
    xptr = (double *)malloc((order + 1) * sizeof(double));
    yptr = (double *)malloc((order + 1) * sizeof(double));
    xptr[0] = xs;
    yptr[0] = ys;
    xptr[order] = xe;
    xptr[order] = ye;
    for (i = 1; i < order; ++i)
    {
        xptr[i] = _Randint(x1, x2);
        yptr[i] = _Randint(y1, y2);
    }

    for (i = 0; i < points; ++i)
    {
        x = 0;
        y = 0;
        t = i / (double)(points - 1);
        for (j = 0; j < order + 1; ++j)
        {
            x += _Combi(order, j) * pow(1 - t, order - j) * pow(t, j) * xptr[j];
            y += _Combi(order, j) * pow(1 - t, order - j) * pow(t, j) * yptr[j];
        }

        pyPointTemp = py::make_tuple(x, y, 255);
        pyPoints.append(pyPointTemp);
    }

    delete[] xptr;
    delete[] yptr;
    return py::tuple(pyPoints);
}

//GetFontSize(font_file, face_id, font_size)
float utility::get_actual_font_size(const char *font_file, int face_id, int font_size)
{
    FT_Library library;
    FT_Face face;
    TT_HoriHeader *hori;
    TT_OS2 *os2;
    double scale;

    if (FT_Init_FreeType(&library) != 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "GetFontSize error, failed to initialize the font.\n");
        return 0;
    }
    if (FT_New_Face(library, font_file, face_id, &face) != 0)
    {
        FT_Done_FreeType(library);
        PyErr_SetString(PyExc_RuntimeError, "GetFontSize error, failed to initialize the font.\n");
        return 0;
    }

    hori = (TT_HoriHeader *)FT_Get_Sfnt_Table(face, ft_sfnt_hhea);
    os2 = (TT_OS2 *)FT_Get_Sfnt_Table(face, ft_sfnt_os2);
    scale = 1;
    /* VSFilter uses metrics from TrueType OS/2 table, The idea was borrowed from libass */
    if (hori && os2)
    {
        int hori_height;
        int os2_height;
        hori_height = hori->Ascender - hori->Descender;
        os2_height = os2->usWinAscent + os2->usWinDescent;
        if (hori_height && os2_height)
            scale = hori_height / (double)os2_height;
    }
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return (font_size * scale);
}

//CairoFontSize(font_file, face_id, font_size)
int utility::calc_cairo_font_size(const char *font_file, int face_id, int font_size)
{
    FT_Library library;
    FT_Face face;
    TT_HoriHeader *hori;
    TT_OS2 *os2;
    double scale;
    FT_Size_RequestRec rq;
    int fontSize2;  /* the NOMINAL font size */

    if (FT_Init_FreeType(&library) != 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "CairoFontSize error, failed to initialize the font.\n");
        return 0;
    }
    if (FT_New_Face(library, font_file, face_id, &face) != 0)
    {
        FT_Done_FreeType(library);
        PyErr_SetString(PyExc_RuntimeError, "CairoFontSize error, failed to initialize the font.\n");
        return 0;
    }

    hori = (TT_HoriHeader *)FT_Get_Sfnt_Table(face, ft_sfnt_hhea);
    os2 = (TT_OS2 *)FT_Get_Sfnt_Table(face, ft_sfnt_os2);
    scale = 1;
    /* VSFilter uses metrics from TrueType OS/2 table, The idea was borrowed from libass */
    if (hori && os2)
    {
        int hori_height;
        int os2_height;
        hori_height = hori->Ascender - hori->Descender;
        os2_height = os2->usWinAscent + os2->usWinDescent;
        if (hori_height && os2_height)
            scale = hori_height / (double)os2_height;
    }
    rq.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
    rq.width = 0;
    rq.height = (long)(font_size * scale * 64);
    rq.horiResolution = 0;
    rq.vertResolution = 0;
    FT_Request_Size(face, &rq);
    fontSize2 = (int)(font_size * scale * face->units_per_EM / (double)(face->ascender - face->descender) + 0.5);
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return fontSize2;
}

/* private */
int utility::get_text_outline_drawing_string(const TCFX_pFont pFont, wchar_t text, int x, int y,
                                             wchar_t **pDrawing, int *pLen)
{
    FT_UInt index;
    FT_Glyph glyph;
    FT_OutlineGlyph outline;
    FT_Outline_Funcs outInterface;
    TCFX_OutlineString outlineString;
    /* get parameters from script */
    index = FT_Get_Char_Index(pFont->face, text);
    if (0 == index)
    {
        printf("TextOutlineDraw Error: find an invalid text that does not exists in the font.\n");
        return -1;
    }
    FT_Load_Glyph(pFont->face, index, FT_LOAD_DEFAULT);
    FT_Get_Glyph(pFont->face->glyph, &glyph);
    if (FT_GLYPH_FORMAT_OUTLINE != glyph->format)
    {
        printf("TextOutlineDraw Error: cannot get the outline of the text.\n");
        return -1;
    }
    outline = (FT_OutlineGlyph)glyph;
    outlineString.offset.x = x * 64;
    outlineString.offset.y = y * 64;
    outlineString.height = (pFont->size << 6) + pFont->face->size->metrics.descender;
    outlineString.last.x = 0;
    outlineString.last.y = 0;
    outlineString.string.len = 0;
    outlineString.string.capacity = 10;
    outlineString.string.buf = (wchar_t *)malloc(outlineString.string.capacity * sizeof(wchar_t));
    outInterface.move_to = (FT_Outline_MoveToFunc)_outline_drawing_move_to;
    outInterface.line_to = (FT_Outline_LineToFunc)_outline_drawing_line_to;
    outInterface.conic_to = (FT_Outline_ConicToFunc)_outline_drawing_conic_to;
    outInterface.cubic_to = (FT_Outline_CubicToFunc)_outline_drawing_cubic_to;
    outInterface.shift = 0;
    outInterface.delta = 0;
    FT_Outline_Decompose(&outline->outline, &outInterface, &outlineString);
    _tcfxlib_string_append(&outlineString.string, L"c", 1);
    FT_Done_Glyph(glyph);
    *pDrawing = outlineString.string.buf;
    *pLen = outlineString.string.len;
    return 0;
}

int utility::is_text_c_or_j_or_k(const wchar_t *text)
{
    int i, count;
    count = wcslen(text);
    for (i = 0; i < count; i++)
    {
        if (text[0] >= 0x3000 && text[1] <= 0x9FFF) return 1;
    }
    return 0;
}

long utility::_Fac(int n)
{
    int i;
    long N = 1;
    for (i = 2; i <= n; i++)
        N *= i;
    return N;
}

long utility::_Combi(int n, int m)
{
    return _Fac(n) / (_Fac(n - m) * _Fac(m));
}

int utility::_Randint(int r1, int r2)
{
    if (r1 > r2) return r2 + rand() % (r1 - r2 + 1);
    return r1 + rand() % (r2 - r1 + 1);
}
