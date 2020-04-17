#include <iostream>

#include "assdumper.hpp"

#define WRITE_ASS_SYMBOL(ptr) \
    writeData("start_time: ", ptr->start_time); \
    writeData("end_time: ", ptr->end_time); \
    writeString("text: ", ptr->text); \
    writeData("i: ", ptr->i); \
    writeData("duration: ", ptr->duration); \
    writeData("mid_time: ", ptr->mid_time); \
    writeData("width: ", ptr->width); \
    writeData("height: ", ptr->height); \
    writeData("ascent: ", ptr->ascent); \
    writeData("descent: ", ptr->descent); \
    writeData("internal_leading: ", ptr->internal_leading); \
    writeData("external_leading: ", ptr->external_leading); \
    writeData("left: ", ptr->left); \
    writeData("center: ", ptr->center); \
    writeData("right: ", ptr->right); \
    writeData("x: ", ptr->x); \
    writeData("top: ", ptr->top); \
    writeData("middle: ", ptr->middle); \
    writeData("bottom: ", ptr->bottom); \
    writeData("y: ", ptr->y)

std::shared_ptr<AssDumper> AssDumper::create()
{
    AssDumper *ret(new (std::nothrow) AssDumper());
    if (!ret)
    {
        return nullptr;
    }

    ret->m_logger = PROJ_NAMESPACE::Utils::Logger::create("out.log", "out.log");
    if (ret->m_logger == nullptr)
    {
        delete ret;
        return nullptr;
    }

    std::string fileName("in.ass");
    try
    {
        ret->m_parser = PROJ_NAMESPACE::Yutils::AssParser::create(fileName);
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        delete ret;
        return nullptr;
    }

    if (ret->m_parser == nullptr)
    {
        std::cerr << "Fail to create parser." << std::endl;
        delete ret;
        return nullptr;
    }

    try
    {
        ret->m_parser->extendDialogs();
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        delete ret;
        return nullptr;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        delete ret;
        return nullptr;
    }

    return std::shared_ptr<AssDumper>(ret);
}

void AssDumper::dumpMeta()
{
    m_logger->writeOut("dumping meta\n");
    m_logger->writeOut("\n");
    auto meta(m_parser->meta());

    writeData("wrap_style: ", meta->wrap_style);
    writeData("scaled_border_and_shadow: ", meta->scaled_border_and_shadow);
    writeData("play_res_x: ", meta->play_res_x);
    writeData("play_res_y: ", meta->play_res_y);
    writeString("colorMatrix: ", meta->colorMatrix);
    m_logger->writeOut("\n");
}

void AssDumper::dumpStyles()
{
    m_logger->writeOut("dumping styles\n");
    m_logger->writeOut("\n");

    auto styles(m_parser->styles());
    std::string out("");
    for (auto it = styles.begin(); it != styles.end(); ++it)
    {
        out = "style name: " + it->first;
        out += "\n";
        m_logger->writeOut(out);
        writeStyle(it->second);
        m_logger->writeOut("\n");
    }

    m_logger->writeOut("\n");
}

void AssDumper::dumpDialogs()
{
    m_logger->writeOut("dumping dialogs\n");
    m_logger->writeOut("\n");
    auto dialogs(m_parser->dialogs());
    for (auto it = dialogs.begin(); it != dialogs.end(); ++it)
    {
        writeDialog(*it);
    }
}

std::shared_ptr<PROJ_NAMESPACE::Yutils::AssParser>
AssDumper::getParser() const
{
    return m_parser;
}

