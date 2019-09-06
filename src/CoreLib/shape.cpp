#include <regex>
#include <stdexcept>

#include <cstdio>

#include "shape.hpp"

CoreShape::CoreShape()
{
    CoreMath();
    CoreUtf8();
}

string CoreShape::filter(string &shape,
                         function<pair<double, double>(double, double, string &)> &flt)
{
    if (!flt)
    {
        throw invalid_argument("flt is empty!");
    }

    string tmpString(shape);
    FIT_STATUS status(start);
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