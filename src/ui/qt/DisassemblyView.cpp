#include "DisassemblyView.hpp"

#include "medusa/cell_action.hpp"
#include "medusa/log.hpp"

#include "Proxy.hpp"

DisassemblyView::DisassemblyView(QWidget * parent, medusa::Medusa * core)
  : QAbstractScrollArea(parent)
  , medusa::FullDisassemblyView(
    *core,
    medusa::FormatDisassembly::ShowAddress | medusa::FormatDisassembly::AddSpaceBeforeXref,
    0, 0,
    core->GetDocument().GetStartAddress())
  , _needRepaint(true)
  , _core(core)
  , _wChar(0),              _hChar(0)
  , _addrLen(static_cast<int>(core->GetDocument().GetStartAddress().ToString().length() + 1))
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
  if (_core->GetDocument().GetCell(address) == nullptr)
    return false;

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
  Refresh();

  emit viewportUpdated();
  _needRepaint = true;
}

void DisassemblyView::horizontalScrollBarChanged(int n)
{
  static int old = 0;
  MoveView(n - old, 0);
  old = n;
  emit viewUpdated();
}

void DisassemblyView::listingUpdated(void)
{
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
  std::lock_guard<MutexType> Lock(m_Mutex);

  QFontMetrics fm = viewport()->fontMetrics();
  int Line = _hChar - 5; // http://doc.qt.digia.com/qt-maemo/qpainter.html#drawText-12 (Note: The y-position is used as the baseline of the font.)
  QColor MarkClr(Qt::black);
  auto SkippedLine = m_Top.m_yAddressOffset;
  m_PrintData.ForEachLine([&](medusa::Address const& rAddr, std::string const& rText, medusa::Mark::List const& rMarks)
  {
    if (SkippedLine)
    {
      --SkippedLine;
      return;
    }
    std::string::size_type TextOff = 0;
    for (auto const& rMark : rMarks)
    {
      auto MarkLen = rMark.GetLength();
      if (rMark.GetType() != medusa::Mark::UnprintableType)
      {
        switch (rMark.GetType())
        {
        case medusa::Mark::MnemonicType:  MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_MNEMONIC,  MEDUSA_COLOR_INSTRUCTION_MNEMONIC_DEFAULT).toString());  break;
        case medusa::Mark::KeywordType:   MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_KEYWORD,   MEDUSA_COLOR_INSTRUCTION_KEYWORD_DEFAULT).toString());   break;
        case medusa::Mark::ImmediateType: MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_IMMEDIATE, MEDUSA_COLOR_INSTRUCTION_IMMEDIATE_DEFAULT).toString()); break;
        case medusa::Mark::OperatorType:  MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_OPERATOR,  MEDUSA_COLOR_INSTRUCTION_OPERATOR_DEFAULT).toString());  break;
        case medusa::Mark::RegisterType:  MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_REGISTER,  MEDUSA_COLOR_INSTRUCTION_REGISTER_DEFAULT).toString());  break;
        case medusa::Mark::LabelType:     MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_LABEL,     MEDUSA_COLOR_INSTRUCTION_LABEL_DEFAULT).toString());     break;
        case medusa::Mark::StringType:    MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_STRING,    MEDUSA_COLOR_INSTRUCTION_STRING_DEFAULT).toString());    break;
        case medusa::Mark::CommentType:   MarkClr = QColor(Settings::instance().value(MEDUSA_COLOR_INSTRUCTION_COMMENT,   MEDUSA_COLOR_INSTRUCTION_COMMENT_DEFAULT).toString());   break;
        default:                          MarkClr = QColor(Qt::black); break;
        };
        p.setPen(MarkClr);
        QString Text = QString::fromUtf8(rText.substr(TextOff, MarkLen).c_str());
        p.drawText(static_cast<int>(TextOff * _wChar), Line, Text);
      }
      TextOff += MarkLen;
    }
    Line += _hChar;
  });
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
  if (evt->buttons() & Qt::LeftButton)
  {
    medusa::u32 x = evt->x() / _wChar;
    medusa::u32 y = evt->y() / _hChar;
    EndSelection(x, y);
    _needRepaint = true; // TODO: selectionChanged
    emit viewUpdated();
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
  bool InvView = false;

  // Move cursor
  if (evt->matches(QKeySequence::MoveToNextChar))
  { MoveCursor(+1, 0, InvView); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousChar))
  { MoveCursor(-1, 0, InvView); ResetSelection(); }

  if (evt->matches(QKeySequence::MoveToStartOfLine))
  { SetCursor(_addrLen, -1); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToEndOfLine))
  {
    medusa::LineData Line;
    if (m_PrintData.GetLine(m_Cursor.m_Address, m_Cursor.m_yAddressOffset, Line))
    {
      auto TextLen = static_cast<medusa::u32>(Line.GetText().length());
      if (TextLen != 0)
        --TextLen;
      SetCursor(TextLen, -1);
    }
  }

  if (evt->matches(QKeySequence::MoveToNextLine))
  { moveCursorPosition(0, +1); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousLine))
  { moveCursorPosition(0, -1); ResetSelection(); }

  if (evt->matches(QKeySequence::MoveToNextPage))
  { MoveCursor(0, +m_Height, InvView); ResetSelection(); }
  if (evt->matches(QKeySequence::MoveToPreviousPage))
  { MoveCursor(0, -static_cast<medusa::s32>(m_Height), InvView); ResetSelection(); }

  if (evt->matches(QKeySequence::MoveToStartOfDocument))
  {
    medusa::Address FirstAddr = m_rDoc.GetFirstAddress();
    m_Cursor.m_Address = FirstAddr;
    m_Cursor.m_yAddressOffset = 0;
    m_Top = m_Cursor;
    m_Format(FirstAddr, m_FormatFlags, m_Height);
    ResetSelection();
  }
  if (evt->matches(QKeySequence::MoveToEndOfDocument))
  {
    medusa::Address LastAddr = m_rDoc.GetLastAddress();
    m_Cursor.m_Address = LastAddr;
    m_Cursor.m_yAddressOffset = 0;
    m_Top = m_Cursor;
    m_Format(LastAddr, m_FormatFlags, m_Height);
    ResetSelection();
  }

  if (evt->matches(QKeySequence::MoveToNextWord))
  {
    medusa::LineData Line;
    if (m_PrintData.GetLine(m_Cursor.m_Address, m_Cursor.m_yAddressOffset, Line))
    {
      auto const& rText = Line.GetText();
      auto Pos = rText.find_first_not_of(" \n", m_Cursor.m_xAddressOffset);
      Pos = rText.find_first_of(" \n", Pos);
      if (Pos != std::string::npos)
        SetCursor(static_cast<medusa::u32>(Pos), -1);
      else
        SetCursor(static_cast<medusa::u32>(rText.length()), -1);
    }
  }

  if (evt->matches(QKeySequence::MoveToPreviousWord))
  {
    medusa::LineData Line;
    if (m_PrintData.GetLine(m_Cursor.m_Address, m_Cursor.m_yAddressOffset, Line))
    {
      auto const& rText = Line.GetText();
      auto Pos = rText.find_last_not_of(" \n", m_Cursor.m_xAddressOffset ? m_Cursor.m_xAddressOffset - 1 : 0);
      Pos = rText.find_last_of(" \n", Pos);
      if (Pos != std::string::npos)
        SetCursor(static_cast<medusa::u32>(Pos) + 1, -1);
      else
        SetCursor(0, -1);
    }
  }

  // Move selection
  if (evt->matches(QKeySequence::SelectNextChar))
    MoveSelection(+1, 0, InvView);
  if (evt->matches(QKeySequence::SelectPreviousChar))
    MoveSelection(-1, 0, InvView);

  if (evt->matches(QKeySequence::SelectStartOfLine))
    SetSelection(_addrLen, -1);
  if (evt->matches(QKeySequence::SelectEndOfLine))
  {
    medusa::LineData Line;
    if (m_PrintData.GetLine(m_Cursor.m_Address, m_Cursor.m_yAddressOffset, Line))
    {
      auto TextLen = static_cast<medusa::u32>(Line.GetText().length());
      if (TextLen != 0)
        --TextLen;
      SetSelection(TextLen, -1);
    }
  }

  if (evt->matches(QKeySequence::SelectNextLine))
    MoveSelection(0, +1, InvView);
  if (evt->matches(QKeySequence::SelectPreviousLine))
    MoveSelection(0, -1, InvView);

  if (evt->matches(QKeySequence::SelectNextPage))
    MoveSelection(0, +m_Height, InvView);
  if (evt->matches(QKeySequence::SelectPreviousPage))
    MoveSelection(0, -static_cast<medusa::s32>(m_Height), InvView);

  if (evt->matches(QKeySequence::SelectStartOfDocument))
  {
    m_Cursor.m_Address        = m_rDoc.GetFirstAddress();
    m_Cursor.m_xAddressOffset = 0;
    m_Cursor.m_yAddressOffset = 0;
    m_Format(m_Cursor.m_Address, m_FormatFlags, m_Height);
    m_SelectionEnd = m_Top = m_Cursor;
  }
  if (evt->matches(QKeySequence::SelectEndOfDocument))
  {
    m_Cursor.m_Address        = m_rDoc.GetLastAddress();
    m_Cursor.m_xAddressOffset = 0;
    m_Cursor.m_yAddressOffset = 0;
    m_Format(m_Cursor.m_Address, m_FormatFlags, m_Height);
    m_SelectionEnd = m_Top = m_Cursor;
  }

  if (evt->matches(QKeySequence::SelectNextWord))
  {
    medusa::LineData Line;
    if (m_PrintData.GetLine(m_Cursor.m_Address, m_Cursor.m_yAddressOffset, Line))
    {
      auto const& rText = Line.GetText();
      auto Pos = rText.find_first_not_of(" \n", m_Cursor.m_xAddressOffset);
      Pos = rText.find_first_of(" \n", Pos);
      if (Pos != std::string::npos)
        SetSelection(static_cast<medusa::u32>(Pos), -1);
      else
        SetSelection(static_cast<medusa::u32>(rText.length()), -1);
    }
  }

  if (evt->matches(QKeySequence::SelectPreviousWord))
  {
    medusa::LineData Line;
    if (m_PrintData.GetLine(m_Cursor.m_Address, m_Cursor.m_yAddressOffset, Line))
    {
      auto const& rText = Line.GetText();
      auto Pos = rText.find_last_not_of(" \n", m_Cursor.m_xAddressOffset ? m_Cursor.m_xAddressOffset - 1 : 0);
      Pos = rText.find_last_of(" \n", Pos);
      if (Pos != std::string::npos)
        SetSelection(static_cast<medusa::u32>(Pos) + 1, -1);
      else
        SetSelection(0, -1);
    }
  }

  if (evt->matches(QKeySequence::SelectAll))
  {
    medusa::Address FirstAddr = m_rDoc.GetFirstAddress();
    medusa::Address LastAddr  = m_rDoc.GetLastAddress();

    m_SelectionBegin.m_Address = FirstAddr;
    m_SelectionBegin.m_xAddressOffset = 0;
    m_SelectionBegin.m_yAddressOffset = 0;
    m_SelectionEnd.m_Address = LastAddr;
    m_SelectionEnd.m_xAddressOffset = 1; // TODO
    m_SelectionEnd.m_yAddressOffset = 1; // TODO
  }

  // Copy
  if (evt->matches(QKeySequence::Copy))
  {
    medusa::PrintData Print;
    medusa::FormatDisassembly FmtDisasm(m_rCore, Print);

    auto Start = m_SelectionBegin;
    auto Last  = m_SelectionEnd;

    if (Start.m_Address > Last.m_Address)
      std::swap(Start, Last);

    FmtDisasm(std::make_pair(Start.m_Address, Last.m_Address), m_FormatFlags);
    // TODO trim it!
    //auto& rLines = Print.GetTextLines();
    //rLines.erase();
    //rLines.erase();
    QApplication::clipboard()->setText(QString::fromStdString(Print.GetTexts()));
  }

  emit viewUpdated();
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
    if (MoveView(0, -numSteps))
      emit viewUpdated();
    evt->accept();
    return;
  }

  QAbstractScrollArea::wheelEvent(evt);
}

