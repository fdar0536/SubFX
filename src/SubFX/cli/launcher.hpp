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

    bool isSuccess() const;

    std::string getLastError() const;

protected:

    Launcher(const std::string &logFile,
             const std::string &outputFile);

    bool success;

    std::string lastError;

    FILE *logFile;

    std::shared_ptr<YFile> file;

private:

    void LauncherInit(const std::string &logFile,
                      const std::string &outputFile);
};

#endif // LAUNCHER_HPP
