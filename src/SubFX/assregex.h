#pragma once

#define REGEX_COUNT 4

#define REGEX_STRING_TO_MS 0
#define REGEX_STR_STRING_TO_MS \
    "^\\d:\\d\\d:\\d\\d\\.\\d\\d$"

#define REGEX_APLHA_ONLY 1
#define REGEX_STR_APLHA_ONLY \
    "^&[Hh]{1}[0-9a-fA-F]{2}&$"

#define REGEX_ASS_COLOR 2
#define REGEX_STR_ASS_COLOR \
    "^&[Hh]{1}[0-9a-fA-F]{6}&$"

#define REGEX_ASS_ALPHA_COLOR 3
#define REGEX_STR_ASS_ALPHA_COLOR \
    "^&[Hh]{1}[0-9a-fA-F]{8}$"
