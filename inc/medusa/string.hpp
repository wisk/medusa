#ifndef MEDUSA_STRING_HPP
#define MEDUSA_STRING_HPP

#include "medusa/namespace.hpp"
#include "medusa/cell.hpp"

#include <cctype>

MEDUSA_NAMESPACE_BEGIN

class StringTrait
{
public:
  virtual bool        IsValidCharacter(int Char) const = 0;
  virtual bool        IsFinalCharacter(int Char) const = 0;
  virtual std::string ConvertToUtf8(void* pStrBuf, size_t StrBufLen) const = 0;
  virtual u16         CharacterLength(int Char) const = 0;
};

class Utf8StringTrait : public StringTrait
{
public:
  typedef s8          CharType;
  virtual bool        IsValidCharacter(int Char) const;
  virtual bool        IsFinalCharacter(int Char) const;
  virtual std::string ConvertToUtf8(void* pStrBuf, size_t StrBufLen) const;
  virtual u16         CharacterLength(int Char) const;
};

// TODO: Improve the handling of utf16 (it may require an external lib like ICU or iconv or utfcpp)
class Utf16StringTrait : public StringTrait
{
public:
  typedef s16         CharType;
  virtual bool        IsValidCharacter(int Char) const;
  virtual bool        IsFinalCharacter(int Char) const;
  virtual std::string ConvertToUtf8(void* pStrBuf, size_t StrBufLen) const;
  virtual u16         CharacterLength(int Char) const;
};

//! String is a Cell which handles a string.
class Medusa_EXPORT String : public Cell
{
public:
  enum
  {
    UnknownType,
    Utf8Type,
    Utf16Type,
  };

  String(u8 SubType, u16 Length)
    : Cell(Cell::StringType, SubType, Length)
  {}
  String(CellData::SPType spDna)
    : Cell(spDna) {}

  StringTrait const* GetStringTrait(void) const;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_STRING_HPP
