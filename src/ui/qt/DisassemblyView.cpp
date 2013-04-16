#include "DisassemblyView.hpp"

#include "medusa/cell_action.hpp"

DisassemblyView::DisassemblyView(QWidget * parent)
  : QAbstractScrollArea(parent)
  , _needRepaint(true)
  , _core(nullptr)
  , _db(nullptr)
  , _xOffset(0),            _yOffset(10)
  , _wChar(0),              _hChar(0)
  , _xCursor(0),            _yCursor(0)
  , _begSelection(0),       _endSelection(0)
  , _begSelectionOffset(0), _endSelectionOffset(0)
  , _oldVertiScValue(0),   _oldHorizScValue(0)
  , _addrLen(0)
  , _lineNo(0x0),           _lineLen(0x100)
  , _cursorTimer(),         _cursorBlink(false)
  , _visibleLines()
  , _curAddr()
  , _cache()
  , _scr(nullptr)
  , _dp(nullptr)
{
  setFont();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  connect(&_cursorTimer, SIGNAL(timeout()), this, SLOT(updateCursor()));
  _cursorTimer.setInterval(400);
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(QPoint const &)), this, SLOT(showContextMenu(QPoint const &)));
  connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScrollBarChanged(int)));
  connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScrollBarChanged(int)));
}

DisassemblyView::~DisassemblyView(void)
{
  delete _scr;
  delete _dp;
}

void DisassemblyView::bindMedusa(medusa::Medusa * core)
{
  _core = core;
  _db = &core->GetDatabase();
  _lineNo = static_cast<int>(_db->GetNumberOfAddress());

  _addrLen = static_cast<int>((*_core->GetDatabase().Begin())->GetVirtualBase().ToString().length() + 1);

  // init disassembly printer
  if (_dp != nullptr)
    delete _dp;
  _dp = new DisassemblyPrinter(*_core);

  // init screen
  if (_scr != nullptr)
    delete _scr;
  auto firstAddr = (*_core->GetDatabase().Begin())->GetVirtualBase();
  int w, h;
  QRect rect = viewport()->rect();
  w = rect.width() / _wChar;
  h = rect.height() / _hChar;
  _scr = new Screen(*_core, *_dp, w, h, firstAddr);
}

void DisassemblyView::clear(void)
{
  _db = nullptr;
  _visibleLines.clear();
}

bool DisassemblyView::goTo(medusa::Address const& address)
{
  u64 newPos = 0;
  if (_core->GetDatabase().ConvertAddressToPosition(address, newPos) == true)
  {
    verticalScrollBar()->setValue(static_cast<int>(newPos));
    emit viewUpdated();
    return true;
  }

  return false;
}

void DisassemblyView::setFont(void)
{
  QString fontInfo = Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString();
  QFont font;
  font.fromString(fontInfo);
  QAbstractScrollArea::setFont(font);

  _wChar = fontMetrics().width('M');
  _hChar = fontMetrics().height();

  updateScrollbars();
  viewUpdated();
}

void DisassemblyView::viewUpdated(void)
{
  if (_db == nullptr)
    return;
  _lineNo = static_cast<int>(_db->GetNumberOfAddress());

  if (_scr != nullptr)
    _scr->Refresh();

  viewport()->update();
  _needRepaint = true;
}

void DisassemblyView::verticalScrollBarChanged(int n)
{
  if (_scr != nullptr)
  {
    _scr->Move(-1, n);
    //_scr->Scroll(0, n - _oldVertiScValue);
    emit viewUpdated();
  }
  _oldVertiScValue = n;
}

void DisassemblyView::horizontalScrollBarChanged(int n)
{
  _oldHorizScValue = n;
  emit viewUpdated();
}

void DisassemblyView::listingUpdated(void)
{
  if (_db == nullptr)
    return;
  _lineNo = static_cast<int>(_db->GetNumberOfAddress());

  if (_scr != nullptr)
    _scr->Refresh();

  viewport()->update();
  _needRepaint = true;
}

void DisassemblyView::updateCursor(void)
{
  _cursorBlink = _cursorBlink ? false : true;
  viewport()->update();
}

