#ifndef _MEDUSA_MEMORY_AREA_
#define _MEDUSA_MEMORY_AREA_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/cell.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/serialize.hpp"

#include <string>
#include <vector>

MEDUSA_NAMESPACE_BEGIN

#define MA_READ    0x00000001
#define MA_WRITE   0x00000002
#define MA_EXEC    0x00000004

//! MemoryArea contains cells for a slice of memory.
class Medusa_EXPORT MemoryArea : public SerializeAccess
{
public:
  typedef std::pair<TOffset, Cell*> TCellPair;
  typedef std::vector<TCellPair>    TCellMap;
  typedef TCellMap::iterator        TIterator;
  typedef TCellMap::const_iterator  TConstIterator;

  virtual ~MemoryArea(void);

  Cell*                   RetrieveCell(TOffset Off);
  Cell const*             RetrieveCell(TOffset Off) const;
  bool                    InsertCell(TOffset Off, Cell* pCell, bool Force = false, bool Safe = true);


  std::string const&      GetName(void)        const   { return m_Name;                       }
  u64                     GetSize(void)        const   { return m_Cells.size();               }
  u32                     GetAccess(void)      const   { return m_Access;                     }

  bool                    Translate(TOffset VirtualOffset, TOffset& rPhysicalOffset) const;

  bool                    IsPresent(TOffset Off) const
  {
    if (m_VirtualBase.GetAddressingType() == Address::UnknownType)
      return false;

    return m_VirtualBase.IsBetween(GetSize(), Off);
  }

  bool                    IsPresent(Address const& Addr) const
  {
    if (m_VirtualBase.GetAddressingType() == Address::UnknownType)
      return false;

    return m_VirtualBase.IsBetween(GetSize(), Addr);
  }

  bool                    IsPresent(TAddressPtr spAddr) const
  {
    if (m_VirtualBase.GetAddressingType() == Address::UnknownType)
      return false;

    return m_VirtualBase.IsBetween(GetSize(), *spAddr.get());
  }

  TAddressPtr             MakeAddress(TOffset Offset) const
  {
    return TAddressPtr(new Address(
      m_VirtualBase.GetAddressingType(),
      m_VirtualBase.GetBase(),     Offset,
      m_VirtualBase.GetBaseSize(), m_VirtualBase.GetOffsetSize()
      ));
  }

  TIterator               Begin(void) { return m_Cells.begin(); }
  TIterator               End(void)   { return m_Cells.end();   }

  TConstIterator          Begin(void) const { return m_Cells.begin(); }
  TConstIterator          End(void)   const { return m_Cells.end();   }

  virtual bool            Read(TOffset Offset, void* pBuffer, u32 Size) const   { return false; }
  virtual bool            Write(TOffset Offset, void const* pBuffer, u32 Size)  { return false; }
  BinaryStream const&     GetBinaryStream(void) const { return m_BinStrm; }

  void                    SetEndianness(EEndianness Endianness) { m_BinStrm.SetEndianness(Endianness); }

  virtual void                  Load(SerializeEntity::SPtr SrlzEtt);
  virtual SerializeEntity::SPtr Save(void);

  Address const&  GetPhysicalBase(void) const { return m_PhysicalBase; }
  u32             GetPhysicalSize(void) const { return m_PhysicalSize; }

  Address const&  GetVirtualBase(void) const { return m_VirtualBase; }
  u32             GetVirtualSize(void) const { return m_VirtualSize; }

protected:
  MemoryArea(
    u8* pMemoryArea = NULL,
    std::string const& rName = "",
    Address const& rPhysicalBase = Address(), u32 PhysicalSize = 0x0,
    Address const& rVirtualBase = Address(),  u32 VirtualSize = 0x0,
    u32 Access = 0x0
    )
    : m_BinStrm(pMemoryArea, VirtualSize)
    , m_Name(rName)
    , m_PhysicalBase(rPhysicalBase),  m_PhysicalSize(PhysicalSize)
    , m_VirtualBase(rVirtualBase),    m_VirtualSize(VirtualSize)
    , m_Access(Access)
    , m_Cells()
  {}

  void                    CreateUnitializeCell(void);

  Cell*                   GetCell(TOffset Off);
  Cell const*             GetCell(TOffset Off) const;
  bool                    SetCell(TOffset Off, Cell* pCell);
  bool                    FillCell(TOffset Off);
  bool                    EraseCell(TOffset Off);

  void                    Sanitize(TOffset NewOff);

