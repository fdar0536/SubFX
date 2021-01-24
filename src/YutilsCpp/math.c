/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

#include "YutilsCpp/math.h"
#include "internal/common.h"
#include "utils/vector.h"
#include "utils/math.h"

#include "config.h"

#ifdef SUBFX_ENABLE_SIMD
#ifdef SUBFX_ENABLE_AVX
#include "immintrin.h" // AVX
#else
#include "emmintrin.h" // SSE
#endif // SUBFX_ENABLE_AVX
#endif // SUBFX_ENABLE_SIMD

#define subfx_max(x, y) x > y ? x : y
#define subfx_min(x, y) x < y ? x : y

subfx_yutils_math *subfx_yutils_math_init()
{
    subfx_yutils_math *ret = calloc(1, sizeof(subfx_yutils_math));
    if (!ret)
    {
        return NULL;
    }

    ret->arc_curve = subfx_yutils_math_arc_curve;
    ret->bezier = subfx_yutils_math_bezier;
    ret->degree = subfx_yutils_math_degree;
    ret->distance = subfx_yutils_math_distance;
    ret->line_intersect = subfx_yutils_math_line_intersect;
    ret->ortho = subfx_yutils_math_ortho;
    ret->randomsteps = subfx_yutils_math_randomsteps;
    ret->round = subfx_yutils_math_round;
    ret->stretch = subfx_yutils_math_stretch;
    ret->trim = subfx_yutils_math_trim;
    ret->ellipse = subfx_yutils_math_ellipse;
    ret->randomway = subfx_yutils_math_randomway;
    ret->rotate = subfx_yutils_math_rotate;

    return ret;
}

#define checkError \
    if (!ptr) \
    { \
        subfx_vector_destroy(curves); \
        subfx_pError(errMsg, "arc_curve: Fail to allocate memory.") \
        return NULL; \
    }

#define pushback(x) \
    if (subfx_vector_pushback(curves, x) == subfx_failed) \
    { \
        subfx_vector_destroy(curves); \
        subfx_pError(errMsg, "arc_curve: Fail to allocate memory.") \
        return NULL; \
    }

