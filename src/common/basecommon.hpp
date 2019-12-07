#ifndef BASECOMMON_HPP
#define BASECOMMON_HPP

#if defined _WIN32 || defined __CYGWIN__
#ifdef __MINGW32__
#define SYMBOL_SHOW __attribute__ ((dllexport))
#else
#define SYMBOL_SHOW __declspec(dllexport)
#endif // __MINGW32__
#else
#define SYMBOL_SHOW __attribute__((visibility("default")))
#endif

#endif // BASECOMMON_HPP
