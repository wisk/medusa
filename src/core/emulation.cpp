#include "medusa/emulation.hpp"

MEDUSA_NAMESPACE_BEGIN

Emulator::Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
  : m_pCpuInfo(pCpuInfo), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt)
{
}

Emulator::~Emulator(void)
{
}

void Emulator::ReadRegister(u32 Register, void* pValue, u32 ValueSize) const
{
  m_pCpuCtxt->ReadRegister(Register, pValue, ValueSize);
}

void Emulator::WriteRegister(u32 Register, void const* pValue, u32 ValueSize)
{
  m_pCpuCtxt->WriteRegister(Register, pValue, ValueSize);
}
void Emulator::ReadMemory(Address const& rAddress, void* pValue, u32 ValueSize) const
{
  m_pMemCtxt->ReadMemory(rAddress, pValue, ValueSize);
}

void Emulator::WriteMemory(Address const& rAddress, void const* pValue, u32 ValueSize)
{
  m_pMemCtxt->WriteMemory(rAddress, pValue, ValueSize);
}


MEDUSA_NAMESPACE_END