subfx_handle
subfx_yutils_math_arc_curve(double x, double y,
                            double cx, double cy,
                            double angle,
                            char *errMsg)
{
    if (angle < -360. || angle > 360.)
    {
        subfx_pError(errMsg,"arc_curve: start & center point and "
                             "valid angle (-360<=x<=360) expected")
        return NULL;
    }

    if (angle == 0.)
    {
        subfx_pError(errMsg, "arc_curve: angle CANNOT be zero")
        return NULL;
    }

    subfx_handle *curves = subfx_vector_create(sizeof(double));
    if (!curves)
    {
        subfx_pError(errMsg, "arc_curve: Fail to create vector.")
        return NULL;
    }

    // Factor for bezier control points distance to node points
    double kappa = 4. * (sqrt(2.) - 1.) / 3.;
    double rx0, ry0, rx1, ry1, rx2, ry2, rx3 = 0., ry3, rx03, ry03;

    rx0 = x - cx;
    ry0 = y - cy;

    double cw = (angle > 0. ? 1. : -1.);

    // convert do-while-loop to while-loop
    size_t curves_n = 4;

    // 4 * 2 = 8
    if (subfx_vector_reserve(curves, 8) == subfx_failed)
    {
        subfx_vector_destroy(curves);
        subfx_pError(errMsg, "arc_curve: Fail to allocate memory.")
        return NULL;
    }

    double angle_sum = 0.;
    double cur_angle_pct;
    // std::pair<double, double> tmpPair;
    double *ptr;
    double tmpDouble;

    while (angle_sum < angle)
    {
        cur_angle_pct = subfx_min((angle - angle_sum),
                                  ((double)90. / 90.));

        ptr = subfx_utils_math_rotate2d(rx0, ry0,
                                        cw * 90. * cur_angle_pct);
        checkError

        rx3 = ptr[0];
        ry3 = ptr[1];
        free(ptr);

        // arc start to end vector
        rx03 = rx3 - rx0;
        ry03 = ry3 - ry0;

        // Scale arc vector to curve node <-> control point distance
        tmpDouble = subfx_yutils_math_distance(rx03, ry03, 0.);
        tmpDouble = pow(tmpDouble, 2) / 2.;
        tmpDouble = sqrt(tmpDouble);

        ptr = subfx_yutils_math_stretch(rx03, ry03, 0,
                                        tmpDouble * kappa);
        checkError

        rx03 = ptr[0];
        ry03 = ptr[1];
        free(ptr);

        ptr = subfx_utils_math_rotate2d(rx03, ry03,
                                        cw * (-45.) * cur_angle_pct);
        checkError

        // Get curve control points
        rx1 = rx0 + ptr[0];
        ry1 = ry0 + ptr[1];
        free(ptr);

        ptr = subfx_utils_math_rotate2d(rx03 * -1., ry03 * -1.,
                                        cw * 45. * cur_angle_pct);
        checkError

        rx2 = rx3 + ptr[0];
        ry2 = ry3 + ptr[1];
        free(ptr);

        // Insert curve to output
        tmpDouble = cx + rx0;
        pushback(&tmpDouble);

        tmpDouble = cy + ry0;
        pushback(&tmpDouble);

        tmpDouble = cx + rx1;
        pushback(&tmpDouble);

        tmpDouble = cy + ry1;
        pushback(&tmpDouble);

        tmpDouble = cx + rx2;
        pushback(&tmpDouble);

        tmpDouble = cy + ry2;
        pushback(&tmpDouble);

        tmpDouble = cx + rx3;
        pushback(&tmpDouble);

        tmpDouble = cy + ry3;
        pushback(&tmpDouble);

        curves_n += 4;

        if (subfx_vector_reserve(curves, curves_n << 1)
                == subfx_failed)
        {
            subfx_vector_destroy(curves);
            subfx_pError(errMsg, "arc_curve: Fail to allocate memory.")
            return NULL;
        }

        // Prepare next curve
        rx0 = rx3;
        ry0 = ry3;
        angle_sum += 90.;
    }

    return curves;
}

#undef checkError
#undef pushback

double
*subfx_yutils_math_bezier(double pct,
                          double *pts,
                          size_t ptsCount,
                          bool is3D,
                          char *errMsg)
{
    if (pct < 0. || pct > 1.)
    {
        subfx_pError(errMsg, "bezier: pct must between 0 and 1")
        return NULL;
    }

    if (!pts)
    {
        subfx_pError(errMsg, "bezier: pct is NULL.")
        return NULL;
    }

    if (ptsCount < 2)
    {
        subfx_pError(errMsg, "bezier: at least 2 points expected.")
        return NULL;
    }

    double *ret = calloc(4, sizeof(double));
    if (!ret)
    {
        subfx_pError(errMsg, "bezier: Fail to allocate memory.")
        return NULL;
    }

    switch(ptsCount)
    {
    case 2:
    {
        bezier2(pct, pts, is3D, ret);
        break;
    }
    case 3:
    {
        bezier3(pct, pts, is3D, ret);
        break;
    }
    case 4:
    {
        bezier4(pct, pts, is3D, ret);
        break;
    }
    default:
    {
        bezierN(pct, pts, ptsCount, is3D, ret);
        break;
    }
    } // end switch

    return ret;
}

double
subfx_yutils_math_degree(double x1, double y1, double z1,
                         double x2, double y2, double z2)
{
    double degree = subfx_yutils_math_distance(
                x1, y1, z1) * subfx_yutils_math_distance(x2, y2, z2);
    if (degree == 0.)
    {
        return 0.;
    }

    degree = acos((x1 * x2 + y1 * y2 + z1 * z2) / degree);
    degree = subfx_utils_math_deg(degree);

    // Return with sign by clockwise direction
    if ((x1 * y2 - y1 * x2) < 0)
    {
        return (degree * -1.);
    }

    return degree;
}

double
subfx_yutils_math_distance(double x, double y, double z)
{
    return sqrt(x * x + y * y + z * z);
}

