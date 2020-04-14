#pragma once

#include <vector>
#include <string>

#include <cstdint>

#include "../basecommon.h"

namespace Yutils
{
class SYMBOL_SHOW Utf8
{
protected:

    Utf8() noexcept {}

    uint32_t utf8StringLen(std::string&) NOTHROW;

    // https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters
    std::vector<std::string> utf8StringSplit(std::string&) NOTHROW;
};
}
