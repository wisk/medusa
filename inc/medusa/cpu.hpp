#ifndef __MEDUSA_CPU_HPP__
#define __MEDUSA_CPU_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/database.hpp"

#include <string>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT CpuInformation
{
public:
  enum Type
  {
    StackPointerRegister,
    StackFrameRegister,
    ProgramPointerRegister,
    FlagRegister,
    InvalidRegister
  };

  virtual char const* ConvertIdentifierToName(u32 Id) const = 0;
  virtual u32         GetRegisterByType(Type RegType) const = 0;
  virtual u32         GetSizeOfRegisterInBit(u32 Id)  const = 0;
};

class Medusa_EXPORT CpuContext
{
public:
  CpuContext(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}

  virtual void ReadRegister (u32 Register, void*       pValue, u32 Size) const = 0;
  virtual void WriteRegister(u32 Register, void const* pValue, u32 Size)       = 0;

  virtual std::string ToString(void) const = 0;

protected:
  CpuInformation const& m_rCpuInfo;
};

class Medusa_EXPORT MemoryContext
{
public:
  MemoryContext(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}

  virtual void ReadMemory    (Address const& rAddress, void* pValue,       u32 ValueSize) const;
  virtual void WriteMemory   (Address const& rAddress, void const* pValue, u32 ValueSize);

  virtual bool AllocateMemory(Address const& rAddress, u32 Size, void** ppRawMemory);
  virtual bool FreeMemory    (Address const& rAddress);
  virtual void MapDatabase   (Database const& rDatabase);

  virtual std::string ToString(void) const;

protected:
  CpuInformation const& m_rCpuInfo;

  struct MemoryChunk
  {
    Address m_Address;
    u32     m_Size;
    void*   m_Buffer;

    MemoryChunk(Address const& rAddress = Address(), u32 Size = 0x0, void* Buffer = nullptr)
      : m_Address(rAddress), m_Size(Size), m_Buffer(Buffer) {}

    bool operator<(MemoryChunk const& rMemChunk) const
    { return m_Address < rMemChunk.m_Address; }
  };

  typedef std::set<MemoryChunk> MemoryChunkSet;
  MemoryChunkSet m_Memories;

  virtual bool FindMemoryChunk(Address const& rAddress, MemoryChunk& rMemChnk) const;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CPU_HPP__
