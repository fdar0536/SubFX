#ifndef UTF8_HPP
#define UTF8_HPP

#include <vector>
#include <string>

#include <cstdint>

#include "../common/basecommon.hpp"

namespace Yutils
{

class Utf8
{
protected:

    Utf8() {}

    uint32_t utf8StringLen(std::string &);

    // https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters
    std::vector<std::string> utf8StringSplit(std::string &);
};

}

#endif // UTF8_HPP
