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

SYMBOL_SHOW void write(const char *fileName,
                       std::shared_ptr<AssMeta> &meta,
                       std::map<std::string, std::shared_ptr<AssStyle>> &styles,
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
