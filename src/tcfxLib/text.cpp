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
#include "text.hpp"

using namespace std;

/* Bezier Curve Evaluation using De Casteljau's Algorithm */
void linear_interpolation(double *pxt, double *pyt,
                          double xa, double ya,
                          double xb, double yb,
                          double t)
{
    *pxt = xa + (xb - xa) * t;
    *pyt = ya + (yb - ya) * t;
}

void conic_bezier(double *pxt, double *pyt,
                  double xs, double ys,
                  double xc, double yc,
                  double xe, double ye,
                  double t)
{
    double xsc, ysc, xce, yce;
    linear_interpolation(&xsc, &ysc, xs, ys, xc, yc, t);
    linear_interpolation(&xce, &yce, xc, yc, xe, ye, t);
    linear_interpolation(pxt, pyt, xsc, ysc, xce, yce, t);
}

void cubic_bezier(double *pxt, double *pyt,
                  double xs, double ys,
                  double xc1, double yc1,
                  double xc2, double yc2,
                  double xe, double ye,
                  double t)
{
    double x10, y10, x11, y11, x12, y12, x20, y20, x21, y21;    /* 00 = s, 01 = c1, 02 = c2, 03 = e */
    linear_interpolation(&x10, &y10, xs, ys, xc1, yc1, t);
    linear_interpolation(&x11, &y11, xc1, yc1, xc2, yc2, t);
    linear_interpolation(&x12, &y12, xc2, yc2, xe, ye, t);
    linear_interpolation(&x20, &y20, x10, y10, x11, y11, t);
    linear_interpolation(&x21, &y21, x11, y11, x12, y12, t);
    linear_interpolation(pxt, pyt, x20, y20, x21, y21, t);
}

double _max_distance_3(double x1, double x2, double x3)
{
    double left, right;
    left = x1;
    if (left > x2)
        left = x2;
    if (left > x3)
        left = x3;
    right = x3;
    if (right < x2)
        right = x2;
    if (right < x1)
        right = x1;
    return right - left;
}

double _max_distance_4(double x1, double x2, double x3, double x4)
{
    double left, right;
    left = x1;
    if (left > x2)
        left = x2;
    if (left > x3)
        left = x3;
    if (left > x4)
        left = x4;
    right = x4;
    if (right < x3)
        right = x3;
    if (right < x2)
        right = x2;
    if (right < x1)
        right = x1;
    return right - left;
}

int _outline_points_move_to(const FT_Vector *to, void *user)
{
    pix Pix;
    TCFX_pOutlinePoints pOutline = (TCFX_pOutlinePoints)user;
    Pix.points_append(&pOutline->points, to->x / 64.0, pOutline->height - to->y / 64.0, 255);
    pOutline->lastX = to->x / 64.0;
    pOutline->lastY = to->y / 64.0;
    return 0;
}

int _outline_points_line_to(const FT_Vector *to, void *user)
{
    pix Pix;
    double xs, ys, xe, ye, x, y, t, step;
    /* int i, points; */
    TCFX_pOutlinePoints pOutline = (TCFX_pOutlinePoints)user;
    xs = pOutline->lastX;
    ys = pOutline->lastY;
    xe = to->x / 64.0;
    ye = to->y / 64.0;
    step = 1 / (max(abs(xe - xs), abs(ye - ys)) * pOutline->density);
    for (t = step; t < 1 + step; t += step) {
        linear_interpolation(&x, &y, xs, ys, xe, ye, t);
        Pix.points_append(&pOutline->points, x, pOutline->height - y, 255);
    }
    pOutline->lastX = xe;
    pOutline->lastY = ye;
    return 0;
}

int _outline_points_conic_to(const FT_Vector *control, const FT_Vector *to, void *user)
{
    pix Pix;
    double xs, ys, xc, yc, xe, ye, x, y, t, step;
    /* int i, points; */
    TCFX_pOutlinePoints pOutline = (TCFX_pOutlinePoints)user;
    xs = pOutline->lastX;
    ys = pOutline->lastY;
    xc = control->x / 64.0;
    yc = control->y / 64.0;
    xe = to->x / 64.0;
    ye = to->y / 64.0;
    step = 1 / (max(_max_distance_3(xs, xc, xe), _max_distance_3(ys, yc, ye)) * pOutline->density);
    for (t = step; t < 1 + step; t += step) {
        conic_bezier(&x, &y, xs, ys, xc, yc, xe, ye, t);
        Pix.points_append(&pOutline->points, x, pOutline->height - y, 255);
    }

    pOutline->lastX = xe;
    pOutline->lastY = ye;
    return 0;
}

