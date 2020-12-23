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
// #include "internal/math_internal.h"

#define subfx_max(x, y) x > y ? x : y
#define subfx_min(x, y) x < y ? x : y

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

    angle = (angle < 0. ? angle * -1. : angle);
    // angle / 90 + 1 = total loop count
    // four pieces of data will be generated in each loop
    // so totalSize = (angle / 90 + 1) * 4
    size_t totalSize = (((size_t)(angle / 90) + 1) << 2);
    subfx_handle
            curves = subfx_yutils_math_create_double_vector(totalSize, 2);
    if (!curves)
    {
        subfx_pError(errMsg, "arc_curve: Fail to allocate memory.")
        return NULL;
    }

    // Factor for bezier control points distance to node points
    double kappa = (4. * (sqrt(2.) - 1.) / 3.);
    double rx0, ry0, rx1, ry1, rx2, ry2, rx3 = 0., ry3, rx03, ry03;

    rx0 = x - cx;
    ry0 = y - cy;

    double cw = (angle > 0. ? 1. : -1.);

    double angle_sum = 0.;
    double cur_angle_pct;
    double tmpDouble;

    size_t index;
    double *tmpPtr;
    // convert do-while-loop to for-loop
    for (index = 0; index < totalSize; index += 4, angle_sum += 90.)
    {
        cur_angle_pct = subfx_min((angle - angle_sum),
                                  (double)90.) / 90.;
        tmpPtr = subfx_utils_math_rotate2d(rx0, ry0, cw * 90. * cur_angle_pct);
        CHECK_TMP_PTR(tmpPtr)

        rx3 = tmpPtr[0];
        ry3 = tmpPtr[1];
        FREE_PTR(tmpPtr);

        // arc start to end vector
        rx03 = rx3 - rx0;
        ry03 = ry3 - ry0;

        // Scale arc vector to curve node <-> control point distance
        tmpDouble = subfx_yutils_math_distance(rx03, ry03, 0.);
        tmpDouble = pow(tmpDouble, 2) / 2.;
        tmpDouble = sqrt(tmpDouble);
        tmpPtr = subfx_yutils_math_stretch(rx03, ry03, 0,
                                           tmpDouble * kappa);
        CHECK_TMP_PTR(tmpPtr)

        rx03 = tmpPtr[0];
        ry03 = tmpPtr[1];
        FREE_PTR(tmpPtr);

        // Get curve control points
        tmpPtr = subfx_utils_math_rotate2d(
                    rx03, ry03, cw * (-45.) * cur_angle_pct);
        CHECK_TMP_PTR(tmpPtr)
        rx1 = rx0 + tmpPtr[0];
        ry1 = ry0 + tmpPtr[1];
        FREE_PTR(tmpPtr);

        tmpPtr = subfx_utils_math_rotate2d(
                    rx03, ry03, cw * (-45.) * cur_angle_pct);
        CHECK_TMP_PTR(tmpPtr)
        rx1 = rx0 + tmpPtr[0];
        ry1 = ry0 + tmpPtr[1];
        FREE_PTR(tmpPtr);

        tmpPtr = subfx_utils_math_rotate2d(
                    rx03 * -1., ry03 * -1., cw * 45. * cur_angle_pct);
        CHECK_TMP_PTR(tmpPtr)
        rx2 = rx3 + tmpPtr[0];
        ry2 = ry3 + tmpPtr[1];
        FREE_PTR(tmpPtr);

        // Insert curve to output
        curves[index][0] = cx + rx0;
        curves[index][1] = cy + ry0;

        curves[index + 1][0] = cx + rx1;
        curves[index + 1][1] = cy + ry1;

        curves[index + 2][0] = cx + rx2;
        curves[index + 2][1] = cy + ry2;

        curves[index + 3][0] = cx + rx3;
        curves[index + 3][1] = cy + ry3;

        // Prepare next curve
        rx0 = rx3;
        ry0 = ry3;
    }

    return curves;
}

SYMBOL_SHOW double
*subfx_yutils_math_bezier(double pct,
                          subfx_double_vector pts,
                          int is3D,
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

    size_t count = 0;
    double **tmpPtr = pts;
    while(tmpPtr != NULL)
    {
        ++count;
        ++tmpPtr;
    }

    if (count < 2)
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

    switch(count)
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
        bezierN(pct, pts, count, is3D, ret);
        break;
    }
    } // end switch

    return ret;
}

SYMBOL_SHOW double
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

SYMBOL_SHOW double
subfx_yutils_math_distance(double x, double y, double z)
{
    return sqrt(x * x + y * y + z * z);
}

SYMBOL_SHOW double
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

SYMBOL_SHOW double
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

SYMBOL_SHOW double
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

SYMBOL_SHOW double
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

SYMBOL_SHOW double
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

SYMBOL_SHOW double
subfx_yutils_math_trim(double x, double min, double max, char *errMsg)
{
    if (max < min)
    {
        subfx_pError(errMsg, "trim: Invalid input!")
        return 0.;
    }

    return (x < min ? min : (x > max ? max : x));
}

SYMBOL_SHOW double
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

SYMBOL_SHOW double subfx_yutils_math_randomway()
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

SYMBOL_SHOW double
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

#undef subfx_max
#undef subfx_min
