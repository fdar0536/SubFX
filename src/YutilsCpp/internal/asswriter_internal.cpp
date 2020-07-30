/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#include "asswriter_internal.hpp"

using namespace PROJ_NAMESPACE::Yutils;

void AssWriter_Internal::checkFileName(const char *fileName) THROW
{
    if (!fileName)
    {
        throw std::invalid_argument("write: fileName CANNOT BE nullptr");
    }

    if (!strlen(fileName))
    {
        throw std::invalid_argument("write: fileName CANNOT BE EMPTY string");
    }
}

void AssWriter_Internal::writeMeta(std::ostream &file,
                                   std::shared_ptr<AssMeta> &meta) NOTHROW
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

void AssWriter_Internal::writeStyle(std::ostream &file,
                                    std::map<std::string,
                                    std::shared_ptr<AssStyle>> &styles) THROW
{
    file << "[V4+ Styles]" << std::endl;
    file << "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding" << std::endl;
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> tmpTuple;
    std::vector<uint8_t> tmpVector;
    tmpVector.resize(4);
    std::string tmpString;
    for (auto i = styles.begin(); i != styles.end(); ++i)
    {
        auto style(i->second);
        file << "Style: ";
        file << i->first << ",";
        file << style->fontname << ",";
        file << style->fontsize << ",";

        // color1
        // here may throw
        tmpTuple= Ass::stringToColorAlpha(style->color1);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha1
        // here may throw
        tmpTuple = Ass::stringToColorAlpha(style->alpha1);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        tmpString = Ass::colorAlphaToString(tmpVector);
        file << tmpString << ",";

        // color2
        // here may throw
        tmpTuple = Ass::stringToColorAlpha(style->color2);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha2
        // here may throw
        tmpTuple = Ass::stringToColorAlpha(style->alpha2);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        tmpString = Ass::colorAlphaToString(tmpVector);
        file << tmpString << ",";

        // color3
        // here may throw
        tmpTuple = Ass::stringToColorAlpha(style->color3);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha3
        // here may throw
        tmpTuple = Ass::stringToColorAlpha(style->alpha3);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        tmpString = Ass::colorAlphaToString(tmpVector);
        file << tmpString << ",";

        // color4
        // here may throw
        tmpTuple = Ass::stringToColorAlpha(style->color4);
        tmpVector.at(0) = std::get<0>(tmpTuple);
        tmpVector.at(1) = std::get<1>(tmpTuple);
        tmpVector.at(2) = std::get<2>(tmpTuple);

        // alpha4
        // here may throw
        tmpTuple = Ass::stringToColorAlpha(style->alpha4);
        tmpVector.at(3) = std::get<3>(tmpTuple);
        tmpString = Ass::colorAlphaToString(tmpVector);
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
}

void AssWriter_Internal::writeEventHeader(std::ostream &file) NOTHROW
{
    file << "[Events]" << std::endl;
    file << "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text" << std::endl;
}

void AssWriter_Internal::writeEvent(std::ostream &file,
                                    std::vector<std::string> &assBuf) NOTHROW
{
    writeEventHeader(file);

    for (auto i = assBuf.begin(); i != assBuf.end(); ++i)
    {
        file << *i << std::endl;
    }
}

void AssWriter_Internal::writeEvent(std::ostream &file,
                                    std::vector<std::shared_ptr<AssDialog>>
                                    &dialogs) NOTHROW
{
    writeEventHeader(file);

    for (auto i = dialogs.begin(); i != dialogs.end(); ++i)
    {
        auto dialog = *i;
        if (dialog->comment)
        {
            file << "Comment: ";
        }
        else
        {
            file << "Dialogue: ";
        }

        file << dialog->layer << ",";
        file << Ass::msToString(dialog->start_time) << ",";
        file << Ass::msToString(dialog->end_time) << ",";
        file << dialog->actor << ",";
        file << dialog->margin_l << ",";
        file << dialog->margin_r << ",";
        file << dialog->margin_v << ",";
        file << dialog->effect << ",";
        file << dialog->text;

        file << std::endl;
    }

    file << std::endl;
}
