#include "llvm_emulator.hpp"

MEDUSA_NAMESPACE_USE

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt) : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
{
}

LlvmEmulator::~LlvmEmulator(void)
{
}

void LlvmEmulator::Execute(Expression const& rExpr)
{
}

void LlvmEmulator::Execute(Expression::List const& rExprList)
{
}