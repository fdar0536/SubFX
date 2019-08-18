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

#include "pix.hpp"
#include <algorithm>

using namespace std;

/* public */
//PixPoints(PIX), where "PIX" is tuple
py::tuple pix::pix_points(py::tuple &PIX)
{
    TCFX_Pix pix;
    int i, w, h;
    TCFX_Points points;
    if (len(PIX) < 1)
    {
        PyErr_SetString(PyExc_RuntimeError, "PixPoints error, too less parameters - `(PIX)'\n");
        return PIX;
    }

    convert_py_pix(PIX, &pix);
    points.count = 0;
    points.capacity = 100;
    points.xBuf = (double *)malloc(points.capacity * sizeof(double));
    points.yBuf = (double *)malloc(points.capacity * sizeof(double));
    points.aBuf = (unsigned char *)malloc(points.capacity * sizeof(unsigned char));
    for (h = 0; h < pix.height; ++h)
    {
        for (w = 0; w < pix.width; ++w)
        {
            unsigned char a = pix.buf[((h * pix.width + w) << 2) + 3];
            if (0 != a)
                points_append(&points, w, h, a);
        }
    }
    free(pix.buf);

    py::list pyPoints;
    for (i = 0; i < points.count; ++i)
    {
        pyPoints.append(py::make_tuple(points.xBuf[i], points.yBuf[i], points.aBuf[i]));
    }

    free(points.xBuf);
    free(points.yBuf);
    free(points.aBuf);

    return py::tuple(pyPoints);
}

//BlankPix(width, height, rgba)
py::tuple pix::create_blank_pix(int width, int height, uint32_t rgba)
{
    TCFX_Pix pix;
    int i;

    pix.initX = 0;
    pix.initY = 0;
    pix.width = width;
    pix.height = height;
    pix.size = pix.height * pix.width;
    pix.buf = (unsigned char *)malloc(pix.size * sizeof(unsigned char));
    for (i = 0; i < pix.size; i += 4)
        *((uint32_t *)&pix.buf[i]) = rgba;
    return convert_pix(&pix, 1);
}

//PixResize(PIX, width, height)
py::tuple pix::resample_py_pix(py::tuple &PIX, int width, int height)
{
    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;

    if (width < 0 || height < 0 || (0 == width && 0 == height))
    {
        PyErr_SetString(PyExc_RuntimeError, "PixResize error, invalid width or height value\n");
        return PIX;
    }

    convert_py_pix(PIX, &pixSrc);
    pixDst.width = width;
    pixDst.height = height;
    resample_pix(&pixSrc, &pixDst);
    free(pixSrc.buf);
    return convert_pix(&pixDst, 1);
}

//PixResizeF(PIX, width, height)
py::tuple pix::resample_py_pix_ex(py::tuple &PIX, double width, double height)
{
    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;

    if (width < 0 || height < 0 || (0 == width && 0 == height))
    {
        PyErr_SetString(PyExc_RuntimeError, "PixResizeF error, invalid width or height value\n");
        return PIX;
    }

    convert_py_pix(PIX, &pixSrc);
    resample_pix_ex(&pixSrc, &pixDst, width, height);
    free(pixSrc.buf);
    return convert_pix(&pixDst, 1);
}

//PixTexture(PIX, texture)
py::tuple pix::apply_pix_texture(py::tuple &PIX, py::tuple &texture)
{
    TCFX_Pix pix;
    TCFX_Pix textureSrc;
    TCFX_Pix textureDst;
    int i, alpha;

    convert_py_pix(PIX, &pix);
    convert_py_pix(texture, &textureSrc);
    textureDst.width = pix.width;
    textureDst.height = pix.height;
    resample_pix(&textureSrc, &textureDst);

    free(textureSrc.buf);
    for (i = 0; i < pix.size; i += 4)
    {
        if ((alpha = textureDst.buf[i + 3]) != 0)
        {
            pix.buf[i]     = textureDst.buf[i]     * alpha / 255 + pix.buf[i]     * (255 - alpha) / 255;
            pix.buf[i + 1] = textureDst.buf[i + 1] * alpha / 255 + pix.buf[i + 1] * (255 - alpha) / 255;
            pix.buf[i + 2] = textureDst.buf[i + 2] * alpha / 255 + pix.buf[i + 2] * (255 - alpha) / 255;
        }
    }
    free(textureDst.buf);
    return convert_pix(&pix, 1);
}

//PixMask(PIX, mask)
py::tuple pix::apply_pix_mask(py::tuple &PIX, py::tuple &mask)
{
    TCFX_Pix pix;
    TCFX_Pix maskSrc;
    int w, h, offset_x, offset_y, index1, index2;

    convert_py_pix(PIX, &pix);
    convert_py_pix(mask, &maskSrc);
    offset_x = static_cast<int>(pix.initX - maskSrc.initX);
    offset_y = static_cast<int>(pix.initY - maskSrc.initY);
    for (h = 0; h < pix.height; ++h)
    {
        for (w = 0; w < pix.width; ++w) {
            index1 = h * (pix.width << 2) + (w << 2) + 3;
            index2 = (((h + offset_y) * maskSrc.width + w + offset_x) << 2) + 3;
            if (!(0 <= offset_y + h &&
                  offset_y + h < maskSrc.height &&
                  0 <= offset_x + w &&
                  offset_x + w < maskSrc.width &&
                  0 != maskSrc.buf[index2]))
                pix.buf[index1] = 0;
            else
                pix.buf[index1] = pix.buf[index1] * maskSrc.buf[index2] / 255;
        }
    }
    free(maskSrc.buf);
    return convert_pix(&pix, 1);
}

//PixReplaceAlpha(PIX, PIX_alpha)
py::tuple pix::pix_replace_alpha(py::tuple &PIX, py::tuple &PIX_alpha)
{
    TCFX_Pix pix;
    TCFX_Pix alphaSrc;
    TCFX_Pix alphaDst;
    int i;

    convert_py_pix(PIX, &pix);
    convert_py_pix(PIX_alpha, &alphaSrc);
    alphaDst.width = pix.width;
    alphaDst.height = pix.height;
    resample_pix(&alphaSrc, &alphaDst);
    free(alphaSrc.buf);
    for (i = 0; i < pix.size; i += 4)
    {
        if (0 != pix.buf[i + 3])
            pix.buf[i + 3] = alphaDst.buf[i + 3];
    }

    free(alphaDst.buf);
    return convert_pix(&pix, 1);
}

