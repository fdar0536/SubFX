#include "assparserpy.hpp"
#include "pybind11/pybind11.h"

AssParserPy::AssParserPy(string &fileName) :
    dialogParsed(false)
{
    parser = make_shared<AssParser>(fileName);
    if (parser == nullptr)
    {
        throw invalid_argument("Failed to create ass parser.");
    }

    initData();
}

py::dict AssParserPy::meta() const
{
    return metaData;
}

py::dict AssParserPy::styles() const
{
    return styleData;
}

py::list AssParserPy::dialogs() const
{
    return dialogData;
}

void AssParserPy::upgradeDialogs()
{
    if (dialogParsed)
    {
        return;
    }

    parser->upgradeDialogs();
    getDialogsData();
    dialogParsed = true;
}

shared_ptr<AssMeta> AssParserPy::getMetaPtr() const
{
    return parser->meta();
}

map<string, shared_ptr<AssStyle>> AssParserPy::getStyleData() const
{
    return parser->styles();
}

vector<shared_ptr<AssDialog>> AssParserPy::getDialogs() const
{
    return parser->dialogs();
}

// private member function
void AssParserPy::initData()
{
    setUpMeta();
    setUpStyles();
    setUpDialogs();
}

void AssParserPy::setUpMeta()
{
    auto meta(parser->meta());
    py::dict newData;

    newData["wrap_style"] = meta->wrap_style;
    newData["scaled_border_and_shadow"] = meta->scaled_border_and_shadow;
    newData["play_res_x"] = meta->play_res_x;
    newData["play_res_y"] = meta->play_res_y;

    metaData = newData;
}

void AssParserPy::setUpStyles()
{
    py::dict newStyles;
    auto assStyles(parser->styles());
    for (auto i = assStyles.begin(); i != assStyles.end(); ++i)
    {
        auto assStyle(i->second);
        py::dict style;
        style["fontname"] = assStyle->fontname;
        style["fontsize"] = assStyle->fontsize;
        style["bold"] = assStyle->bold;
        style["italic"] = assStyle->italic;
        style["underline"] = assStyle->underline;
        style["strikeout"] = assStyle->strikeout;
        style["scale_x"] = assStyle->scale_x;
        style["scale_y"] = assStyle->scale_y;
        style["spaceing"] = assStyle->spaceing;
        style["angle"] = assStyle->angle;
        style["bolder_style"] = assStyle->bolder_style;
        style["outline"] = assStyle->outline;
        style["shadow"] = assStyle->shadow;
        style["alignment"] = assStyle->alignment;
        style["margin_l"] = assStyle->margin_l;
        style["margin_r"] = assStyle->margin_r;
        style["margin_v"] = assStyle->margin_v;
        style["encoding"] = assStyle->encoding;
        style["color1"] = assStyle->color1;
        style["alpha1"] = assStyle->alpha1;
        style["color2"] = assStyle->color2;
        style["alpha2"] = assStyle->alpha2;
        style["color3"] = assStyle->color3;
        style["alpha3"] = assStyle->alpha3;
        style["color4"] = assStyle->color4;
        style["alpha4"] = assStyle->alpha4;

        newStyles[i->first.c_str()] = style;
    }

    styleData = newStyles;
}

void AssParserPy::setUpDialogs()
{
    auto assDialogs(parser->dialogs());
    py::list newDialogs;

    for (auto i = assDialogs.begin(); i != assDialogs.end(); ++i)
    {
        auto assDialog(*i);
        py::dict dialog;
        dialog["comment"] = assDialog->comment;
        dialog["layer"] = assDialog->layer;
        dialog["start_time"] = assDialog->start_time;
        dialog["end_time"] = assDialog->end_time;
        dialog["style"] = assDialog->style;
        dialog["actor"] = assDialog->actor;
        dialog["margin_l"] = assDialog->margin_l;
        dialog["margin_r"] = assDialog->margin_r;
        dialog["margin_v"] = assDialog->margin_v;
        dialog["effect"] = assDialog->effect;
        dialog["text"] = assDialog->text;
        newDialogs.append(dialog);
    }

    dialogData = newDialogs;
}

