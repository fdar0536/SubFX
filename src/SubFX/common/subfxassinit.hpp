#ifndef SUBFXASSINIT_HPP
#define SUBFXASSINIT_HPP

#include "../../YutilsCpp/python/assparserpy.hpp"
#include "configparser.hpp"

typedef Yutils::AssParserPy YAssParserPy;

class SYMBOL_HIDE SubFXAssInit : public ConfigParser
{
public:

    SubFXAssInit(std::string &jsonFileName);

    bool isSylAvailable() const;

    bool isWordAvailable() const;

    bool isCharAvailable() const;

    std::shared_ptr<YAssParserPy> getParser() const;

private:

    void init();

    std::shared_ptr<YAssParserPy> assParser;

};

#endif // SUBFXASSINIT_HPP
