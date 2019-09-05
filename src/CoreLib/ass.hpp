#ifndef ASS_HPP
#define ASS_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <tuple>
#include <vector>

#include <cstdint>

using namespace std;

class CoreAss
{
public:

    CoreAss() {}

    // stringToMs may throw invalid_argument
    uint64_t stringToMs(string &ass_ms);

    string msToString(uint64_t ms_ass);

    // stringToColorAlpha may throw invalid_argument
    tuple<uint8_t, uint8_t, uint8_t, uint8_t> stringToColorAlpha(string &input);

    // colorAlphaToString may throw invalid_argument
    string colorAlphaToString(vector<uint8_t> &input);
};

#endif // ASS_HPP