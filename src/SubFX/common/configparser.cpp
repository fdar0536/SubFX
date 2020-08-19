#include <fstream>
#include <regex>
#include <map>

#include "internal/basecommon.h"
#include "configparser.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

using json = nlohmann::json;

// public member functions
std::shared_ptr<ConfigParser>
    ConfigParser::create(std::string &jsonFileName) THROW
{
    ConfigParser *ret(new (std::nothrow) ConfigParser());
    if (!ret)
    {
        return nullptr;
    }

    // here may throw exception
    ret->parseConfig(jsonFileName);
    return std::shared_ptr<ConfigParser>(ret);
}

std::string ConfigParser::getSubName() const NOTHROW
{
    return m_subName;
}

std::string ConfigParser::getLogFileName() const NOTHROW
{
    return m_logFile;
}

std::string ConfigParser::getOutputFileName() const NOTHROW
{
    return m_outputFile;
}

std::vector<std::shared_ptr<ConfigData>>
ConfigParser::getConfigDatas() const NOTHROW
{
    return m_configDatas;
}

// private member function
void ConfigParser::parseConfig(std::string &jsonFileName) THROW
{
    std::fstream inputFile;
    inputFile.open(jsonFileName, std::ios::in);
    if (inputFile.fail())
    {
        throw std::invalid_argument("Fail to open config file.");
    }

    std::string errString;
    json config;
    try
    {
        inputFile >> config;
    }
    catch (nlohmann::detail::parse_error &e)
    {
        inputFile.close();
        throw std::invalid_argument(e.what());
    }

    inputFile.close();

    getConfigItem(m_subName, config, "subtitle");

    getConfigItem(m_logFile, config, "logFile");
    if (m_logFile == "")
    {
        m_logFile = "stdout";
    }

    getConfigItem(m_outputFile, config, "outputFile");

    json scripts;
    getConfigItem(scripts, config, "scripts");
    if (scripts.size() == 0)
    {
        throw std::invalid_argument("No input script.");
    }

    m_configDatas.reserve(50);
    std::map<std::string, SubMode> modeMap;
    modeMap["line"] = SubMode::lines;
    modeMap["word"] = SubMode::words;
    modeMap["syl"] = SubMode::syls;
    modeMap["char"] = SubMode::chars;
    std::string modeRes("");
    std::regex pyScript("^.*\\.py$");
    for (size_t i = 0; i < scripts.size(); ++i)
    {
        std::shared_ptr<ConfigData>
                configData = std::make_shared<ConfigData>();
        if (configData == nullptr)
        {
            throw std::invalid_argument("Fail to allocate memory.");
        }

        getConfigItem(configData->scriptName, scripts.at(i), "script");
        if (!regex_match(configData->scriptName, pyScript))
        {
            errString = "Invalid python script name: "
                    + configData->scriptName;
            throw std::invalid_argument(errString);
        }

        getConfigItem(modeRes, scripts.at(i), "mode");
        if (modeRes != "line" &&
            modeRes != "word" &&
            modeRes != "syl" &&
            modeRes != "char")
        {
            errString = ("Invalid mode: " + modeRes);
            m_configDatas.clear();
            throw std::invalid_argument(errString);
        }

        configData->mode = modeMap[modeRes];

        getConfigItem(configData->startLine, scripts.at(i), "startLine");
        if (configData->startLine < 0)
        {
            m_configDatas.clear();
            throw std::invalid_argument("\"startLine\" CANNOT less "
                                        "than ZERO.");
        }

        getConfigItem(configData->endLine, scripts.at(i), "endLine");
        if (configData->endLine >= 0 &&
            configData->startLine > configData->endLine)
        {
            m_configDatas.clear();
            throw std::invalid_argument("\"startLine\" is GREATER "
                                        "THAN \"endLine\".");
        }

        m_configDatas.push_back(configData);
    }

    if (m_configDatas.size() == 0)
    {
        throw std::invalid_argument("No input script.");
    }
}
