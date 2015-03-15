#include "medusa/emulation.hpp"

MEDUSA_NAMESPACE_BEGIN

Emulator::Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
  : m_pCpuInfo(pCpuInfo), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt)
{
}

Emulator::~Emulator(void)
{
}

bool Emulator::ReadRegister(u32 Reg, void* pVal, u32 BitSize) const
{
  return m_pCpuCtxt->ReadRegister(Reg, pVal, BitSize);
}

template<>
bool Emulator::ReadRegister<bool>(u32 Reg, bool& rRegVal) const
{
  return ReadRegister(Reg, &rRegVal, 1);
}

bool Emulator::WriteRegister(u32 Reg, void const* pVal, u32 BitSize)
{
  return m_pCpuCtxt->WriteRegister(Reg, pVal, BitSize);
}

template<>
bool Emulator::WriteRegister<bool>(u32 Reg, bool RegVal)
{
  return WriteRegister(Reg, &RegVal, 1);
}

bool Emulator::ReadMemory(Address const& rAddr, void* pVal, u32 Size) const
{
  u64 LinAddr;
  if (m_pCpuCtxt->Translate(rAddr, LinAddr) == false)
    LinAddr = rAddr.GetOffset();
  return m_pMemCtxt->ReadMemory(LinAddr, pVal, Size);
}

bool Emulator::WriteMemory(Address const& rAddr, void const* pVal, u32 Size)
{
  u64 LinAddr;
  if (m_pCpuCtxt->Translate(rAddr, LinAddr) == false)
    LinAddr = rAddr.GetOffset();
  return m_pMemCtxt->WriteMemory(LinAddr, pVal, Size);
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
