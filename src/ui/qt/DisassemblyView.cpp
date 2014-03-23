#include "DisassemblyView.hpp"

#include "medusa/cell_action.hpp"
#include "medusa/log.hpp"

#include "Proxy.hpp"

DisassemblyView::DisassemblyView(QWidget * parent, medusa::Medusa * core)
  : QAbstractScrollArea(parent)
  , medusa::FullDisassemblyView(
    *core,
    new DisassemblyPrinter(*core),
    medusa::Printer::ShowAddress | medusa::Printer::AddSpaceBeforeXref,
    0, 0,
    core->GetDocument().GetStartAddress())
  , _needRepaint(true)
  , _core(core)
  , _xOffset(0),            _yOffset(10)
  , _wChar(0),              _hChar(0)
  , _xCursor(0)
  , _addrLen(static_cast<int>(core->GetDocument().GetStartAddress().ToString().length() + 1))
  , _lineNo(core->GetDocument().GetNumberOfAddress()), _lineLen(0x100)
  , _cursorTimer(),         _cursorBlink(false)
  , _cache()
{
  setFont(); // this method initializes both _wChar and _hChar
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  connect(&_cursorTimer, SIGNAL(timeout()), this, SLOT(updateCursor()));
  _cursorTimer.setInterval(400);
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(QPoint const &)), this, SLOT(showContextMenu(QPoint const &)));
  connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(horizontalScrollBarChanged(int)));

  connect(this, SIGNAL(DisassemblyViewAdded(medusa::Address const&)),      this->parent(),   SLOT(addDisassemblyView(medusa::Address const&)));
  connect(this, SIGNAL(SemanticViewAdded(medusa::Address const&)),         this->parent(),   SLOT(addSemanticView(medusa::Address const&)));
  connect(this, SIGNAL(ControlFlowGraphViewAdded(medusa::Address const&)), this->parent(),   SLOT(addControlFlowGraphView(medusa::Address const&)));
  connect(this, SIGNAL(viewportUpdated()),                                 this->viewport(), SLOT(update()));

  int w, h;
  QRect rect = viewport()->rect();
  w = rect.width() / _wChar + 1;
  h = rect.height() / _hChar + 1;
  Resize(w, h);
  viewUpdated();
}

DisassemblyView::~DisassemblyView(void)
{
}

void DisassemblyView::OnDocumentUpdated(void)
{
  emit viewUpdated();
}

bool DisassemblyView::goTo(medusa::Address const& address)
{
  //if (_core->GetDocument().IsPresent(address) == false)
  //  return false;

  GoTo(address);
  viewUpdated();

  return true;
}

void DisassemblyView::setFont(void)
{
  QString fontInfo = Settings::instance().value(MEDUSA_FONT_TEXT, MEDUSA_FONT_TEXT_DEFAULT).toString();
  QFont font;
  font.fromString(fontInfo);
  QAbstractScrollArea::setFont(font);

  _wChar = fontMetrics().width('M');
  _hChar = fontMetrics().height();

  viewUpdated();
}

void DisassemblyView::viewUpdated(void)
{
  _lineNo = static_cast<int>(_core->GetDocument().GetNumberOfAddress());

  Refresh();

  emit viewportUpdated();
  _needRepaint = true;
}

void DisassemblyView::horizontalScrollBarChanged(int n)
{
  static int old = 0;
  Scroll(n - old, 0);
  old = n;
  emit viewUpdated();
}