double
*subfx_yutils_math_line_intersect(double x0, double y0,
                                  double x1, double y1,
                                  double x2, double y2,
                                  double x3, double y3,
                                  int strict, char *errMsg)
{
    // Get line vectors & check valid lengths
    double x10 = x0 - x1;
    double y10 = y0 - y1;
    double x32 = x2 - x3;
    double y32 = y2 - y3;

    if ((x10 == 0. && y10 == 0.) ||
        (x32 == 0. && y32 == 0.))
    {
        subfx_pError(errMsg, "line_intersect: lines mustn't have "
                             "zero length")
        return NULL;
    }

    double *ret = calloc(2, sizeof(double));
    if (!ret)
    {
        subfx_pError(errMsg, "line_intersect: Fail to allocate memory.")
        return NULL;
    }

    // Calculate determinant and check for parallel lines
    double det = x10 * y32 - y10 * x32;
    if (det == 0.)
    {
        return ret;
    }

    // Calculate line intersection (endless line lengths)
    double pre = x0 * y1 - y0 * x1;
    double post = x2 * y3 - y2 * x3;

    double ix = (pre * x32 - x10 * post) / det;
    double iy = (pre * y32 - y10 * post) / det;

    if (strict)
    {
        double s = (x10 != 0. ? (ix - x1) / x10 : (iy - y1) / y10);
        double t = (x32 != 0. ? (ix - x3) / x32 : (iy - y3) / y32);

        if (s < 0. || s > 1. || t < 0. || t > 1.)
        {
            ret[0] = DBL_MAX;
            ret[1] = DBL_MAX;
            return ret;
        }
    }

    ret[0] = ix;
    ret[1] = iy;
    return ret;
}

double
*subfx_yutils_math_ortho(double x1,double y1,double z1,
                         double x2,double y2,double z2)
{
    double *ret = calloc(3, sizeof(double));
    if (!ret)
    {
        return NULL;
    }

    ret[0] = y1 * z2 - z1 * y2;
    ret[1] = z1 * x2 - x1 * z2;
    ret[2] = x1 * y2 - y1 * x2;
    return ret;
}

double
subfx_yutils_math_randomsteps(double min, double max,
                              double step, char *errMsg)
{
    if (max < min || step <= 0)
    {
        subfx_pError(errMsg, "randomsteps: Invalid input!");
        return 0.;
    }

    return subfx_min(min +
                     subfx_utils_math_random(0,
                                             ceil((max - min) / step)) * step,
                     max);
}

double
subfx_yutils_math_round(double x, double dec)
{
    // Return number rounded to wished decimal size
    if (dec != 0. && dec >= 1.)
    {
        dec = pow(10, floor(dec));
        return floor(x * dec + 0.5) / dec;
    }

    return floor(x + 0.5);
}

double
*subfx_yutils_math_stretch(double x, double y, double z, double length)
{
    double *ret = calloc(3, sizeof(double));
    if (!ret)
    {
        return NULL;
    }

    double cur_length = subfx_yutils_math_distance(x, y, z);
    if (cur_length == 0.)
    {
        return ret;
    }

    double factor = length / cur_length;
    ret[0] = x * factor;
    ret[1] = y * factor;
    ret[2] = z * factor;
    return ret;
}

double
subfx_yutils_math_trim(double x, double min, double max, char *errMsg)
{
    if (max < min)
    {
        subfx_pError(errMsg, "trim: Invalid input!")
        return 0.;
    }

    return (x < min ? min : (x > max ? max : x));
}

double
*subfx_yutils_math_ellipse(double x, double y,
                           double w, double h,double a)
{
    double *ret = calloc(2, sizeof(double));
    if (!ret)
    {
        return NULL;
    }

    double ra = subfx_utils_math_rad(a);
    ret[0] = x + w / 2. * sin(ra);
    ret[1] = y + h / 2. * cos(ra);
    return ret;
}

