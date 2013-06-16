#include "medusa/printer.hpp"
#include <boost/algorithm/string.hpp>
#include <algorithm>

MEDUSA_NAMESPACE_USE;

u32 Printer::operator()(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  auto& rDatabase = m_rCore.GetDatabase();
  u32 NumberOfRow = 0, NumberOfLine = 0;

  // MemoryArea
  auto pMemArea = rDatabase.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetVirtualBase() == rAddress)
  {
    NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset);
    NumberOfLine += PrintMemoryArea(rAddress, xOffset + NumberOfRow, yOffset);
  }

  // XRefs
  if (rDatabase.GetXRefs().HasXRefFrom(rAddress))
  {
    NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintXref(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  // Label
  auto rLbl = rDatabase.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::LabelUnknown)
  {
    NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintLabel(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  // Multicell
  if (rDatabase.RetrieveMultiCell(rAddress) != nullptr)
  {
    NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintMultiCell(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  if (rDatabase.RetrieveCell(rAddress) != nullptr)
  {
    NumberOfRow   = PrintAddress(rAddress, xOffset, yOffset + NumberOfLine);
    NumberOfLine += PrintCell(rAddress, xOffset + NumberOfRow, yOffset + NumberOfLine);
  }

  return NumberOfLine;
}

u32 Printer::GetNumberOfLine(Address const& rAddress)
{
  auto& rDatabase = m_rCore.GetDatabase();
  u32 NumberOfLine = 0;

  // MemoryArea
  auto pMemArea = rDatabase.GetMemoryArea(rAddress);
  if (pMemArea != nullptr && pMemArea->GetVirtualBase() == rAddress)
    NumberOfLine++;

  // XRefs
  if (rDatabase.GetXRefs().HasXRefFrom(rAddress))
    NumberOfLine++;

  // Label
  auto rLbl = rDatabase.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::LabelUnknown)
    NumberOfLine++;

  // Multicell
  if (rDatabase.RetrieveMultiCell(rAddress) != nullptr)
    NumberOfLine++;

  NumberOfLine++;

  return NumberOfLine;
}

u32 Printer::GetLineWidth(Address const& rAddress)
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
    LineWidth = std::max(LineWidth, pMultiCell->ToString().length());

  // Cell
  auto pCell = m_rCore.GetCell(rAddress);
  if (pCell != nullptr)
    LineWidth = std::max(LineWidth, pCell->ToString().length());

  return static_cast<u32>(LineWidth) + rAddress.ToString().length() + 2;
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

  Buffer << pCell->ToString();
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

  Buffer << pMultiCell->ToString() << ":";
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