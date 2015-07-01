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

  // TODO(wisk): handle the difference between int and float parameter (e.g. MS x64 param0 could be either rcx or xmm0)
  // TODO(wisk): add return value and write return address methods
  virtual Expression::SPType GetParameter(u16 ParamNr) const { return nullptr; }
  virtual StackCleanerType   StackCleanupBy(void)      const { return StackCleanedByUnknown; }
  virtual RegisterType       GetRegisterType(u32 Id)   const { return UnknownRegister; }
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CALLING_CONVENTION_HPP