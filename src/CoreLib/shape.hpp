#ifndef SHAPE_HPP
#define SHAPE_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <functional>
#include <utility>

#include "math.hpp"
#include "utf8.hpp"

using namespace std;

class CoreShape : protected CoreMath, protected CoreUtf8
{
public:
    typedef enum _FIT_STATUS
    {
        start, detect_type, read_data
    } FIT_STATUS;

    CoreShape();

    // Filters shape points
    // It may throw invalid argument
    string filter(string &, function<pair<double, double>(double, double, string &)> &);

private:
};

#endif // SHAPE_HPP