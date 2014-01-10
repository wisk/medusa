#include "medusa/string.hpp"

#include <boost/locale.hpp>

MEDUSA_NAMESPACE_BEGIN

bool Utf8StringTrait::IsValidCharacter(int Char) const
{
  switch (Char)
  {
  case '\a': case '\b': case '\t': case '\n':
  case '\v': case '\f': case '\r':
    return true;
  default:                        return !!isprint(Char & 0xff);
  }
}

bool Utf8StringTrait::IsFinalCharacter(int Char) const
{
  return Char == '\0';
}

std::string Utf8StringTrait::ConvertToUtf8(void* pStrBuf, size_t StrBufLen) const
{
  return std::string(
    reinterpret_cast<char const*>(pStrBuf),
    reinterpret_cast<char const*>(pStrBuf) + StrBufLen);
}

u16 Utf8StringTrait::CharacterLength(int Char) const
{
  return sizeof(CharType);
}

bool Utf16StringTrait::IsValidCharacter(int Char) const
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

bool Utf16StringTrait::IsFinalCharacter(int Char) const
{
  return (Char & 0xffff) == 0x0;
}

std::string Utf16StringTrait::ConvertToUtf8(void* pStrBuf, size_t StrBufLen) const
{
  return boost::locale::conv::utf_to_utf
    <std::string::value_type, CharType>
    (reinterpret_cast<CharType*>(pStrBuf),
     reinterpret_cast<CharType*>(pStrBuf) + StrBufLen / sizeof(CharType));
}

u16 Utf16StringTrait::CharacterLength(int Char) const
{
  return sizeof(CharType);
}

StringTrait const* String::GetStringTrait(void) const
{
  static Utf8StringTrait  Utf8;
  static Utf16StringTrait Utf16;

  switch (GetSubType())
  {
  case Utf8Type:  return &Utf8;
  case Utf16Type: return &Utf16;
  default:        return nullptr;
  }
}

MEDUSA_NAMESPACE_END