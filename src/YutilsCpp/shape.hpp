#ifndef SHAPE_HPP
#define SHAPE_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <functional>
#include <utility>
#include <map>

#include "math.hpp"
#include "../common/basecommon.hpp"

namespace Yutils
{

class SYMBOL_SHOW Shape : protected Math
{
public:

    Shape();

    // Calculates shape bounding box
    // It may throw invalid_argument
    std::tuple<double, double, double, double> bounding(std::string &);

    // Filters shape points
    // It may throw invalid argument
    std::string filter(std::string &, std::function<std::pair<double, double>(double, double, std::string &)> &);

    // Converts shape curves to lines
    // It may throw invalid_argument
    std::string flatten(std::string &);

    // Shifts shape coordinates
    // It may throw invalid_argument
    std::string move(std::string &, double, double);

    // Converts shape to pixels
    // It may throw invalid_argument
    std::vector<std::map<std::string, double>> to_pixels(std::string &);

private:

    // 4th degree curve subdivider
    // for flatten
    std::vector<double> curve4_subdivide(double, double,
                                         double, double,
                                         double, double,
                                         double, double,
                                         double);

    // Check flatness of 4th degree curve with angles
    // for flatten
    bool curve4_is_flat(double, double,
                        double, double,
                        double, double,
                        double, double,
                        double);

    // Convert 4th degree curve to line points
    // for flatten
    std::vector<double> curve4_to_lines(double, double,
                                        double, double,
                                        double, double,
                                        double, double);

    // Renderer (on binary image with aliasing)
    // for to_pixels
    void render_shape(double, double, std::vector<bool> &, std::string &);
};

}

#endif // SHAPE_HPP
