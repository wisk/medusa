#ifndef MEDUSA_MEMORY_AREA_HPP
#define MEDUSA_MEMORY_AREA_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/bits.hpp"
#include "medusa/bitmask.hpp"
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

class MEDUSA_EXPORT MemoryArea
{
public:
  enum class Access : u8
  {
    NoAccess = 0,
    Read     = 1 << 1,
    Write    = 1 << 2,
    Execute  = 1 << 3,
  };

  enum Type : u8
  {
    UnknownType,
    VirtualType,
    MappedType,
    PhysicalType,
  };

  struct Compare
  {
    bool operator()(MemoryArea const* pMA0, MemoryArea const* pMA1)
    {
      return pMA0->GetBaseAddress() < pMA1->GetBaseAddress();
    }
  };

  typedef std::function<void (OffsetType, CellData::SPType)> CellDataPredicat;

  MemoryArea(void)
    : m_Id(), m_Name("")
    , m_Type(UnknownType), m_Access(Access::NoAccess)
    , m_DefaultArchitectureTag(MEDUSA_ARCH_UNK), m_DefaultArchitectureMode()
    , m_FileOffset(), m_FileSize()
    , m_BaseAddress(), m_Size()
  {}

  static MemoryArea CreateVirtual(
    std::string const& rName, Access Access,
    Address const& rBaseAddress, u32 Size,
    Tag DefaultArchitectureTag = MEDUSA_ARCH_UNK,
    u8 DefaultArchitectureMode = 0
  )
  {
    MemoryArea MemArea(rName, Access, DefaultArchitectureTag, DefaultArchitectureMode);
    MemArea.m_Type = VirtualType;
    MemArea.m_BaseAddress = rBaseAddress;
    MemArea.m_Size = Size;
    return MemArea;
  }

  static MemoryArea CreateMapped(
    std::string const& rName, Access Access,
    u32 FileOffset, u32 FileSize,
    Address const& rBaseAddress, u32 Size,
    Tag DefaultArchitectureTag = MEDUSA_ARCH_UNK,
    u8 DefaultArchitectureMode = 0
  )
  {
    MemoryArea MemArea(rName, Access, DefaultArchitectureTag, DefaultArchitectureMode);
    MemArea.m_Type = MappedType;
    MemArea.m_FileOffset = FileOffset;
    MemArea.m_FileSize = FileSize;
    MemArea.m_BaseAddress = rBaseAddress;
    MemArea.m_Size = Size;
    return MemArea;
  }

  static MemoryArea CreatePhysical(
    std::string const& rName, Access Access,
    u32 FileOffset, u32 FileSize,
    Tag DefaultArchitectureTag = MEDUSA_ARCH_UNK,
    u8 DefaultArchitectureMode = 0
  )
  {
    MemoryArea MemArea(rName, Access, DefaultArchitectureTag, DefaultArchitectureMode);
    MemArea.m_Type = PhysicalType;
    MemArea.m_FileOffset = FileOffset;
    MemArea.m_FileSize = FileSize;
    MemArea.m_BaseAddress = Address(Address::PhysicalType, 0xffff, FileOffset);
    MemArea.m_Size = FileSize;
    return MemArea;
  }

  std::string ToString(void) const;

  // Information methods
  std::string const& GetName(void)             const { return m_Name;                    }
  Type               GetType(void)             const { return m_Type;                    }
  Access             GetAccess(void)           const { return m_Access;                  }
  Tag                GetArchitectureTag(void)  const { return m_DefaultArchitectureTag;  }
  u8                 GetArchitectureMode(void) const { return m_DefaultArchitectureMode; }

  // Set Architecture Tag and Mode
  void SetDefaultArchitectureTag(Tag arch);
  void SetDefaultArchitectureMode(u8 mode);

  void SetName(std::string const& Name) { m_Name = Name;     }
  void SetAccess(Access Access)         { m_Access = Access; }
  void SetId(u32 Id)                    { m_Id = Id;         }

  OffsetType     GetFileOffset(void)  const { return m_FileOffset;  }
  u32            GetFileSize(void)    const { return m_FileSize;    }
  Address const& GetBaseAddress(void) const { return m_BaseAddress; }
  u32            GetSize(void)        const { return m_Size;        }
  u32            GetId(void)          const { return m_Id;          }

protected:
  u32         m_Id;
  std::string m_Name;
  Type        m_Type;
  Access      m_Access;

  Tag         m_DefaultArchitectureTag;
  u8          m_DefaultArchitectureMode;

  OffsetType  m_FileOffset;
  u32         m_FileSize;
  Address     m_BaseAddress;
  u32         m_Size;

private:
  MemoryArea(
    std::string const& rName,
    Access Access,
    Tag DefaultArchitectureTag,
    u8 DefaultArchitectureMode);
};

MEDUSA_ENUM_CLASS_BITMASK(MemoryArea::Access)

MEDUSA_NAMESPACE_END

#endif // MEDUSA_MEMORY_AREA_HPP
