#include "subfxassinit.hpp"

SubFXAssInit::SubFXAssInit(std::string &jsonFileName) :
    ConfigParser(jsonFileName),
    assParser(nullptr)
{
    init();
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

std::shared_ptr<YAssParserPy> SubFXAssInit::getParser() const
{
    return assParser;
}

// private member function
void SubFXAssInit::init()
{
    if (!success)
    {
        return;
    }

    try
    {
        assParser = std::make_shared<YAssParserPy>(subName);
        assParser->upgradeDialogs();
    }
    catch (std::invalid_argument &e)
    {
        lastError = e.what();
        success = false;
        return;
    }
    catch (std::runtime_error &e)
    {
        lastError = e.what();
        success = false;
        return;
    }

    success = true;
}