double subfx_yutils_math_randomway()
{
    double ret;
    while(1)
    {
        ret = subfx_utils_math_random(0, 1) * 2. - 1.;
        if (ret != 0.)
        {
            break;
        }
    }

    return (ret < 0. ? -1. : 1.);
}

double
*subfx_yutils_math_rotate(const double *p,
                          const char *axis,
                          double angle,
                          char *errMsg)
{
    if (strcmp(axis, "x") != 0 &&
        strcmp(axis, "y") != 0 &&
        strcmp(axis, "z") != 0)
    {
        subfx_pError(errMsg, "rotate: invalid axis");
        return NULL;
    }

    double *ret = calloc(3, sizeof(double));
    if (!ret)
    {
        subfx_pError(errMsg, "rotate: Fail to allocate memory.")
        return NULL;
    }

    double ra = subfx_utils_math_rad(angle);

    // Is here has any better way to solve this problem?
    if (strcmp(axis, "x") == 0)
    {
        ret[0] = p[0];
        ret[1] = cos(ra) * p[1] - sin(ra) * p[2];
        ret[2] = sin(ra) * p[1] + cos(ra) * p[2];
    }
    else if (strcmp(axis, "y") == 0)
    {
        ret[0] = cos(ra) * p[0] + sin(ra) * p[2];
        ret[1] = p[1];
        ret[2] = cos(ra) * p[2] - sin(ra) * p[0];
    }
    else
    {
        ret[0] = cos(ra) * p[0] - sin(ra) * p[1];
        ret[1] = sin(ra) * p[0] + cos(ra) * p[1];
        ret[2] = p[2];
    }

    return ret;
}

void bezier2(double pct, double *pts, bool is3D, double *dst)
{
    double pct_inv = 1 - pct;
#if defined(SUBFX_ENABLE_SIMD) && !defined(SUBFX_ENABLE_AVX) // SSE

    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};

    __m128d pctArray_reg = _mm_loadu_pd(pctArray);
    __m128d pct_inv_array_reg = _mm_loadu_pd(pct_inv_array);
    __m128d ctrl0_reg = _mm_loadu_pd(pts);
    __m128d ctrl1_reg = _mm_loadu_pd(pts + 4);

    __m128d tmp_reg = _mm_mul_pd(pct_inv_array_reg, ctrl0_reg);
    __m128d res_reg = _mm_mul_pd(pctArray_reg, ctrl1_reg);
    res_reg = _mm_add_pd(tmp_reg, res_reg);
    _mm_storeu_pd(dst, res_reg);

    if (is3D)
    {
        // 0 * 4 + 2 = 2
        // 1 * 4 + 2 = 6
        dst[2] = pct_inv * pts[2] + pct * pts[6];
    }
#elif defined(SUBFX_ENABLE_SIMD) && defined(SUBFX_ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};

    __m256d pctArray_reg = _mm256_loadu_pd(pctArray);
    __m256d pct_inv_array_reg = _mm256_loadu_pd(pct_inv_array);
    __m256d ctrl0_reg = _mm256_loadu_pd(pts);
    __m256d ctrl1_reg = _mm256_loadu_pd(pts + 4);

    __m256d tmp_reg = _mm256_mul_pd(pct_inv_array_reg, ctrl0_reg);
    __m256d res_reg = _mm256_mul_pd(pctArray_reg, ctrl1_reg);
    res_reg = _mm256_add_pd(tmp_reg, res_reg);
    _mm256_storeu_pd(dst, res_reg);

    if (!is3D)
    {
        dst[2] = 0.;
    }
#else // pure c
    // 0 * 4 + 0 = 0
    // 1 * 4 + 0 = 4
    dst[0] = pct_inv * pts[0] + pct * pts[4];

    // 0 * 4 + 1 = 1
    // 1 * 4 + 1 = 5
    dst[1] = pct_inv * pts[1] + pct * pts[5];

    if (is3D)
    {
        // 0 * 4 + 2 = 2
        // 1 * 4 + 2 = 6
        dst[2] = pct_inv * pts[2] + pct * pts[6];
    }
#endif
}

