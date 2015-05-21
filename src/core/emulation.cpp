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

bool Emulator::Execute(Address const& rAddress, Expression::SPType spExpr)
{
  return Execute(rAddress, Expression::LSPType({ spExpr }));
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

bool Emulator::AddHookOnInstruction(HookCallback InsnCb)
{
  m_InsnCb = InsnCb;
  return true;
}

void Emulator::CallInstructionHook(void)
{
  if (!m_InsnCb)
    return;
  Address CurAddr;
  if (!m_pCpuCtxt->GetAddress(CpuContext::AddressExecution, CurAddr))
  {
    Log::Write("core").Level(LogError) << "failed to get execution address" << LogEnd;
    return;
  }
  m_InsnCb(m_pCpuCtxt, m_pMemCtxt, CurAddr);
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

  itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt, rAddress);
  return true;
}

bool Emulator::_IsSemanticCached(Address const& rAddress) const
{
  return m_SemCache.find(rAddress) != std::end(m_SemCache);
}

bool Emulator::_CacheSemantic(Address const& rAddress, Expression::VSPType& rExprs)
{
  if (_IsSemanticCached(rAddress))
    return false;
  m_SemCache[rAddress] = rExprs;
  return true;
}

bool Emulator::_InvalidSemantic(Address const& rAddress)
{
  auto itCache = m_SemCache.find(rAddress);
  if (itCache == std::end(m_SemCache))
    return false;
  m_SemCache.erase(itCache);
  return true;
}

MEDUSA_NAMESPACE_END
