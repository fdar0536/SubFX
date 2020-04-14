#include <regex>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include <cstdio>
#include <cmath>

#include "config.h"
#ifdef ENABLE_SIMD
#include "emmintrin.h" // SSE
#endif

#include "YutilsCpp"

#define CURVE_TOLERANCE 1 // Angle in degree to define a curve as flat

using namespace Yutils;

// public member function
std::shared_ptr<Shape> Shape::create() NOTHROW
{
    Shape *ret(new (std::nothrow) Shape());
    if (!ret)
    {
        return std::shared_ptr<Shape>(nullptr);
    }

    return std::shared_ptr<Shape>(ret);
}

std::tuple<double, double, double, double>
Shape::bounding(std::string &shape) THROW
{
    // Bounding data
    double x0(0.), y0(0.), x1(0.), y1(0.);
    uint8_t flag(0);
    std::function<std::pair<double, double>(double, double, std::string&)> flt(
        [&](double x, double y, std::string &pointType)
        {
            UNUSED(pointType);
            if (flag == 0)
            {
                x0 = x;
                y0 = y;
                x1 = x;
                y1 = y;
                flag = 1;
            }

            x0 = std::min(x0, x);
            y0 = std::min(y0, y);
            x1 = std::max(x1, x);
            y1 = std::max(y1, y);

            return std::pair<double, double>();
        }
    );

    // here may throw exception
    filter(shape, flt);

    return std::make_tuple(x0, y0, x1, y1);
}

std::string
Shape::filter(std::string &shape,
              std::function<std::pair<double, double>(double, double, std::string &)> &flt)
THROW
{
    if (!flt)
    {
        throw std::invalid_argument("filter: flt is empty!");
    }

    typedef enum _FLT_STATUS
    {
        start, detect_type, read_data
    } FLT_STATUS;

    std::string tmpString(shape);
    FLT_STATUS status(start);
    std::regex typeReg("^([mnlbspc])(\\s*)");
    std::regex dataReg("^(-?\\d+\\.?\\d*)(\\s+)(-?\\d+\\.?\\d*)(\\s*)");
    std::smatch sm;
    std::string shapeType("");
    std::string output("");
    double point1(0.), point2(0.);
    while (tmpString.length() != 0)
    {
        if ((static_cast<uint8_t>(tmpString.at(0)) & 0x80) != 0)
        {
            // input is not ascii
            throw std::invalid_argument("filter: input is out of ASCII!");
        }

        switch (status)
        {
        case start:
        {
            if (std::regex_search(tmpString, sm, std::regex("^([mnlbsp])(\\s+)")))
            {
                status = read_data;
                shapeType = sm[1];
                tmpString = sm.suffix();
                output += (shapeType + " ");
            }
            else
            {
                throw std::invalid_argument("filter: shape syntax error: "
                                            "unexpected token in the begging of shape.");
            }

            continue; // goto next loop
        }
        case detect_type:
        {
            if (regex_search(tmpString, sm, typeReg))
            {
                status = read_data;
                shapeType = sm[1];
                tmpString = sm.suffix();
                output += (shapeType + " ");
            }
            else
            {
                std::string err(tmpString.substr(0, 1));
                err = "filter: shape syntax error: expect m, n, l, b, s, p "
                      "or c, but get " + err + " .";
                throw std::invalid_argument(err);
            }

            continue; // goto next loop
        }
        default: // read data
        {
            if (shapeType == "c")
            {
                status = detect_type;
                continue;
            }

            if (regex_search(tmpString, sm, dataReg))
            {
                std::string number(sm[1]);
                sscanf(number.c_str(), "%lf", &point1);

                number = sm[3];
                sscanf(number.c_str(), "%lf", &point2);

                tmpString = sm.suffix();
            }
            else
            {
                status = detect_type;
                continue;
            }

            break;
        }
        } //end switch

        std::pair<double, double> newPoints(flt(point1, point2, shapeType));
        output += (doubleToString(round(newPoints.first, FP_PRECISION)) + " ");
        output += (doubleToString(round(newPoints.second, FP_PRECISION)) + " ");
    } // end while (tmpString.length() != 0)

    return output;
}

