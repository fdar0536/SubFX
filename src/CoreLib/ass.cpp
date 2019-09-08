#include <regex>
#include <stdexcept>

#include <cmath>
#include <cstdio>

#include "boost/lexical_cast.hpp"

#include "ass.hpp"

using namespace std;

uint64_t CoreAss::stringToMs(string &ass_ms)
{
    regex reg("^\\d:\\d\\d:\\d\\d\\.\\d\\d$");
    if (!regex_match(ass_ms, reg))
    {
        throw invalid_argument("ASS timestamp expected");
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
        throw invalid_argument("cannot convert!");
    }

    return ret;
}

string CoreAss::msToString(uint64_t ms_ass)
{
    uint32_t hr((int)floor(ms_ass / 3600000) % 10); //hour
    uint32_t mins(floor(ms_ass % 3600000 / 60000)); // minutes
    uint32_t sec(floor(ms_ass % 60000 / 1000)); // second
    uint32_t csec(floor(ms_ass % 1000 / 10)); // centisecond

    char buf[500];
    sprintf(buf, "%d:%02d:%02d.%02d", hr, mins, sec, csec);
    return string(buf);
}

tuple<uint8_t, uint8_t, uint8_t, uint8_t> CoreAss::stringToColorAlpha(string &input)
{
    uint8_t r(0), g(0), b(0), a(0);
    string tmpString;
    if (regex_match(input, regex("^&[Hh]{1}[0-9a-fA-F]{2}&$")))
    {
        // alpha only &HAA&
        tmpString = input.substr(2, 2);
        a = stoul(tmpString, nullptr, 16);
    }
    else if (regex_match(input, regex("^&[Hh]{1}[0-9a-fA-F]{6}&$")))
    {
        // ass color &HBBGGRR&
        tmpString = input.substr(2, 2);
        b = stoul(tmpString, nullptr, 16);

        tmpString = input.substr(4, 2);
        g = stoul(tmpString, nullptr, 16);

        tmpString = input.substr(6, 2);
        r = stoul(tmpString, nullptr, 16);
    }
    else if (regex_match(input, regex("^&[Hh]{1}[0-9a-fA-F]{8}$")))
    {
        // both &HAABBGGRR
        tmpString = input.substr(2, 2);
        a = stoul(tmpString, nullptr, 16);

        tmpString = input.substr(4, 2);
        b = stoul(tmpString, nullptr, 16);

        tmpString = input.substr(6, 2);
        g = stoul(tmpString, nullptr, 16);

        tmpString = input.substr(8, 2);
        r = stoul(tmpString, nullptr, 16);
    }
    else
    {
        throw invalid_argument("Invalid input");
    }

    return make_tuple(r, g, b, a);
}

string CoreAss::colorAlphaToString(vector<uint8_t> &input)
{
    if (input.size() != 1 &&
        input.size() != 3 &&
        input.size() != 4)
    {
        throw invalid_argument("Invalid input!");
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

    return string(buf);
}