#include "medusa/cpu.hpp"
#include <iomanip>

MEDUSA_NAMESPACE_BEGIN

bool CpuContext::Translate(Address const& rLogicalAddress, u64& rLinearAddress) const
{
  auto itAddr = m_AddressMap.find(Address(rLogicalAddress.GetBase(), 0x0));
  if (itAddr == std::end(m_AddressMap))
    return false;
  rLinearAddress = itAddr->second + rLogicalAddress.GetOffset();
  return true;
}

bool CpuContext::AddMapping(Address const& rLogicalAddress, u64 LinearAddress)
{
  m_AddressMap[rLogicalAddress] = LinearAddress;
  return true;
}

bool CpuContext::RemoveMapping(Address const& rLogicalAddress)
{
  auto itAddr = m_AddressMap.find(rLogicalAddress);
  if (itAddr == std::end(m_AddressMap))
    return false;
  m_AddressMap.erase(itAddr);
  return true;
}

bool MemoryContext::ReadMemory(u64 LinearAddress, void* pValue, u32 ValueSize) const
{
  MemoryChunk MemChnk;
  if (!FindMemoryChunk(LinearAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = LinearAddress - MemChnk.m_Address;
  memcpy(pValue, reinterpret_cast<u8 const*>(MemChnk.m_Buffer) + Offset, ValueSize);
  return true;
}

bool MemoryContext::WriteMemory(u64 LinearAddress, void const* pValue, u32 ValueSize)
{
  MemoryChunk MemChnk;
  if (!FindMemoryChunk(LinearAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = LinearAddress - MemChnk.m_Address;
  memcpy(reinterpret_cast<u8 *>(MemChnk.m_Buffer) + Offset, pValue, ValueSize);
  return true;
}

bool MemoryContext::AllocateMemory(u64 Address, u32 Size, void** ppRawMemory)
{
  if (ppRawMemory)
    *ppRawMemory = nullptr;
  void *pRawMemory = static_cast<void*>(new (std::nothrow) u8[Size]);
  memset(pRawMemory, 0xfa, Size);
  if (pRawMemory == nullptr)
    return false;
  m_Memories.insert(MemoryChunk(Address, Size, pRawMemory));
  if (ppRawMemory)
    *ppRawMemory = pRawMemory;
  return true;
}

bool MemoryContext::FreeMemory(u64 Address)
{
  auto itMemChunk = m_Memories.find(MemoryChunk(Address));
  if (itMemChunk == std::end(m_Memories))
    return false;
  delete [] static_cast<u8*>(itMemChunk->m_Buffer);
  m_Memories.erase(itMemChunk);
  return true;
}

void MemoryContext::MapDatabase(Database const& rDatabase, CpuContext const* pCpuCtxt)
{
  for (auto itMemArea = rDatabase.Begin(); itMemArea != rDatabase.End(); ++itMemArea)
  {
    Address const& rMemAreaAddr = (*itMemArea)->GetVirtualBase();
    u32 MemAreaSize             = (*itMemArea)->GetVirtualSize();

    void* pRawMemory;
    u64 LinearAddress;
    if (pCpuCtxt->Translate(rMemAreaAddr, LinearAddress) == false)
      LinearAddress = rMemAreaAddr.GetOffset();

    AllocateMemory(LinearAddress, MemAreaSize, &pRawMemory);
    if ((*itMemArea)->Read(rMemAreaAddr.GetOffset(), pRawMemory, MemAreaSize) == false)
    {
      FreeMemory(LinearAddress);
      return;
    }
  }
}

std::string MemoryContext::ToString(void) const
{
  if (m_Memories.empty())
    return "";

  std::ostringstream oss;
  for (auto itMemChnk = std::begin(m_Memories); itMemChnk != std::end(m_Memories); ++itMemChnk)
    oss << "addr: " << std::hex << std::setw(16) << itMemChnk->m_Address << ", size: " << std::hex << std::setw(8) << std::setfill('0') << itMemChnk->m_Size << ", rawb: " << itMemChnk->m_Buffer << std::endl;
  return oss.str();
}

bool MemoryContext::FindMemoryChunk(Address const& rAddress, MemoryChunk& rMemChnk) const
{
  for (auto itMemChnk = std::begin(m_Memories); itMemChnk != std::end(m_Memories); ++itMemChnk)
  {
    if (rAddress >= itMemChnk->m_Address && rAddress < (itMemChnk->m_Address + itMemChnk->m_Size))
    {
      rMemChnk = *itMemChnk;
      return true;
    }
  }
  return false;
}

bool VariableContext::ReadVariable(std::string const& rVariableName, u64& rValue) const
{
  auto const itVar = m_Variables.find(rVariableName);
  if (itVar == std::end(m_Variables))
    return false;

  switch (itVar->second.m_Type)
  {
  case Var1Bit:  rValue = (itVar->second.m_Value & 0x00000001); break;
  case Var8Bit:  rValue = (itVar->second.m_Value & 0x000000ff); break;
  case Var16Bit: rValue = (itVar->second.m_Value & 0x0000ffff); break;
  case Var32Bit: rValue = (itVar->second.m_Value & 0xffffffff); break;
  case Var64Bit: rValue = (itVar->second.m_Value);              break;
  default: return false;
  }

  return true;
}

bool VariableContext::WriteVariable(std::string const& rVariableName, u64 Value)
{
  auto itVar = m_Variables.find(rVariableName);
  if (itVar == std::end(m_Variables))
    return false;

  switch (itVar->second.m_Type)
  {
  case Var1Bit:  itVar->second.m_Value = (Value & 0x00000001); break;
  case Var8Bit:  itVar->second.m_Value = (Value & 0x000000ff); break;
  case Var16Bit: itVar->second.m_Value = (Value & 0x0000ffff); break;
  case Var32Bit: itVar->second.m_Value = (Value & 0xffffffff); break;
  case Var64Bit: itVar->second.m_Value = (Value);              break;
  default: return false;
  }

  return true;
}

bool VariableContext::AllocateVariable(u32 Type, std::string const& rVariableName)
{
  FreeVariable(rVariableName);

  m_Variables[rVariableName] = VariableInformation(Type);
  return true;
}

bool VariableContext::FreeVariable(std::string const& rVariableName)
{
  auto VarNo = m_Variables.size();
  m_Variables.erase(rVariableName);
  return VarNo != m_Variables.size();
}

std::string VariableContext::ToString(void) const
{
  if (m_Variables.empty())
    return "";

  std::ostringstream oss;

  for (auto itVar = std::begin(m_Variables); itVar != std::end(m_Variables); ++itVar)
    oss << "type: " << static_cast<int>(itVar->second.m_Type) << ", value: " << std::hex << std::setfill('0') << std::setw(itVar->second.m_Type / 8) << itVar->second.m_Value << std::endl;
  return oss.str();
}

MEDUSA_NAMESPACE_END