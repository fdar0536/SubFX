#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <memory>

#include <cstdint>

#include "../common/basecommon.h"

namespace Yutils
{
class SYMBOL_SHOW Ass
{
public:

    static std::shared_ptr<Ass> create();

    uint64_t stringToMs(std::string &ass_ms, std::string &errMsg);

    std::string msToString(uint64_t ms_ass);

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>
    stringToColorAlpha(std::string &input, std::string &errMsg);

    std::string
    colorAlphaToString(std::vector<uint8_t> &input, std::string &errMsg);

protected:

    Ass() {}
};
}
