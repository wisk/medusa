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

  enum AddressKind
  {
    AddressUnknown,
    AddressExecution,
  };

  CpuContext(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}

  virtual bool ReadRegister(u32 Reg, void* pVal, u32 BitSize) const = 0;
  template<typename _RegTy>
  bool ReadRegister(u32 Reg, _RegTy& rVal) const
  {
    return ReadRegister(Reg, &rVal, sizeof(rVal) * 8);
  }

  virtual bool WriteRegister(u32 Reg, void const* pVal, u32 BitSize, bool SignExtend = false) = 0;
  template<typename _RegTy>
  bool WriteRegister(u32 Reg, _RegTy Val)
  {
    return WriteRegister(Reg, &Val, sizeof(Val) * 8);
  }

  virtual void* GetRegisterAddress(u32 Register) = 0;
  virtual void* GetContextAddress(void) = 0;
  virtual u16   GetRegisterOffset(u32 Register) = 0;

  virtual void  GetRegisters(RegisterList& RegList) const = 0;

  virtual bool Translate(Address const& rLogicalAddress, u64& rLinearAddress) const;
  virtual bool AddMapping(Address const& rLogicalAddress, u64 LinearAddress);
  virtual bool RemoveMapping(Address const& rLogicalAddress);

  virtual u8   GetMode(void) const = 0;
  virtual void SetMode(u8 Mode) = 0;

  virtual bool GetAddress(AddressKind AddrKind, Address& rAddr) const = 0;
  virtual bool SetAddress(AddressKind AddrKind, Address const& rAddr) = 0;

  virtual std::string ToString(void) const = 0;

  CpuInformation const& GetCpuInformation(void) const { return m_rCpuInfo; }

protected:
  CpuInformation const& m_rCpuInfo;
  typedef std::unordered_map<Address, u64> AddressMap;
  AddressMap m_AddressMap;
};

template<> Medusa_EXPORT bool CpuContext::ReadRegister<bool>(u32 Reg, bool& rVal) const;
template<> Medusa_EXPORT bool CpuContext::WriteRegister<bool>(u32 Reg, bool Val);

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

  virtual bool ReadMemory(u64 LinAddr, void* pVal, u32 Size) const;
  template<typename _MemTy>
  bool ReadMemory(u64 LinAddr, _MemTy& rVal) const
  {
    return ReadMemory(LinAddr, &rVal, sizeof(rVal));
  }

  virtual bool WriteMemory(u64 LinAddr, void const* pVal, u32 Size, bool SignExtend = false);
  template<typename _MemTy>
  bool WriteMemory(u64 LinAddr, _MemTy const& rVal)
  {
    return WriteMemory(LinAddr, &rVal, sizeof(rVal));
  }

  virtual bool FindMemory(u64 LinAddr, void*& prAddr, u32& rSize) const;

  virtual bool AllocateMemory(u64 LinAddr, u32 Size, void** ppRawMemory);
  virtual bool FreeMemory(u64 LinAddr);
  virtual bool MapDocument(Document const& rDoc, CpuContext const* pCpuCtxt);

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
