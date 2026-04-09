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
class MEDUSA_EXPORT Value : public Cell
{
public:
  using SPType = std::shared_ptr<Value>;

  explicit Value(u8 SubType = ValueDetail::HexadecimalType, u16 Length = 1)
    : Cell(Cell::ValueType, SubType, Length)
  {}
  explicit Value(CellData::SPType spDna) : Cell(spDna) {}
  virtual ~Value() = default;

  void Modify(u8&  rValue) const;
  void Modify(u16& rValue) const;
  void Modify(u32& rValue) const;
  void Modify(u64& rValue) const;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_VALUE_HPP