//PixEnlarge(PIX, x, y)
py::tuple pix::enlarge_py_pix(py::tuple &PIX, int x, int y)
{
    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;

    convert_py_pix(PIX, &pixSrc);
    pixDst.width = pixSrc.width + x;
    pixDst.height = pixSrc.height + y;
    enlarge_pix(&pixSrc, &pixDst);
    free(pixSrc.buf);
    return convert_pix(&pixDst, 1);
}

//PixCrop(PIX, left, top, right, bottom)
py::tuple pix::crop_py_pix(py::tuple &PIX, int left, int top, int right, int bottom)
{
    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;

    int h, w, srcIndex, dstIndex;

    convert_py_pix(PIX, &pixSrc);
    if (right <= 0) right += pixSrc.width;
    if (bottom <= 0) bottom += pixSrc.height;
    if (left < 0 ||
        top < 0 ||
        right > pixSrc.width ||
        bottom > pixSrc.height ||
        left >= right ||
        top >= bottom)
    {
        free(pixSrc.buf);
        PyErr_SetString(PyExc_RuntimeError, "PixCrop error, invalid value(s) in `left, top, right, bottom'\n");
        return PIX;
    }

    pixDst.initX = pixSrc.initX + left;
    pixDst.initY = pixSrc.initY + top;
    pixDst.width = right - left;
    pixDst.height = bottom - top;
    pixDst.size = pixDst.height * (pixDst.width << 2);
    pixDst.buf = (unsigned char *)malloc(pixDst.size * sizeof(unsigned char));
    for (h = 0; h < pixDst.height; ++h)
    {
        for (w = 0; w < pixDst.width; ++w) {
            dstIndex = (h * pixDst.width + w) << 2;
            srcIndex = ((h + top) * pixSrc.width + w + left) << 2;
            memcpy(pixDst.buf + dstIndex, pixSrc.buf + srcIndex, 4 * sizeof(unsigned char));
        }
    }

    free(pixSrc.buf);
    return convert_pix(&pixDst, 1);
}

//PixStrip(PIX)
py::tuple pix::py_pix_strip_blank(py::tuple &PIX)
{
    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;
    int left, top, right, bottom;
    int h, w, srcIndex, dstIndex;

    convert_py_pix(PIX, &pixSrc);
    left = pixSrc.width;
    right = 0;
    top = pixSrc.height;
    bottom = 0;
    for (h = 0; h < pixSrc.height; ++h)
    {
        for (w = 0; w < pixSrc.width; ++w)
        {
            srcIndex = (h * pixSrc.width + w) << 2;
            if (pixSrc.buf[srcIndex + 3] != 0)
            {
                left = min(left, w);
                right = max(right, w + 1);
                top = min(top, h);
                bottom = max(bottom, h + 1);
            }
        }
    }

    pixDst.initX = pixSrc.initX + left;
    pixDst.initY = pixSrc.initY + top;
    pixDst.width = right - left;
    pixDst.height = bottom - top;
    if (pixDst.width <= 0 || pixDst.height <= 0)
    {
        free(pixSrc.buf);
        return py::tuple();
    }

    pixDst.size = pixDst.height * (pixDst.width << 2);
    pixDst.buf = (unsigned char *)malloc(pixDst.size * sizeof(unsigned char));
    for (h = 0; h < pixDst.height; ++h)
    {
        for (w = 0; w < pixDst.width; ++w)
        {
            dstIndex = (h * pixDst.width + w) << 2;
            srcIndex = ((h + top) * pixSrc.width + w + left) << 2;
            memcpy(pixDst.buf + dstIndex, pixSrc.buf + srcIndex, 4 * sizeof(unsigned char));
        }
    }

    free(pixSrc.buf);
    return convert_pix(&pixDst, 1);
}

//PixBlur(PIX, radius)
py::tuple pix::apply_gauss_blur(py::tuple &PIX, int radius)
{

    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;

    convert_py_pix(PIX, &pixSrc);
    pixDst.width = pixSrc.width + 2 * radius;
    pixDst.height = pixSrc.height + 2 * radius;
    enlarge_pix(&pixSrc, &pixDst);
    free(pixSrc.buf);
    gauss_iir_blur(pixDst.buf, pixDst.width, pixDst.height, radius, radius);
    return convert_pix(&pixDst, 1);
}

/**
 * PIX blender, 0 - video, 1 - back, 2 - overlay
 * R_t = R1 * A1 / 255 + R0 * (255 - A1) / 255
 * R = R2 * A2 / 255 + R_t * (255 - A2) / 255
 * R = Rx * Ax / 255 + R0 * (255 - Ax) / 255
 * result:
 * Ax = 255 - (255 - A1) * (255 - A2) / 255
 * Rx = (R2 * A2 + R1 * A1 * (255 - A2) / 255) / Ax
 */
