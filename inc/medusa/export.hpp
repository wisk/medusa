#ifndef _CORE_EXPORT_
#define _CORE_EXPORT_

#if defined(_WIN32) || defined(WIN32)
#ifdef Medusa_EXPORTS
#  define Medusa_EXPORT __declspec(dllexport)
#else
#  define Medusa_EXPORT __declspec(dllimport)
#endif
#else
#define Medusa_EXPORT
#endif

#endif // _CORE_EXPORT_
