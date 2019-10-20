#ifndef ASS_HPP
#define ASS_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <tuple>
#include <vector>

#include <cstdint>

#include "../common/basecommon.hpp"

namespace Yutils
{

class SYMBOL_SHOW Ass
{
public:

    Ass() {}

    // stringToMs may throw invalid_argument
    uint64_t stringToMs(std::string &ass_ms);

    std::string msToString(uint64_t ms_ass);

    // stringToColorAlpha may throw invalid_argument
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> stringToColorAlpha(std::string &input);

    // colorAlphaToString may throw invalid_argument
    std::string colorAlphaToString(std::vector<uint8_t> &input);
};

}

#endif // ASS_HPP
