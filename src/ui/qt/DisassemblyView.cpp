#include "DisassemblyView.hpp"

DisassemblyView::DisassemblyView(QWidget * parent)
  : QAbstractScrollArea(parent)
  , _core(nullptr)
  , _db(nullptr)
  , _xOffset(0),            _yOffset(10)
  , _wChar(0),              _hChar(0)
  , _xCursor(0),            _yCursor(0)
  , _begSelection(0),       _endSelection(0)
  , _begSelectionOffset(0), _endSelectionOffset(0)
  , _addrLen(0)
  , _lineNo(0x0),           _lineLen(0x100)
  , _cursorTimer(),         _cursorBlink(false)
  , _visibleLines()
  , _curAddr()
  , _disasmAct(nullptr)
  , _toByteAct(nullptr), _toWordAct(nullptr), _toDwordAct(nullptr), _toQwordAct(nullptr)
{
  setFont();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  connect(&_cursorTimer, SIGNAL(timeout()), this, SLOT(updateCursor()));
  _cursorTimer.setInterval(400);
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(QPoint const &)), this, SLOT(showContextMenu(QPoint const &)));

  _disasmAct = new QAction(tr("&Disassemble"), this);
  _disasmAct->setStatusTip(tr("Disassemble the current line"));
  connect(_disasmAct, SIGNAL(triggered()), this, SLOT(disassembleCurrentLine(void)));

  _toByteAct = new QAction(tr("To byte"), this);
  _toByteAct->setStatusTip("Convert to byte");
  connect(_toByteAct, SIGNAL(triggered()), this, SLOT(toByte()));

  _toWordAct = new QAction(tr("To word"), this);
  _toWordAct->setStatusTip("Convert to word");
  connect(_toWordAct, SIGNAL(triggered()), this, SLOT(toWord()));

  _toDwordAct = new QAction(tr("To dword"), this);
  _toDwordAct->setStatusTip("Convert to dword");
  connect(_toDwordAct, SIGNAL(triggered()), this, SLOT(toDword()));

  _toQwordAct = new QAction(tr("To qword"), this);
  _toQwordAct->setStatusTip("Convert to qword");
  connect(_toQwordAct, SIGNAL(triggered()), this, SLOT(toQword()));

  _toStringAct = new QAction(tr("To ASCII string"), this);
  _toStringAct->setStatusTip("Convert to ASCII string");
  connect(_toStringAct, SIGNAL(triggered()), this, SLOT(toString()));
}

DisassemblyView::~DisassemblyView(void)
{
}

void DisassemblyView::bindMedusa(medusa::Medusa * core)
{
  _core = core;
  _db = &core->GetDatabase();
  _lineNo = static_cast<int>(_db->GetView().GetNumberOfLine());
}

void DisassemblyView::clear(void)
{
  _db = nullptr;
  _visibleLines.clear();
}

bool DisassemblyView::goTo(medusa::Database::View::LineInformation const & lineInfo)
{
  int line;
  if (!_db->GetView().ConvertLineInformationToLine(lineInfo, line)) return false;
  verticalScrollBar()->setValue(line);
  resetSelection();
  return true;
}

void DisassemblyView::setFont(void)
{
  QString fontInfo = Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString();
  QFont font;
  font.fromString(fontInfo);
  QAbstractScrollArea::setFont(font);
  const QFontMetrics metrics(font);

  _wChar = metrics.width('X');
  _hChar = metrics.height();

  updateScrollbars();
  listingUpdated();
}

void DisassemblyView::listingUpdated(void)
{
  viewport()->update();
}

void DisassemblyView::updateCursor(void)
{
  _cursorBlink = _cursorBlink ? false : true;
  emit listingUpdated();
}

void DisassemblyView::showContextMenu(QPoint const & pos)
{
  if (_db == nullptr) return;

  QMenu menu;
  QPoint globalPos = viewport()->mapToGlobal(pos);

  medusa::Address selectedAddress;
  if (!convertPositionToAddress(pos, selectedAddress)) return;

  menu.addAction(QString::fromStdString(selectedAddress.ToString()));
  menu.addAction(_disasmAct);
  menu.addAction(_toByteAct);
  menu.addAction(_toWordAct);
  menu.addAction(_toDwordAct);
  menu.addAction(_toQwordAct);
  menu.addAction(_toStringAct);

  QAction * selectedItem = menu.exec(globalPos);
}

