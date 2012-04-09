#include "DisassemblyView.hpp"

DisassemblyView::DisassemblyView(QWidget * parent)
  : QAbstractScrollArea(parent)
  , _db(nullptr)
  , _yOffset(10)
  , _xData(0),          _yData(0)
  , _wChar(0),          _hChar(0)
  , _xCursor(0),        _yCursor(0)
  , _firstSelection(0), _lastSelection(0)
  , _lineNo(0x10000),   _lineLen(0x100)
{
  setFont(QFont("consolas", 10));

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

DisassemblyView::~DisassemblyView(void)
{
}

void DisassemblyView::setDatabase(medusa::Database const * db)
{
  _db = db;
}

bool DisassemblyView::goTo(medusa::Database::View::LineInformation const & lineInfo)
{
  int line;
  if (!_db->GetView().ConvertLineInformationToLine(lineInfo, line)) return false;
  verticalScrollBar()->setValue(line);
  return true;
}

void DisassemblyView::setFont(QFont const & font)
{
  QAbstractScrollArea::setFont(font);
  const QFontMetrics metrics(font);

  _wChar = metrics.width('X');
  _hChar = metrics.height();

  updateScrollbars();
}

void DisassemblyView::listingUpdated(void)
{
  viewport()->update();
}

void DisassemblyView::paintEvent(QPaintEvent * evt)
{
  if (_db == nullptr) return;

  updateScrollbars();
  typedef medusa::Database::View::LineInformation LineInformation;

  QColor color;

  QPainter p(viewport());
  p.fillRect(viewport()->rect(), Qt::white);

  if (_db == nullptr) return;

  int curLine = verticalScrollBar()->value();
  int offLine = -(horizontalScrollBar()->value() * _wChar);
  int endLine = viewport()->rect().height() / _hChar + 1;

  p.setRenderHints(QPainter::TextAntialiasing);

  LineInformation lineInfo;
  for (int line = 0; line < endLine && _db->GetLineInformation(line + curLine, lineInfo); ++line)
  {
    QString lineStr = QString::fromStdString(lineInfo.GetAddress().ToString()) + QString(": ");
    switch (lineInfo.GetType())
    {
    case LineInformation::CellLineType:
      {
        medusa::Cell const * curCell = _db->RetrieveCell(lineInfo.GetAddress());
        color = Qt::red;

        if (curCell != nullptr)
          lineStr += QString("  ") + QString::fromStdString(curCell->ToString());
        break;
      }

    case LineInformation::MultiCellLineType:
      {
        medusa::MultiCell const *curMultiCell = _db->RetrieveMultiCell(lineInfo.GetAddress());
        color = Qt::green;

        if (curMultiCell != nullptr)
          lineStr += QString::fromStdString(curMultiCell->ToString());
        break;
      }

    case LineInformation::LabelLineType:
      {
        medusa::Label curLabel = _db->GetLabelFromAddress(lineInfo.GetAddress());
        color = Qt::blue;

        if (curLabel.GetType() != medusa::Label::LabelUnknown)
          lineStr += QString::fromStdString(curLabel.GetLabel()) + QString(":");
        break;
      }

    case LineInformation::MemoryAreaLineType:
      {
        medusa::MemoryArea const* memArea = _db->GetMemoryArea(lineInfo.GetAddress());
        if (memArea == nullptr) break;
        lineStr += QString::fromStdString(memArea->ToString());
        color = Qt::magenta;
        break;
      }

    case LineInformation::EmptyLineType:
      continue;

    default:
      break;
    }

    p.setPen(color);
    p.drawText(offLine, _yOffset + line * _hChar, lineStr);
  }
}

void DisassemblyView::mouseMoveEvent(QMouseEvent * evt)
{
  medusa::Address addr;

  if (!convertMouseToAddress(evt, addr)) return;
  qDebug() << QString::fromStdString(addr.ToString());
}

//void DisassemblyView::mousePressEvent(QMouseEvent * evt)
//{
//  int line;
//  medusa::Address addr;
//
//  if (!convertMouseToAddress(evt, addr)) return;
//
//  medusa::Database::View::LineInformation lineInfo(medusa::Database::View::LineInformation::CellLineType, addr);
//  if (!_db->GetView().ConvertLineInformationToLine(lineInfo, line)) return;
//
//  qDebug() << QString::fromStdString(addr.ToString()) << " " << line;
//  if (evt->type() == QEvent::MouseButtonDblClick)
//  {
//    goTo(lineInfo);
//  }
//}

void DisassemblyView::mouseDoubleClickEvent(QMouseEvent * evt)
{
  medusa::Address srcAddr, dstAddr;

  if (!convertMouseToAddress(evt, srcAddr)) return;

  medusa::Cell const* cell = _db->RetrieveCell(srcAddr);
  if (cell == nullptr) return;

  typedef medusa::Database::View::LineInformation LineInformation;

  auto memArea = _db->GetMemoryArea(srcAddr);

  for (medusa::u8 op = 0; op < 4; ++op)
  {
    if ( memArea != nullptr
      && cell->GetType() == medusa::Cell::InstructionType
      && static_cast<medusa::Instruction const*>(cell)->GetOperandReference(memArea->GetBinaryStream(), op, srcAddr, dstAddr))
      if (goTo(LineInformation(LineInformation::CellLineType, dstAddr)))
        return;
  }
}

void DisassemblyView::updateScrollbars(void)
{
  if (_db == nullptr) return;

  int numberOfLine = static_cast<int>(_db->GetNumberOfLine());
  if (numberOfLine == 0) return;

  int max = numberOfLine - (viewport()->rect().height() / _hChar);
  if (max < 0) max = 0;

  verticalScrollBar()->setMaximum(max);
  //horizontalScrollBar()->setMaximum(_lineLen);
}

bool DisassemblyView::convertMouseToAddress(QMouseEvent * evt, medusa::Address & addr)
{
  int line = evt->pos().y() / _hChar + verticalScrollBar()->value();
  medusa::Database::View::LineInformation lineInfo;

  if (!_db->GetLineInformation(line, lineInfo)) return false;

  addr = lineInfo.GetAddress();
  return true;
}