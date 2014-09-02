#ifndef MEDUSA_VALUE_HPP
#define MEDUSA_VALUE_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/cell.hpp"
#include "medusa/detail.hpp"

#include <sstream>
#include <iomanip>

MEDUSA_NAMESPACE_BEGIN

//! Value is a cell which handles a value, this is a default cell type.
class Medusa_EXPORT Value : public Cell
{
public:
  typedef std::shared_ptr<Value> SPtr;

  Value(u8 SubType = ValueDetail::HexadecimalType, u16 Length = 1)
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

#endif // !MEDUSA_VALUE_HPP