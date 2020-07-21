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


