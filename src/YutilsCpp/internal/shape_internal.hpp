#pragma once

#include <vector>
#include <string>

#include "internal/basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Shape_Internal
{

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

} // end namespace Shape_Internal

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
