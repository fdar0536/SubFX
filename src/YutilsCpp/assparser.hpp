#ifndef ASSPARSER_HPP
#define ASSPARSER_HPP

#include <string>
#include <vector>
#include <map>

#include "ass.hpp"
#include "asscommon.hpp"
#include "utf8.hpp"
#include "../common/basecommon.hpp"

namespace Yutils
{

class SYMBOL_SHOW AssParser : protected Ass, protected Utf8
{
public:

    static std::pair<std::shared_ptr<AssParser>, const char *>
    create(const std::string &fileName);

    std::shared_ptr<AssMeta> meta() const;

    std::map<std::string, std::shared_ptr<AssStyle>> styles() const;

    std::vector<std::shared_ptr<AssDialog>> dialogs() const;

    const char *upgradeDialogs();

    bool dialogIsUpgraded() const;

    bool isSylAvailable() const;

    bool isWordAvailable() const;

    bool isCharAvailable() const;


protected:

    AssParser() :
        Ass(),
        Utf8(),
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

    // https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    std::istream &safeGetline(std::istream &is, std::string &t);

    typedef enum _PARSER_SECTION
    {
        Idle,
        Script_Info,
        V4_Styles,
        Events
    } PARSER_SECTION;

    PARSER_SECTION section;

    std::string checkBom(std::string &);

    const char *parseLine(std::string &);

    std::shared_ptr<AssMeta> metaData;

    std::map<std::string, std::shared_ptr<AssStyle>> styleData;

    bool dialogParsed;

    std::vector<std::shared_ptr<AssDialog>> dialogData;

    bool sylReady;

    bool wordReady;

    bool charReady;

    const char *parseDialogs();

    typedef struct _TEXT_SIZE
    {
        double width;
        double height;
        double ascent;
        double descent;
        double internal_leading;
        double external_leading;
    } TEXT_SIZE;

    std::pair<std::shared_ptr<TEXT_SIZE>, const char *>
    textSize(std::string &text, std::shared_ptr<AssStyle> &style);
};

}

#endif // ASSPARSER_HPP
