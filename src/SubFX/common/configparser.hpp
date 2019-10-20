#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>
#include <memory>

#include "nlohmann/json.hpp"

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

    bool isSuccess() const;

    std::string getLastError() const;

    std::string getSubName() const;

    std::string getLogFileName() const;

    std::string getOutputFileName() const;

    std::vector<std::shared_ptr<ConfigData>> getConfigDatas() const;

protected:

    ConfigParser(std::string &jsonFileName);

    std::string subName;

    std::string logFile;

    std::string outputFile;

    std::vector<std::shared_ptr<ConfigData>> configDatas;

    bool success;

    std::string lastError;

private:

    void parseConfig(std::string &jsonFileName);

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

};

#endif // CONFIGPARSER_HPP
