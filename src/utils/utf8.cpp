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
