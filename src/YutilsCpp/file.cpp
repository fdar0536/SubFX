#include "file.hpp"

using namespace Yutils;

File::File(const std::string &fileName,
           const std::string &assHeader)
{
    Ass();
    if (assHeader.size() == 0)
    {
        file.open(fileName, std::fstream::app | std::fstream::out);
        if (file.fail())
        {
            throw std::invalid_argument("CANNOT open file.");
        }

        append = true;
        this->assHeader = "";
    }
    else
    {
        file.open(fileName, std::fstream::trunc | std::fstream::out);
        if (file.fail())
        {
            throw std::invalid_argument("CANNOT open file.");
        }

        append = false;
        this->assHeader = assHeader;
    }
}

File::~File()
{
    file.close();
}

void File::writeAssFile(std::vector<std::string> &assBuf)
{
    if (assHeader.size() != 0)
    {
        file << assHeader;
    }

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << std::endl;
    }
}

void File::writeAssFile(std::shared_ptr<AssMeta> &meta,
                        std::map<std::string, std::shared_ptr<AssStyle> > &styles,
                        std::vector<std::string> &assBuf)
{
    writeAssMeta(meta);
    writeAssStyle(styles);
    writeAssEvent(assBuf);
}

void File::reset(const std::string &fileName,
                 const std::string &assHeader)
{
    std::fstream newFile;
    if (assHeader.size() == 0)
    {
        newFile.open(fileName, std::fstream::app | std::fstream::out);
        if (newFile.fail())
        {
            throw std::invalid_argument("CANNOT reset file!");
        }

        append = true;
        this->assHeader = "";
    }
    else
    {
        newFile.open(fileName, std::fstream::trunc | std::fstream::out);
        if (newFile.fail())
        {
            throw std::invalid_argument("CANNOT reset file!");
        }

        append = false;
        this->assHeader = assHeader;
    }

    file.close();
    file.swap(newFile);
}

bool File::isAppend()
{
    return append;
}

//private member function
void File::writeAssMeta(std::shared_ptr<AssMeta> &meta)
{
    file << "[Script Info]" << std::endl;
    file << "Title:" << std::endl;
    file << "ScriptType: v4.00+" << std::endl;
    file << "WrapStyle: " << meta->wrap_style << std::endl;
    file << "ScaledBorderAndShadow: " << (meta->scaled_border_and_shadow ? "yes" : "no") << std::endl;

    if (meta->colorMatrix != "")
    {
        file << "YCbCr Matrix: " << meta->colorMatrix << std::endl;
    }
    else if (meta->play_res_x >= 1000 || meta->play_res_y >= 1000)
    {
        file << "YCbCr Matrix: TV.709" << std::endl;
    }
    else
    {
        file << "YCbCr Matrix: TV.601" << std::endl;
    }

    file << "PlayResX: " << meta->play_res_x << std::endl;
    file << "PlayResY: " << meta->play_res_y << std::endl;
    file << std::endl;
}

void File::writeAssStyle(std::map<std::string, std::shared_ptr<AssStyle>> &styles)
{
    file << "[V4+ Styles]" << std::endl;
    file << "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding" << std::endl;
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> tmpTuple;
    std::vector<uint8_t> tmpVector;
    tmpVector.resize(4);
    for (auto i = styles.begin(); i != styles.end(); ++i)
    {
        auto style(i->second);
        file << "Style: ";
        file << i->first << ",";
        file << style->fontname << ",";
        file << style->fontsize << ",";

        // color1
        tmpTuple = stringToColorAlpha(style->color1);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha1
        tmpTuple = stringToColorAlpha(style->alpha1);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        file << colorAlphaToString(tmpVector) << ",";

        // color2
        tmpTuple = stringToColorAlpha(style->color2);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha2
        tmpTuple = stringToColorAlpha(style->alpha2);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        file << colorAlphaToString(tmpVector) << ",";

        // color3
        tmpTuple = stringToColorAlpha(style->color3);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha3
        tmpTuple = stringToColorAlpha(style->alpha3);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        file << colorAlphaToString(tmpVector) << ",";

        // color4
        tmpTuple = stringToColorAlpha(style->color4);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha4
        tmpTuple = stringToColorAlpha(style->alpha4);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        file << colorAlphaToString(tmpVector) << ",";

        file << (int)style->bold << ",";
        file << (int)style->italic << ",";
        file << (int)style->underline << ",";
        file << (int)style->strikeout << ",";

        file << style->scale_x << ",";
        file << style->scale_y << ",";

        file << style->spaceing << ",";
        file << style->angle << ",";

        file << style->bolder_style << ",";
        file << style->outline << ",";
        file << style->shadow << ",";
        file << style->alignment << ",";

        file << style->margin_l << ",";
        file << style->margin_r << ",";
        file << style->margin_v << ",";

        file << style->encoding;

        file << std::endl;
    }

    file << std::endl;
}

void File::writeAssEvent(std::vector<std::string> &assBuf)
{
    file << "[Events]" << std::endl;
    file << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text" << std::endl;

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << std::endl;
    }
}
