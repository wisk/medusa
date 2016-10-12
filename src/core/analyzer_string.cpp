#include "medusa/analyzer.hpp"

namespace medusa
{
  bool AnalyzerString::CreateUtf8String(void)
  {
    static Utf8StringTrait Utf8Str;
    s8 CurChar;
    OffsetType Offset;
    u16 RawLen = 0;
    std::string StrData;
    auto const& rBinStrm = m_rDoc.GetBinaryStream();

    if (m_rDoc.ConvertAddressToFileOffset(m_Addr.GetOffset(), Offset) == false)
      return false;

    for (;;)
    {
      if (!rBinStrm.Read(Offset + RawLen, CurChar))
      {
        Log::Write("core") << "Unable to read utf-8 string at " << m_Addr << LogEnd;
        return false;
      }

      if (CurChar == '\0')
        break;

      StrData += CurChar;
      ++RawLen;
    }

    if (RawLen == 0)
      return false;

    ++RawLen;

    auto spString = std::make_shared<String>(String::Utf8Type, RawLen);
    m_rDoc.SetCellWithLabel(m_Addr, spString, Label(StrData, Label::String | Label::Global), true);

    return true;
  }

  bool AnalyzerString::CreateUtf16String(void)
  {
    static Utf16StringTrait Utf16Str;
    Utf16StringTrait::CharType Utf16Char;
    OffsetType StrOff;
    u16 RawLen = 0;
    auto const& rBinStrm = m_rDoc.GetBinaryStream();

    if (m_rDoc.ConvertAddressToFileOffset(m_Addr, StrOff) == false)
      return false;

    for (;;)
    {
      if (!rBinStrm.Read(StrOff + RawLen, Utf16Char))
      {
        Log::Write("core") << "Unable to read utf-16 string at " << m_Addr << LogEnd;
        return false;
      }

      if (Utf16Str.IsFinalCharacter(Utf16Char))
        break;

      RawLen += sizeof(Utf16Char);
    }

    if (RawLen == 0x0)
      return false;

    RawLen += sizeof(Utf16Char); // include L'\0'

    auto upStrBuf = std::unique_ptr<u8[]>(new u8[RawLen]);
    if (!rBinStrm.Read(StrOff, upStrBuf.get(), RawLen))
    {
      Log::Write("core") << "Unable to read utf-16 string at " << m_Addr << LogEnd;
      return false;
    }
    std::string CvtStr = Utf16Str.ConvertToUtf8(upStrBuf.get(), RawLen);
    if (CvtStr.empty())
    {
      Log::Write("core") << "Unable to convert utf-16 string at " << m_Addr << LogEnd;
      return false;
    }

    auto spString = std::make_shared<String>(String::Utf16Type, RawLen);
    m_rDoc.SetCellWithLabel(m_Addr, spString, Label(CvtStr, Label::String | Label::Global), true);

    return true;
  }
}