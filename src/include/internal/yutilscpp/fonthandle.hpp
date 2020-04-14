#pragma once

#include <string>
#include <map>

#ifdef _WIN32
#include "windows.h"
#else
#include "pango/pangocairo.h"
#endif

#include "math.hpp"
#include "../basecommon.h"

#define FONT_PRECISION 64

namespace Yutils
{
class SYMBOL_SHOW FontHandle : protected Math
{
public:

    static std::shared_ptr<FontHandle>
    create(std::string &family,
           bool bold,
           bool italic,
           bool underline,
           bool strikeout,
           int size,
           double xscale = 1.,
           double yscale = 1.,
           double hspace = 0.) THROW;

    ~FontHandle();

    std::map<std::string, double> metrics() NOTHROW;

    std::map<std::string, double>
    text_extents(std::string &text) NOTHROW;

    std::string text_to_shape(std::string &text) THROW;

protected:

    FontHandle(double xscale, double yscale, double hspace) :
    Math(),
    #ifdef _WIN32
        dc(nullptr),
        font(nullptr),
        old_font(nullptr),
        hspace(hspace),
        upscale(FONT_PRECISION),
    #else
        surface(nullptr),
        context(nullptr),
        layout(nullptr),
    #endif
        xscale(xscale),
        yscale(yscale)
    {}

private:

    FontHandle() {}

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
