#ifndef UTF8_HPP
#define UTF8_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <vector>
#include <string>

#include <cstdint>

#include "../common/basecommon.hpp"

using namespace std;

class SYMBOL_SHOW CoreUtf8
{
public:
    CoreUtf8() {}

protected:
    
    uint32_t utf8StringLen(string &);

    // https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters
    vector<string> utf8StringSplit(string &);
};

#endif // UTF8_HPP
