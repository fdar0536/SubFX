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
