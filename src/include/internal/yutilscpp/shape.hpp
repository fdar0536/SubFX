#ifndef SHAPE_HPP
#define SHAPE_HPP

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

    static std::shared_ptr<Shape> create();

    // Calculates shape bounding box
    std::pair<std::tuple<double, double, double, double>, const char *>
    bounding(std::string &);

    // Filters shape points
    std::pair<std::string, const char *>
    filter(std::string &,
           std::function<std::pair<double, double>(double, double, std::string &)> &);

    // Converts shape curves to lines
    std::pair<std::string, const char *>
    flatten(std::string &);

    // Shifts shape coordinates
    std::pair<std::string, const char *>
    move(std::string &, double, double);

    // Converts shape to pixels
    std::pair<std::vector<std::map<std::string, double>>, const char *>
    to_pixels(std::string &);

protected:

    Shape() : Math() {}

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
    const char *render_shape(double,
                             double,
                             std::vector<bool> &,
                             std::string &);
};

}

#endif // SHAPE_HPP