//CombinePixs(back, overlay)
py::tuple pix::combine_two_pixs(py::tuple &back, py::tuple &overlay)
{
    int i, h, w, dstIndex, srcIndex, left, top, right, bottom, offset_x1, offset_y1, offset_x2, offset_y2;
    unsigned char *overlayBuf;
    unsigned char R1, R2, R;
    unsigned char G1, G2, G;
    unsigned char B1, B2, B;
    unsigned char A1, A2, A;
    TCFX_Pix backPix;
    TCFX_Pix overlayPix;
    TCFX_Pix blendedPix;

    convert_py_pix(back, &backPix);
    convert_py_pix(overlay, &overlayPix);
    left = min((int)backPix.initX, (int)overlayPix.initX);
    top = min((int)backPix.initY, (int)overlayPix.initY);
    right = max((int)backPix.initX + backPix.width, (int)overlayPix.initX + overlayPix.width);
    bottom = max((int)backPix.initY + backPix.height, (int)overlayPix.initY + overlayPix.height);
    blendedPix.width = right - left;
    blendedPix.height = bottom - top;
    blendedPix.size = blendedPix.height * (blendedPix.width << 2);
    blendedPix.buf = (unsigned char *)malloc(blendedPix.size * sizeof(unsigned char));
    memset(blendedPix.buf, 0, blendedPix.size * sizeof(unsigned char));
    offset_x1 = (int)backPix.initX - left;
    offset_y1 = (int)backPix.initY - top;
    for (h = 0; h < backPix.height; ++h)
    {
        for (w = 0; w < backPix.width; ++w)
        {
            dstIndex = ((h + offset_y1) * blendedPix.width + w + offset_x1) << 2;
            srcIndex = (h * backPix.width + w) << 2;
            memcpy(blendedPix.buf + dstIndex, backPix.buf + srcIndex, 4 * sizeof(unsigned char));
        }
    }

    offset_x2 = (int)overlayPix.initX - left;
    offset_y2 = (int)overlayPix.initY - top;
    overlayBuf = (unsigned char *)malloc(blendedPix.size * sizeof(unsigned char));
    memset(overlayBuf, 0, blendedPix.size * sizeof(unsigned char));
    for (h = 0; h < overlayPix.height; ++h)
    {
        for (w = 0; w < overlayPix.width; ++w)
        {
            dstIndex = ((h + offset_y2) * blendedPix.width + w + offset_x2) << 2;
            srcIndex = (h * overlayPix.width + w) << 2;
            memcpy(overlayBuf + dstIndex, overlayPix.buf + srcIndex, 4 * sizeof(unsigned char));
        }
    }

    blendedPix.initX = left;
    blendedPix.initY = top;
    for (i = 0; i < blendedPix.size; i += 4)
    {
        R1 = blendedPix.buf[i];
        G1 = blendedPix.buf[i + 1];
        B1 = blendedPix.buf[i + 2];
        A1 = blendedPix.buf[i + 3];
        R2 = overlayBuf[i];
        G2 = overlayBuf[i + 1];
        B2 = overlayBuf[i + 2];
        A2 = overlayBuf[i + 3];
        A = 255 - (255 - A1) * (255 - A2) / 255;
        if (0 != A)
        {
            R = (R2 * A2 + R1 * A1 * (255 - A2) / 255) / A;
            G = (G2 * A2 + G1 * A1 * (255 - A2) / 255) / A;
            B = (B2 * A2 + B1 * A1 * (255 - A2) / 255) / A;
            blendedPix.buf[i]     = R;
            blendedPix.buf[i + 1] = G;
            blendedPix.buf[i + 2] = B;
            blendedPix.buf[i + 3] = A;
        }
    }

    free(backPix.buf);
    free(overlayPix.buf);
    free(overlayBuf);
    return convert_pix(&blendedPix, 1);
}

//PixColorMul(PIX, r_f, g_f, b_f, a_f)
py::tuple pix::pix_color_multiply(py::tuple &PIX, double r_f, double g_f, double b_f, double a_f)
{
    TCFX_Pix pix;
    int i;

    convert_py_pix(PIX, &pix);
    for (i = 0; i < pix.size; i += 4)
    {
        if (0 != pix.buf[i + 3])
        {
            pix.buf[i] = CLIP_0_255((int)(pix.buf[i] * r_f + 0.5));
            pix.buf[i + 1] = CLIP_0_255((int)(pix.buf[i + 1] * g_f + 0.5));
            pix.buf[i + 2] = CLIP_0_255((int)(pix.buf[i + 2] * b_f + 0.5));
            pix.buf[i + 3] = CLIP_0_255((int)(pix.buf[i + 3] * a_f + 0.5));
            if (0 == pix.buf[i + 3])
                pix.buf[i + 3] = 1;  /* transparent DIP will not be written to the tcas file */
        }
    }

    return convert_pix(&pix, 1);
}

//PixColorShift(PIX, r, g, b, a)
py::tuple pix::pix_color_shift(py::tuple &PIX, int r, int g, int b, int a)
{
    TCFX_Pix pix;
    int i;

    convert_py_pix(PIX, &pix);
    for (i = 0; i < pix.size; i += 4)
    {
        if (0 != pix.buf[i + 3])
        {
            pix.buf[i] = CLIP_0_255(pix.buf[i] + r);
            pix.buf[i + 1] = CLIP_0_255(pix.buf[i + 1] + g);
            pix.buf[i + 2] = CLIP_0_255(pix.buf[i + 2] + b);
            pix.buf[i + 3] = CLIP_0_255(pix.buf[i + 3] + a);
            if (0 == pix.buf[i + 3])
                pix.buf[i + 3] = 1;  /* transparent DIP will not be written to the tcas file */
        }
    }
    return convert_pix(&pix, 1);
}

//PixColorTrans(PIX, rgb)
py::tuple pix::pix_color_transparent(py::tuple &PIX, uint32_t rgb)
{
    TCFX_Pix pix;
    int i;

    convert_py_pix(PIX, &pix);
    for (i = 0; i < pix.size; i += 4)
    {
        if ((0x00FFFFFF & (*((uint32_t *)&pix.buf[i]))) == rgb)
            pix.buf[i + 3] = 0;
    }

    return convert_pix(&pix, 1);
}

//PixColorRGBA(PIX, rgba)
py::tuple pix::pix_color_flat_rgba(py::tuple &PIX, uint32_t rgba)
{
    TCFX_Pix pix;
    int i;

    convert_py_pix(PIX, &pix);
    for (i = 0; i < pix.size; i += 4)
    {
        if (0 != pix.buf[i + 3])
            (*((uint32_t  *)&pix.buf[i])) = rgba;
    }

    return convert_pix(&pix, 1);
}

//PixColorRGB(PIX, rgb)
py::tuple pix::pix_color_flat_rgb(py::tuple &PIX, uint32_t rgb)
{
    TCFX_Pix pix;
    int i;

    convert_py_pix(PIX, &pix);
    for (i = 0; i < pix.size; i += 4)
    {
        if (0 != pix.buf[i + 3])
            memcpy(pix.buf + i, &rgb, 3 * sizeof(unsigned char));
    }

    return convert_pix(&pix, 1);
}

//PixColorA(PIX, alpha)
py::tuple pix::pix_color_flat_alpha(py::tuple &PIX, unsigned int alpha)
{
    TCFX_Pix pix;
    int i;
    if (alpha >= 256)
    {
        PyErr_SetString(PyExc_RuntimeError, "PixColorA error, alpha must less than 256'\n");
        return PIX;
    }

    convert_py_pix(PIX, &pix);
    for (i = 0; i < pix.size; i += 4)
    {
        if (0 != pix.buf[i + 3])
            pix.buf[i + 3] = static_cast<unsigned char>(alpha);
    }

    return convert_pix(&pix, 1);
}

//BilinearFilter(PIX, offset_x, offset_y)
py::tuple pix::bilinear_filter(py::tuple &PIX, double offset_x, double offset_y)
{
    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;

    convert_py_pix(PIX, &pixSrc);
    bilinear_filter_internal(&pixSrc, offset_x, offset_y, &pixDst);
    delete[] pixSrc.buf;
    return convert_pix(&pixDst, 1);
}

