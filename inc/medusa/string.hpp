#ifndef __MEDUSA_STRING_HPP__
#define __MEDUSA_STRING_HPP__

#include "medusa/namespace.hpp"
#include "medusa/cell.hpp"

#include <cctype>

MEDUSA_NAMESPACE_BEGIN

class StringTrait
{
public:
  virtual bool        IsValidCharacter(int Char) const = 0;
  virtual bool        IsFinalCharacter(int Char) const = 0;
  virtual std::string ConvertToUf8(int Char)     const = 0;
  virtual u16         CharacterLength(int Char)  const = 0;
};

class AsciiString : public StringTrait
{
public:
  typedef u8 CharType;
  virtual bool IsValidCharacter(int Char) const
  {
    switch (Char)
    {
      case '\a': case '\b': case '\t': case '\n':
      case '\v': case '\f': case '\r':
                                      return true;
      default:                        return !!isprint(Char & 0xff);
    }
  }
  virtual bool IsFinalCharacter(int Char) const { return Char == '\0';  }
  virtual std::string ConvertToUf8(int Char) const { return std::string(1, Char); }
  virtual u16 CharacterLength(int Char) const { return 1; }
};

// TODO: Improve the handling of utf16 (it may require an external lib like ICU or iconv or utfcpp)
class WinString : public StringTrait
{
public:
  typedef u16 CharType;
  virtual bool IsValidCharacter(int Char) const
  {
    if ((Char & 0xff00) != 0x0) return false;

    switch (Char & 0xff)
    {
      case '\a': case '\b': case '\t': case '\n':
      case '\v': case '\f': case '\r':
                                      return true;
      default:                        return !!isprint(Char & 0xff);
    }

  }

  virtual bool IsFinalCharacter(int Char) const { return (Char & 0xffff) == 0x0000; }
  virtual std::string ConvertToUf8(int Char) const { return std::string(1, static_cast<char>(Char & 0xff)); }
  virtual u16 CharacterLength(int Char) const { return 2; }
};

//! String is a Cell which handles a string.
class Medusa_EXPORT String : public Cell
{
public:
  enum StringType
  {
    UnknownType,
    AsciiType,
    Utf16Type
  };

  String(StringType Type, u16 StringLength)
    : Cell(CellData::StringType, StringLength)
    , m_StringType(Type) {}
  String(CellData::SPtr spDna, std::string const& rComment = "")
    : Cell(spDna, rComment) {}

  StringType          GetStringType(void) const { return m_StringType; }

protected:
  StringType  m_StringType;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_STRING_HPP__
