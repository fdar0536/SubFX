#include <regex>
#include <stdexcept>
#include <iostream>

#include <cstdio>
#include <cmath>

#include "YutilsCpp"
#include "internal/shape_internal.hpp"

using namespace PROJ_NAMESPACE::Yutils;

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
        output +=
                (PROJ_NAMESPACE::Utils::Misc::doubleToString(
                     Math::round(newPoints.first, FP_PRECISION)) + " ");
        output +=
                (PROJ_NAMESPACE::Utils::Misc::doubleToString(
                     Math::round(newPoints.second, FP_PRECISION)) + " ");
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

                std::vector<double>
                        line_points(Shape_Internal::curve4_to_lines(x0, y0,
                                                                    x1, y1,
                                                                    x2, y2,
                                                                    x3, y3));

                for (size_t i = 0; i < line_points.size(); ++i)
                {
                    line_points.at(i) = Math::round(
                                line_points.at(i), FP_PRECISION);
                    output +=
                            (PROJ_NAMESPACE::Utils::Misc::doubleToString(
                                 line_points.at(i)) + " ");
                }

                x0 = x3;
                y0 = y3;
                tmpString = sm.suffix();
                number.clear();
            }
            else
            {
                number =
                        PROJ_NAMESPACE::Utils::Misc::doubleToString(
                            Math::round(x3, FP_PRECISION)) + " ";
                number +=
                        PROJ_NAMESPACE::Utils::Misc::doubleToString(
                            Math::round(y3, FP_PRECISION)) + " ";
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
    Shape_Internal::render_shape(img_width, img_height, img_data, newShape);

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
                pixel["alpha"] = static_cast<uint8_t>(opacity * (downscale * downscale));
                pixel["x"] = (x - shift_x) * downscale;
                pixel["y"] = (y - shift_y) * downscale;
                pixels.push_back(pixel);
            }
        } // end for x
    } // end for y

    return pixels;
}

// private member function

