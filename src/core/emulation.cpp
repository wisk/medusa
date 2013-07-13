#include "medusa/emulation.hpp"

MEDUSA_NAMESPACE_BEGIN

Emulator::Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt)
  : m_pCpuInfo(pCpuInfo), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_pVarCtxt(pVarCtxt)
{
}

Emulator::~Emulator(void)
{
}

bool Emulator::ReadRegister(u32 Register, void* pValue, u32 ValueSize) const
{
  return m_pCpuCtxt->ReadRegister(Register, pValue, ValueSize);
}

bool Emulator::WriteRegister(u32 Register, void const* pValue, u32 ValueSize)
{
  return m_pCpuCtxt->WriteRegister(Register, pValue, ValueSize);
}
bool Emulator::ReadMemory(Address const& rAddress, void* pValue, u32 ValueSize) const
{
  u64 LinAddr;
  if (m_pCpuCtxt->Translate(rAddress, LinAddr) == false)
    LinAddr = rAddress.GetOffset();
  return m_pMemCtxt->ReadMemory(LinAddr, pValue, ValueSize);
}

bool Emulator::WriteMemory(Address const& rAddress, void const* pValue, u32 ValueSize)
{
  u64 LinAddr;
  if (m_pCpuCtxt->Translate(rAddress, LinAddr) == false)
    LinAddr = rAddress.GetOffset();
  return m_pMemCtxt->WriteMemory(LinAddr, pValue, ValueSize);
}

bool Emulator::AddHook(Address const& rAddress, u32 Type, HookCallback Callback)
{
  auto itHook = m_Hooks.find(rAddress);
  if (itHook != std::end(m_Hooks))
  {
    itHook->second.m_Callback = Callback;
    itHook->second.m_Type |= Type;
    return true;
  }

  m_Hooks[rAddress] = HookInformation(Type, Callback);
  return true;
}

bool Emulator::AddHook(Document const& rDoc, std::string const& rLabelName, u32 Type, HookCallback Callback)
{
  auto Addr = rDoc.GetAddressFromLabelName(rLabelName);
  return AddHook(Addr, Type, Callback);
}

bool Emulator::RemoveHook(Address const& rAddress)
{
  auto itHook = m_Hooks.find(rAddress);
  if (itHook == std::end(m_Hooks))
    return false;
  m_Hooks.erase(itHook);
  return true;
}

bool Emulator::TestHook(Address const& rAddress, u32 Type) const
{
  auto itHook = m_Hooks.find(rAddress);
  if (itHook == std::end(m_Hooks))
    return false;

  if (!(itHook->second.m_Type & Type))
    return false;

  itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  return true;
}

MEDUSA_NAMESPACE_END