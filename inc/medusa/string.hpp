#ifndef __MEDUSA_STRING_HPP__
#define __MEDUSA_STRING_HPP__

#include "medusa/namespace.hpp"
#include "medusa/cell.hpp"

#include <cctype>

MEDUSA_NAMESPACE_BEGIN

class StringTrait
{
public:
  virtual bool IsValidCharacter(int Char) const = 0;
  virtual bool IsFinalCharacter(int Char) const = 0;
};

class AsciiString
{
public:
  virtual bool IsValidCharacter(int Char) const
  {
    switch (Char)
    {
      case '\a': case '\b': case '\t': case '\n':
      case '\v': case '\f': case '\r':
                                      return true;
      default:                        return !!isprint(Char);
    }
  }
  virtual bool IsFinalCharacter(int Char) const { return Char == '\0';  }
};

//! String is a MultiCell which handles a string.
class Medusa_EXPORT String : public Cell
{
public:
  String(std::string const& rCharacters = "") : Cell(Cell::StringType), m_Characters(rCharacters) {}

  virtual size_t GetLength(void)         const { return m_Characters.length() + 1; }
  std::string const& GetCharacters(void) const { return m_Characters;          }

  //virtual void Load(SerializeEntity::SPtr spSrlzEtt);
  //virtual SerializeEntity::SPtr Save(void);

protected:
  std::string m_Characters;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_STRING_HPP__