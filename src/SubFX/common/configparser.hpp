#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#ifdef _MSC_VER // fix me, this is just a workaround
#undef snprintf
#endif
#include "nlohmann/json.hpp"

#include "internal/basecommon.h"

using json = nlohmann::json;

enum class SubMode
{
    lines, syls, words, chars
};

class ConfigData
{
public:
    ConfigData() :
    scriptName(""),
    mode(SubMode::lines),
    startLine(0),
    endLine(0)
    {}

    std::string scriptName;

    SubMode mode;

    int startLine;

    int endLine;
};

class ConfigParser
{
public:

    static std::shared_ptr<ConfigParser>
    create(std::string &jsonFileName) THROW;

    std::string getSubName() const NOTHROW;

    std::string getLogFileName() const NOTHROW;

    std::string getOutputFileName() const NOTHROW;

    std::vector<std::shared_ptr<ConfigData>>
    getConfigDatas() const NOTHROW;

protected:

    ConfigParser() :
        m_subName(""),
        m_logFile(""),
        m_outputFile(""),
        m_configDatas(std::vector<std::shared_ptr<ConfigData>>())
    {}

    std::string m_subName;

    std::string m_logFile;

    std::string m_outputFile;

    std::vector<std::shared_ptr<ConfigData>> m_configDatas;

    void parseConfig(std::string &jsonFileName) THROW;

    template<class T>
    void getConfigItem(T &dst, json &config, const char *entry) THROW
    {
        try
        {
            dst = config[entry];
        }
        catch (nlohmann::detail::type_error &e)
        {
            throw std::invalid_argument(e.what());
        }
    }
};

#endif // CONFIGPARSER_HPP