void DisassemblyView::disassembleCurrentLine(void)
{
  //medusa::Cell* cell = _db->RetrieveCell(_curAddr);
  //if (cell == nullptr) return;

  //cell->SetComment(_curAddr.ToString());
  //_db->UpdateCell(_curAddr, cell);
  _core->DisassembleAsync(_curAddr);
}

void DisassemblyView::toByte(void)
{
  _db->ChangeValueSize(_curAddr, 8, true);
}

void DisassemblyView::toWord(void)
{
  _db->ChangeValueSize(_curAddr, 16, true);
}

void DisassemblyView::toDword(void)
{
  _db->ChangeValueSize(_curAddr, 32, true);
}

void DisassemblyView::toQword(void)
{
  _db->ChangeValueSize(_curAddr, 64, true);
}

void DisassemblyView::toString(void)
{
  _db->MakeString(_curAddr);
}

void DisassemblyView::paintEvent(QPaintEvent * evt)
{
  QPainter p(viewport());

  auto drawText = [this](QPainter & p, int x, int y, QString const & text)
  {
    foreach (QChar chr, text)
    {
      QFontMetrics fm(this->font());
      int chrWidth = fm.width(chr);
      int chrOffset = this->_wChar / 2 - chrWidth / 2;
      if (chrOffset < 0)
        chrOffset = 0;
      p.drawText(x + chrOffset, y, chr);
      x += this->_wChar;
    }
  };

  // Draw background
  QColor addrColor = QColor(Settings::instance().value(MEDUSA_COLOR_ADDRESS_BACKGROUND, MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT).toString());
  QColor codeColor = QColor(Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());
  QColor slctColor = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_SELECTION, MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT).toString());

  viewport()->rect();
  QRect addrRect = viewport()->rect();
  QRect codeRect = viewport()->rect();

  addrRect.setWidth((_addrLen - horizontalScrollBar()->value()) * _wChar);
  codeRect.setX((_addrLen - horizontalScrollBar()->value()) * _wChar);

  p.fillRect(addrRect, addrColor);
  p.fillRect(codeRect, codeColor);

  // If we don't have the database, we don't need to redraw the whole widget
  if (_db == nullptr) return;

  _visibleLines.clear();
  _visibleLines.reserve(horizontalScrollBar()->maximum());

  int begSelect    = _begSelection;
  int endSelect    = _endSelection;
  int begSelectOff = _begSelectionOffset;
  int endSelectOff = _endSelectionOffset;
  int deltaSelect  = _endSelection - _begSelection;

  // If the user select from the bottom to the top, we have to swap up and down
  if (deltaSelect < 0)
  {
    deltaSelect  = -deltaSelect;
    begSelect    = _endSelection;
    endSelect    = _begSelection;
    begSelectOff = _endSelectionOffset;
    endSelectOff = _begSelectionOffset;
  }

  if (deltaSelect) deltaSelect++;

  if (deltaSelect == 0)
  {
    int deltaOffset = endSelectOff - begSelectOff;
    if (deltaOffset < 0)
    {
      deltaOffset = -deltaOffset;
      begSelectOff = _endSelectionOffset;
      endSelectOff = _begSelectionOffset;
    }

    int x = (begSelectOff - horizontalScrollBar()->value()) * _wChar;
    int w = (horizontalScrollBar()->value() - endSelectOff) * _wChar;
    QRect slctRect(x, (begSelect - verticalScrollBar()->value()) * _hChar, deltaOffset * _wChar, _hChar);
    p.fillRect(slctRect, slctColor);
  }

  // Draw selection background
  // NOTE I'm pretty sure we can replace this loop with clever math, but I'm not in the mood
  else if (deltaSelect > 0)
  {
    int x = (begSelectOff - horizontalScrollBar()->value()) * _wChar;
    QRect slctRect(x, (begSelect - verticalScrollBar()->value()) * _hChar, viewport()->rect().width() - x, _hChar);
    p.fillRect(slctRect, slctColor);

    for (int slctLine = 1; slctLine < deltaSelect; ++slctLine)
    {
      slctRect.setX((_addrLen - horizontalScrollBar()->value()) * _wChar);
      slctRect.setWidth(horizontalScrollBar()->maximumWidth());
      slctRect.setY(slctRect.y() + _hChar);
      p.fillRect(slctRect, slctColor);
    }

    int w = (endSelectOff - _addrLen) * _wChar;
    slctRect.setWidth(w);
    slctRect.setY(slctRect.y() + _hChar);
    p.fillRect(slctRect, slctColor);
  }

  typedef medusa::Database::View::LineInformation LineInformation;

  QColor color;

  p.setRenderHints(QPainter::TextAntialiasing);

  LineInformation lineInfo;

  int endLine = viewport()->rect().height() / _hChar + 1;
  int curLine = verticalScrollBar()->value();

  for (int line = 0; line < endLine && _db->GetView().GetLineInformation(line + curLine, lineInfo); ++line)
    _addrLen = qMax(static_cast<int>(lineInfo.GetAddress().ToString().length() + 1), _addrLen);

  int offLine = (_addrLen + 1) * _wChar;
  int begLine = -(horizontalScrollBar()->value() * _wChar);

  // Draw address lines
  for (int line = 0; line < endLine && _db->GetView().GetLineInformation(line + curLine, lineInfo); ++line)
    drawText(p, begLine, _yOffset + line * _hChar, QString::fromStdString(lineInfo.GetAddress().ToString()));

  // Draw assembly code lines
  for (int line = 0; line < endLine && _db->GetView().GetLineInformation(line + curLine, lineInfo); ++line)
  {
    QString lineStr = "Invalid line !";
    QString visibleLine = "";
    switch (lineInfo.GetType())
    {
    case LineInformation::CellLineType:
      {
        medusa::Cell const * curCell = _core->GetCell(lineInfo.GetAddress());
        if (curCell == nullptr) break;

        medusa::u16 offset = 0;

        medusa::Cell::Mark::List marks = curCell->GetMarks();
        if (marks.empty())
        {
          color = Qt::black;
          lineStr = QString::fromStdString(curCell->ToString());
          break;
        }

        std::for_each(std::begin(marks), std::end(marks), [&](medusa::Cell::Mark mark)
        {
          QColor cellClr(Qt::black);
          QString cellStr = QString::fromStdString(curCell->ToString().substr(offset, mark.GetLength()));

          switch (mark.GetType())
          {
          case medusa::Cell::Mark::MnemonicType:  cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_MNEMONIC, MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT).toString()); break;
          case medusa::Cell::Mark::KeywordType:   cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_KEYWORD, MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT).toString()); break;
          case medusa::Cell::Mark::ImmediateType: cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString()); break;
          case medusa::Cell::Mark::OperatorType:  cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_OPERATOR, MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT).toString()); break;
          case medusa::Cell::Mark::RegisterType:  cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_REGISTER, MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT).toString()); break;
          case medusa::Cell::Mark::LabelType:     cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL, MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString()); break;
          case medusa::Cell::Mark::StringType:    cellClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_STRING, MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT).toString()); break;
          default: break;
          };

          p.setPen(cellClr);
          drawText(p, begLine + offset * _wChar + offLine, _yOffset + line * _hChar, cellStr);
          offset += mark.GetLength();
        });

        lineStr = "";
        visibleLine = QString::fromStdString(curCell->ToString());
        if (!curCell->GetComment().empty())
        {
          p.setPen(QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString()));
          drawText(p, begLine + offset * _wChar + offLine, _yOffset + line * _hChar, QString(" ; ") + QString::fromStdString(curCell->GetComment()));
          visibleLine += QString(" ; %1").arg(QString::fromStdString(curCell->GetComment()));
        }
        break;
      }

    case LineInformation::MultiCellLineType:
      {
        medusa::MultiCell const *curMultiCell = _db->RetrieveMultiCell(lineInfo.GetAddress());
        color = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT, MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());

        if (curMultiCell != nullptr)
          lineStr = QString::fromStdString(curMultiCell->ToString());
        break;
      }

    case LineInformation::LabelLineType:
      {
        medusa::Label curLabel = _db->GetLabelFromAddress(lineInfo.GetAddress());
        color = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL, MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());

        if (curLabel.GetType() != medusa::Label::LabelUnknown)
          lineStr = QString::fromStdString(curLabel.GetLabel()) + QString(":");
        break;
      }

    case LineInformation::XrefLineType:
      {
        medusa::Address::List RefAddrList;
        _db->GetXRefs().From(lineInfo.GetAddress(), RefAddrList);
        lineStr = QString::fromUtf8(";:xref");

        std::for_each(std::begin(RefAddrList), std::end(RefAddrList), [&](medusa::Address const& rRefAddr)
        {
          lineStr += QString(" ") + (rRefAddr < lineInfo.GetAddress() ? QString::fromUtf8("\xe2\x86\x91") : QString::fromUtf8("\xe2\x86\x93")) + QString::fromStdString(rRefAddr.ToString());
        });
        color = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_KEYWORD, MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT).toString());
        break;
      }

    case LineInformation::MemoryAreaLineType:
      {
        medusa::MemoryArea const* memArea = _db->GetMemoryArea(lineInfo.GetAddress());
        if (memArea == nullptr) break;
        lineStr = QString::fromStdString(memArea->ToString());
        color = Qt::magenta;
        break;
      }

    case LineInformation::EmptyLineType: _visibleLines.push_back(""); continue;
    default: break;
    }

    if (visibleLine.isEmpty())
      visibleLine = lineStr;

    if (!visibleLine.isEmpty())
      _visibleLines.push_back(visibleLine);

    if (lineStr.isEmpty()) continue;

    p.setPen(color);
    drawText(p, begLine + offLine, _yOffset + line * _hChar, lineStr);
  }

  // Draw cursor
  if (_cursorBlink)
  {
    QColor cursorColor = ~codeColor.value();
    QRect cursorRect((_xCursor - horizontalScrollBar()->value()) * _wChar, (_yCursor - verticalScrollBar()->value()) * _hChar, 2, _hChar);
    p.fillRect(cursorRect, cursorColor);
  }

  updateScrollbars();
}

