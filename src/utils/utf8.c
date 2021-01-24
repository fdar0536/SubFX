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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal/common.h"
#include "ptrvector.h"
#include "utf8.h"

subfx_utils_utf8 *subfx_utils_utf8_init()
{
    subfx_utils_utf8 *utf8 = calloc(1, sizeof(subfx_utils_utf8));
    if (!utf8)
    {
        return NULL;
    }

    utf8->stringLen = subfx_utils_utf8_stringLen;
    utf8->stringSplit = subfx_utils_utf8_stringSplit;

    return utf8;
}

uint32_t subfx_utils_utf8_stringLen(const char *utf8str)
{
    uint32_t len = 0;
    size_t strsize = strlen(utf8str);
    size_t i;
    for (i = 0; i < strsize; ++i)
    {
        len += (((uint8_t)utf8str[i] & 0xc0) != 0x80);
    }

    return len;
} // end subfx_utils_utf8_stringLen

subfx_handle subfx_utils_utf8_stringSplit(const char *utf8str,
                                          char *msg)
{
    subfx_handle ret = subfx_ptrVector_create(free);
    if (!ret)
    {
        return NULL;
    }

    if (subfx_ptrVector_reserve(ret, 20) == subfx_failed)
    {
        if (subfx_ptrVector_destroy(ret) == subfx_failed)
        {
            subfx_pError(msg, "utf8->stringSplit: Fail to destroy handle")
        }

        return NULL;
    }

    size_t length = strlen(utf8str);
    size_t i;
    char *str = NULL;
    for(i = 0; i < length;)
    {
        size_t len = 1;
        if(((uint8_t)utf8str[i] & 0xf8) == 0xf0)
            len = 4;
        else if(((uint8_t)utf8str[i] & 0xf0) == 0xe0)
            len = 3;
        else if(((uint8_t)utf8str[i] & 0xe0) == 0xc0)
            len = 2;
        if((i + len) > length)
            len = 1;

        //ret.push_back(input.substr(i, len));
        str = calloc(len + 1, sizeof(char));
        if (!str)
        {
            if (subfx_ptrVector_destroy(ret) == subfx_failed)
            {
                subfx_pError(msg, "utf8->stringSplit: Fail to destroy handle")
            }

            return NULL;
        }

        strncpy(str, utf8str + i, len);
        str[len] = '\0';

        if (subfx_ptrVector_pushback(ret, str) == subfx_failed)
        {
            if (subfx_ptrVector_destroy(ret) == subfx_failed)
            {
                subfx_pError(msg, "utf8->stringSplit: Fail to destroy handle")
            }

            return NULL;
        }

        i += len;
    }

    return ret;
} // end subfx_utils_utf8_stringSplit