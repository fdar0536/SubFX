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

using namespace std;

class CoreShape : protected CoreMath
{
public:

    CoreShape();

    // Calculates shape bounding box
    // It may throw invalid_argument
    tuple<double, double, double, double> bounding(string &);

    // Filters shape points
    // It may throw invalid argument
    string filter(string &, function<pair<double, double>(double, double, string &)> &);

    // Converts shape curves to lines
    // It may throw invalid_argument
    string flatten(string &);

    // Shifts shape coordinates
    // It may throw invalid_argument
    string move(string &, double, double);

    // Converts shape to pixels
    // It may throw invalid_argument
    vector<map<string, double>> to_pixels(string &);

private:
    
    // 4th degree curve subdivider
    // for flatten
    vector<double> curve4_subdivide(double, double,
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
    vector<double> curve4_to_lines(double, double,
                                   double, double,
                                   double, double,
                                   double, double);
    
    // Renderer (on binary image with aliasing)
    // for to_pixels
    void render_shape(double, double, vector<bool> &, string &);
};

#endif // SHAPE_HPP