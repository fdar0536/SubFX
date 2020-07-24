#include <iostream>
#include <algorithm>
#include <limits>
#include <stdexcept>

#include <cmath>

#include "Utils"
#include "YutilsCpp"
#include "internal/math_internal.hpp"

using namespace PROJ_NAMESPACE::Yutils;

// public member function
std::vector<std::pair<double, double>>
Math::arc_curve(double x, double y,
                double cx, double cy,
                double angle) THROW
{
    std::vector<std::pair<double, double>> curves;
    if (angle < -360. || angle > 360.)
    {
        throw std::invalid_argument("arc_curve: start & center point and "
                                    "valid angle (-360<=x<=360) expected");
    }

    if (angle == 0.)
    {
        throw std::invalid_argument("arc_curve: angle CANNOT be zero");
    }

    // Factor for bezier control points distance to node points
    double kappa(4. * (sqrt(2.) - 1.) / 3.);
    double rx0, ry0, rx1, ry1, rx2, ry2, rx3(0.), ry3, rx03, ry03;

    rx0 = x - cx;
    ry0 = y - cy;

    double cw = (angle > 0. ? 1. : -1.);

    // convert do-while-loop to while-loop
    size_t curves_n(4);
    curves.reserve(curves_n);
    double angle_sum(0.);
    double cur_angle_pct;
    std::pair<double, double> tmpPair;
    double tmpDouble;

    while (angle_sum < angle)
    {
        cur_angle_pct = std::min((angle - angle_sum),
                                 static_cast<double>(90.)) / 90.;
        std::tie(rx3, ry3) = PROJ_NAMESPACE::Utils::Math::rotate2d(
                    rx0, ry0, cw * 90. * cur_angle_pct);

        // arc start to end vector
        rx03 = rx3 - rx0;
        ry03 = ry3 - ry0;

        // Scale arc vector to curve node <-> control point distance
        tmpDouble = distance(rx03, ry03);
        tmpDouble = pow(tmpDouble, 2) / 2.;
        tmpDouble = sqrt(tmpDouble);
        std::tie(rx03, ry03, std::ignore) = stretch(rx03, ry03, 0,
                                                    tmpDouble * kappa);

        // Get curve control points
        tmpPair = PROJ_NAMESPACE::Utils::Math::rotate2d(
                    rx03, ry03, cw * (-45.) * cur_angle_pct);
        rx1 = rx0 + tmpPair.first;
        ry1 = ry0 + tmpPair.second;

        tmpPair = PROJ_NAMESPACE::Utils::Math::rotate2d(
                    rx03 * -1., ry03 * -1., cw * 45. * cur_angle_pct);
        rx2 = rx3 + tmpPair.first;
        ry2 = ry3 + tmpPair.second;

        // Insert curve to output
        curves.push_back(std::make_pair(cx + rx0, cy + ry0));
        curves.push_back(std::make_pair(cx + rx1, cy + ry1));
        curves.push_back(std::make_pair(cx + rx2, cy + ry2));
        curves.push_back(std::make_pair(cx + rx3, cy + ry3));
        curves_n += 4;
        curves.reserve(curves_n);

        // Prepare next curve
        rx0 = rx3;
        ry0 = ry3;
        angle_sum += 90.;
    }

    return curves;
} // end CoreMath::arc_curve

std::tuple<double, double, double>
Math::bezier(double pct,
             std::vector<std::tuple<double, double, double>> &pts,
             bool is3D) THROW
{
    if (pct < 0. || pct > 1.)
    {
        throw std::invalid_argument("bezier: pct must between 0 and 1");
    }

    size_t ptsSize(pts.size());
    if (ptsSize < 2)
    {
        throw std::invalid_argument("bezier: at least 2 points expected");
    }

    switch (ptsSize)
    {
    case 2:
        return Math_Internal::bezier2(pct, pts, is3D);
    case 3:
        return Math_Internal::bezier3(pct, pts, is3D);
    case 4:
        return Math_Internal::bezier4(pct, pts, is3D);
    }

    return Math_Internal::bezierN(pct, pts, is3D);
}

double Math::degree(double x1, double y1, double z1,
                    double x2, double y2, double z2) NOTHROW
{
    double degree = distance(x1, y1, z1) * distance(x2, y2, z2);
    if (degree == 0.)
    {
        return 0.;
    }

    degree = acos((x1 * x2 + y1 * y2 + z1 * z2) / degree);
    degree = PROJ_NAMESPACE::Utils::Math::deg(degree);

    // Return with sign by clockwise direction
    if ((x1 * y2 - y1 * x2) < 0)
    {
        return (degree * -1.);
    }

    return degree;
}

double Math::distance(double x, double y, double z) NOTHROW
{
    return sqrt(x * x + y * y + z * z);
}

