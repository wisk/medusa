#include "DisassemblyPrinter.hpp"
#include "Settings.hpp"
#include <algorithm>

medusa::u32 DisassemblyPrinter::PrintAddress(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset)
{
  _p->setPen(Qt::black);
  auto strAddr = QString::fromStdString(rAddress.ToString());
  drawText(xOffset, yOffset, strAddr);
  return strAddr.size() + 2;
}

medusa::u32 DisassemblyPrinter::PrintCell(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset)
{
  auto curCell = m_rCore.GetCell(rAddress);
  if (curCell == nullptr)
    return 1;
  medusa::u16 offset = 0;

  QColor clr;
  QString lineStr = "** invalid line **";

  std::string str;
  medusa::Cell::Mark::List marks;
  if (m_rCore.FormatCell(rAddress, *curCell, str, marks) == false)
    return 0;

  if (marks.empty())
    return 0;

  std::for_each(std::begin(marks), std::end(marks), [&](medusa::Cell::Mark const& mark)
  {
    QColor cellClr(Qt::black);
    QString cellStr = QString::fromUtf8(str.substr(offset, mark.GetLength()).c_str());

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
    drawText(xOffset - offset, yOffset, cellStr);
    offset += mark.GetLength();
  });

  lineStr = "";
  std::string cmt;
  if (m_rCore.GetDocument().GetComment(rAddress, cmt))
  {
    _p->setPen(QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString()));
    drawText(xOffset - offset, yOffset, QString(" ; ") + QString::fromStdString(cmt));
  }
  return 1;
}

medusa::u32 DisassemblyPrinter::PrintMultiCell(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset)
{
  auto const* curMC = m_rCore.GetMultiCell(rAddress);
  if (curMC == nullptr)
    return 1;
  auto clr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  _p->setPen(clr);
  std::string str;
  medusa::Cell::Mark::List marks;
  m_rCore.FormatMultiCell(rAddress, *curMC, str, marks);
  QString strLbl = QString::fromUtf8(str.c_str());
  drawText(xOffset, yOffset, strLbl);
  return 1;
}

medusa::u32 DisassemblyPrinter::PrintLabel(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset)
{
  auto curLbl = m_rCore.GetDocument().GetLabelFromAddress(rAddress);
  auto clr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL, MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());
  if (curLbl.GetType() == medusa::Label::Unknown)
    return 1;
  _p->setPen(clr);
  QString strLbl = QString::fromStdString(curLbl.GetLabel()) + QString(":");
  drawText(xOffset, yOffset, strLbl);
  return 1;
}

medusa::u32 DisassemblyPrinter::PrintXref(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset)
{
  medusa::Address::List RefAddrList;
  m_rCore.GetDocument().GetCrossReferenceFrom(rAddress, RefAddrList);
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

medusa::u32 DisassemblyPrinter::PrintMemoryArea(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset)
{
  auto const* memArea = m_rCore.GetDocument().GetMemoryArea(rAddress);
  if (memArea == nullptr)
    return 1;
  auto strMemArea = QString::fromStdString(memArea->ToString());
  auto clr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());
  _p->setPen(clr);
  drawText(xOffset, yOffset, strMemArea);
  return 1;
}

medusa::u32 DisassemblyPrinter::PrintEmpty(medusa::Address const& rAddress, medusa::u32 xOffset, medusa::u32 yOffset)
{
  return 1;
}

void DisassemblyPrinter::drawText(int x, int y, QString const & text)
{
  QString fontInfo = Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString();
  QFont font;
  font.fromString(fontInfo);
  QFontMetrics fm(font);

  int wChr = fm.width('M');
  int hChr = fm.height();
  x *= wChr;
  y *= hChr;
  y += 10; // dunno why I have to do this...

  _p->setFont(font);
  _p->drawText(-x, y, text);
}
