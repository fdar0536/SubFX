#pragma once

#include <string>
#include <functional>
#include <utility>
#include <map>
#include <vector>

#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Shape
{

// Calculates shape bounding box
SYMBOL_SHOW std::tuple<double, double, double, double>
bounding(std::string &) THROW;

// Filters shape points
SYMBOL_SHOW std::string
filter(std::string &,
       std::function<std::pair<double, double>(double, double, std::string &)> &)
THROW;

// Converts shape curves to lines
SYMBOL_SHOW std::string flatten(std::string &) THROW;

// Shifts shape coordinates
SYMBOL_SHOW std::string move(std::string &, double, double) THROW;

// Converts shape to pixels
SYMBOL_SHOW std::vector<std::map<std::string, double>>
to_pixels(std::string &) THROW;

}

/*
class SYMBOL_SHOW Shape : protected Math
{
private:


};
*/
} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
