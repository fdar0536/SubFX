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

#ifndef TCFXLIB_PIX_H
#define TCFXLIB_PIX_H
#pragma once

#include <stdint.h>
#include "std.h"

#define MATH_PI 3.14159265358979323846264338327950288

#define MAKE8B8B8B(r,g,b) ((unsigned long)((unsigned char)(r) | ((unsigned short)((unsigned char)(g)) << 8) | ((unsigned long)((unsigned char)(b)) << 16)))
#define MAKERGB MAKE8B8B8B


/**
 * TCFX_Pix structure used to store pixel info of text(s) / image(s).
 */
typedef struct _tcfx_pix {
    double initX;    /**< left most */
    double initY;    /**< upper most */
    int width;     /**< width of the PIX */
    int height;    /**< height of the PIX */
    int size;    /**< size = height * (width * 4) */
    unsigned char *buf;    /**< pixel values of the PIX */
} TCFX_Pix, *TCFX_pPix;

/**
 * TCFX_Points structure is an auto increase list.
 */
typedef struct _tcfx_points {
    double *xBuf;
    double *yBuf;
    unsigned char *aBuf;
    int count;       /**< number of points */
    int capacity;    /**< maximun points that the buffer can contain */
} TCFX_Points, *TCFX_pPoints;

/**
 * TCFX_PyPix structure that has the following structure ((initX, initY), (width, height), (a1, a2, a3, ...)).
 */
typedef py::tuple TCFX_PyPix;

/**
 * TCFX_PyBigPix structure that has the following structure [(PIX, offset_x, offset_y, layer), (PIX, offset_x, offset_y, layer), ...].
 */
typedef py::list TCFX_PyBigPix;

/**
 * TCFX_PyPoints structure a tuple of tuples (points).
 * ((x1, y1, a1), (x2, y2, a2), ...)
 */
typedef py::tuple TCFX_PyPoints;

class pix
{
public:

    pix(){};

    ~pix(){};

    /**
     * Get points from TCFX PY PIX.
     *
     * @param PIX
     * @return TCFX_PyPoints
     */
    //PixPoints(PIX), where "PIX" is tuple
    TCFX_PyPoints pix_points(TCFX_PyPix &PIX);

    /**
    * Resample TCFX PY PIX.
    *
    * @param PIX
    * @param width
    * @param height
    * @return TCFX_PyPix
    */
    //PixResize(PIX, width, height)
    TCFX_PyPix resample_py_pix(TCFX_PyPix &PIX, int width, int height);

    /**
     * supports fractional width and height
     */
    //PixResizeF(PIX, width, height)
    TCFX_PyPix resample_py_pix_ex(TCFX_PyPix &PIX, double width, double height);

    /**
     * Create a blank PIX.
     *
     * @param width
     * @param height
     * @param rgba
     * @return TCFX_PyPix
     */
    //BlankPix(width, height, rgba)
    TCFX_PyPix create_blank_pix(int width, int height, uint32_t rgba);

    /**
     * Applay a texture to PIX.
     *
     * @param PIX
     * @param Texture
     * @return TCFX_PyPix
     */
    //PixTexture(PIX, texture)
    TCFX_PyPix apply_pix_texture(TCFX_PyPix &PIX, TCFX_PyPix &texture);

    /**
     * Applay a mask to PIX.
     *
     * @param PIX
     * @param mask
     * @return TCFX_PyPix
     */
    //PixMask(PIX, mask)
    TCFX_PyPix apply_pix_mask(TCFX_PyPix &PIX, py::tuple &mask);

    /**
     * Replace the alpha channel of the source PIX with the alpha channel of the destination PIX.
     * Remark: the transparent pixels of the source PIX will stay unchanged.
     *
     * @param PIX
     * @param PIX_alpha
     * @return TCFX_PyPix
     */
    //PixReplaceAlpha(PIX, PIX_alpha)
    TCFX_PyPix pix_replace_alpha(TCFX_PyPix &PIX, TCFX_PyPix &PIX_alpha);

    /**
     * Enlarge a TCFX PY PIX.
     *
     * @param PIX
     * @param x
     * @param y
     * @return TCFX_PyPix
     */
    //PixEnlarge(PIX, x, y)
    TCFX_PyPix enlarge_py_pix(TCFX_PyPix &PIX, int x, int y);

    /**
     * Crop a TCFX PY PIX.
     * @param PIX
     * @param left
     * @param top
     * @param right
     * @param bottom
     * @return TCFX_PyPix
     */
    //PixCrop(PIX, left, top, right, bottom)
    TCFX_PyPix crop_py_pix(TCFX_PyPix &PIX, int left, int top, int right, int bottom);

    /**
     * Strip blank border of a TCFX PY PIX.
     * @param PIX
     * @return TCFX_PyPix
     */
    //PixStrip(PIX)
    TCFX_PyPix py_pix_strip_blank(TCFX_PyPix &PIX);

