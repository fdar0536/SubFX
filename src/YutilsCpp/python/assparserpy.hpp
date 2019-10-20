#ifndef ASSPARSERPY_HPP
#define ASSPARSERPY_HPP

#ifdef _MSC_VER
#pragma once
#endif    /* _MSC_VER */

#include "../YutilsCpp.hpp"
#include "pybind11/pybind11.h"

#include "../../common/basecommon.hpp"

namespace py = pybind11;

namespace Yutils
{

class SYMBOL_SHOW AssParserPy
{
public:

    // constructor may throw invalid_argument
    AssParserPy(std::string &fileName);

    py::dict meta() const;

    py::dict styles() const;

    py::list dialogs() const;

    void upgradeDialogs();

    std::shared_ptr<AssMeta> getMetaPtr() const;

    std::map<std::string, std::shared_ptr<AssStyle>> getStyleData() const;

    std::vector<std::shared_ptr<AssDialog>> getDialogs() const;

    bool isSylAvailable() const;

    bool isWordAvailable() const;

    bool isCharAvailable() const;

private:

    std::shared_ptr<AssParser> parser;

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

}

#endif // ASSPARSERPY_HPP
