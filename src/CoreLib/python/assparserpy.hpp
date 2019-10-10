#ifndef ASSPARSERPY_HPP
#define ASSPARSERPY_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include "../CoreLib.hpp"
#include "pybind11/pybind11.h"

using namespace std;

namespace py = pybind11;

#ifdef __GNUC__
#pragma GCC visibility push(hidden)
#endif
class AssParserPy
{
public:
    
    // constructor may throw invalid_argument
    AssParserPy(string &fileName);

    py::dict meta() const;

    py::dict styles() const;

    py::list dialogs() const;

    void upgradeDialogs();

    shared_ptr<AssMeta> getMetaPtr() const;

    map<string, shared_ptr<AssStyle>> getStyleData() const;

    vector<shared_ptr<AssDialog>> getDialogs() const;

private:

    shared_ptr<AssParser> parser;

    void initData();

    void setUpMeta();
    py::dict metaData;

    void setUpStyles();
    py::dict styleData;

    void setUpDialogs();
    bool dialogParsed;
    py::list dialogData;

    void getDialogsData();
};
#ifdef __GNUC__
#pragma GCC visibility pop
#endif

#endif // ASSPARSERPY_HPP
