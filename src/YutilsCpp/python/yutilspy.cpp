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

#include "pybind11/stl.h"
#include "pybind11/attr.h"
#include "pybind11/functional.h"

#include "internal/basecommon.h"
#include "YutilsCpp"

namespace py = pybind11;

using namespace PROJ_NAMESPACE::Yutils;

PYBIND11_MODULE(SubFX_YutilsPy, m)
{
    m.doc() = "This is core library for SubFX, a modified version of Yutils.";

    m.def("version", [](){
        return PROJ_VERSION;
    });

    /* in math.hpp*/
    auto math(m.def_submodule("Math"));

    math.def("arc_curve", &Math::arc_curve,
             "list[tuple(cx, cy)] = arc_curve(x, y, cx, cy, angle)\n"
             "Converts arc data to bezier curves.\n"
             "x & y is the arc starting point, cx & cy the arc center"
             " (= orientation point to keep the same distance to all arc points)"
             " and angle the angle in degree of the arc.\n"
             "For each 90° one curve is generated, "
             "so a maximum of 4 curves can span a circle.\n"
             "Curves are 3rd order bezier curves\n"
             "It returns a list of tuples. "
             "Each tuple is one of the control points of a bezier curve.\n"
             "Every four tuples describe a bezier curve.\n");

    math.def("bezier", &Math::bezier,
             py::arg("pct"),
             py::arg("pts"),
             py::arg("is3D") = false,
         "tuple(x, y, z) = bezier(pct, pts, is3D)\n"
         "Calculates a point on a bezier curve of any order."
         "pct is the position on the curve in range 0<=x<=1.\n"
         "pts is a list of tuples, "
         "each one containing 3 numbers as curve point.\n"
         "is3D is boolean indicates pts is 3D or not.\n"
         "If \"is3D\" is set to false, "
         "the third number of tuples of pts will be ignored,\n"
         "and the third number of returned tuple always is zero.\n");

    math.def("degree", &Math::degree,
             "degree = degree(x1, y1, z1, x2, y2, z2)\n"
             "Calculates the degree between vectors x1|y1|z1 and x2|y2|z3\n");

    math.def("distance", &Math::distance,
            py::arg("x"),
            py::arg("y"),
            py::arg("z") = 0.f,
    "length = distance(x, y[, z])\n"
    "Calculates length of given vector.\n");

    math.def("line_intersect", &Math::line_intersect,
    "tuple(x, y) = line_intersect(x0, y0, x1, y1, x2, y2, x3, y3, strict)\n"
    "Calculates intersection point of two lines.\n"
    "x0, y0, x1, y1 are both points of line 1, "
    "x2, y2, x3, y3 are points of line 2.\n"
    "strict is a flag, "
    "determining the intersection has to be located on the lines.\n"
    "x, y can be the intersection point.\n"
    "If both lines are parallel, x is nil(= tuple(0, 0)).\n"
    "If strict is true and there's no intersection on the strict length lines, "
    "x is inf (std::numeric_limits<double>::infinity()).\n");

    math.def("ortho", &Math::ortho,
    "tuple(rx, ry, rz) = ortho(x1, y1, z1, x2, y2, z2)\n"
    "Calculates the orthogonal vector to vectors x1|y1|z1 and x2|y2|z3.\n");

    math.def("randomsteps", &Math::randomsteps,
    "r = randomsteps(min, max, step)\n"
    "Generates randomly a number in range min to max "
    "with gap size step between numbers.\n");

    math.def("round", &Math::round,
        py::arg("x"),
        py::arg("dec") = 0.f,
    "r = round(x[, dec])\n"
    "Rounds x to nearest integer.\n"
    "Optionally, dec defines the position behind decimal point to round to.\n");

    math.def("stretch", &Math::stretch,
    "tuple(rx, ry, rz) = stretch(x, y, z, length)\n"
    "Stretches vector x|y|z to length length.\n");

    math.def("trim", &Math::trim,
    "r = trim(x, min, max)\n"
    "If x is smaller than min, returns min.\n"
    "If x is greater than max, returns max.\n"
    "Otherwise returns x.\n");

    math.def("ellipse", &Math::ellipse,
    "tuple(new_x, new_y) = ellipse(x, y, width, height, angle)\n"
    "Calculates a point on ellipse with given angle, "
    "center point x/y, width and height.\n");

    math.def("randomway", &Math::randomway,
    "direction = randomway()\n"
    "Returns randomly -1 or 1.\n");

    math.def("rotate", &Math::rotate,
    "rotated_point = rotate(point, axis, angle)\n"
    "Allows to rotate a point in 3D room.\n");

    /* in shape.hpp */
    auto shape(m.def_submodule("Shape"));

    shape.def("bounding", &Shape::bounding,
    "tuple(x0, y0, x1, y1) = bounding(shape)\n"
    "Calculates the bounding box of shape shape.\n"
    "x0|y0 is the upper-left and "
    "x1|y1 the lower-right corner of the rectangle.\n");

    shape.def("filter", &Shape::filter,
    "new_shape = filter(shape, flt)\n"
    "Filters points of shape shape by function flt and returns a new one.\n"
    "flt receives point coordinates x and y as well as the point type"
    " and have to return a list contains 2 numbers, replacing x and y.\n"
    "If failed, an error message will store in err.\n"
    "example:\n"
    "import SubFX_YutilsPy as Yutils\n"
    "shape = Yutils.Shape\n"
    "\n"
    "def flt(x, y, pointType):\n"
    "    if pointType == \"m\":\n"
    "        return [x + 5, y + 2]\n"
    "\n"
    "    return [x + 2, y + 5]\n"
    "\n"
    "test = \"m 1 2 l 3 4 5 6 b 7 8 9 10 11 12 13 14 c\"\n"
    "print(shape.filter(test, flt))\n");

    shape.def("flatten", &Shape::flatten,
    "flattened_shape = flatten(shape)\n"
    "Converts all 3rd order bezier curves in shape shape to lines,\n"
    "creating a new shape.\n");

    shape.def("move", &Shape::move,
    "new_shape = move(shape, x, y)\n"
    "Shifts points of shape shape horizontally by x and vertically by y,\n"
    "creating a new shape.\n");

    shape.def("to_pixels", &Shape::to_pixels,
    "pixels = to_pixels(shape)\n"
    "Renders shape shape and returns pixels.\n"
    "pixels is a list of dictionaries, each one with following fields:\n"
    "x: horizontal position\n"
    "y: vertical position\n"
    "alpha: opacity\n");

    /* in ass.hpp */
    auto ass(m.def_submodule("Ass"));

    ass.def("stringToMs", &Ass::stringToMs,
    "ms_ass = stringToMs(ass_ms)\n"
    "Converts time to numeric.\n"
    "ass_ms is a string in ASS format H:MM:SS.XX "
    "(H=Hours, M=Minutes, S=Seconds, X=Milliseconds*10).\n"
    "ms_ass is milliseconds as number.\n");

    ass.def("msToString", &Ass::msToString,
    "ass_ms = msToString(ms_ass)\n"
    "Converts time to ASS presentation.\n"
    "ass_ms is string in ASS format H:MM:SS.XX "
    "(H=Hours, M=Minutes, S=Seconds, X=Milliseconds*10)\n"
    "ms_ass is milliseconds as number.\n");

    ass.def("stringToColorAlpha", &Ass::stringToColorAlpha,
    "tuple(r, g, b, a) = stringToColorAlpha(input)\n"
    "Converts color, alpha or color+alpha to numeric\n"
    "input is a string as ASS color (&HBBGGRR&), "
    "alpha (&HAA&) or both (&HAABBGGRR)\n");

    ass.def("colorAlphaToString", &Ass::colorAlphaToString,
    "output = colorAlphaToString(input)\n"
    "input = [r, g, b] or [a]\n"
    "Converts color or alpha to ASS presentation.\n"
    "input is a list must contain 1 or 3 elements.\n"
    "If input has 3 elements, "
    "output will be an ASS color presentation(&HBBGGRR&),\n"
    "else output will be an ASS alpha presentation(&HAA&).\n");

    /* in asscommon.hpp */
    py::class_<AssMeta, std::shared_ptr<AssMeta>>(m, "AssMeta")
    .def(py::init())
    .def_readwrite("wrap_style", &AssMeta::wrap_style)
    .def_readwrite("scaled_border_and_shadow", &AssMeta::scaled_border_and_shadow)
    .def_readwrite("play_res_x", &AssMeta::play_res_x)
    .def_readwrite("play_res_y", &AssMeta::play_res_y)
    .def_readwrite("colorMatrix", &AssMeta::colorMatrix);

    py::class_<AssStyle, std::shared_ptr<AssStyle>>(m, "AssStyle")
    .def(py::init())
    .def_readwrite("fontname", &AssStyle::fontname)
    .def_readwrite("fontsize", &AssStyle::fontsize)
    .def_readwrite("bold", &AssStyle::bold)
    .def_readwrite("italic", &AssStyle::italic)
    .def_readwrite("underline", &AssStyle::underline)
    .def_readwrite("strikeout", &AssStyle::strikeout)
    .def_readwrite("scale_x", &AssStyle::scale_x)
    .def_readwrite("scale_y", &AssStyle::scale_y)
    .def_readwrite("spaceing", &AssStyle::spaceing)
    .def_readwrite("angle", &AssStyle::angle)
    .def_readwrite("bolder_style", &AssStyle::bolder_style)
    .def_readwrite("outline", &AssStyle::outline)
    .def_readwrite("shadow", &AssStyle::shadow)
    .def_readwrite("alignment", &AssStyle::alignment)
    .def_readwrite("margin_l", &AssStyle::margin_l)
    .def_readwrite("margin_r", &AssStyle::margin_r)
    .def_readwrite("margin_v", &AssStyle::margin_v)
    .def_readwrite("encoding", &AssStyle::encoding)
    .def_readwrite("color1", &AssStyle::color1)
    .def_readwrite("alpha1", &AssStyle::alpha1)
    .def_readwrite("color2", &AssStyle::color2)
    .def_readwrite("alpha2", &AssStyle::alpha2)
    .def_readwrite("color3", &AssStyle::color3)
    .def_readwrite("alpha3", &AssStyle::alpha3)
    .def_readwrite("color4", &AssStyle::color4)
    .def_readwrite("alpha4", &AssStyle::alpha4);

    py::class_<AssTextChunked, std::shared_ptr<AssTextChunked>>(m, "AssTextChunked")
    .def(py::init())
    .def_readwrite("tags", &AssTextChunked::tags)
    .def_readwrite("text", &AssTextChunked::text);

    py::class_<AssSyl, std::shared_ptr<AssSyl>>(m, "AssSyl")
    .def(py::init())
    .def_readwrite("start_time", &AssSyl::start_time)
    .def_readwrite("end_time", &AssSyl::end_time)
    .def_readwrite("text", &AssSyl::text)
    .def_readwrite("i", &AssSyl::i)
    .def_readwrite("duration", &AssSyl::duration)
    .def_readwrite("mid_time", &AssSyl::mid_time)
    .def_readwrite("width", &AssSyl::width)
    .def_readwrite("height", &AssSyl::height)
    .def_readwrite("ascent", &AssSyl::ascent)
    .def_readwrite("descent", &AssSyl::descent)
    .def_readwrite("internal_leading", &AssSyl::internal_leading)
    .def_readwrite("external_leading", &AssSyl::external_leading)
    .def_readwrite("left", &AssSyl::left)
    .def_readwrite("center", &AssSyl::center)
    .def_readwrite("right", &AssSyl::right)
    .def_readwrite("x", &AssSyl::x)
    .def_readwrite("top", &AssSyl::top)
    .def_readwrite("middle", &AssSyl::middle)
    .def_readwrite("bottom", &AssSyl::bottom)
    .def_readwrite("y", &AssSyl::y)
    .def_readwrite("tags", &AssSyl::tags)
    .def_readwrite("prespace", &AssSyl::prespace)
    .def_readwrite("postspace", &AssSyl::postspace);

    py::class_<AssWord, std::shared_ptr<AssWord>>(m, "AssWord")
    .def(py::init())
    .def_readwrite("start_time", &AssWord::start_time)
    .def_readwrite("end_time", &AssWord::end_time)
    .def_readwrite("text", &AssWord::text)
    .def_readwrite("i", &AssWord::i)
    .def_readwrite("duration", &AssWord::duration)
    .def_readwrite("mid_time", &AssWord::mid_time)
    .def_readwrite("width", &AssWord::width)
    .def_readwrite("height", &AssWord::height)
    .def_readwrite("ascent", &AssWord::ascent)
    .def_readwrite("descent", &AssWord::descent)
    .def_readwrite("internal_leading", &AssWord::internal_leading)
    .def_readwrite("external_leading", &AssWord::external_leading)
    .def_readwrite("left", &AssWord::left)
    .def_readwrite("center", &AssWord::center)
    .def_readwrite("right", &AssWord::right)
    .def_readwrite("x", &AssWord::x)
    .def_readwrite("top", &AssWord::top)
    .def_readwrite("middle", &AssWord::middle)
    .def_readwrite("bottom", &AssWord::bottom)
    .def_readwrite("y", &AssWord::y)
    .def_readwrite("prespace", &AssWord::prespace)
    .def_readwrite("postspace", &AssWord::postspace);

    py::class_<AssChar, std::shared_ptr<AssChar>>(m, "AssChar")
    .def(py::init())
    .def_readwrite("start_time", &AssChar::start_time)
    .def_readwrite("end_time", &AssChar::end_time)
    .def_readwrite("text", &AssChar::text)
    .def_readwrite("i", &AssChar::i)
    .def_readwrite("duration", &AssChar::duration)
    .def_readwrite("mid_time", &AssChar::mid_time)
    .def_readwrite("width", &AssChar::width)
    .def_readwrite("height", &AssChar::height)
    .def_readwrite("ascent", &AssChar::ascent)
    .def_readwrite("descent", &AssChar::descent)
    .def_readwrite("internal_leading", &AssChar::internal_leading)
    .def_readwrite("external_leading", &AssChar::external_leading)
    .def_readwrite("left", &AssChar::left)
    .def_readwrite("center", &AssChar::center)
    .def_readwrite("right", &AssChar::right)
    .def_readwrite("x", &AssChar::x)
    .def_readwrite("top", &AssChar::top)
    .def_readwrite("middle", &AssChar::middle)
    .def_readwrite("bottom", &AssChar::bottom)
    .def_readwrite("y", &AssChar::y)
    .def_readwrite("syl_i", &AssChar::syl_i)
    .def_readwrite("word_i", &AssChar::word_i);

    py::class_<AssDialog, std::shared_ptr<AssDialog>>(m, "AssDialog")
    .def(py::init())
    .def_readwrite("start_time", &AssDialog::start_time)
    .def_readwrite("end_time", &AssDialog::end_time)
    .def_readwrite("text", &AssDialog::text)
    .def_readwrite("i", &AssDialog::i)
    .def_readwrite("duration", &AssDialog::duration)
    .def_readwrite("mid_time", &AssDialog::mid_time)
    .def_readwrite("width", &AssDialog::width)
    .def_readwrite("height", &AssDialog::height)
    .def_readwrite("ascent", &AssDialog::ascent)
    .def_readwrite("descent", &AssDialog::descent)
    .def_readwrite("internal_leading", &AssDialog::internal_leading)
    .def_readwrite("external_leading", &AssDialog::external_leading)
    .def_readwrite("left", &AssDialog::left)
    .def_readwrite("center", &AssDialog::center)
    .def_readwrite("right", &AssDialog::right)
    .def_readwrite("x", &AssDialog::x)
    .def_readwrite("top", &AssDialog::top)
    .def_readwrite("middle", &AssDialog::middle)
    .def_readwrite("bottom", &AssDialog::bottom)
    .def_readwrite("y", &AssDialog::y)
    .def_readwrite("styleref", &AssDialog::styleref)
    .def_readwrite("text_stripped", &AssDialog::text_stripped)
    .def_readwrite("comment", &AssDialog::comment)
    .def_readwrite("layer", &AssDialog::layer)
    .def_readwrite("style", &AssDialog::style)
    .def_readwrite("actor", &AssDialog::actor)
    .def_readwrite("margin_l", &AssDialog::margin_l)
    .def_readwrite("margin_r", &AssDialog::margin_r)
    .def_readwrite("margin_v", &AssDialog::margin_v)
    .def_readwrite("effect", &AssDialog::effect)
    .def_readwrite("leadin", &AssDialog::leadin)
    .def_readwrite("leadout", &AssDialog::leadout)
    .def_readwrite("textChunked", &AssDialog::textChunked)
    .def_readwrite("syls", &AssDialog::syls)
    .def_readwrite("words", &AssDialog::words)
    .def_readwrite("chars", &AssDialog::chars);

    /* in assparser.hpp */
    py::class_<AssParser, std::shared_ptr<AssParser>>(m, "AssParser")

    .def_static("create", &AssParser::create,
        py::arg("fileName"),
        py::arg("warningOut") = "",
    "Create a AssParser object.\n"
    "input string is ass's file name.\n")

    .def("meta", &AssParser::meta,
    "Returns AssMeta object with the following fields\n"
    "wrap_style: text line wrapping mode as number\n"
    "scaled_border_and_shadow: borders and shadows should be implicated "
    "in script-to-frame scale?\n"
    "play_res_x: script horizontal resolution\n"
    "play_res_y: script vertical resolution\n")

    .def("styles", &AssParser::styles,
    "Returns ASS styles as dictionary. Dictionary keys are style names, values "
    "are AssStyle object with the following fields:\n"
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

    .def("dialogs", &AssParser::dialogs,
    "Returns AssDialog objects as list with the following fields:\n"
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

    .def("extendDialogs", &AssParser::extendDialogs,
    "Add the following fields to AssDialog object.\n"
    "After call this function, "
    "you can get extended dialogs by calling dialogs().\n"
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
    "    tags: content between brackets {} in front of text\n"
    "    text: text after brackets{}\n"
    "syls: raw text in sylable chunks with following fields:\n"
    "    i: sylable index\n"
    "    start_time: sylable start time in milliseconds\n"
    "    mid_time: sylable mid time in milliseconds\n"
    "    end_time: sylable end time in milliseconds\n"
    "    duration: sylable duration in milliseconds\n"
    "    tags: sylable tags beside her duration definition\n"
    "    text: sylable text\n"
    "    prespace: sylable spaces number in front\n"
    "    postspace: sylable spaces number behind\n"
    "    width: sylable width\n"
    "    height: sylable height\n"
    "    ascent: sylable font ascent\n"
    "    descent: sylable font descent\n"
    "    internal_leading: sylable font internal leading\n"
    "    external_leading: sylable font external leading\n"
    "    left: sylable left position\n"
    "    center: sylable center position\n"
    "    right: sylable right position\n"
    "    x: sylable horizontal position by alignment\n"
    "    top: sylable top position\n"
    "    middle: sylable middle position\n"
    "    bottom: sylable bottom position\n"
    "    y: sylable vertical position by alignment\n"
    "words: raw text in word chunks with following fields:\n"
    "    i: word index\n"
    "    start_time: word start time in milliseconds\n"
    "    mid_time: word mid time in milliseconds\n"
    "    end_time: word end time in milliseconds\n"
    "    duration: word duration in milliseconds\n"
    "    text: word text\n"
    "    prespace: word spaces number in front\n"
    "    postspace: word spaces number behind\n"
    "    width: word width\n"
    "    height: word height\n"
    "    ascent: word font ascent\n"
    "    descent: word font descent\n"
    "    internal_leading: word font internal leading\n"
    "    external_leading: word font external leading\n"
    "    left: word left position\n"
    "    center: word center position\n"
    "    right: word right position\n"
    "    x: word horizontal position by alignment\n"
    "    top: word top position\n"
    "    middle: word middle position\n"
    "    bottom: word bottom position\n"
    "    y: word vertical position by alignment\n"
    "chars: raw text in character chunks with followings fields:\n"
    "    i: character index\n"
    "    start_time: character start time in milliseconds\n"
    "    mid_time: character mid time in milliseconds\n"
    "    end_time: character end time in milliseconds\n"
    "    duration: character duration in milliseconds\n"
    "    text: character text\n"
    "    syl_i: index of sylable which contains the character\n"
    "    word_i: index of word which contains the character\n"
    "    width: character width\n"
    "    height: character height\n"
    "    ascent: character font ascent\n"
    "    descent: character font descent\n"
    "    internal_leading: character font internal leading\n"
    "    external_leading: character font external leading\n"
    "    left: character left position\n"
    "    center: character center position\n"
    "    right: character right position\n"
    "    x: character horizontal position by alignment\n"
    "    top: character top position\n"
    "    middle: character middle position\n"
    "    bottom: character bottom position\n"
    "    y: character vertical position by alignment\n"
    "leadin: dialog pretime / duration from last dialog to this one "
    "(in case there's none, it's 1000.1)\n"
    "leadout: dialog posttime / duration from this dialog to next one "
    "(in case there's none, it's 1000.1)\n");

    /* in asswriter.hpp */
    auto assWriter(m.def_submodule("AssWriter"));

    assWriter.def("write",
                  py::overload_cast<const char *,
                  std::shared_ptr<AssParser> &>(&AssWriter::write),
                  "write(fileName, assParser)\n"
    "Write all contents to file(ass) by providing AssParser, "
    "fileName as its name.");

    assWriter.def("write", py::overload_cast<const char *,
                  const char *,
                  std::vector<std::string> &>(&AssWriter::write),
    "write(fileName, assHeader, assBuf)\n"
    "Write all contents to file(ass) by providing assHeader and assBuf\n"
    "fileName as its name.\n"
    "assHeader as its name, too.\n"
    "If assHeader is an empty string, "
    "the file will be opened in append mode.\n"
    "If not, the file will be opened in write mode.\n"
    "assBuf is a list of strings.\n"
    "If assHeader is not empty, it will write assHeader first, "
    "then write contents in assBuf to file, "
    "else just write assBuf to file.");

    assWriter.def("write", py::overload_cast<const char *,
                  std::shared_ptr<AssMeta> &,
                  std::map<std::string,
                  std::shared_ptr<AssStyle>> &,
                  std::vector<std::string> &>(&AssWriter::write),
    "write(meta, styles, assBuf)"
    "Write all contents by providing meta, styles and assbuf.");

    /* in fonthandle.hpp */
    py::class_<FontHandle, std::shared_ptr<FontHandle>>(m, "FontHandle")
    .def_static("create", &FontHandle::create,
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
    "height: font maximal height\n"
    "If failed, it will return an empty dictionary.\n")

    .def("text_extents", &FontHandle::text_extents,
    "extents = text_extents(text)\n"
    "Returns extents of text with given font as "
    "dictionary with followings fields:\n"
    "width: text width\n"
    "height: text height\n"
    "If failed, it will return an empty dictionary.\n")

    .def("text_to_shape", &FontHandle::text_to_shape,
    "shape = text_to_shape(text)\n"
    "Converts text with given font to an ASS shape.\n"
    "If failed, it will return an empty string.\n");
}
