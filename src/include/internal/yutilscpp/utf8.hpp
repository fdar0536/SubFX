#pragma once

#include <vector>
#include <string>

#include <cstdint>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{
class SYMBOL_SHOW Utf8
{
protected:

    Utf8() NOTHROW {}

    uint32_t utf8StringLen(std::string&) NOTHROW;

    // https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters
    std::vector<std::string> utf8StringSplit(std::string&) NOTHROW;
};
} // end namespace Yutils

} // end namespace PROJ_NAMESPACE


