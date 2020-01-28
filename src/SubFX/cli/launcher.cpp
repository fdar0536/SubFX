#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <iostream>

#include <cstdio>

#include "launcher.hpp"

Launcher::~Launcher()
{
    if (logFile != nullptr && logFile != stderr)
    {
        fclose(logFile);
    }
}

// protected member function
const char *Launcher::LauncherInit(const std::string &logFile,
                                   const std::string &outputFile)
{
    if (logFile == "")
    {
        this->logFile = stderr;
    }
    else
    {
        this->logFile = fopen(logFile.c_str(), "a");
        if (!this->logFile)
        {
            return "Cannot open log file.";
        }
    }

    const char *err(nullptr);
    remove(outputFile.c_str());
    std::tie(this->file, err) = YFile::create(outputFile);
    if (err)
    {
        return err;
    }

    return nullptr;
}
