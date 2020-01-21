#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <memory>
#include <cstdio>

#include "../../YutilsCpp/YutilsCpp.hpp"

typedef Yutils::File YFile;

class Launcher
{
public:

    virtual ~Launcher();

protected:

    Launcher() {}

    FILE *logFile;

    std::shared_ptr<YFile> file;

    const char *LauncherInit(const std::string &logFile,
                             const std::string &outputFile);
};

#endif // LAUNCHER_HPP
