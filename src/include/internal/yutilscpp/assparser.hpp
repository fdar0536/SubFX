#pragma once

#include <string>
#include <vector>
#include <map>

#include "ass.hpp"
#include "asscommon.hpp"
#include "utf8.hpp"
#include "../basecommon.h"
#include "../utils/utils.hpp"

namespace PROJ_NAMESPACE
{

namespace Yutils
{
class SYMBOL_SHOW AssParser : protected Ass, protected Utf8
{
public:

    static std::shared_ptr<AssParser>
    create(const std::string &fileName,
           const std::string &warningOut = "") THROW;

    std::shared_ptr<AssMeta> meta() const NOTHROW;

    std::map<std::string, std::shared_ptr<AssStyle>> styles() const NOTHROW;

    std::vector<std::shared_ptr<AssDialog>> dialogs() const NOTHROW;

    void extendDialogs() THROW;

    bool dialogIsExtended() const NOTHROW;

    bool isSylAvailable() const NOTHROW;

    bool isWordAvailable() const NOTHROW;

    bool isCharAvailable() const NOTHROW;

protected:

    AssParser() NOTHROW :
        Ass(),
        Utf8(),
        m_logger(nullptr),
        section(Idle),
        metaData(std::make_shared<AssMeta>()),
        styleData(std::map<std::string, std::shared_ptr<AssStyle>>()),
        dialogParsed(false),
        dialogData(std::vector<std::shared_ptr<AssDialog>>()),
        sylReady(false),
        wordReady(false),
        charReady(false)
    {}

private:

    AssParser(const AssParser &) = delete;

    AssParser& operator=(const AssParser &other) = delete;

    AssParser& operator=(AssParser &&other) = delete;

    std::shared_ptr<PROJ_NAMESPACE::Utils::Logger> m_logger;

    // https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    std::istream &safeGetline(std::istream &is, std::string &t) NOTHROW;

    typedef enum _PARSER_SECTION
    {
        Idle,
        Script_Info,
        V4_Styles,
        Events
    } PARSER_SECTION;

    PARSER_SECTION section;

    std::string checkBom(std::string &) NOTHROW;

    void parseLine(std::string &, uint8_t *flags) THROW;

    std::shared_ptr<AssMeta> metaData;

    std::map<std::string, std::shared_ptr<AssStyle>> styleData;

    bool dialogParsed;

    std::vector<std::shared_ptr<AssDialog>> dialogData;

    bool sylReady;

    bool wordReady;

    bool charReady;

    void parseDialogs() THROW;

    typedef struct TEXT_SIZE
    {
        double width;
        double height;
        double ascent;
        double descent;
        double internal_leading;
        double external_leading;
    } TEXT_SIZE;

    std::shared_ptr<TEXT_SIZE>
    textSize(std::string &text, std::shared_ptr<AssStyle> &style) THROW;
};
} // end namespace Yutils

} // end namespace PROJ_NAMESPACE
