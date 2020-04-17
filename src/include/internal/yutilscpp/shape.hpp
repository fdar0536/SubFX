#pragma once

#include <string>
#include <functional>
#include <utility>
#include <map>

#include "math.hpp"
#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{
class SYMBOL_SHOW Shape : protected Math
{
public:

    static std::shared_ptr<Shape> create() NOTHROW;

    // Calculates shape bounding box
    std::tuple<double, double, double, double>
    bounding(std::string &) THROW;

    // Filters shape points
    std::string
    filter(std::string &,
           std::function<std::pair<double, double>(double, double, std::string &)> &)
    THROW;

    // Converts shape curves to lines
    std::string flatten(std::string &) THROW;

    // Shifts shape coordinates
    std::string move(std::string &, double, double) THROW;

    // Converts shape to pixels
    std::vector<std::map<std::string, double>>
    to_pixels(std::string &) THROW;

protected:

    Shape() : Math() {}

private:

    // 4th degree curve subdivider
    // for flatten
    std::vector<double> curve4_subdivide(double, double,
                                         double, double,
                                         double, double,
                                         double, double,
                                         double) NOTHROW;

    // Check flatness of 4th degree curve with angles
    // for flatten
    bool curve4_is_flat(double, double,
                        double, double,
                        double, double,
                        double, double,
                        double) NOTHROW;

    // Convert 4th degree curve to line points
    // for flatten
    std::vector<double> curve4_to_lines(double, double,
                                        double, double,
                                        double, double,
                                        double, double) NOTHROW;

    // Renderer (on binary image with aliasing)
    // for to_pixels
    void render_shape(double,
                      double,
                      std::vector<bool> &,
                      std::string &) THROW;
};
} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
