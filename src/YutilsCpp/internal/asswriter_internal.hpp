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

#include <fstream>

#include "internal/basecommon.h"
#include "YutilsCpp"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace AssWriter_Internal
{

void checkFileName(const char *fileName) THROW;

void writeMeta(std::fstream &file, std::shared_ptr<AssMeta> &meta) NOTHROW;

void writeStyle(std::fstream &file,
                std::map<std::string,
                std::shared_ptr<AssStyle>> &styles) THROW;

void writeEventHeader(std::fstream &file) NOTHROW;

void writeEvent(std::fstream &file,
                std::vector<std::string> &assBuf) NOTHROW;

void writeEvent(std::fstream &file,
                std::vector<std::shared_ptr<AssDialog>> &dialogs) NOTHROW;

} // end namespace AssWriter_Internal

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
