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

  enum SubType
  {
    BaseMask        = 0x0f,
    BinaryType      = 0x01,
    DecimalType     = 0x02,
    HexadecimalType = 0x03,

    ModifierMask    = 0xf0,
    NotType         = 0x10,
    NegateType      = 0x20,
  };

  Value(u8 SubType = HexadecimalType, u16 Length = 1)
    : Cell(Cell::ValueType, SubType, Length)
  {}
  Value(CellData::SPtr spDna, std::string const& rComment = "") : Cell(spDna, rComment) {}
  virtual ~Value(void) {}
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_VALUE_