#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#include <iostream>

#include "launcher.hpp"

Launcher::Launcher(const std::string &logFile,
                   const std::string &outputFile) :
    success(false),
    lastError("")
{
    LauncherInit(logFile, outputFile);
}

Launcher::~Launcher()
{
    if (logFile != nullptr && logFile != stderr)
    {
        fclose(logFile);
    }
}

bool Launcher::isSuccess() const
{
    return success;
}

std::string Launcher::getLastError() const
{
    return lastError;
}

// private member function
void Launcher::LauncherInit(const std::string &logFile,
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
            lastError = "Cannot open log file.";
            success = false;
            return;
        }
    }

    try
    {
        this->file = std::make_shared<YFile>(outputFile);
    }
    catch (std::invalid_argument &)
    {
        lastError = "Cannot open output subtitle file.";
        success = false;
        return;
    }

    if (this->file == nullptr)
    {
        lastError = "Fail to allocate memory.";
        success = false;
        return;
    }

    success = true;
}
