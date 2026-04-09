#ifndef ARM_CALLING_CONVENTION_HPP
#define ARM_CALLING_CONVENTION_HPP

#include "medusa/calling_convention.hpp"

using namespace medusa;

//ref: http://infocenter.arm.com/help/topic/com.arm.doc.ihi0042f/IHI0042F_aapcs.pdf
class AapcsCallingConvention : public CallingConvention
{
public:
  bool               GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt, u16 ParamNr, BitVector& rParamVal) const override;
  bool               ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo) const override;
  bool               ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo, BitVector const& rRetVal) const override;

  Expression::SPType EmitGetParameter(u16 ParamNr, ValueType ParamTy) const override;
  Expression::SPType EmitReturnFromFunction(u16 ParamNo) const override;
  Expression::SPType EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const override;

  bool               AnalyzeArgument(
    Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy)                                const override;
  bool               AnalyzeParameter(
    Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy)                            const override;
  RegisterType       GetRegisterType(u32 Id)                                          const override;
  StackCleanerType   StackCleanupBy()                                             const override;

private:
};

#endif // !ARM_CALLING_CONVENTION_HPP