std::pair<double, double>
Math::line_intersect(double x0, double y0,
                     double x1, double y1,
                     double x2, double y2,
                     double x3, double y3,
                     bool strict) THROW
{
    // Get line vectors & check valid lengths
    double x10(x0 - x1);
    double y10(y0 - y1);
    double x32(x2 - x3);
    double y32(y2 - y3);

    if ((x10 == 0. && y10 == 0.) ||
        (x32 == 0. && y32 == 0.))
    {
        throw std::invalid_argument("line_intersect: lines mustn't have "
                                    "zero length");
    }

    // Calculate determinant and check for parallel lines
    double det = x10 * y32 - y10 * x32;
    if (det == 0.)
    {
        return std::pair<double, double>();
    }

    // Calculate line intersection (endless line lengths)
    double pre(x0 * y1 - y0 * x1);
    double post(x2 * y3 - y2 * x3);

    double ix = (pre * x32 - x10 * post) / det;
    double iy = (pre * y32 - y10 * post) / det;

    if (strict)
    {
        double s = (x10 != 0. ? (ix - x1) / x10 : (iy - y1) / y10);
        double t = (x32 != 0. ? (ix - x3) / x32 : (iy - y3) / y32);

        if (s < 0. || s > 1. || t < 0. || t > 1.)
        {
            return std::make_pair(std::numeric_limits<double>::infinity(),
                                  std::numeric_limits<double>::infinity());
        }
    }

    return std::make_pair(ix, iy);
}

std::tuple<double, double, double>
Math::ortho(double x1, double y1, double z1,
            double x2, double y2, double z2) NOTHROW
{
    return std::make_tuple(y1 * z2 - z1 * y2,
                           z1 * x2 - x1 * z2,
                           x1 * y2 - y1 * x2);
}

double Math::randomsteps(double min, double max, double step) THROW
{
    if (max < min || step <= 0)
    {
        throw std::invalid_argument("randomsteps: Invalid input!");
    }

    return std::min(
                min +
                PROJ_NAMESPACE::
                Utils::Math::
                random(0, ceil((max - min) / step)) * step, max);
}

double Math::round(double x, double dec) NOTHROW
{
    // Return number rounded to wished decimal size
    if (dec != 0. && dec >= 1.)
    {
        dec = pow(10, floor(dec));
        return floor(x * dec + 0.5) / dec;
    }

    return floor(x + 0.5);
}

std::tuple<double, double, double>
Math::stretch(double x, double y, double z, double length) NOTHROW
{
    double cur_length(distance(x, y, z));
    if (cur_length == 0.)
    {
        return std::make_tuple(0.f, 0.f, 0.f);
    }

    double factor(length / cur_length);
    return std::make_tuple(x * factor, y * factor, z * factor);
}

double Math::trim(double x, double min, double max) THROW
{
    if (max < min)
    {
        throw std::invalid_argument("trim: Invalid input!");
    }

    return (x < min ? min : (x > max ? max : x));
}

std::pair<double, double> Math::ellipse(double x, double y,
                                        double w, double h,
                                        double a) NOTHROW
{
    double ra(PROJ_NAMESPACE::Utils::Math::rad(a));
    return std::make_pair(x + w / 2. * sin(ra),
                          y + h / 2. * cos(ra));
}

double Math::randomway() NOTHROW
{
    double ret;
    while(1)
    {
        ret = PROJ_NAMESPACE::Utils::Math::random(0, 1) * 2. - 1.;
        if (ret != 0.)
        {
            break;
        }
    }

    return (ret < 0. ? -1. : 1.);
}

std::tuple<double, double, double>
Math::rotate(std::tuple<double, double, double> p,
             std::string axis,
             double angle) THROW
{
    if (axis != "x" && axis != "y" && axis != "z")
    {
        throw std::invalid_argument("rotate: invalid axis");
    }

    double ra(PROJ_NAMESPACE::Utils::Math::rad(angle));

    // Is here has any better way to solve this problem?
    if (axis == "x")
    {
        return std::make_tuple(std::get<0>(p),
                               cos(ra) * std::get<1>(p) -
                               sin(ra) * std::get<2>(p),
                               sin(ra) * std::get<1>(p) +
                               cos(ra) * std::get<2>(p));
    }

    if (axis == "y")
    {
        return std::make_tuple(cos(ra) * std::get<0>(p) +
                               sin(ra) * std::get<2>(p),
                               std::get<1>(p),
                               cos(ra) * std::get<2>(p) -
                               sin(ra) * std::get<0>(p));
    }

    return std::make_tuple(cos(ra) * std::get<0>(p) -
                           sin(ra) * std::get<1>(p),
                           sin(ra) * std::get<0>(p) +
                           cos(ra) * std::get<1>(p),
                           std::get<2>(p));
}

// private member functions
