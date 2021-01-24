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
#include <stdbool.h>

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

    double *(*bezier)(double pct,
                      double *pts,
                      size_t ptsCount,
                      bool is3D,
                      char *errMsg);

    double (*degree)(double x1, double y1, double z1,
                     double x2, double y2, double z2);

    double (*distance)(double x, double y, double z);

    double *(*line_intersect)(double x0, double y0,
                              double x1, double y1,
                              double x2, double y2,
                              double x3, double y3,
                              int strict, char *errMsg);

    double *(*ortho)(double x1,double y1,double z1,
                     double x2,double y2,double z2);

    double (*randomsteps)(double min, double max,
                          double step, char *errMsg);

    double (*round)(double x, double dec);

    double *(*stretch)(double x, double y, double z, double length);

    double (*trim)(double x, double min, double max, char *errMsg);

    double *(*ellipse)(double x, double y,
                       double w, double h,double a);

    double (*randomway)();

    double *(*rotate)(const double *p,
                      const char *axis,
                      double angle,
                      char *errMsg);
} subfx_yutils_math;

#ifdef __cplusplus
}
#endif
