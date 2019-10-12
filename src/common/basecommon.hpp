#ifndef BASECOMMON_HPP
#define BASECOMMON_HPP

#ifdef _WIN32
#ifdef __MINGW32__
#define SYMBOL_SHOW __attribute__((visibility("default")))
#define SYMBOL_HIDE __attribute__((visibility("hidden")))
#else
#define SYMBOL_SHOW
#define SYMBOL_HIDE
#endif // __MINGW32__
#else
#define SYMBOL_SHOW __attribute__((visibility("default")))
#define SYMBOL_HIDE __attribute__((visibility("hidden")))
#endif

#endif // BASECOMMON_HPP
