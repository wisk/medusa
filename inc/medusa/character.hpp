#ifndef MEDUSA_CHARACTER_HPP
#define MEDUSA_CHARACTER_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/cell.hpp"

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Character : public Cell
{
public:
  enum
  {
    UnknownCharacterType,
    AsciiCharacterType
  };

  Character(u8 SubType = UnknownCharacterType, u16 Length = 1)
    : Cell(Cell::CharacterType, SubType, Length)
  {}

  Character(CellData::SPType spDna) : Cell(spDna) {}
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CHARACTER_HPP