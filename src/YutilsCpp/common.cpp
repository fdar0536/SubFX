#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "common.hpp"

#define PI \
    3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651e+00

using namespace Yutils;

Common::Common() NOTHROW
{
    srand(static_cast<uint32_t>(time(nullptr)));
}

// protected member function
std::pair<double, double>
Common::rotate2d(double x, double y, double angle) NOTHROW
{
    double ra(rad(angle));
    return std::make_pair(cos(ra) * x - sin(ra) * y, sin(ra) * x + cos(ra) * y);
}

double Common::rad(double angle) NOTHROW
{
    return (angle * PI / 180.);
}

double Common::deg(double r) NOTHROW
{
    return (r * 180. / pi);
}

double Common::random(double min, double max) NOTHROW
{
    return ((max - min) * rand() / (RAND_MAX + 1.0) + min);
}

std::string Common::doubleToString(double input) NOTHROW
{
    double buffer[500];
    sprintf(reinterpret_cast<char *>(buffer), "%.3lf", input);
    return std::string(reinterpret_cast<char *>(buffer));
}
