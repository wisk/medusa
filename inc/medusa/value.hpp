#ifndef _MEDUSA_VALUE_
#define _MEDUSA_VALUE_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/cell.hpp"

#include <sstream>
#include <iomanip>

MEDUSA_NAMESPACE_BEGIN

//! Value is a cell which handles a value, this is a default cell type.
class Medusa_EXPORT Value : public Cell
{
public:
  typedef std::shared_ptr<Value> SPtr;

  /*!
  **
  ** Value type:
  **  - {s,u}{8,16,32,64}
  **  - float not implemented
  **  - composite → include an another element
  **  - reference → pointer to an another element
  **  - relative  → the another element is located at the beginning of struct +
  **  field value
  **  - intrusive → the another element is located at field value - offsetof field
  **
  */

  enum
  {
    BaseMask        = 0x0f,
    BinaryType      = 0x01,
    DecimalType     = 0x02,
    HexadecimalType = 0x03,
    FloatType       = 0x04,
    CharacterType   = 0x05,
    TagType         = 0x06,
    CompositeType   = 0x07,
    ReferenceType   = 0x08,
    RelativeType    = 0x09,
    InstrusiveType  = 0x0a,


    ModifierMask    = 0xf0,
    NotType         = 0x10,
    NegateType      = 0x20,
  };

  Value(u8 SubType = HexadecimalType, u16 Length = 1)
    : Cell(Cell::ValueType, SubType, Length)
  {}
  Value(CellData::SPtr spDna) : Cell(spDna) {}
  virtual ~Value(void) {}

  void Modify(u8&  rValue) const;
  void Modify(u16& rValue) const;
  void Modify(u32& rValue) const;
  void Modify(u64& rValue) const;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_VALUE_