#include "file.hpp"

CoreFile::CoreFile(const string &fileName, const string &assHeader)
{
    CoreAss();
    if (assHeader.size() == 0)
    {
        file.open(fileName, fstream::app | fstream::out);
        if (file.fail())
        {
            throw invalid_argument("CANNOT open file.");
        }
        
        append = true;
        this->assHeader = "";
    }
    else
    {
        file.open(fileName, fstream::trunc | fstream::out);
        if (file.fail())
        {
            throw invalid_argument("CANNOT open file.");
        }
        
        append = false;
        this->assHeader = assHeader;
    }
}

CoreFile::~CoreFile()
{
    file.close();
}

void CoreFile::writeAssFile(vector<string> &assBuf)
{
    if (assHeader.size() != 0)
    {
        file << assHeader;
    }

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << endl;
    }
}

void CoreFile::writeAssFile(shared_ptr<AssMeta> &meta,
                            map<string, shared_ptr<AssStyle> > &styles,
                            vector<string> &assBuf)
{
    writeAssMeta(meta);
    writeAssStyle(styles);
    writeAssEvent(assBuf);
}

void CoreFile::reset(const string &fileName, const string &assHeader)
{
    fstream newFile;
    if (assHeader.size() == 0)
    {
        newFile.open(fileName, fstream::app | fstream::out);
        if (newFile.fail())
        {
            throw invalid_argument("CANNOT reset file!");
        }
        
        append = true;
        this->assHeader = "";
    }
    else
    {
        newFile.open(fileName, fstream::trunc | fstream::out);
        if (newFile.fail())
        {
            throw invalid_argument("CANNOT reset file!");
        }

        append = false;
        this->assHeader = assHeader;
    }

    file.close();
    file.swap(newFile);
}

bool CoreFile::isAppend()
{
    return append;
}

//private member function
void CoreFile::writeAssMeta(shared_ptr<AssMeta> &meta)
{
    file << "[Script Info]" << endl;
    file << "Title:" << endl;
    file << "ScriptType: v4.00+" << endl;
    file << "WrapStyle: " << meta->wrap_style << endl;
    file << "ScaledBorderAndShadow: " << (meta->scaled_border_and_shadow ? "yes" : "no") << endl;
    
    if (meta->colorMatrix != "")
    {
        file << "YCbCr Matrix: " << meta->colorMatrix << endl;
    }
    else if (meta->play_res_x >= 1000 || meta->play_res_y >= 1000)
    {
        file << "YCbCr Matrix: TV.709" << endl;
    }
    else
    {
        file << "YCbCr Matrix: TV.601" << endl;
    }
    
    file << "PlayResX: " << meta->play_res_x << endl;
    file << "PlayResY: " << meta->play_res_y << endl;
    file << endl;
}

void CoreFile::writeAssStyle(map<string, shared_ptr<AssStyle>> &styles)
{
    file << "[V4+ Styles]" << endl;
    file << "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding" << endl;
    tuple<uint8_t, uint8_t, uint8_t, uint8_t> tmpTuple;
    vector<uint8_t> tmpVector;
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
        tmpVector.at(0) = get<0>(tmpTuple);
        tmpVector.at(1) = get<1>(tmpTuple);
        tmpVector.at(2) = get<2>(tmpTuple);

        // alpha1
        tmpTuple = stringToColorAlpha(style->alpha1);
        tmpVector.at(3) = get<3>(tmpTuple);
        file << colorAlphaToString(tmpVector) << ",";

        // color2
        tmpTuple = stringToColorAlpha(style->color2);
        tmpVector.at(0) = get<0>(tmpTuple);
        tmpVector.at(1) = get<1>(tmpTuple);
        tmpVector.at(2) = get<2>(tmpTuple);

        // alpha2
        tmpTuple = stringToColorAlpha(style->alpha2);
        tmpVector.at(3) = get<3>(tmpTuple);
        file << colorAlphaToString(tmpVector) << ",";

        // color3
        tmpTuple = stringToColorAlpha(style->color3);
        tmpVector.at(0) = get<0>(tmpTuple);
        tmpVector.at(1) = get<1>(tmpTuple);
        tmpVector.at(2) = get<2>(tmpTuple);

        // alpha3
        tmpTuple = stringToColorAlpha(style->alpha3);
        tmpVector.at(3) = get<3>(tmpTuple);
        file << colorAlphaToString(tmpVector) << ",";

        // color4
        tmpTuple = stringToColorAlpha(style->color4);
        tmpVector.at(0) = get<0>(tmpTuple);
        tmpVector.at(1) = get<1>(tmpTuple);
        tmpVector.at(2) = get<2>(tmpTuple);

        // alpha4
        tmpTuple = stringToColorAlpha(style->alpha4);
        tmpVector.at(3) = get<3>(tmpTuple);
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
        
        file << endl;
    }

    file << endl;
}

void CoreFile::writeAssEvent(vector<string> &assBuf)
{
    file << "[Events]" << endl;
    file << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text" << endl;

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << endl;
    }
}
