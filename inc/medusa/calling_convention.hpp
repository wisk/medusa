#ifndef MEDUSA_CALLING_CONVENTION_HPP
#define MEDUSA_CALLING_CONVENTION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/expression.hpp"
#include "medusa/context.hpp"

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

  enum ArgumentType
  {
    UnknownArgument,
    IntegerArgument,
    FloatingPointArgument,
  };

  // TODO(wisk): handle the difference between int and float parameter (e.g. MS x64 param0 could be either rcx or xmm0)
  // TODO(wisk): add return value and write return address methods

  virtual bool               GetIntParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt,
    u16 ParamNr, IntType& rParamValue)                                                        const { return false; }
  virtual bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo)                                                                              const { return false; }
  virtual bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo, IntType const& rRetVal)                                                      const { return false; }

  virtual Expression::SPType EmitGetIntParameter(u16 ParamNr)                                 const { return nullptr; }
  virtual Expression::SPType EmitReturnFromFunction(u16 ParamNo)                              const { return nullptr; }
  virtual Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, IntType const& rRetVal) const { return nullptr; }

  virtual RegisterType       GetRegisterType(u32 Id)                                          const { return UnknownRegister; }
  virtual bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ArgumentType& rArgType)                           const { return false; }
  virtual StackCleanerType   StackCleanupBy(void)                                             const { return StackCleanedByUnknown; }
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CALLING_CONVENTION_HPP