void DisassemblyView::showContextMenu(QPoint const & pos)
{
  if (_db == nullptr) return;

  QMenu menu;
  QPoint globalPos = viewport()->mapToGlobal(pos);

  medusa::Address::List selectedAddresses;
  getSelectedAddresses(selectedAddresses);

  if (selectedAddresses.size() == 0)
    selectedAddresses.push_back(_curAddr);

  medusa::CellAction::PtrList actions;
  medusa::CellAction::GetCellActionBuilders(actions);
  QHash<QAction*, medusa::CellAction*> actToCellAct;

  for (auto curAddress = std::begin(selectedAddresses); curAddress != std::end(selectedAddresses); ++curAddress)
  {
    medusa::Address selectedAddress = *curAddress;

    auto cell = _core->GetCell(selectedAddress);
    if (cell == nullptr) continue;

    if (actions.size() == 0) return;

    for (auto act = std::begin(actions); act != std::end(actions); ++act)
    {
      if ((*act)->IsCompatible(*cell) == false)
        continue;

      if (actToCellAct.values().contains(*act))
        continue;

      auto curAct = new QAction(QString::fromStdString((*act)->GetName()), this);
      curAct->setStatusTip(QString::fromStdString((*act)->GetDescription()));
      menu.addAction(curAct);
      actToCellAct[curAct] = *act;
    }
  }

  QAction * selectedItem = menu.exec(globalPos);
  auto curAct = actToCellAct[selectedItem];

  if (curAct == nullptr)
    goto end;

  curAct->Do(*_core, selectedAddresses);

end:
    for (auto act = std::begin(actions); act != std::end(actions); ++act)
      delete *act;
}

void DisassemblyView::paintBackground(QPainter& p)
{
  // Draw background
  QColor addrColor = QColor(Settings::instance().value(MEDUSA_COLOR_ADDRESS_BACKGROUND, MEDUSA_COLOR_ADDRESS_BACKGROUND_DEFAULT).toString());
  QColor codeColor = QColor(Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());

  QRect addrRect = viewport()->rect();
  QRect codeRect = viewport()->rect();

  addrRect.setWidth((_addrLen - horizontalScrollBar()->value()) * _wChar);
  codeRect.setX((_addrLen - horizontalScrollBar()->value()) * _wChar);

  p.fillRect(addrRect, addrColor);
  p.fillRect(codeRect, codeColor);
}

void DisassemblyView::paintSelection(QPainter& p)
{
  QColor slctColor = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_SELECTION, MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT).toString());

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
    int y = (begSelect - verticalScrollBar()->value()) * _hChar;
    int w = deltaOffset * _wChar;
    int h = _hChar;
    QRect slctRect(x, y, w, h);
    p.fillRect(slctRect, slctColor);
  }

  // Draw selection background
  // This part is pretty tricky:
  // To draw the selection we use the lazy method by three passes.
  /*
     +-----------------+
     |     ############+ Part¹
     |#################+ Part²
     |#################+ Part²
     |####             | Part³
     +-----------------+
  */
  else if (deltaSelect > 0)
  {
    // Part¹
    int x = (begSelectOff - horizontalScrollBar()->value()) * _wChar;
    int y = (begSelect - verticalScrollBar()->value()) * _hChar;
    int w = (viewport()->width() - _addrLen) * _wChar;
    int h = _hChar;
    QRect slctRect(x, y, w, h);
    p.fillRect(slctRect, slctColor);

    // Part²
    if (deltaSelect > 2)
    {
      //auto limit = verticalScrollBar()->value() + viewport()->height();
      //if (limit && deltaSelect > limit)
      //  deltaSelect = limit;

      x = (_addrLen - horizontalScrollBar()->value()) * _wChar;
      y = slctRect.bottom();
      w = (viewport()->width() - _addrLen) * _wChar;
      h = (deltaSelect - 2) * _hChar;
      slctRect.setRect(x, y, w, h);
      p.fillRect(slctRect, slctColor);
    }

    // Part³
    x = (_addrLen - horizontalScrollBar()->value()) * _wChar;
    y = slctRect.bottom();
    w = (endSelectOff - _addrLen) * _wChar;
    h = _hChar;
    slctRect.setRect(x, y, w, h);
    p.fillRect(slctRect, slctColor);
  }
}

void DisassemblyView::paintText(QPainter& p)
{
  QFontMetrics fm = viewport()->fontMetrics();
  _dp->SetPainter(&p);
  _dp->SetFontMetrics(&fm);
  _scr->Print();
  _dp->SetPainter(nullptr);
  _dp->SetFontMetrics(nullptr);
  return;
}

void DisassemblyView::paintCursor(QPainter& p)
{
  QColor codeColor = QColor(Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());

  // Draw cursor
  if (_cursorBlink)
  {
    QColor cursorColor = ~codeColor.value();
    QRect cursorRect((_xCursor - horizontalScrollBar()->value()) * _wChar, (_yCursor - verticalScrollBar()->value()) * _hChar, 2, _hChar);
    p.fillRect(cursorRect, cursorColor);
  }
}

