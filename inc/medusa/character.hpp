#ifndef __MEDUSA_CHARACTER_HPP__
#define __MEDUSA_CHARACTER_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/cell.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Character : public Cell
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

  Character(CellData::SPtr spDna, std::string const& rComment = "")
    : Cell(spDna, rComment) {}
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CHARACTER_HPP__