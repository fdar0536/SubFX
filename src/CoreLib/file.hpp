#ifndef FILE_HPP
#define FILE_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <vector>
#include <stdexcept>
#include <fstream>

using namespace std;

class CoreFile
{
public:

    // constructor may throw invalid_argument
    CoreFile(const string &fileName, const string &assHeader = string());

    ~CoreFile();

    void writeAssFile(vector<string> &assBuf);

    // reset may throw invalid_argument
    void reset(const string &fileName, const string &assHeader);

    bool isAppend();

private:

    bool append;

    string assHeader;

    fstream file;
};

#endif // FILE_HPP