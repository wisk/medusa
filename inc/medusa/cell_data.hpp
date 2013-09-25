#ifndef _MEDUSA_CELL_DATA_
#define _MEDUSA_CELL_DATA_

#include <list>
#include <string>
#include <forward_list>
#include <memory>

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

class CellData
{
public:
  typedef std::shared_ptr<CellData> SPtr;

  CellData(u8 Type, u8 SubType, u16 Size = 0x0, u8 FormatStyle = 0x0, u32 Flags = 0x0, Tag ArchTag = MEDUSA_ARCH_UNK)
    : m_Type(Type)
    , m_SubType(SubType)
    , m_Length(Size)
    , m_FormatStyle(FormatStyle)
    , m_Flags(Flags)
    , m_ArchTag(ArchTag)
  {}

  u8  GetType(void)            const { return m_Type;        }
  u8  GetSubType(void)         const { return m_SubType;     }
  u16 GetLength(void)          const { return m_Length;      }
  u32 GetFormatStyle(void)     const { return m_FormatStyle; }
  Tag GetArchitectureTag(void) const { return m_ArchTag;     }

  u16& Length(void)       { return m_Length;      }
  u32& FormatStyle(void)  { return m_FormatStyle; }
  u32& Flags(void)        { return m_Flags;       }

private:
  u8  m_Type;
  u8  m_SubType;
  u16 m_Length;
  u32 m_FormatStyle;
  u32 m_Flags;
  Tag m_ArchTag;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CELL_DATA_