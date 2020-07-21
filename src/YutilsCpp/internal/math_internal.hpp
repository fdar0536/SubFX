#pragma once

#include <vector>
#include <tuple>

#include "internal/basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Math_Internal
{

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

} // end namespace Math_Internal

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