int _outline_points_cubic_to(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user)
{
    pix Pix;
    double xs, ys, xc1, yc1, xc2, yc2, xe, ye, x, y, t, step;
    TCFX_pOutlinePoints pOutline = (TCFX_pOutlinePoints)user;
    xs = pOutline->lastX;
    ys = pOutline->lastY;
    xc1 = control1->x / 64.0;
    yc1 = control1->y / 64.0;
    xc2 = control2->x / 64.0;
    yc2 = control2->y / 64.0;
    xe = to->x / 64.0;
    ye = to->y / 64.0;
    step = 1 / (max(_max_distance_4(xs, xc1, xc2, xe), _max_distance_4(ys, yc1, yc2, ye)) * pOutline->density);
    for (t = step; t < 1 + step; t += step) {
        cubic_bezier(&x, &y, xs, ys, xc1, yc1, xc2, yc2, xe, ye, t);
        Pix.points_append(&pOutline->points, x, pOutline->height - y, 255);
    }
    pOutline->lastX = xe;
    pOutline->lastY = ye;
    return 0;
}

text::text(const char *font_file, int face_id, int font_size, int spacing,
           double space_scale, uint32_t color, int bord, int is_outline)
    : pFont(new TCFX_Font)
{
    common();

    char *tmp_str = new char[strlen(font_file) + 1]();
    memcpy(tmp_str, font_file, strlen(font_file));
    tmp_str[strlen(font_file)] = '\0';

    if (init_font(pFont, tmp_str, face_id, font_size, spacing,
                  space_scale, color, bord, is_outline) != 0)
    {
        delete[] pFont->filename;
        pFont->filename = nullptr;
        delete pFont;
        pFont = nullptr;
        success = false;
        PyErr_SetString(PyExc_RuntimeError, "TextPix error, failed to initialize the font!\n");
    }

    success = true;
}

text::~text()
{
    fin_font(this->pFont);
    if (pFont != nullptr)
    {
        delete[] pFont->filename;
        delete pFont;
        pFont = nullptr;
    }
}

//TextPix(pyFont, texts)
TCFX_PyPix text::get_pix_from_text(const char *texts)
{
    TCFX_Pix pix;
    const wchar_t *text;

    std::string src_string = std::string(texts);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    text = dst_string.c_str();
    if (wcslen(text) <= 1)
    {
        if (get_text_pix(this->pFont, text[0], &pix) != 0)
        {
            PyErr_SetString(PyExc_RuntimeError, "TextPix error, failed to get the PIX!\n");
            return py::tuple();
        }
    }
    else
    {
        if (get_texts_pix(this->pFont, text, &pix) != 0)
        {
            PyErr_SetString(PyExc_RuntimeError, "TextPix error, failed to get the PIX!\n");
            return py::tuple();
        }
    }

    return Pix.convert_pix(&pix, 1);
}

TCFX_PyPix text::get_pix_from_text_2(const char *font_file, int face_id, int font_size, int spacing,
                                     double space_scale, uint32_t color, int bord, int is_outline,
                                     const char *texts)
{
    TCFX_Font font;
    TCFX_Pix pix;
    const wchar_t *text;
    std::string src_string = std::string(texts);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    text = dst_string.c_str();

    char *tmp_str = new char[strlen(font_file) + 1]();
    memcpy(tmp_str, font_file, strlen(font_file));
    tmp_str[strlen(font_file)] = '\0';

    if (init_font(&font, tmp_str, face_id, font_size, spacing, space_scale, color, bord, is_outline) != 0)
    {
        delete[] font.filename;
        PyErr_SetString(PyExc_RuntimeError, "TextPix error, failed to initialize the font!\n");
        return py::tuple();
    }
    if (wcslen(text) <= 1)
    {
        if (get_text_pix(&font, text[0], &pix) != 0)
        {
            delete[] font.filename;
            fin_font(&font);
            PyErr_SetString(PyExc_RuntimeError, "TextPix error, failed to get the PIX!\n");
            return py::tuple();
        }
    }
    else
    {
        if (get_texts_pix(&font, text, &pix) != 0)
        {
            delete[] font.filename;
            fin_font(&font);
            PyErr_SetString(PyExc_RuntimeError, "TextPix error, failed to get the PIX!\n");
            return py::tuple();
        }
    }

    fin_font(&font);
    delete[] font.filename;
    return Pix.convert_pix(&pix, 1);
}

