#include "medusa/log.hpp"
#include "medusa/user_configuration.hpp"

MEDUSA_NAMESPACE_BEGIN

LogWrapper::MutexType      LogWrapper::m_Mutex;
Log::LogMap                Log::m_LogMap;
LogWrapper::LoggerCallback Log::m_pLog;

LogWrapper::LogWrapper(LoggerCallback pLog, std::string const& rName, std::string& rBuffer)
: m_pLog(pLog)
, m_Name(rName)
, m_rBuffer(rBuffer)
, m_Level(LogDefault)
{
  if (m_rBuffer.empty())
  {
    m_rBuffer = rName;
    m_rBuffer += ": ";
  }
}

LogWrapper& LogWrapper::Flush(void)
{
  if (m_pLog)
  {
    boost::recursive_mutex::scoped_lock(m_Mutex);
    m_pLog(m_rBuffer);
    m_rBuffer = m_Name;
    m_rBuffer += ": ";
  }
  return *this;
}

LogWrapper& LogWrapper::Level(LogLevelType NewLvl)
{
  m_Level = NewLvl;
  return *this;
}

std::string& LogWrapper::GetBuffer(void)
{
  return m_rBuffer;
}

void LogWrapper::Write(char const* pMsg)
{
  m_rBuffer += pMsg;
}

void LogWrapper::Lock(void)
{
  m_Mutex.lock();
}

void LogWrapper::Unlock(void)
{
  m_Mutex.unlock();
}

bool LogWrapper::_CheckLogLevel(void) const
{
  static std::unordered_map<std::string, LogLevelType> Str2LogLvl
  {
    { "warning", LogWarning },
    { "error",   LogError   },
    { "info",    LogInfo    },
    { "debug",   LogDebug   },
  };

  UserConfiguration UsrCfg;
  LogLevelType UsrLvl = LogDefault;
  auto StrLogLvl = UsrCfg.GetOption("core.log_level");
  auto itLogLvl = Str2LogLvl.find(StrLogLvl);
  if (itLogLvl == std::end(Str2LogLvl))
    UsrCfg.SetOption("core.log_level", "default");
  if (itLogLvl != std::end(Str2LogLvl))
    UsrLvl = itLogLvl->second;

  return UsrLvl >= m_Level;
}

template<> LogWrapper& LogWrapper::operator<<(s8 Value)
{
  if (_CheckLogLevel())
  {
    std::ostringstream oss;
    oss << std::hex << std::internal << "0x" << std::setfill('0') << std::setw(sizeof(Value)* 2) << static_cast<int>(Value);
    m_rBuffer += oss.str();
  }
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(u8 Value)
{
  if (_CheckLogLevel())
  {
    std::ostringstream oss;
    oss << std::hex << std::internal << "0x" << std::setfill('0') << std::setw(sizeof(Value)* 2) << static_cast<int>(Value);
    m_rBuffer += oss.str();
  }
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(s16 Value)
{
  if (_CheckLogLevel())
  {
    std::ostringstream oss;
    oss << std::hex << std::internal << std::showbase << std::setfill('0') << std::setw(sizeof(Value)* 2 + 2) << static_cast<s32>(Value);
    m_rBuffer += oss.str();
  }
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(u16 Value)
{
  if (_CheckLogLevel())
  {
    std::ostringstream oss;
    oss << std::hex << std::internal << std::showbase << std::setfill('0') << std::setw(sizeof(Value)* 2 + 2) << static_cast<u32>(Value);
    m_rBuffer += oss.str();
  }
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(Address Addr)
{
  if (_CheckLogLevel())
  {
    boost::recursive_mutex::scoped_lock(m_Mutex);
    m_rBuffer += Addr.ToString();
  }
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(std::string Msg)
{
  if (_CheckLogLevel())
  {
    boost::recursive_mutex::scoped_lock(m_Mutex);
    m_rBuffer += Msg;
  }
  return *this;
}

template<> LogWrapper& LogWrapper::operator<<(LogWrapper::LoggerFunction pFunc)
{
  if (!_CheckLogLevel())
    return *this;
  return pFunc(*this);
}

LogWrapper& LogFlush(LogWrapper& rLogWrapper)
{
  rLogWrapper.Flush();
  return rLogWrapper;
}

LogWrapper& LogEnd(LogWrapper& rLogWrapper)
{
  rLogWrapper.Lock();
  rLogWrapper.Write("\n");
  LogFlush(rLogWrapper);
  rLogWrapper.Unlock();
  return rLogWrapper;
}

LogWrapper& LogLevel(LogWrapper& rLogWrapper, LogLevelType NewLvl)
{
  rLogWrapper.Level(NewLvl);
  return rLogWrapper;
}

LogWrapper Log::Write(std::string const& rType)
{
  return LogWrapper(Log::m_pLog, rType, Log::m_LogMap[rType]);
}

void Log::Flush(void)
{
  m_LogMap.clear();
}

bool Log::ChangeLogLevel(LogLevelType NewLvl)
{
  static std::map<LogLevelType, std::string> LogLvl2Str
  {
    { LogDefault, "default" },
    { LogWarning, "warning" },
    { LogError,   "error"   },
    { LogInfo,    "info"    },
    { LogDebug,   "debug"   },
  };

  auto itLvl = LogLvl2Str.find(NewLvl);
  if (itLvl == std::end(LogLvl2Str))
    return false;
  UserConfiguration UsrCfg;
  return UsrCfg.SetOption("core.log_level", itLvl->second);
}

Log::Log(void)
{
}

Log::~Log(void)
{
  Flush();
}

MEDUSA_NAMESPACE_END
