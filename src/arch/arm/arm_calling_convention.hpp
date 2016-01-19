#ifndef ARM_CALLING_CONVENTION_HPP
#define ARM_CALLING_CONVENTION_HPP

#include "medusa/calling_convention.hpp"

using namespace medusa;

//ref: http://infocenter.arm.com/help/topic/com.arm.doc.ihi0042f/IHI0042F_aapcs.pdf
class AapcsCallingConvention : public CallingConvention
{
public:
  virtual bool               GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt, u16 ParamNr, BitVector& rParamVal) const;
  virtual bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo) const;
  virtual bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo, BitVector const& rRetVal) const;

  virtual Expression::SPType EmitGetParameter(u16 ParamNr, ValueType ParamTy) const;
  virtual Expression::SPType EmitReturnFromFunction(u16 ParamNo) const;
  virtual Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const;

  virtual bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy)                                const;
  virtual bool               AnalyzeParameter(
    Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy)                            const;
  virtual RegisterType       GetRegisterType(u32 Id)                                          const;
  virtual StackCleanerType   StackCleanupBy(void)                                             const;

private:
};

#endif // !ARM_CALLING_CONVENTION_HPP