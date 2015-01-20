#include "medusa/context.hpp"
#include <iomanip>

MEDUSA_NAMESPACE_BEGIN

bool CpuContext::Translate(Address const& rLogicalAddress, u64& rLinearAddress) const
{
  auto itAddr = m_AddressMap.find(Address(rLogicalAddress.GetBase(), 0x0));
  if (itAddr != std::end(m_AddressMap))
  {
    rLinearAddress = itAddr->second + rLogicalAddress.GetOffset();
    return true;
  }

  rLinearAddress = rLogicalAddress.GetOffset();

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

MemoryContext::~MemoryContext(void)
{
  for (auto& rMemChunk : m_Memories)
    ::free(rMemChunk.m_Buffer);
}

bool MemoryContext::ReadMemory(u64 LinearAddress, void* pValue, u32 ValueSize) const
{
  MemoryChunk MemChnk;
  if (!FindMemoryChunk(LinearAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = LinearAddress - MemChnk.m_LinearAddress;
  //std::cout << "read: ";
  //for (size_t i = 0; i < ValueSize; ++i)
  //  std::cout << std::hex << static_cast<int>(*(reinterpret_cast<u8 const*>(MemChnk.m_Buffer) + Offset + i)) << " ";
  //std::cout << std::endl;
  memcpy(pValue, reinterpret_cast<u8 const*>(MemChnk.m_Buffer) + Offset, ValueSize);
  return true;
}

bool MemoryContext::WriteMemory(u64 LinearAddress, void const* pValue, u32 ValueSize, bool SignExtend)
{
  MemoryChunk MemChnk;
  if (!FindMemoryChunk(LinearAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = LinearAddress - MemChnk.m_LinearAddress;
  memcpy(reinterpret_cast<u8 *>(MemChnk.m_Buffer) + Offset, pValue, ValueSize);
  return true;
}

bool MemoryContext::FindMemory(u64 LinearAddress, void*& prAddress, u32& rSize) const
{
  MemoryChunk MemChk;

  prAddress = nullptr;
  rSize = 0;

  if (FindMemoryChunk(LinearAddress, MemChk) == false)
    return false;

  prAddress = MemChk.m_Buffer;
  rSize = MemChk.m_Size;
  return true;
}

bool MemoryContext::AllocateMemory(u64 Address, u32 Size, void** ppRawMemory)
{
  if (ppRawMemory)
    *ppRawMemory = nullptr;
  void *pRawMemory = ::malloc(Size);
  if (pRawMemory == nullptr)
    return false;
  memset(pRawMemory, 0xfa, Size);
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
  ::free(itMemChunk->m_Buffer);
  m_Memories.erase(itMemChunk);
  return true;
}

bool MemoryContext::MapDocument(Document const& rDoc, CpuContext const* pCpuCtxt)
{
  bool Res = true;
  rDoc.ForEachMemoryArea([&](MemoryArea const& rMemArea)
  {
    Address const& rMemAreaAddr = rMemArea.GetBaseAddress();
    u32 MemAreaSize             = rMemArea.GetSize();
    u32 MemAreaFileSize         = rMemArea.GetFileSize();
    u32 Size                    = std::max(MemAreaSize, MemAreaFileSize); // TODO: be more careful with that

    void* pRawMemory;
    u64 LinearAddress;
    if (pCpuCtxt->Translate(rMemAreaAddr, LinearAddress) == false)
      LinearAddress = rMemAreaAddr.GetOffset();

    if (AllocateMemory(LinearAddress, Size, &pRawMemory) == false)
    {
      Res = false;
      return;
    }

    // TODO: Do we have to zero-out memory?
    if (MemAreaFileSize == 0x0)
      return;

    TOffset MemAreaFileOff;
    if (rMemArea.ConvertOffsetToFileOffset(rMemAreaAddr.GetOffset(), MemAreaFileOff) == false)
      return;

    if (!rDoc.GetBinaryStream().Read(MemAreaFileOff, pRawMemory, MemAreaFileSize))
    {
      FreeMemory(LinearAddress);
      Res = false;
      return;
    }
  });
  return Res;
}

std::string MemoryContext::ToString(void) const
{
  if (m_Memories.empty())
    return "";

  std::ostringstream oss;
  for (MemoryChunk const& rMemChnk : m_Memories)
  {
    oss << std::setfill('0') << "laddr: " << std::hex << std::setw(16) << rMemChnk.m_LinearAddress << ", size: " << std::hex << std::setw(8) << std::setfill('0') << rMemChnk.m_Size << ", rawb: " << rMemChnk.m_Buffer << std::endl;
    HexDump(oss, rMemChnk.m_Buffer, static_cast<u16>(rMemChnk.m_Size), rMemChnk.m_LinearAddress);
  }
  return oss.str();
}

bool MemoryContext::FindMemoryChunk(u64 LinearAddress, MemoryChunk& rFoundMemChnk) const
{
  for (MemoryChunk const& rMemChnk : m_Memories)
  {
    if (LinearAddress >= rMemChnk.m_LinearAddress && LinearAddress < (rMemChnk.m_LinearAddress + rMemChnk.m_Size))
    {
      rFoundMemChnk = rMemChnk;
      return true;
    }
  }
  return false;
}

MEDUSA_NAMESPACE_END
