#ifndef __MEDUSA_STRING_HPP__
#define __MEDUSA_STRING_HPP__

#include "medusa/namespace.hpp"
#include "medusa/multicell.hpp"

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
class Medusa_EXPORT String : public MultiCell
{
public:
  String(u16 StrLen) : MultiCell(MultiCell::StringType, StrLen) {}

private:
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_STRING_HPP__