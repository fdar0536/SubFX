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

SYMBOL_SHOW void write(const char *fileName,
                       const char *assHeader,
                       std::vector<std::string> &assBuf) THROW;

SYMBOL_SHOW void write(const char *fileName,
                       std::shared_ptr<AssMeta> &meta,
                       std::map<std::string, std::shared_ptr<AssStyle>> &styles,
                       std::vector<std::string> &assBuf) THROW;

} // end namespace AssWriter

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
