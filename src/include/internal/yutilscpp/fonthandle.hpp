#pragma once

#include <string>
#include <map>
#include <memory>

#ifdef _WIN32
#include "windows.h"
#else
#include "pango/pangocairo.h"
#endif

#include "../basecommon.h"

#define FONT_PRECISION 64

namespace PROJ_NAMESPACE
{

namespace Yutils
{
class SYMBOL_SHOW FontHandle
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

#ifndef _WIN32
    FontHandle(double xscale, double yscale) NOTHROW :
        surface(nullptr),
        context(nullptr),
        layout(nullptr),
#else
    FontHandle(double xscale, double yscale, double hspace) NOTHROW :
        dc(nullptr),
        font(nullptr),
        old_font(nullptr),
        hspace(hspace),
        upscale(FONT_PRECISION),
#endif
        xscale(xscale),
        yscale(yscale)
    {}

private:

    FontHandle() = delete;

    FontHandle(const FontHandle &) = delete;

    FontHandle& operator=(const FontHandle &other) = delete;

    FontHandle& operator=(FontHandle &&other) = delete;

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
} // end namespace Yutils

} // end PROJ_NAMESPACE