void DisassemblyView::setCursorPosition(QMouseEvent * evt)
{
  if (!(evt->button() & (Qt::LeftButton | Qt::RightButton)))
    return;

  int x = evt->pos().x() / _wChar;
  int y = evt->pos().y() / _hChar;

  setCursorPosition(x, y);
}

void DisassemblyView::setCursorPosition(int x, int y)
{
  if (!SetCursor(x, y))
    return;

  _cursorTimer.start();
  _cursorBlink = false;
  updateCursor();

  emit cursorAddressUpdated(GetCursorAddress());
}

void DisassemblyView::moveCursorPosition(int x, int y)
{
  bool InvView;
  if (!MoveCursor(x, y, InvView))
    return;

  _cursorTimer.start();
  _cursorBlink = false;
  updateCursor();

  emit cursorAddressUpdated(GetCursorAddress());
  if (InvView)
    emit viewUpdated();
}

bool DisassemblyView::convertPositionToAddress(QPoint const & pos, medusa::Address & addr)
{
  return GetAddressFromPosition(addr, pos.x() / _wChar, pos.y() / _hChar);
}

bool DisassemblyView::convertMouseToAddress(QMouseEvent * evt, medusa::Address & addr)
{
  return convertPositionToAddress(evt->pos(), addr);
}
