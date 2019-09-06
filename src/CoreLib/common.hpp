#ifndef COMMON_HPP
#define COMMON_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <utility>

#define FP_PRECISION 3 // Floating point precision by numbers behind point (for shape points)

#define UNUSED(x) static_cast<void>(x)

using namespace std;

class CoreCommon
{
public:
    CoreCommon();

protected:
    pair<double, double> rotate2d(double, double, double);

    // math.rad
    double rad(double angle);

    // math.deg
    double deg(double r);

    // math.random
    double random(double min, double max);

    string doubleToString(double input);
};

#endif // COMMON_HPP