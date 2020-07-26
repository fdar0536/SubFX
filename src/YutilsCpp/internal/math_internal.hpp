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

#include "internal/basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Math_Internal
{

std::tuple<double, double, double> bezier2(double pct,
                 std::vector<std::tuple<double, double, double>> &pts,
                                           bool is3D) NOTHROW;

std::tuple<double, double, double> bezier3(double pct,
                 std::vector<std::tuple<double, double, double>> &pts,
                                           bool is3D) NOTHROW;

std::tuple<double, double, double> bezier4(double pct,
                 std::vector<std::tuple<double, double, double>> &pts,
                                           bool is3D) NOTHROW;

std::tuple<double, double, double> bezierN(double pct,
                 std::vector<std::tuple<double, double, double>> &pts,
                                           bool is3D) NOTHROW;

double fac(double n) NOTHROW;

} // end namespace Math_Internal

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
