#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>

#include <cstdint>
#include <vector>
#include <memory>

using namespace std;

class ConfigData
{
public:
    ConfigData() :
    scriptName(""),
    startLine(0),
    endLine(0)
    {}
    
    string scriptName;
    
    uint32_t startLine;
    
    uint32_t endLine;
};

class ConfigParser
{
public:
    
    ConfigParser(string &jsonFileName);
    
    bool isSuccess() const;
    
    string getLastError() const;
    
    vector<shared_ptr<ConfigData>> getConfigDatas() const;
    
private:
    
    string assName;
    
    vector<shared_ptr<ConfigData>> configDatas;
    
    bool success;
    
    string lastError;
};

#endif // CONFIGPARSER_HPP
