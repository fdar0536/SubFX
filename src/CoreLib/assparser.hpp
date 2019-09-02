#ifndef ASSPARSER_HPP
#define ASSPARSER_HPP

#define PYTHON_BIND 1

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include <string>
#include <vector>
#include <map>

#include "ass.hpp"
#include "asscommon.hpp"

using namespace std;

class AssParser : protected CoreAss
{
public:
    
    AssParser(const string &fileName);

    ~AssParser();

    AssMeta *meta() const;

    map<string, AssStyle *> styles() const;

    vector<AssDialog *> dialogs() const;

private:
    
    // https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    istream &safeGetline(istream &is, string &t);

    typedef enum _PARSER_SECTION
    {
        Idle,
        Script_Info,
        V4_Styles,
        Events
    } PARSER_SECTION;

    PARSER_SECTION section;

    void parseLine(string &);

    AssMeta *metaData;

    map<string, AssStyle *> styleData;

    vector<AssDialog *> dialogData;
};

#endif // ASSPARSER_HPP