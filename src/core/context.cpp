#include "medusa/context.hpp"
#include <iomanip>

MEDUSA_NAMESPACE_BEGIN

template<>
bool CpuContext::ReadRegister<bool>(u32 Reg, bool& rVal) const
{
  std::lock_guard<std::mutex> Lock(m_CpuLock);
  return ReadRegister(Reg, &rVal, 1);
}

template<>
bool CpuContext::WriteRegister<bool>(u32 Reg, bool const& rVal)
{
  std::lock_guard<std::mutex> Lock(m_CpuLock);
  u8 Tmp = rVal ? 1 : 0;
  return WriteRegister(Reg, &Tmp, 1);
}

namespace
{
  template<typename _Ty>
  bool ReadRegisterHelper(CpuContext const& rCpuCtxt, u32 Reg, IntType& rVal)
  {
    auto RegBitSize = rCpuCtxt.GetCpuInformation().GetSizeOfRegisterInBit(Reg);
    if (RegBitSize != rVal.GetBitSize())
      return false;
    _Ty RegVal;
    if (!rCpuCtxt.ReadRegister(Reg, &RegVal, RegBitSize))
      return false;
    rVal = IntType(RegBitSize, RegVal);
    return true;
  }

  template<typename _Ty>
  bool WriteRegisterHelper(CpuContext& rCpuCtxt, u32 Reg, IntType const& rVal)
  {
    auto RegBitSize = rCpuCtxt.GetCpuInformation().GetSizeOfRegisterInBit(Reg);
    if (RegBitSize != rVal.GetBitSize())
      return false;
    auto RegVal = rVal.ConvertTo<_Ty>();
    if (!rCpuCtxt.WriteRegister(Reg, &RegVal, RegBitSize))
      return false;
    return true;
  }
}

template<>
bool CpuContext::ReadRegister<IntType>(u32 Reg, IntType& rVal) const
{
  std::lock_guard<std::mutex> Lock(m_CpuLock);
  auto RegBitSize = m_rCpuInfo.GetSizeOfRegisterInBit(Reg);
  switch (RegBitSize)
  {
  default:  return false;
  case   1: return ReadRegisterHelper<  u8>(*this, Reg, rVal);
  case   8: return ReadRegisterHelper<  u8>(*this, Reg, rVal);
  case  16: return ReadRegisterHelper< u16>(*this, Reg, rVal);
  case  32: return ReadRegisterHelper< u32>(*this, Reg, rVal);
  case  64: return ReadRegisterHelper< u64>(*this, Reg, rVal);
  case 128: return ReadRegisterHelper<u128>(*this, Reg, rVal);
  case 256: return ReadRegisterHelper<u256>(*this, Reg, rVal);
  case 512: return ReadRegisterHelper<u512>(*this, Reg, rVal);
  }
}

template<>
bool CpuContext::WriteRegister<IntType>(u32 Reg, IntType const& rVal)
{
  std::lock_guard<std::mutex> Lock(m_CpuLock);
  auto RegBitSize = m_rCpuInfo.GetSizeOfRegisterInBit(Reg);
  switch (RegBitSize)
  {
  default:  return false;
  case   1: return WriteRegisterHelper<  u8>(*this, Reg, rVal);
  case   8: return WriteRegisterHelper<  u8>(*this, Reg, rVal);
  case  16: return WriteRegisterHelper< u16>(*this, Reg, rVal);
  case  32: return WriteRegisterHelper< u32>(*this, Reg, rVal);
  case  64: return WriteRegisterHelper< u64>(*this, Reg, rVal);
  case 128: return WriteRegisterHelper<u128>(*this, Reg, rVal);
  case 256: return WriteRegisterHelper<u256>(*this, Reg, rVal);
  case 512: return WriteRegisterHelper<u512>(*this, Reg, rVal);
  }
}

bool CpuContext::Translate(Address const& rLogicalAddress, u64& rLinearAddress) const
{
  std::lock_guard<std::mutex> Lock(m_CpuLock);
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
  std::lock_guard<std::mutex> Lock(m_CpuLock);
  m_AddressMap[rLogicalAddress] = LinearAddress;
  return true;
}

bool CpuContext::RemoveMapping(Address const& rLogicalAddress)
{
  std::lock_guard<std::mutex> Lock(m_CpuLock);
  auto itAddr = m_AddressMap.find(rLogicalAddress);
  if (itAddr == std::end(m_AddressMap))
    return false;
  m_AddressMap.erase(itAddr);
  return true;
}