void DisassemblyView::mouseMoveEvent(QMouseEvent * evt)
{
  if (_db == nullptr) return;

  medusa::Address addr;

  if (!convertMouseToAddress(evt, addr)) return;

  setCursorPosition(evt);

  if (evt->buttons() & Qt::LeftButton)
  {
    int xCursor = evt->x() / _wChar + horizontalScrollBar()->value();
    int yCursor = evt->y() / _hChar + verticalScrollBar()->value();

    if (xCursor < _addrLen)
      xCursor = _addrLen;
    _endSelection       = yCursor;
    _endSelectionOffset = xCursor;
  }
}

void DisassemblyView::mousePressEvent(QMouseEvent * evt)
{
  if (_db == nullptr) return;

  medusa::Address addr;
  if (convertMouseToAddress(evt, addr))
    _curAddr = addr;

  setCursorPosition(evt);

  if (evt->buttons() & Qt::LeftButton)
  {
    int xCursor = evt->x() / _wChar + horizontalScrollBar()->value();
    int yCursor = evt->y() / _hChar + verticalScrollBar()->value();

    if (xCursor < _addrLen)
      xCursor = _addrLen;
    _begSelection       = yCursor;
    _begSelectionOffset = xCursor;
    _endSelection       = yCursor;
    _endSelectionOffset = xCursor;
  }
}

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
      && cell->GetType() == medusa::CellData::InstructionType
      && static_cast<medusa::Instruction const*>(cell)->GetOperandReference(*_db, op, srcAddr, dstAddr))
      if (goTo(LineInformation(LineInformation::CellLineType, dstAddr)))
        return;
  }
}