void DisassemblyView::listingUpdated(void)
{
  _lineNo = static_cast<int>(_core->GetDocument().GetNumberOfAddress());

  // OPTIMIZEME: this part of code is too time consumming
  // we should find a way to only update when it's necessary
  Refresh();

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
  QMenu menu;
  QPoint globalPos = viewport()->mapToGlobal(pos);

  // TODO update this...
  medusa::Address::List selectedAddresses;
  selectedAddresses.push_back(m_Cursor.m_Address);

  medusa::CellAction::PtrList actions;
  medusa::CellAction::GetCellActionBuilders(m_rCore, m_Cursor.m_Address, actions);
  QHash<QAction*, medusa::CellAction*> actToCellAct;

  for (auto curAddress = std::begin(selectedAddresses); curAddress != std::end(selectedAddresses); ++curAddress)
  {
    medusa::Address selectedAddress = *curAddress;

    auto cell = _core->GetCell(selectedAddress);
    if (cell == nullptr) continue;

    if (actions.size() == 0) return;

    for (auto act = std::begin(actions); act != std::end(actions); ++act)
    {
      //if ((*act)->IsCompatible(*cell) == false)
      //  continue;

      if (actToCellAct.values().contains(*act))
        continue;

      auto curAct = new QAction(QString::fromStdString((*act)->GetName()), this);
      curAct->setStatusTip(QString::fromStdString((*act)->GetDescription()));
      curAct->setIcon(QIcon(QString(":/icons/%1").arg((*act)->GetIconName().c_str())));
      menu.addAction(curAct);
      actToCellAct[curAct] = *act;
    }
  }

  menu.addSeparator();

  AddDisassemblyViewAction      addDisasmViewAct;
  AddSemanticViewAction         addSemanticViewAct;
  AddControlFlowGraphViewAction addCfgViewAct;

  medusa::CellAction* qtAction[] = { &addDisasmViewAct, &addSemanticViewAct, &addCfgViewAct };
  std::for_each(std::begin(qtAction), std::end(qtAction), [&menu, &actToCellAct, this](medusa::CellAction* cellAct)
  {
    auto curAct = new QAction(QString::fromStdString(cellAct->GetName()), this);
    curAct->setStatusTip(QString::fromStdString(cellAct->GetDescription()));
    curAct->setIcon(QIcon(QString(":/icons/%1").arg(cellAct->GetIconName().c_str())));
    menu.addAction(curAct);
    actToCellAct[curAct] = cellAct;
  });

  menu.addSeparator();

  ShowCommentDialog ShwCmtDlg;
  auto pShwCmtDlgAct = new QAction(QString::fromStdString(ShwCmtDlg.GetName()), this);
  pShwCmtDlgAct->setStatusTip(QString::fromStdString(ShwCmtDlg.GetDescription()));
  pShwCmtDlgAct->setIcon(QIcon(":/icons/comment.png"));
  menu.addAction(pShwCmtDlgAct);
  actToCellAct[pShwCmtDlgAct] = &ShwCmtDlg;

  ShowLabelDialog ShwLblDlg;
  auto pShwLblDlgAct = new QAction(QString::fromStdString(ShwLblDlg.GetName()), this);
  pShwLblDlgAct->setStatusTip(QString::fromStdString(ShwLblDlg.GetDescription()));
  pShwLblDlgAct->setIcon(QIcon(":/icons/label.png"));
  menu.addAction(pShwLblDlgAct);
  actToCellAct[pShwLblDlgAct] = &ShwLblDlg;

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
  QColor cursColor = QColor(Qt::black);

  QRect addrRect = viewport()->rect();
  QRect codeRect = viewport()->rect();

  addrRect.setWidth((_addrLen - horizontalScrollBar()->value()) * _wChar);
  codeRect.setX((_addrLen - horizontalScrollBar()->value()) * _wChar);

  p.fillRect(addrRect, addrColor);
  p.fillRect(codeRect, codeColor);
}

