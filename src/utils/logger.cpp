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

#include "SubFX.h"
#include "internal/logger.hpp"

extern "C"
{

SYMBOL_SHOW
void *subfx_utils_logger_create(FILE *out,
                                FILE *err,
                                int autoCloseFiles)
{
    Logger *ret(Logger::create(out, err, autoCloseFiles));
    if (!ret)
    {
        return nullptr;
    }

    return reinterpret_cast<void *>(ret);
}

SYMBOL_SHOW
void *subfx_utils_logger_create2(const char *outFile,
                                 const char *errFile)
{
    Logger *ret(Logger::create(outFile, errFile));
    if (!ret)
    {
        return nullptr;
    }

    return reinterpret_cast<void *>(ret);
}

SYMBOL_SHOW
void subfx_utils_logger_destory(subfx_utils_logger *input)
{
    if (!input)
    {
        return;
    }

    Logger *logger(reinterpret_cast<Logger *>(input));
    delete logger;
    input = nullptr;
}

SYMBOL_SHOW
void subfx_utils_logger_writeOut(subfx_utils_logger *input,
                                 const char *msg)
{
    if (!input)
    {
        return;
    }

    Logger *logger(reinterpret_cast<Logger *>(input));
    logger->writeOut(msg);
}

SYMBOL_SHOW
void subfx_utils_logger_writeErr(subfx_utils_logger *input,
                                 const char *msg)
{
    if (!input)
    {
        return;
    }

    Logger *logger(reinterpret_cast<Logger *>(input));
    logger->writeErr(msg);
}

} // end extern "C"
