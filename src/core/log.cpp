#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

LogWrapper::MutexType      LogWrapper::m_Mutex;
Log::LogMap                Log::m_LogMap;
LogWrapper::LoggerCallback Log::m_pLog;

template<> LogWrapper& LogWrapper::operator<<(s8 Value)
{
  std::ostringstream oss;
  oss << std::hex << std::internal << "0x" << std::setfill('0') << std::setw(sizeof(Value) * 2) << static_cast<int>(Value);
  m_rBuffer += oss.str();

  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(u8 Value)
{
  std::ostringstream oss;
  oss << std::hex << std::internal << "0x" << std::setfill('0') << std::setw(sizeof(Value) * 2) << static_cast<int>(Value);
  m_rBuffer += oss.str();

  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(s16 Value)
{
  std::ostringstream oss;

  oss << std::hex << std::internal << std::showbase << std::setfill('0') << std::setw(sizeof(Value) * 2 + 2) << static_cast<s32>(Value);
  m_rBuffer += oss.str();
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(u16 Value)
{
  std::ostringstream oss;

  oss << std::hex << std::internal << std::showbase << std::setfill('0') << std::setw(sizeof(Value) * 2 + 2) << static_cast<u32>(Value);
  m_rBuffer += oss.str();
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(Address Addr)
{
  boost::recursive_mutex::scoped_lock(m_Mutex);
  m_rBuffer += Addr.ToString();
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(std::string Msg)
{
  boost::recursive_mutex::scoped_lock(m_Mutex);
  m_rBuffer += Msg;
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

LogWrapper& LogEnd(LogWrapper &rLogWrapper)
{
  rLogWrapper.Lock();
  rLogWrapper.Write("\n");
  LogFlush(rLogWrapper);
  rLogWrapper.Unlock();
  return rLogWrapper;
}

LogWrapper Log::Write(std::string const& rType)
{
  return LogWrapper(Log::m_pLog, rType, Log::m_LogMap[rType]);
}

MEDUSA_NAMESPACE_END
