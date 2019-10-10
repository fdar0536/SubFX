#include <iostream>

#include <cstring>

#include "../common/subfxinit.hpp"
#include "config.h"

using namespace std;

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
    
    string jsonFileName(argv[1]);
    SubFXInit *config = new (nothrow) SubFXInit(jsonFileName);
    if (!config)
    {
        cerr << "Fail to allocate memory" << endl;
        return 1;
    }
    
    if (!config->isSuccess())
    {
        cerr << config->getLastError() << endl;
        delete config;
        return 1;
    }
    
    delete config;
    return 0;
}

void printHelp(char **argv)
{
    cout << argv[0] << " " << SUBFX_VERSION << " usage:" << endl;
    cout << argv[0] << " /path/to/your/configFile.json" << endl;
    cout << "\"-h\" or \"--help\": print this message and exit." << endl;
}
