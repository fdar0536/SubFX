#ifndef SUBFXASSINIT_HPP
#define SUBFXASSINIT_HPP

#include "../../CoreLib/python/assparserpy.hpp"
#include "configparser.hpp"

using namespace std;

class SubFXAssInit : public ConfigParser
{
public:
    
    SubFXAssInit(string &jsonFileName);
    
    bool isSylAvailable() const;
    
    bool isWordAvailable() const;
    
    bool isCharAvailable() const;
    
    shared_ptr<AssParserPy> getParser() const;
    
private:
    
    void init();
    
    shared_ptr<AssParserPy> assParser;
    
};

#endif // SUBFXASSINIT_HPP