void DisassemblyView::paintSelection(QPainter& p)
{
  if (m_SelectionBegin == m_SelectionEnd)
    return;

  QColor slctColor = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_SELECTION, MEDUSA_COLOR_INSTRUCTION_SELECTION_DEFAULT).toString());

  medusa::u32 xSelectBeg, ySelectBeg, xSelectEnd, ySelectEnd;

  if (!_ConvertAddressOffsetToViewOffset(m_SelectionBegin, xSelectBeg, ySelectBeg))
  {
    xSelectBeg = _addrLen;
    ySelectBeg = 0;
  }

  if (static_cast<int>(xSelectBeg) < _addrLen)
    xSelectBeg = _addrLen;

  if (!_ConvertAddressOffsetToViewOffset(m_SelectionEnd, xSelectEnd, ySelectEnd))
    GetDimension(xSelectEnd, ySelectEnd);

  if (static_cast<int>(xSelectEnd) < _addrLen)
    xSelectEnd = _addrLen;

  int begSelect    = ySelectBeg;
  int endSelect    = ySelectEnd;
  int begSelectOff = xSelectBeg;
  int endSelectOff = xSelectEnd;
  int deltaSelect  = endSelect    - begSelect;
  int deltaOffset  = endSelectOff - begSelectOff;

  if (deltaSelect == 0 && deltaOffset == 0)
    return;

  // If the user select from the bottom to the top, we have to swap up and down
  if (deltaSelect < 0)
  {
    deltaSelect  = -deltaSelect;
    std::swap(begSelect, endSelect);
    std::swap(begSelectOff, endSelectOff);
  }

  if (deltaSelect)
    deltaSelect++;

  if (deltaSelect == 0)
  {
    if (deltaOffset < 0)
    {
      deltaOffset = -deltaOffset;
      std::swap(begSelectOff, endSelectOff);
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

  // TODO: find another way to use this method
  static_cast<DisassemblyPrinter*>(m_pPrinter)->SetPainter(&p);
  Print();
  medusa::u32 width, height;
  GetDimension(width, height);
  horizontalScrollBar()->setMaximum(static_cast<int>(width + _addrLen));
  static_cast<DisassemblyPrinter*>(m_pPrinter)->SetPainter(nullptr);
  return;
}

void DisassemblyView::paintCursor(QPainter& p)
{
  QColor codeColor = QColor(Settings::instance().value(MEDUSA_COLOR_VIEW_BACKGROUND, MEDUSA_COLOR_VIEW_BACKGROUND_DEFAULT).toString());

  // Draw cursor
  if (_cursorBlink)
  {
    medusa::u32 x, y;

    if (!_ConvertAddressOffsetToViewOffset(m_Cursor, x, y))
      return;

    QColor cursorColor = ~codeColor.value();
    QRect cursorRect(x * _wChar, y * _hChar, 2, _hChar);
    p.fillRect(cursorRect, cursorColor);
  }
}

void DisassemblyView::paintEvent(QPaintEvent * evt)
{
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
}

void DisassemblyView::mouseMoveEvent(QMouseEvent * evt)
{
  setCursorPosition(evt);

  if (evt->buttons() & Qt::LeftButton)
  {
    medusa::u32 x = (evt->x() + horizontalScrollBar()->value()) / _wChar;
    medusa::u32 y = (evt->y() + verticalScrollBar()->value()) / _hChar;
    EndSelection(x, y);
    _needRepaint = true; // TODO: selectionChanged
  }
}

void DisassemblyView::mousePressEvent(QMouseEvent * evt)
{
  setCursorPosition(evt);

  // FIXME
  if (evt->buttons() & Qt::LeftButton)
  {
    ResetSelection();
    _needRepaint = true; // TODO: selectionChanged
  }
}

void DisassemblyView::mouseDoubleClickEvent(QMouseEvent * evt)
{
  auto const& doc = _core->GetDocument();
  medusa::Address srcAddr, dstAddr;

  if (!convertMouseToAddress(evt, srcAddr)) return;

  auto insn = std::dynamic_pointer_cast<medusa::Instruction const>(doc.GetCell(srcAddr));
  if (insn == nullptr)
    return;

  auto memArea = doc.GetMemoryArea(srcAddr);

  for (medusa::u8 op = 0; op < 4; ++op)
  {
    if ( memArea != nullptr
      && insn->GetOperandReference(doc, op, srcAddr, dstAddr))
      if (goTo(dstAddr))
        return;
  }
}

void DisassemblyView::keyPressEvent(QKeyEvent * evt)
{
  // Move cursor
  if (evt->matches(QKeySequence::MoveToNextChar))
  { moveCursorPosition(+1, 0); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousChar))
  { moveCursorPosition(-1, 0); ResetSelection(); }

  if (evt->matches(QKeySequence::MoveToStartOfLine))
  { setCursorPosition(_addrLen, -1); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToEndOfLine))
  {

  }

  if (evt->matches(QKeySequence::MoveToNextLine))
  { moveCursorPosition(0, +1); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousLine))
  { moveCursorPosition(0, -1); ResetSelection(); }

  if (evt->matches(QKeySequence::MoveToNextPage))
  { moveCursorPosition(0, static_cast<int>(m_VisiblesAddresses.size())); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousPage))
  { moveCursorPosition(0, -static_cast<int>(m_VisiblesAddresses.size())); ResetSelection(); }

  if (evt->matches(QKeySequence::MoveToStartOfDocument))
  { setCursorPosition(_addrLen, 0); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToEndOfDocument))
  { setCursorPosition(_addrLen, horizontalScrollBar()->maximum() - 1); ResetSelection(); }

  if (evt->matches(QKeySequence::MoveToNextWord))
  {

  }

  if (evt->matches(QKeySequence::MoveToPreviousWord))
  {

  }

  // Move selection
  //if (evt->matches(QKeySequence::SelectNextChar))
  //  moveSelection(+1, 0);
  //if (evt->matches(QKeySequence::SelectPreviousChar))
  //  moveSelection(-1, 0);

  //if (evt->matches(QKeySequence::SelectStartOfLine))
  //  setSelection(_addrLen, -1);
  //if (evt->matches(QKeySequence::SelectEndOfLine))
  //{
  //  // FIXME
  //  //do
  //  //{
  //  //  int line = _yCursor - horizontalScrollBar()->value();
  //  //  if (line >= static_cast<int>(_visibleLines.size())) break;
  //  //  QString curLine = _visibleLines.at(static_cast<std::vector<QString>::size_type>(line));

  //  //  setSelection(_addrLen + 1 + curLine.length(), -1);
  //  //} while (0);
  //}

  //if (evt->matches(QKeySequence::SelectNextLine))
  //  moveSelection(0, +1);
  //if (evt->matches(QKeySequence::SelectPreviousLine))
  //  moveSelection(0, -1);

  //if (evt->matches(QKeySequence::SelectNextPage))
  //  moveSelection(0, +viewport()->rect().height());
  //if (evt->matches(QKeySequence::SelectPreviousPage))
  //  moveSelection(0, -viewport()->rect().height());

  //if (evt->matches(QKeySequence::SelectStartOfDocument))
  //  setSelection(-1, 0);
  //if (evt->matches(QKeySequence::SelectEndOfDocument))
  //  setSelection(-1, horizontalScrollBar()->maximum());

  if (evt->matches(QKeySequence::SelectNextWord))
  {

  }

  if (evt->matches(QKeySequence::SelectPreviousWord))
  {

  }

  if (evt->matches(QKeySequence::SelectAll))
  {
    // UNHANDLED
  }

  // Copy
  if (evt->matches(QKeySequence::Copy))
  {
  }
}

