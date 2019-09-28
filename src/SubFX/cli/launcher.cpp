#include <iostream>
#include <new>
#include <string>

#include <cstring>

#include "config.h"

#include "launcher.hpp"

using namespace std;

Launcher::Launcher(int argc, char **argv) :
    argc(argc),
    argv(argv),
    configParser(nullptr)
{}

int Launcher::exec()
{
    if (argc != 2)
    {
        printHelp();
        return 1;
    }
    
    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
        printHelp();
        return 0;
    }
    
    string jsonFileName(argv[1]);
    configParser = new (nothrow) ConfigParser(jsonFileName);
    if (!configParser)
    {
        cerr << "Fail to allocate memory" << endl;
        return 1;
    }
    
    if (!configParser->isSuccess())
    {
        delete configParser;
        cerr << configParser->getLastError() << endl;
        return 1;
    }
    
    delete configParser;
    return 0;
}

// private member function
void Launcher::printHelp()
{
    cout << argv[0] << " " << SUBFX_VERSION << " usage:" << endl;
    cout << argv[0] << " /path/to/your/configFile.json" << endl;
    cout << "\"-h\" or \"--help\": print this message and exit." << endl;
}
