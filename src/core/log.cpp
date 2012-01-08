#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

LogWrapper::MutexType      LogWrapper::m_Mutex;
Log::LogMap                Log::m_LogMap;
LogWrapper::LoggerCallback Log::m_pLog = NULL;

LogWrapper& LogFlush(LogWrapper &rLogWrapper)
{
  rLogWrapper.Flush();
  return rLogWrapper;
}

LogWrapper& LogEnd(LogWrapper &rLogWrapper)
{
  rLogWrapper.Lock();
  rLogWrapper.Write(L"\n");
  LogFlush(rLogWrapper);
  rLogWrapper.Unlock();
  return rLogWrapper;
}

std::wstring LogWrapper::StringToWString(std::string const& rString)
{
  wchar_t *pWcs = new wchar_t[rString.length() + 1];
  std::wstring Result;

  if (mbstowcs(pWcs, rString.c_str(), rString.length()) == -1)
    return L"Unknown string";

  pWcs[rString.length()] = L'\0';

  Result = pWcs;

  delete[] pWcs;

  return Result;
}

LogWrapper Log::Write(std::string const& rType)
{
  return LogWrapper(Log::m_pLog, rType, Log::m_LogMap[rType]);
}

MEDUSA_NAMESPACE_END