    /**
     * Applay Gauss IIR blur to a PIX.
     * @param PIX
     * @param radius
     * @return TCFX_PyPix
     */
    //PixBlur(PIX, radius)
    TCFX_PyPix apply_gauss_blur(TCFX_PyPix &PIX, int radius);

    /**
     * Combine two PIXs, i.e., overlay one PIX upon the other.
     * Remark: during overlaying, the float of the input PIXs' initX and initY will
     * be discarded and the result PIX's initX and initY will be integers,
     * so, use BilinearFilter to remove the float of the input PIXs if necessary.
     *
     * @param back
     * @param overlay
     * @return py::tuple
     */
    //CombinePixs(back, overlay)
    TCFX_PyPix combine_two_pixs(TCFX_PyPix &back, TCFX_PyPix &overlay);

    /**
     * Change the color of a PIX by multiplying factors to each channel of RGBA.
     * @param PIX
     * @param r_f
     * @param g_f
     * @param b_f
     * @param a_f
     * @return TCFX_PyPix
     */
    //PixColorMul(PIX, r_f, g_f, b_f, a_f)
    TCFX_PyPix pix_color_multiply(TCFX_PyPix &PIX, double r_f, double g_f, double b_f, double a_f);

    /**
     * Change the color of a PIX by shifting values of each channel of RGBA.
     * @param PIX
     * @param r
     * @param g
     * @param b
     * @param a
     * @return TCFX_PyPix
     */
    //PixColorShift(PIX, r, g, b, a)
    TCFX_PyPix pix_color_shift(TCFX_PyPix &PIX, int r, int g, int b, int a);

    /**
     * Make the color in a PIX with specified rgb value transparent.
     * @param PIX
     * @param rgb
     * @return TCFX_PyPix
     */
    //PixColorTrans(PIX, rgb)
    TCFX_PyPix pix_color_transparent(TCFX_PyPix &PIX, uint32_t rgb);

    /**
     * Change the value of colors including alpha channels in the PIX with flat rgba value.
     * @param PIX
     * @param rgba
     * @return TCFX_PyPix
     */
    //PixColorRGBA(PIX, rgba)
    TCFX_PyPix pix_color_flat_rgba(TCFX_PyPix &PIX, uint32_t rgba);

    /**
     * Change the value of colors in the PIX with flat rgb value.
     * @param PIX
     * @param rgb
     * @return TCFX_PyPix
     */
    //PixColorRGB(PIX, rgb)
    TCFX_PyPix pix_color_flat_rgb(TCFX_PyPix &PIX, uint32_t rgb);

    /**
     * Change the value of alpha channels in the PIX with constant alpha value.
     * @param PIX
     * @param alpha
     * @return TCFX_PyPix
     */
    //PixColorA(PIX, alpha)
    TCFX_PyPix pix_color_flat_alpha(TCFX_PyPix &PIX, unsigned int alpha);

    /**
     * Bilinear filter, mainly used in moving a PIX, so that it will look smoother.
     * @see tcfxlib_bilinear_filter_internal()
     * @param PIX
     * @param offset_x
     * @param offset_y
     * @return TCFX_PyPix
     */
    //BilinearFilter(PIX, offset_x, offset_y)
    TCFX_PyPix bilinear_filter(TCFX_PyPix &PIX, double offset_x, double offset_y);

    /**
     * Scale filter, mainly used in moving a PIX, so that it will look smoother.
     * @see tcfxlib_scale_filter_internal()
     * @param PIX
     * @param offset_x
     * @param offset_y
     * @return TCFX_PyPix
     */
    //ScaleFilter(PIX, offset_x, offset_y)
    TCFX_PyPix scale_filter(TCFX_PyPix &PIX, double offset_x, double offset_y);

    /**
     * Create a TCFX PY BIG PIX structure.
     * @param self reserved
     * @param args ()
     * @return TCFX_PyBigPix
     */
    //InitBigPix()
    TCFX_PyBigPix init_big_pix();

    /**
     * Add a PIX to BigPIX.
     * @param BIG_PIX
     * @param PIX
     * @param offset_x
     * @param offset_y
     * @param layer
     * @return int
     */
    //BigPixAdd(BIG_PIX, PIX, offset_x, offset_y, layer)
    int big_pix_add(TCFX_PyBigPix &BIG_PIX, TCFX_PyPix &PIX, double offset_x, double offset_y, int layer);

    /**
     * Convert BigPIX to PIX.
     * @param BIG_PIX
     * @return TCFX_PyPix
     */
    //ConvertBigPix(BIG_PIX)
    TCFX_PyPix convert_big_pix(TCFX_PyBigPix BIG_PIX);

    /**
     * Clip a number into 0 to 255 inclusive
     */
    unsigned char CLIP_0_255(int a);

