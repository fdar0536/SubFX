#ifndef FILE_HPP
#define FILE_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <vector>
#include <stdexcept>
#include <fstream>
#include <map>

#include "asscommon.hpp"
#include "ass.hpp"
#include "../common/basecommon.hpp"

using namespace std;

class SYMBOL_SHOW CoreFile : protected CoreAss
{
public:

    // constructor may throw invalid_argument
    CoreFile(const string &fileName, const string &assHeader = string());

    ~CoreFile();

    void writeAssFile(vector<string> &assBuf);

    void writeAssFile(shared_ptr<AssMeta> &meta,
                      map<string, shared_ptr<AssStyle>> &styles,
                      vector<string> &assBuf);

    // reset may throw invalid_argument
    void reset(const string &fileName, const string &assHeader);

    bool isAppend();

private:

    bool append;

    string assHeader;

    fstream file;

    void writeAssMeta(shared_ptr<AssMeta> &meta);

    void writeAssStyle(map<string, shared_ptr<AssStyle>> &styles);

    void writeAssEvent(vector<string> &assBuf);
};

#endif // FILE_HPP
