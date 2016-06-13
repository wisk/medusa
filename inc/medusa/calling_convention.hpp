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

  enum ValueType
  {
    UnknownType,
    IntegerType,
    FloatingPointType,
    VectorType,
    VariableArgumentsType,
  };

  // TODO(wisk): handle the difference between int and float parameter (e.g. MS x64 param0 could be either rcx or xmm0)
  // TODO(wisk): add return value and write return address methods

  virtual bool               GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt,
    u16 ParamNr, BitVector& rParamVal)                                                          const { return false; }
  virtual bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo)                                                                              const { return false; }
  virtual bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt,
    u16 ParamNo, BitVector const& rRetVal)                                                      const { return false; }

  virtual Expression::SPType EmitGetParameter(u16 ParamNr, ValueType ParamTy)                 const { return nullptr; }
  virtual Expression::SPType EmitReturnFromFunction(u16 ParamNo)                              const { return nullptr; }
  virtual Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const { return nullptr; }

  virtual bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy)                                const { return false; }
  virtual bool               AnalyzeParameter(
    Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy)                            const { return false; }
  virtual RegisterType       GetRegisterType(u32 Id)                                          const { return UnknownRegister; }
  virtual StackCleanerType   StackCleanupBy(void)                                             const { return StackCleanedByUnknown; }
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CALLING_CONVENTION_HPP