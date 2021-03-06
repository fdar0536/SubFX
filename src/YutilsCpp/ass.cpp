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

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <regex>
#include <stdexcept>

#include <cmath>
#include <cstdio>

#include "boost/lexical_cast.hpp"

#include "YutilsCpp"

using namespace PROJ_NAMESPACE::Yutils;

SYMBOL_SHOW uint64_t Ass::stringToMs(std::string &ass_ms) THROW
{
    std::regex reg("^\\d:\\d\\d:\\d\\d\\.\\d\\d$");
    if (!std::regex_match(ass_ms, reg))
    {
        throw std::invalid_argument("stringToMs: ASS timestamp expected");
    }

    using boost::lexical_cast;
    using boost::bad_lexical_cast;
    uint64_t ret(0);

    try
    {
        // hour
        ret = lexical_cast<uint64_t>(ass_ms.substr(0, 1)) * 3600000;

        // minute
        ret += (lexical_cast<uint64_t>(ass_ms.substr(2, 2)) * 60000);

        //second
        ret += (lexical_cast<uint64_t>(ass_ms.substr(5, 2)) * 1000);

        // centisecond
        ret += (lexical_cast<uint64_t>(ass_ms.substr(8, 2)) * 10);
    }
    catch (const bad_lexical_cast &) // for safety
    {
        throw std::invalid_argument("stringToMs: Cannot convert!");
    }

    return ret;
}

SYMBOL_SHOW std::string Ass::msToString(uint64_t ms_ass) NOTHROW
{
    uint32_t hr(static_cast<int>(floor(ms_ass / 3600000)) % 10); //hour
    uint32_t mins(static_cast<uint32_t>(floor(ms_ass % 3600000 / 60000))); // minutes
    uint32_t sec(static_cast<uint32_t>(floor(ms_ass % 60000 / 1000))); // second
    uint32_t csec(static_cast<uint32_t>(floor(ms_ass % 1000 / 10))); // centisecond

    char buf[500];
    sprintf(buf, "%d:%02d:%02d.%02d", hr, mins, sec, csec);
    return std::string(buf);
}

SYMBOL_SHOW std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>
Ass::stringToColorAlpha(std::string &input) THROW
{
    uint8_t r(0), g(0), b(0), a(0);
    std::string tmpString;
    if (std::regex_match(input, std::regex("^&[Hh]{1}[0-9a-fA-F]{2}&$")))
    {
        // alpha only &HAA&
        tmpString = input.substr(2, 2);
        a = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));
    }
    else if (std::regex_match(input, std::regex("^&[Hh]{1}[0-9a-fA-F]{6}&$")))
    {
        // ass color &HBBGGRR&
        tmpString = input.substr(2, 2);
        b = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));

        tmpString = input.substr(4, 2);
        g = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));

        tmpString = input.substr(6, 2);
        r = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));
    }
    else if (std::regex_match(input, std::regex("^&[Hh]{1}[0-9a-fA-F]{8}$")))
    {
        // both &HAABBGGRR
        tmpString = input.substr(2, 2);
        a = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));

        tmpString = input.substr(4, 2);
        b = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));

        tmpString = input.substr(6, 2);
        g = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));

        tmpString = input.substr(8, 2);
        r = static_cast<uint8_t>(stoul(tmpString, nullptr, 16));
    }
    else
    {
        throw std::invalid_argument("stringToColorAlpha: Invalid input");
    }

    return std::make_tuple(r, g, b, a);
}

SYMBOL_SHOW std::string
Ass::colorAlphaToString(std::vector<uint8_t> &input) THROW
{
    if (input.size() != 1 &&
        input.size() != 3 &&
        input.size() != 4)
    {
        throw std::invalid_argument("colorAlphaToString: Invalid input!");
    }

    char buf[500];
    switch (input.size())
    {
    case 1:
    {
        // alpha only &HAA&
        sprintf(buf, "&H%02X&", input.at(0));
        break;
    }
    case 3:
    {
        // rgb &HBBGGRR&
        sprintf(buf, "&H%02X%02X%02X&",
                input.at(2),
                input.at(1),
                input.at(0));
        break;
    }
    default:
    {
        // rgba &HAABBGGRR
        sprintf(buf, "&H%02X%02X%02X%02X",
                input.at(3),
                input.at(2),
                input.at(1),
                input.at(0));
        break;
    }
    } // end switch

    return std::string(buf);
}
