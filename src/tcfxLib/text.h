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

#ifndef TCFXLIB_TEXT_H
#define TCFXLIB_TEXT_H
#pragma once

#include <string>
#include <cstdint>
#include <cstdlib>
#include "boost/locale.hpp"
#include "pix.h"
extern "C"
{
#include "freetype2/ft2build.h"
#include "freetype2/freetype/freetype.h"
#include "freetype2/freetype/ftstroke.h"
#include "freetype2/freetype/tttables.h"
#include "freetype2/freetype/ftimage.h"
#include "freetype2/freetype/fttypes.h"
};

#ifdef _WIN32
#pragma comment(lib, "../FreeType/freetype.lib")
#endif

#define FALSE 0
#define TRUE 1

#define TCFXLIB_TEXT_MAX(x, y) (x > y ? x : y)
#define TCFXLIB_TEXT_MIN(x, y) (x < y ? x : y)

/**
 * TCFX_Font structure
 */
typedef struct _tcfx_font {
    char *filename;         /**< font file name */
    FT_Library library;     /**< FT library */
    FT_Face face;           /**< FT face */
    int id;                 /**< font face id */
    int size;               /**< font size */
    int spacing;            /**< spacing between texts */
    double spaceScale;      /**< white space (including L' ' and L'　') scale */
    unsigned long color;    /**< color of the font */
    int bord;               /**< border of the font */
    int is_outline;         /**< indicates if the PIX only containing outline info */
} TCFX_Font, *TCFX_pFont;

/**
 * TCFX_OutlinePoints structure is used in tcfxlib_get_text_outline_as_points function().
 */
typedef struct _tcfx_outline_points {
    TCFX_Points points;
    double lastX;
    double lastY;
    int height;
    double density;
} TCFX_OutlinePoints, *TCFX_pOutlinePoints;

/**
 * TCFX_TextMetrics structure
 */
typedef struct _tcfx_text_metrics {
    /* FT_Glyph_Metrics */
    FT_Pos width;
    FT_Pos height;
    FT_Pos horiBearingX;
    FT_Pos horiBearingY;
    FT_Pos horiAdvance;
    FT_Pos vertBearingX;
    FT_Pos vertBearingY;
    FT_Pos vertAdvance;
    /* FT_Size_Metrics */
    FT_UShort x_ppem;   /* horizontal pixels per EM */
    FT_UShort y_ppem;   /* vertical pixels per EM */
    FT_Fixed x_scale;   /* scaling values used to convert font */
    FT_Fixed y_scale;   /* units to 26.6 fractional pixels */
    FT_Pos ascender;    /* ascender in 26.6 frac. pixels */
    FT_Pos descender;   /* descender in 26.6 frac. pixels */
    FT_Pos px_height;   /* text height in 26.6 frac. pixels */
    FT_Pos max_advance; /* max horizontal advance, in 26.6 pixels */
} TCFX_TextMetrics, *TCFX_pTextMetrics;

/**
 * TCFX_PyTextMetrics structure that has the following structure
 * (width, height, horiBearingX, horiBearingY, horiAdvance, vertBearingX, vertBearingY, vertAdvance, x_ppem, y_ppem, x_scale, y_scale, ascender, descender, px_height, max_advance)
 */
typedef py::tuple TCFX_PyTextMetrics;

/* Bezier Curve Evaluation using De Casteljau's Algorithm */
void linear_interpolation(double *pxt, double *pyt,
                          double xa, double ya,
                          double xb, double yb,
                          double t);

void conic_bezier(double *pxt, double *pyt,
                  double xs, double ys,
                  double xc, double yc,
                  double xe, double ye,
                  double t);

void cubic_bezier(double *pxt, double *pyt,
                  double xs, double ys,
                  double xc1, double yc1,
                  double xc2, double yc2,
                  double xe, double ye,
                  double t);

double _max_distance_3(double x1, double x2, double x3);

double _max_distance_4(double x1, double x2, double x3, double x4);

int _outline_points_move_to(const FT_Vector *to, void *user);

int _outline_points_line_to(const FT_Vector *to, void *user);

