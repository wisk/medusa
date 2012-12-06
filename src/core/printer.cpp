#include "medusa/printer.hpp"

MEDUSA_NAMESPACE_USE;

void Printer::operator()(View::LineInformation const& rLineInfo, u16 Offset)
{
  switch (rLineInfo.GetType())
  {
  case View::LineInformation::CellLineType:       PrintCell(rLineInfo,       Offset); break;
  case View::LineInformation::MultiCellLineType:  PrintMultiCell(rLineInfo,  Offset); break;
  case View::LineInformation::LabelLineType:      PrintLabel(rLineInfo,      Offset); break;
  case View::LineInformation::XrefLineType:       PrintXref(rLineInfo,       Offset); break;
  case View::LineInformation::MemoryAreaLineType: PrintMemoryArea(rLineInfo, Offset); break;
  case View::LineInformation::EmptyLineType:      PrintEmpty(rLineInfo,      Offset); break;
  default:                                                                            break;
  }
}

void StreamPrinter::PrintCell(View::LineInformation const& rLineInfo, u16 Offset)
{
  std::ostringstream Buffer;
  auto pCell = m_rCore.GetCell(rLineInfo.GetAddress());
  if (pCell == nullptr)
  {
    Buffer << "invalid cell" << std::endl;
    return;
  }

  Buffer << pCell->ToString();
  auto rComment = pCell->GetComment();
  if (!rComment.empty())
    Buffer << " ; " << rComment;

  Buffer.str().erase(0, Offset);
  m_rStream << Buffer.str() << std::endl;
}

void StreamPrinter::PrintMultiCell(View::LineInformation const& rLineInfo, u16 Offset)
{
  std::ostringstream Buffer;
  auto pMultiCell = m_rCore.GetMultiCell(rLineInfo.GetAddress());
  if (pMultiCell == nullptr)
  {
    Buffer << "invalid multicell" << std::endl;
    return;
  }

  Buffer << pMultiCell->ToString() << ":";
  Buffer.str().erase(0, Offset);
  m_rStream << Buffer.str() << std::endl;
}

void StreamPrinter::PrintLabel(View::LineInformation const& rLineInfo, u16 Offset)
{
  std::ostringstream Buffer;
  auto Lbl = m_rCore.GetDatabase().GetLabelFromAddress(rLineInfo.GetAddress());
  if (Lbl.GetType() == Label::LabelUnknown)
    Buffer << "unknown label:";
  else
    Buffer << Lbl.GetLabel() << ":";

  Buffer.str().erase(0, Offset);
  m_rStream << Buffer.str() << std::endl;
}

void StreamPrinter::PrintXref(View::LineInformation const& rLineInfo, u16 Offset)
{
  std::ostringstream Buffer;
  Buffer << "xref";
  Buffer.str().erase(0, Offset);
  m_rStream << Buffer.str() << std::endl;
}

void StreamPrinter::PrintMemoryArea(View::LineInformation const& rLineInfo, u16 Offset)
{
  std::ostringstream Buffer;
  Buffer << "mem_area";
  Buffer.str().erase(0, Offset);
  m_rStream << Buffer.str() << std::endl;
}

void medusa::StreamPrinter::PrintEmpty(View::LineInformation const& rLineInfo, u16 Offset)
{
  m_rStream << std::endl;
}
