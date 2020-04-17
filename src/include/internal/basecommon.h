#ifdef _MSC_VER
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)
#endif // _MSC_VER

#pragma once

#ifdef _WIN32
#define NOMINMAX
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef __MINGW32__
#define SYMBOL_SHOW __attribute__ ((dllexport))
#else
#define SYMBOL_SHOW __declspec(dllexport)
#endif // __MINGW32__
#else
#define SYMBOL_SHOW __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
#define THROW noexcept(false)
#define NOTHROW noexcept
#define UNUSED(x) static_cast<void>(x)
#else
#define THROW
#define NOTHROW
#define UNUSED(x) (void)x
#endif
