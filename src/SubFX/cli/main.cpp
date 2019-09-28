#include "launcher.hpp"

int main(int argc, char **argv)
{
    Launcher launcher(argc, argv);
    
    return launcher.exec();
}
