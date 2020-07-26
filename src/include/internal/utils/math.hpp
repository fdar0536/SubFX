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

#include <string>
#include <utility>

#include "../basecommon.h"

// Floating point precision by numbers behind point (for shape points)
#define FP_PRECISION 3

// Anti-aliasing precision for shape to pixels conversion
#define SUPERSAMPLING 8

namespace PROJ_NAMESPACE
{

namespace Utils
{

namespace Math
{

SYMBOL_SHOW std::pair<double, double> rotate2d(double, double, double) NOTHROW;

// math.rad
SYMBOL_SHOW double rad(double angle) NOTHROW;

// math.deg
SYMBOL_SHOW double deg(double r) NOTHROW;

// math.random
SYMBOL_SHOW double random(double min, double max) NOTHROW;

} // end namespace Math

} // end namespace Utils

} // end PROJ_NAMESPACE
