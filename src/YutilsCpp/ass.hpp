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
#include <tuple>
#include <vector>

#include <cstdint>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Ass
{

SYMBOL_SHOW uint64_t stringToMs(std::string &ass_ms) THROW;

SYMBOL_SHOW std::string msToString(uint64_t ms_ass) NOTHROW;

SYMBOL_SHOW std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>
stringToColorAlpha(std::string &input) THROW;

SYMBOL_SHOW std::string
colorAlphaToString(std::vector<uint8_t> &input) THROW;

} // end namespace Ass

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
