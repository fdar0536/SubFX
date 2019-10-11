#include <iostream>

#include <cstring>

#include "pybind11/embed.h"
#include "asslauncher.hpp"
#include "config.h"

using namespace std;
namespace py = pybind11;

void printHelp(char **argv);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printHelp(argv);
        return 1;
    }
    
    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
        printHelp(argv);
        return 0;
    }
    
    py::scoped_interpreter guard{};
    
    string jsonFileName(argv[1]);
    SubFXAssInit *assConfig = new (nothrow) SubFXAssInit(jsonFileName);
    if (!assConfig)
    {
        cerr << "Fail to allocate memory" << endl;
        return 1;
    }
    
    if (!assConfig->isSuccess())
    {
        cerr << assConfig->getLastError() << endl;
        delete assConfig;
        return 1;
    }
    
    AssLauncher *assLauncher = new (nothrow) AssLauncher(assConfig);
    if (!assLauncher)
    {
        cerr << "Fail to allocate memory" << endl;
        return 1;
    }
    
    if (!assLauncher->isSuccess())
    {
        cerr << assLauncher->getLastError() << endl;
        delete assLauncher;
        delete assConfig;
        return 1;
    }
    
    assLauncher->exec(assConfig);
    if (!assLauncher->isSuccess())
    {
        cerr << assLauncher->getLastError() << endl;
        delete assLauncher;
        delete assConfig;
        return 1;
    }
    
    delete assLauncher;
    delete assConfig;
    return 0;
}

void printHelp(char **argv)
{
    cout << argv[0] << " " << SUBFX_VERSION << " usage:" << endl;
    cout << argv[0] << " /path/to/your/configFile.json" << endl;
    cout << "\"-h\" or \"--help\": print this message and exit." << endl;
}
