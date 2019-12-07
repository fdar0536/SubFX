#ifndef FONTHANDLE_HPP
#define FONTHANDLE_HPP

#include <string>
#include <map>

#ifdef _WIN32
#include "windows.h"
#else
#include "pango/pangocairo.h"
#endif

#include "math.hpp"
#include "../common/basecommon.hpp"

#define FONT_PRECISION 64

namespace Yutils
{

class SYMBOL_SHOW FontHandle : protected Math
{
public:

    // constructor may throw
    // 1. invalid_argument
    // 2. runtime_error
    FontHandle(std::string &family,
               bool bold,
               bool italic,
               bool underline,
               bool strikeout,
               int size,
               double xscale = 1,
               double yscale = 1,
               double hspace = 0);

    ~FontHandle();

    // metrics may throw runtime_error
    std::map<std::string, double> metrics();

    // text_extents may throw runtime_error
    std::map<std::string, double> text_extents(std::string &text);

    // text_to_shape may throw
    // 1. invalid_argument
    // 2. runtome_error
    std::string text_to_shape(std::string &text);

private:
#ifdef _WIN32
    HDC dc;

    HFONT font;

    HGDIOBJ old_font;

    double hspace;

    int upscale;
#else
    cairo_surface_t *surface;

    cairo_t *context;

    PangoLayout *layout;

    double fonthack_scale;
#endif

    double xscale;

    double yscale;

    double downscale;
};

}

#endif // FONTHANDLE_HPP
