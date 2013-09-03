#include "medusa/printer.hpp"
#include <boost/algorithm/string.hpp>
#include <algorithm>

MEDUSA_NAMESPACE_USE;

u32 Printer::operator()(Address const& rAddress, u32 xOffset, u32 yOffset, u32 Flags)
{
  auto& rDoc = m_rCore.GetDocument();
  u32 NumberOfRow = 0, NumberOfLine = 0;

  // MemoryArea
  auto pMemArea = rDoc.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetBaseAddress() == rAddress)
  {
    if (Flags & ShowAddress)
      NumberOfRow = PrintAddress(rAddress, xOffset, yOffset);
    NumberOfLine += PrintMemoryArea(rAddress, xOffset + NumberOfRow, yOffset);
  }

  // XRefs
  if (rDoc.GetXRefs().HasXRefFrom(rAddress))
  {
    if (Flags & AddSpaceBeforeXref)
    {
      if (Flags & ShowAddress)
        NumberOfRow = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
      NumberOfLine += PrintEmpty(rAddress, xOffset, yOffset + NumberOfLine);
    }
    if (Flags & ShowAddress)
      NumberOfRow = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintXref(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  // Label
  auto rLbl = rDoc.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::Unknown)
  {
    if (Flags & ShowAddress)
      NumberOfRow = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintLabel(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  // Multicell
  if (rDoc.GetMultiCell(rAddress) != nullptr)
  {
    if (Flags & ShowAddress)
      NumberOfRow = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintMultiCell(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  if (rDoc.GetCell(rAddress) != nullptr)
  {
    if (Flags & ShowAddress)
      NumberOfRow = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintCell(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  return NumberOfLine;
}

u16 Printer::GetLineHeight(Address const& rAddress, u32 Flags) const
{
  auto const& rDoc = m_rCore.GetDocument();
  u16 Height = 0;

  // MemoryArea
  auto pMemArea = rDoc.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetBaseAddress() == rAddress)
    Height++;

  // XRefs
  if (rDoc.GetXRefs().HasXRefFrom(rAddress))
    Height += (Flags & AddSpaceBeforeXref ? 2 : 1);

  // Label
  auto rLbl = rDoc.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::Unknown)
    Height++;

  // Multicell
  if (rDoc.GetMultiCell(rAddress) != nullptr)
    Height++;

  Height++;

  return Height;
}

u16 Printer::GetLineWidth(Address const& rAddress, u32 Flags) const
{
  auto& rDoc = m_rCore.GetDocument();
  size_t LineWidth = 0;

  // MemoryArea
  auto pMemArea = rDoc.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetBaseAddress() == rAddress)
    LineWidth = std::max(LineWidth, pMemArea->ToString().length());

  // XRefs
  if (rDoc.GetXRefs().HasXRefFrom(rAddress))
  {
    // LATER
  }

  // Label
  auto rLbl = rDoc.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::Unknown)
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

  return static_cast<u16>(LineWidth);
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
  auto Lbl = m_rCore.GetDocument().GetLabelFromAddress(rAddress);
  if (Lbl.GetType() == Label::Unknown)
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
  m_rCore.GetDocument().GetXRefs().From(rAddress, AddrFrom);
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
  auto pMemArea = m_rCore.GetDocument().GetMemoryArea(rAddress);
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