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
  , m_Indent(2)
  , m_CurrentCommenOffsetType()
{
}

PrintData::PrintData(PrintData const& rPD)
  : m_PrependAddress(rPD.m_PrependAddress)
  , m_CurrentAddress(rPD.m_CurrentAddress)
  , m_CurrentText(rPD.m_CurrentText)
  , m_CurrentMarks(rPD.m_CurrentMarks)
  , m_CurrentOperandsOffset(rPD.m_CurrentOperandsOffset)
  , m_CurrentCommenOffsetType(rPD.m_CurrentCommenOffsetType)
  , m_Lines(rPD.m_Lines)
  , m_Width(rPD.m_Width)
  , m_LineWidth(rPD.m_LineWidth)
  , m_Height(rPD.m_Height)
  , m_Indent(rPD.m_Indent)
{

}

PrintData& PrintData::operator()(Address const& rAddress)
{
  std::lock_guard<MutexType> Lock(m_Mutex);
  m_CurrentAddress = rAddress;
  return *this;
}

PrintData& PrintData::operator=(PrintData const& rPD)
{
  m_PrependAddress = rPD.m_PrependAddress;

  m_CurrentAddress        = rPD.m_CurrentAddress;
  m_CurrentText           = rPD.m_CurrentText;
  m_CurrentMarks          = rPD.m_CurrentMarks;
  m_CurrentOperandsOffset = rPD.m_CurrentOperandsOffset;
  m_CurrentCommenOffsetType  = rPD.m_CurrentCommenOffsetType;

  m_Lines     = rPD.m_Lines;
  m_Width     = rPD.m_Width;
  m_LineWidth = rPD.m_LineWidth;
  m_Height    = rPD.m_Height;
  m_Indent    = rPD.m_Indent;

  return *this;
}

PrintData& PrintData::AppendImmediate(ap_int const& rImmediate, u32 Bits, u8 Base)
{
  std::ostringstream Buf;

  switch (Base)
  {
  case  2:
    {
      Buf << "0b";
      while (Bits--)
        Buf << ((rImmediate & (1 << Bits)) ? "1" : "0");
      return AppendImmediate(Buf.str());
    }
  case  8: Buf << std::oct << "0";  break;
  case 10: Buf << std::dec;         break;
  default: Buf << std::hex << "0x"; break;
  }

  Buf << std::setfill('0') << std::setw(Bits / 4) << rImmediate;
  return AppendImmediate(Buf.str());
}

PrintData& PrintData::AppendImmediate(BitVector const& rVal, u8 Base)
{
  return AppendImmediate(rVal.ToString(Base));
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
    AppendAddress(m_CurrentAddress).AppendSpace(m_Indent);

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

Address::Vector PrintData::GetAddresses(void) const
{
  Address::Vector Addrs;

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
    AppendAddress(m_CurrentAddress).AppendSpace(m_Indent);

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

bool GraphData::AddVertex(PrintData const& rPD, u16 X, u16 Y)
{
  std::lock_guard<MutexType> Lock(m_Mutex);

  Address VtxAddr;
  if (!rPD.GetFirstAddress(VtxAddr))
    return false;
  if (m_Map.find(VtxAddr) != std::end(m_Map))
    return false;
  m_Map[VtxAddr] = std::make_tuple(rPD, X, Y);
  return true;
}

bool GraphData::GetVertexPosition(Address const& rVtxAddr, u16& rX, u16& rY) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);

  auto itVtxData = m_Map.find(rVtxAddr);
  if (itVtxData == std::end(m_Map))
    return false;
  rX = std::get<1>(itVtxData->second);
  rY = std::get<2>(itVtxData->second);
  return true;
}

bool GraphData::SetVertexPosition(Address const& rVtxAddr, u16 X, u16 Y)
{
  std::lock_guard<MutexType> Lock(m_Mutex);

  auto itVtxData = m_Map.find(rVtxAddr);
  if (itVtxData == std::end(m_Map))
    return false;
  std::get<1>(itVtxData->second) = X;
  std::get<2>(itVtxData->second) = Y;
  return true;
}

void GraphData::ForEachVertex(GraphData::VertexCallbackType CB) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);

  for (auto const& rVtxPair : m_Map)
  {
    CB(std::get<0>(rVtxPair.second), std::get<1>(rVtxPair.second), std::get<2>(rVtxPair.second));
  }
}

bool GraphData::GetBends(Graph::EdgeDescriptor const& rEdgeDesc, BendsType& rBends) const
{
  auto itBends = m_Bends.find(rEdgeDesc);
  if (itBends == std::end(m_Bends))
    return false;
  rBends = itBends->second;
  return true;
}

void GraphData::SetBends(Graph::EdgeDescriptor const& rEdgeDesc, BendsType const& rBends)
{
  m_Bends[rEdgeDesc] = rBends;
}