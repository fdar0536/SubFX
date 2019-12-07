#include "pybind11/stl.h"
#include "pybind11/attr.h"
#include "pybind11/functional.h"

#include "assparserpy.hpp"

namespace py = pybind11;

using namespace Yutils;

PYBIND11_MODULE(YutilsPy, m)
{
    m.doc() = "This is core library for SubFX, a modified version of Yutils.";

    /* in math.hpp */
    py::class_<Math>(m, "Math")

    .def(py::init<>())

    .def("arc_curve", &Math::arc_curve,
    "list[tuple(cx, cy)] = arc_curve(x, y, cx, cy, angle)\n"
    "Converts arc data to bezier curves.\n"
    "x & y is the arc starting point, cx & cy the arc center (= orientation point to keep the same distance to all arc points) and angle the angle in degree of the arc.\n"
    "For each 90° one curve is generated, so a maximum of 4 curves can span a circle.\n"
    "Curves are 3rd order bezier curves\n"
    "It returns a list of tuples. Each tuple is one of the control points of a bezier curve.\n"
    "Every four tuples describe a bezier curve.")

    .def("bezier", &Math::bezier,
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

    .def("degree", &Math::degree,
    "degree = degree(x1, y1, z1, x2, y2, z2)\n"
    "Calculates the degree between vectors x1|y1|z1 and x2|y2|z3\n")

    .def("distance", &Math::distance,
            py::arg("x"),
            py::arg("y"),
            py::arg("z") = 0.f,
    "length = distance(x, y[, z])\n"
    "Calculates length of given vector.\n")

    .def("line_intersect", &Math::line_intersect,
    "tuple(x, y) = line_intersect(x0, y0, x1, y1, x2, y2, x3, y3, strict)\n"
    "Calculates intersection point of two lines.\n"
    "x0, y0, x1, y1 are both points of line 1, x2, y2, x3, y3 are points of line 2.\n"
    "strict is a flag, determining the intersection has to be located on the lines.\n"
    "x, y can be the intersection point.\n"
    "If both lines are parallel, x is nil(= tuple(0, 0)).\n"
    "If strict is true and there's no intersection on the strict length lines, x is inf (std::numeric_limits<double>::infinity()).\n")

    .def("ortho", &Math::ortho,
    "tuple(rx, ry, rz) = ortho(x1, y1, z1, x2, y2, z2)\n"
    "Calculates the orthogonal vector to vectors x1|y1|z1 and x2|y2|z3.\n")

    .def("randomsteps", &Math::randomsteps,
    "r = randomsteps(min, max, step)\n"
    "Generates randomly a number in range min to max with gap size step between numbers.\n")

    .def("round", &Math::round,
        py::arg("x"),
        py::arg("dec") = 0.f,
    "r = round(x[, dec])\n"
    "Rounds x to nearest integer.\n"
    "Optionally, dec defines the position behind decimal point to round to.\n")

    .def("stretch", &Math::stretch,
    "tuple(rx, ry, rz) = stretch(x, y, z, length)\n"
    "Stretches vector x|y|z to length length.\n")

    .def("trim", &Math::trim,
    "r = trim(x, min, max)\n"
    "If x is smaller than min, returns min.\n"
    "If x is greater than max, returns max.\n"
    "Otherwise returns x.\n")

    .def("ellipse", &Math::ellipse,
    "tuple(new_x, new_y) = ellipse(x, y, width, height, angle)\n"
    "Calculates a point on ellipse with given angle, center point x/y, width and height.\n")

    .def("randomway", &Math::randomway,
    "direction = randomway()\n"
    "Returns randomly -1 or 1.\n")

    .def("rotate", &Math::rotate,
    "rotated_point = rotate(point, axis, angle)\n"
    "Allows to rotate a point in 3D room.\n");

    /* in shape.hpp */
    py::class_<Shape>(m, "Shape")

    .def(py::init<>())

    .def("bounding", &Shape::bounding,
    "tuple(x0, y0, x1, y1) = bounding(shape)\n"
    "Calculates the bounding box of shape shape.\n"
    "x0|y0 is the upper-left and x1|y1 the lower-right corner of the rectangle.\n")

    .def("filter", &Shape::filter,
    "new_shape = filter(shape, flt)\n"
    "Filters points of shape shape by function flt and returns a new one.\n"
    "flt receives point coordinates x and y as well as the point type and have to return a list contains 2 numbers, replacing x and y.\n"
    "example:\n"
    "import YutilsPy\n"
    "shape = YutilsPy.Shape()\n"
    "\n"
    "def flt(x, y, pointType):\n"
    "    if pointType == \"m\":\n"
    "        return [x + 5, y + 2]\n"
    "\n"
    "    return [x + 2, y + 5]\n"
    "\n"
    "test = \"m 1 2 l 3 4 5 6 b 7 8 9 10 11 12 13 14 c\"\n"
    "print(shape.filter(test, flt))\n")

    .def("flatten", &Shape::flatten,
    "flattened_shape = flatten(shape)\n"
    "Converts all 3rd order bezier curves in shape shape to lines,\n"
    "creating a new shape.\n")

    .def("move", &Shape::move,
    "new_shape = move(shape, x, y)\n"
    "Shifts points of shape shape horizontally by x and vertically by y,\n"
    "creating a new shape.\n")

    .def("to_pixels", &Shape::to_pixels,
    "pixels = to_pixels(shape)\n"
    "Renders shape shape and returns pixels.\n"
    "pixels is a list of dictionaries, each one with following fields:\n"
    "x: horizontal position\n"
    "y: vertical position\n"
    "alpha: opacity\n");

    /* in ass.hpp */
    py::class_<Ass>(m, "Ass")

    .def(py::init<>())

    .def("stringToMs", &Ass::stringToMs,
    "ms_ass = stringToMs(ass_ms)\n"
    "Converts time to numeric.\n"
    "ass_ms is a string in ASS format H:MM:SS.XX (H=Hours, M=Minutes, S=Seconds, X=Milliseconds*10).\n"
    "ms_ass is milliseconds as number.\n")

    .def("msToString", &Ass::msToString,
    "ass_ms = msToString(ms_ass)\n"
    "Converts time to ASS presentation.\n"
    "ass_ms is string in ASS format H:MM:SS.XX (H=Hours, M=Minutes, S=Seconds, X=Milliseconds*10)\n"
    "ms_ass is milliseconds as number.\n")

    .def("stringToColorAlpha", &Ass::stringToColorAlpha,
    "tuple(r, g, b, a) = stringToColorAlpha(input)\n"
    "Converts color, alpha or color+alpha to numeric\n"
    "input is a string as ASS color (&HBBGGRR&), alpha (&HAA&) or both (&HAABBGGRR)\n")

    .def("colorAlphaToString", &Ass::colorAlphaToString,
    "output = colorAlphaToString(input)\n"
    "input = [r, g, b] or [a]\n"
    "Converts color or alpha to ASS presentation.\n"
    "input is a list must contain 1 or 3 elements.\n"
    "if input has 3 elements, output will be an ASS color presentation(&HBBGGRR&),\n"
    "else output will be an ASS alpha presentation(&HAA&).");

    py::class_<AssParserPy>(m, "AssParser")

    .def(py::init<std::string &>(),
    "Create a AssParser object.\n"
    "input string is ass's file name.\n")

    .def("meta", &AssParserPy::meta,
    "Returns ASS meta data as dictionary with following fields\n"
    "wrap_style: text line wrapping mode as number\n"
    "scaled_border_and_shadow: borders and shadows should be implicated in script-to-frame scale?\n"
    "play_res_x: script horizontal resolution\n"
    "play_res_y: script vertical resolution\n")

    .def("styles", &AssParserPy::styles,
    "Returns ASS styles as dictionary. Dictionary keys are style names, values are dictionaries with following fields:\n"
    "fontname: font face name\n"
    "fontsize: font size\n"
    "bold: bold weight?\n"
    "italic: italic style?\n"
    "underline: underline decoration?\n"
    "strikeout: strikeout decoration?\n"
    "scale_x: horizontal scale in percent\n"
    "scale_y: vertical scale in percent\n"
    "spacing: intercharacter spacing\n"
    "angle: z-axis rotation angle\n"
    "border_style: border is an outline box?\n"
    "outline: outline width\n"
    "shadow: shadow distance right-down\n"
    "alignment: object alignment on screen (see keypad)\n"
    "margin_l: margin from left screen border\n"
    "margin_r: margin from right screen border\n"
    "margin_v: margin from vertical screen borders\n"
    "encoding: codepage to interpret text\n")

    .def("dialogs", &AssParserPy::dialogs,
    "Returns ASS dialogs as list. Each entry is a dictionary with following fields:\n"
    "comment: dialog is comment?\n"
    "layer: dialog layer number\n"
    "start_time: dialog start time in milliseconds\n"
    "end_time: dialog end time in milliseconds\n"
    "style: dialog style name\n"
    "actor: dialog actor name\n"
    "margin_l: dialog margin to left edge\n"
    "margin_r: dialog margin to right\n"
    "margin_v: dialog margin to horizontal edges\n"
    "effect: dialog effect description\n"
    "text: dialog text\n")

    .def("upgradeDialogs", &AssParserPy::upgradeDialogs,
    "Add the following fields to dialogs dictionary.\n"
    "After upgrade, you can get upgraded dialogs by calling dialogs().\n"
    "i: dialog index\n"
    "duration: dialog duration in milliseconds\n"
    "mid_time: dialog mid time in milliseconds\n"
    "styleref: reference to dialog related style table\n"
    "text_stripped: dialog text without tags\n"
    "width: dialog width\n"
    "height: dialog height\n"
    "ascent: dialog font ascent\n"
    "descent: dialog font descent\n"
    "internal_leading: dialog font internal leading\n"
    "external_leading: dialog font external leading\n"
    "left: dialog left position\n"
    "center: dialog center position\n"
    "right: dialog right position\n"
    "x: dialog horizontal position by alignment\n"
    "top: dialog top position\n"
    "middle: dialog middle position\n"
    "bottom: dialog bottom positon\n"
    "y: dialog vertical position by alignment\n"
    "text_chunked: raw text in tag+text chunks with followings fields:\n"
    "   tags: content between brackets {} in front of text\n"
    "   text: text after brackets{}\n"
    "syls: raw text in sylable chunks with following fields:\n"
    "   i: sylable index\n"
    "   start_time: sylable start time in milliseconds\n"
    "   mid_time: sylable mid time in milliseconds\n"
    "   end_time: sylable end time in milliseconds\n"
    "   duration: sylable duration in milliseconds\n"
    "   tags: sylable tags beside her duration definition\n"
    "   text: sylable text\n"
    "   prespace: sylable spaces number in front\n"
    "   postspace: sylable spaces number behind\n"
    "   width: sylable width\n"
    "   height: sylable height\n"
    "   ascent: sylable font ascent\n"
    "   descent: sylable font descent\n"
    "   internal_leading: sylable font internal leading\n"
    "   external_leading: sylable font external leading\n"
    "   left: sylable left position\n"
    "   center: sylable center position\n"
    "   right: sylable right position\n"
    "   x: sylable horizontal position by alignment\n"
    "   top: sylable top position\n"
    "   middle: sylable middle position\n"
    "   bottom: sylable bottom position\n"
    "   y: sylable vertical position by alignment\n"
    "words: raw text in word chunks with following fields:\n"
    "   i: word index\n"
    "   start_time: word start time in milliseconds\n"
    "   mid_time: word mid time in milliseconds\n"
    "   end_time: word end time in milliseconds\n"
    "   duration: word duration in milliseconds\n"
    "   text: word text\n"
    "   prespace: word spaces number in front\n"
    "   postspace: word spaces number behind\n"
    "   width: word width\n"
    "   height: word height\n"
    "   ascent: word font ascent\n"
    "   descent: word font descent\n"
    "   internal_leading: word font internal leading\n"
    "   external_leading: word font external leading\n"
    "   left: word left position\n"
    "   center: word center position\n"
    "   right: word right position\n"
    "   x: word horizontal position by alignment\n"
    "   top: word top position\n"
    "   middle: word middle position\n"
    "   bottom: word bottom position\n"
    "   y: word vertical position by alignment\n"
    "chars: raw text in character chunks with followings fields:\n"
    "   i: character index\n"
    "   start_time: character start time in milliseconds\n"
    "   mid_time: character mid time in milliseconds\n"
    "   end_time: character end time in milliseconds\n"
    "   duration: character duration in milliseconds\n"
    "   text: character text\n"
    "   syl_i: index of sylable which contains the character\n"
    "   word_i: index of word which contains the character\n"
    "   width: character width\n"
    "   height: character height\n"
    "   ascent: character font ascent\n"
    "   descent: character font descent\n"
    "   internal_leading: character font internal leading\n"
    "   external_leading: character font external leading\n"
    "   left: character left position\n"
    "   center: character center position\n"
    "   right: character right position\n"
    "   x: character horizontal position by alignment\n"
    "   top: character top position\n"
    "   middle: character middle position\n"
    "   bottom: character bottom position\n"
    "   y: character vertical position by alignment\n"
    "leadin: dialog pretime / duration from last dialog to this one (in case there's none, it's 1000.1)\n"
    "leadout: dialog posttime / duration from this dialog to next one (in case there's none, it's 1000.1)\n");

    /* in file.hpp */
    py::class_<File>(m, "CoreFile", "This class is for handling file IO of Ass.")
    .def(py::init<const std::string &, const std::string &>(),
        py::arg("fileName"),
        py::arg("assHeader") = "",
    "Create a CoreFile object.\n"
    "fileName as its name.\n"
    "assHeader as its name, too.\n"
    "If assHeader is an empty string, the file will be opened in append mode.\n"
    "If not, the file will be opened in write mode.\n")

    .def("writeAssFile",
         py::overload_cast<std::vector<std::string>&>(&File::writeAssFile),
    "writeFile(assBuf)\n"
    "Write all contents in assBuf to the file.\n"
    "assBuf is a list of strings.\n")

    .def("reset", &File::reset,
        py::arg("fileName"),
        py::arg("assHeader") = ""
    "reset(fileName, assHeader)\n"
    "Same as constructor, but this is set new content to object.\n")

    .def("isAppend", &File::isAppend,
    "It returns this object's file is in append mode or not.");

    /* in fonthandle.hpp */
    py::class_<FontHandle>(m, "FontHandle")
    .def(py::init<std::string &, bool, bool, bool,
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
