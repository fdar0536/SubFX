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
