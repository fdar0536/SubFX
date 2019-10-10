#include "subfxinit.hpp"

using namespace std;

SubFXInit::SubFXInit(string &jsonFileName) :
    configParser(make_shared<ConfigParser>(jsonFileName)),
    isSylReady(false),
    isWordReady(false),
    isCharReady(false),
    lastError(""),
    success(false),
    assParser(nullptr)
{
    init();
}

// private member function
void SubFXInit::init()
{
    if (configParser == nullptr)
    {
        lastError = "Fail to allocate memory";
        success = false;
        return;
    }
    
    if (!configParser->isSuccess())
    {
        lastError = configParser->getLastError();
        success = false;
        return;
    }
    
    try
    {
        assParser = make_shared<AssParserPy>(configParser->getSubName());
        assParser->upgradeDialogs();
        isSylReady = (assParser->getDialogs());
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