void DisassemblyView::resizeEvent(QResizeEvent *event)
{
  QAbstractScrollArea::resizeEvent(event);
  int w, h;
  QRect rect = viewport()->rect();
  w = rect.width() / _wChar + 1;
  h = rect.height() / _hChar + 1;
  Resize(w, h);
  emit viewUpdated();
}

void DisassemblyView::wheelEvent(QWheelEvent * evt)
{
  int numSteps = evt->delta() / 20;

  if (evt->orientation() == Qt::Vertical)
  {
    if (Scroll(0, -numSteps))
      emit viewUpdated();
    evt->accept();
    return;
  }

  QAbstractScrollArea::wheelEvent(evt);
}

void DisassemblyView::setCursorPosition(QMouseEvent * evt)
{
  int x = evt->pos().x() / _wChar;
  int y = evt->pos().y() / _hChar;

  if (!SetCursor(x, y))
    return;

  _cursorTimer.start();
  _cursorBlink = false;
  updateCursor();
  ensureCursorIsVisible();

  emit cursorAddressUpdated(GetCursorAddress());
}

void DisassemblyView::setCursorPosition(int x, int y)
{
  _xCursor += x;
  if (_xCursor < 0)
    _xCursor = 0;
  if (_xCursor >= horizontalScrollBar()->maximum())
    _xCursor = horizontalScrollBar()->maximum() - 1;

  MoveCursor(x, y);

  _cursorTimer.start();
  _cursorBlink = false;
  updateCursor();
  ensureCursorIsVisible();

  emit cursorAddressUpdated(GetCursorAddress());
}

// TODO rename to scroll instead of move
void DisassemblyView::moveCursorPosition(int x, int y)
{
  setCursorPosition(x, y); // TODO: y is an offset but x is absolute
}

bool DisassemblyView::convertPositionToAddress(QPoint const & pos, medusa::Address & addr)
{
  return GetAddressFromPosition(addr, pos.x() / _wChar, pos.y() / _hChar);
}

bool DisassemblyView::convertMouseToAddress(QMouseEvent * evt, medusa::Address & addr)
{
  return convertPositionToAddress(evt->pos(), addr);
}

void DisassemblyView::ensureCursorIsVisible(void)
{
}