void DisassemblyView::keyPressEvent(QKeyEvent * evt)
{
  // Move cursor
  if (evt->matches(QKeySequence::MoveToNextChar))           { moveCursorPosition(+1, 0); resetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousChar))       { moveCursorPosition(-1, 0); resetSelection(); }

  if (evt->matches(QKeySequence::MoveToStartOfLine))        { setCursorPosition(_addrLen, -1); resetSelection(); }
  if (evt->matches(QKeySequence::MoveToEndOfLine))
  {
    do
    {
      int line = _yCursor - horizontalScrollBar()->value();
      if (line >= _visibleLines.size()) break;
      QString curLine = _visibleLines.at(static_cast<std::vector<QString>::size_type>(line));

      setCursorPosition(_addrLen + 1 + curLine.length(), -1);
    } while (0);
    resetSelection();
  }

  if (evt->matches(QKeySequence::MoveToNextLine))           { moveCursorPosition(0, +1); resetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousLine))       { moveCursorPosition(0, -1); resetSelection(); }

  if (evt->matches(QKeySequence::MoveToNextPage))           { moveCursorPosition(0, +viewport()->rect().height() / _hChar); resetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousPage))       { moveCursorPosition(0, -viewport()->rect().height() / _hChar); resetSelection(); }

  if (evt->matches(QKeySequence::MoveToStartOfDocument))    { setCursorPosition(_addrLen, 0); resetSelection(); }
  if (evt->matches(QKeySequence::MoveToEndOfDocument))      { setCursorPosition(_addrLen, horizontalScrollBar()->maximum() - 1); resetSelection(); }

  if (evt->matches(QKeySequence::MoveToNextWord))
  {
    do
    {
      int line = _yCursor - horizontalScrollBar()->value();
      if (line >= _visibleLines.size()) break;

      QString curLine = _visibleLines.at(static_cast<std::vector<QString>::size_type>(line));

      int newPosition = curLine.indexOf(" ", _xCursor - _addrLen - 1);

      if      (newPosition == -1) newPosition = curLine.length();
      else if (newPosition < curLine.length() && newPosition == _xCursor - _addrLen - 1)
      {
        newPosition = curLine.indexOf(" ", _xCursor - _addrLen);
        if (newPosition == -1) newPosition = curLine.length();
      }

      setCursorPosition(newPosition + _addrLen + 1, -1);
    } while (0);
    resetSelection();
  }

  if (evt->matches(QKeySequence::MoveToPreviousWord))
  {
    do
    {
      int line = _yCursor - horizontalScrollBar()->value();
      if (line >= _visibleLines.size()) break;

      QString curLine = _visibleLines.at(static_cast<std::vector<QString>::size_type>(line));

      if (_xCursor - _addrLen - 2 < 0) break;

      int newPosition = curLine.lastIndexOf(" ", _xCursor - _addrLen - 2);

      if (newPosition == 0) break;

      else if (newPosition == -1) newPosition = 0;

      setCursorPosition(newPosition + _addrLen + 1, -2);
    } while (0);
    resetSelection();
  }

  // Move selection
  if (evt->matches(QKeySequence::SelectNextChar))           moveSelection(+1, 0);
  if (evt->matches(QKeySequence::SelectPreviousChar))       moveSelection(-1, 0);

  if (evt->matches(QKeySequence::SelectStartOfLine))        setSelection(_addrLen, -1);
  if (evt->matches(QKeySequence::SelectEndOfLine))
  {
    do
    {
      int line = _yCursor - horizontalScrollBar()->value();
      if (line >= _visibleLines.size()) break;
      QString curLine = _visibleLines.at(static_cast<std::vector<QString>::size_type>(line));

      setSelection(_addrLen + 1 + curLine.length(), -1);
    } while (0);
  }

  if (evt->matches(QKeySequence::SelectNextLine))           moveSelection(0, +1);
  if (evt->matches(QKeySequence::SelectPreviousLine))       moveSelection(0, -1);

  if (evt->matches(QKeySequence::SelectNextPage))           moveSelection(0, +viewport()->rect().height());
  if (evt->matches(QKeySequence::SelectPreviousPage))       moveSelection(0, -viewport()->rect().height());

  if (evt->matches(QKeySequence::SelectStartOfDocument))    setSelection(-1, 0);
  if (evt->matches(QKeySequence::SelectEndOfDocument))      setSelection(-1, horizontalScrollBar()->maximum());

  if (evt->matches(QKeySequence::SelectNextWord))
  {
    do
    {
      int line = _yCursor - horizontalScrollBar()->value();
      if (line >= _visibleLines.size()) break;

      QString curLine = _visibleLines.at(static_cast<std::vector<QString>::size_type>(line));

      int newPosition = curLine.indexOf(" ", _xCursor - _addrLen - 1);

      if      (newPosition == -1) newPosition = curLine.length();
      else if (newPosition < curLine.length() && newPosition == _xCursor - _addrLen - 1)
      {
        newPosition = curLine.indexOf(" ", _xCursor - _addrLen);
        if (newPosition == -1) newPosition = curLine.length();
      }

      setSelection(newPosition + _addrLen + 1, -1);
    } while (0);
  }

  if (evt->matches(QKeySequence::SelectPreviousWord))
  {
    do
    {
      int line = _yCursor - horizontalScrollBar()->value();
      if (line >= _visibleLines.size()) break;

      QString curLine = _visibleLines.at(static_cast<std::vector<QString>::size_type>(line));

      if (_xCursor - _addrLen - 2 < 0) break;

      int newPosition = curLine.lastIndexOf(" ", _xCursor - _addrLen - 2);

      if (newPosition == 0) break;

      else if (newPosition == -1) newPosition = 0;

      setSelection(newPosition + _addrLen + 1, -2);
    } while (0);
  }

  if (evt->matches(QKeySequence::SelectAll))
  {
    _begSelection       = 0;
    _endSelection       = horizontalScrollBar()->maximum();
    _begSelectionOffset = _addrLen;
    _endSelectionOffset = verticalScrollBar()->maximum();
    moveCursorPosition(_endSelectionOffset, _endSelection);
  }

  // Copy
  if (evt->matches(QKeySequence::Copy))
  {
    do
    {
      QClipboard * clipboard = QApplication::clipboard();

      if (_db == nullptr) break;

      if (_begSelection == _endSelection && _begSelectionOffset == _endSelectionOffset) break;

      int selectLineNr = _endSelection - _begSelection;
      int begLines     = _begSelection;
      int leftOffLine  = _begSelectionOffset - _addrLen - 1;
      int rightOffLine = _endSelectionOffset - _addrLen;

      if (selectLineNr == 0 && leftOffLine > rightOffLine)
      {
        leftOffLine    = _endSelectionOffset - _addrLen - 1;
        rightOffLine   = _begSelectionOffset - _addrLen;
      }

      else if (selectLineNr < 0)
      {
        selectLineNr   = -selectLineNr;
        begLines       = _endSelection;
        leftOffLine    = _endSelectionOffset - _addrLen - 1;
        rightOffLine   = _begSelectionOffset - _addrLen;
      }

      selectLineNr++;

      typedef medusa::Database::View::LineInformation LineInformation;
      LineInformation lineInfo;

      QString clipboardBuf = "";
      for (int selectLine = 0; selectLine < selectLineNr; ++selectLine)
      {
        if (!_db->GetView().GetLineInformation(begLines + selectLine, lineInfo)) break;
        switch (lineInfo.GetType())
        {
        case LineInformation::EmptyLineType: clipboardBuf += "\n"; break;;

        case LineInformation::CellLineType:
          {
            medusa::Cell const * cell = _db->RetrieveCell(lineInfo.GetAddress());
            if (cell == nullptr) break;
            clipboardBuf += QString::fromStdString(cell->ToString());
            if (!cell->GetComment().empty())
              clipboardBuf += QString(" ; %1").arg(QString::fromStdString(cell->GetComment()));
            clipboardBuf += "\n";
            break;
          }

        case LineInformation::MultiCellLineType:
          {
            medusa::MultiCell const * multicell = _db->RetrieveMultiCell(lineInfo.GetAddress());
            if (multicell == nullptr) break;
            clipboardBuf += QString("%1\n").arg(QString::fromStdString(multicell->ToString()));
            break;
          }

        case LineInformation::LabelLineType:
          {
            medusa::Label lbl = _db->GetLabelFromAddress(lineInfo.GetAddress());
            if (lbl.GetType() == medusa::Label::LabelUnknown) break;
            clipboardBuf += QString("%1:\n").arg(QString::fromStdString(lbl.GetLabel()));
            break;
          }

        case LineInformation::XrefLineType:
          {
            medusa::Address::List RefAddrList;
            _db->GetXRefs().From(lineInfo.GetAddress(), RefAddrList);
            clipboardBuf += QString::fromUtf8(";:xref");

            std::for_each(std::begin(RefAddrList), std::end(RefAddrList), [&](medusa::Address const& rRefAddr)
            {
              clipboardBuf += QString(" ") + (rRefAddr < lineInfo.GetAddress() ? QString::fromUtf8("\xe2\x86\x91") : QString::fromUtf8("\xe2\x86\x93")) + QString::fromStdString(rRefAddr.ToString());
            });
            clipboardBuf += "\n";
            break;
          }

        case LineInformation::MemoryAreaLineType:
          {
            medusa::MemoryArea const * memArea = _db->GetMemoryArea(lineInfo.GetAddress());
            if (memArea == nullptr) break;
            clipboardBuf += QString("%1\n").arg(QString::fromStdString(memArea->ToString()));
            break;
          }

        default: break;
        }
      }
      int clipboardLen = clipboardBuf.length() - leftOffLine;

      int lastLineOff = clipboardBuf.lastIndexOf("\n", -2);
      int lastLineLen = clipboardBuf.length() - lastLineOff;
      clipboardLen -= (lastLineLen - rightOffLine);

      clipboardBuf = clipboardBuf.mid(leftOffLine, clipboardLen);
      if (!clipboardBuf.isEmpty())
        clipboard->setText(clipboardBuf);
    } while (0);
  }
}

