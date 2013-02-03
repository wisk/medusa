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

MEDUSA_NAMESPACE_END