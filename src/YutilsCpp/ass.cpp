#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <regex>
#include <stdexcept>

#include <cmath>
#include <cstdio>

#include "boost/lexical_cast.hpp"

#include "ass.hpp"

using namespace Yutils;

std::shared_ptr<Ass> Ass::create()
{
    return createCommon<Ass>();
}

std::pair<uint64_t, const char *> Ass::stringToMs(std::string &ass_ms)
{
    std::regex reg("^\\d:\\d\\d:\\d\\d\\.\\d\\d$");
    if (!std::regex_match(ass_ms, reg))
    {
        return std::make_pair(0, "ASS timestamp expected");
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
        return std::make_pair(0, "cannot convert!");
    }

    return std::make_pair(ret, nullptr);
}

std::string Ass::msToString(uint64_t ms_ass)
{
    uint32_t hr(static_cast<int>(floor(ms_ass / 3600000)) % 10); //hour
    uint32_t mins(static_cast<uint32_t>(floor(ms_ass % 3600000 / 60000))); // minutes
    uint32_t sec(static_cast<uint32_t>(floor(ms_ass % 60000 / 1000))); // second
    uint32_t csec(static_cast<uint32_t>(floor(ms_ass % 1000 / 10))); // centisecond

    char buf[500];
    sprintf(buf, "%d:%02d:%02d.%02d", hr, mins, sec, csec);
    return std::string(buf);
}

std::pair<std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>, const char *>
Ass::stringToColorAlpha(std::string &input)
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
        return std::make_pair(std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>(),
                              "Invalid input");
    }

    return std::make_pair(std::make_tuple(r, g, b, a),
                          nullptr);
}

std::pair<std::string, const char *>
Ass::colorAlphaToString(std::vector<uint8_t> &input)
{
    if (input.size() != 1 &&
        input.size() != 3 &&
        input.size() != 4)
    {
        return std::make_pair(std::string(), "Invalid input!");
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

    return std::make_pair(std::string(buf), nullptr);
}
