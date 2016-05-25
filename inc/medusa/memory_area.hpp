#ifndef MEDUSA_MEMORY_AREA_HPP
#define MEDUSA_MEMORY_AREA_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/cell.hpp"
#include "medusa/value.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include <string>
#include <vector>
#include <memory>
#include <mutex>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT MemoryArea
{
public:
  enum Access
  {
    Unknown = 0,
    Read    = 1 << 1,
    Write   = 1 << 2,
    Execute = 1 << 3,
  };

  struct Compare
  {
    bool operator()(MemoryArea const* pMA0, MemoryArea const* pMA1)
    {
      return pMA0->GetBaseAddress() < pMA1->GetBaseAddress();
    }
  };

  typedef std::function<void (TOffset, CellData::SPType)> CellDataPredicat;

  MemoryArea(
    std::string const& rName,
    u32 Access,
    Tag DefaultArchitectureTag,
    u8 DefaultArchitectureMode);
  virtual ~MemoryArea(void);

  virtual std::string Dump(void) const = 0;

  // Information methods
  virtual std::string const& GetName(void)             const { return m_Name;   }
  virtual u32                GetAccess(void)           const { return m_Access; }
  virtual u32                GetSize(void)             const = 0;
  virtual std::string        ToString(void)            const = 0;
  /*    */Tag                GetArchitectureTag(void)  const { return m_DefaultArchitectureTag; }
  /*    */u8                 GetArchitectureMode(void) const { return m_DefaultArchitectureMode; }

  // Set Architecture Tag and Mode
  void                       SetDefaultArchitectureTag(Tag arch);
  void                       SetDefaultArchitectureMode(u8 mode);

  virtual TOffset            GetFileOffset(void) const = 0;
  virtual u32                GetFileSize(void)   const = 0;

  // Cell methods
  virtual CellData::SPType GetCellData(TOffset Offset) const = 0;
  virtual bool           SetCellData(TOffset Offset, CellData::SPType spCell, Address::List& rDeletedCellAddresses, bool Force) = 0;
  virtual bool           SetCellData(TOffset Offset, CellData::SPType spCellData) = 0;
  virtual void           ForEachCellData(CellDataPredicat Predicat) const = 0;

  bool IsCellPresent(Address const& rAddress) const
  {
    if (GetBaseAddress().GetBase() != rAddress.GetBase())
      return false;
    return IsCellPresent(rAddress.GetOffset());
  }

  bool IsCellPresent(TOffset Offset) const
  { return GetBaseAddress().IsBetween(GetSize(), Offset); }

  // Address methods
  virtual Address GetBaseAddress(void) const = 0;
  virtual Address MakeAddress(TOffset Offset) const = 0;
  virtual bool    GetNextAddress(Address const& rAddress, Address& rNextAddress) const = 0;
  virtual bool    GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const = 0;

  virtual bool    MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const = 0;
  virtual bool    MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const = 0;
  virtual bool    MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const = 0;
  virtual bool    ConvertOffsetToPosition(TOffset Offset, u64& rPosition) const = 0;
  virtual bool    ConvertOffsetToFileOffset(TOffset Offset, TOffset& rFileOffset) const = 0;

protected:
  std::string m_Name;
  u32         m_Access;
  Tag         m_DefaultArchitectureTag;
  u8          m_DefaultArchitectureMode;
};

class Medusa_EXPORT MappedMemoryArea : public MemoryArea
{
public:
  MappedMemoryArea(
    std::string const& rName,
    TOffset FileOffset, u32 FileSize,
    Address const& rVirtualBase,  u32 VirtualSize,
    u32 Access,
    Tag DefaultArchitectureTag = MEDUSA_ARCH_UNK,
    u8 DefaultArchitectureMode = 0
    )
    : MemoryArea(rName, Access, DefaultArchitectureTag, DefaultArchitectureMode)
    , m_FileOffset(FileOffset), m_FileSize(FileSize)
    , m_VirtualBase(rVirtualBase), m_VirtualSize(VirtualSize)
  {}

  virtual ~MappedMemoryArea(void);

  virtual std::string Dump(void) const;

  virtual u32         GetSize(void)  const;
  virtual std::string ToString(void) const;

  virtual TOffset     GetFileOffset(void) const;
  virtual u32         GetFileSize(void)   const;

  virtual CellData::SPType GetCellData(TOffset Offset) const;
  virtual bool           SetCellData(TOffset Offset, CellData::SPType spCellData, Address::List& rDeletedCellAddresses, bool Force);
  virtual bool           SetCellData(TOffset Offset, CellData::SPType spCellData);

  virtual void           ForEachCellData(CellDataPredicat Predicat) const;

  virtual Address GetBaseAddress(void) const;
  virtual Address MakeAddress(TOffset Offset) const;
  virtual bool    GetNextAddress(Address const& rAddress, Address& rNextAddress) const;
  virtual bool    GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const;

  virtual bool    MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  virtual bool    MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  virtual bool    MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  virtual bool    ConvertOffsetToPosition(TOffset Offset, u64& rPosition) const;
  virtual bool    ConvertOffsetToFileOffset(TOffset Offset, TOffset& rFileOffset) const;

protected:
  bool _InsertCell(TOffset Offset, CellData::SPType spCellData);
  bool _RemoveCell(TOffset Offset, CellData::SPType spCellData);

  typedef std::vector<CellData::SPType> CellDataVectorType;

  TOffset            m_FileOffset;
  u32                m_FileSize;
  Address            m_VirtualBase;
  u32                m_VirtualSize;
  CellDataVectorType m_Cells;

  typedef std::mutex MutexType;
  mutable MutexType m_Mutex;
};

class Medusa_EXPORT VirtualMemoryArea : public MemoryArea
{
public:
  VirtualMemoryArea(
    std::string const& rName,
    Address const& rVirtualBase,  u32 VirtualSize,
    u32 Access,
    Tag DefaultArchitectureTag = MEDUSA_ARCH_UNK,
    u8 DefaultArchitectureMode = 0
    )
    : MemoryArea(rName, Access, DefaultArchitectureTag, DefaultArchitectureMode)
    , m_VirtualBase(rVirtualBase), m_VirtualSize(VirtualSize)
  {}

  virtual ~VirtualMemoryArea(void);

  virtual std::string Dump(void) const;

  virtual u32         GetSize(void)  const;
  virtual std::string ToString(void) const;

  virtual TOffset     GetFileOffset(void) const;
  virtual u32         GetFileSize(void)   const;

  virtual CellData::SPType GetCellData(TOffset Offset) const;
virtual bool           SetCellData(TOffset Offset, CellData::SPType spCellData, Address::List& rDeletedCellAddresses, bool Force);
virtual bool           SetCellData(TOffset Offset, CellData::SPType spCellData);
bool                   SetCellWithArchMode(TOffset Offset, Tag Arch, u8 Mode); //
  virtual void           ForEachCellData(CellDataPredicat Predicat) const;

  virtual Address GetBaseAddress(void) const;
  virtual Address MakeAddress(TOffset Offset) const;
  virtual bool    GetNextAddress(Address const& rAddress, Address& rNextAddress) const;
  virtual bool    GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const;

  virtual bool    MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  virtual bool    MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  virtual bool    MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  virtual bool    ConvertOffsetToPosition(TOffset Offset, u64& rPosition) const;
  virtual bool    ConvertOffsetToFileOffset(TOffset Offset, TOffset& rFileOffset) const;

protected:
  Address m_VirtualBase;
  u32     m_VirtualSize;
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_MEMORY_AREA_HPP
