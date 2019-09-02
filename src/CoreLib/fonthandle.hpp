#ifndef FONTHANDLE_HPP
#define FONTHANDLE_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <map>

#ifdef _WIN32
#include "windows.h"
#else
#include "pango/pangocairo.h"
#endif

#include "math.hpp"

#define FONT_PRECISION 64

using namespace std;

class FontHandle : public CoreMath
{
public:
    FontHandle(string &family, bool bold, bool italic, bool underline,
               bool strikeout, int size,
               int xscale = 1, int yscale = 1, int hspace = 0);
    
    ~FontHandle();

    map<string, double> metrics();

    map<string, double> text_extents(string &text);

    string text_to_shape(string &text);

private:
#ifdef _WIN32
    HDC dc;

    HFONT font;
    
    HGDIOBJ old_font;

    int hspace;

    int upscale;
#else
    cairo_surface_t *surface;

    cairo_t *context;

    PangoLayout *layout;

    double fonthack_scale;
#endif

    int xscale;

    int yscale;

    double downscale;
};

#endif // FONTHANDLE_HPP
