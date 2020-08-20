/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#include <new>

#include "Utils"

namespace PROJ_NAMESPACE
{

namespace Utils
{

std::shared_ptr<Logger>
Logger::create(FILE *out,
               FILE *err,
               bool autoCloseFiles) NOTHROW
{
    if (!out || !err)
    {
        return nullptr;
    }

    return createInternal(out, err, autoCloseFiles, false);
}

std::shared_ptr<Logger>
Logger::create(const std::string &outFile,
               const std::string &errFile) NOTHROW
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

void Logger::writeOut(const char *msg) NOTHROW
{
    fprintf(m_out, "%s", msg);
}

void Logger::writeOut(std::string &msg) NOTHROW
{
    return writeOut(msg.c_str());
}

void Logger::writeErr(const char *msg) NOTHROW
{
    fprintf(m_err, "%s", msg);
}

void Logger::writeErr(std::string &msg) NOTHROW
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
