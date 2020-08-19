#include <iostream>

#include <cstring>

#include "pybind11/embed.h"
#include "asslauncher.hpp"
#include "config.h"

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

    std::string jsonFileName(argv[1]);
    std::shared_ptr<ConfigParser> assConfig;
    try
    {
        assConfig = ConfigParser::create(jsonFileName);
    }
    catch (std::invalid_argument &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    std::shared_ptr<AssLauncher> assLauncher(AssLauncher::create());
    int ret(assLauncher->exec(assConfig));
    if (ret)
    {
        std::cerr << "Something error happened. Please see log for details.";
        std::cerr << std::endl;
    }

    return ret;
}

void printHelp(char **argv)
{
    std::cout << argv[0] << " " << PROJ_VERSION << " usage:" << std::endl;
    std::cout << argv[0] << " /path/to/your/configFile.json" << std::endl;
    std::cout << "\"-h\" or \"--help\": print this message and exit." << std::endl;
}
