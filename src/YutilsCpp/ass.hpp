#ifndef ASS_HPP
#define ASS_HPP

#include <string>
#include <tuple>
#include <vector>
#include <memory>

#include <cstdint>

#include "../common/basecommon.hpp"

namespace Yutils
{

class SYMBOL_SHOW Ass
{
public:

    static std::shared_ptr<Ass> create();

    std::pair<uint64_t, const char *> stringToMs(std::string &ass_ms);

    std::string msToString(uint64_t ms_ass);

    std::pair<std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>, const char *>
    stringToColorAlpha(std::string &input);

    std::pair<std::string, const char *>
    colorAlphaToString(std::vector<uint8_t> &input);

protected:

    Ass() {}
};

}

#endif // ASS_HPP