// private member functions
void AssDumper::writeStyle(std::shared_ptr<SubFX::Yutils::AssStyle> &style)
{
    writeString("fontname: ", style->fontname);
    writeData("fontsize: ", style->fontsize);
    writeData("bold: ", static_cast<int>(style->bold));
    writeData("italic: ", static_cast<int>(style->italic));
    writeData("underline: ", static_cast<int>(style->underline));
    writeData("strikeout: ", static_cast<int>(style->strikeout));
    writeData("scale_x: ", style->scale_x);
    writeData("scale_y: ", style->scale_y);
    writeData("spaceing: ", style->spaceing);
    writeData("angle: ", style->angle);
    writeData("bolder_style: ", style->bolder_style);
    writeData("outline: ", style->outline);
    writeData("shadow: ", style->shadow);
    writeData("alignment: ", style->alignment);
    writeData("margin_l: ", style->margin_l);
    writeData("margin_r: ", style->margin_r);
    writeData("margin_v: ", style->margin_v);
    writeData("encoding: ", style->encoding);
    writeString("color1: ", style->color1);
    writeString("alpha1: ", style->alpha1);
    writeString("color2: ", style->color2);
    writeString("alpha2: ", style->alpha2);
    writeString("color3: ", style->color3);
    writeString("alpha3: ", style->alpha3);
    writeString("color4: ", style->color4);
    writeString("alpha4: ", style->alpha4);
}

void AssDumper::writeDialog(std::shared_ptr<SubFX::Yutils::AssDialog> &dialog)
{
    m_logger->writeOut("styleref:\n");
    writeStyle(dialog->styleref);
    m_logger->writeOut("\n");

    WRITE_ASS_SYMBOL(dialog);
    writeString("text_stripped: ", dialog->text_stripped);
    writeData("comment: ", dialog->comment);
    writeData("layer: ", dialog->layer);
    writeString("style: ", dialog->style);
    writeString("actor: ", dialog->actor);
    writeData("margin_l: ", dialog->margin_l);
    writeData("margin_r: ", dialog->margin_r);
    writeData("margin_v: ", dialog->margin_v);
    writeString("effect: ", dialog->effect);
    writeData("leadin: ", dialog->leadin);
    writeData("leadout: ", dialog->leadout);
    m_logger->writeOut("\n");

    for (auto it = dialog->textChunked.begin();
         it != dialog->textChunked.end();
         ++it)
    {
        writeTextChunked(*it);
    }

    m_logger->writeOut("\n");

    m_logger->writeOut("syls:\n");
    for (auto it = dialog->syls.begin();
         it != dialog->syls.end();
         ++it)
    {
        writeSyls(*it);
    }

    m_logger->writeOut("\n");

    m_logger->writeOut("words:\n");
    for (auto it = dialog->words.begin();
         it != dialog->words.end();
         ++it)
    {
        writeWords(*it);
    }

    m_logger->writeOut("\n");

    m_logger->writeOut("chars:\n");
    for (auto it = dialog->chars.begin();
         it != dialog->chars.end();
         ++it)
    {
        writeChars(*it);
    }

    m_logger->writeOut("\n");
}

void
AssDumper::writeTextChunked(
        std::shared_ptr<PROJ_NAMESPACE::Yutils::AssTextChunked> &in)
{
    m_logger->writeOut("text chunked:\n");
    writeString("tags: ", in->tags);
    writeString("text: ", in->text);
    m_logger->writeOut("\n");
}

void AssDumper::writeSyls(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssSyl> &syl)
{
    m_logger->writeOut("syl:\n");
    WRITE_ASS_SYMBOL(syl);
    writeString("tags: ", syl->tags);
    writeData("prespace: ", syl->prespace);
    writeData("postspace: ", syl->postspace);
    m_logger->writeOut("\n");
}

void AssDumper::writeWords(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssWord> &word)
{
    m_logger->writeOut("word:\n");
    WRITE_ASS_SYMBOL(word);
    writeData("prespace: ", word->prespace);
    writeData("postspace: ", word->postspace);
    m_logger->writeOut("\n");
}

void AssDumper::writeChars(std::shared_ptr<PROJ_NAMESPACE::Yutils::AssChar> &Char)
{
    m_logger->writeOut("char:\n");
    WRITE_ASS_SYMBOL(Char);
    writeData("syl_i: ", Char->syl_i);
    writeData("word_i: ", Char->word_i);
    m_logger->writeOut("\n");
}

void AssDumper::writeString(const std::string &prefix, std::string &data)
{
    std::string out(prefix);
    out += data;
    out += "\n";
    m_logger->writeOut(out);
}
