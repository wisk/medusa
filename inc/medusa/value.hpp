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

MEDUSA_NAMESPACE_BEGIN

//! Value is a cell which handles a value, this is a default cell type.
class Medusa_EXPORT Value : public Cell
{
public:
  Value(u32 ValueType = VT_UNK | VS_8BIT)
    : Cell(Cell::ValueType)
    , m_ValueType(ValueType) {}
  virtual ~Value(void) {}

  virtual size_t GetLength(void) const
  {
    switch (m_ValueType & VS_MASK)
    {
    case VS_8BIT: default: return sizeof(u8 );
    case VS_16BIT:         return sizeof(u16);
    case VS_32BIT:         return sizeof(u32);
    case VS_64BIT:         return sizeof(u64);
    }
  }

  u32 GetValueType(void) const { return m_ValueType; }

  //virtual void                  Load(SerializeEntity::SPtr spSrlzEtt);
  //virtual SerializeEntity::SPtr Save(void);

private:
  u32   m_ValueType;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_VALUE_