int _outline_points_conic_to(const FT_Vector *control, const FT_Vector *to, void *user);

int _outline_points_cubic_to(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user);

class text : public common
{
public:

    /**
     * Initialize TCFX_Font structure, keep the content in memory, then pass its pointer as TCFX_PyFont.
     * Remark: the real content of TCFX_PyFont is kept in memory, and only can be accessed through tcfxLib APIs,
     * the memory will be freed by FinFont function.
     *
     * @param font_file
     * @param face_id
     * @param font_size
     * @param spacing
     * @param space_scale
     * @param color
     * @param bord
     * @param is_outline
     * @return TCFX_PyFont
     */
    //InitFont(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline)
    text(const char *font_file, int face_id, int font_size, int spacing,
         double space_scale, uint32_t color, int bord, int is_outline);

    text() : pFont(nullptr)
    {
        common();
    }

    /**
     * Finalize TCFX_Font structure free all the memory occupied by it.
     */
    //FinFont(pyFont)
    ~text();

    /**
     * Make TCFX PIX from text(s).
     * @param texts
     * @return TCFX_PyPix
     */
    //TextPix(pyFont, texts)
    TCFX_PyPix get_pix_from_text(const char *texts);

    /* overload */
    //TextPix(font_file, face_id, font_size, spacing, space_scale, color, bord, is_outline, texts)
    TCFX_PyPix get_pix_from_text_2(const char *font_file, int face_id, int font_size, int spacing,
                                   double space_scale, uint32_t color, int bord, int is_outline,
                                   const char *texts);

    /**
     * Get text outline points.
     * @param text
     * @param density
     * @return TCFX_PyPoints
     */
    //TextOutlinePoints(pyFont, text, density)
    TCFX_PyPoints get_text_outline_as_points(const char *text, double density);

    /**
     * Get text outline points.
     * @param font_file
     * @param face_id
     * @param font_size
     * @param text
     * @param density
     * @return TCFX_PyPoints
     */
    /* overload */
    //TextOutlinePoints(font_file, face_id, font_size, text, density)
    TCFX_PyPoints get_text_outline_as_points_2(const char *font_file, int face_id, int font_size,
                                               const char *text, double density);

    /**
     * Get text(s) metrics.
     * @param self reserved
     * @param args (pyFont, texts) or (font_file, face_id, font_size, spacing, space_scale, text)
     * @return TCFX_PyTextMetrics
     */
    //TextMetrics(pyFont, text)
    TCFX_PyTextMetrics get_text_metrics(const char *text);

    //TextMetrics(font_file, face_id, font_size, spacing, space_scale, text)
    TCFX_PyTextMetrics get_text_metrics_2(const char *font_file, int face_id, int font_size, int spacing,
                                          double space_scale, const char *text);

    void reset(const char *font_file, int face_id, int font_size, int spacing,
               double space_scale, uint32_t color, int bord, int is_outline);

    int init_font(TCFX_pFont pFont, char *fontFilename, int fontFaceID,
                  int fontSize, int spacing, double spaceScale, uint32_t color,
                  int bord, int is_outline);

    void fin_font(TCFX_pFont pFont);

    TCFX_pFont get_pFont() const
    {
        return pFont;
    }

private:

    void face_set_size(FT_Face face, int size);

    int get_text_pix(TCFX_pFont pFont, wchar_t text, TCFX_pPix pPix);

    int get_texts_pix(TCFX_pFont pFont, const wchar_t *text, TCFX_pPix pPix);

    int get_text_outline_points(const TCFX_pFont pFont, wchar_t text, double density, double **pXBuf,
                                double **pYBuf, unsigned char **pABuf, int *pCount);

    int _tcfx_lib_get_text_metrics(const TCFX_pFont pFont, const wchar_t *text, TCFX_pTextMetrics pTextMetrics);

    TCFX_PyTextMetrics convert_text_metrics(const TCFX_pTextMetrics pTextMetrics);

    TCFX_pFont pFont;

    pix Pix;

};

#endif    /* TCFXLIB_TEXT_H */
