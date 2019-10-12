#include <regex>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include <cstdio>
#include <cmath>

#include "config.h"

#ifdef ENABLE_SSE2
#include <emmintrin.h> // SSE2
#endif

#include "shape.hpp"

#define CURVE_TOLERANCE 1 // Angle in degree to define a curve as flat

CoreShape::CoreShape()
{
    CoreMath();
}

tuple<double, double, double, double> CoreShape::bounding(string &shape)
{
    // Bounding data
    double x0(0.f), y0(0.f), x1(0.f), y1(0.f);
    uint8_t flag(0);
    function<pair<double, double>(double, double, string&)> flt(
        [&](double x, double y, string &pointType)
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

            x0 = min(x0, x);
            y0 = min(y0, y);
            x1 = max(x1, x);
            y1 = max(y1, y);

            return pair<double, double>();
        }
    );

    filter(shape, flt);

    return make_tuple(x0, y0, x1, y1);
}

string CoreShape::filter(string &shape,
                         function<pair<double, double>(double, double, string &)> &flt)
{
    if (!flt)
    {
        throw invalid_argument("flt is empty!");
    }

    typedef enum _FLT_STATUS
    {
        start, detect_type, read_data
    } FLT_STATUS;

    string tmpString(shape);
    FLT_STATUS status(start);
    regex typeReg("^([mnlbspc])(\\s*)");
    regex dataReg("^(-?\\d+\\.?\\d*)(\\s+)(-?\\d+\\.?\\d*)(\\s*)");
    smatch sm;
    string shapeType("");
    string output("");
    double point1(0.f), point2(0.f);
    while (tmpString.length() != 0)
    {
        if (((uint8_t)tmpString.at(0) & 0x80) != 0)
        {
            // input is not ascii
            throw invalid_argument("input is out of ASCII!");
        }

        switch (status)
        {
        case start:
        {
            if (regex_search(tmpString, sm, regex("^([mnlbsp])(\\s+)")))
            {
                status = read_data;
                shapeType = sm[1];
                tmpString = sm.suffix();
                output += (shapeType + " ");
            }
            else
            {
                throw invalid_argument("shape syntax error: unexpected token in the begging of shape.");
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
                string err(tmpString.substr(0, 1));
                err = "shape syntax error: expect m, n, l, b, s, p or c, but get " + err + " .";
                throw invalid_argument(err);
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
                string number(sm[1]);
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

        pair<double, double> newPoints(flt(point1, point2, shapeType));
        output += (doubleToString(round(newPoints.first, FP_PRECISION)) + " ");
        output += (doubleToString(round(newPoints.second, FP_PRECISION)) + " ");
    } // end while (tmpString.length() != 0)

    return output;
}

string CoreShape::flatten(string &shape)
{
    typedef enum _STATUS
    {
        find_start, get_data
    } STATUS;
    string tmpString = shape;
    STATUS status(find_start);
    double x0(0.f), y0(0.f), x1(0.f), y1(0.f);
    double x2(0.f), y2(0.f), x3(0.f), y3(0.f);
    string output("");
    string number("");
    regex startReg("(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+b(\\s+)");
    regex pointReg("^(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)\\s+(-?\\d+\\.?\\d*)(\\s*)");
    smatch sm;
    while (tmpString.length() != 0)
    {
        if (((uint8_t)tmpString.at(0) & 0x80) != 0)
        {
            // input is not ascii
            throw invalid_argument("input is out of ASCII!");
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

                vector<double> line_points(curve4_to_lines(x0, y0, x1, y1, x2, y2, x3, y3));

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

string CoreShape::move(string &shape, double x, double y)
{
    function<pair<double, double>(double, double, string &)> flt([&](
        double cx, double cy, string &input)
        {
            UNUSED(input);
            return make_pair(cx + x, cy + y);
        }
    );

    return filter(shape, flt);
}

vector<map<string, double>> CoreShape::to_pixels(string &shape)
{
    // Scale values for supersampled rendering
    uint8_t upscale(SUPERSAMPLING);
    double downscale(0.125f); // 1 / 8
    function<pair<double, double>(double, double, string &)> flt([&](double x, double y,
                                                                     string &input)
    {
        UNUSED(input);
        return make_pair(x * static_cast<double>(upscale),
                         y * static_cast<double>(upscale));
    });

    // Upscale shape for later downsampling
    string newShape(filter(shape, flt));
    tuple<double, double, double, double> tmpTuple(bounding(newShape));
    double x1(get<0>(tmpTuple)), y1(get<1>(tmpTuple));
    double x2(get<2>(tmpTuple)), y2(get<3>(tmpTuple));

    double shift_x(-(x1 - ((int64_t)x1 % upscale)));
    double shift_y(-(y1 - ((int64_t)y1 % upscale)));

    newShape = move(newShape, shift_x, shift_y);

    // Create image
    double img_width(ceil((x2 + shift_x) * downscale) * upscale);
    double img_height(ceil((y2 + shift_y) * downscale) * upscale);
    vector<bool> img_data;
    img_data.resize(img_width * img_height);
    fill(img_data.begin(), img_data.end(), false);

    // Render shape on image
    render_shape(img_width, img_height, img_data, newShape);

    // Extract pixels from image
    vector<map<string, double>> pixels;
    pixels.reserve((img_height-upscale) * (img_width-upscale));
    double opacity(0.f);
    for (double y = 0; y <= (img_height - upscale); y += upscale)
    {
        for (double x = 0; x <= (img_width - upscale); x += upscale)
        {
            opacity = 0.f;
            for (uint8_t yy = 0; yy <= (upscale - 1); ++yy)
            {
                for (uint8_t xx = 0; xx <= (upscale - 1); ++xx)
                {
                    if (img_data.at(((y + yy) * img_width) + (x + xx)))
                    {
                        opacity += 255;
                    }
                } // end for xx
            } // end for yy

            if (opacity > 0.f)
            {
                map<string, double> pixel;
                pixel["alpha"] = (uint8_t)(255 - (opacity * (downscale * downscale)));
                pixel["x"] = (x - shift_x) * downscale;
                pixel["y"] = (y - shift_y) * downscale;
                pixels.push_back(pixel);
            }
        } // end for x
    } // end for y

    return pixels;
}


// private member function
vector<double> CoreShape::curve4_subdivide(double x0, double y0,
                                           double x1, double y1,
                                           double x2, double y2,
                                           double x3, double y3,
                                           double pct)
{
    vector<double> ret;
    ret.reserve(16);
    ret.push_back(x0);
    ret.push_back(y0);
#ifdef ENABLE_SSE2
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

bool CoreShape::curve4_is_flat(double x0, double y0,
                               double x1, double y1,
                               double x2, double y2,
                               double x3, double y3,
                               double tolerance)
{
    vector<pair<double, double>> vecs;
    vecs.reserve(3);

    vecs.push_back(make_pair(x1 - x0, y1 - y0));
    vecs.push_back(make_pair(x2 - x1, y2 - y1));
    vecs.push_back(make_pair(x3 - x2, y3 - y2));

    size_t i(0), n(3);
    while (i < n)
    {
        if (vecs.at(i).first == 0 && vecs.at(i).second == 0)
        {
            vecs.erase(vecs.begin() + i);
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

vector<double> CoreShape::curve4_to_lines(double px0, double py0,
                                          double px1, double py1,
                                          double px2, double py2,
                                          double px3, double py3)
{
    vector<double> pts;
    pts.reserve(4);

    function<void(double, double, double, double, double, double, double, double)> 
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
            vector<double> resVec(curve4_subdivide(x0, y0, x1, y1, x2, y2, x3, y3, 0.5));
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

void CoreShape::render_shape(double width, double height,
                             vector<bool> &image, string &shape)
{
    vector<tuple<double, double, double, double>> lines;
    lines.reserve(500);
    vector<double> last_point;
    last_point.reserve(2);
    vector<double> last_move;
    last_move.reserve(2);
    function<pair<double, double>(double, double, string &)>
    flt([&](double x, double y, string &typ)
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
                    lines.push_back(make_tuple(last_point.at(0),
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
                    lines.push_back(make_tuple(last_point.at(0),
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

        return pair<double, double>();
    });

    string shapeBak(shape);
    shapeBak = flatten(shapeBak);
    filter(shapeBak, flt);

    if (last_move.size() > 0)
    {
        if (last_move.at(1) != last_point.at(1) &&
            !(last_point.at(1) < 0 && last_move.at(1) < 0) &&
            !(last_point.at(1) > height && last_move.at(1) > height))
        {
            lines.push_back(make_tuple(last_point.at(0),
                                       last_point.at(1),
                                       last_move.at(0) - last_point.at(0),
                                       last_move.at(1) - last_point.at(1)));
        }
    }

    // Calculates line x horizontal line intersection
    function<vector<double>(double, double, double, double, double)>
    line_x_hline([](double x, double y, double vx, double vy, double y2)
    {
        vector<double> ret;
        ret.reserve(2);
        if (vy != 0)
        {
            double s((y2 - y) / vy);
            if (s >= 0.f && s <= 1.f)
            {
                ret.push_back(x + s * vx);
                ret.push_back(y2);
            }
        }

        return ret;
    });

    auto tmpTuple(bounding(shape));
    for (double y = max(floor(get<1>(tmpTuple)), (double)0.f);
         y <= (min(ceil(get<3>(tmpTuple)), height) - 1);
         ++y)
    {
        vector<pair<double, double>> row_stops;
        row_stops.reserve(lines.size());

        for (size_t i = 0; i < lines.size(); ++i)
        {
            auto line(lines.at(i));
            vector<double> cx(line_x_hline(get<0>(line),
                                           get<1>(line),
                                           get<2>(line),
                                           get<3>(line),
                                           y + 0.5));
            if (cx.size() != 0)
            {
                row_stops.push_back(make_pair(trim(cx.at(0), 0, width),
                                              get<3>(line) > 0.f ? 1.f : -1.f));
            }
        } //end for i

        if (row_stops.size() > 0)
        {
            sort(row_stops.begin(), row_stops.end(), [](const pair<double, double> &a,
                                                        const pair<double, double> &b)
                                                        {
                                                            return (a.first < b.first);
                                                        });
            double status(0.f), row_index(y * width);
            for (size_t i = 0; i < row_stops.size() - 1; ++i)
            {
                status += row_stops.at(i).second;
                if (status != 0.f)
                {
                    for (double x = ceil(row_stops.at(i).first - 0.5);
                         x <= (floor(row_stops.at(i + 1).first + 0.5) - 1);
                         ++x)
                    {
                        image.at(row_index + x) = true;
                    } // end for x
                }
            } // end for i
        }
    } // end for y
}
