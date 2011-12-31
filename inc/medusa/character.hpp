#ifndef __MEDUSA_CHARACTER_HPP__
#define __MEDUSA_CHARACTER_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/cell.hpp"

MEDUSA_NAMESPACE_BEGIN

template<typename T> class Medusa_EXPORT Character : public Cell
{
public:
  enum CharacterType
  {
    UnknownCharacterType,
    AsciiCharacterType
  };

  Character(T Char = 0x0, CharacterType CharType = UnknownCharacterType)
    : m_Char(Char)
    , m_CharType(CharType)
  {}

  virtual std::string ToString(void) const
  {
    std::string CharFmt = "' '";
    CharFmt[1] = m_Char;
    return CharFmt;
  }

  virtual size_t GetLength(void) const { return sizeof m_Char; }

  CharacterType GetCharacterType(void) const { return m_CharType; }

private:
  T m_Char;
  CharacterType m_CharType;
};

typedef Character<char> AsciiCharacter;

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CHARACTER_HPP__
