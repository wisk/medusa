#include "llvm_emulator.hpp"

MEDUSA_NAMESPACE_USE

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt, VariableContext *pVarCtxt) : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt, pVarCtxt)
{
}

LlvmEmulator::~LlvmEmulator(void)
{
}

bool LlvmEmulator::Execute(Expression const& rExpr)
{
  return false;
}

bool LlvmEmulator::Execute(Expression::List const& rExprList)
{
  return false;
}