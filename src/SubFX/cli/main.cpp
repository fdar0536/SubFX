#include <iostream>
#include <new>

#include "../common/configparser.hpp"
#include "config.h"

using namespace std;

void printHelp(char **);

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printHelp(argv);
        return 0;
    }
    /* TODO */
    return 0;
}

void printHelp(char **argv)
{
    cout << argv[0] << " " << SUBFX_VERSION << " usage:" << endl;
    /* TODO */
}
