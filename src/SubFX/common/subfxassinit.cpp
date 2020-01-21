#include "subfxassinit.hpp"

// public member function
std::pair<std::shared_ptr<SubFXAssInit>, const char *>
SubFXAssInit::create(std::string &jsonFileName)
{
    SubFXAssInit *ret(new (std::nothrow) SubFXAssInit());
    if (!ret)
    {
        return std::make_pair(std::shared_ptr<SubFXAssInit>(),
                              "Fail to allocate memory");
    }

    const char *err(ret->parseConfig(jsonFileName));
    if (err)
    {
        delete ret;
        return std::make_pair(std::shared_ptr<SubFXAssInit>(),
                              err);
    }

    std::tie(ret->assParser, err) = YAssParser::create(ret->subName);
    if (err)
    {
        delete ret;
        return std::make_pair(std::shared_ptr<SubFXAssInit>(),
                              err);
    }

    return std::make_pair(std::shared_ptr<SubFXAssInit>(ret),
                          nullptr);
}

bool SubFXAssInit::isSylAvailable() const
{
    return assParser->isSylAvailable();
}

bool SubFXAssInit::isWordAvailable() const
{
    return assParser->isWordAvailable();
}

bool SubFXAssInit::isCharAvailable() const
{
    return assParser->isCharAvailable();
}

std::shared_ptr<YAssParser> SubFXAssInit::getParser() const
{
    return assParser;
}
