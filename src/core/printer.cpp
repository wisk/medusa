#include "medusa/printer.hpp"
#include <boost/algorithm/string.hpp>

MEDUSA_NAMESPACE_USE;

u16 Printer::operator()(Address const& rAddress, u16 xOffset, u16 yOffset)
{
  auto rDatabase = m_rCore.GetDatabase();
  u16 NumberOfLine = 0;

  // MemoryArea
  auto pMemArea = rDatabase.GetMemoryArea(rAddress);
  if (pMemArea->GetVirtualBase() == rAddress)
    NumberOfLine += PrintMemoryArea(rAddress, xOffset, yOffset);
  if (yOffset < NumberOfLine) yOffset = 0;
  else                        yOffset -= NumberOfLine;

  // XRefs
  if (rDatabase.GetXRefs().HasXRef(rAddress))
    NumberOfLine += PrintXref(rAddress, xOffset, yOffset);
  if (yOffset < NumberOfLine) yOffset = 0;
  else                        yOffset -= NumberOfLine;

  // Label
  auto rLbl = rDatabase.GetLabelFromAddress(rAddress);
  if (rLbl.GetType() != Label::LabelUnknown)
    NumberOfLine += PrintLabel(rAddress, xOffset, yOffset);
  if (yOffset < NumberOfLine) yOffset = 0;
  else                        yOffset -= NumberOfLine;

  // Multicell
  if (rDatabase.RetrieveMultiCell(rAddress) != nullptr)
    NumberOfLine += PrintMultiCell(rAddress, xOffset, yOffset);
  if (yOffset < NumberOfLine) yOffset = 0;
  else                        yOffset -= NumberOfLine;

  NumberOfLine += PrintCell(rAddress, xOffset, yOffset);

  return NumberOfLine;
}

u16 StreamPrinter::PrintCell(Address const& rAddress, u16 xOffset, u16 yOffset)
{
  if (xOffset == 0)
  {
    std::ostringstream Buffer;
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
  }
  return 1;
}

u16 StreamPrinter::PrintMultiCell(Address const& rAddress, u16 xOffset, u16 yOffset)
{
  if (xOffset == 0)
  {
    std::ostringstream Buffer;
    auto pMultiCell = m_rCore.GetMultiCell(rAddress);
    if (pMultiCell == nullptr)
    {
      Buffer << "invalid multicell" << std::endl;
      return 1;
    }

    Buffer << pMultiCell->ToString() << ":";
    Buffer.str().erase(0, yOffset);
    m_rStream << Buffer.str() << std::endl;
  }
  return 1;
}

u16 StreamPrinter::PrintLabel(Address const& rAddress, u16 xOffset, u16 yOffset)
{
  if (xOffset == 0)
  {
    std::ostringstream Buffer;
    auto Lbl = m_rCore.GetDatabase().GetLabelFromAddress(rAddress);
    if (Lbl.GetType() == Label::LabelUnknown)
      Buffer << "unknown label:";
    else
      Buffer << Lbl.GetLabel() << ":";

    Buffer.str().erase(0, yOffset);
    m_rStream << Buffer.str() << std::endl;
  }
  return 1;
}

u16 StreamPrinter::PrintXref(Address const& rAddress, u16 xOffset, u16 yOffset)
{
  if (xOffset == 0)
  {
    std::ostringstream Buffer;
    Address::List AddrFrom;
    std::list<std::string> AddrFromStr;
    m_rCore.GetDatabase().GetXRefs().From(rAddress, AddrFrom);
    std::for_each(std::begin(AddrFrom), std::end(AddrFrom), [&AddrFromStr](Address const& rAddr)
    { AddrFromStr.push_back(rAddr.ToString()); });
    Buffer << "xref: " << boost::algorithm::join(AddrFromStr, ", ");
    Buffer.str().erase(0, yOffset);
    m_rStream << Buffer.str() << std::endl;
  }
  return 1;
}

u16 StreamPrinter::PrintMemoryArea(Address const& rAddress, u16 xOffset, u16 yOffset)
{
  if (xOffset == 0)
  {
    std::ostringstream Buffer;
    auto pMemArea = m_rCore.GetDatabase().GetMemoryArea(rAddress);
    if (pMemArea == nullptr)
      Buffer << "mem_area";
    else
      Buffer << pMemArea->ToString();
    Buffer.str().erase(0, yOffset);
    m_rStream << Buffer.str() << std::endl;
  }
  return 1;
}

u16 medusa::StreamPrinter::PrintEmpty(Address const& rAddress, u16 xOffset, u16 yOffset)
{
  if (xOffset == 0)
  {
    m_rStream << std::endl;
  }
  return 1;
}
