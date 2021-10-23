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

#include "misc.h"
#include "common.h"

subfx_exitstate subfx_misc_init(subfx_misc_api *misc)
{
    if (!misc)
    {
        return subfx_failed;
    }

    misc->doubleToString = subfx_misc_doubleToString;
    misc->getLine = subfx_misc_getLine;

    return subfx_success;
}

char *subfx_misc_doubleToString(double input)
{
    double *buffer = calloc(500, sizeof(double));
    if (!buffer)
    {
        return NULL;
    }

    sprintf((char *)(buffer), "%.3lf", input);

    return (char *)(buffer);
}

subfx_exitstate
subfx_misc_getLine(char *buffer,
                   size_t bufferSize,
                   FILE *file,
                   char *errMsg)
{
    if (!file || !buffer || bufferSize <= 0)
    {
        subfx_pError(errMsg, "Invalid input.");
        return subfx_failed;
    }

    // note that "newline" just have two situations
    // crlf(\r\n) or lf(\n)
    if (fgets(buffer, (int)bufferSize, file))
    {
        size_t length = strlen(buffer);
        // for safety
        if (length > 0)
        {
            // buffer[length] == '\0'
            // buffer[length - 1] == '\n'
            if (buffer[length - 2] == '\r') // crlf
            {
                buffer[length - 2] = '\0';
            }
            else // lf
            {
                buffer[length - 1] = '\0';
            }
        }
        else
        {
            buffer[0] = '\0';
            subfx_pError(errMsg, "The result is an empty string.");
            return subfx_successWithWarning;
        }
    }
    else
    {
        if (feof(file))
        {
            buffer[0] = '\0';
            return subfx_eof;
        }
        else
        {
            subfx_pError(errMsg, "Fail to get line.");
            return subfx_failed;
        }
    }

    return subfx_success;
}
