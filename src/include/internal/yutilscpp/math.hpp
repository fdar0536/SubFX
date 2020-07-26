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

#include <vector>
#include <tuple>
#include <string>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Math
{

SYMBOL_SHOW std::vector<std::pair<double, double>>
arc_curve(double x, double y,
          double cx, double cy,
          double angle) THROW;

SYMBOL_SHOW std::tuple<double, double, double>
bezier(double pct,
       std::vector<std::tuple<double, double, double>> &pts,
       bool is3D = false) THROW;

SYMBOL_SHOW double degree(double x1, double y1, double z1,
                          double x2, double y2, double z2) NOTHROW;

SYMBOL_SHOW double distance(double x, double y, double z = 0.) NOTHROW;

SYMBOL_SHOW std::pair<double, double>
line_intersect(double x0, double y0,
               double x1, double y1,
               double x2, double y2,
               double x3, double y3,
               bool strict) THROW;

SYMBOL_SHOW std::tuple<double, double, double>
ortho(double x1,double y1,double z1,
      double x2,double y2,double z2) NOTHROW;

SYMBOL_SHOW double randomsteps(double min, double max, double step) THROW;

SYMBOL_SHOW double round(double x, double dec = 0.) NOTHROW;

SYMBOL_SHOW std::tuple<double, double, double>
stretch(double x, double y, double z, double length) NOTHROW;

SYMBOL_SHOW double trim(double x, double min, double max) THROW;

SYMBOL_SHOW std::pair<double, double>
ellipse(double x, double y,
        double w, double h,double a) NOTHROW;

SYMBOL_SHOW double randomway() NOTHROW;

SYMBOL_SHOW std::tuple<double, double, double>
rotate(std::tuple<double, double, double> p,
       std::string axis,
       double angle) THROW;

} // end namespace Math

} // end namespace Yutils

} // end PROJ_NAMESPACE


