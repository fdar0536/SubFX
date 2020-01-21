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

    static std::pair<std::shared_ptr<FontHandle>, const char *>
    create(std::string &family,
           bool bold,
           bool italic,
           bool underline,
           bool strikeout,
           int size,
           double xscale = 1,
           double yscale = 1,
           double hspace = 0);

    ~FontHandle();

    std::pair<std::map<std::string, double>, const char *> metrics();

    std::pair<std::map<std::string, double>, const char *>
    text_extents(std::string &text);

    std::pair<std::string, const char *> text_to_shape(std::string &text);

protected:

    FontHandle(double xscale, double yscale) :
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

#endif // FONTHANDLE_HPP