//TextOutlinePoints(pyFont, text, density)
TCFX_PyPoints text::get_text_outline_as_points(const char *text, double density)
{
    const wchar_t *texts;
    double *xBuf, *yBuf;
    unsigned char *aBuf;
    int i, count;
    py::list pyPoints;
    py::tuple pyPointTemp;

    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();

    if (get_text_outline_points(pFont, texts[0], density, &xBuf, &yBuf, &aBuf, &count) != 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "TextOutlinePoints error, failed to get the outline!\n");
        return py::tuple();
    }

    for (i = 0; i < count; ++i)
    {
        pyPointTemp = py::make_tuple(xBuf[i], yBuf[i], aBuf[i]);
        pyPoints.append(pyPointTemp);
    }

    delete xBuf;
    delete yBuf;
    delete aBuf;
    return py::tuple(pyPoints);
}

//TextOutlinePoints(font_file, face_id, font_size, text, density)
TCFX_PyPoints text::get_text_outline_as_points_2(const char *font_file, int face_id, int font_size,
                                                 const char *text, double density)
{
    const wchar_t *texts;
    TCFX_Font font;
    double *xBuf, *yBuf;
    unsigned char *aBuf;
    int i, count;
    py::list pyPoints;
    py::tuple pyPointTemp;

    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();

    char *tmp_str = new char[strlen(font_file) + 1]();
    memcpy(tmp_str, font_file, strlen(font_file));
    tmp_str[strlen(font_file)] = '\0';

    if (init_font(&font, tmp_str, face_id, font_size, 0, 0, 0, 0, 0) != 0)
    {
        delete[] font.filename;
        PyErr_SetString(PyExc_RuntimeError, "TextOutlinePoints error, failed to initialize the font!\n");
        return py::tuple();
    }

    if (get_text_outline_points(&font, texts[0], density, &xBuf, &yBuf, &aBuf, &count) != 0)
    {
        fin_font(&font);
        delete[] font.filename;
        PyErr_SetString(PyExc_RuntimeError, "TextOutlinePoints error, failed to get the outline!\n");
        return py::tuple();
    }

    fin_font(&font);
    delete[] font.filename;

    for (i = 0; i < count; i++)
    {
        pyPointTemp = py::make_tuple(xBuf[i], yBuf[i], aBuf[i]);
        pyPoints.append(pyPointTemp);
    }

    delete xBuf;
    delete yBuf;
    delete aBuf;
    return py::tuple(pyPoints);
}

//TextMetrics(pyFont, text)
TCFX_PyTextMetrics text::get_text_metrics(const char *text)
{
    TCFX_TextMetrics textMetrics;
    const wchar_t *texts;

    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();

    if (_tcfx_lib_get_text_metrics(pFont, texts, &textMetrics) != 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "TextMetrics error, failed to get the text metrics!\n");
        return py::tuple();
    }

    return convert_text_metrics(&textMetrics);
}

//TextMetrics(font_file, face_id, font_size, spacing, space_scale, text)
TCFX_PyTextMetrics text::get_text_metrics_2(const char *font_file, int face_id, int font_size, int spacing,
                                            double space_scale, const char *text)
{
    const wchar_t *texts;
    TCFX_Font font;
    TCFX_TextMetrics textMetrics;

    std::string src_string = std::string(text);
    std::wstring dst_string = boost::locale::conv::utf_to_utf<wchar_t>(src_string);
    texts = dst_string.c_str();

    char *tmp_str = new char[strlen(font_file) + 1]();
    memcpy(tmp_str, font_file, strlen(font_file));
    tmp_str[strlen(font_file)] = '\0';

    if (init_font(&font, tmp_str, face_id, font_size, spacing, space_scale, 0, 0, 0) != 0)
    {
        delete[] font.filename;
        PyErr_SetString(PyExc_RuntimeError, "TextMetrics error, failed to initialize the font!\n");
        return py::tuple();
    }
    if (_tcfx_lib_get_text_metrics(&font, texts, &textMetrics) != 0)
    {
        delete[] font.filename;
        PyErr_SetString(PyExc_RuntimeError, "TextMetrics error, failed to get the text metrics!\n");
        return py::tuple();
    }

    fin_font(&font);
    delete[] font.filename;
    return convert_text_metrics(&textMetrics);
}

