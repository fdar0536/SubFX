/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#include <functional>
#include <algorithm>

#include <cmath>

#include "shape_internal.hpp"
#include "YutilsCpp"

#ifdef ENABLE_SIMD
#include "emmintrin.h" // SSE
#endif

#define CURVE_TOLERANCE 1 // Angle in degree to define a curve as flat

using namespace PROJ_NAMESPACE::Yutils;

std::vector<double>
Shape_Internal::curve4_subdivide(double x0, double y0,
                                 double x1, double y1,
                                 double x2, double y2,
                                 double x3, double y3,
                                 double pct) NOTHROW
{
    std::vector<double> ret;
    ret.reserve(16);
    ret.push_back(x0);
    ret.push_back(y0);
#ifdef ENABLE_SIMD
    double point0[] = {x0, y0};
    double point1[] = {x1, y1};
    double point2[] = {x2, y2};
    double point3[] = {x3, y3};
    double pcts[] = {pct, pct};

    __m128d point0r(_mm_loadu_pd(point0));
    __m128d point1r(_mm_loadu_pd(point1));
    __m128d point2r(_mm_loadu_pd(point2));
    __m128d point3r(_mm_loadu_pd(point3));
    __m128d pctsr(_mm_loadu_pd(pcts));

    __m128d res1r(_mm_add_pd(point0r, point1r));
    __m128d res2r(_mm_add_pd(point1r, point2r));
    __m128d res3r(_mm_add_pd(point2r, point3r));

    res1r = _mm_mul_pd(res1r, pctsr);
    // now res1r contains {x01, y01}

    res2r = _mm_mul_pd(res2r, pctsr);
    // now res2r contains {x12, y12}

    res3r = _mm_mul_pd(res3r, pctsr);
    // now res3r contains {x23, y23}

    // store result
    _mm_storeu_pd(point0, res1r);
    ret.push_back(point0[0]); // x01
    ret.push_back(point0[1]); // y01

    res1r = _mm_add_pd(res1r, res2r);
    res2r = _mm_add_pd(res2r, res3r);

    res1r = _mm_mul_pd(res1r, pctsr);
    // now res1r contains {x012, y012}

    res2r = _mm_mul_pd(res2r, pctsr);
    // now res2r contains {x123, y123}

    // store result
    _mm_storeu_pd(point0, res1r);
    ret.push_back(point0[0]); // x012
    ret.push_back(point0[1]); // y012

    res1r = _mm_add_pd(res1r, res2r);
    res1r = _mm_mul_pd(res1r, pctsr);
    // now res1r contains {x0123, y0123}

    // store result
    _mm_storeu_pd(point0, res1r);
    ret.push_back(point0[0]); // x0123
    ret.push_back(point0[1]); // y0123
    ret.push_back(point0[0]); // x0123
    ret.push_back(point0[1]); // y0123

    _mm_storeu_pd(point0, res2r); // {x123, y123}
    ret.push_back(point0[0]); // x123
    ret.push_back(point0[1]); // y123

    _mm_storeu_pd(point0, res3r); // {x23, y23}
    ret.push_back(point0[0]); // x23
    ret.push_back(point0[1]); // y23

    ret.push_back(x3);
    ret.push_back(y3);
#else // pure c++
    double x01 = (x0 + x1) * pct;
    double y01 = (y0 + y1) * pct;
    ret.push_back(x01);
    ret.push_back(y01);

    double x12 = (x1 + x2) * pct;
    double y12 = (y1 + y2) * pct;
    double x23 = (x2 + x3) * pct;
    double y23 = (y2 + y3) * pct;

    double x012 = (x01 + x12) * pct;
    double y012 = (y01 + y12) * pct;
    ret.push_back(x012);
    ret.push_back(y012);

    double x123 = (x12 + x23) * pct;
    double y123 = (y12 + y23) * pct;

    double x0123 = (x012 + x123) * pct;
    double y0123 = (y012 + y123) * pct;
    ret.push_back(x0123);
    ret.push_back(y0123);
    ret.push_back(x0123);
    ret.push_back(y0123);

    ret.push_back(x123);
    ret.push_back(y123);

    ret.push_back(x23);
    ret.push_back(y23);

    ret.push_back(x3);
    ret.push_back(y3);
#endif
    return ret;
}

