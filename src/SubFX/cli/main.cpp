#include <iostream>

#include <cstring>

#include "pybind11/embed.h"
#include "asslauncher.hpp"
#include "config.h"

namespace py = pybind11;

void printHelp(char **argv);

void cleanUp(AssLauncher *, SubFXAssInit *);

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

    std::string jsonFileName(argv[1]);
    SubFXAssInit *assConfig = new (std::nothrow) SubFXAssInit(jsonFileName);
    if (!assConfig)
    {
        std::cerr << "Fail to allocate memory" << std::endl;
        return 1;
    }

    if (!assConfig->isSuccess())
    {
        std::cerr << assConfig->getLastError() << std::endl;
        cleanUp(nullptr, assConfig);
        return 1;
    }

    AssLauncher *assLauncher = new (std::nothrow) AssLauncher(assConfig);
    if (!assLauncher)
    {
        std::cerr << "Fail to allocate memory" << std::endl;
        cleanUp(nullptr, assConfig);
        return 1;
    }

    if (!assLauncher->isSuccess())
    {
        std::cerr << assLauncher->getLastError() << std::endl;
        cleanUp(assLauncher, assConfig);
        return 1;
    }

    int ret(assLauncher->exec(assConfig));
    if (ret)
    {
        std::cerr << "Something error happened. Please see log for details.";
        std::cerr << std::endl;
    }

    cleanUp(assLauncher, assConfig);
    return ret;
}

void printHelp(char **argv)
{
    std::cout << argv[0] << " " << SUBFX_VERSION << " usage:" << std::endl;
    std::cout << argv[0] << " /path/to/your/configFile.json" << std::endl;
    std::cout << "\"-h\" or \"--help\": print this message and exit." << std::endl;
}

void cleanUp(AssLauncher *input1, SubFXAssInit *input2)
{
    if (input1)
    {
        delete input1;
    }

    if (input2)
    {
        delete input2;
    }
}
