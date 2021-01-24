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

#include "include/internal/yutilscpp/math.h"

#ifdef __cplusplus
extern "C"
{
#endif

subfx_yutils_math *subfx_yutils_math_init();

subfx_handle
subfx_yutils_math_arc_curve(double x, double y,
                            double cx, double cy,
                            double angle,
                            char *errMsg);

double
*subfx_yutils_math_bezier(double pct,
                          double *pts,
                          size_t ptsCount,
                          bool is3D,
                          char *errMsg);

double
subfx_yutils_math_degree(double x1, double y1, double z1,
                         double x2, double y2, double z2);

double
subfx_yutils_math_distance(double x, double y, double z);

double
*subfx_yutils_math_line_intersect(double x0, double y0,
                                  double x1, double y1,
                                  double x2, double y2,
                                  double x3, double y3,
                                  int strict, char *errMsg);

double
*subfx_yutils_math_ortho(double x1,double y1,double z1,
                         double x2,double y2,double z2);

double
subfx_yutils_math_randomsteps(double min, double max,
                              double step, char *errMsg);

double
subfx_yutils_math_round(double x, double dec);

double
*subfx_yutils_math_stretch(double x, double y, double z, double length);

double
subfx_yutils_math_trim(double x, double min, double max, char *errMsg);

double
*subfx_yutils_math_ellipse(double x, double y,
                           double w, double h,double a);

double subfx_yutils_math_randomway();

double
*subfx_yutils_math_rotate(const double *p,
                          const char *axis,
                          double angle,
                          char *errMsg);

void bezier2(double pct, double *pts, bool is3D, double *dst);

void bezier3(double pct, double *pts, bool is3D, double *dst);

void bezier4(double pct, double *pts, bool is3D, double *dst);

void bezierN(double pct, double *pts, size_t ptsCount,
             bool is3D, double *dst);

double fac(double n);

#ifdef __cplusplus
}
#endif
