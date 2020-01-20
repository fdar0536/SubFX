#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "boost/math/constants/constants.hpp"

#include "common.hpp"

using namespace boost::math::double_constants;

using namespace Yutils;

Common::Common()
{
    srand(static_cast<uint32_t>(time(nullptr)));
}

// protected member function
std::pair<double, double> Common::rotate2d(double x, double y, double angle)
{
    double ra(rad(angle));
    return std::make_pair(cos(ra) * x - sin(ra) * y, sin(ra) * x + cos(ra) * y);
}

double Common::rad(double angle)
{
    return (angle * pi / 180.);
}

double Common::deg(double r)
{
    return (r * 180. / pi);
}

double Common::random(double min, double max)
{
    return ((max - min) * rand() / (RAND_MAX + 1.0) + min);
}

std::string Common::doubleToString(double input)
{
    double buffer[500];
    sprintf(reinterpret_cast<char *>(buffer), "%.3lf", input);
    return std::string(reinterpret_cast<char *>(buffer));
}
