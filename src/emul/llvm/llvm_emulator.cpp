#include "llvm_emulator.hpp"

MEDUSA_NAMESPACE_USE

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt) : Emulator(pCpuInfo, pCpuCtxt)
{
}

LlvmEmulator::~LlvmEmulator(void)
{
}

void LlvmEmulator::ReadRegister(u32 Register, void* pValue, u32 ValueSize) const
{
}

void LlvmEmulator::WriteRegister(u32 Register, void const* pValue, u32 ValueSize)
{
}

void LlvmEmulator::ReadMemory(Address const& rAddress, void* pValue, u32 ValueSize) const
{
}

void LlvmEmulator::WriteMemory(Address const& rAddress, void const* pValue, u32 ValueSize)
{
}

bool LlvmEmulator::AllocateMemory(Address const& rAddress, u32 Size, void** ppRawMemory)
{
  return false;
}

bool LlvmEmulator::FreeMemory(Address const& rAddress)
{
  return false;
}

void LlvmEmulator::Execute(Expression const& rExpr)
{
}

void LlvmEmulator::Execute(Expression::List const& rExprList)
{
}