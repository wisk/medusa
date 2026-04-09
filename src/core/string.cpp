#include "medusa/string.hpp"

#include <string>

MEDUSA_NAMESPACE_BEGIN

bool Utf8StringTrait::IsValidCharacter(int Char) const
{
  switch (Char)
  {
  case '\a': case '\b': case '\t': case '\n':
  case '\v': case '\f': case '\r':
    return true;
  default:
    return !!isprint(Char & 0xff);
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
  if ((Char & 0xff00) != 0x0)
    return false;

  switch (Char & 0xff)
  {
  case '\a': case '\b': case '\t': case '\n':
  case '\v': case '\f': case '\r':
    return true;
  default:
    return !!isprint(Char & 0xff);
  }
}

bool Utf16StringTrait::IsFinalCharacter(int Char) const
{
  return (Char & 0xffff) == 0x0;
}

std::string Utf16StringTrait::ConvertToUtf8(void* pStrBuf, size_t StrBufLen) const
{
  auto pSrc = reinterpret_cast<CharType const*>(pStrBuf);
  size_t SrcLen = StrBufLen / sizeof(CharType);
  std::string Result;
  Result.reserve(SrcLen);

  for (size_t i = 0; i < SrcLen; ++i)
  {
    uint32_t cp = static_cast<uint16_t>(pSrc[i]);

    // Handle surrogate pairs
    if (cp >= 0xD800 && cp <= 0xDBFF && i + 1 < SrcLen)
    {
      uint32_t lo = static_cast<uint16_t>(pSrc[++i]);
      if (lo >= 0xDC00 && lo <= 0xDFFF)
        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
    }

    if (cp < 0x80)
      Result += static_cast<char>(cp);
    else if (cp < 0x800)
    {
      Result += static_cast<char>(0xC0 | (cp >> 6));
      Result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else if (cp < 0x10000)
    {
      Result += static_cast<char>(0xE0 | (cp >> 12));
      Result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      Result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else
    {
      Result += static_cast<char>(0xF0 | (cp >> 18));
      Result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
      Result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      Result += static_cast<char>(0x80 | (cp & 0x3F));
    }
  }
  return Result;
}

u16 Utf16StringTrait::CharacterLength(int Char) const
{
  return sizeof(CharType);
}

StringTrait const* String::GetStringTrait() const
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
