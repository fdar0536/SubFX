#include "file.hpp"

using namespace Yutils;

std::pair<std::shared_ptr<File>, const char *>
File::create(const std::string &fileName,
             const std::string &assHeader)
{
    File *ret(new (std::nothrow) File());
    if (!ret)
    {
        return std::make_pair(std::shared_ptr<File>(nullptr),
                              "Fail to allocate memory");
    }

    if (assHeader.size() == 0)
    {
        ret->file.open(fileName, std::fstream::app | std::fstream::out);
        if (ret->file.fail())
        {
            delete ret;
            return std::make_pair(std::shared_ptr<File>(nullptr),
                                  "CANNOT open file.");
        }

        ret->append = true;
        ret->assHeader = "";
    }
    else
    {
        ret->file.open(fileName, std::fstream::trunc | std::fstream::out);
        if (ret->file.fail())
        {
            delete ret;
            return std::make_pair(std::shared_ptr<File>(nullptr),
                                  "CANNOT open file.");
        }

        ret->append = false;
        ret->assHeader = assHeader;
    }

    return std::make_pair(std::shared_ptr<File>(ret), nullptr);
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

const char *File::writeAssFile(std::shared_ptr<AssMeta> &meta,
                               std::map<std::string,
                               std::shared_ptr<AssStyle> > &styles,
                               std::vector<std::string> &assBuf)
{
    writeAssMeta(meta);
    const char *err = writeAssStyle(styles);
    TESTERR(err)
    writeAssEvent(assBuf);
    return nullptr;
}

const char *File::reset(const std::string &fileName,
                        const std::string &assHeader)
{
    std::fstream newFile;
    if (assHeader.size() == 0)
    {
        newFile.open(fileName, std::fstream::app | std::fstream::out);
        if (newFile.fail())
        {
            return "CANNOT reset file!";
        }

        append = true;
        this->assHeader = "";
    }
    else
    {
        newFile.open(fileName, std::fstream::trunc | std::fstream::out);
        if (newFile.fail())
        {
            return "CANNOT reset file!";
        }

        append = false;
        this->assHeader = assHeader;
    }

    file.close();
    file.swap(newFile);
    return nullptr;
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
    file << "WrapStyle: " << (static_cast<int>(meta->wrap_style) - 48) << std::endl;
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

const char *File::writeAssStyle(std::map<std::string,
                                std::shared_ptr<AssStyle>> &styles)
{
    file << "[V4+ Styles]" << std::endl;
    file << "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding" << std::endl;
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> tmpTuple;
    std::vector<uint8_t> tmpVector;
    tmpVector.resize(4);
    const char *err(nullptr);
    std::string tmpString;
    for (auto i = styles.begin(); i != styles.end(); ++i)
    {
        auto style(i->second);
        file << "Style: ";
        file << i->first << ",";
        file << style->fontname << ",";
        file << style->fontsize << ",";

        // color1
        std::tie(tmpTuple, err) = stringToColorAlpha(style->color1);
        TESTERR(err)
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha1
        std::tie(tmpTuple, err) = stringToColorAlpha(style->alpha1);
        TESTERR(err)
        tmpVector.at(3) = std::get<3>(tmpTuple);
        std::tie(tmpString, err) = colorAlphaToString(tmpVector);
        TESTERR(err)
        file << tmpString << ",";

        // color2
        std::tie(tmpTuple, err) = stringToColorAlpha(style->color2);
        TESTERR(err)
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha2
        std::tie(tmpTuple, err) = stringToColorAlpha(style->alpha2);
        TESTERR(err)
        tmpVector.at(3) = std::get<3>(tmpTuple);
        std::tie(tmpString, err) = colorAlphaToString(tmpVector);
        TESTERR(err)
        file << tmpString << ",";

        // color3
        std::tie(tmpTuple, err) = stringToColorAlpha(style->color3);
        TESTERR(err)
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha3
        std::tie(tmpTuple, err) = stringToColorAlpha(style->alpha3);
        TESTERR(err)
        tmpVector.at(3) = std::get<3>(tmpTuple);
        std::tie(tmpString, err) = colorAlphaToString(tmpVector);
        TESTERR(err)
        file << tmpString << ",";

        // color4
        std::tie(tmpTuple, err) = stringToColorAlpha(style->color4);
        TESTERR(err)
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha4
        std::tie(tmpTuple, err) = stringToColorAlpha(style->alpha4);
        TESTERR(err)
        tmpVector.at(3) = std::get<3>(tmpTuple);
        std::tie(tmpString, err) = colorAlphaToString(tmpVector);
        file << tmpString << ",";

        file << static_cast<int>(style->bold) << ",";
        file << static_cast<int>(style->italic) << ",";
        file << static_cast<int>(style->underline) << ",";
        file << static_cast<int>(style->strikeout) << ",";

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
    return nullptr;
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
