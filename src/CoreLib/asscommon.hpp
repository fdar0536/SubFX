#ifndef ASSCOMMON_HPP
#define ASSCOMMON_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <vector>
#include <memory>

#include <cstdint>

#define ASS_TRUE -1
#define ASS_FALSE 0

using namespace std;

// reference: https://github.com/weizhenye/ASS/wiki/ASS-%E5%AD%97%E5%B9%95%E6%A0%BC%E5%BC%8F%E8%A7%84%E8%8C%83

class AssMeta
{
public:
    AssMeta() :
    wrap_style(0),
    scaled_border_and_shadow(true),
    play_res_x(640),
    play_res_y(360)
    {}

    uint8_t wrap_style;
    bool scaled_border_and_shadow;
    uint16_t play_res_x;
    uint16_t play_res_y;
};

class AssStyle
{
public:
    AssStyle() :
    fontname("Arial"),
    fontsize(20),
    bold(ASS_FALSE),
    italic(ASS_FALSE),
    underline(ASS_FALSE),
    strikeout(ASS_FALSE),
    scale_x(100.f),
    scale_y(100.f),
    spaceing(0.f),
    angle(0.f),
    bolder_style(1),
    outline(0.f),
    shadow(0.f),
    alignment(2),
    margin_l(0.f),
    margin_r(0.f),
    margin_v(20.f),
    encoding(0),
    color1("&HFFFFFF&"),
    alpha1("&H00&"),
    color2("&H000000&"),
    alpha2("&H00&"),
    color3("&HFFFFFF&"),
    alpha3("&H00&"),
    color4("&HFFFFFF&"),
    alpha4("&H00&")
    {}

    string fontname;
    int fontsize;
    int8_t bold;
    int8_t italic;
    int8_t underline;
    int8_t strikeout;
    double scale_x;
    double scale_y;
    double spaceing;
    double angle;
    uint8_t bolder_style;
    double outline;
    double shadow;
    uint8_t alignment;
    double margin_l;
    double margin_r;
    double margin_v;
    int encoding;
    string color1;
    string alpha1;
    string color2;
    string alpha2;
    string color3;
    string alpha3;
    string color4;
    string alpha4;
};

class AssSymbol
{
public:
    AssSymbol() :
    start_time(0),
    end_time(0),
    text(""),
    i(0),
    duration(0),
    mid_time(0),
    width(0.f),
    height(0.f),
    ascent(0.f),
    descent(0.f),
    internal_leading(0.f),
    external_leading(0.f),
    left(0.f),
    center(0.f),
    right(0.f),
    x(0.f),
    top(0.f),
    middle(0.f),
    bottom(0.f),
    y(0.f)
    {}

    uint64_t start_time;
    uint64_t end_time;
    string text;
    uint32_t i;
    uint64_t duration;
    uint64_t mid_time;
    double width;
    double height;
    double ascent;
    double descent;
    double internal_leading;
    double external_leading;
    double left;
    double center;
    double right;
    double x;
    double top;
    double middle;
    double bottom;
    double y;
};

class AssTextChunked
{
public:
    AssTextChunked() :
    tags(""),
    text("")
    {}

    string tags;
    string text;
};

class AssSyl : public AssSymbol
{
public:
    AssSyl() :
    tags(""),
    prespace(0),
    postspace(0)
    {
        AssSymbol();
    }

    string tags;
    uint32_t prespace;
    uint32_t postspace;
};

class AssWord : public AssSymbol
{
public:
    AssWord() :
    prespace(0),
    postspace(0)
    {
        AssSymbol();
    }

    uint32_t prespace;
    uint32_t postspace;
};

class AssChar : public AssSymbol
{
public:
    AssChar() :
    syl_i(0),
    word_i(0)
    {
        AssSymbol();
    }

    int syl_i;
    int word_i;
};

class AssDialog : public AssSymbol
{
public:
    AssDialog() :
    styleref(make_shared<AssStyle>()),
    text_stripped(""),
    comment(false),
    layer(0),
    style(""),
    actor(""),
    margin_l(0.f),
    margin_r(0.f),
    margin_v(0.f),
    effect(""),
    leadin(0.f),
    leadout(0.f),
    textChunked(vector<shared_ptr<AssTextChunked>>()),
    syls(vector<shared_ptr<AssSyl>>()),
    words(vector<shared_ptr<AssWord>>()),
    chars(vector<shared_ptr<AssChar>>())
    {
        AssSymbol();
    }

    shared_ptr<AssStyle> styleref;
    string text_stripped;
    bool comment;
    uint32_t layer;
    string style;
    string actor;
    double margin_l;
    double margin_r;
    double margin_v;
    string effect;
    double leadin;
    double leadout;
    vector<shared_ptr<AssTextChunked>> textChunked;
    vector<shared_ptr<AssSyl>> syls;
    vector<shared_ptr<AssWord>> words;
    vector<shared_ptr<AssChar>> chars;
};

#endif // ASSCOMMON_HPP
