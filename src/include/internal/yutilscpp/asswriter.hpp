#pragma once

#include <vector>
#include <map>

#include "../basecommon.h"
#include "assparser.hpp"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace AssWriter
{

SYMBOL_SHOW void write(const char *fileName,
                       std::shared_ptr<AssParser> &parser) THROW;

SYMBOL_SHOW void write(const char *fileName,
                       const char *assHeader,
                       std::vector<std::string> &assBuf) THROW;

SYMBOL_SHOW void write(const char *fileName,
                       std::shared_ptr<AssMeta> &meta,
                       std::map<std::string, std::shared_ptr<AssStyle>> &styles,
                       std::vector<std::string> &assBuf) THROW;

} // end namespace AssWriter

} // end namespace Yutils

} // end namespace PROJ_NAMESPACE

/*
namespace Yutils
{

class SYMBOL_SHOW File : protected Ass
{
public:

    static std::pair<std::shared_ptr<File>, const char *>
    create(const std::string &fileName,
           const std::string &assHeader = std::string());

    ~File();

    void writeAssFile(std::vector<std::string> &assBuf);

    const char *writeAssFile(std::shared_ptr<AssMeta> &meta,
                             std::map<std::string, std::shared_ptr<AssStyle>> &styles,
                             std::vector<std::string> &assBuf);

    const char *reset(const std::string &fileName,
                      const std::string &assHeader);

    bool isAppend();

protected:

    File() : Ass() {}

private:

    bool append;

    std::string assHeader;

    std::fstream file;

    void writeAssMeta(std::shared_ptr<AssMeta> &meta);

    const char *writeAssStyle(std::map<std::string,
                              std::shared_ptr<AssStyle>> &styles);

    void writeAssEvent(std::vector<std::string> &assBuf);
};

}

*/