bool
Shape_Internal::curve4_is_flat(double x0, double y0,
                               double x1, double y1,
                               double x2, double y2,
                               double x3, double y3,
                               double tolerance) NOTHROW
{
    std::vector<std::pair<double, double>> vecs;
    vecs.reserve(3);

    vecs.push_back(std::make_pair(x1 - x0, y1 - y0));
    vecs.push_back(std::make_pair(x2 - x1, y2 - y1));
    vecs.push_back(std::make_pair(x3 - x2, y3 - y2));

    size_t i(0), n(3);
    while (i < n)
    {
        if (vecs.at(i).first == 0. && vecs.at(i).second == 0.)
        {
            vecs.erase(vecs.begin() + static_cast<int>(i));
            --n;
        }
        else
        {
            ++i;
        }
    }

    for (i = 1; i < n; ++i)
    {
        if (abs(Math::degree(vecs.at(i - 1).first,
                       vecs.at(i - 1).second,
                       0,
                       vecs.at(i).first,
                       vecs.at(i).second,
                       0)) > tolerance)
        {
            return false;
        }
    }

    return true;
}

std::vector<double>
Shape_Internal::curve4_to_lines(double px0, double py0,
                                double px1, double py1,
                                double px2, double py2,
                                double px3, double py3) NOTHROW
{
    std::vector<double> pts;
    pts.reserve(4);

    std::function<void(double, double, double, double, double, double, double, double)>
    convert_recursive{ [&convert_recursive, &pts](
                           double x0, double y0,
                           double x1, double y1,
                           double x2, double y2,
                           double x3, double y3)
    {
        if (curve4_is_flat(x0, y0, x1, y1, x2, y2, x3, y3, CURVE_TOLERANCE))
        {
            pts.push_back(x3);
            pts.push_back(y3);
            pts.reserve(pts.size() + 2);
        }
        else
        {
            std::vector<double> resVec(curve4_subdivide(x0, y0,
                                                        x1, y1,
                                                        x2, y2,
                                                        x3, y3,
                                                        0.5));
            convert_recursive(resVec.at(0), resVec.at(1),
                              resVec.at(2), resVec.at(3),
                              resVec.at(4), resVec.at(5),
                              resVec.at(6), resVec.at(7));
            convert_recursive(resVec.at(8), resVec.at(9),
                              resVec.at(10), resVec.at(11),
                              resVec.at(12), resVec.at(13),
                              resVec.at(14), resVec.at(15));
        }
    } };

    convert_recursive(px0, py0, px1, py1, px2, py2, px3, py3);
    return pts;
}

