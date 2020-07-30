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

#include <ostream>

#include "internal/basecommon.h"
#include "YutilsCpp"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace AssWriter_Internal
{

void checkFileName(const char *fileName) THROW;

void writeMeta(std::ostream &file, std::shared_ptr<AssMeta> &meta) NOTHROW;

void writeStyle(std::ostream &file,
                std::map<std::string,
                std::shared_ptr<AssStyle>> &styles) THROW;

void writeEventHeader(std::ostream &file) NOTHROW;

void writeEvent(std::ostream &file,
                std::vector<std::string> &assBuf) NOTHROW;

void writeEvent(std::ostream &file,
                std::vector<std::shared_ptr<AssDialog>> &dialogs) NOTHROW;

template<class T>
void write(T &file, std::shared_ptr<AssParser> &parser) THROW
{
    auto meta(parser->meta());
    writeMeta(file, meta);

    auto styles(parser->styles());
    writeStyle(file, styles);

    auto dialogs(parser->dialogs());
    writeEvent(file, dialogs);
}

template<class T>
void write(T &file,
           const char *assHeader,
           size_t headerLength,
           std::vector<std::string> &assBuf) THROW
{
    if (headerLength) // length != 0
    {
        file << assHeader;
    }

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << std::endl;
    }
}

template<class T>
void write(T &file,
           std::shared_ptr<AssMeta> &meta,
           std::map<std::string,
           std::shared_ptr<AssStyle>> &styles,
           std::vector<std::string> &assBuf) THROW
{
    writeMeta(file, meta);
    // here may throw
    writeStyle(file, styles);
    writeEvent(file, assBuf);
}

} // end namespace AssWriter_Internal

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
