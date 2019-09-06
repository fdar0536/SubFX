#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/attr.h"
#include "pybind11/functional.h"

#include "assparserpy.hpp"
#include "../fonthandle.hpp"
#include "../file.hpp"
#include "../shape.hpp"

namespace py = pybind11;

using namespace std;

PYBIND11_MODULE(CoreLib, m)
{
    m.doc() = "This is core library for TCFX, a modified version of Yutils.";

    /* in math.hpp */
    py::class_<CoreMath>(m, "CoreMath")
    
    .def(py::init<>())
    
    .def("arc_curve", &CoreMath::arc_curve,
    "list[tuple(cx, cy)] = arc_curve(x, y, cx, cy, angle)\n"
    "Converts arc data to bezier curves.\n"
    "x & y is the arc starting point, cx & cy the arc center (= orientation point to keep the same distance to all arc points) and angle the angle in degree of the arc.\n"
    "For each 90° one curve is generated, so a maximum of 4 curves can span a circle.\n"
    "Curves are 3rd order bezier curves\n"
    "It returns a list of tuples. Each tuple is one of the control points of a bezier curve.\n"
    "Every four tuples describe a bezier curve.")

    .def("bezier", &CoreMath::bezier,
        py::arg("pct"),
        py::arg("pts"),
        py::arg("is3D") = false,
    "tuple(x, y, z) = bezier(pct, pts, is3D)\n"
    "Calculates a point on a bezier curve of any order."
    "pct is the position on the curve in range 0<=x<=1.\n"
    "pts is a list of tuples, each one containing 3 numbers as curve point.\n"
    "is3D is boolean indicates pts is 3D or not.\n"
    "if \"is3D\" is set to false, the third number of tuples of pts will be ignored,\n"
    "and the third number of returned tuple always is zero.\n")

    .def("degree", &CoreMath::degree,
    "degree = degree(x1, y1, z1, x2, y2, z2)\n"
    "Calculates the degree between vectors x1|y1|z1 and x2|y2|z3\n")

    .def("distance", &CoreMath::distance,
            py::arg("x"),
            py::arg("y"),
            py::arg("z") = 0.f,
    "length = distance(x, y[, z])\n"
    "Calculates length of given vector.\n")

    .def("line_intersect", &CoreMath::line_intersect,
    "tuple(x, y) = line_intersect(x0, y0, x1, y1, x2, y2, x3, y3, strict)\n"
    "Calculates intersection point of two lines.\n"
    "x0, y0, x1, y1 are both points of line 1, x2, y2, x3, y3 are points of line 2.\n"
    "strict is a flag, determining the intersection has to be located on the lines.\n"
    "x, y can be the intersection point.\n"
    "If both lines are parallel, x is nil(= tuple(0, 0)).\n"
    "If strict is true and there's no intersection on the strict length lines, x is inf (std::numeric_limits<double>::infinity()).\n")

    .def("ortho", &CoreMath::ortho,
    "tuple(rx, ry, rz) = ortho(x1, y1, z1, x2, y2, z2)\n"
    "Calculates the orthogonal vector to vectors x1|y1|z1 and x2|y2|z3.\n")

    .def("randomsteps", &CoreMath::randomsteps,
    "r = randomsteps(min, max, step)\n"
    "Generates randomly a number in range min to max with gap size step between numbers.\n")

    .def("round", &CoreMath::round,
        py::arg("x"),
        py::arg("dec") = 0.f,
    "r = round(x[, dec])\n"
    "Rounds x to nearest integer.\n"
    "Optionally, dec defines the position behind decimal point to round to.\n")

    .def("stretch", &CoreMath::stretch,
    "tuple(rx, ry, rz) = stretch(x, y, z, length)\n"
    "Stretches vector x|y|z to length length.\n")

    .def("trim", &CoreMath::trim,
    "r = trim(x, min, max)\n"
    "If x is smaller than min, returns min.\n"
    "If x is greater than max, returns max.\n"
    "Otherwise returns x.\n")
    
    .def("ellipse", &CoreMath::ellipse,
    "tuple(new_x, new_y) = ellipse(x, y, width, height, angle)\n"
    "Calculates a point on ellipse with given angle, center point x/y, width and height.\n")
    
    .def("randomway", &CoreMath::randomway,
    "direction = randomway()\n"
    "Returns randomly -1 or 1.\n")
    
    .def("rotate", &CoreMath::rotate,
    "rotated_point = rotate(point, axis, angle)\n"
    "Allows to rotate a point in 3D room.\n");

    /* in shape.hpp */
    py::class_<CoreShape>(m, "CoreShape")

    .def(py::init<>())
    
    .def("filter", &CoreShape::filter,
    "new_shape = filter(shape, flt)\n"
    "Filters points of shape shape by function flt and returns a new one.\n"
    "filter receives point coordinates x and y as well as the point type and have to return a list contains 2 numbers, replacing x and y.\n"
    "example:\n"
    "import CoreLib\n"
    "shape = CoreLib.CoreShape()\n"
    "\n"
    "def flt(x, y, pointType):\n"
    "    if pointType == \"m\":\n"
    "        return [x + 5, y + 2]\n"
    "\n"
    "    return [x + 2, y + 5]\n"
    "\n"
    "test = \"m 1 2 l 3 4 5 6 b 7 8 9 10 11 12 13 14 c\"\n"
    "print(shape.filter(test, flt))\n");

    /* in ass.hpp */
    py::class_<CoreAss>(m, "CoreAss")
    
    .def(py::init<>())
    
    .def("stringToMs", &CoreAss::stringToMs,
    "ms_ass = stringToMs(ass_ms)\n"
    "Converts time to numeric.\n"
    "ass_ms is a string in ASS format H:MM:SS.XX (H=Hours, M=Minutes, S=Seconds, X=Milliseconds*10).\n"
    "ms_ass is milliseconds as number.\n")
    
    .def("msToString", &CoreAss::msToString,
    "ass_ms = msToString(ms_ass)\n"
    "Converts time to ASS presentation.\n"
    "ass_ms is string in ASS format H:MM:SS.XX (H=Hours, M=Minutes, S=Seconds, X=Milliseconds*10)\n"
    "ms_ass is milliseconds as number.\n")
    
    .def("stringToColorAlpha", &CoreAss::stringToColorAlpha,
    "tuple(r, g, b, a) = stringToColorAlpha(input)\n"
    "Converts color, alpha or color+alpha to numeric\n"
    "input is a string as ASS color (&HBBGGRR&), alpha (&HAA&) or both (&HAABBGGRR)\n")
    
    .def("colorAlphaToString", &CoreAss::colorAlphaToString,
    "output = colorAlphaToString(input)\n"
    "input = [r, g, b] or [a]\n"
    "Converts color or alpha to ASS presentation.\n"
    "input is a list must contain 1 or 3 elements.\n"
    "if input has 3 elements, output will be an ASS color presentation(&HBBGGRR&),\n"
    "else output will be an ASS alpha presentation(&HAA&).");

    /* in file.hpp */
    py::class_<CoreFile>(m, "CoreFile", "This class is for handling file IO of Ass.")
    .def(py::init<const string &, const string &>(),
        py::arg("fileName"),
        py::arg("assHeader") = "",
    "Create a CoreFile object.\n"
    "fileName as its name.\n"
    "assHeader as its name, too.\n"
    "If assHeader is an empty string, the file will be opened in append mode.\n"
    "If not, the file will be opened in write mode.\n")
    
    .def("writeAssFile", &CoreFile::writeAssFile,
    "writeFile(assBuf)\n"
    "Write all contents in assBuf to the file.\n"
    "assBuf is a list of strings.\n")
    
    .def("reset", &CoreFile::reset,
        py::arg("fileName"),
        py::arg("assHeader") = ""
    "reset(fileName, assHeader)\n"
    "Same as constructor, but this is set new content to object.\n")
    
    .def("isAppend", &CoreFile::isAppend,
    "It returns this object's file is in append mode or not.");

    /* in fonthandle.hpp */
    py::class_<FontHandle>(m, "FontHandle")
    .def(py::init<string &, bool, bool, bool,
               bool, int, int, int, int>(),
               py::arg("family"),
               py::arg("bold"),
               py::arg("italic"),
               py::arg("underline"),
               py::arg("strikeout"),
               py::arg("size"),
               py::arg("xscale") = 1,
               py::arg("yscale") = 1,
               py::arg("hspace") = 0,
    "family is the font family.\n"
    "If bold is true, font has bold weight.\n"
    "If italic is true, font has italic style.\n"
    "If underline is true, font has underline decoration.\n"
    "If strikeout is true, font has strikeout decoration.\n"
    "size is the font size.\n"
    "xscale and yscale can define horizontal & vertical scale.\n"
    "hspace can define intercharacter space.\n")

    .def("metrics", &FontHandle::metrics,
    "metrics = metrics()\n"
    "Returns font metrics as dictionary with followings fields:\n"
    "ascent: font ascent\n"
    "descent: font descent\n"
    "internal_leading: font internal leading\n"
    "external_leading: font external leading\n"
    "height: font maximal height\n")

    .def("text_extents", &FontHandle::text_extents,
    "extents = text_extents(text)\n"
    "Returns extents of text with given font as dictionary with followings fields:\n"
    "width: text width\n"
    "height: text height\n")

    .def("text_to_shape", &FontHandle::text_to_shape,
    "shape = text_to_shape(text)\n"
    "Converts text with given font to an ASS shape.\n");
}