int text::init_font(TCFX_pFont pFont, char *fontFilename, int fontFaceID, int fontSize, int spacing,
                    double spaceScale, uint32_t color, int bord, int is_outline)
{
    pFont->filename = fontFilename;
    pFont->id = fontFaceID;
    pFont->size = fontSize;
    pFont->spacing = spacing;
    pFont->spaceScale = spaceScale;
    pFont->color = color;
    pFont->bord = bord;
    pFont->is_outline = is_outline;
    if (FT_Init_FreeType(&pFont->library) != 0)
    {
        return -1;
    }

    if (FT_New_Face(pFont->library, pFont->filename, pFont->id, &pFont->face) != 0)
    {
        FT_Done_FreeType(pFont->library);
        return -1;
    }
    /* FT_Select_Charmap(pFont->face, FT_ENCODING_UNICODE); */
    if (!pFont->face->charmap)
    {
        FT_Done_Face(pFont->face);
        FT_Done_FreeType(pFont->library);
        return -1;
    }

    face_set_size(pFont->face, pFont->size);
    return 0;
}

void text::fin_font(TCFX_pFont pFont)
{
    FT_Done_Face(pFont->face);
    FT_Done_FreeType(pFont->library);
}

/* private */
void text::face_set_size(FT_Face face, int size)
{
    TT_HoriHeader *hori;
    TT_OS2 *os2;
    double scale;
    FT_Size_RequestRec rq;
    FT_Size_Metrics *mt;
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
    rq.height = static_cast<FT_Long>(size * scale * 64);
    rq.horiResolution = 0;
    rq.vertResolution = 0;
    FT_Request_Size(face, &rq);
    mt = &face->size->metrics;
    mt->ascender = static_cast<FT_Pos>(mt->ascender / scale);
    mt->descender = static_cast<FT_Pos>(mt->descender / scale);
    mt->height = static_cast<FT_Pos>(mt->height / scale);
}

