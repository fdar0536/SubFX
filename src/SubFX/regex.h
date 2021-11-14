#pragma once

#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct RegexData
{
    pcre2_code *regex;
    pcre2_match_data *matchData;
    pcre2_match_context *matchContext;
    pcre2_jit_stack *jitStack;
} RegexData;

uint8_t RegexData_init(RegexData *, const char *);

void RegexData_fin(RegexData *);

uint8_t RegexData_match(RegexData *, const char *, int *);

#ifdef __cplusplus
}
#endif