void DisassemblyView::setCursorPosition(QMouseEvent * evt)
{
  int xCursor = evt->x() / _wChar + horizontalScrollBar()->value();
  int yCursor = evt->y() / _hChar + verticalScrollBar()->value();

  if (xCursor > _addrLen)
  {
    _xCursor = xCursor;
    _yCursor = yCursor;
  }
  _cursorTimer.start();
  _cursorBlink = false;
  updateCursor();
  ensureCursorIsVisible();
}

void DisassemblyView::setCursorPosition(int x, int y)
{
  if (x < 0)
    x = _xCursor;
  if (x >= verticalScrollBar()->maximum())
    x = verticalScrollBar()->maximum() - 1;
  _xCursor = x;

  if (y < 0)
    y = _yCursor;
  if (y >= horizontalScrollBar()->maximum())
    y = horizontalScrollBar()->maximum() - 1;
  _yCursor = y;

  _cursorTimer.start();
  _cursorBlink = false;
  updateCursor();
  ensureCursorIsVisible();
}

void DisassemblyView::moveCursorPosition(int x, int y)
{
  x += _xCursor;
  y += _yCursor;
  setCursorPosition(x, y);
}

void DisassemblyView::resetSelection(void)
{
  _begSelection       = _endSelection       = _yCursor;
  _begSelectionOffset = _endSelectionOffset = _xCursor;
}