//ScaleFilter(PIX, offset_x, offset_y)
py::tuple pix::scale_filter(py::tuple &PIX, double offset_x, double offset_y)
{
    TCFX_Pix pixSrc;
    TCFX_Pix pixDst;

    convert_py_pix(PIX, &pixSrc);
    scale_filter_internal(&pixSrc, offset_x, offset_y, &pixDst);
    delete[] pixSrc.buf;
    return convert_pix(&pixDst, 1);
}

//InitBigPix()
py::list pix::init_big_pix()
{
    return py::list();
}

//BigPixAdd(BIG_PIX, PIX, offset_x, offset_y, layer)
int pix::big_pix_add(py::list &BIG_PIX, py::tuple &PIX, double offset_x, double offset_y, int layer)
{
    py::tuple pyBigPixUnit = py::make_tuple(PIX, offset_x, offset_y, layer);
    BIG_PIX.append(pyBigPixUnit);
    return 0;
}

//ConvertBigPix(BIG_PIX)
py::tuple pix::convert_big_pix(py::list BIG_PIX)
{
    TCFX_Pix pix;
    int i, j, h, w, srcIndex, dstIndex, count;
    py::tuple pyBigPixUnit;
    TCFX_pPix pPixSrc;
    double *pOffsetX, *pOffsetY;
    int *pLayer;
    TCFX_Pix pixTemp;
    double fTemp1, fTemp2;
    int iTemp;
    int initPosX, initPosY, left, top, right, bottom, offset_x, offset_y;
    unsigned char r0, g0, b0, a0, r, g, b, a, A;

    count = static_cast<int>(py::len(BIG_PIX));
    pPixSrc = (TCFX_pPix)malloc(count * sizeof(TCFX_Pix));
    pOffsetX = (double *)malloc(count * sizeof(double));
    pOffsetY = (double *)malloc(count * sizeof(double));
    pLayer = (int *)malloc(count * sizeof(int));
    for (i = 0; i < count; ++i)
    {
        pyBigPixUnit = py::extract<py::tuple>(BIG_PIX[i]);
        convert_py_pix(py::extract<py::tuple>(pyBigPixUnit[0]), &pPixSrc[i]);
        pOffsetX[i] = py::extract<double>(pyBigPixUnit[1]);
        pOffsetY[i] = py::extract<double>(pyBigPixUnit[2]);
        pLayer[i] = py::extract<int>(pyBigPixUnit[3]);
    }

    for (i = 0; i < count; ++i)
    {
        for (j = 0; j < count - i - 1; ++j)
        {
            if (pLayer[j] > pLayer[j + 1])
            {
                pixTemp = pPixSrc[j + 1];
                fTemp1 = pOffsetX[j + 1];
                fTemp2 = pOffsetY[j + 1];
                iTemp = pLayer[j + 1];
                pPixSrc[j + 1] = pPixSrc[j];
                pOffsetX[j + 1] = pOffsetX[j];
                pOffsetY[j + 1] = pOffsetY[j];
                pLayer[j + 1] = pLayer[j];
                pPixSrc[j] = pixTemp;
                pOffsetX[j] = fTemp1;
                pOffsetY[j] = fTemp2;
                pLayer[j] = iTemp;
            }
        }
    }

    initPosX = (int)(pOffsetX[0] + pPixSrc[0].initX);
    initPosY = (int)(pOffsetY[0] + pPixSrc[0].initY);
    left = initPosX;
    top = initPosY;
    right = initPosX + pPixSrc[0].width;
    bottom = initPosY + pPixSrc[0].height;
    pix.initX = pOffsetX[0] + pPixSrc[0].initX;
    pix.initY = pOffsetY[0] + pPixSrc[0].initY;
    for (i = 1; i < count; ++i)
    {
        initPosX = (int)(pOffsetX[i] + pPixSrc[i].initX);
        if (initPosX < left)
        {
            left = initPosX;
            pix.initX = pOffsetX[i] + pPixSrc[i].initX;
        }

        initPosY = (int)(pOffsetY[i] + pPixSrc[i].initY);
        if (initPosY < top)
        {
            top = initPosY;
            pix.initY = pOffsetY[i] + pPixSrc[i].initY;
        }

        right = max(right, initPosX + pPixSrc[i].width);
        bottom = max(bottom, initPosY + pPixSrc[i].height);
    }

    pix.width = right - left;
    pix.height = bottom - top;
    pix.size = pix.height * (pix.width << 2);
    pix.buf = (unsigned char *)malloc(pix.size * sizeof(unsigned char));
    memset(pix.buf, 0, pix.size * sizeof(unsigned char));
    for (i = 0; i < count; ++i)
    {
        initPosX = (int)(pOffsetX[i] + pPixSrc[i].initX);
        initPosY = (int)(pOffsetY[i] + pPixSrc[i].initY);
        offset_x = initPosX - left;
        offset_y = initPosY - top;
        for (h = 0; h < pPixSrc[i].height; ++h)
        {
            for (w = 0; w < pPixSrc[i].width; ++w)
            {
                dstIndex = ((h + offset_y) * pix.width + w + offset_x) << 2;
                srcIndex = (h * pPixSrc[i].width + w) << 2;
                r = pPixSrc[i].buf[srcIndex];
                g = pPixSrc[i].buf[srcIndex + 1];
                b = pPixSrc[i].buf[srcIndex + 2];
                a = pPixSrc[i].buf[srcIndex + 3];
                r0 = pix.buf[dstIndex];
                g0 = pix.buf[dstIndex + 1];
                b0 = pix.buf[dstIndex + 2];
                a0 = pix.buf[dstIndex + 3];
                A = 255 - (255 - a) * (255 - a0) / 255;
                if (0 != A) {
                    pix.buf[dstIndex]     = (r * a + r0 * a0 * (255 - a) / 255) / A;
                    pix.buf[dstIndex + 1] = (g * a + g0 * a0 * (255 - a) / 255) / A;
                    pix.buf[dstIndex + 2] = (b * a + b0 * a0 * (255 - a) / 255) / A;
                    pix.buf[dstIndex + 3] =  A;
                }
            }
        }

        free(pPixSrc[i].buf);
    }

    free(pPixSrc);
    free(pOffsetX);
    free(pOffsetY);
    free(pLayer);
    return convert_pix(&pix, 1);
}

unsigned char pix::CLIP_0_255(int a)
{
    if (a < 0) return static_cast<unsigned char>(0);
    else if (a > 255) return static_cast<unsigned char>(255);
    else return static_cast<unsigned char>(a);
}

