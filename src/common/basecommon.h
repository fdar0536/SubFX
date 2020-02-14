#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif    /* _MSC_VER */

#pragma once

#if defined _WIN32 || defined __CYGWIN__
#ifdef __MINGW32__
#define SYMBOL_SHOW __attribute__ ((dllexport))
#else
#define SYMBOL_SHOW __declspec(dllexport)
#endif // __MINGW32__
#else
#define SYMBOL_SHOW __attribute__((visibility("default")))
#endif

#define TESTERR(x) \
    if (x) \
    { \
        return x;\
    }

#define PRINT_ERR_MSG(x, y) \
    if (x) \
    { \
        sprintf(x, "%s", y); \
    }

