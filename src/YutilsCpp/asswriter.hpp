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
#include <map>

#include "../basecommon.h"
#include "assparser.hpp"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace AssWriter
{

SYMBOL_SHOW void write(const char *fileName,
                       std::shared_ptr<AssParser> &parser) THROW;

SYMBOL_SHOW void write(std::shared_ptr<AssParser> &parser) THROW;

SYMBOL_SHOW void write(const char *fileName,
                       const char *assHeader,
                       std::vector<std::string> &assBuf) THROW;

SYMBOL_SHOW void write(const char *assHeader,
                       std::vector<std::string> &assBuf) THROW;

SYMBOL_SHOW void write(const char *fileName,
                       std::shared_ptr<AssMeta> &meta,
                       std::map<std::string,
                       std::shared_ptr<AssStyle>> &styles,
                       std::vector<std::string> &assBuf) THROW;

SYMBOL_SHOW void write(std::shared_ptr<AssMeta> &meta,
                       std::map<std::string,
                       std::shared_ptr<AssStyle>> &styles,
                       std::vector<std::string> &assBuf) THROW;

} // end namespace AssWriter

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
