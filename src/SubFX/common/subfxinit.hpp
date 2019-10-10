#ifndef SUBFXINIT_HPP
#define SUBFXINIT_HPP

#include "../../CoreLib/python/assparserpy.hpp"
#include "configparser.hpp"

using namespace std;

class SubFXInit : public ConfigParser
{
public:
    
    SubFXInit(string &jsonFileName);
    
    bool isSylAvailable() const;
    
    bool isWordAvailable() const;
    
    bool isCharAvailable() const;
    
private:
    
    void init();
    
    shared_ptr<AssParserPy> assParser;
    
};

#endif // SUBFXINIT_HPP
