#ifndef MEDUSA_CPU_HPP
#define MEDUSA_CPU_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/document.hpp"
#include "medusa/information.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT CpuContext
{
public:
  typedef std::list<u32> RegisterList;

  virtual ~CpuContext() {}

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

  virtual bool WriteRegister(u32 Reg, void const* pVal, u32 BitSize) = 0;
  template<typename _RegTy>
  bool WriteRegister(u32 Reg, _RegTy const& rVal)
  {
    return WriteRegister(Reg, &rVal, sizeof(rVal) * 8);
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

  virtual u32  GetException(void) const { return m_Exception; }
  virtual void SetException(u32 Excpt) { m_Exception = Excpt; }

  virtual std::string ToString(void) const = 0;

  CpuInformation const& GetCpuInformation(void) const { return m_rCpuInfo; }

protected:
  CpuInformation const& m_rCpuInfo;
  typedef std::unordered_map<Address, u64> AddressMap;
  AddressMap m_AddressMap;
  u32 m_Exception;
  mutable std::mutex m_CpuLock;
};

template<> MEDUSA_EXPORT bool CpuContext::ReadRegister<bool>(u32 Reg, bool& rVal) const;
template<> MEDUSA_EXPORT bool CpuContext::WriteRegister<bool>(u32 Reg, bool const& rVal);

template<> MEDUSA_EXPORT bool CpuContext::ReadRegister<BitVector>(u32 Reg, BitVector& rVal) const;
template<> MEDUSA_EXPORT bool CpuContext::WriteRegister<BitVector>(u32 Reg, BitVector const& rVal);

class MEDUSA_EXPORT MemoryContext
{
public:
  struct MemoryChunk
  {
    u64                        m_LinearAddress;
    MemoryArea::Access         m_Flags;
    MemoryBinaryStream::SPType m_spMemStrm;

    MemoryChunk(u64 LinAddr = 0x0, void* Buffer = nullptr, u32 Size = 0x0, MemoryArea::Access Flags = MemoryArea::Access::NoAccess)
      : m_LinearAddress(LinAddr), m_spMemStrm(std::make_shared<MemoryBinaryStream>(Buffer, Size)), m_Flags(Flags) {}

    bool operator<(MemoryChunk const& rMemChunk) const
    { return m_LinearAddress < rMemChunk.m_LinearAddress; }

    bool operator==(MemoryChunk const& rMemChunk) const
    { return m_LinearAddress == rMemChunk.m_LinearAddress; }

    bool operator==(u64 LinAddr) const
    { return m_LinearAddress == LinAddr; }
  };

  MemoryContext(CpuInformation const& rCpuInfo) : m_rCpuInfo(rCpuInfo) {}
  ~MemoryContext(void);

  typedef std::function<void (MemoryChunk const& rMemChunk)> CallbackType;
  void ForEachMemoryChunk(CallbackType Callback);
  virtual bool ReadMemory(u64 LinAddr, void* pVal, u32 Size) const;
  template<typename _MemTy>
  bool ReadMemory(u64 LinAddr, _MemTy& rVal) const
  {
    return ReadMemory(LinAddr, &rVal, sizeof(rVal));
  }

  virtual bool WriteMemory(u64 LinAddr, void const* pVal, u32 Size);
  template<typename _MemTy>
  bool WriteMemory(u64 LinAddr, _MemTy const& rVal)
  {
    return WriteMemory(LinAddr, &rVal, sizeof(rVal));
  }

  virtual bool FindMemory(u64 LinAddr, BinaryStream::SPType& rspBinStrm, u32& rOffset, MemoryArea::Access& rFlags) const;
  virtual bool FindMemory(u64 LinAddr, void*& prAddr, u32& rOffset, u32& rSize, MemoryArea::Access& rFlags) const;

  virtual bool AllocateMemory(u64 LinAddr, u32 Size, MemoryArea::Access Flags, void** ppRawMemory);
  virtual bool ProtectMemory(u64 LinAddr, MemoryArea::Access Flags); // TODO: add protection by pages
  virtual bool FreeMemory(u64 LinAddr);
  virtual bool MapDocument(Document const& rDoc, CpuContext const* pCpuCtxt);

  virtual std::string ToString(void) const;

protected:
  virtual bool _FindMemoryChunk(u64 LinearAddress, MemoryChunk& rMemChnk) const;

  CpuInformation const& m_rCpuInfo;

  typedef std::vector<MemoryChunk> MemoryChunksType;
  MemoryChunksType m_Memories;
  mutable std::recursive_mutex m_MemoryLock;

private:
  MemoryContext(MemoryContext const&);
  MemoryContext const& operator=(MemoryContext const&);
};

class StaticContext
{
public:
  StaticContext(CpuInformation const& rCpuInformation) : m_rCpuInformation(rCpuInformation) {}

  bool GetRegister(std::string const& rRegisterName, void* pValue, size_t ValueSize);
  bool SetRegister(std::string const& rRegisterName, void const* pValue, size_t ValueSize);

  bool GetFeature(std::string const& rFeatureName, void* pValue, size_t ValueSize);
  bool SetFeature(std::string const& rFeatureName, void const* pValue, size_t ValueSize);

private:
  CpuInformation const& m_rCpuInformation;
};

template<> MEDUSA_EXPORT bool MemoryContext::ReadMemory<BitVector>(u64 LinAddr, BitVector& rVal) const;
template<> MEDUSA_EXPORT bool MemoryContext::WriteMemory<BitVector>(u64 LinAddr, BitVector const& rVal);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CPU_HPP