void AssParserPy::getDialogsData()
{
    auto assDialogs(parser->dialogs());
    for (size_t dialogIndex = 0; dialogIndex < py::len(dialogData); ++dialogIndex)
    {
        // fill dialog first
        py::dict dialog(dialogData[dialogIndex]);
        auto assDialog(assDialogs.at(dialogIndex));

        dialog["i"] = assDialog->i;
        dialog["duration"] = assDialog->duration;
        dialog["mid_time"] = assDialog->mid_time;
        
        auto assStyle(assDialog->styleref);
        py::dict style;
        style["fontname"] = assStyle->fontname;
        style["fontsize"] = assStyle->fontsize;
        style["bold"] = assStyle->bold;
        style["italic"] = assStyle->italic;
        style["underline"] = assStyle->underline;
        style["strikeout"] = assStyle->strikeout;
        style["scale_x"] = assStyle->scale_x;
        style["scale_y"] = assStyle->scale_y;
        style["spaceing"] = assStyle->spaceing;
        style["angle"] = assStyle->angle;
        style["bolder_style"] = assStyle->bolder_style;
        style["outline"] = assStyle->outline;
        style["shadow"] = assStyle->shadow;
        style["alignment"] = assStyle->alignment;
        style["margin_l"] = assStyle->margin_l;
        style["margin_r"] = assStyle->margin_r;
        style["margin_v"] = assStyle->margin_v;
        style["encoding"] = assStyle->encoding;
        style["color1"] = assStyle->color1;
        style["alpha1"] = assStyle->alpha1;
        style["color2"] = assStyle->color2;
        style["alpha2"] = assStyle->alpha2;
        style["color3"] = assStyle->color3;
        style["alpha3"] = assStyle->alpha3;
        style["color4"] = assStyle->color4;
        style["alpha4"] = assStyle->alpha4;
        
        dialog["styleref"] = style;
        dialog["text_stripped"] = assDialog->text_stripped;
        dialog["width"] = assDialog->width;
        dialog["height"] = assDialog->height;
        dialog["ascent"] = assDialog->ascent;
        dialog["descent"] = assDialog->descent;
        dialog["internal_leading"] = assDialog->internal_leading;
        dialog["external_leading"] = assDialog->external_leading;
        dialog["left"] = assDialog->left;
        dialog["center"] = assDialog->center;
        dialog["right"] = assDialog->right;
        dialog["x"] = assDialog->x;
        dialog["top"] = assDialog->top;
        dialog["middle"] = assDialog->middle;
        dialog["bottom"] = assDialog->bottom;
        dialog["y"] = assDialog->y;

        auto assTextChunkeds(assDialog->textChunked);
        py::list chunkList;
        for (size_t i = 0; i < assTextChunkeds.size(); ++i)
        {
            auto assTextChunked(assTextChunkeds.at(i));
            py::dict textChunked;
            textChunked["tags"] = assTextChunked->tags;
            textChunked["text"] = assTextChunked->text;
            chunkList.append(textChunked);
        } // end for i

        dialog["text_chunked"] = chunkList;
        dialog["leadin"] = assDialog->leadin;
        dialog["leadout"] = assDialog->leadout;

        py::list sylList;
        auto assSyls(assDialog->syls);
        for (size_t sylIndex = 0; sylIndex < assSyls.size(); ++sylIndex)
        {
            auto assSyl(assSyls.at(sylIndex));
            py::dict syl;
            syl["i"] = assSyl->i;
            syl["start_time"] = assSyl->start_time;
            syl["mid_time"] = assSyl->mid_time;
            syl["end_time"] = assSyl->end_time;
            syl["duration"] = assSyl->duration;
            syl["tags"] = assSyl->tags;
            syl["text"] = assSyl->text;
            syl["prespace"] = assSyl->prespace;
            syl["postspace"] = assSyl->postspace;
            syl["width"] = assSyl->width;
            syl["height"] = assSyl->height;
            syl["ascent"] = assSyl->ascent;
            syl["descent"] = assSyl->descent;
            syl["internal_leading"] = assSyl->internal_leading;
            syl["external_leading"] = assSyl->external_leading;
            syl["left"] = assSyl->left;
            syl["center"] = assSyl->center;
            syl["right"] = assSyl->right;
            syl["x"] = assSyl->x;
            syl["top"] = assSyl->top;
            syl["middle"] = assSyl->middle;
            syl["bottom"] = assSyl->bottom;
            syl["y"] = assSyl->y;

            sylList.append(syl);
        } // end for sylIndex

        dialog["syls"] = sylList;

        auto assWords(assDialog->words);
        py::list wordList;
        for (size_t wordIndex = 0; wordIndex < assWords.size(); ++wordIndex)
        {
            auto assWord(assWords.at(wordIndex));
            py::dict word;

            word["i"] = assWord->i;
            word["start_time"] = assWord->start_time;
            word["mid_time"] = assWord->mid_time;
            word["end_time"] = assWord->end_time;
            word["duration"] = assWord->duration;
            word["text"] = assWord->text;
            word["prespace"] = assWord->prespace;
            word["postspace"] = assWord->postspace;
            word["width"] = assWord->width;
            word["height"] = assWord->height;
            word["ascent"] = assWord->ascent;
            word["descent"] = assWord->descent;
            word["internal_leading"] = assWord->internal_leading;
            word["external_leading"] = assWord->external_leading;
            word["left"] = assWord->left;
            word["center"] = assWord->center;
            word["right"] = assWord->right;
            word["x"] = assWord->x;
            word["top"] = assWord->top;
            word["middle"] = assWord->middle;
            word["bottom"] = assWord->bottom;
            word["y"] = assWord->y;

            wordList.append(word);
        } // end for word index

        dialog["words"] = wordList;

        auto assChars(assDialog->chars);
        py::list charList;
        for (size_t charIndex = 0; charIndex < assChars.size(); ++charIndex)
        {
            auto assChar(assChars.at(charIndex));
            py::dict chara;

            chara["i"] = assChar->i;
            chara["start_time"] = assChar->start_time;
            chara["mid_time"] = assChar->mid_time;
            chara["end_time"] = assChar->end_time;
            chara["duration"] = assChar->duration;
            chara["text"] = assChar->text;
            chara["syl_i"] = assChar->syl_i;
            chara["word_i"] = assChar->word_i;
            chara["width"] = assChar->width;
            chara["height"] = assChar->height;
            chara["ascent"] = assChar->ascent;
            chara["descent"] = assChar->descent;
            chara["internal_leading"] = assChar->internal_leading;
            chara["external_leading"] = assChar->external_leading;
            chara["left"] = assChar->left;
            chara["center"] = assChar->center;
            chara["right"] = assChar->right;
            chara["x"] = assChar->x;
            chara["top"] = assChar->top;
            chara["middle"] = assChar->middle;
            chara["bottom"] = assChar->bottom;
            chara["y"] = assChar->y;

            charList.append(chara);
        } // end for charIndex

        dialog["chars"] = charList;
    } // end for dialogIndex
}
