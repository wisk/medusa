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

    if (!rDoc.GetBinaryStream().Read(MemAreaFileOff, pRawMemory, Size))
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
    u32 End  = rMemChnk.m_Size;
    u64 Addr = rMemChnk.m_LinearAddress;
    u8 const* pBuf = reinterpret_cast<u8 const*>(rMemChnk.m_Buffer);
    for (u32 Cur = 0; Cur < End; Cur += 0x10)
    {
      Addr += 0x10;
      oss << std::setw(16) << Addr << ":";
      for (u32 Line = 0; Line < 16 && Cur + Line < End; ++Line)
        oss << " " << std::setw(2) << static_cast<int>(pBuf[Cur + Line]);
      oss << "\n";
    }
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

bool VariableContext::ReadVariable(std::string const& rVariableName, u64& rValue) const
{
  auto const itVar = m_Variables.find(rVariableName);
  if (itVar == std::end(m_Variables))
    return false;

  switch (itVar->second.m_Type)
  {
  case Var1Bit:  rValue = (itVar->second.u.m_Value & 0x00000001); break;
  case Var8Bit:  rValue = (itVar->second.u.m_Value & 0x000000ff); break;
  case Var16Bit: rValue = (itVar->second.u.m_Value & 0x0000ffff); break;
  case Var32Bit: rValue = (itVar->second.u.m_Value & 0xffffffff); break;
  case Var64Bit: rValue = (itVar->second.u.m_Value);              break;
  default: return false;
  }

  return true;
}

bool VariableContext::WriteVariable(std::string const& rVariableName, u64 Value, bool SignExtend)
{
  auto itVar = m_Variables.find(rVariableName);
  if (itVar == std::end(m_Variables))
    return false;

  switch (itVar->second.m_Type)
  {
  case Var1Bit:  itVar->second.u.m_Value = (Value & 0x00000001); break;
  case Var8Bit:  itVar->second.u.m_Value = (Value & 0x000000ff); break;
  case Var16Bit: itVar->second.u.m_Value = (Value & 0x0000ffff); break;
  case Var32Bit: itVar->second.u.m_Value = (Value & 0xffffffff); break;
  case Var64Bit: itVar->second.u.m_Value = (Value);              break;
  default: return false;
  }

  return true;
}

void* VariableContext::GetVariable(std::string const& rVariableName)
{
  auto itVar = m_Variables.find(rVariableName);
  if (itVar == std::end(m_Variables))
    return nullptr;
  return itVar->second.u.m_pValue;
}

bool VariableContext::AllocateVariable(u32 Type, std::string const& rVariableName)
{
  FreeVariable(rVariableName);

  m_Variables[rVariableName] = VariableInformation(Type, 0x0);
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
    oss
      << "var: " << itVar->first
      << ", type: " << static_cast<int>(itVar->second.m_Type)
      << ", value: " << std::hex << std::setfill('0') << std::setw(itVar->second.m_Type / 8 * 2) << itVar->second.u.m_Value
      << std::endl;
  return oss.str();
}

MEDUSA_NAMESPACE_END