int text::get_text_pix(TCFX_pFont pFont, wchar_t text, TCFX_pPix pPix)
{
    int i, l, m;
    FT_UInt index;
    index = FT_Get_Char_Index(pFont->face, text);
    if (0 == index) {
        printf("TextPix Error: find an invalid text that does not exists in the font.\n");
        return -1;
    }
    if (0 == pFont->bord) {
        ///FT_Load_Glyph(pFont->face, index, FT_LOAD_RENDER);
        FT_Glyph glyph;
        FT_BitmapGlyph bitmap_glyph;
        FT_Load_Glyph(pFont->face, index, FT_LOAD_NO_BITMAP);
        FT_Get_Glyph(pFont->face->glyph, &glyph);
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, NULL, TRUE);
        bitmap_glyph = (FT_BitmapGlyph)glyph;
        pPix->initX = bitmap_glyph->left;    //pFont->face->glyph->metrics.horiBearingX / 64;
        pPix->initY = pFont->size + pFont->face->size->metrics.descender / 64 - bitmap_glyph->top;    ///pFont->size + ((pFont->face->size->metrics.descender - pFont->face->glyph->metrics.horiBearingY) / 64);
        pPix->width = bitmap_glyph->bitmap.width;    //pFont->face->glyph->bitmap.width;
        pPix->height = bitmap_glyph->bitmap.rows;    //pFont->face->glyph->bitmap.rows;
        pPix->size = pPix->height * (pPix->width << 2);
        pPix->buf = (unsigned char *)malloc(pPix->size * sizeof(unsigned char));
        for (l = 0; l < pPix->height; l++) {
            for (m = 0; m < pPix->width; m++) {
                i = (l * pPix->width + m) << 2;
                *((unsigned long *)&pPix->buf[i]) = pFont->color;
                pPix->buf[i + 3] = bitmap_glyph->bitmap.buffer[l * pPix->width + m];
            }
        }
        FT_Done_Glyph(glyph);
    } else if (pFont->is_outline) {    /* has border and only retrieve outline pixels */
        if (pFont->bord > 0) {
            int initX_old, initY_old, initX_new, initY_new;    /* variables with the _old post-fix hold the value of the original text glyph */
            int width_old, height_old, width_new, height_new;
            int left, right, top, bottom, offset_x, offset_y;
            unsigned char *buf_old;
            unsigned char *buf_new;
            FT_Glyph glyph;
            FT_Glyph glyph_old;
            FT_Stroker stroker;
            FT_BitmapGlyph bitmap_glyph;
            FT_BitmapGlyph bitmap_glyph_old;
            FT_Load_Glyph(pFont->face, index, FT_LOAD_NO_BITMAP);
            FT_Get_Glyph(pFont->face->glyph, &glyph);
            glyph_old = glyph;
            FT_Stroker_New(pFont->library, &stroker);
            FT_Stroker_Set(stroker, pFont->bord << 6, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
            FT_Glyph_StrokeBorder(&glyph, stroker, FALSE, FALSE);
            FT_Stroker_Done(stroker);
            FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, NULL, TRUE);
            bitmap_glyph = (FT_BitmapGlyph)glyph;
            FT_Glyph_To_Bitmap(&glyph_old, FT_RENDER_MODE_NORMAL, NULL, TRUE);
            bitmap_glyph_old = (FT_BitmapGlyph)glyph_old;
            initX_old = bitmap_glyph_old->left;
            initY_old = pFont->size + (pFont->face->size->metrics.descender / 64) - bitmap_glyph_old->top;
            width_old = bitmap_glyph_old->bitmap.width;
            height_old = bitmap_glyph_old->bitmap.rows;
            initX_new = bitmap_glyph->left;
            initY_new = pFont->size + (pFont->face->size->metrics.descender / 64) - bitmap_glyph->top;
            width_new = bitmap_glyph->bitmap.width;
            height_new = bitmap_glyph->bitmap.rows;
            left = min(initX_old, initX_new);
            right = max(initX_old + width_old, initX_new + width_new);
            top = min(initY_old, initY_new);
            bottom = max(initY_old + height_old, initY_new + height_new);
            pPix->initX = left;
            pPix->initY = top;
            pPix->width = right - left;
            pPix->height = bottom - top;
            pPix->size = pPix->height * (pPix->width << 2);
            offset_x = initX_old - left;
            offset_y = initY_old - top;
            buf_old = (unsigned char *)malloc(pPix->size * sizeof(unsigned char));
            memset(buf_old, 0, pPix->size * sizeof(unsigned char));
            for (l = 0; l < height_old; l++) {
                for (m = 0; m < width_old; m++)
                    buf_old[(l + offset_y) * pPix->width + m + offset_x] = bitmap_glyph_old->bitmap.buffer[l * width_old + m];
            }
            FT_Done_Glyph(glyph_old);
            offset_x = initX_new - left;
            offset_y = initY_new - top;
            buf_new = (unsigned char *)malloc(pPix->size * sizeof(unsigned char));
            memset(buf_new, 0, pPix->size * sizeof(unsigned char));
            for (l = 0; l < height_new; l++) {
                for (m = 0; m < width_new; m++)
                    buf_new[(l + offset_y) * pPix->width + m + offset_x] = bitmap_glyph->bitmap.buffer[l * width_new + m];
            }
            FT_Done_Glyph(glyph);
            pPix->buf = (unsigned char *)malloc(pPix->size * sizeof(unsigned char));
            for (l = 0; l < pPix->height; l++) {
                for (m = 0; m < pPix->width; m++) {
                    i = 4 * (l * pPix->width + m);
                    *((unsigned long *)&pPix->buf[i]) = pFont->color;
                    pPix->buf[i + 3] = Pix.CLIP_0_255(buf_new[l * pPix->width + m] - buf_old[l * pPix->width + m]);
                }
            }
            free(buf_old);
            free(buf_new);
        } else {
            printf("TextPix Error: if you want to get the outline of texts, the `bord' can not be minus.\n");
            return -1;
        }
    } else {    /* can handle cases for both bord < 0 and bord > 0 */
        FT_Glyph glyph;
        FT_Stroker stroker;
        FT_BitmapGlyph bitmap_glyph;
        FT_Load_Glyph(pFont->face, index, FT_LOAD_NO_BITMAP);
        FT_Get_Glyph(pFont->face->glyph, &glyph);
        FT_Stroker_New(pFont->library, &stroker);
        FT_Stroker_Set(stroker, pFont->bord * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);    /* should use `* 64' instead of `<< 6' in case of bord < 0 */
        FT_Glyph_StrokeBorder(&glyph, stroker, FALSE, TRUE);
        FT_Stroker_Done(stroker);
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, NULL, TRUE);
        bitmap_glyph = (FT_BitmapGlyph)glyph;
        pPix->initX = bitmap_glyph->left;
        pPix->initY = pFont->size + (pFont->face->size->metrics.descender / 64) - bitmap_glyph->top;
        pPix->width = bitmap_glyph->bitmap.width;
        pPix->height = bitmap_glyph->bitmap.rows;
        pPix->size = pPix->height * (pPix->width << 2);
        pPix->buf = (unsigned char *)malloc(pPix->size * sizeof(unsigned char));
        for (l = 0; l < pPix->height; l++) {
            for (m = 0; m < pPix->width; m++) {
                i = 4 * (l * pPix->width + m);
                *((unsigned long *)&pPix->buf[i]) = pFont->color;
                pPix->buf[i + 3] = bitmap_glyph->bitmap.buffer[l * pPix->width + m];
            }
        }
        FT_Done_Glyph(glyph);
    }
    return 0;
}

