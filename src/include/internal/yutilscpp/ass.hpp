#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <cstdint>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Ass
{

SYMBOL_SHOW uint64_t stringToMs(std::string &ass_ms) THROW;

SYMBOL_SHOW std::string msToString(uint64_t ms_ass) NOTHROW;

SYMBOL_SHOW std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>
stringToColorAlpha(std::string &input) THROW;

SYMBOL_SHOW std::string
colorAlphaToString(std::vector<uint8_t> &input) THROW;

} // end namespace Ass

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