void DisassemblyView::setSelection(int x, int y)
{
  setCursorPosition(x, y);

  if ( x >= _addrLen
    && x < verticalScrollBar()->maximum())
    _endSelectionOffset = x;

  if ( y >= 0
    && y < horizontalScrollBar()->maximum())
    _endSelection = y;
}

void DisassemblyView::moveSelection(int x, int y)
{
  moveCursorPosition(x, y);
  _endSelectionOffset += x;
  _endSelection       += y;
}

void DisassemblyView::updateScrollbars(void)
{
  if (_db == nullptr) return;

  int max = _lineNo - (viewport()->rect().height() / _hChar);
  if (max < 0) max = 0;

  verticalScrollBar()->setMaximum(max);
  horizontalScrollBar()->setMaximum(_lineLen);
}

bool DisassemblyView::convertPositionToAddress(QPoint const & pos, medusa::Address & addr)
{
  int line = pos.y() / _hChar + verticalScrollBar()->value();
  medusa::Database::View::LineInformation lineInfo;

  if (!_db->GetView().GetLineInformation(line, lineInfo)) return false;

  addr = lineInfo.GetAddress();
  return true;
}

bool DisassemblyView::convertMouseToAddress(QMouseEvent * evt, medusa::Address & addr)
{
  return convertPositionToAddress(evt->pos(), addr);
}

void DisassemblyView::ensureCursorIsVisible(void)
{
  int begViewport = viewport()->rect().y() / _hChar + verticalScrollBar()->value();
  int endViewport = begViewport + viewport()->rect().height() / _hChar;

  if (_yCursor < begViewport)
    verticalScrollBar()->setValue(_yCursor);
  else if (_yCursor > endViewport)
    verticalScrollBar()->setValue(_yCursor - (endViewport - begViewport) - 2);
}