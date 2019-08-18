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

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include "image.h"

TCFX_PyPix image::get_pix_from_image(const char *filename, int dst_width, int dst_height)
{

    TCFX_Pix pix;
    FILE *fp;
    png_byte sig[PNG_SIG_BYTES];
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytepp png_data;
    png_byte color_type, bit_depth;
    int i, index, w, h, width, height, rowbytes;

    fp = fopen(filename, "rb");
    if (!fp) {
        PyErr_SetString(PyExc_RuntimeError, "ImagePix error, failed to open the image!\n");
        return py::tuple();
    }
    if (fread(sig, sizeof(png_byte), PNG_SIG_BYTES, fp) != PNG_SIG_BYTES) {
        fclose(fp);
        PyErr_SetString(PyExc_RuntimeError, "ImagePix error, failed to read the image!\n");
        return py::tuple();
    }
    if (png_sig_cmp(sig, 0, PNG_SIG_BYTES) != 0) {
        fclose(fp);
        PyErr_SetString(PyExc_RuntimeError, "ImagePix error, the file is not a PNG file!\n");
        return py::tuple();
    }
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_set_sig_bytes(png_ptr, PNG_SIG_BYTES);
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    /* convert all types to rgba */
    if (PNG_COLOR_TYPE_GRAY == color_type && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (PNG_COLOR_TYPE_GRAY == color_type || PNG_COLOR_TYPE_GRAY_ALPHA == color_type)
        png_set_gray_to_rgb(png_ptr);
    if (PNG_COLOR_TYPE_PALETTE == color_type)
        png_set_palette_to_rgb(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    else
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    if (16 == bit_depth)
        png_set_strip_16(png_ptr);
    png_read_update_info(png_ptr, info_ptr);
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_data = (png_bytepp)malloc(height * sizeof(png_bytep));
    for (i = 0; i < height; i++)
        png_data[i] = (png_bytep)malloc(rowbytes * sizeof(png_byte));
    png_read_image(png_ptr, png_data);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    pix.initX = 0;
    pix.initY = 0;
    pix.width = width;
    pix.height = height;
    pix.size = pix.height * (pix.width << 2);
    pix.buf = (unsigned char *)malloc(pix.size * sizeof(unsigned char));
    for (h = 0; h < height; ++h)
    {
        for (w = 0; w < width; ++w)
        {
            index = (h * width + w) << 2;
            memcpy(pix.buf + index, png_data[h] + (w << 2), 4 * sizeof(unsigned char));
        }
    }

    for (i = 0; i < height; ++i)
        free(png_data[i]);
    free(png_data);
    if ((dst_width > 0) && (dst_height > 0))
    {
        TCFX_Pix pixDst;
        pixDst.width = dst_width;
        pixDst.height = dst_height;
        Pix.resample_pix(&pix, &pixDst);
        free(pix.buf);
        pixDst.initX = 0;   /* since tcfxlib_resample_pix function will change the value of initX and initY */
        pixDst.initY = 0;
        return Pix.convert_pix(&pixDst, 1);
    } else return Pix.convert_pix(&pix, 1);
}

bool image::save_pix_to_image(const char *filename, TCFX_PyPix &PIX,
                              int dst_width, int dst_height)
{
    TCFX_Pix pixSource;
    TCFX_Pix pix;
    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytepp png_data;
    int w, h, rowbytes;

    Pix.convert_py_pix(PIX, &pixSource);
    if ((dst_width > 0) && (dst_height > 0))
    {
        pix.width = dst_width;
        pix.height = dst_height;
        Pix.resample_pix(&pixSource, &pix);
        free(pixSource.buf);
    } else pix = pixSource;
    rowbytes = pix.width << 2;
    png_data = (png_bytepp)malloc(pix.height * sizeof(png_bytep));
    for (h = 0; h < pix.height; ++h)
    {
        png_data[h] = (png_bytep)malloc(rowbytes * sizeof(png_byte));
        memset(png_data[h], 0, rowbytes * sizeof(png_byte));
    }
    for (h = 0; h < pix.height; ++h)
    {
        for (w = 0; w < pix.width; ++w)
            memcpy(png_data[h] + (w << 2), pix.buf + ((h * pix.width + w) << 2), 4 * sizeof(unsigned char));
    }
    free(pix.buf);
    fp = fopen(filename, "wb");
    if (!fp)
    {
        PyErr_SetString(PyExc_RuntimeError, "SavePix error, failed to create the image!\n");
        return false;
    }
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, pix.width, pix.height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, png_data);
    png_write_end(png_ptr, NULL);
    for (h = 0; h < pix.height; ++h)
        free(png_data[h]);
    free(png_data);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}
