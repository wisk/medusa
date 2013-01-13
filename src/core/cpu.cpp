#include "medusa/cpu.hpp"
#include <iomanip>

MEDUSA_NAMESPACE_BEGIN

bool MemoryContext::ReadMemory(Address const& rAddress, void* pValue, u32 ValueSize) const
{
  MemoryChunk MemChnk;
  if (!FindMemoryChunk(rAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = rAddress.GetOffset() - MemChnk.m_Address.GetOffset();
  memcpy(pValue, reinterpret_cast<u8 const*>(MemChnk.m_Buffer) + Offset, ValueSize);
  return true;
}

bool MemoryContext::WriteMemory(Address const& rAddress, void const* pValue, u32 ValueSize)
{
  MemoryChunk MemChnk;
  if (!FindMemoryChunk(rAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = rAddress.GetOffset() - MemChnk.m_Address.GetOffset();
  memcpy(reinterpret_cast<u8 *>(MemChnk.m_Buffer) + Offset, pValue, ValueSize);
  return true;
}

bool MemoryContext::AllocateMemory(Address const& rAddress, u32 Size, void** ppRawMemory)
{
  if (ppRawMemory)
    *ppRawMemory = nullptr;
  void *pRawMemory = static_cast<void*>(new (std::nothrow) u8[Size]);
  memset(pRawMemory, 0xfa, Size);
  if (pRawMemory == nullptr)
    return false;
  m_Memories.insert(MemoryChunk(rAddress, Size, pRawMemory));
  if (ppRawMemory)
    *ppRawMemory = pRawMemory;
  return true;
}

bool MemoryContext::FreeMemory(Address const& rAddress)
{
  auto itMemChunk = m_Memories.find(MemoryChunk(rAddress));
  if (itMemChunk == std::end(m_Memories))
    return false;
  delete [] static_cast<u8*>(itMemChunk->m_Buffer);
  m_Memories.erase(itMemChunk);
  return true;
}

void MemoryContext::MapDatabase(Database const& rDatabase)
{
  for (auto itMemArea = rDatabase.Begin(); itMemArea != rDatabase.End(); ++itMemArea)
  {
    Address const& rMemAreaAddr = (*itMemArea)->GetVirtualBase();
    u32 MemAreaSize             = (*itMemArea)->GetVirtualSize();

    void* pRawMemory;
    AllocateMemory(rMemAreaAddr, MemAreaSize, &pRawMemory);
    if ((*itMemArea)->Read(rMemAreaAddr.GetOffset(), pRawMemory, MemAreaSize) == false)
    {
      FreeMemory(rMemAreaAddr);
      return;
    }
  }
}

std::string MemoryContext::ToString(void) const
{
  std::ostringstream oss;
  for (auto itMemChnk = std::begin(m_Memories); itMemChnk != std::end(m_Memories); ++itMemChnk)
    oss << "addr: " << itMemChnk->m_Address.ToString() << ", size: " << std::hex << std::setw(8) << std::setfill('0') << itMemChnk->m_Size << ", rawb: " << itMemChnk->m_Buffer << std::endl;
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