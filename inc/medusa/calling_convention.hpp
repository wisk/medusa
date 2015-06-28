#ifndef MEDUSA_CALLING_CONVENTION_HPP
#define MEDUSA_CALLING_CONVENTION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/expression.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT CallingConvention
{
public:
  enum StackCleanerType
  {
    StackCleanedByUnknown,
    StackCleanedByCaller,
    StackCleanedByCallee,
  };

  enum RegisterType
  {
    UnknownRegister,
    VolatileRegister,
    NonVolatileRegister,
  };

  virtual Expression::SPType GetParameter(u16 ParamNr) { return nullptr; }
  virtual StackCleanerType   StackCleanupBy(void)      { return StackCleanedByUnknown; }
  virtual RegisterType       GetRegisterType(u32 Id)   { return UnknownRegister; }
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CALLING_CONVENTION_HPP