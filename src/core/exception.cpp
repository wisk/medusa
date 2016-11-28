#include "medusa/exception.hpp"

#ifdef WIN32

#include <Windows.h>

MEDUSA_NAMESPACE_BEGIN

std::string Exception_System::ErrorCodeToMessage(void)
{
  HLOCAL hLocal = nullptr;

  if (FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    nullptr,
    GetLastError(),
    0x0,
    (LPSTR)&hLocal,
    0x0,
    nullptr
    ) == 0x0)
    return "Unable to retrieve the exception message";

  std::string Res = (LPCSTR)LocalLock(hLocal);
  LocalFree(hLocal);
  return Res;
}

MEDUSA_NAMESPACE_END

#else

# include <cstdlib>
# include <errno.h>
# include <cstring>

MEDUSA_NAMESPACE_BEGIN

std::string Exception_System::ErrorCodeToMessage(void)
{
  return strerror(errno);
}

MEDUSA_NAMESPACE_END

#endif