void bezier3(double pct, double *pts, bool is3D, double *dst)
{
    double pct_inv = 1 - pct;
#if defined(SUBFX_ENABLE_SIMD) && !defined(SUBFX_ENABLE_AVX) // SSE
    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};
    double twoArray[] = {2., 2.};

    __m128d pctArray_reg = _mm_loadu_pd(pctArray);
    __m128d pct_inv_array_reg = _mm_loadu_pd(pct_inv_array);
    __m128d twoArray_reg = _mm_loadu_pd(twoArray);
    __m128d ctrl0_reg = _mm_loadu_pd(pts);
    __m128d ctrl1_reg = _mm_loadu_pd(pts + 4);
    __m128d ctrl2_reg = _mm_loadu_pd(pts + 8);

    __m128d res_reg = _mm_mul_pd(pct_inv_array_reg, pct_inv_array_reg);
    res_reg = _mm_mul_pd(pct_inv_array_reg, ctrl0_reg);

    __m128d tmp_reg = _mm_mul_pd(twoArray_reg, pct_inv_array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm_mul_pd(pctArray_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);
    _mm_storeu_pd(dst, res_reg);

    if (is3D)
    {
        // 0 * 4 + 2 = 2
        // 1 * 4 + 2 = 6
        // 2 * 4 + 2 = 10
        dst[2] = pct_inv * pct_inv * pts[2];
        dst[2] += (2. * pct_inv * pct * pts[6]);
        dst[2] += (pct * pct * pts[10]);
    }
#elif defined(SUBFX_ENABLE_SIMD) && defined(SUBFX_ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};
    double twoArray[] = {2., 2., 2., 2.};

    __m256d pctArray_reg = _mm256_loadu_pd(pctArray);
    __m256d pct_inv_array_reg = _mm256_loadu_pd(pct_inv_array);
    __m256d twoArray_reg = _mm256_loadu_pd(twoArray);
    __m256d ctrl0_reg = _mm256_loadu_pd(pts);
    __m256d ctrl1_reg = _mm256_loadu_pd(pts + 4);
    __m256d ctrl2_reg = _mm256_loadu_pd(pts + 8);

    __m256d res_reg = _mm256_mul_pd(pct_inv_array_reg, pct_inv_array_reg);
    res_reg = _mm256_mul_pd(pct_inv_array_reg, ctrl0_reg);

    __m256d tmp_reg = _mm256_mul_pd(twoArray_reg, pct_inv_array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm256_mul_pd(pctArray_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);
    _mm256_storeu_pd(dst, res_reg);

    if (!is3D)
    {
        dst[2] = 0.;
    }
#else
    dst[0] = pct_inv * pct_inv * pts[0]; // 0 * 4 + 0 = 0
    dst[0] += (2. * pct_inv * pct * pts[4]); // 1 * 4 + 0 = 4
    dst[0] += (pct * pct * pts[8]); // 2 * 4 + 0 = 8

    dst[1] = pct_inv * pct_inv * pts[1]; // 0 * 4 + 1 = 1
    dst[1] += (2. * pct_inv * pct * pts[5]); // 1 * 4 + 1 = 5
    dst[1] += (pct * pct * pts[9]); // 2 * 4 + 1 = 9

    if (is3D)
    {
        dst[2] = pct_inv * pct_inv * pts[2]; // 0 * 4 + 2 = 2
        dst[2] += (2. * pct_inv * pct * pts[6]); // 1 * 4 + 2 = 6
        dst[2] += (pct * pct * pts[10]); // 2 * 4 + 2 = 10
    }
#endif
}

void bezier4(double pct, double *pts, bool is3D, double *dst)
{
    double pct_inv = 1 - pct;
#if defined(SUBFX_ENABLE_SIMD) && !defined(SUBFX_ENABLE_AVX) // SSE
    double pctArray[] = {pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv};
    double threeArray[] = {3., 3.};

    __m128d pctArray_reg = _mm_loadu_pd(pctArray);
    __m128d pct_inv_array_reg = _mm_loadu_pd(pct_inv_array);
    __m128d threeArray_reg = _mm_loadu_pd(threeArray);
    __m128d ctrl0_reg = _mm_loadu_pd(pts);
    __m128d ctrl1_reg = _mm_loadu_pd(pts + 4);
    __m128d ctrl2_reg = _mm_loadu_pd(pts + 8);
    __m128d ctrl3_reg = _mm_loadu_pd(pts + 12);

    __m128d pct2Array_reg = _mm_mul_pd(pctArray_reg, pctArray_reg);
    __m128d pct_inv2_array_reg =
            _mm_mul_pd(pct_inv_array_reg, pct_inv_array_reg);

    __m128d res_reg = _mm_mul_pd(pct_inv2_array_reg, pct_inv_array_reg);
    res_reg = _mm_mul_pd(res_reg, ctrl0_reg);

    __m128d tmp_reg = _mm_mul_pd(threeArray_reg, pct_inv2_array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm_mul_pd(threeArray_reg, pct_inv_array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, pct2Array_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm_mul_pd(pct2Array_reg, pctArray_reg);
    tmp_reg = _mm_mul_pd(tmp_reg, ctrl3_reg);
    res_reg = _mm_add_pd(res_reg, tmp_reg);
    _mm_storeu_pd(dst, res_reg);

    if (is3D)
    {
        // 0 * 4 + 2 = 2
        // 1 * 4 + 2 = 6
        // 2 * 4 + 2 = 10
        // 3 * 4 + 2 = 14
        dst[2] = pct_inv * pct_inv * pct_inv * pts[2];
        dst[2] += (3. * pct_inv * pct_inv * pct * pts[6]);
        dst[2] += (3. * pct_inv * pct * pct * pts[10]);
        dst[2] += (pct * pct * pct * pts[14]);
    }
#elif defined(SUBFX_ENABLE_SIMD) && defined(SUBFX_ENABLE_AVX) // AVX
    double pctArray[] = {pct, pct, pct, pct};
    double pct_inv_array[] = {pct_inv, pct_inv, pct_inv, pct_inv};
    double threeArray[] = {3., 3., 3., 3.};

    __m256d pctArray_reg = _mm256_loadu_pd(pctArray);
    __m256d pct_inv_array_reg = _mm256_loadu_pd(pct_inv_array);
    __m256d threeArray_reg = _mm256_loadu_pd(threeArray);
    __m256d ctrl0_reg = _mm256_loadu_pd(pts);
    __m256d ctrl1_reg = _mm256_loadu_pd(pts + 4);
    __m256d ctrl2_reg = _mm256_loadu_pd(pts + 8);
    __m256d ctrl3_reg = _mm256_loadu_pd(pts + 12);

    __m256d pct2Array_reg = _mm256_mul_pd(pctArray_reg, pctArray_reg);
    __m256d pct_inv2_array_reg =
            _mm256_mul_pd(pct_inv_array_reg, pct_inv_array_reg);

    __m256d res_reg = _mm256_mul_pd(pct_inv2_array_reg, pct_inv_array_reg);
    res_reg = _mm256_mul_pd(res_reg, ctrl0_reg);

    __m256d tmp_reg = _mm256_mul_pd(threeArray_reg, pct_inv2_array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl1_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm256_mul_pd(threeArray_reg, pct_inv_array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, pct2Array_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl2_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);

    tmp_reg = _mm256_mul_pd(pct2Array_reg, pctArray_reg);
    tmp_reg = _mm256_mul_pd(tmp_reg, ctrl3_reg);
    res_reg = _mm256_add_pd(res_reg, tmp_reg);
    _mm256_storeu_pd(dst, res_reg);

    if (!is3D)
    {
        dst[2] = 0.;
    }
#else
    dst[0] = pct_inv * pct_inv * pct_inv * pts[0]; // 0 * 4 + 0 = 0
    dst[0] += (3.f * pct_inv * pct_inv * pct * pts[4]); // 1 * 4 + 0 = 4
    dst[0] += (3.f * pct_inv * pct * pct * pts[8]); // 2 * 4 + 0 = 8
    dst[0] += (pct * pct * pct * pts[12]); // 3 * 4 + 0 = 12

    dst[1] = pct_inv * pct_inv * pct_inv * pts[1]; // 0 * 4 + 1 = 1
    dst[1] += (3.f * pct_inv * pct_inv * pct * pts[5]); // 1 * 4 + 1 = 5
    dst[1] += (3.f * pct_inv * pct * pct * pts[9]); // 2 * 4 + 1 = 9
    dst[1] += (pct * pct * pct * pts[13]); // 3 * 4 + 1 = 13

    if (is3D)
    {
        dst[2] = pct_inv * pct_inv * pct_inv * pts[2]; // 0 * 4 + 2 = 2
        dst[2] += (3. * pct_inv * pct_inv * pct * pts[6]); // 1 * 4 + 2 = 6
        dst[2] += (3. * pct_inv * pct * pct * pts[10]); // 2 * 4 + 2 = 10
        dst[2] += (pct * pct * pct * pts[14]); // 3 * 4 + 2 = 14
    }
#endif
}

void bezierN(double pct, double *pts, size_t ptsCount,
             bool is3D, double *dst)
{
    double pct_inv = 1 - pct;
    size_t ni = ptsCount - 1;
    double nd = (double)ni;
    double bern = 0;
    double *pt;
    double *tmpPt = pts;

#if defined(SUBFX_ENABLE_SIMD) && !defined(SUBFX_ENABLE_AVX) // SSE
    double retArray[] = {0., 0.};
    __m128d ret_reg = _mm_loadu_pd(retArray);
#elif defined(SUBFX_ENABLE_SIMD) && defined(SUBFX_ENABLE_AVX) // AVX
    double retArray[] = {0., 0., 0., 0.};
    __m256d ret_reg = _mm256_loadu_pd(retArray);
#endif
    for (size_t i = 0; i <= ni; ++i, tmpPt += 4)
    {
        pt = tmpPt;

        // Bernstein polynom
        // Binomial coefficient
        bern = fac(nd) / (fac((double)i) * fac((double)ni - i));
        bern *= pow(pct, (double)i);
        bern += pow(pct_inv, (double)(ni - i));

#if defined(SUBFX_ENABLE_SIMD) && !defined(SUBFX_ENABLE_AVX) // SSE
        double bernArray[] = {bern, bern};
        __m128d bern_reg = _mm_loadu_pd(bernArray);
        __m128d pt_reg = _mm_loadu_pd(pt);

        __m128d tmp_reg = _mm_mul_pd(pt_reg, bern_reg);
        ret_reg = _mm_add_pd(ret_reg, tmp_reg);
        if (is3D)
        {
            dst[2] += (pt[2] * bern);
        }
#elif defined(SUBFX_ENABLE_SIMD) && defined(SUBFX_ENABLE_AVX) // AVX
        double bernArray[] = {bern, bern, bern, bern};
        __m256d bern_reg = _mm256_loadu_pd(bernArray);
        __m256d pt_reg = _mm256_loadu_pd(pt);

        __m256d tmp_reg = _mm256_mul_pd(pt_reg, bern_reg);
        ret_reg = _mm256_add_pd(ret_reg, tmp_reg);
#else
        dst[0] += (pt[0] * bern);
        dst[1] += (pt[1] * bern);
        if (is3D)
        {
            dst[2] += (pt[2] * bern);
        }
#endif
    }
#if defined(SUBFX_ENABLE_SIMD) && !defined(SUBFX_ENABLE_AVX) // SSE
    _mm_storeu_pd(dst, ret_reg);
#elif defined(SUBFX_ENABLE_SIMD) && defined(SUBFX_ENABLE_AVX) // AVX
    _mm256_storeu_pd(dst, ret_reg);
    if (!is3D)
    {
        dst[2] = 0.;
    }
#endif
}

double fac(double n)
{
    double k = 1.;
    for (double i = 2.; i <= n; ++i)
    {
        k *= i;
    }

    return k;
}

#undef subfx_max
#undef subfx_min
