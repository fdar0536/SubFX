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

#pragma once

#include <stdbool.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Regex
{
    pcre2_code *regex;
    pcre2_match_data *matchData;
    pcre2_match_context *matchContext;
    pcre2_jit_stack *jitStack;
} Regex;

uint8_t RegexData_init(Regex *, const char *);

void RegexData_fin(Regex *);

bool RegexData_match(Regex *, const char *, int *);

#ifdef __cplusplus
}
#endif
