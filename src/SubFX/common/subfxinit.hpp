#ifndef SUBFXINIT_HPP
#define SUBFXINIT_HPP

#include "../../CoreLib/python/assparserpy.hpp"
#include "configparser.hpp"

using namespace std;

class SubFXInit
{
public:
    
    SubFXInit(string &jsonFileName);
    
    bool isSylAvailable() const;
    
    bool isWordAvailable() const;
    
    bool isCharAvailable() const;
    
    vector<shared_ptr<ConfigData>> getConfigDatas() const;
    
private:
    
    void init();
    
    shared_ptr<ConfigParser> configParser;
    
    bool isSylReady;
    
    bool isWordReady;
    
    bool isCharReady;
    
    string lastError;
    
    bool success;
    
    shared_ptr<AssParserPy> assParser;
    
};

#endif // SUBFXINIT_HPP
