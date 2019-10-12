#ifndef MATH_HPP
#define MATH_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <vector>
#include <tuple>
#include <string>

#include "common.hpp"
#include "../common/basecommon.hpp"

using namespace std;

class SYMBOL_SHOW CoreMath : public CoreCommon
{
public:
     CoreMath();

     // arc_curve may throw invalid_argument
     vector<pair<double, double>> arc_curve(double x, double y,
                                    double cx, double cy, double angle);
     
     // bezier may throw invalid_argument
     tuple<double, double, double> bezier(double pct,
                    vector<tuple<double, double, double>> &pts, bool is3D = false);
     
     double degree(double x1, double y1, double z1,
                    double x2, double y2, double z2);
     
     // Degree between two 3d vectors
     double distance(double x, double y, double z = 0.f);

     // line_intersect may throw invalid_argument
     pair<double, double> line_intersect(double x0, double y0,
                                         double x1, double y1,
                                         double x2, double y2,
                                         double x3, double y3,
                                         bool strict);
     
     //Get orthogonal vector of 2 given vectors
     tuple<double, double, double> ortho(double x1, double y1, double z1,
                                         double x2, double y2, double z2);
     
     // Generates a random number in given range with specific item distance
     // it may throw invalid_argument
     double randomsteps(double min, double max, double step);

     // Rounds number
     double round(double x, double dec = 0.f);

     // Scales vector to given length
     tuple<double, double, double> stretch(double x, double y,
                                           double z, double length);
     
     // Trim number in range
     // it may throw invalid_argument
     double trim(double x, double min, double max);

     pair<double, double> ellipse(double x, double y,
                                  double w, double h,
                                  double a);
     
     double randomway();

     // rotate may throw invalid_argument
     tuple<double, double, double> rotate(tuple<double, double, double> p,
                                          string axis,
                                          double angle);

private:
     tuple<double, double, double> bezier2(double pct,
                         vector<tuple<double, double, double>> &pts, bool is3D);
     
     tuple<double, double, double> bezier3(double pct,
                         vector<tuple<double, double, double>> &pts, bool is3D);
     
     tuple<double, double, double> bezier4(double pct,
                         vector<tuple<double, double, double>> &pts, bool is3D);
     
     tuple<double, double, double> bezierN(double pct,
                         vector<tuple<double, double, double>> &pts, bool is3D);
     
     double fac(double n);
};

#endif // MATH_HPP
