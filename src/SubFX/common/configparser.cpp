#include <fstream>
#include <map>

#include "configparser.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

using namespace std;
using json = nlohmann::json;

#define GetCofigItem(x, y, z) if (!getConfigItem(x, y, z)) \
                              { \
                                  return; \
                              }

ConfigParser::ConfigParser(string &jsonFileName) :
    subName(""),
    logFile(""),
    configDatas(vector<shared_ptr<ConfigData>>()),
    success(false),
    lastError("")
{
    parseConfig(jsonFileName);
}

bool ConfigParser::isSuccess() const
{
    return success;
}

string ConfigParser::getLastError() const
{
    return lastError;
}

vector<shared_ptr<ConfigData>> ConfigParser::getConfigDatas() const
{
    return configDatas;
}

// private member function
void ConfigParser::parseConfig(string &jsonFileName)
{
    fstream inputFile;
    inputFile.open(jsonFileName, ios::in);
    if (inputFile.fail())
    {
        success = false;
        lastError = "Fail to open config file.";
        return;
    }
    
    json config;
    try
    {
        inputFile >> config;
    }
    catch (nlohmann::detail::parse_error &e)
    {
        inputFile.close();
        lastError = e.what();
        success = false;
        return;
    }
    
    inputFile.close();
    
    GetCofigItem(subName, config, "subtitle")
    
    GetCofigItem(logFile, config, "logFile")
    if (logFile == "")
    {
        logFile = "stdout";
    }
    
    json scripts;
    GetCofigItem(scripts, config, "scripts")
    if (scripts.size() == 0)
    {
        success = false;
        lastError = ("No input script.");
        return;
    }
    
    configDatas.reserve(50);
    map<string, SubMode> modeMap;
    modeMap["line"] = SubMode::lines;
    modeMap["word"] = SubMode::words;
    modeMap["syl"] = SubMode::syls;
    modeMap["char"] = SubMode::chars;
    string modeRes("");
    for (size_t i = 0; i < scripts.size(); ++i)
    {
        shared_ptr<ConfigData> configData = make_shared<ConfigData>();
        if (configData == nullptr)
        {
            lastError = "Fail to allocate memory.";
            success = false;
            return;
        }
        
        GetCofigItem(configData->scriptName, scripts.at(i), "script")
        GetCofigItem(modeRes, scripts.at(i), "mode")
        if (modeRes != "line" &&
            modeRes != "word" &&
            modeRes != "syl" &&
            modeRes != "char")
        {
            success = false;
            lastError = ("Invalid mode: " + modeRes);
            configDatas.clear();
            return;
        }
        
        configData->mode = modeMap[modeRes];
        
        GetCofigItem(configData->startLine, scripts.at(i), "startLine")
        if (configData->startLine < 0)
        {
            success = false;
            lastError = "\"startLine\" CANNOT less than ZERO.";
            configDatas.clear();
            return;
        }
        
        GetCofigItem(configData->endLine, scripts.at(i), "endLine")
        configDatas.push_back(configData);
    }
    
    if (configDatas.size() == 0)
    {
        success = false;
        lastError = ("No input script.");
        return;
    }
    
    success = true;
}