TCFX_PyPix pix::convert_pix(const TCFX_pPix pPix, int delPix)
{
    int i;
    py::tuple pyTemp = py::make_tuple(pPix->initX, pPix->initY);
    py::tuple pyTemp2 = py::make_tuple(pPix->width, pPix->height);
    py::list tmp_list;
    for (i = 0; i < pPix->size; ++i)
    {
        tmp_list.append(pPix->buf[i]);
    }
    py::tuple pyTemp3 = py::tuple(tmp_list);
    if (delPix) free(pPix->buf);
    return py::make_tuple(pyTemp, pyTemp2, pyTemp3);
}

TCFX_pPix pix::convert_py_pix(const py::tuple &pyPix, TCFX_pPix pPix)
{
    int i;
    py::tuple pos, size, buf;
    pos = py::extract<py::tuple>(pyPix[0]);
    size = py::extract<py::tuple>(pyPix[1]);
    buf = py::extract<py::tuple>(pyPix[2]);

    pPix->initX = py::extract<double>(pos[0]);
    pPix->initY = py::extract<double>(pos[1]);
    pPix->width = py::extract<int>(size[0]);
    pPix->height = py::extract<int>(size[1]);

    pPix->size = pPix->height * (pPix->width << 2);
    pPix->buf = (unsigned char *)malloc(pPix->size * sizeof(unsigned char));

    for (i = 0; i < pPix->size; ++i)
        pPix->buf[i] = py::extract<unsigned char>(buf[i]);
    return pPix;
}

void pix::points_append(TCFX_pPoints pPoints, double x, double y, unsigned char a)
{
    double *xBuf, *yBuf;
    unsigned char *aBuf;
    int count, capacity;
    xBuf = pPoints->xBuf;
    yBuf = pPoints->yBuf;
    aBuf = pPoints->aBuf;
    count = pPoints->count;

    capacity = pPoints->capacity;
    if (count + 1 > capacity)
    {  /* we need more space */
        capacity += capacity / 2 + 1;
        xBuf = (double *)realloc(xBuf, capacity * sizeof(double));
        yBuf = (double *)realloc(yBuf, capacity * sizeof(double));
        aBuf = (unsigned char *)realloc(aBuf, capacity * sizeof(unsigned char));
        pPoints->xBuf = xBuf;
        pPoints->yBuf = yBuf;
        pPoints->aBuf = aBuf;
        pPoints->capacity = capacity;
    }
    
    xBuf[count] = x;
    yBuf[count] = y;
    aBuf[count] = a;
    count++;
    pPoints->count = count;
}

/* private */

double pix::filter_cubic(double x, double a)
{
    if (x < 0) x = -x;    /* x = fabs(x) */
    if (x < 1) return (a + 2) * x * x * x - (a + 3) * x * x + 1;
    else if (x < 2) return a * x * x * x - 5 * a * x * x + 8 * a * x - 4 * a;
    else return 0;
}

void pix::resample_rgba(const unsigned char *src, int width, int height,
                        unsigned char *dst, int targetWidth, int targetHeight)
{
    int h, w, m, n, index;
    double fx, fy;
    int ix, iy, xx, yy;
    double xScale, yScale, r1, r2;
    double rr, gg, bb, aa;
    if (targetWidth == width && targetHeight == height)
        memcpy(dst, src, height * (width << 2) * sizeof(unsigned char));
    else {
        xScale = targetWidth / (double)width;
        yScale = targetHeight / (double)height;
        for (h = 0; h < targetHeight; h++) {
            fy = h / yScale;
            iy = (int)fy;
            for (w = 0; w < targetWidth; w++) {
                fx = w / xScale;
                ix = (int)fx;
                rr = 0;
                gg = 0;
                bb = 0;
                aa = 0;
                for (m = 0; m < 4; m++) {
                    yy = iy + m - 1;
                    r1 = filter_cubic(yy - fy, -0.5);    /* can use _tcfxlib_filter_MitchellNetravali (best quality but slowest) or _tcfxlib_filter_BSpline (smoothest) */
                    if (yy < 0) yy = 0;
                    if (yy >= height) yy = height - 1;
                    for (n = 0; n < 4; n++) {
                        xx = ix + n - 1;
                        r2 = r1 * filter_cubic(xx - fx, -0.5);    /* can use _tcfxlib_filter_MitchellNetravali (best quality but slowest) or _tcfxlib_filter_BSpline (smoothest) */
                        if (xx < 0) xx = 0;
                        if (xx >= width) xx = width - 1;
                        index = (yy * width + xx) << 2;
                        rr += src[index] * r2;
                        gg += src[index + 1] * r2;
                        bb += src[index + 2] * r2;
                        aa += src[index + 3] * r2;
                    }
                }
                index = (h * targetWidth + w) << 2;
                dst[index] = CLIP_0_255((int)rr);
                dst[index + 1] = CLIP_0_255((int)gg);
                dst[index + 2] = CLIP_0_255((int)bb);
                dst[index + 3] = CLIP_0_255((int)aa);
            }
        }
    }
}

TCFX_pPix pix::resample_pix(const TCFX_pPix pPixSrc, TCFX_pPix pPixDst)
{
    if (0 == pPixDst->width) pPixDst->width = pPixDst->height * pPixSrc->width / pPixSrc->height;    /* keep aspect ratio */
    else if (0 == pPixDst->height) pPixDst->height = pPixDst->width * pPixSrc->height / pPixSrc->width;    /* keep aspect ratio */
    pPixDst->initX = pPixSrc->initX - (pPixDst->width - pPixSrc->width) / 2;
    pPixDst->initY = pPixSrc->initY - (pPixDst->height - pPixSrc->height) / 2;
    pPixDst->size = pPixDst->height * (pPixDst->width << 2);
    pPixDst->buf = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    resample_rgba(pPixSrc->buf, pPixSrc->width, pPixSrc->height, pPixDst->buf, pPixDst->width, pPixDst->height);
    return pPixDst;
}

