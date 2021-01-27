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

#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "types.h"

subfx_logger
*subfx_logger_init()
{
    subfx_logger *logger = calloc(1, sizeof(subfx_logger));
    if (!logger)
    {
        return NULL;
    }

    logger->create = subfx_logger_create;
    logger->create2 = subfx_logger_create2;
    logger->writeOut = subfx_logger_writeOut;
    logger->writeErr = subfx_logger_writeErr;

    return logger;
}

subfx_handle
subfx_logger_create(FILE *out,
                    FILE *err,
                    bool autoCloseFiles)
{
    if (!out || !err)
    {
        return NULL;
    }

    return subfx_logger_createInternal(out,
                                       err,
                                       autoCloseFiles,
                                       false);
}

subfx_handle
subfx_logger_create2(const char *outFile,
                     const char *errFile)
{
    FILE *out = NULL;
    FILE *err = NULL;

    out = fopen(outFile, "a");
    if (!out)
    {
        return NULL;
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
            return NULL;
        }
    }

    return subfx_logger_createInternal(out, err, true, true);
}

subfx_exitstate subfx_logger_destroy(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_logger))
    {
        return subfx_failed;
    }

    Logger *logger = (Logger *)in;
    if (logger->haveToCloseFiles == true)
    {
        subfx_logger_closeFiles(logger->out, logger->err);
    }


    free(logger);
    return subfx_success;
}

subfx_exitstate subfx_logger_writeOut(subfx_handle in,
                                      const char *msg)
{
    if (subfx_checkInput(in, subfx_types_logger))
    {
        return subfx_failed;
    }

    Logger *logger = (Logger *)in;
    fprintf(logger->out, "%s", msg);

    return subfx_success;
}

subfx_exitstate subfx_logger_writeErr(subfx_handle in,
                                      const char *msg)
{
    if (subfx_checkInput(in, subfx_types_logger))
    {
        return subfx_failed;
    }

    Logger *logger = (Logger *)in;
    fprintf(logger->err, "%s", msg);

    return subfx_success;
}

subfx_handle subfx_logger_createInternal(FILE *out,
                                         FILE *err,
                                         bool autoCloseFiles,
                                         bool haveToCloseFiles)
{
    Logger *ret = calloc(1, sizeof(Logger));
    if (!ret)
    {
        if (haveToCloseFiles == true)
        {
            subfx_logger_closeFiles(out, err);
        }

        return NULL;
    }

    ret->out = out;
    ret->err = err;
    ret->haveToCloseFiles = autoCloseFiles;

    return ret;
}

void subfx_logger_closeFiles(FILE *out, FILE *err)
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
    }
}
