#pragma once

#include <string>
#include <utility>

#define FP_PRECISION 3 // Floating point precision by numbers behind point (for shape points)
#define SUPERSAMPLING 8 // Anti-aliasing precision for shape to pixels conversion

#define UNUSED(x) static_cast<void>(x)

namespace Yutils
{

class Common
{
protected:

    Common();

    std::pair<double, double> rotate2d(double, double, double);

    // math.rad
    double rad(double angle);

    // math.deg
    double deg(double r);

    // math.random
    double random(double min, double max);

    std::string doubleToString(double input);
};

} // end namespace Yutils
