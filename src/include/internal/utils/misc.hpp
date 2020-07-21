#pragma once

#include <istream>
#include <string>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Utils
{

namespace Misc
{

SYMBOL_SHOW std::string doubleToString(double input) NOTHROW;

// https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
SYMBOL_SHOW std::istream &safeGetline(std::istream &is, std::string &t) NOTHROW;

}

} // end namespace Utils

} // end namespace PROJ_NAMESPACE
