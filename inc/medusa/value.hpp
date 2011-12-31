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


MEDUSA_NAMESPACE_BEGIN

//! Value is a cell which handles a value, this is a default cell type.
template<typename T> class Medusa_EXPORT Value : public Cell
{
public:
  Value(T Data = 0x0, u32 ValueType = VT_HEX | VS_8BIT)
    : Cell(Cell::ValueType),  m_Data(Data), m_ValueType(ValueType) {}
  virtual ~Value(void) {}

  T GetData(void) const
  {
    return (m_Data);
  }

  virtual std::string ToString(void) const
  {
    std::ostringstream oss;

    oss << std::setfill('0');

    switch (m_ValueType & VS_MASK)
    {
    case VS_8BIT: default:  oss << "db "; break;
    case VS_16BIT:          oss << "dw "; break;
    case VS_32BIT:          oss << "dd "; break;
    case VS_64BIT:          oss << "dq "; break;
    }

    switch (m_ValueType & VT_MASK)
    {
    case VT_BIN:                                      break; // TODO: Unimplemented
    case VT_OCT:            oss << std::oct;          break;
    case VT_DEC: default:                             break;
    case VT_HEX:            oss << std::hex << "0x";  break;
    }

    switch (m_ValueType & VS_MASK)
    {
    case VS_8BIT: default:  oss << std::setw(2)  << static_cast<int>(static_cast<u8>(m_Data));  break;
    case VS_16BIT:          oss << std::setw(4)  << static_cast<u16>(m_Data);                   break;
    case VS_32BIT:          oss << std::setw(8)  << static_cast<u32>(m_Data);                   break;
    case VS_64BIT:          oss << std::setw(16) << m_Data;                                     break;
    }

    return oss.str();
  }

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

  virtual void                  Load(SerializeEntity::SPtr spSrlzEtt);
  virtual SerializeEntity::SPtr Save(void);

private:
  T     m_Data;
  u32   m_ValueType;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_VALUE_
