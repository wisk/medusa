#include "medusa/cell_text.hpp"

MEDUSA_NAMESPACE_USE;

bool LineData::GetOperandNo(u16 Offset, u8& rOperandNo) const
{
  if (m_OperandsOffset.empty())
    return false;

  if ((Offset + 1) >= m_Text.length()) // NOTE: we have to add 1 since m_Text includes '\n' character
    return false;

  rOperandNo = 0;
  for (u16 OprdOff : m_OperandsOffset)
  {
    if (Offset < OprdOff)
    {
      if (rOperandNo == 0)
        return false;
      --rOperandNo;
      return true;
    }
    ++rOperandNo;
  }

  --rOperandNo;
  return true;
}

PrintData::PrintData(void)
  : m_Width(), m_LineWidth(), m_Height()
  , m_PrependAddress(true)
  , m_CurrentCommentOffset()
{
}

PrintData& PrintData::operator()(Address const& rAddress)
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  m_CurrentAddress = rAddress;
  return *this;
}

PrintData& PrintData::AppendImmediate(u64 Immediate, u32 Bits, u8 Base)
{
  std::ostringstream Buf;

  switch (Base)
  {
    // TODO: binary
  case  8: Buf << std::oct << "0o"; break;
  case 10: Buf << std::dec << "0d"; break;
  default: Buf << std::hex << "0x"; break;
  }

  Buf << std::setfill('0') << std::setw(Bits / 4) << Immediate;
  return AppendImmediate(Buf.str());
}

PrintData& PrintData::AppendAddress(Address const& rAddress)
{
  m_PrependAddress = false;
  _AppendText(rAddress.ToString(), Mark::ImmediateType);
  m_PrependAddress = true;
  return *this;
}

PrintData& PrintData::AppendSpace(u16 SpaceNo)
{
  std::string Spaces(static_cast<std::string::size_type>(SpaceNo), ' ');
  _AppendText(Spaces, Mark::UnprintableType);
  return *this;
}

PrintData& PrintData::AppendNewLine(void)
{
  if (m_PrependAddress && m_CurrentText.empty())
    AppendAddress(m_CurrentAddress).AppendSpace(8);

  std::lock_guard<MutexType> Lock(m_Mutex);
  ++m_Height;
  if (m_LineWidth > m_Width)
    m_Width = m_LineWidth;
  m_LineWidth = 0;

  m_CurrentText += "\n";
  m_CurrentMarks.push_back(Mark(Mark::UnprintableType, 1));
  m_Lines.push_back(LineData(m_CurrentAddress, m_CurrentText, m_CurrentMarks, m_CurrentOperandsOffset));

  m_CurrentText = "";
  m_CurrentMarks.clear();
  m_CurrentOperandsOffset.clear();
  return *this;
}

PrintData& PrintData::MarkOffset(void)
{
  m_CurrentOperandsOffset.insert(m_CurrentText.length());
  return *this;
}

Address::List PrintData::GetAddresses(void) const
{
  Address::List Addrs;

  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& Line : m_Lines)
    Addrs.push_back(Line.GetAddress());
  Addrs.push_back(m_CurrentAddress);
  return Addrs;
}

bool PrintData::GetFirstAddress(Address& rAddress) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  if (m_Lines.empty())
    return false;
  rAddress = m_Lines.front().GetAddress();
  return true;
}

bool PrintData::GetLastAddress(Address& rAddress) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  if (m_Lines.empty())
    return false;
  rAddress = m_Lines.back().GetAddress();
  return true;
}

std::string PrintData::GetTexts(void) const
{
  std::string Texts;

  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& rLine : m_Lines)
    Texts += rLine.GetText();
  Texts += m_CurrentText;
  return Texts;
}

std::vector<std::string> PrintData::GetTextLines(void) const
{
  std::vector<std::string> Lines;
  Lines.reserve(m_Lines.size());

  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& rLine : m_Lines)
    Lines.push_back(rLine.GetText());
  return Lines;
}

Mark::List PrintData::GetMarks(void) const
{
  Mark::List Marks;

  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& rLine : m_Lines)
    for (auto const& rMark : rLine.GetMarks())
      Marks.push_back(rMark);
  for (auto const& rMark : m_CurrentMarks)
    Marks.push_back(rMark);
  return Marks;
}

bool PrintData::GetLine(u16 LineNo, u16& rOffset, LineData& rLine) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);

  if (m_Lines.empty())
    return false;

  if (LineNo > m_Lines.size())
    return false;

  u16 LineOff = 0;
  Address RefAddr = Address();
  for (auto const& rCurLine : m_Lines)
  {
    if (!(RefAddr == rCurLine.GetAddress()))
    {
      RefAddr = rCurLine.GetAddress();
      LineOff = 0;
    }

    if (LineNo-- == 0)
    {
      rOffset = LineOff;
      rLine = rCurLine;
      return true;
    }

    if (RefAddr == rCurLine.GetAddress())
      ++LineOff;
  }

  return false;
}

bool PrintData::GetLine(Address const& rAddress, u16 Offset, LineData& rLine) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& rCurLine : m_Lines)
  {
    if (rCurLine.GetAddress() == rAddress)
    {
      if (Offset == 0x0)
      {
        rLine = rCurLine;
        return true;
      }
      else
        --Offset;
    }
  }
  return false;
}

u16 PrintData::GetLineNo(Address const& rAddress) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  u16 LineNo = 0x0;
  for (auto const& rCurLine : m_Lines)
    if (rCurLine.GetAddress() == rAddress)
      ++LineNo;
  return LineNo;
}

bool PrintData::GetLineOffset(Address const& rAddress, u16& rOffset) const
{
  rOffset = 0;
  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& rCurLine : m_Lines)
  {
    if (rCurLine.GetAddress() == rAddress)
      return true;
    else
      ++rOffset;
  }
  return false;
}

bool PrintData::GetOperandNo(Address const& rAddress, u16 xOffset, u16 yOffset, u8& rOperandNo) const
{
  LineData Line;
  if (!GetLine(rAddress, yOffset, Line))
    return false;
  return Line.GetOperandNo(xOffset, rOperandNo);
}

bool PrintData::Contains(Address const& rAddress) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& rCurLine : m_Lines)
    if (rCurLine.GetAddress() == rAddress)
      return true;
  return false;
}

void PrintData::ForEachLine(LineCallback Callback) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  for (auto const& rLine : m_Lines)
    Callback(rLine.GetAddress(), rLine.GetText(), rLine.GetMarks());
}

void PrintData::_AppendText(std::string const& rText, Mark::Type MarkType)
{
  if (m_PrependAddress && m_CurrentText.empty())
    AppendAddress(m_CurrentAddress).AppendSpace(8);

  u16 TextLen = static_cast<u16>(rText.length());
  {
    std::lock_guard<MutexType> Lock(m_Mutex);
    m_CurrentText.append(rText);
    m_CurrentMarks.push_back(Mark(MarkType, TextLen));
    m_LineWidth += TextLen;
  }

  if (MarkType == Mark::MnemonicType && TextLen < 16)
    AppendSpace(16 - TextLen);
}

void PrintData::Clear(void)
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  m_CurrentAddress = Address();
  m_CurrentText.clear();
  m_CurrentMarks.clear();
  m_CurrentOperandsOffset.clear();
  m_Lines.clear();
  m_Width     = 0;
  m_LineWidth = 0;
  m_Height    = 0;
}