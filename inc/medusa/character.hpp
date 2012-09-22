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
  enum CharacterType
  {
    UnknownCharacterType,
    AsciiCharacterType
  };

  Character(CharacterType CharType = UnknownCharacterType)
    : Cell(CellData::CharacterType)
    , m_CharType(CharType)
  {}

  // Since only ascii character is implemented we can simply return 1, this should be changed if we support more character type.
  virtual size_t GetLength(void) const { return 1; }

  CharacterType GetCharacterType(void) const { return m_CharType; }

private:
  CharacterType m_CharType;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CHARACTER_HPP__