    /**
     * Convert TCFX PIX to TCFX PY PIX.
     *
     * @param pPix pointer to TCFX_Pix structure
     * @param delPix indicates whether to delete PIX before returning of the function
     * @return TCFX_PyPix
     */
    TCFX_PyPix convert_pix(const TCFX_pPix pPix, int delPix);

    /**
     * Convert TCFX PY PIX to TCFX PIX.
     * Remark: in the following functions we will just use PIX, it may stand for TCFX PIX or TCFX PY PIX, in case that there is no ambiguity.
     *
     * @param pyPix the TCFX_PyPix structure
     * @param pPix pointer to TCFX_Pix structure that is going to hold the converted data
     * @return const TCFX_pPix
     */
    TCFX_pPix convert_py_pix(const py::tuple &pyPix, TCFX_pPix pPix);

    /**
     * pPoints->xBuf and pPoints->yBuf need to be allocated before being passed to the function, and it will be reallocated if more space is needed
     * pPoints->count and pPoints->capacity should also be specified before being passed to the function, and their value may be modified
     */
    void points_append(TCFX_pPoints pPoints, double x, double y, unsigned char a);

    /**
     * Resample TCFX PIX.
     * Remark: the target width and height should be specified in the target TCFX_Pix structure before it is sent to the function.
     * Other variables will be ignored. Note that the `buf' of the target TCFX_Pix structure should not be allocated before it is
     * sent to the function, otherwise will cause memory leak.
     *
     * @param pPixSrc pointer to source TCFX_Pix structure
     * @param pPixDst pointer to target TCFX_Pix structure
     * @return TCFX_pPix
     */
    TCFX_pPix resample_pix(const TCFX_pPix pPixSrc, TCFX_pPix pPixDst);

private:

    double filter_cubic(double x, double a);

    void resample_rgba(const unsigned char *src, int width, int height,
                       unsigned char *dst, int targetWidth, int targetHeight);

    /**
     * supports fractional width and height
     */
    TCFX_pPix resample_pix_ex(const TCFX_pPix pPixSrc,
                              TCFX_pPix pPixDst,
                              double width, double height);

    /**
     * Enlarge a PIX.
     * Remark: the PIX is put in the middle of the enlarged one, and the extra space is filled with 0.
     * The target width and height should be specified in the target TCFX_Pix structure before it is sent to the function.
     * And target width and height should be both larger than (or equal to) source ones.
     * Other variables will be ignored. Note that the `buf' of the target TCFX_Pix structure should be empty before it is sent to the function,
     * otherwise will cause memory leaking.
     *
     * @param pPixSrc pointer to source TCFX_Pix structure
     * @param pPixDst pointer to target TCFX_Pix structure
     * @return TCFX_pPix
     */
    TCFX_pPix enlarge_pix(const TCFX_pPix pPixSrc, TCFX_pPix pPixDst);

    void multiply_alpha(unsigned char *buf, int length);

    void separate_alpha(unsigned char *buf, int length);

    void gauss_iir_find_constants(double *n_p, double *n_m, double *d_p,
                                  double *d_m, double *bd_p, double *bd_m, double std_dev);

    void gauss_iir_blur(unsigned char *img, int width, int height, int vertRadius, int horiRadius);

    /**
     * Bilinear filter, mainly used in moving a PIX, so that it will look smoother.
     * Remark: this function is for internal use, it is used to manipulate a TCFX PIX.
     *
     * @see tcfxlib_bilinear_filter()
     * @param pPixSrc pointer to the source TCFX_Pix structure
     * @param offset_x offset of the PIX in the horizontal direction
     * @param offset_y offset of the PIX in the vertical direction
     * @param pPixDst pointer to the target TCFX_Pix structure
     * @return const TCFX_pPix
     */
    TCFX_pPix bilinear_filter_internal(const TCFX_pPix pPixSrc, double offset_x, double offset_y, TCFX_pPix pPixDst);

    /**
     * Scale filter, mainly used in moving a PIX, so that it will look smoother.
     * Remark: this function is for internal use, it is used to manipulate a TCFX PIX.
     * The difference between scale filter and bilinear filter is that, the scale filter is most fit for
     * complicated pictures such as text, rich colored pictures, while it is bad for simple solid color squares
     *
     * @see tcfxlib_scale_filter()
     * @param pPixSrc pointer to the source TCFX_Pix structure
     * @param offset_x offset of the PIX in the horizontal direction
     * @param offset_y offset of the PIX in the vertical direction
     * @param pPixDst pointer to the target TCFX_Pix structure
     * @return const TCFX_pPix
     */
    TCFX_pPix scale_filter_internal(const TCFX_pPix pPixSrc, double offset_x, double offset_y, TCFX_pPix pPixDst);
};

#endif    /* TCFXLIB_PIX_H */