void DisassemblyView::paintEvent(QPaintEvent * evt)
{
  // If we don't have the database, we don't need to redraw the whole widget, only the background
  if (_db == nullptr)
  {
    QPainter p(viewport());
    paintBackground(p);
    return;
  }

  if (_needRepaint == true)
  {
    _cache = QPixmap(viewport()->size());
    QPainter cachedPainter(&_cache);
    paintBackground(cachedPainter);
    paintSelection(cachedPainter);
    cachedPainter.setRenderHint(QPainter::TextAntialiasing);
    cachedPainter.setFont(font());
    paintText(cachedPainter);
    _needRepaint = false;
  }

  QPainter p(viewport());
  p.drawPixmap(0, 0, _cache);
  paintCursor(p);

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

    _needRepaint = true; // TODO: selectionChanged
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

    _needRepaint = true; // TODO: selectionChanged
  }
}

void DisassemblyView::mouseDoubleClickEvent(QMouseEvent * evt)
{
  medusa::Address srcAddr, dstAddr;

  if (!convertMouseToAddress(evt, srcAddr)) return;

  medusa::Cell const* cell = _db->RetrieveCell(srcAddr);
  if (cell == nullptr) return;

  auto memArea = _db->GetMemoryArea(srcAddr);

  for (medusa::u8 op = 0; op < 4; ++op)
  {
    if ( memArea != nullptr
      && cell->GetType() == medusa::CellData::InstructionType
      && static_cast<medusa::Instruction const*>(cell)->GetOperandReference(*_db, op, srcAddr, dstAddr))
      if (goTo(dstAddr))
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

  if (evt->matches(QKeySequence::MoveToNextPage))           { moveCursorPosition(0, +(viewport()->rect().height() / _hChar)); resetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousPage))       { moveCursorPosition(0, -(viewport()->rect().height() / _hChar)); resetSelection(); }

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
    _endSelection       = _lineNo;
    _begSelectionOffset = _addrLen;
    _endSelectionOffset = verticalScrollBar()->maximum();
    //moveCursorPosition(_endSelectionOffset, _endSelection);
  }

  // Copy
  if (evt->matches(QKeySequence::Copy))
  {
    //do
    //{
    //  QClipboard * clipboard = QApplication::clipboard();

    //  if (_db == nullptr) break;

    //  if (_begSelection == _endSelection && _begSelectionOffset == _endSelectionOffset) break;

    //  int selectLineNr = _endSelection - _begSelection;
    //  int begLines     = _begSelection;
    //  int leftOffLine  = _begSelectionOffset - _addrLen - 1;
    //  int rightOffLine = _endSelectionOffset - _addrLen;

    //  if (selectLineNr == 0 && leftOffLine > rightOffLine)
    //  {
    //    leftOffLine    = _endSelectionOffset - _addrLen - 1;
    //    rightOffLine   = _begSelectionOffset - _addrLen;
    //  }

    //  else if (selectLineNr < 0)
    //  {
    //    selectLineNr   = -selectLineNr;
    //    begLines       = _endSelection;
    //    leftOffLine    = _endSelectionOffset - _addrLen - 1;
    //    rightOffLine   = _begSelectionOffset - _addrLen;
    //  }

    //  selectLineNr++;

    //  typedef medusa::View::LineInformation LineInformation;
    //  LineInformation lineInfo;

    //  QString clipboardBuf = "";
    //  for (int selectLine = 0; selectLine < selectLineNr; ++selectLine)
    //  {
    //    if (!_db->GetView().GetLineInformation(begLines + selectLine, lineInfo)) break;
    //    switch (lineInfo.GetType())
    //    {
    //    case LineInformation::EmptyLineType: clipboardBuf += "\n"; break;;

    //    case LineInformation::CellLineType:
    //      {
    //        medusa::Cell const * cell = _db->RetrieveCell(lineInfo.GetAddress());
    //        if (cell == nullptr) break;
    //        clipboardBuf += QString::fromStdString(cell->ToString());
    //        if (!cell->GetComment().empty())
    //          clipboardBuf += QString(" ; %1").arg(QString::fromStdString(cell->GetComment()));
    //        clipboardBuf += "\n";
    //        break;
    //      }

    //    case LineInformation::MultiCellLineType:
    //      {
    //        medusa::MultiCell const * multicell = _db->RetrieveMultiCell(lineInfo.GetAddress());
    //        if (multicell == nullptr) break;
    //        clipboardBuf += QString("%1\n").arg(QString::fromStdString(multicell->ToString()));
    //        break;
    //      }

    //    case LineInformation::LabelLineType:
    //      {
    //        medusa::Label lbl = _db->GetLabelFromAddress(lineInfo.GetAddress());
    //        if (lbl.GetType() == medusa::Label::LabelUnknown) break;
    //        clipboardBuf += QString("%1:\n").arg(QString::fromStdString(lbl.GetLabel()));
    //        break;
    //      }

    //    case LineInformation::XrefLineType:
    //      {
    //        medusa::Address::List RefAddrList;
    //        _db->GetXRefs().From(lineInfo.GetAddress(), RefAddrList);
    //        clipboardBuf += QString::fromUtf8(";:xref");

    //        std::for_each(std::begin(RefAddrList), std::end(RefAddrList), [&](medusa::Address const& rRefAddr)
    //        {
    //          clipboardBuf += QString(" ") + (rRefAddr < lineInfo.GetAddress() ? QString::fromUtf8("\xe2\x86\x91") : QString::fromUtf8("\xe2\x86\x93")) + QString::fromStdString(rRefAddr.ToString());
    //        });
    //        clipboardBuf += "\n";
    //        break;
    //      }

    //    case LineInformation::MemoryAreaLineType:
    //      {
    //        medusa::MemoryArea const * memArea = _db->GetMemoryArea(lineInfo.GetAddress());
    //        if (memArea == nullptr) break;
    //        clipboardBuf += QString("%1\n").arg(QString::fromStdString(memArea->ToString()));
    //        break;
    //      }

    //    default: break;
    //    }
    //  }
    //  int clipboardLen = clipboardBuf.length() - leftOffLine;

    //  int lastLineOff = clipboardBuf.lastIndexOf("\n", -2);
    //  int lastLineLen = clipboardBuf.length() - lastLineOff;
    //  clipboardLen -= (lastLineLen - rightOffLine);

    //  clipboardBuf = clipboardBuf.mid(leftOffLine, clipboardLen);
    //  if (!clipboardBuf.isEmpty())
    //    clipboard->setText(clipboardBuf);
    //} while (0);
  }
}

