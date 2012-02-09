#ifndef __MEDUSA_LOG__
#define __MEDUSA_LOG__

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

class Medusa_EXPORT LogWrapper
{
public:
  typedef boost::function<void(wchar_t const*)> LoggerCallback;
  typedef LogWrapper& (*LoggerFunction)(LogWrapper&);

  LogWrapper(LoggerCallback pLog, std::string Name, std::wstring& rBuffer)
    : m_pLog(pLog)
    , m_Name(L"")
    , m_rBuffer(rBuffer)
  {
    std::wstring WideName = StringToWString(Name);
    m_Name     = WideName;

    if (m_rBuffer.empty())
    {
      m_rBuffer  = WideName;
      m_rBuffer += L": ";
    }
  }

  template<typename T> LogWrapper& operator<<(T Value)
  {
    std::wostringstream oss;

    if (boost::is_arithmetic<T>::value)
      oss << std::hex << std::internal << std::showbase << std::setfill(L'0') << std::setw(sizeof(Value) * 2 + 2) << Value;
    else
      oss << Value;
    m_rBuffer += oss.str();

    return *this;
  }

  LogWrapper& Flush(void)
  {
    if (m_pLog)
    {
      boost::recursive_mutex::scoped_lock(m_Mutex);
      m_pLog(m_rBuffer.c_str());
      m_rBuffer = m_Name;
      m_rBuffer += L": ";
    }
    return *this;
  }

  std::wstring& GetBuffer(void)   { return m_rBuffer;  }
  void Write(wchar_t const* pMsg) { m_rBuffer += pMsg; }
  void Lock(void)                 { m_Mutex.lock();    }
  void Unlock(void)               { m_Mutex.unlock();  }

private:
  static std::wstring StringToWString(std::string const& rString);

  typedef boost::recursive_mutex MutexType;

  LoggerCallback   m_pLog;
  std::wstring     m_Name;
  std::wstring&    m_rBuffer;
  static MutexType m_Mutex;
};

template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(s16 Value);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(u16 Value);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(Address Addr);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(std::string Msg);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(std::wstring Msg);
template<> Medusa_EXPORT LogWrapper& LogWrapper::operator<<(LogWrapper::LoggerFunction pFunc);

Medusa_EXPORT LogWrapper& LogFlush(LogWrapper& rLogWrapper);
Medusa_EXPORT LogWrapper& LogEnd(LogWrapper& rLogWrapper);

//! this class allows core or modules to log information
class Medusa_EXPORT Log : boost::noncopyable
{
public:
  static void SetLog(LogWrapper::LoggerCallback pLog)
  { m_pLog = pLog; }

  static LogWrapper Write(std::string const& rType);

private:
  Log(void);
  ~Log(void);

  typedef std::map<std::string, std::wstring> LogMap;

  static LogMap                     m_LogMap;
  static LogWrapper::LoggerCallback m_pLog;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_LOG__