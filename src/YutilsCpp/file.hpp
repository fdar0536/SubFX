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

namespace Yutils
{

class SYMBOL_SHOW File : protected Ass
{
public:

    // constructor may throw invalid_argument
    File(const std::string &fileName,
         const std::string &assHeader = std::string());

    ~File();

    void writeAssFile(std::vector<std::string> &assBuf);

    void writeAssFile(std::shared_ptr<AssMeta> &meta,
                      std::map<std::string, std::shared_ptr<AssStyle>> &styles,
                      std::vector<std::string> &assBuf);

    // reset may throw invalid_argument
    void reset(const std::string &fileName,
               const std::string &assHeader);

    bool isAppend();

private:

    bool append;

    std::string assHeader;

    std::fstream file;

    void writeAssMeta(std::shared_ptr<AssMeta> &meta);

    void writeAssStyle(std::map<std::string, std::shared_ptr<AssStyle>> &styles);

    void writeAssEvent(std::vector<std::string> &assBuf);
};

}

#endif // FILE_HPP