namespace
{
  template<typename _Ty>
  bool ReadMemoryHelper(MemoryContext const& rMemCtxt, u64 LinAddr, IntType& rVal)
  {
    _Ty MemVal = 0;
    if (!rMemCtxt.ReadMemory(LinAddr, &MemVal, rVal.GetBitSize() / 8))
      return false;
    rVal = IntType(MemVal);
    return true;
  }

  template<typename _Ty>
  bool WriteMemoryHelper(MemoryContext& rMemCtxt, u64 LinAddr, IntType const& rVal)
  {
    auto MemVal = rVal.ConvertTo<_Ty>();
    if (!rMemCtxt.WriteMemory(LinAddr, &MemVal, rVal.GetBitSize() / 8))
      return false;
    return true;
  }
}

template<>
bool MemoryContext::ReadMemory<IntType>(u64 LinAddr, IntType& rVal) const
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  switch (rVal.GetBitSize())
  {
  default:  return false;
  case   8: return ReadMemoryHelper<  u8>(*this, LinAddr, rVal);
  case  16: return ReadMemoryHelper< u16>(*this, LinAddr, rVal);
  case  32: return ReadMemoryHelper< u32>(*this, LinAddr, rVal);
  case  64: return ReadMemoryHelper< u64>(*this, LinAddr, rVal);
  case 128: return ReadMemoryHelper<u128>(*this, LinAddr, rVal);
  case 256: return ReadMemoryHelper<u256>(*this, LinAddr, rVal);
  case 512: return ReadMemoryHelper<u512>(*this, LinAddr, rVal);
  }
}

template<>
bool MemoryContext::WriteMemory<IntType>(u64 LinAddr, IntType const& rVal)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  switch (rVal.GetBitSize())
  {
  default:  return false;
  case   8: return WriteMemoryHelper<  u8>(*this, LinAddr, rVal);
  case  16: return WriteMemoryHelper< u16>(*this, LinAddr, rVal);
  case  32: return WriteMemoryHelper< u32>(*this, LinAddr, rVal);
  case  64: return WriteMemoryHelper< u64>(*this, LinAddr, rVal);
  case 128: return WriteMemoryHelper<u128>(*this, LinAddr, rVal);
  case 256: return WriteMemoryHelper<u256>(*this, LinAddr, rVal);
  case 512: return WriteMemoryHelper<u512>(*this, LinAddr, rVal);
  }
}

MemoryContext::~MemoryContext(void)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  for (auto& rMemChunk : m_Memories)
    rMemChunk.m_spMemStrm->Close();
}

bool MemoryContext::ReadMemory(u64 LinearAddress, void* pValue, u32 ValueSize) const
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  MemoryChunk MemChnk;
  if (!_FindMemoryChunk(LinearAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = LinearAddress - MemChnk.m_LinearAddress;
  memcpy(pValue, reinterpret_cast<u8 const*>(MemChnk.m_spMemStrm->GetBuffer()) + Offset, ValueSize);
  return true;
}

bool MemoryContext::WriteMemory(u64 LinearAddress, void const* pValue, u32 ValueSize)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  MemoryChunk MemChnk;
  if (!_FindMemoryChunk(LinearAddress, MemChnk))
    return false;

  // LATER: Check boundary!
  auto Offset = LinearAddress - MemChnk.m_LinearAddress;
  memcpy(reinterpret_cast<u8 *>(MemChnk.m_spMemStrm->GetBuffer()) + Offset, pValue, ValueSize);
  return true;
}

void MemoryContext::ForEachMemoryChunk(CallbackType Callback)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  if (m_Memories.empty())
    return;

  for (MemoryChunk const& rMemChnk : m_Memories)
  {
    Callback(rMemChnk);
  }
}

bool MemoryContext::FindMemory(u64 LinAddr, BinaryStream::SPType& rspBinStrm, u32& rOffset, u32& rFlags) const
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  MemoryChunk MemChk;

  rOffset = 0;
  rFlags = 0;

  if (!_FindMemoryChunk(LinAddr, MemChk))
    return false;

  rspBinStrm = MemChk.m_spMemStrm;
  rOffset = LinAddr - MemChk.m_LinearAddress;
  rFlags = MemChk.m_Flags;
  return true;
}

