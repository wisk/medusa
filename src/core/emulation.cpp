#include "medusa/emulation.hpp"

MEDUSA_NAMESPACE_BEGIN

Emulator::Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt)
  : m_pCpuInfo(pCpuInfo), m_pCpuCtxt(pCpuCtxt)
{
}

Emulator::~Emulator(void)
{
}

bool Emulator::AllocateMemory(Address const& rAddress, u32 Size, void** ppRawMemory)
{
  *ppRawMemory = static_cast<void*>(new (std::nothrow) u8[Size]);
  if (*ppRawMemory == nullptr)
    return false;
  m_Memories.insert(MemoryChunk(rAddress, Size, *ppRawMemory));
  return true;
}

bool Emulator::FreeMemory(Address const& rAddress)
{
  auto itMemChunk = m_Memories.find(MemoryChunk(rAddress));
  if (itMemChunk == std::end(m_Memories))
    return false;
  delete [] static_cast<u8*>(itMemChunk->m_Buffer);
  m_Memories.erase(itMemChunk);
  return true;
}

void Emulator::MapDatabase(Database const& rDatabase)
{
  for (auto itMemArea = rDatabase.Begin(); itMemArea != rDatabase.End(); ++itMemArea)
  {
    Address const& rMemAreaAddr = (*itMemArea)->GetVirtualBase();
    u32 MemAreaSize             = (*itMemArea)->GetVirtualSize();

    void* pRawMemory;
    AllocateMemory(rMemAreaAddr, MemAreaSize, &pRawMemory);
    if ((*itMemArea)->Read(0x0, pRawMemory, MemAreaSize) == false)
    {
      FreeMemory(rMemAreaAddr);
      return;
    }
  }
}

MEDUSA_NAMESPACE_END