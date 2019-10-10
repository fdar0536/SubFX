#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>
#include <memory>

#include "nlohmann/json.hpp"

using namespace std;
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
    
    string scriptName;
    
    SubMode mode;
    
    int startLine;
    
    int endLine;
};

class ConfigParser
{
public:
    
    ConfigParser(string &jsonFileName);
    
    bool isSuccess() const;
    
    string getLastError() const;
    
    string getSubName() const;
    
    string getLogFileName() const;
    
    vector<shared_ptr<ConfigData>> getConfigDatas() const;
    
private:
    
    void parseConfig(string &jsonFileName);
    
    template<class T>
    bool getConfigItem(T &dst, json &config, const char *entry)
    {
        try
        {
            dst = config[entry];
        }
        catch (nlohmann::detail::type_error &e)
        {
            lastError = e.what();
            success = false;
            return false;
        }
        
        return true;
    }
    
    string subName;
    
    string logFile;
    
    vector<shared_ptr<ConfigData>> configDatas;
    
    bool success;
    
    string lastError;
};

#endif // CONFIGPARSER_HPP