int text::get_texts_pix(TCFX_pFont pFont, const wchar_t *text, TCFX_pPix pPix)
{
    int i, j, count, above, below, h, w, offset_x, offset_y, offset_dst, offset_src;
    int *pAdvanceHori;
    int *pBearingYHori;
    int *pKerning;
    FT_UInt index;
    TCFX_pPix pPixTemp;
    count = wcslen(text);
    pAdvanceHori = (int *)malloc(count * sizeof(int));
    pBearingYHori = (int *)malloc(count * sizeof(int));
    pKerning = (int *)malloc(count * sizeof(int));
    pPixTemp = (TCFX_pPix)malloc(count * sizeof(TCFX_Pix));
    for (i = 0; i < count; ++i)
    {
        index = FT_Get_Char_Index(pFont->face, text[i]);
        if (0 == index)
        {
            free(pAdvanceHori);
            free(pBearingYHori);
            free(pKerning);
            free(pPixTemp);
            printf("TextPix Error: find an invalid text that does not exists in the font.\n");
            return -1;
        }
        FT_Load_Glyph(pFont->face, index, FT_LOAD_DEFAULT);
        pAdvanceHori[i] = pFont->face->glyph->metrics.horiAdvance >> 6;
        pBearingYHori[i] = pFont->face->glyph->metrics.horiBearingY / 64;
        if (L' ' == text[i] || L'　' == text[i])
            pAdvanceHori[i] = (int)(pAdvanceHori[i] * pFont->spaceScale);
        if (get_text_pix(pFont, text[i], &pPixTemp[i]) != 0)
        {
            free(pAdvanceHori);
            free(pBearingYHori);
            free(pKerning);
            for (j = 0; j < i; j++)
                free(pPixTemp[j].buf);
            free(pPixTemp);
            return -1;
        }
    }
    if (FT_HAS_KERNING(pFont->face))
    {
        FT_UInt left;
        FT_UInt right;
        FT_Vector kerning;
        for (i = 0; i < count - 1; ++i)
        {
            left  = FT_Get_Char_Index(pFont->face, text[i]);
            right = FT_Get_Char_Index(pFont->face, text[i + 1]);
            FT_Get_Kerning(pFont->face, left, right, FT_KERNING_DEFAULT, &kerning);
            pKerning[i] = kerning.x / 64;
        }

        pKerning[i] = 0;
    } else memset(pKerning, 0, count * sizeof(int));
    pPix->initX = pPixTemp[0].initX;
    pPix->initY = pPixTemp[0].initY;
    for (i = 1; i < count; i++)
        pPix->initY = min(pPix->initY, pPixTemp[i].initY);
    pPix->width = -(int)pPixTemp[0].initX;
    for (i = 0; i < count; ++i)
        pPix->width += pAdvanceHori[i] + pKerning[i] + pFont->spacing;
    pPix->width += pPixTemp[count - 1].width + (int)pPixTemp[count - 1].initX - pAdvanceHori[count - 1] - pFont->spacing;
    above = pBearingYHori[0];
    below = pPixTemp[0].height - pBearingYHori[0];
    for (i = 1; i < count; ++i)
    {
        above = max(above, pBearingYHori[i]);
        below = max(below, pPixTemp[i].height - pBearingYHori[i]);
    }
    pPix->height = above + below;
    pPix->size = pPix->height * (pPix->width << 2);
    pPix->buf = (unsigned char *)malloc(pPix->size * sizeof(unsigned char));
    memset(pPix->buf, 0, pPix->size * sizeof(unsigned char));
    offset_x = -(int)pPixTemp[0].initX;
    for (i = 0; i < count; ++i)
    {
        offset_y = above - pBearingYHori[i];
        for (h = 0; h < pPixTemp[i].height; ++h)
        {
            for (w = 0; w < pPixTemp[i].width; ++w)
            {
                offset_dst = ((h + offset_y) * pPix->width + w + offset_x + (int)pPixTemp[i].initX) << 2;
                offset_src = (h * pPixTemp[i].width + w) << 2;
                if (0 != pPixTemp[i].buf[offset_src + 3])
                    memcpy(pPix->buf + offset_dst, pPixTemp[i].buf + offset_src, 4 * sizeof(unsigned char));
            }
        }
        offset_x += pAdvanceHori[i] + pKerning[i] + pFont->spacing;
        free(pPixTemp[i].buf);
    }
    free(pAdvanceHori);
    free(pBearingYHori);
    free(pKerning);
    free(pPixTemp);
    return 0;
}