void
Shape_Internal::render_shape(double width,
                             double height,
                             std::vector<bool> &image,
                             std::string &shape) THROW
{
    std::vector<std::tuple<double, double, double, double>> lines;
    lines.reserve(500);
    std::vector<double> last_point;
    last_point.reserve(2);
    std::vector<double> last_move;
    last_move.reserve(2);
    std::function<std::pair<double, double>(double, double, std::string &)>
    flt([&](double x, double y, std::string &typ)
    {
        // Use integers to avoid rounding errors
        x = round(x);
        y = round(y);

        if (typ == "m")
        {
            // Close figure with non-horizontal line in image
            if (last_move.size() != 0)
            {
                if ((last_move.at(1) != last_point.at(1)) &&
                    !(last_point.at(1) < 0 && last_move.at(1) < 0) &&
                    !(last_point.at(1) > height && last_move.at(1) > height))
                {
                    lines.push_back(std::make_tuple(last_point.at(0),
                                                    last_point.at(1),
                                                    last_move.at(0) - last_point.at(0),
                                                    last_move.at(1) - last_point.at(1)));
                }

                last_move.at(0) = x;
                last_move.at(1) = y;
            }
            else
            {
                last_move.push_back(x);
                last_move.push_back(y);
            }
        }
        else // Non-horizontal line in image
        {
            if (last_point.size() != 0)
            {
                if (last_point.at(1) != y &&
                    !(last_point.at(1) < 0 && y < 0) &&
                    !(last_point.at(1) > height && y > height))
                {
                    lines.push_back(std::make_tuple(last_point.at(0),
                                                    last_point.at(1),
                                                    x - last_point.at(0),
                                                    y - last_point.at(1)));
                }
            }
        }

        // Remember last point
        if (last_point.size() != 0)
        {
            last_point.at(0) = x;
            last_point.at(1) = y;
        }
        else
        {
            last_point.push_back(x);
            last_point.push_back(y);
        }

        return std::pair<double, double>();
    });

    std::string shapeBak(shape);

    // here may throw exception
    shapeBak = Shape::flatten(shapeBak);
    // here may throw exception
    Shape::filter(shapeBak, flt);

    if (last_move.size() > 0)
    {
        if (last_move.at(1) != last_point.at(1) &&
            !(last_point.at(1) < 0 && last_move.at(1) < 0) &&
            !(last_point.at(1) > height && last_move.at(1) > height))
        {
            lines.push_back(std::make_tuple(last_point.at(0),
                                            last_point.at(1),
                                            last_move.at(0) - last_point.at(0),
                                            last_move.at(1) - last_point.at(1)));
        }
    }

    // Calculates line x horizontal line intersection
    std::function<std::vector<double>(double, double, double, double, double)>
    line_x_hline([](double x, double y, double vx, double vy, double y2)
    {
        std::vector<double> ret;
        ret.reserve(2);
        if (vy != 0.)
        {
            double s((y2 - y) / vy);
            if (s >= 0. && s <= 1.)
            {
                ret.push_back(x + s * vx);
                ret.push_back(y2);
            }
        }

        return ret;
    });

    double tmpDouble;
    // here may throw exception
    auto tmpTuple(Shape::bounding(shape));
    for (double y = std::max(floor(std::get<1>(tmpTuple)), 0.);
         y <= (std::min(ceil(std::get<3>(tmpTuple)), height) - 1);
         ++y)
    {
        std::vector<std::pair<double, double>> row_stops;
        row_stops.reserve(lines.size());

        for (size_t i = 0; i < lines.size(); ++i)
        {
            auto line(lines.at(i));
            std::vector<double> cx(line_x_hline(std::get<0>(line),
                                                std::get<1>(line),
                                                std::get<2>(line),
                                                std::get<3>(line),
                                                y + 0.5));
            if (cx.size() != 0)
            {
                // here may throw exception
                tmpDouble = Math::trim(cx.at(0), 0, width);
                row_stops.push_back(std::make_pair(tmpDouble,
                                    std::get<3>(line) > 0. ? 1. : -1.));
            }
        } //end for i

        if (row_stops.size() > 0)
        {
            std::sort(row_stops.begin(), row_stops.end(), [](const std::pair<double, double> &a,
                                                        const std::pair<double, double> &b)
                                                        {
                                                            return (a.first < b.first);
                                                        });
            double status(0.), row_index(y * width);
            for (size_t i = 0; i < row_stops.size() - 1; ++i)
            {
                status += row_stops.at(i).second;
                if (status != 0.)
                {
                    for (double x = ceil(row_stops.at(i).first - 0.5);
                         x <= (floor(row_stops.at(i + 1).first + 0.5) - 1);
                         ++x)
                    {
                        image.at(static_cast<size_t>(row_index + x)) = true;
                    } // end for x
                }
            } // end for i
        }
    } // end for y
}
