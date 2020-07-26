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

#include <cstdint>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Utils
{

namespace Utf8
{

SYMBOL_SHOW uint32_t stringLen(std::string&) NOTHROW;

// https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters
SYMBOL_SHOW std::vector<std::string> stringSplit(std::string&) NOTHROW;

}

} // end namespace Utils

} // end namespace PROJ_NAMESPACE
