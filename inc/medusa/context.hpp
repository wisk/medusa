#ifndef MEDUSA_CPU_HPP
#define MEDUSA_CPU_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/document.hpp"
#include "medusa/information.hpp"

#include <set>
#include <string>
#include <unordered_map>
#include <functional>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT CpuContext
{
public:
  typedef std::list<u32> RegisterList;

  CpuContext(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}

  virtual bool  ReadRegister (u32 Register, void*       pValue, u32 Size) const = 0;
  virtual bool  WriteRegister(u32 Register, void const* pValue, u32 Size, bool SignExtend = false) = 0;
  virtual void* GetRegisterAddress(u32 Register) = 0;
  virtual void* GetContextAddress(void) = 0;
  virtual u16   GetRegisterOffset(u32 Register) = 0;

  virtual void  GetRegisters(RegisterList& RegList) const = 0;

  virtual bool Translate(Address const& rLogicalAddress, u64& rLinearAddress) const;
  virtual bool AddMapping(Address const& rLogicalAddress, u64 LinearAddress);
  virtual bool RemoveMapping(Address const& rLogicalAddress);

  virtual std::string ToString(void) const = 0;

  CpuInformation const& GetCpuInformation(void) const { return m_rCpuInfo; }

  u8   GetMode(void) const { return m_Mode; }
  void SetMode(u8 Mode)    { m_Mode = Mode; }

protected:
  CpuInformation const& m_rCpuInfo;
  typedef std::unordered_map<Address, u64> AddressMap;
  AddressMap m_AddressMap;
  u8 m_Mode;
};

class Medusa_EXPORT MemoryContext
{
public:
  struct MemoryChunk
  {
    u64   m_LinearAddress;
    u32   m_Size;
    void* m_Buffer;

    MemoryChunk(u64 Address = 0, u32 Size = 0x0, void* Buffer = nullptr)
      : m_LinearAddress(Address), m_Size(Size), m_Buffer(Buffer) {}

    bool operator<(MemoryChunk const& rMemChunk) const
    { return m_LinearAddress < rMemChunk.m_LinearAddress; }
  };

  MemoryContext(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}
  ~MemoryContext(void);

  virtual bool ReadMemory(u64 LinearAddress, void* pValue,       u32 ValueSize) const;
  virtual bool WriteMemory(u64 LinearAddress, void const* pValue, u32 ValueSize, bool SignExtend = false);
  virtual bool FindMemory(u64 LinearAddress, void*& prAddress, u32& rSize) const;

  virtual bool AllocateMemory(u64 LinearAddress, u32 Size, void** ppRawMemory);
  virtual bool FreeMemory    (u64 LinearAddress);
  virtual bool MapDocument   (Document const& rDoc, CpuContext const* pCpuCtxt);

  virtual std::string ToString(void) const;

protected:
  virtual bool FindMemoryChunk(u64 LinearAddress, MemoryChunk& rMemChnk) const;

  CpuInformation const& m_rCpuInfo;

  typedef std::set<MemoryChunk> MemoryChunkSet;
  MemoryChunkSet m_Memories;

private:
  MemoryContext(MemoryContext const&);
  MemoryContext const& operator=(MemoryContext const&);
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CPU_HPP
