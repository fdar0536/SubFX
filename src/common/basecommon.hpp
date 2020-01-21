#ifndef BASECOMMON_HPP
#define BASECOMMON_HPP

#include <memory>

#if defined _WIN32 || defined __CYGWIN__
#ifdef __MINGW32__
#define SYMBOL_SHOW __attribute__ ((dllexport))
#else
#define SYMBOL_SHOW __declspec(dllexport)
#endif // __MINGW32__
#else
#define SYMBOL_SHOW __attribute__((visibility("default")))
#endif

#define TESTERR(x) if (x) \
                   { \
                       return x;\
                   }

#endif // BASECOMMON_HPP
