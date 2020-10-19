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

#include <string>
#include <vector>

#include <cstring>

#include "SubFX.h"

extern "C"
{

SYMBOL_SHOW uint32_t subfx_utils_utf8_stringLen(const char *utf8str)
{
    uint32_t len(0);
    size_t strsize(strlen(utf8str));
    for (size_t i = 0; i < strsize; ++i)
    {
        len += ((static_cast<uint8_t>(utf8str[i]) & 0xc0) != 0x80);
    }

    return len;
} // end subfx_utils_utf8_stringLen

SYMBOL_SHOW
char **subfx_utils_utf8_stringSplit(const char *utf8str)
{
    std::vector<std::string> ret;
    std::string input(utf8str);
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

    // copy back to array
    char **retptr(reinterpret_cast<char **>(
                  calloc(ret.size() + 1, sizeof(char *))));
    if (!retptr)
    {
        return nullptr;
    }

    int allocatedCount(0);
    for (size_t i = 0; i < ret.size(); ++i)
    {
        retptr[i] = reinterpret_cast<char *>(
                    calloc(ret.at(i).length() + 1, sizeof(char)));
        if (!retptr[i])
        {
            for (int j = 0; j < allocatedCount; ++j)
            {
                free(retptr[j]);
            }

            free(retptr);
            return nullptr;
        }

        std::copy(ret.at(i).begin(), ret.at(i).end(), retptr[i]);
        retptr[i][ret.at(i).length()] = '\0';
        ++allocatedCount;
    }

    retptr[ret.size()] = nullptr;
    return retptr;
} // end subfx_utils_utf8_stringSplit

} // end extern "C"
