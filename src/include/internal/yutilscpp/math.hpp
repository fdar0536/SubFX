#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <memory>

#include "common.hpp"

namespace PROJ_NAMESPACE
{

namespace Yutils
{
class SYMBOL_SHOW Math
{
public:

    static std::shared_ptr<Math> create() NOTHROW;

    std::vector<std::pair<double, double>>
    arc_curve(double x, double y,
              double cx, double cy,
              double angle) THROW;

    std::tuple<double, double, double>
    bezier(double pct,
           std::vector<std::tuple<double, double, double>> &pts,
           bool is3D = false) THROW;

    double degree(double x1, double y1, double z1,
                  double x2, double y2, double z2) NOTHROW;

    double distance(double x, double y, double z = 0.) NOTHROW;

    std::pair<double, double>
    line_intersect(double x0, double y0,
                   double x1, double y1,
                   double x2, double y2,
                   double x3, double y3,
                   bool strict) THROW;

    std::tuple<double, double, double> ortho(double x1,
                                             double y1,
                                             double z1,
                                             double x2,
                                             double y2,
                                             double z2) NOTHROW;

    double randomsteps(double min, double max, double step) THROW;

    double round(double x, double dec = 0.) NOTHROW;

    std::tuple<double, double, double>
    stretch(double x, double y, double z, double length) NOTHROW;

    double trim(double x, double min, double max) THROW;

    std::pair<double, double> ellipse(double x, double y,
                                      double w, double h,
                                      double a) NOTHROW;

    double randomway() NOTHROW;

    std::tuple<double, double, double>
    rotate(std::tuple<double, double, double> p,
           std::string axis,
           double angle) THROW;

protected:

    Math() NOTHROW :
        m_common(Common())
    {}

private:
    std::tuple<double, double, double> bezier2(double pct,
                     std::vector<std::tuple<double, double, double>> &pts,
                                               bool is3D) NOTHROW;

    std::tuple<double, double, double> bezier3(double pct,
                     std::vector<std::tuple<double, double, double>> &pts,
                                               bool is3D) NOTHROW;

    std::tuple<double, double, double> bezier4(double pct,
                     std::vector<std::tuple<double, double, double>> &pts,
                                               bool is3D) NOTHROW;

    std::tuple<double, double, double> bezierN(double pct,
                     std::vector<std::tuple<double, double, double>> &pts,
                                               bool is3D) NOTHROW;

    double fac(double n) NOTHROW;

    Common m_common;
};
} // end namespace Yutils

} // end PROJ_NAMESPACE


