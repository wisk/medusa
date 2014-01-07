#ifndef _MEDUSA_CELL_DATA_
#define _MEDUSA_CELL_DATA_

#include <list>
#include <string>
#include <forward_list>
#include <memory>
#include <sstream>

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

  CellData(
      u8 Type = 0x0, u8 SubType = 0x0, u16 Size = 0x0,
      u16 FormatStyle = 0x0,
      u8  Flags = 0x0,
      Tag ArchTag = MEDUSA_ARCH_UNK,
      u8  Mode = 0)
    : m_Type(Type)
    , m_SubType(SubType)
    , m_Length(Size)
    , m_FormatStyle(FormatStyle)
    , m_Flags(Flags)
    , m_Mode(Mode)
    , m_ArchTag(ArchTag)
  {}

  std::string Dump(void) const
  {
    std::ostringstream oss;
    oss
      << std::hex << std::showbase
      << "dna("   << static_cast<int>(m_Type)
      << ", "     << static_cast<int>(m_SubType)
      << ", "     << m_Length
      << ", "     << m_FormatStyle
      << ", "     << static_cast<int>(m_Flags)
      << ", "     << static_cast<int>(m_Mode)
      << ", "     << m_ArchTag
      << ")";
    return oss.str();
  }

  u8  GetType(void)            const { return m_Type;        }
  u8  GetSubType(void)         const { return m_SubType;     }
  u16 GetLength(void)          const { return m_Length;      }
  u16 GetFormatStyle(void)     const { return m_FormatStyle; }
  Tag GetArchitectureTag(void) const { return m_ArchTag;     }
  u8  GetMode(void)            const { return m_Mode;        }

  u8&  SubType(void)      { return m_SubType;     }
  u16& Length(void)       { return m_Length;      }
  u16& FormatStyle(void)  { return m_FormatStyle; }
  u8&  Flags(void)        { return m_Flags;       }
  u8&  Mode(void)         { return m_Mode;        }

protected:
  u8  m_Type;         //! This field defines the type of cell
  u8  m_SubType;      //! This field defines the subtype of cell and is cell dependant
  u16 m_Length;       //! This field contains the size of cell

  u16 m_FormatStyle;  //! This field is reserved
  u8  m_Flags;        //! This field is reserved
  u8  m_Mode;         //! This field is given to the architecture to change the mode

  Tag m_ArchTag;      //! This field allows to identify the desired architecture
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CELL_DATA_
