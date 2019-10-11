#include "subfxassinit.hpp"

using namespace std;

SubFXAssInit::SubFXAssInit(string &jsonFileName) :
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

shared_ptr<AssParserPy> SubFXAssInit::getParser() const
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
        assParser = make_shared<AssParserPy>(subName);
        assParser->upgradeDialogs();
    }
    catch (invalid_argument &e)
    {
        lastError = e.what();
        success = false;
        return;
    }
    catch (runtime_error &e)
    {
        lastError = e.what();
        success = false;
        return;
    }
    /*
    catch (...)
    {
        lastError = "Unknown error";
        success = false;
        return;
    }
    */
    success = true;
}
