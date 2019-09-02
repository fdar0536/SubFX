#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "boost/math/constants/constants.hpp"

#include "common.hpp"

using namespace std;

using namespace boost::math::double_constants;

CoreCommon::CoreCommon()
{
    srand(time(nullptr));
}

// protected member function
pair<double, double> CoreCommon::rotate2d(double x, double y, double angle)
{
    double ra(rad(angle));
    return make_pair(cos(ra) * x - sin(ra) * y, sin(ra) * x + cos(ra) * y);
}

double CoreCommon::rad(double angle)
{
    return (angle * pi / 180.f);
}

double CoreCommon::deg(double r)
{
    return (r * 180.f / pi);
}

double CoreCommon::random(double min, double max)
{
    return ((max - min) * rand() / (RAND_MAX + 1.0) + min);
}

string CoreCommon::doubleToString(double input)
{
    double buffer[500];
    sprintf((char*)buffer, "%.3lf", input);
    return string((char*)buffer);
}