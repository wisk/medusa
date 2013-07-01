#include "DisassemblyPrinter.hpp"
#include "Settings.hpp"
#include <algorithm>

u32 DisassemblyPrinter::GetNumberOfLine(Address const& rAddress)
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

u32 DisassemblyPrinter::PrintAddress(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  _p->setPen(Qt::black);
  auto strAddr = QString::fromStdString(rAddress.ToString());
  drawText(xOffset, yOffset, strAddr);
  return strAddr.size() + 2;
}

u32 DisassemblyPrinter::PrintCell(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  auto const* curCell = m_rCore.GetCell(rAddress);
  if (curCell == nullptr)
    return 1;
  u16 offset = 0;

  QColor clr;
  QString lineStr = "** invalid line **";

  auto marks = curCell->GetMarks();
  if (marks.empty())
    return 1;

  std::for_each(std::begin(marks), std::end(marks), [&](Cell::Mark const& mark)
  {
    QColor cellClr(Qt::black);
    QString cellStr = QString::fromUtf8(curCell->ToString().substr(offset, mark.GetLength()).c_str());

    switch (mark.GetType())
    {
    case medusa::Cell::Mark::MnemonicType:  cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_MNEMONIC,  MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT).toString());  break;
    case medusa::Cell::Mark::KeywordType:   cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_KEYWORD,   MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT).toString());   break;
    case medusa::Cell::Mark::ImmediateType: cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString()); break;
    case medusa::Cell::Mark::OperatorType:  cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_OPERATOR,  MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT).toString());  break;
    case medusa::Cell::Mark::RegisterType:  cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_REGISTER,  MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT).toString());  break;
    case medusa::Cell::Mark::LabelType:     cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL,     MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());     break;
    case medusa::Cell::Mark::StringType:    cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_STRING,    MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT).toString());    break;
    default: break;
    };

    _p->setPen(cellClr);
    drawText(xOffset + offset, yOffset, cellStr);
    offset += mark.GetLength();
  });

  lineStr = "";
  if (curCell->GetComment().empty() == false)
  {
    _p->setPen(QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString()));
    drawText(xOffset + offset, yOffset, QString(" ; ") + QString::fromUtf8(curCell->GetComment().c_str()));
  }
  return 1;
}

u32 DisassemblyPrinter::PrintMultiCell(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  auto const* curMC = m_rCore.GetMultiCell(rAddress);
  if (curMC == nullptr)
    return 1;
  auto clr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  _p->setPen(clr);
  QString strLbl = QString::fromUtf8(curMC->ToString().c_str());
  drawText(xOffset, yOffset, strLbl);
  return 1;
}

u32 DisassemblyPrinter::PrintLabel(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  auto curLbl = m_rCore.GetDatabase().GetLabelFromAddress(rAddress);
  auto clr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL, MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());
  if (curLbl.GetType() == Label::LabelUnknown)
    return 1;
  _p->setPen(clr);
  QString strLbl = QString::fromStdString(curLbl.GetLabel()) + QString(":");
  drawText(xOffset, yOffset, strLbl);
  return 1;
}

u32 DisassemblyPrinter::PrintXref(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  medusa::Address::List RefAddrList;
  m_rCore.GetDatabase().GetXRefs().From(rAddress, RefAddrList);
  QString xrefs = QString::fromUtf8(";:xref");

  std::for_each(std::begin(RefAddrList), std::end(RefAddrList), [&](medusa::Address const& rRefAddr)
  {
    xrefs += QString(" ") + (rRefAddr < rAddress ? QString::fromUtf8("\xe2\x86\x91") : QString::fromUtf8("\xe2\x86\x93")) + QString::fromStdString(rRefAddr.ToString());
  });
  auto clr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  _p->setPen(clr);
  drawText(xOffset, yOffset, xrefs);
  return 1;
}

u32 DisassemblyPrinter::PrintMemoryArea(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  auto const* memArea = m_rCore.GetDatabase().GetMemoryArea(rAddress);
  if (memArea == nullptr)
    return 1;
  auto strMemArea = QString::fromStdString(memArea->ToString());
  auto clr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  _p->setPen(clr);
  drawText(xOffset, yOffset, strMemArea);
  return 1;
}

u32 DisassemblyPrinter::PrintEmpty(Address const& rAddress, u32 xOffset, u32 yOffset)
{
  return 1;
}

void DisassemblyPrinter::drawText(int x, int y, QString const & text)
{
  // LATER: could be optimize by caching font metrics
  QString fontInfo = Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString();
  QFont font;
  font.fromString(fontInfo);
  QFontMetrics fm(font);
  int wChr = fm.width('M');
  int hChr = fm.height();
  x *= wChr;
  y *= hChr;
  y += 10;
  foreach (QChar chr, text)
  {
    int wCurChr = fm.width(chr);
    int chrOffset = wChr / 2 - wCurChr / 2;
    if (chrOffset < 0)
      chrOffset = 0;
    _p->drawText(x + chrOffset, y, chr);
    x += wChr;
  }
}