int text::get_text_outline_points(const TCFX_pFont pFont, wchar_t text, double density, double **pXBuf,
                                  double **pYBuf, unsigned char **pABuf, int *pCount)
{
    FT_UInt index;
    FT_Glyph glyph;
    FT_OutlineGlyph outline;
    FT_Outline_Funcs outInterface;
    TCFX_OutlinePoints outlinePoints;
    /* get parameters from script */
    index = FT_Get_Char_Index(pFont->face, text);
    if (0 == index)
    {
        printf("TextOutlinePoints Error: find an invalid text that does not exists in the font.\n");
        return -1;
    }
    FT_Load_Glyph(pFont->face, index, FT_LOAD_DEFAULT);
    FT_Get_Glyph(pFont->face->glyph, &glyph);
    if (FT_GLYPH_FORMAT_OUTLINE != glyph->format)
    {
        printf("TextOutlinePoints Error: cannot get the outline of the text.\n");
        return -1;
    }
    outline = (FT_OutlineGlyph)glyph;
    outlinePoints.density = density;
    outlinePoints.height = pFont->size + pFont->face->size->metrics.descender / 64;
    outlinePoints.lastX = 0;
    outlinePoints.lastY = 0;
    outlinePoints.points.count = 0;
    outlinePoints.points.capacity = 100;
    outlinePoints.points.xBuf = (double *)malloc(outlinePoints.points.capacity * sizeof(double));
    outlinePoints.points.yBuf = (double *)malloc(outlinePoints.points.capacity * sizeof(double));
    outlinePoints.points.aBuf = (unsigned char *)malloc(outlinePoints.points.capacity * sizeof(unsigned char));
    outInterface.move_to = (FT_Outline_MoveToFunc)_outline_points_move_to;
    outInterface.line_to = (FT_Outline_LineToFunc)_outline_points_line_to;
    outInterface.conic_to = (FT_Outline_ConicToFunc)_outline_points_conic_to;
    outInterface.cubic_to = (FT_Outline_CubicToFunc)_outline_points_cubic_to;
    outInterface.shift = 0;
    outInterface.delta = 0;
    FT_Outline_Decompose(&outline->outline, &outInterface, &outlinePoints);
    FT_Done_Glyph(glyph);
    *pXBuf = outlinePoints.points.xBuf;
    *pYBuf = outlinePoints.points.yBuf;
    *pABuf = outlinePoints.points.aBuf;
    *pCount = outlinePoints.points.count;
    return 0;
}

