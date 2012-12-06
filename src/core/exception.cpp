#include "medusa/exception.hpp"

#ifdef WIN32
# include <Windows.h>

/* Silly windows.h ... */
#undef max
#undef min

MEDUSA_NAMESPACE_BEGIN

std::wstring Exception_System::ErrorCodeToMessage(void)
{
  HLOCAL hLocal = NULL;

  if (FormatMessageW(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    NULL,
    GetLastError(),
    0x0,
    (LPWSTR)&hLocal,
    0x0,
    NULL
    ) == 0x0)
    return L"Unable to retrieve the exception message";

  std::wstring Res = (LPCWSTR)LocalLock(hLocal);
  LocalFree(hLocal);
  return Res;
}

MEDUSA_NAMESPACE_END

#else

# include <cstdlib>
# include <errno.h>
# include <cstring>

MEDUSA_NAMESPACE_BEGIN

std::wstring Exception_System::ErrorCodeToMessage(void)
{
  wchar_t*  pErrorMsgUtf16;
  char*     pErrorMsgUtf8;
  size_t    ErrorMsgLen;


  pErrorMsgUtf8 = strerror(errno);
  ErrorMsgLen = strlen(pErrorMsgUtf8);

  pErrorMsgUtf16 = new wchar_t[ErrorMsgLen + 1];
  mbstowcs(pErrorMsgUtf16, pErrorMsgUtf8, ErrorMsgLen);

  std::wstring Res = pErrorMsgUtf16;
  delete[] pErrorMsgUtf16;
  return Res;
}

MEDUSA_NAMESPACE_END

#endif
