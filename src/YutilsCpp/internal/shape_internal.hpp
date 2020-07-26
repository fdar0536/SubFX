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
#include <string>

#include "internal/basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Shape_Internal
{

// 4th degree curve subdivider
// for flatten
std::vector<double> curve4_subdivide(double, double,
                                     double, double,
                                     double, double,
                                     double, double,
                                     double) NOTHROW;

// Check flatness of 4th degree curve with angles
// for flatten
bool curve4_is_flat(double, double,
                    double, double,
                    double, double,
                    double, double,
                    double) NOTHROW;

// Convert 4th degree curve to line points
// for flatten
std::vector<double> curve4_to_lines(double, double,
                                    double, double,
                                    double, double,
                                    double, double) NOTHROW;

// Renderer (on binary image with aliasing)
// for to_pixels
void render_shape(double,
                  double,
                  std::vector<bool> &,
                  std::string &) THROW;

} // end namespace Shape_Internal

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
