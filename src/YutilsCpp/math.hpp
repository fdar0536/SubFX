#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <memory>

#include "common.hpp"

namespace Yutils
{
class SYMBOL_SHOW Math : public Common
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

    /**
     * @brief Get orthogonal vector of 2 given vectors.
     *
     * Calculates the orthogonal vector to vectors x1|y1|z1 and x2|y2|z3.
     */
    std::tuple<double, double, double> ortho(double x1,
                                             double y1,
                                             double z1,
                                             double x2,
                                             double y2,
                                             double z2) NOTHROW;

    /**
     * @brief Generates a random number in given range with specific item distance.
     *
     * Generates randomly a number in range min to max with gap size step between numbers.
     * @throw std::invalid_argument
     */
    double randomsteps(double min, double max, double step) THROW;

    /**
     * @brief Rounds number.
     *
     * Rounds input to nearest integer.
     *
     * @param x input number.
     * @param dec Defines the position behind decimal point to round to.
     */
    double round(double x, double dec = 0.) NOTHROW;

    /**
     * @brief Scales vector to given length.
     *
     * Stretches vector x|y|z to length length.
     */
    std::tuple<double, double, double>
    stretch(double x, double y, double z, double length) NOTHROW;

    /**
     * @brief Trim number in range
     *
     * If x is smaller than min, returns min.
     * If x is greater than max, returns max.
     * Otherwise returns x.
     * @throw std::invalid_argument
     */
    double trim(double x, double min, double max) THROW;

    /**
     * Calculates a point on ellipse with given angle,
     * center point x/y, width and height.
     */
    std::pair<double, double> ellipse(double x, double y,
                                      double w, double h,
                                      double a) NOTHROW;

    /**
     * Returns randomly -1 or 1.
     */
    double randomway() NOTHROW;

    /**
     * Allows to rotate a point in 3D room.
     *
     * @param p input point(x, y, z) in tuple.
     * @param axis "x" or "y" or "z".
     * @param angle As its name in degree.
     * @return Rotated point(x, y, z) in the tuple.
     * @throw std::invalid_argument
     */
    std::tuple<double, double, double>
    rotate(std::tuple<double, double, double> p,
           std::string axis,
           double angle) THROW;

protected:

    Math() NOTHROW : Common() {}

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
};
}
