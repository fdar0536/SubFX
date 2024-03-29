/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "include/internal/assparser.h"
#include "logger.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct TEXT_SIZE
{
    double width;
    double height;
    double ascent;
    double descent;
    double internal_leading;
    double external_leading;
} TEXT_SIZE;

typedef enum PARSER_SECTION
{
    Idle,
    Script_Info,
    V4_Styles,
    Events
} PARSER_SECTION;

typedef struct AssParser
{
    subfx_ass_meta meta;

    fdsa_ptrMap *styles;

    fdsa_ptrVector *dialogs;

    subfx_logger *logger;

    PARSER_SECTION section;

    bool dialogParsed;

} AssParser;

subfx_exitstate subfx_assParser_init(subfx_assParser_api *);

void subfx_assParser_fin();

uint8_t subfx_assParser_parseLineRegexInit();

subfx_assParser *subfx_assParser_create(const char *fileName,
                                        const char *warningOut,
                                        char *errMsg);

subfx_exitstate subfx_assParser_destory(subfx_assParser *parser);

subfx_exitstate subfx_assParser_extendDialogs(subfx_assParser *parser,
                                              char *errMsg);

subfx_exitstate subfx_assParser_dialogIsExtended(subfx_assParser *parser,
                                                 bool *out);

void subfx_assParser_checkBom(AssParser *, uint8_t *, size_t *);

uint8_t subfx_assParser_parseLine(AssParser *,
                                  const char *,
                                  uint8_t *,
                                  char *);

subfx_exitstate subfx_assParser_parseDialogs(AssParser *,
                                             char *);

#ifdef __cplusplus
}
#endif

/*
#include <string>
#include <vector>
#include <map>

#include "../../Utils"
#include "asscommon.hpp"
#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Yutils
{
class SYMBOL_SHOW AssParser
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

    bool dialogParsed;

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
*/
