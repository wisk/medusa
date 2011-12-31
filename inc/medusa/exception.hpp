#ifndef _MEDUSA_EXCEPTION_
#define _MEDUSA_EXCEPTION_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include <string>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Exception is a base exception type.
class Medusa_EXPORT Exception
{
public:
  Exception(void)                                  : m_ExceptionMessage(L"Unknown exception")  {}
  Exception(std::wstring const& ExceptionMessage)  : m_ExceptionMessage(ExceptionMessage)      {}
  ~Exception(void) {}

  std::wstring const& What(void) const { return m_ExceptionMessage; }

protected:
  std::wstring m_ExceptionMessage;
};

//! Exception_System is an exception related to a system error.
class Medusa_EXPORT Exception_System : public Exception
{
public:
  Exception_System(void) : Exception(ErrorCodeToMessage()) {}
  Exception_System(std::wstring const& FunctionName) : Exception(L"")
  { m_ExceptionMessage = FunctionName; m_ExceptionMessage += L": "; m_ExceptionMessage += ErrorCodeToMessage(); }
  ~Exception_System(void) {}

private:
  static std::wstring ErrorCodeToMessage(void);
};

//! Exception_NotImplemented is n exception threw when a feature is not implemented.
class Medusa_EXPORT Exception_NotImplemented : public Exception
{
public:
  Exception_NotImplemented(void) : Exception(L"Not implemented") {}
  Exception_NotImplemented(std::wstring const& FeatureName) : Exception(L"")
  { m_ExceptionMessage = FeatureName + L": not implemented"; }
  ~Exception_NotImplemented(void) {}
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_EXCEPTION_
