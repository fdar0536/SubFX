/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2021 fdar0536.
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
#include <string.h>

#include "regex.h"
#include "common.h"

uint8_t RegexData_init(Regex *in, const char *pattern)
{
    if (!in || !pattern) return 1;

    int errorCode;
    PCRE2_SIZE offset;
    PCRE2_UCHAR buffer[256];
    in->regex = pcre2_compile((const unsigned char *)pattern,
                              PCRE2_ZERO_TERMINATED,
                              PCRE2_UTF,
                              &errorCode,
                              &offset,
                              NULL);

    if (!in->regex)
    {

        pcre2_get_error_message(errorCode,
                                buffer,
                                sizeof(buffer));
        printf("%s\n", pattern);
        printf("PCRE2 compilation failed at offset %d: %s\n",
               (int)offset,
               buffer);
        return 1;
    }

    errorCode = pcre2_jit_compile(in->regex, PCRE2_JIT_COMPLETE);
    if (errorCode)
    {
        pcre2_get_error_message(errorCode,
                                buffer,
                                sizeof(buffer));
        printf("PCRE2 compilation failed: %s\n",
               buffer);
        return 1;
    }

    in->matchContext = pcre2_match_context_create(NULL);
    if (!in->matchContext)
    {
        puts("Fail to allocate match context.");
        return 1;
    }

    in->jitStack = pcre2_jit_stack_create(32 * 1024,
                                          1024 * 1024,
                                          NULL);
    if (!in->jitStack)
    {
        puts("Fail to allocate stack for jit.");
        return 1;
    }

    pcre2_jit_stack_assign(in->matchContext, NULL, in->jitStack);

    in->matchData = pcre2_match_data_create(128, NULL);
    if(!in->matchData)
    {
        puts("Fail to allocate match data.");
        return 1;
    }

    return 0;
}

void RegexData_fin(Regex *in)
{
    if (!in) return;
    if (in->regex) pcre2_code_free(in->regex);
    if (in->matchData) pcre2_match_data_free(in->matchData);
    if (in->matchContext) pcre2_match_context_free(in->matchContext);
    if (in->jitStack) pcre2_jit_stack_free(in->jitStack);
}

bool RegexData_match(Regex *data,
                     const char *in,
                     int *ret)
{
    if (!data || !in || !ret)
    {
        return false;
    }

    *ret = pcre2_jit_match(data->regex,
                           (const unsigned char *)in,
                           strlen(in),
                           0,
                           0,
                           data->matchData,
                           data->matchContext);

    PCRE2_UCHAR errBuf[256];
    if (*ret < 0 && *ret == PCRE2_ERROR_NOMATCH)
    {
        return false;
    }
    else if (*ret == 0)
    {
        puts("RegexData_match: No enough match data buffer");
        return false;
    }
    else if (*ret < 0 && *ret != PCRE2_ERROR_NOMATCH)
    {
        pcre2_get_error_message(*ret, errBuf, sizeof(errBuf));
        puts((char *)errBuf);
        return false;
    }

    return true;
}