bool MemoryContext::FindMemory(u64 LinAddr, void*& prAddress, u32& rOffset, u32& rSize, u32& rFlags) const
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);

  for (MemoryChunk const& rMemChnk : m_Memories)
  {
    if (LinAddr >= rMemChnk.m_LinearAddress && LinAddr < (rMemChnk.m_LinearAddress + rMemChnk.m_spMemStrm->GetSize()))
    {
      prAddress = rMemChnk.m_spMemStrm->GetBuffer();
      rOffset = LinAddr - rMemChnk.m_LinearAddress;
      rSize = rMemChnk.m_spMemStrm->GetSize();
      rFlags = rMemChnk.m_Flags;
      return true;
    }
  }
  return false;
}

bool MemoryContext::AllocateMemory(u64 LinAddr, u32 Size, u32 Flags, void** ppRawMemory)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  if (ppRawMemory)
    *ppRawMemory = nullptr;

  m_Memories.emplace_back(LinAddr, nullptr, Size, Flags);
  if (ppRawMemory != nullptr)
    *ppRawMemory = m_Memories.back().m_spMemStrm->GetBuffer();
  m_Memories.back().m_spMemStrm->Write(0x0, 0xfa, Size);
  std::sort(std::begin(m_Memories), std::end(m_Memories));
  return true;
}

bool MemoryContext::ProtectMemory(u64 LinAddr, u32 Flags)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  auto itMemChnk = std::find(std::begin(m_Memories), std::end(m_Memories), LinAddr);
  if (itMemChnk == std::end(m_Memories))
    return false;
  itMemChnk->m_Flags = Flags;
  return true;
}

bool MemoryContext::FreeMemory(u64 LinAddr)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  auto itMemChnk = std::find(std::begin(m_Memories), std::end(m_Memories), LinAddr);
  if (itMemChnk == std::end(m_Memories))
    return false;
  itMemChnk->m_spMemStrm->Close();
  m_Memories.erase(itMemChnk);
  return true;
}

bool MemoryContext::MapDocument(Document const& rDoc, CpuContext const* pCpuCtxt)
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  bool Res = true;
  rDoc.ForEachMemoryArea([&](MemoryArea const& rMemArea)
  {
    Address const& rMemAreaAddr = rMemArea.GetBaseAddress();
    u32 MemAreaSize             = rMemArea.GetSize();
    u32 MemAreaFileSize         = rMemArea.GetFileSize();
    u32 Size                    = std::max(MemAreaSize, MemAreaFileSize); // TODO: be more careful with that
    u32 Flags                   = rMemArea.GetAccess();

    void* pRawMemory;
    u64 LinearAddress;
    if (pCpuCtxt->Translate(rMemAreaAddr, LinearAddress) == false)
      LinearAddress = rMemAreaAddr.GetOffset();

    if (AllocateMemory(LinearAddress, Size, Flags, &pRawMemory) == false)
    {
      Res = false;
      return;
    }

    ::memset(pRawMemory, 0x0, Size);

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
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  if (m_Memories.empty())
    return "";

  std::ostringstream oss;
  for (MemoryChunk const& rMemChnk : m_Memories)
  {
    oss
      << "laddr: "  << std::hex << std::setw(16) << std::setfill('0') << rMemChnk.m_LinearAddress
      << ", size: " << std::hex << std::setw(8)  << std::setfill('0') << rMemChnk.m_spMemStrm->GetSize()
      << ", flags:" << std::hex << std::setw(8)  << std::setfill('0') << rMemChnk.m_Flags
      << ", rawb: " << rMemChnk.m_spMemStrm->GetBuffer() << std::endl;
    HexDump(oss, rMemChnk.m_spMemStrm->GetBuffer(), static_cast<u16>(rMemChnk.m_spMemStrm->GetSize()), rMemChnk.m_LinearAddress);
  }
  return oss.str();
}

bool MemoryContext::_FindMemoryChunk(u64 LinearAddress, MemoryChunk& rFoundMemChnk) const
{
  std::lock_guard<decltype(m_MemoryLock)> Lock(m_MemoryLock);
  for (MemoryChunk const& rMemChnk : m_Memories)
  {

    if (LinearAddress >= rMemChnk.m_LinearAddress && LinearAddress < (rMemChnk.m_LinearAddress + rMemChnk.m_spMemStrm->GetSize()))
    {
      rFoundMemChnk = rMemChnk;
      return true;
    }
  }
  return false;
}

MEDUSA_NAMESPACE_END