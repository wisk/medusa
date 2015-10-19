#ifndef MEDUSA_EXPORT
#define MEDUSA_EXPORT

#if defined(_WIN32) || defined(WIN32)
# ifdef Medusa_EXPORTS
#  define Medusa_EXPORT __declspec(dllexport)
# else
#  define Medusa_EXPORT __declspec(dllimport)
# endif // !WINDOWS
# if defined(__GNUC__)
#  define Medusa_EXPORT __attribute__((visibility("default")))
# endif // !__GNUC__
#else
# define Medusa_EXPORT
#endif

#endif // !MEDUSA_EXPORT
