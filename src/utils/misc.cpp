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

#include "Utils"

using namespace PROJ_NAMESPACE::Utils;

SYMBOL_SHOW std::string Misc::doubleToString(double input) NOTHROW
{
    double buffer[500];
    sprintf(reinterpret_cast<char *>(buffer), "%.3lf", input);
    return std::string(reinterpret_cast<char *>(buffer));
}

SYMBOL_SHOW std::istream
&Misc::safeGetline(std::istream &is, std::string &buf) NOTHROW
{
    buf.clear();
    buf.reserve(5120);

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    while(1)
    {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(buf.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            buf += static_cast<char>(c);
        }
    }
}
