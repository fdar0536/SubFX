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
#include <functional>
#include <utility>
#include <map>
#include <vector>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Shape
{

// Calculates shape bounding box
SYMBOL_SHOW std::tuple<double, double, double, double>
bounding(std::string &) THROW;

// Filters shape points
SYMBOL_SHOW std::string
filter(std::string &,
       std::function<std::pair<double, double>(double, double, std::string &)> &)
THROW;

// Converts shape curves to lines
SYMBOL_SHOW std::string flatten(std::string &) THROW;

// Shifts shape coordinates
SYMBOL_SHOW std::string move(std::string &, double, double) THROW;

// Converts shape to pixels
SYMBOL_SHOW std::vector<std::map<std::string, double>>
to_pixels(std::string &) THROW;

} // end namespace Shape

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
