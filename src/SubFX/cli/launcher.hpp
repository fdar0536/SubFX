#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include "../common/configparser.hpp"

class Launcher
{
public:
    
    Launcher(int argc, char **argv);
    
    int exec();
    
private:
    
    int argc;
    
    char **argv;
    
    ConfigParser *configParser;
    
    void printHelp();
};

#endif // LAUNCHER_HPP
