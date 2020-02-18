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

    static std::shared_ptr<Ass> create() NOTHROW;

    uint64_t stringToMs(std::string &ass_ms) THROW;

    std::string msToString(uint64_t ms_ass) NOTHROW;

    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>
    stringToColorAlpha(std::string &input) THROW;

    std::string
    colorAlphaToString(std::vector<uint8_t> &input) THROW;

protected:

    Ass() {}
};
}
