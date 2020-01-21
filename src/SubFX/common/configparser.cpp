#include <fstream>
#include <regex>
#include <map>

#include "../../common/basecommon.hpp"
#include "configparser.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

using json = nlohmann::json;

// public member function
std::string ConfigParser::getSubName() const
{
    return subName;
}

std::string ConfigParser::getLogFileName() const
{
    return logFile;
}

std::string ConfigParser::getOutputFileName() const
{
    return outputFile;
}

std::vector<std::shared_ptr<ConfigData>> ConfigParser::getConfigDatas() const
{
    return configDatas;
}

// private member function
const char *ConfigParser::parseConfig(std::string &jsonFileName)
{
    std::fstream inputFile;
    inputFile.open(jsonFileName, std::ios::in);
    if (inputFile.fail())
    {
        return "Fail to open config file.";
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
        errString = e.what();
        return errString.c_str();
    }

    inputFile.close();

    const char *err(nullptr);
    err = getConfigItem(subName, config, "subtitle");
    TESTERR(err)

    err = getConfigItem(logFile, config, "logFile");
    TESTERR(err)
    if (logFile == "")
    {
        logFile = "stdout";
    }

    err = getConfigItem(outputFile, config, "outputFile");
    TESTERR(err)

    json scripts;
    err = getConfigItem(scripts, config, "scripts");
    TESTERR(err)
    if (scripts.size() == 0)
    {
        return "No input script.";
    }

    configDatas.reserve(50);
    std::map<std::string, SubMode> modeMap;
    modeMap["line"] = SubMode::lines;
    modeMap["word"] = SubMode::words;
    modeMap["syl"] = SubMode::syls;
    modeMap["char"] = SubMode::chars;
    std::string modeRes("");
    std::regex pyScript("^.*\\.py$");
    for (size_t i = 0; i < scripts.size(); ++i)
    {
        std::shared_ptr<ConfigData> configData = std::make_shared<ConfigData>();
        if (configData == nullptr)
        {
            return "Fail to allocate memory.";
        }

        err = getConfigItem(configData->scriptName, scripts.at(i), "script");
        TESTERR(err)
        if (!regex_match(configData->scriptName, pyScript))
        {
            errString = "Invalid python script name: " + configData->scriptName;
            return errString.c_str();
        }

        err = getConfigItem(modeRes, scripts.at(i), "mode");
        TESTERR(err)
        if (modeRes != "line" &&
            modeRes != "word" &&
            modeRes != "syl" &&
            modeRes != "char")
        {
            errString = ("Invalid mode: " + modeRes);
            configDatas.clear();
            return errString.c_str();
        }

        configData->mode = modeMap[modeRes];

        err = getConfigItem(configData->startLine, scripts.at(i), "startLine");
        TESTERR(err)
        if (configData->startLine < 0)
        {
            configDatas.clear();
            return "\"startLine\" CANNOT less than ZERO.";
        }

        err = getConfigItem(configData->endLine, scripts.at(i), "endLine");
        TESTERR(err)
        if (configData->endLine >= 0 &&
            configData->startLine > configData->endLine)
        {
            configDatas.clear();
            return "\"startLine\" is GREATER THAN \"endLine\".";
        }

        configDatas.push_back(configData);
    }

    if (configDatas.size() == 0)
    {
        return "No input script.";
    }

    return nullptr;
}
