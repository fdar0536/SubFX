#include "Utils"

using namespace PROJ_NAMESPACE::Utils;

SYMBOL_SHOW uint32_t Utf8::stringLen(std::string &input) NOTHROW
{
    uint32_t len(0);
    for (size_t i = 0; i < input.length(); ++i)
    {
        len += ((static_cast<uint8_t>(input.at(i)) & 0xc0) != 0x80);
    }

    return len;
}

SYMBOL_SHOW std::vector<std::string>
Utf8::stringSplit(std::string &input) NOTHROW
{
    std::vector<std::string> ret;
    ret.reserve(20);

    for(size_t i = 0; i < input.length();)
    {
        size_t len = 1;
        if((static_cast<uint8_t>(input.at(i)) & 0xf8) == 0xf0)
            len = 4;
        else if((static_cast<uint8_t>(input.at(i)) & 0xf0) == 0xe0)
            len = 3;
        else if((static_cast<uint8_t>(input.at(i)) & 0xe0) == 0xc0)
            len = 2;
        if((i + len) > input.length())
            len = 1;

        ret.push_back(input.substr(i, len));
        i += len;
    }

    return ret;
}