TCFX_pPix pix::resample_pix_ex(const TCFX_pPix pPixSrc,
                               TCFX_pPix pPixDst,
                               double width, double height)
{
    const int m = 4;
    double f_w, f_h, f_mw, f_mh;
    int i_mw, i_mh, pad_w, pad_h;
    int sf_mw, sf_mh, sf_msize;
    unsigned char *sBuf, *bufm;
    int i, index1, index2, size;
    f_w = width;
    f_h = height;
    if (0 == f_w) f_w = f_h * pPixSrc->width / (double)pPixSrc->height;    /* keep aspect ratio */
    else if (0 == f_h) f_h = f_w * pPixSrc->height / (double)pPixSrc->width;    /* keep aspect ratio */
    f_mw = m * f_w;
    f_mh = m * f_h;
    i_mw = (int)f_mw;
    i_mh = (int)f_mh;
    pad_w = m - i_mw % m;
    if (m == pad_w)
        pad_w = 0;
    pad_h = m - i_mh % m;
    if (m == pad_h)
        pad_h = 0;
    sf_mw = i_mw + pad_w;
    sf_mh = i_mh + pad_h;
    sf_msize = sf_mh * (sf_mw << 2);
    sBuf = (unsigned char *)malloc(sf_msize * sizeof(unsigned char));
    memset(sBuf, 0, sf_msize * sizeof(unsigned char));      /* now we've got the surface to draw */
    bufm = (unsigned char *)malloc(i_mh * (i_mw << 2) * sizeof(unsigned char));   /* magnitude the source to m * m */
    resample_rgba(pPixSrc->buf, pPixSrc->width, pPixSrc->height, bufm, i_mw, i_mh);
    size = (i_mw << 2) * sizeof(unsigned char);
    for (i = 0; i < i_mh; i++) {
        index1 = i * (sf_mw << 2) * sizeof(unsigned char);
        index2 = i * (i_mw << 2) * sizeof(unsigned char);
        memcpy(sBuf + index1, bufm + index2, size);
    }
    free(bufm);
    pPixDst->initX = pPixSrc->initX - (f_w - pPixSrc->width) / 2;
    pPixDst->initY = pPixSrc->initY - (f_h - pPixSrc->height) / 2;
    pPixDst->width = sf_mw / m;
    pPixDst->height = sf_mh / m;
    pPixDst->size = pPixDst->height * (pPixDst->width << 2);
    pPixDst->buf = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    resample_rgba(sBuf, sf_mw, sf_mh, pPixDst->buf, pPixDst->width, pPixDst->height);
    free(sBuf);
    return pPixDst;
}

TCFX_pPix pix::enlarge_pix(const TCFX_pPix pPixSrc, TCFX_pPix pPixDst)
{
    int w, h, offset_x, offset_y, srcIndex, dstIndex;
    offset_x = (pPixDst->width - pPixSrc->width) / 2;
    offset_y = (pPixDst->height - pPixSrc->height) / 2;
    pPixDst->initX = pPixSrc->initX - offset_x;
    pPixDst->initY = pPixSrc->initY - offset_y;
    pPixDst->size = pPixDst->height * (pPixDst->width << 2);
    pPixDst->buf = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    memset(pPixDst->buf, 0, pPixDst->size * sizeof(unsigned char));
    for (w = 0; w < pPixSrc->width; ++w)
    {
        for (h = 0; h < pPixSrc->height; ++h)
        {
            dstIndex = ((h + offset_y) * pPixDst->width + w + offset_x) << 2;
            srcIndex = (h * pPixSrc->width + w) << 2;
            memcpy(pPixDst->buf + dstIndex, pPixSrc->buf + srcIndex, 4 * sizeof(unsigned char));
        }
    }
    return pPixDst;
}

/* Gauss IIR blur */
/* Convert from separated to premultiplied alpha, on a single scan line. */
void pix::multiply_alpha(unsigned char *buf, int length)
{
    int i, index;
    double a;
    for (i = 0; i < length; ++i)
    {
        index = i << 2;
        a = buf[index + 3] / 255.0;
        buf[index] = (unsigned char)(buf[index] * a);
        buf[index + 1] = (unsigned char)(buf[index + 1] * a);
        buf[index + 2] = (unsigned char)(buf[index + 2] * a);
    }
}

/* Convert from premultiplied to separated alpha, on a single scan line. */
void pix::separate_alpha(unsigned char *buf, int length)
{
    int i, index;
    unsigned char alpha;
    double a;
    for (i = 0; i < length; ++i)
    {
        index = i << 2;
        alpha = buf[index + 3];
        if (0 != alpha && 255 != alpha)
        {
            a = 255 / (double)alpha;
            buf[index] = min(255, (int)(buf[index] * a));
            buf[index + 1] = min(255, (int)(buf[index + 1] * a));
            buf[index + 2] = min(255, (int)(buf[index + 2] * a));
        }
    }
}

void pix::gauss_iir_find_constants(double *n_p, double *n_m, double *d_p,
                                   double *d_m, double *bd_p, double *bd_m, double std_dev)
{
    int i;
    double constants[8];
    double div;
    double sum_n_p, sum_n_m, sum_d_p, a, b;
    /* The constants used in the implemenation of a casual sequence using a 4th order approximation of the gaussian operator */
    div = sqrt(2 * MATH_PI) * std_dev;
    constants[0] = -1.783 / std_dev;
    constants[1] = -1.723 / std_dev;
    constants[2] = 0.6318 / std_dev;
    constants[3] = 1.997  / std_dev;
    constants[4] = 1.6803 / div;
    constants[5] = 3.735 / div;
    constants[6] = -0.6803 / div;
    constants[7] = -0.2598 / div;
    n_p[0] = constants[4] + constants[6];
    n_p[1] = exp(constants[1]) * (constants[7] * sin(constants[3]) - (constants[6] + 2 * constants[4]) * cos(constants[3])) + exp(constants[0]) * (constants[5] * sin(constants[2]) - (2 * constants[6] + constants[4]) * cos(constants[2]));
    n_p[2] = 2 * exp(constants[0] + constants[1]) * ((constants[4] + constants[6]) * cos(constants[3]) * cos(constants[2]) - constants[5] * cos(constants[3]) * sin(constants[2]) - constants[7] * cos(constants[2]) * sin(constants[3])) + constants[6] * exp(2 * constants[0]) + constants[4] * exp(2 * constants[1]);
    n_p[3] = exp(constants[1] + 2 * constants[0]) * (constants[7] * sin(constants[3]) - constants[6] * cos(constants[3])) + exp(constants[0] + 2 * constants[1]) * (constants[5] * sin(constants[2]) - constants[4] * cos(constants[2]));
    n_p[4] = 0;
    d_p[0] = 0;
    d_p[1] = -2 * exp(constants[1]) * cos(constants[3]) - 2 * exp(constants[0]) * cos(constants[2]);
    d_p[2] = 4 * cos(constants[3]) * cos(constants[2]) * exp(constants[0] + constants[1]) + exp(2 * constants[1]) + exp(2 * constants[0]);
    d_p[3] = -2 * cos(constants[2]) * exp(constants[0] + 2 * constants[1]) - 2 * cos(constants[3]) * exp(constants[1] + 2 * constants[0]);
    d_p[4] = exp(2 * constants[0] + 2 * constants[1]);
    for (i = 0; i < 5; ++i)
        d_m[i] = d_p[i];
    n_m[0] = 0;
    for (i = 1; i < 5; ++i)
        n_m[i] = n_p[i] - d_p[i] * n_p[0];
    sum_n_p = 0;
    sum_n_m = 0;
    sum_d_p = 0;
    for (i = 0; i < 5; ++i)
    {
        sum_n_p += n_p[i];
        sum_n_m += n_m[i];
        sum_d_p += d_p[i];
    }
    a = sum_n_p / (1 + sum_d_p);
    b = sum_n_m / (1 + sum_d_p);
    for (i = 0; i < 5; ++i)
    {
        bd_p[i] = d_p[i] * a;
        bd_m[i] = d_m[i] * b;
    }
}

