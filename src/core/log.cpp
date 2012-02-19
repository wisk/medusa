#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

LogWrapper::MutexType      LogWrapper::m_Mutex;
Log::LogMap                Log::m_LogMap;
LogWrapper::LoggerCallback Log::m_pLog = NULL;

template<> LogWrapper& LogWrapper::operator<<(s16 Value)
{
  std::wostringstream oss;

  oss << std::hex << std::internal << std::showbase << std::setfill(L'0') << std::setw(sizeof(Value) * 2 + 2) << static_cast<s32>(Value);
  m_rBuffer += oss.str();
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(u16 Value)
{
  std::wostringstream oss;

  oss << std::hex << std::internal << std::showbase << std::setfill(L'0') << std::setw(sizeof(Value) * 2 + 2) << static_cast<u32>(Value);
  m_rBuffer += oss.str();
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(Address Addr)
{
  boost::recursive_mutex::scoped_lock(m_Mutex);
  m_rBuffer += StringToWString(Addr.ToString());
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(std::string Msg)
{
  boost::recursive_mutex::scoped_lock(m_Mutex);
  m_rBuffer += StringToWString(Msg);
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(std::wstring Msg)
{
  boost::recursive_mutex::scoped_lock(m_Mutex);
  m_rBuffer += (Msg);
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(LogWrapper::LoggerFunction pFunc)
{
  return pFunc(*this);
}

LogWrapper& LogFlush(LogWrapper &rLogWrapper)
{
  rLogWrapper.Flush();
  return rLogWrapper;
}

//XXX: We should probably put a lock here, but it looks to deadlock the whole thing :(
LogWrapper& LogEnd(LogWrapper &rLogWrapper)
{
  //rLogWrapper.Lock();
  rLogWrapper.Write(L"\n");
  LogFlush(rLogWrapper);
  //rLogWrapper.Unlock();
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