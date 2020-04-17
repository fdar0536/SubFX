#pragma once

#include <string>
#include <memory>

#include <cstdio>

#include "config.h"
#include "../basecommon.h"

namespace PROJ_NAMESPACE
{

namespace Utils
{

class Logger
{
public:

    static std::shared_ptr<Logger> create(FILE *out = stdout,
                                          FILE *err = stderr,
                                          bool autoCloseFiles = false);

    static std::shared_ptr<Logger> create(std::string &outFile,
                                          std::string &errFile);

    ~Logger();

    void writeOut(const char *);

    void writeOut(std::string &);

    void writeErr(const char *);

    void writeErr(std::string &);

private:

    Logger() :
        m_out(nullptr),
        m_err(nullptr),
        m_haveToCloseFiles(false)
    {}

    static std::shared_ptr<Logger> createInternal(FILE *out,
                                                  FILE *err,
                                                  bool autoCloseFiles,
                                                  bool haveToClose);

    static void closeFiles(FILE *out, FILE *err);

    Logger(const Logger &) = delete;

    Logger& operator=(const Logger &other) = delete;

    Logger& operator=(Logger &&other) = delete;

    Logger& operator=(Logger other) = delete;

    FILE *m_out;

    FILE *m_err;

    bool m_haveToCloseFiles;

};

} // end namespace Utils

} // end namespace PROJ_NAMESPACE
