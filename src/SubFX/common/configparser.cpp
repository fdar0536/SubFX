#include "nlohmann/json.hpp"

#include "configparser.hpp"

ConfigParser::ConfigParser(string &jsonFileName) :
    assName(""),
    configDatas(vector<shared_ptr<ConfigData>>()),
    success(false),
    lastError("")
{
    static_cast<void>(jsonFileName);
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
