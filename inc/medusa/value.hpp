#ifndef _MEDUSA_VALUE_
#define _MEDUSA_VALUE_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/cell.hpp"

#include <sstream>
#include <iomanip>

// Value size
#define VS_MASK   0x000000ff  //! Mask for Value size
#define VS_8BIT   0x00000001
#define VS_16BIT  0x00000002
#define VS_32BIT  0x00000004
#define VS_64BIT  0x00000008

// Value representation
#define VT_MASK   ~VS_MASK    //! Mask for Value type
#define VT_BIN    0x00000100
#define VT_OCT    0x00000200
#define VT_DEC    0x00000400
#define VT_HEX    0x00000800
#define VT_FLOAT  0x00001000
#define VT_UNK    0x00002000

#define VT_NOT    0x00010000
#define VT_NEG    0x00020000

MEDUSA_NAMESPACE_BEGIN

//! Value is a cell which handles a value, this is a default cell type.
class Medusa_EXPORT Value : public Cell
{
public:
  typedef std::shared_ptr<Value> SPtr;

  Value(u32 ValueType = VT_UNK | VS_8BIT)
    : Cell(CellData::ValueType)
    , m_ValueType(ValueType)
  {
    switch (m_ValueType & VS_MASK)
    {
    case VS_8BIT: default: m_spDna->Length() = sizeof(u8 ); break;
    case VS_16BIT:         m_spDna->Length() = sizeof(u16); break;
    case VS_32BIT:         m_spDna->Length() = sizeof(u32); break;
    case VS_64BIT:         m_spDna->Length() = sizeof(u64); break;
    }
  }
  Value(CellData::SPtr spDna, std::string const& rComment = "") : Cell(spDna, rComment) {}
  virtual ~Value(void) {}

  u32 GetValueType(void) const { return m_ValueType; }

private:
  u32   m_ValueType;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_VALUE_