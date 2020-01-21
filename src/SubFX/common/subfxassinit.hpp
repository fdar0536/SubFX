#ifndef SUBFXASSINIT_HPP
#define SUBFXASSINIT_HPP

#include "../../YutilsCpp/YutilsCpp.hpp"
#include "configparser.hpp"
#include "assparserpy.hpp"

class SubFXAssInit : public ConfigParser
{
public:

    static std::pair<std::shared_ptr<SubFXAssInit>, const char *>
    create(std::string &jsonFileName);

    bool isSylAvailable() const;

    bool isWordAvailable() const;

    bool isCharAvailable() const;

    std::shared_ptr<AssParserPy> getParser() const;

protected:

    SubFXAssInit() :
        ConfigParser(),
        assParser(nullptr)
    {}

private:

    std::shared_ptr<AssParserPy> assParser;

};

#endif // SUBFXASSINIT_HPP