int text::_tcfx_lib_get_text_metrics(const TCFX_pFont pFont, const wchar_t *text, TCFX_pTextMetrics pTextMetrics)
{
    int i, count, height;
    FT_UInt index;
    int *pWidth;
    int *pHeight;
    int *pHoriBearingX;
    int *pHoriBearingY;
    int *pHoriAdvance;
    int *pVertBearingX;
    int *pVertBearingY;
    int *pVertAdvance;
    int *pKerning;
    count = wcslen(text);
    pWidth = (int *)malloc(count * sizeof(int));
    pHeight = (int *)malloc(count * sizeof(int));
    pHoriBearingX = (int *)malloc(count * sizeof(int));
    pHoriBearingY = (int *)malloc(count * sizeof(int));
    pHoriAdvance = (int *)malloc(count * sizeof(int));
    pVertBearingX = (int *)malloc(count * sizeof(int));
    pVertBearingY = (int *)malloc(count * sizeof(int));
    pVertAdvance = (int *)malloc(count * sizeof(int));
    pKerning = (int *)malloc(count * sizeof(int));
    for (i = 0; i < count; i++)
    {
        index = FT_Get_Char_Index(pFont->face, text[i]);
        if (0 == index)
        {
            free(pWidth);
            free(pHeight);
            free(pHoriBearingX);
            free(pHoriBearingY);
            free(pHoriAdvance);
            free(pVertBearingX);
            free(pVertBearingY);
            free(pVertAdvance);
            free(pKerning);
            printf("TextMetrics Error: find an invalid text that does not exists in the font.\n");
            return -1;
        }
        FT_Load_Glyph(pFont->face, index, FT_LOAD_DEFAULT);
        pWidth[i] = pFont->face->glyph->metrics.width >> 6;
        pHeight[i] = pFont->face->glyph->metrics.height >> 6;
        pHoriBearingX[i] = pFont->face->glyph->metrics.horiBearingX / 64;
        pHoriBearingY[i] = pFont->face->glyph->metrics.horiBearingY / 64;
        pHoriAdvance[i] = pFont->face->glyph->metrics.horiAdvance >> 6;
        pVertBearingX[i] = pFont->face->glyph->metrics.vertBearingX / 64;
        pVertBearingY[i] = pFont->face->glyph->metrics.vertBearingY / 64;
        pVertAdvance[i] = pFont->face->glyph->metrics.vertAdvance >> 6;
    }
    if (FT_HAS_KERNING(pFont->face))
    {
        FT_UInt left;
        FT_UInt right;
        FT_Vector kerning;
        for (i = 0; i < count - 1; i++)
        {
            left  = FT_Get_Char_Index(pFont->face, text[i]);
            right = FT_Get_Char_Index(pFont->face, text[i + 1]);
            FT_Get_Kerning(pFont->face, left, right, FT_KERNING_DEFAULT, &kerning);
            pKerning[i] = kerning.x / 64;
        }
        pKerning[i] = 0;
    } else memset(pKerning, 0, count * sizeof(int));
    pTextMetrics->horiAdvance = 0;
    pTextMetrics->width = 0;
    height = pHeight[0] - pHoriBearingY[0];
    pTextMetrics->horiBearingX = pHoriBearingX[0];
    pTextMetrics->horiBearingY = pHoriBearingY[0];
    pTextMetrics->vertAdvance = 0;
    pTextMetrics->vertBearingX = pVertBearingX[0];
    pTextMetrics->vertBearingY = pVertBearingY[0];
    for (i = 0; i < count; i++)
    {
        pTextMetrics->horiAdvance += pHoriAdvance[i] + pKerning[i] + pFont->spacing;
        height = max(height, pHeight[i] - pHoriBearingY[i]);
        pTextMetrics->horiBearingY = TCFXLIB_TEXT_MAX(pTextMetrics->horiBearingY, pHoriBearingY[i]);
        pTextMetrics->vertAdvance += pVertAdvance[i] + pFont->spacing;    /* kerning only supports horizontal texts */
        pTextMetrics->vertBearingX = TCFXLIB_TEXT_MIN(pTextMetrics->vertBearingX, pVertBearingX[i]);    /* in most cases it's negative for vertical texts */
    }
    pTextMetrics->horiAdvance -= pFont->spacing;
    pTextMetrics->width = pTextMetrics->horiAdvance - pHoriBearingX[0] - (pHoriAdvance[count - 1] - pHoriBearingX[count - 1] - pWidth[count - 1]);
    pTextMetrics->height = height + pTextMetrics->horiBearingY;
    pTextMetrics->vertAdvance -= pFont->spacing;
    free(pWidth);
    free(pHeight);
    free(pHoriBearingX);
    free(pHoriBearingY);
    free(pHoriAdvance);
    free(pVertBearingX);
    free(pVertBearingY);
    free(pVertAdvance);
    free(pKerning);
    pTextMetrics->x_ppem = pFont->face->size->metrics.x_ppem;
    pTextMetrics->y_ppem = pFont->face->size->metrics.y_ppem;
    pTextMetrics->x_scale = pFont->face->size->metrics.x_scale;
    pTextMetrics->y_scale = pFont->face->size->metrics.y_scale;
    pTextMetrics->ascender = pFont->face->size->metrics.ascender / 64;
    pTextMetrics->descender = pFont->face->size->metrics.descender / 64;
    pTextMetrics->px_height = pFont->face->size->metrics.height >> 6;
    pTextMetrics->max_advance = pFont->face->size->metrics.max_advance >> 6;
    return 0;
}

TCFX_PyTextMetrics text::convert_text_metrics(const TCFX_pTextMetrics pTextMetrics)
{
    py::list pyTextMetrics;
    pyTextMetrics.append(pTextMetrics->width);
    pyTextMetrics.append(pTextMetrics->height);
    pyTextMetrics.append(pTextMetrics->horiBearingX);
    pyTextMetrics.append(pTextMetrics->horiBearingY);
    pyTextMetrics.append(pTextMetrics->horiAdvance);
    pyTextMetrics.append(pTextMetrics->vertBearingX);
    pyTextMetrics.append(pTextMetrics->vertBearingY);
    pyTextMetrics.append(pTextMetrics->vertAdvance);
    pyTextMetrics.append(pTextMetrics->x_ppem);
    pyTextMetrics.append(pTextMetrics->y_ppem);
    pyTextMetrics.append(pTextMetrics->x_scale / 65536.0);
    pyTextMetrics.append(pTextMetrics->y_scale / 65536.0);
    pyTextMetrics.append(pTextMetrics->ascender);
    pyTextMetrics.append(pTextMetrics->descender);
    pyTextMetrics.append(pTextMetrics->px_height);
    pyTextMetrics.append(pTextMetrics->max_advance);

    return py::tuple(pyTextMetrics);
}