void pix::gauss_iir_blur(unsigned char *img, int width, int height, int vertRadius, int horiRadius)
{
    int i, j, b, x, y, vert, hori, size, terms;
    double n_p[5], n_m[5], d_p[5], d_m[5], bd_p[5], bd_m[5];
    double std_dev;
    int initial_p[4], initial_m[4];
    unsigned char *src;
    unsigned char *sp_p, *sp_m, *p;
    double *val_p, *val_m;
    double *vp, *vm, *vpptr, *vmptr, *pTmp_p, *pTmp_m;
    /* First the vertical pass */
    vert = vertRadius + 1;
    std_dev = sqrt(-(vert * vert) / (2 * log(1 / 255.0)));
    /* derive the constants for calculating the gaussian from the std dev */
    gauss_iir_find_constants(n_p, n_m, d_p, d_m, bd_p, bd_m, std_dev);
    size = height << 2;
    val_p = (double *)malloc(size * sizeof(double));
    val_m = (double *)malloc(size * sizeof(double));
    src = (unsigned char *)malloc(size * sizeof(unsigned char));
    for (x = 0; x < width; ++x)
    {
        memset(val_p, 0, size * sizeof(double));
        memset(val_m, 0, size * sizeof(double));
        /* get col */
        for (y = 0; y < height; ++y)
            memcpy(&src[y << 2], &img[(y * width + x) << 2], sizeof(unsigned char) << 2);
        multiply_alpha(src, height);
        sp_p = src;
        sp_m = src + (height - 1) * 4;
        vp = val_p;
        vm = val_m + (height - 1) * 4;
        /* Set up the first vals */
        for (i = 0; i < 4; ++i)
        {
            initial_p[i] = sp_p[i];
            initial_m[i] = sp_m[i];
        }
        for (y = 0; y < height; ++y)
        {
            terms = min(y, 4);
            for (b = 0; b < 4; ++b)
            {
                vpptr = vp + b;
                vmptr = vm + b;
                for (i = 0; i <= terms; i++) {
                    *vpptr += n_p[i] * sp_p[(-i * 4) + b] - d_p[i] * vp[(-i * 4) + b];
                    *vmptr += n_m[i] * sp_m[(i * 4) + b] - d_m[i] * vm[(i * 4) + b];
                }
                for (j = i ; j <= 4 ; ++j) {
                    *vpptr += (n_p[j] - bd_p[j]) * initial_p[b];
                    *vmptr += (n_m[j] - bd_m[j]) * initial_m[b];
                }
            }
            sp_p += 4;
            sp_m -= 4;
            vp += 4;
            vm -= 4;
        }
        pTmp_p = val_p;
        pTmp_m = val_m;
        for (i = 0; i < height; ++i)
        {
            p = &img[(i * width + x) << 2];
            for (b = 0; b < 4; ++b)
            {
                p[b] = CLIP_0_255((int)(*pTmp_p + *pTmp_m));
                pTmp_p++;
                pTmp_m++;
            }
        }
    }
    /* Now the horizontal pass */
    hori = horiRadius + 1;
    std_dev = sqrt(-(hori * hori) / (2 * log(1 / 255.0)));
    /*derive the constants for calculating the gaussian from the std dev */
    gauss_iir_find_constants(n_p, n_m, d_p, d_m, bd_p, bd_m, std_dev);
    free(val_p);
    free(val_m);
    free(src);
    size = width << 2;
    val_p = (double *)malloc(size * sizeof(double));
    val_m = (double *)malloc(size * sizeof(double));
    src = (unsigned char *)malloc(size * sizeof(unsigned char));
    for (y = 0; y < height; ++y)
    {
        memset(val_p, 0, size * sizeof(double));
        memset(val_m, 0, size * sizeof(double));
        /* get row */
        memcpy(src, &img[y * (width << 2)], size * sizeof(unsigned char));
        sp_p = src;
        sp_m = src + (width - 1) * 4;
        vp = val_p;
        vm = val_m + (width - 1) * 4;
        /* Set up the first vals */
        for (i = 0; i < 4; ++i)
        {
            initial_p[i] = sp_p[i];
            initial_m[i] = sp_m[i];
        }
        for (x = 0; x < width; ++x)
        {
            terms = min(x, 4);
            for (b = 0; b < 4; ++b)
            {
                vpptr = vp + b;
                vmptr = vm + b;
                for (i = 0; i <= terms; ++i)
                {
                    *vpptr += n_p[i] * sp_p[(-i * 4) + b] - d_p[i] * vp[(-i * 4) + b];
                    *vmptr += n_m[i] * sp_m[(i * 4) + b] - d_m[i] * vm[(i * 4) + b];
                }
                for (j = i; j <= 4; ++j)
                {
                    *vpptr += (n_p[j] - bd_p[j]) * initial_p[b];
                    *vmptr += (n_m[j] - bd_m[j]) * initial_m[b];
                }
            }
            sp_p += 4;
            sp_m -= 4;
            vp += 4;
            vm -= 4;
        }
        pTmp_p = val_p;
        pTmp_m = val_m;
        for (i = 0; i < width; ++i)
        {
            p = &img[(y * width + i) << 2];
            for (b = 0; b < 4; ++b)
            {
                p[b] = CLIP_0_255((int)(*pTmp_p + *pTmp_m));
                pTmp_p++;
                pTmp_m++;
            }
        }
        separate_alpha(&img[y * (width << 2)], width);
    }
    free(val_p);
    free(val_m);
    free(src);
}

