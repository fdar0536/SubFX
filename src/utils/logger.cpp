#include <new>

#include "utils.hpp"

namespace PROJ_NAMESPACE
{

namespace Utils
{

std::shared_ptr<Logger>
Logger::create(FILE *out,
               FILE *err,
               bool autoCloseFiles)
{
    if (!out || !err)
    {
        return nullptr;
    }

    return createInternal(out, err, autoCloseFiles, false);
}

std::shared_ptr<Logger>
Logger::create(std::string &outFile,
               std::string &errFile)
{
    FILE *out(nullptr);
    FILE *err(nullptr);

    out = fopen(outFile.c_str(), "a");
    if (!out)
    {
        return nullptr;
    }

    if (outFile == errFile)
    {
        err = out;
    }
    else
    {
        err = fopen(errFile.c_str(), "a");
        if (!err)
        {
            fclose(out);
            return nullptr;
        }
    }

    return createInternal(out, err, true, true);
}

Logger::~Logger()
{
    if (m_haveToCloseFiles)
    {
        closeFiles(m_out, m_err);
    }
}

void Logger::writeOut(const char *msg)
{
    fprintf(m_out, "%s", msg);
}

void Logger::writeOut(std::string &msg)
{
    return writeOut(msg.c_str());
}

void Logger::writeErr(const char *msg)
{
    fprintf(m_err, "%s", msg);
}

void Logger::writeErr(std::string &msg)
{
    return writeErr(msg.c_str());
}

// private member functions
std::shared_ptr<Logger>
Logger::createInternal(FILE *out,
                       FILE *err,
                       bool autoCloseFiles,
                       bool haveToCloseFiles)
{
    Logger *ret(new(std::nothrow) Logger());
    if (!ret)
    {
        if (haveToCloseFiles)
        {
            closeFiles(out, err);
        }

        return nullptr;
    }

    ret->m_out = out;
    ret->m_err = err;
    ret->m_haveToCloseFiles = autoCloseFiles;

    return std::shared_ptr<Logger>(ret);
}

void Logger::closeFiles(FILE *out, FILE *err)
{
    if (out != stdout &&
        out != stderr &&
        err != stdout &&
        err != stderr)
    {
        if (out == err)
        {
            fclose(out);
        }
        else
        {
            fclose(out);
            fclose(err);
        }

        out = nullptr;
        err = nullptr;
    }
}

} // end namespace Utils

} // end namespace PROJ_NAMESPACE
