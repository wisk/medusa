#ifndef MEDUSA_LOG_HPP
#define MEDUSA_LOG_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"

#include "medusa/address.hpp"

#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <boost/function.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/type_traits.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4251 4275)
#endif

MEDUSA_NAMESPACE_BEGIN

enum LogLevelType
{
  LogUnknown,
  LogWarning,
  LogError,
  LogInfo,
  LogDefault,
  LogDebug,
};

class Medusa_EXPORT LogWrapper
{
public:
  typedef boost::function<void(std::string const&)> LoggerCallback;
  typedef LogWrapper& (*LoggerFunction)(LogWrapper&);

  LogWrapper(LoggerCallback pLog, std::string const& rName, std::string& rBuffer);

  template<typename T> LogWrapper& operator<<(T Value)
  {
    if (!_CheckLogLevel())
      return *this;

    std::ostringstream oss;

    if (boost::is_arithmetic<T>::value)
    {
      oss << std::hex << std::internal << "0x" << std::setfill('0') << std::setw(sizeof(Value) * 2) << Value;
    }
    else
      oss << Value;
    m_rBuffer += oss.str();

    return *this;
  }

  LogWrapper& Flush(void);
  LogWrapper& Level(LogLevelType NewLvl);

  std::string& GetBuffer(void);
  void Write(char const* pMsg);
  void Lock(void);
  void Unlock(void);

private:
  typedef boost::recursive_mutex MutexType;

  bool _CheckLogLevel(void) const;

  LoggerCallback   m_pLog;
  std::string      m_Name;
  std::string&     m_rBuffer;
  static MutexType m_Mutex;
  LogLevelType     m_Level;
};

template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(s8 Value);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(u8 Value);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(s16 Value);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(u16 Value);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(Address Addr);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(std::string Msg);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(LogWrapper::LoggerFunction pFunc);

Medusa_EXPORT LogWrapper& LogFlush(LogWrapper& rLogWrapper);
Medusa_EXPORT LogWrapper& LogEnd(LogWrapper& rLogWrapper);
Medusa_EXPORT LogWrapper& LogLevel(LogWrapper& rLogWrapper, LogLevelType NewLvl);

//! this class allows core or modules to log information
class Medusa_EXPORT Log : boost::noncopyable
{
public:
  static void SetLog(LogWrapper::LoggerCallback pLog)
  { m_pLog = pLog; }

  static LogWrapper Write(std::string const& rType);
  static void Flush(void);
  static bool ChangeLogLevel(LogLevelType NewLvl);

private:
  Log(void);
  ~Log(void);

  typedef std::map<std::string, std::string> LogMap;

  static LogMap                     m_LogMap;
  static LogWrapper::LoggerCallback m_pLog;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_LOG_HPP
