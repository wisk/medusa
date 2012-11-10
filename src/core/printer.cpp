#include "medusa/printer.hpp"

MEDUSA_NAMESPACE_USE;

void Printer::operator()(View::LineInformation const& rLineInfo)
{
  switch (rLineInfo.GetType())
  {
  case View::LineInformation::CellLineType:       PrintCell(rLineInfo);       break;
  case View::LineInformation::MultiCellLineType:  PrintMultiCell(rLineInfo);  break;
  case View::LineInformation::LabelLineType:      PrintLabel(rLineInfo);      break;
  case View::LineInformation::XrefLineType:       PrintXref(rLineInfo);       break;
  case View::LineInformation::MemoryAreaLineType: PrintMemoryArea(rLineInfo); break;
  case View::LineInformation::EmptyLineType:      PrintEmpty(rLineInfo);      break;
  default:                                                                    break;
  }
}

void StreamPrinter::PrintCell(View::LineInformation const& rLineInfo)
{
  auto pCell = m_rCore.GetCell(rLineInfo.GetAddress());
  if (pCell == nullptr)
  {
    m_rStream << "invalid cell" << std::endl;
    return;
  }

  m_rStream << pCell->ToString();
  auto rComment = pCell->GetComment();
  if (!rComment.empty())
    m_rStream << " ; " << rComment;
  m_rStream << std::endl;
}

void StreamPrinter::PrintMultiCell(View::LineInformation const& rLineInfo)
{
  auto pMultiCell = m_rCore.GetMultiCell(rLineInfo.GetAddress());
  if (pMultiCell == nullptr)
  {
    m_rStream << "invalid multicell" << std::endl;
    return;
  }

  m_rStream << pMultiCell->ToString() << ":" << std::endl;
}

void StreamPrinter::PrintLabel(View::LineInformation const& rLineInfo)
{
  m_rStream << "label" << std::endl;
}

void StreamPrinter::PrintXref(View::LineInformation const& rLineInfo)
{
  m_rStream << "xref" << std::endl;
}

void StreamPrinter::PrintMemoryArea(View::LineInformation const& rLineInfo)
{
  m_rStream << "mem_area" << std::endl;
}

void medusa::StreamPrinter::PrintEmpty( View::LineInformation const& rLineInfo )
{
  m_rStream << std::endl;
}
