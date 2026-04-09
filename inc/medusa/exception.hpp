#ifndef MEDUSA_EXCEPTION_HPP
#define MEDUSA_EXCEPTION_HPP

#include "medusa/types.hpp"
#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include <string>

MEDUSA_NAMESPACE_BEGIN

//! Exception is a base exception type.
class MEDUSA_EXPORT Exception
{
public:
  Exception()                                  : m_ExceptionMessage("Unknown exception")  {}
  explicit Exception(std::string const& ExceptionMessage)  : m_ExceptionMessage(ExceptionMessage)      {}
  ~Exception() {}

  std::string const& What() const { return m_ExceptionMessage; }

protected:
  std::string m_ExceptionMessage;
};

//! Exception_System is an exception related to a system error.
class MEDUSA_EXPORT Exception_System : public Exception
{
public:
  Exception_System() : Exception(ErrorCodeToMessage()) {}
  explicit Exception_System(std::string const& FunctionName) : Exception("")
  { m_ExceptionMessage = FunctionName; m_ExceptionMessage += ": "; m_ExceptionMessage += ErrorCodeToMessage(); }
  ~Exception_System() {}

private:
  static std::string ErrorCodeToMessage();
};

//! Exception_NotImplemented is n exception threw when a feature is not implemented.
class MEDUSA_EXPORT Exception_NotImplemented : public Exception
{
public:
  Exception_NotImplemented() : Exception("Not implemented") {}
  explicit Exception_NotImplemented(std::string const& FeatureName) : Exception("")
  { m_ExceptionMessage = FeatureName + ": not implemented"; }
  ~Exception_NotImplemented() {}
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_EXCEPTION_HPP