std::string Shape::flatten(std::string &shape) THROW
{
    typedef enum _STATUS
    {
        find_start, get_data
    } STATUS;
    std::string tmpString = shape;
    STATUS status(find_start);
    double x0(0.), y0(0.), x1(0.), y1(0.);
    double x2(0.), y2(0.), x3(0.), y3(0.);
    std::string output("");
    std::string number("");
    std::regex startReg("(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+b(\\s+)");
    std::regex pointReg("^(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+"
                        "(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+"
                        "(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)(\\s*)");
    std::smatch sm;
    while (tmpString.length() != 0)
    {
        if ((static_cast<uint8_t>(tmpString.at(0)) & 0x80) != 0)
        {
            // input is not ascii
            throw std::invalid_argument("flatten: input is out of ASCII!");
        }

        switch (status)
        {
        case find_start:
        {
            if (regex_search(tmpString, sm, startReg))
            {
                number = sm.prefix();
                output += number;

                number = sm[1];
                sscanf(number.c_str(), "%lf", &x0);
                output += (number + " ");

                number = sm[2];
                sscanf(number.c_str(), "%lf", &y0);
                output += (number + " l ");
                tmpString = sm.suffix();
                number.clear();
                status = get_data;
            }
            else
            {
                output += tmpString.substr(number.length());
                tmpString.clear();
                continue;
            }

            break;
        }
        default: // get_data
        {
            if (regex_search(tmpString, sm, pointReg))
            {
                number = sm[1];
                sscanf(number.c_str(), "%lf", &x1);
                number = sm[2];
                sscanf(number.c_str(), "%lf", &y1);

                number = sm[3];
                sscanf(number.c_str(), "%lf", &x2);
                number = sm[4];
                sscanf(number.c_str(), "%lf", &y2);

                number = sm[5];
                sscanf(number.c_str(), "%lf", &x3);
                number = sm[6];
                sscanf(number.c_str(), "%lf", &y3);

                std::vector<double> line_points(curve4_to_lines(x0, y0,
                                                                x1, y1,
                                                                x2, y2,
                                                                x3, y3));

                for (size_t i = 0; i < line_points.size(); ++i)
                {
                    line_points.at(i) = round(line_points.at(i), FP_PRECISION);
                    output += (doubleToString(line_points.at(i)) + " ");
                }

                x0 = x3;
                y0 = y3;
                tmpString = sm.suffix();
                number.clear();
            }
            else
            {
                number = doubleToString(round(x3, FP_PRECISION)) + " ";
                number += doubleToString(round(y3, FP_PRECISION)) + " ";
                tmpString = number + tmpString;
                status = find_start;
            }
            break;
        }
        }
    } // end while (tmpString.length() != 0)

    return output;
}

std::string Shape::move(std::string &shape, double x, double y) THROW
{
    std::function<std::pair<double, double>(double, double, std::string &)> flt([&](
        double cx, double cy, std::string &input)
        {
            UNUSED(input);
            return std::make_pair(cx + x, cy + y);
        }
    );

    // here may throw exception
    return filter(shape, flt);
}

std::vector<std::map<std::string, double>>
Shape::to_pixels(std::string &shape) THROW
{
    // Scale values for supersampled rendering
    uint8_t upscale(SUPERSAMPLING);
    double downscale(0.125); // 1 / 8
    std::function<std::pair<double, double>(double, double, std::string &)> flt([&](double x, double y,
                                                                     std::string &input)
    {
        UNUSED(input);
        return std::make_pair(x * static_cast<double>(upscale),
                              y * static_cast<double>(upscale));
    });

    // Upscale shape for later downsampling
    // here may throw exception
    std::string newShape(filter(shape, flt));

    // here may throw exception
    auto tmpTuple(bounding(newShape));
    double x1(std::get<0>(tmpTuple)), y1(std::get<1>(tmpTuple));
    double x2(std::get<2>(tmpTuple)), y2(std::get<3>(tmpTuple));

    double shift_x(-(x1 - (static_cast<int64_t>(x1) % upscale)));
    double shift_y(-(y1 - (static_cast<int64_t>(y1) % upscale)));

    // here may throw exception
    newShape = move(newShape, shift_x, shift_y);

    // Create image
    double img_width(ceil((x2 + shift_x) * downscale) * upscale);
    double img_height(ceil((y2 + shift_y) * downscale) * upscale);
    std::vector<bool> img_data;
    img_data.resize(static_cast<size_t>(img_width * img_height));
    std::fill(img_data.begin(), img_data.end(), false);

    // Render shape on image
    // here may throw exception
    render_shape(img_width, img_height, img_data, newShape);

    // Extract pixels from image
    std::vector<std::map<std::string, double>> pixels;
    pixels.reserve(static_cast<size_t>((img_height-upscale) * (img_width-upscale)));
    double opacity(0.);
    for (double y = 0; y <= (img_height - upscale); y += upscale)
    {
        for (double x = 0; x <= (img_width - upscale); x += upscale)
        {
            opacity = 0.;
            for (uint8_t yy = 0; yy <= (upscale - 1); ++yy)
            {
                for (uint8_t xx = 0; xx <= (upscale - 1); ++xx)
                {
                    if (img_data.at(static_cast<size_t>(((y + yy) * img_width) + (x + xx))))
                    {
                        opacity += 255;
                    }
                } // end for xx
            } // end for yy

            if (opacity > 0.)
            {
                std::map<std::string, double> pixel;
                pixel["alpha"] = static_cast<uint8_t>(255 - (opacity * (downscale * downscale)));
                pixel["x"] = (x - shift_x) * downscale;
                pixel["y"] = (y - shift_y) * downscale;
                pixels.push_back(pixel);
            }
        } // end for x
    } // end for y

    return pixels;
}

// private member function
std::vector<double> Shape::curve4_subdivide(double x0, double y0,
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

bool Shape::curve4_is_flat(double x0, double y0,
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
        if (abs(degree(vecs.at(i - 1).first,
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

std::vector<double> Shape::curve4_to_lines(double px0, double py0,
                                           double px1, double py1,
                                           double px2, double py2,
                                           double px3, double py3) NOTHROW
{
    std::vector<double> pts;
    pts.reserve(4);

    std::function<void(double, double, double, double, double, double, double, double)>
    convert_recursive{ [&convert_recursive, this, &pts](
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

void Shape::render_shape(double width,
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
    shapeBak = flatten(shapeBak);
    // here may throw exception
    filter(shapeBak, flt);

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
    auto tmpTuple(bounding(shape));
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
                tmpDouble = trim(cx.at(0), 0, width);
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
