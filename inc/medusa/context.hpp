#ifndef __MEDUSA_CPU_HPP__
#define __MEDUSA_CPU_HPP__

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

protected:
  CpuInformation const& m_rCpuInfo;
  typedef std::unordered_map<Address, u64> AddressMap;
  AddressMap m_AddressMap;
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
};

class Medusa_EXPORT VariableContext
{
public:
  enum Type
  {
    VarUnknown = 0,
    Var1Bit    = 1,
    Var8Bit    = 8,
    Var16Bit   = 16,
    Var32Bit   = 32,
    Var64Bit   = 64,
  };

  virtual bool ReadVariable(std::string const& rVariableName, u64& rValue) const;
  virtual bool WriteVariable(std::string const& rVariableName, u64 Value, bool SignExtend = false);
  virtual void* GetVariable(std::string const& rVariableName);

  virtual bool AllocateVariable(u32 Type, std::string const& rVariableName);
  virtual bool FreeVariable(std::string const& rVariableName);

  virtual std::string ToString(void) const;

protected:
  struct VariableInformation
  {
    u32 m_Type;
    union
    {
      void* m_pValue;
      u64 m_Value;
    } u;

    VariableInformation(bool Value)
      : m_Type(Var1Bit) { u.m_Value = Value ? 1 : 0; }
    VariableInformation(u8 Value)
      : m_Type(Var8Bit) { u.m_Value = Value; }
    VariableInformation(u16 Value)
      : m_Type(Var16Bit) { u.m_Value = Value; }
    VariableInformation(u32 Value)
      : m_Type(Var32Bit) { u.m_Value = Value; }
    VariableInformation(u64 Value)
      : m_Type(Var64Bit) { u.m_Value = Value; }
    VariableInformation(u32 Type = VarUnknown, void* pValue = nullptr)
      : m_Type(Type) { u.m_pValue = pValue; }

    u32 GetSizeInBit(void) const { return m_Type; }
  };
  typedef std::unordered_map<std::string, VariableInformation> VariableMap;
  VariableMap m_Variables;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CPU_HPP__
