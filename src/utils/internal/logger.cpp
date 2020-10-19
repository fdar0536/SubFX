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

#include <cstring>

#include "logger.hpp"

Logger
*Logger::create(FILE *out,
                FILE *err,
                bool autoCloseFiles) NOTHROW
{
    if (!out || !err)
    {
        return nullptr;
    }

    return createInternal(out, err, autoCloseFiles, false);
}

Logger
*Logger::create(const char *outFile,
                const char *errFile) NOTHROW
{
    FILE *out(nullptr);
    FILE *err(nullptr);

    out = fopen(outFile, "a");
    if (!out)
    {
        return nullptr;
    }

    if (strcmp(outFile, errFile) == 0)
    {
        err = out;
    }
    else
    {
        err = fopen(errFile, "a");
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

void Logger::writeErr(const char *msg) NOTHROW
{
    fprintf(m_err, "%s", msg);
}

// private member functions
Logger
*Logger::createInternal(FILE *out,
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

    return ret;
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
