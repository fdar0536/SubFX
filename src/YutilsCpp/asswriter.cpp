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

#include <stdexcept>
#include <iostream>
#include <fstream>

#include <cstring>

#include "YutilsCpp"
#include "internal/asswriter_internal.hpp"

using namespace PROJ_NAMESPACE::Yutils;

SYMBOL_SHOW void AssWriter::write(const char *fileName,
                                  std::shared_ptr<AssParser> &parser) THROW
{
    // here may throw
    AssWriter_Internal::checkFileName(fileName);

    std::fstream file;
    file.open(fileName, std::fstream::trunc | std::fstream::out);
    if (file.fail())
    {
        throw std::invalid_argument("write: CANOT open file");
    }

    auto meta(parser->meta());
    AssWriter_Internal::writeMeta(file, meta);

    auto styles(parser->styles());
    AssWriter_Internal::writeStyle(file, styles);

    auto dialogs(parser->dialogs());
    AssWriter_Internal::writeEvent(file, dialogs);

    file.close();
}

SYMBOL_SHOW void AssWriter::write(const char *fileName,
                                  const char *assHeader,
                                  std::vector<std::string> &assBuf) THROW
{
    // here may throw
    AssWriter_Internal::checkFileName(fileName);

    std::fstream file;
    size_t length(0);
    if (assHeader)
    {
        length = strlen(assHeader);
        if (length)
        {
            file.open(fileName, std::fstream::trunc | std::fstream::out);
        }
        else
        {
            file.open(fileName, std::fstream::app | std::fstream::out);
        }
    }
    else
    {
        file.open(fileName, std::fstream::app | std::fstream::out);
    }

    if (file.fail())
    {
        throw std::invalid_argument("write: CANOT open file");
    }

    if (length) // length != 0
    {
        file << assHeader;
    }

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << std::endl;
    }

    file.close();
}

SYMBOL_SHOW void AssWriter::write(const char *fileName,
                                  std::shared_ptr<AssMeta> &meta,
                                  std::map<std::string,
                                  std::shared_ptr<AssStyle>> &styles,
                                  std::vector<std::string> &assBuf) THROW
{
    // here may throw
    AssWriter_Internal::checkFileName(fileName);

    std::fstream file;
    file.open(fileName, std::fstream::trunc | std::fstream::out);
    if (file.fail())
    {
        throw std::invalid_argument("write: CANOT open file");
    }

    AssWriter_Internal::writeMeta(file, meta);
    // here may throw
    AssWriter_Internal::writeStyle(file, styles);
    AssWriter_Internal::writeEvent(file, assBuf);

    file.close();
}