TCFX_pPix pix::bilinear_filter_internal(const TCFX_pPix pPixSrc, double offset_x, double offset_y, TCFX_pPix pPixDst)
{
    int h, w, index, index1, index2, index3, index4;
    double initPosX, initPosY;
    double u_r, v_r, u_o, v_o;
    unsigned char *buf1, *buf2, *buf3, *buf4;    /* top-left, top-right, bottom-left, bottom-right */
    initPosX = pPixSrc->initX + offset_x;
    initPosY = pPixSrc->initY + offset_y;
    pPixDst->initX = pPixSrc->initX;    /* when computing initX we need to use float, but when come to write them to TCAS file we will use integer instead */
    pPixDst->initY = pPixSrc->initY;
    pPixDst->width = pPixSrc->width + 1;
    pPixDst->height = pPixSrc->height + 1;
    pPixDst->size = pPixDst->height * (pPixDst->width << 2);
    buf1 = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    buf2 = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    buf3 = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    buf4 = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    memset(buf1, 0, pPixDst->size * sizeof(unsigned char));
    memset(buf2, 0, pPixDst->size * sizeof(unsigned char));
    memset(buf3, 0, pPixDst->size * sizeof(unsigned char));
    memset(buf4, 0, pPixDst->size * sizeof(unsigned char));
    u_r = initPosX - (int)initPosX;    /* factor for the right */
    if (u_r < 0) ++u_r;
    v_r = initPosY - (int)initPosY;    /* factor for the bottom */
    if (v_r < 0) ++v_r;
    u_o = 1 - u_r;    /* factor for the left */
    v_o = 1 - v_r;    /* factor for the top */
    for (h = 0; h < pPixSrc->height; ++h)
    {
        for (w = 0; w < pPixSrc->width; ++w)
        {
            index = (h * pPixSrc->width + w) << 2;
            index1 = (h * pPixDst->width + w) << 2;
            index2 = (h * pPixDst->width + w + 1) << 2;
            index3 = ((h + 1) * pPixDst->width + w) << 2;
            index4 = ((h + 1) * pPixDst->width + w + 1) << 2;
            memcpy(buf1 + index1, pPixSrc->buf + index, 4 * sizeof(unsigned char));
            memcpy(buf2 + index2, pPixSrc->buf + index, 4 * sizeof(unsigned char));
            memcpy(buf3 + index3, pPixSrc->buf + index, 4 * sizeof(unsigned char));
            memcpy(buf4 + index4, pPixSrc->buf + index, 4 * sizeof(unsigned char));
        }
    }
    pPixDst->buf = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    for (h = 0; h < pPixDst->height; ++h)
    {
        for (w = 0; w < pPixDst->width; ++w)
        {
            index = (h * pPixDst->width + w) << 2;
            pPixDst->buf[index]     = CLIP_0_255((int)((buf1[index]     * u_o + buf2[index]     * u_r) * v_o + (buf3[index]     * u_o + buf4[index]     * u_r) * v_r));
            pPixDst->buf[index + 1] = CLIP_0_255((int)((buf1[index + 1] * u_o + buf2[index + 1] * u_r) * v_o + (buf3[index + 1] * u_o + buf4[index + 1] * u_r) * v_r));
            pPixDst->buf[index + 2] = CLIP_0_255((int)((buf1[index + 2] * u_o + buf2[index + 2] * u_r) * v_o + (buf3[index + 2] * u_o + buf4[index + 2] * u_r) * v_r));
            pPixDst->buf[index + 3] = CLIP_0_255((int)((buf1[index + 3] * u_o + buf2[index + 3] * u_r) * v_o + (buf3[index + 3] * u_o + buf4[index + 3] * u_r) * v_r));
        }
    }
    free(buf1);
    free(buf2);
    free(buf3);
    free(buf4);
    return pPixDst;
}

TCFX_pPix pix::scale_filter_internal(const TCFX_pPix pPixSrc, double offset_x, double offset_y, TCFX_pPix pPixDst)
{
    const int m = 4;
    int i_w, i_h, i_mw, i_mh;
    double f_dx, f_dy, f_mdx, f_mdy;
    int i_mdx, i_mdy;
    int pad_dx_i, pad_dy_i, pad_dx, pad_dy;
    int sf_mw, sf_mh, sf_msize;
    unsigned char *sBuf, *bufm;
    int i, index1, index2, size;
    i_w = pPixSrc->width;
    i_h = pPixSrc->height;
    i_mw = m * i_w;
    i_mh = m * i_h;
    f_dx = pPixSrc->initX + offset_x;
    f_dy = pPixSrc->initY + offset_y;
    f_mdx = m * f_dx;
    f_mdy = m * f_dy;
    i_mdx = (int)f_mdx;
    i_mdy = (int)f_mdy;
    pad_dx_i = i_mdx % m;
    pad_dx = 0;
    if (0 != pad_dx_i)
        pad_dx = m;
    pad_dy_i = i_mdy % m;
    pad_dy = 0;
    if (0 != pad_dy_i)
        pad_dy = m;
    sf_mw = i_mw + pad_dx;
    sf_mh = i_mh + pad_dy;
    sf_msize = sf_mh * (sf_mw << 2);
    sBuf = (unsigned char *)malloc(sf_msize * sizeof(unsigned char));
    memset(sBuf, 0, sf_msize * sizeof(unsigned char));      /* now we've got the surface to draw */
    bufm = (unsigned char *)malloc(i_mh * (i_mw << 2) * sizeof(unsigned char));   /* magnitude the source to m * m */
    resample_rgba(pPixSrc->buf, i_w, i_h, bufm, i_mw, i_mh);
    size = (i_mw << 2) * sizeof(unsigned char);
    for (i = 0; i < i_mh; ++i)
    {
        index1 = (((i + pad_dy_i) * sf_mw + pad_dx_i) << 2) * sizeof(unsigned char);
        index2 = i * (i_mw << 2) * sizeof(unsigned char);
        memcpy(sBuf + index1, bufm + index2, size);
    }
    free(bufm);
    pPixDst->initX = pPixSrc->initX;
    pPixDst->initY = pPixSrc->initY;
    pPixDst->width = sf_mw / m;
    pPixDst->height = sf_mh / m;
    pPixDst->size = pPixDst->height * (pPixDst->width << 2);
    pPixDst->buf = (unsigned char *)malloc(pPixDst->size * sizeof(unsigned char));
    resample_rgba(sBuf, sf_mw, sf_mh, pPixDst->buf, pPixDst->width, pPixDst->height);
    free(sBuf);
    return pPixDst;
}
