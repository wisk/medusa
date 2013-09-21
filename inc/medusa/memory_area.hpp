#ifndef _MEDUSA_MEMORY_AREA_
#define _MEDUSA_MEMORY_AREA_

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
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

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
  MemoryArea(std::string const& rName, u32 Access) : m_Name(rName), m_Access(Access) {}

  // Information methods
  virtual std::string const& GetName(void)   const { return m_Name;   }
  virtual u32                GetAccess(void) const { return m_Access; }
  virtual u32                GetSize(void)   const = 0;
  virtual std::string        ToString(void)  const = 0;

  // Cell methods
  virtual CellData::SPtr GetCellData(TOffset Offset) const = 0;
  virtual bool           SetCellData(TOffset Offset, CellData::SPtr spCell, Address::List& rDeletedCellAddresses, bool Force) = 0;
  virtual bool           IsCellPresent(TOffset Offset) const = 0;

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
  u32 m_Access;
};

class Medusa_EXPORT MappedMemoryArea : public MemoryArea
{

public:
  MappedMemoryArea(
    std::string const& rName,
    TOffset FileOffset, u32 FileSize,
    Address const& rVirtualBase,  u32 VirtualSize,
    u32 Access
    )
    : MemoryArea(rName, Access)
    , m_FileOffset(FileOffset), m_FileSize(FileSize)
    , m_VirtualBase(rVirtualBase), m_VirtualSize(VirtualSize)
  {}

  virtual u32         GetSize(void)  const;
  virtual std::string ToString(void) const;

  virtual CellData::SPtr GetCellData(TOffset Offset) const;
  virtual bool           SetCellData(TOffset Offset, CellData::SPtr spCellData, Address::List& rDeletedCellAddresses, bool Force);
  virtual bool           IsCellPresent(TOffset Offset) const;

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
  bool _GetPreviousCellOffset(TOffset Offset, TOffset& rPreviousOffset) const;

  typedef std::pair<TOffset, CellData::SPtr> CellDataPairType;
  typedef std::vector<CellDataPairType> CellDataMapType;

  TOffset         m_FileOffset;
  u32             m_FileSize;
  Address         m_VirtualBase;
  u32             m_VirtualSize;
  CellDataMapType m_Cells;

  typedef boost::mutex    MutexType;
  mutable MutexType       m_Mutex;
};

class Medusa_EXPORT VirtualMemoryArea : public MemoryArea
{
public:
  VirtualMemoryArea(
    std::string const& rName,
    Address const& rVirtualBase,  u32 VirtualSize,
    u32 Access
    )
    : MemoryArea(rName, Access)
    , m_VirtualBase(rVirtualBase), m_VirtualSize(VirtualSize)
  {}

  virtual u32         GetSize(void)  const;
  virtual std::string ToString(void) const;

  virtual CellData::SPtr GetCellData(TOffset Offset) const;
  virtual bool           SetCellData(TOffset Offset, CellData::SPtr spCellData, Address::List& rDeletedCellAddresses, bool Force);
  virtual bool           IsCellPresent(TOffset Offset) const;

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
  u32 m_VirtualSize;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_MEMORY_AREA_
