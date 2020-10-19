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

#pragma once

#include <string>

#include <cstdio>

#include "internal/basecommon.h"

class SYMBOL_SHOW Logger
{
public:

    static Logger *create(FILE *out = stdout,
                          FILE *err = stderr,
                          bool autoCloseFiles = true) NOTHROW;

    static Logger *create(const char *outFile,
                          const char *errFile) NOTHROW;

    ~Logger();

    void writeOut(const char *) NOTHROW;

    void writeErr(const char *) NOTHROW;

private:

    Logger() :
        m_out(nullptr),
        m_err(nullptr),
        m_haveToCloseFiles(false)
    {}

    static Logger *createInternal(FILE *out,
                                  FILE *err,
                                  bool autoCloseFiles,
                                  bool haveToClose);

    static void closeFiles(FILE *out, FILE *err);

    Logger(const Logger &) = delete;

    Logger& operator=(const Logger &other) = delete;

    Logger& operator=(Logger &&other) = delete;

    FILE *m_out;

    FILE *m_err;

    bool m_haveToCloseFiles;

};