  bool                    GetPreviousCell(TOffset& rOff, Cell*& prInfo);
  bool                    GetNextCell(TOffset& rOff, Cell*& prInfo, size_t LimitSize = -1);

  std::string             m_Name;
  Address                 m_PhysicalBase;
  u32                     m_PhysicalSize;
  Address                 m_VirtualBase;
  u32                     m_VirtualSize;
  u32                     m_Access;
  TCellMap                m_Cells;
  MemoryBinaryStream      m_BinStrm;
};

//! PhysicalMemoryArea is a MemoryArea which contains cells present in file but not in memory.
class Medusa_EXPORT PhysicalMemoryArea : public MemoryArea
{
public:
  PhysicalMemoryArea(void) : MemoryArea() {}

  PhysicalMemoryArea(
    BinaryStream const& rBinStrm,
    std::string const& rName,
    Address const& rPhysicalBase, u32 PhysicalSize,
    u32 Access
    )
    : MemoryArea(NULL, rName, rPhysicalBase, PhysicalSize, Address(), 0x0, Access)
  {
    u8* pMemArea = new u8[PhysicalSize];
    rBinStrm.Read(rPhysicalBase.GetOffset(), pMemArea, PhysicalSize);
    m_BinStrm.Open(pMemArea, PhysicalSize);
    m_BinStrm.SetEndianness(rBinStrm.GetEndianness());

    CreateUnitializeCell();
  }

  virtual bool                  Read(TOffset Offset, void* pBuffer, u32 Size) const;
  virtual bool                  Write(TOffset Offset, void const* pBuffer, u32 Size);

  virtual void                  Load(SerializeEntity::SPtr SrlzEtt);
  virtual SerializeEntity::SPtr Save(void);
};

//! MappedMemoryArea is a MemoryArea which contains cells present in both file and memory.
class Medusa_EXPORT MappedMemoryArea : public MemoryArea
{
public:
  MappedMemoryArea(void) : MemoryArea() {}

  MappedMemoryArea(
    BinaryStream const& rBinStrm,
    std::string const& rName,
    Address const& rPhysicalBase, u32 PhysicalSize,
    Address const& rVirtualBase,  u32 VirtualSize,
    u32 Access
    )
    : MemoryArea(NULL, rName
    , rPhysicalBase, PhysicalSize
    , rVirtualBase,  VirtualSize
    , Access)
  {
    u8* pMemArea = new u8[VirtualSize];

    if (VirtualSize <= PhysicalSize)
      rBinStrm.Read(rPhysicalBase.GetOffset(), pMemArea, VirtualSize);
    else
    {
      rBinStrm.Read(rPhysicalBase.GetOffset(), pMemArea, PhysicalSize);
      u32 Diff = VirtualSize - PhysicalSize;
      memset(pMemArea + PhysicalSize, 0x0, Diff);
    }

    m_BinStrm.Open(pMemArea, VirtualSize);
    m_BinStrm.SetEndianness(rBinStrm.GetEndianness());

    CreateUnitializeCell();
  }

  virtual bool                  Read(TOffset Offset, void* pBuffer, u32 Size) const;
  virtual bool                  Write(TOffset Offset, void const* pBuffer, u32 Size);

  virtual void                  Load(SerializeEntity::SPtr SrlzEtt);
  virtual SerializeEntity::SPtr Save(void);
};

//! VirtualMemoryArea is a MemoryArea which contains cells present in memory but not in file.
class Medusa_EXPORT VirtualMemoryArea : public MemoryArea
{
public:
  VirtualMemoryArea(void) : MemoryArea() {}

  VirtualMemoryArea(
    EEndianness Endianness,
    std::string const& rName,
    Address const& rVirtualBase, u32 VirtualSize,
    u32 Access
    )
    : MemoryArea(NULL, rName, Address(), 0x0, rVirtualBase, VirtualSize, Access)
  {
    u8* pMemArea = new u8[VirtualSize];
    memset(pMemArea, 0x0, VirtualSize);
    m_BinStrm.Open(pMemArea, VirtualSize);
    m_BinStrm.SetEndianness(Endianness);

    CreateUnitializeCell();
  }

  virtual bool                  Read(TOffset Offset, void* pBuffer, u32 Size) const;
  virtual bool                  Write(TOffset Offset, void const* pBuffer, u32 Size);

  virtual void                  Load(SerializeEntity::SPtr SrlzEtt);
  virtual SerializeEntity::SPtr Save(void);

protected:
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_MEMORY_AREA_
