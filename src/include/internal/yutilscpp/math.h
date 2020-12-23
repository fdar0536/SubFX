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

#pragma once

#include <stddef.h>

#include "../defines.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct subfx_yutils_math
{
    subfx_handle (*arc_curve)(double x, double y,
                              double cx, double cy,
                              double angle,
                              char *errMsg);
} subfx_yutils_math;

SUBFX_API subfx_double_vector
subfx_yutils_math_arc_curve(double x, double y,
                            double cx, double cy,
                            double angle,
                            char *errMsg);

/*
 * row is the size of base array(double **)
 * column is the size of each subarray(double *)
 * it will return null-terminated 2D-array(double **).
*/
SUBFX_API subfx_double_vector
subfx_yutils_math_create_double_vector(size_t row, size_t col);

SUBFX_API void
subfx_yutils_math_free_double_vector(subfx_double_vector);

/*
 * note that in parameter pts
 * pts must be null-terminated 2D-array(double **)
 * row = control points count
 * column must equal to four for optimum
 * i.e. pts = subfx_yutils_math_create_double_vector(control points count, 4);
*/
SUBFX_API double
*subfx_yutils_math_bezier(double pct,
                          subfx_double_vector pts,
                          int is3D,
                          char *errMsg);

SUBFX_API double
subfx_yutils_math_degree(double x1, double y1, double z1,
                         double x2, double y2, double z2);

SUBFX_API double
subfx_yutils_math_distance(double x, double y, double z);

SUBFX_API double
*subfx_yutils_math_line_intersect(double x0, double y0,
                                  double x1, double y1,
                                  double x2, double y2,
                                  double x3, double y3,
                                  int strict, char *errMsg);

SUBFX_API double
*subfx_yutils_math_ortho(double x1,double y1,double z1,
                         double x2,double y2,double z2);

SUBFX_API double
subfx_yutils_math_randomsteps(double min, double max,
                              double step, char *errMsg);

SUBFX_API double
subfx_yutils_math_round(double x, double dec);

SUBFX_API double
*subfx_yutils_math_stretch(double x, double y, double z, double length);

SUBFX_API double
subfx_yutils_math_trim(double x, double min, double max, char *errMsg);

SUBFX_API double
*subfx_yutils_math_ellipse(double x, double y,
                           double w, double h,double a);

SUBFX_API double subfx_yutils_math_randomway();

SUBFX_API double
*subfx_yutils_math_rotate(const double *p,
                          const char *axis,
                          double angle,
                          char *errMsg);

#ifdef __cplusplus
}
#endif
