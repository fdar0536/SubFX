#include "subfxinit.hpp"

using namespace std;

SubFXInit::SubFXInit(string &jsonFileName) :
    ConfigParser(jsonFileName),
    assParser(nullptr)
{
    init();
}

bool SubFXInit::isSylAvailable() const
{
    return assParser->isSylAvailable();
}

bool SubFXInit::isWordAvailable() const
{
    return assParser->isWordAvailable();
}

bool SubFXInit::isCharAvailable() const
{
    return assParser->isCharAvailable();
}

// private member function
void SubFXInit::init()
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
}
