#include "arm_calling_convention.hpp"
#include "arm_def.hpp"

using namespace medusa;

bool AapcsCallingConvention::GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt, u16 ParamNr, BitVector& rParamVal) const
{
  u32 ParamVal = 0;
  bool Result = true;

  switch (ParamNr)
  {
  case 0: Result = pCpuCtxt->ReadRegister(ARM_RegR0, ParamVal); break;
  case 1: Result = pCpuCtxt->ReadRegister(ARM_RegR1, ParamVal); break;
  case 2: Result = pCpuCtxt->ReadRegister(ARM_RegR2, ParamVal); break;
  case 3: Result = pCpuCtxt->ReadRegister(ARM_RegR3, ParamVal); break;
  default: break;
  }

  if (!Result)
  {
    u32 StkVal;
    if (!pCpuCtxt->ReadRegister(ARM_RegSP, StkVal))
      return false;
    if (!pMemCtxt->ReadMemory(StkVal + (ParamNr - 4) * 4, ParamVal))
      return false;
    Result = true;
  }

  if (Result)
    rParamVal = BitVector(ParamVal);
  return Result;
}

bool AapcsCallingConvention::ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo) const
{
  u32 LnkReg;
  if (!pCpuCtxt->ReadRegister(ARM_RegLR, LnkReg))
    return false;
  if (!pCpuCtxt->WriteRegister(ARM_RegPC, LnkReg))
    return false;
  return true;
}

bool AapcsCallingConvention::ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo, BitVector const& rRetVal) const
{
  if (!pCpuCtxt->WriteRegister(ARM_RegR0, rRetVal))
    return false;
  return ReturnFromFunction(pCpuCtxt, pMemCtxt, ParamNo);
}

Expression::SPType AapcsCallingConvention::EmitGetParameter(u16 ParamNr, ValueType ParamTy) const
{
  // TODO(wisk):
  return nullptr;
}

Expression::SPType AapcsCallingConvention::EmitReturnFromFunction(u16 ParamNo) const
{
  // TODO(wisk):
  return nullptr;
}

Expression::SPType AapcsCallingConvention::EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const
{
  // TODO(wisk):
  return nullptr;
}

bool AapcsCallingConvention::AnalyzeArgument(Expression::SPType spExpr, u16& rArgNr, ValueType& rArgTy) const
{
  // TODO(wisk):
  return false;
}

bool AapcsCallingConvention::AnalyzeParameter(Expression::SPType spExpr, u16& rParamNr, ValueType& rParamTy) const
{
  // TODO(wisk):
  return false;
}

CallingConvention::RegisterType AapcsCallingConvention::GetRegisterType(u32 Id) const
{
  // TODO(wisk):
  return UnknownRegister;
}

CallingConvention::StackCleanerType AapcsCallingConvention::StackCleanupBy(void) const
{
  return StackCleanedByUnknown;
}
