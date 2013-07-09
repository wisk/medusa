#include "medusa/printer.hpp"
#include <boost/algorithm/string.hpp>
#include <algorithm>

MEDUSA_NAMESPACE_USE;

u32 Printer::operator()(Address const& rAddress, u32 xOffset, u32 yOffset, u32 Flags)
{
  auto& rDatabase = m_rCore.GetDatabase();
  u32 NumberOfRow = 0, NumberOfLine = 0;

  // MemoryArea
  auto pMemArea = rDatabase.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetVirtualBase() == rAddress)
  {
    if (Flags & ShowAddress)
      NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset);
    NumberOfLine += PrintMemoryArea(rAddress, xOffset + NumberOfRow, yOffset);
  }

  // XRefs
  if (rDatabase.GetXRefs().HasXRefFrom(rAddress))
  {
    if (Flags & AddSpaceBeforeXref)
    {
      if (Flags & ShowAddress)
        NumberOfRow = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
      NumberOfLine += PrintEmpty(rAddress, xOffset, yOffset + NumberOfLine);
    }
    if (Flags & ShowAddress)
      NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintXref(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  // Label
  auto rLbl = rDatabase.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::LabelUnknown)
  {
    if (Flags & ShowAddress)
      NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintLabel(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  // Multicell
  if (rDatabase.RetrieveMultiCell(rAddress) != nullptr)
  {
    if (Flags & ShowAddress)
      NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintMultiCell(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  if (rDatabase.RetrieveCell(rAddress) != nullptr)
  {
    if (Flags & ShowAddress)
      NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintCell(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  return NumberOfLine;
}

u16 Printer::GetLineHeight(Address const& rAddress, u32 Flags) const
{
  auto const& rDatabase = m_rCore.GetDatabase();
  u16 Height = 0;

  // MemoryArea
  auto pMemArea = rDatabase.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetVirtualBase() == rAddress)
    Height++;

  // XRefs
  if (rDatabase.GetXRefs().HasXRefFrom(rAddress))
    Height += (Flags & AddSpaceBeforeXref ? 2 : 1);

  // Label
  auto rLbl = rDatabase.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::LabelUnknown)
    Height++;

  // Multicell
  if (rDatabase.RetrieveMultiCell(rAddress) != nullptr)
    Height++;

  Height++;

  return Height;
}

u16 Printer::GetLineWidth(Address const& rAddress, u32 Flags) const
{
  auto& rDatabase = m_rCore.GetDatabase();
  size_t LineWidth = 0;

  // MemoryArea
  auto pMemArea = rDatabase.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetVirtualBase() == rAddress)
    LineWidth = std::max(LineWidth, pMemArea->ToString().length());

  // XRefs
  if (rDatabase.GetXRefs().HasXRefFrom(rAddress))
  {
    // LATER
  }

  // Label
  auto rLbl = rDatabase.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::LabelUnknown)
    LineWidth = std::max(LineWidth, rLbl.GetLabel().length());

  // Multicell
  auto pMultiCell = m_rCore.GetMultiCell(rAddress);
  if (pMultiCell != nullptr)
  {
    std::string StrMultiCell;
    Cell::Mark::List MarksMultiCell;
    m_rCore.FormatMultiCell(rAddress, *pMultiCell, StrMultiCell, MarksMultiCell);
    LineWidth = std::max(LineWidth, StrMultiCell.length());
  }

  // Cell
  size_t CellLen = 0;
  auto pCell = m_rCore.GetCell(rAddress);
  if (pCell == nullptr)
    return 0;

  std::string StrCell;
  Cell::Mark::List CellMarks;
  m_rCore.FormatCell(rAddress, *pCell, StrCell, CellMarks);
  CellLen += StrCell.length();
  auto const& rCellCmt = pCell->GetComment();
  if (rCellCmt.empty() == false)
    CellLen += (rCellCmt.length() + 3);

  LineWidth = std::max(LineWidth, CellLen);

  if (Flags & ShowAddress)
    LineWidth += (rAddress.ToString().length() + 2);

  return LineWidth;
}

u32 StreamPrinter::PrintAddress(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  std::ostringstream Buffer;
  Buffer << rAddress.ToString() << ": ";
  Buffer.str().erase(0, yOffset);
  m_rStream << Buffer.str() << std::endl;
  return static_cast<u32>(Buffer.str().length());
}

u32 StreamPrinter::PrintCell(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  std::ostringstream Buffer;
  Buffer << rAddress.ToString() << " ";
  auto pCell = m_rCore.GetCell(rAddress);
  if (pCell == nullptr)
  {
    Buffer << "invalid cell" << std::endl;
    return 1;
  }

  std::string CellStr;
  Cell::Mark::List Marks;
  m_rCore.FormatCell(rAddress, *pCell, CellStr, Marks);
  Buffer << CellStr;
  auto rComment = pCell->GetComment();
  if (!rComment.empty())
    Buffer << " ; " << rComment;

  Buffer.str().erase(0, yOffset);
  m_rStream << Buffer.str() << std::endl;
  return 1;
}

u32 StreamPrinter::PrintMultiCell(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  std::ostringstream Buffer;
  Buffer << rAddress.ToString() << " ";
  auto pMultiCell = m_rCore.GetMultiCell(rAddress);
  if (pMultiCell == nullptr)
  {
    Buffer << "invalid multicell" << std::endl;
    return 1;
  }

  std::string StrMultiCell;
  Cell::Mark::List MarksMultiCell;
  m_rCore.FormatMultiCell(rAddress, *pMultiCell, StrMultiCell, MarksMultiCell);
  Buffer << StrMultiCell << ":";
  Buffer.str().erase(0, yOffset);
  m_rStream << Buffer.str() << std::endl;
  return 1;
}

u32 StreamPrinter::PrintLabel(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  std::ostringstream Buffer;
  Buffer << rAddress.ToString() << " ";
  auto Lbl = m_rCore.GetDatabase().GetLabelFromAddress(rAddress);
  if (Lbl.GetType() == Label::LabelUnknown)
    Buffer << "unknown label:";
  else
    Buffer << Lbl.GetLabel() << ":";

  Buffer.str().erase(0, yOffset);
  m_rStream << Buffer.str() << std::endl;
  return 1;
}

u32 StreamPrinter::PrintXref(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  std::ostringstream Buffer;
  Buffer << rAddress.ToString() << " ";
  Address::List AddrFrom;
  std::list<std::string> AddrFromStr;
  m_rCore.GetDatabase().GetXRefs().From(rAddress, AddrFrom);
  std::for_each(std::begin(AddrFrom), std::end(AddrFrom), [&AddrFromStr](Address const& rAddr)
  { AddrFromStr.push_back(rAddr.ToString()); });
  Buffer << "xref: " << boost::algorithm::join(AddrFromStr, ", ");
  Buffer.str().erase(0, yOffset);
  m_rStream << Buffer.str() << std::endl;
  return 1;
}

u32 StreamPrinter::PrintMemoryArea(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  std::ostringstream Buffer;
  Buffer << rAddress.ToString() << " ";
  auto pMemArea = m_rCore.GetDatabase().GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    Buffer << "mem_area";
  else
    Buffer << pMemArea->ToString();
  Buffer.str().erase(0, yOffset);
  m_rStream << Buffer.str() << std::endl;
  return 1;
}

u32 StreamPrinter::PrintEmpty(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  m_rStream << rAddress.ToString() << std::endl;
  return 1;
}