void DisassemblyView::resizeEvent(QResizeEvent * evt)
{
  QAbstractScrollArea::resizeEvent(evt);

  //if (_db == nullptr)
  //  return;

  //int endLine = viewport()->rect().height() / _hChar + 1;
  //int curLine = verticalScrollBar()->value();
  //typedef medusa::View::LineInformation LineInformation;
  //LineInformation lineInfo;

  //for (int line = 0; line < endLine && _db->GetView().GetLineInformation(line + curLine, lineInfo); ++line)
  //  _addrLen = qMax(static_cast<int>(lineInfo.GetAddress().ToString().length() + 1), _addrLen);
  //_needRepaint = true;
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
  if (x >= horizontalScrollBar()->maximum())
    x = horizontalScrollBar()->maximum() - 1;
  _xCursor = x;

  if (y < 0)
    y = _yCursor;
  if (y >= verticalScrollBar()->maximum())
    y = verticalScrollBar()->maximum() - 1;
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
  _needRepaint = true; // TODO: selectionChanged
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

  _needRepaint = true; // TODO: selectionChanged
}

void DisassemblyView::getSelectedAddresses(medusa::Address::List& addresses)
{
  //for (auto curSelection = _begSelection; curSelection < _endSelection; ++curSelection)
  //{
  //  medusa::View::LineInformation lineInfo;

  //  if (!_db->GetView().GetLineInformation(curSelection, lineInfo)) break;
  //  addresses.push_back(lineInfo.GetAddress());
  //}
}

void DisassemblyView::moveSelection(int x, int y)
{
  moveCursorPosition(x, y);
  _endSelectionOffset += x;
  _endSelection       += y;

  _needRepaint = true; // TODO: selectionChanged
}

void DisassemblyView::updateScrollbars(void)
{
  if (_db == nullptr)
    return;

  int max = _lineNo - (viewport()->rect().height() / _hChar);
  if (max < 0) max = 0;

  verticalScrollBar()->setMaximum(max);
  horizontalScrollBar()->setMaximum(_lineLen);
}

bool DisassemblyView::convertPositionToAddress(QPoint const & pos, medusa::Address & addr)
{
  return _scr->GetAddressFromPosition(addr, pos.x() / _wChar, pos.y() / _hChar);
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