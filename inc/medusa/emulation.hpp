#ifndef __MEDUSA_EMULATION_HPP__
#define __MEDUSA_EMULATION_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/expression.hpp"
#include "medusa/cpu.hpp"
#include "medusa/database.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Emulator
{
public:
           Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt);
  virtual ~Emulator(void);

  virtual void ReadRegister (u32 Register,            void* pValue,       u32 ValueSize) const = 0;
  virtual void WriteRegister(u32 Register,            void const* pValue, u32 ValueSize)       = 0;
  virtual void ReadMemory   (Address const& rAddress, void* pValue,       u32 ValueSize) const = 0;
  virtual void WriteMemory  (Address const& rAddress, void const* pValue, u32 ValueSize)       = 0;

  virtual bool AllocateMemory(Address const& rAddress, u32 Size, void** ppRawMemory);
  virtual bool FreeMemory    (Address const& rAddress);
  virtual void MapDatabase   (Database const& rDatabase);

  virtual void Execute(Expression const& rExpr) = 0;
  virtual void Execute(Expression::List const& rExprList) = 0;
protected:
  CpuInformation const* m_pCpuInfo;
  CpuContext*           m_pCpuCtxt;

  struct MemoryChunk
  {
    Address m_Address;
    u32     m_Size;
    void*   m_Buffer;

    MemoryChunk(Address const& rAddress, u32 Size = 0x0, void* Buffer = nullptr)
      : m_Address(rAddress), m_Size(Size), m_Buffer(Buffer) {}

    bool operator<(MemoryChunk const& rMemChunk) const
    { return m_Address < rMemChunk.m_Address; }
  };

  typedef std::set<MemoryChunk> MemoryChunkSet;
  MemoryChunkSet m_Memories;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_EMULATION_HPP__