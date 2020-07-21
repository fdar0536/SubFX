#pragma once

#include <string>
#include <utility>

#include "../basecommon.h"

// Floating point precision by numbers behind point (for shape points)
#define FP_PRECISION 3

// Anti-aliasing precision for shape to pixels conversion
#define SUPERSAMPLING 8

namespace PROJ_NAMESPACE
{

namespace Utils
{

namespace Math
{

SYMBOL_SHOW std::pair<double, double> rotate2d(double, double, double) NOTHROW;

// math.rad
SYMBOL_SHOW double rad(double angle) NOTHROW;

// math.deg
SYMBOL_SHOW double deg(double r) NOTHROW;

// math.random
SYMBOL_SHOW double random(double min, double max) NOTHROW;

} // end namespace Math

} // end namespace Utils

} // end PROJ_NAMESPACE
