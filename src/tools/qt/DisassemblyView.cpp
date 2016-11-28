#include "DisassemblyView.hpp"

#include "medusa/module.hpp"
#include "medusa/cell_action.hpp"
#include "medusa/log.hpp"
#include "medusa/user_configuration.hpp"

#include "Proxy.hpp"

DisassemblyView::DisassemblyView(QWidget * parent, medusa::Medusa * core)
  : QAbstractScrollArea(parent)
  , medusa::FullDisassemblyView(
    *core,
    medusa::FormatDisassembly::ShowAddress |
    medusa::FormatDisassembly::AddNewLineBeforeCrossReference |
    medusa::FormatDisassembly::Indent,
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
  connect(this, SIGNAL(GraphViewAdded(medusa::Address const&)),            this->parent(),   SLOT(addGraphView(medusa::Address const&)));
  connect(this, SIGNAL(ControlFlowGraphViewAdded(medusa::Address const&)), this->parent(),   SLOT(addControlFlowGraphView(medusa::Address const&)));
  connect(this, SIGNAL(viewportUpdated()),                                 this->viewport(), SLOT(update()));

  int w, h;
  QRect rect = viewport()->rect();
  w = rect.width() / _wChar + 1;
  h = rect.height() / _hChar + 1;
  Resize(w, h);
  viewUpdated();
  _UpdateActions();
}

DisassemblyView::~DisassemblyView(void)
{
}

void DisassemblyView::OnDocumentUpdated(medusa::Address::Vector ErasedCellAddresses)
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
  medusa::UserConfiguration UserCfg;
  QString fontInfo = QString::fromStdString(UserCfg.GetOption("font.listing"));
  QFont font;
  font.setStyleHint(QFont::Monospace);
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

  medusa::Address CurAddr;
  std::list<QAction *> DynActs;
  if (convertPositionToAddress(pos, CurAddr))
  {
    auto SpecActions = medusa::Action::GetSpecificActions(*_core, this, CurAddr);
    for (auto spAct : SpecActions)
    {
      auto pUiAct = new UiAction(this, spAct, QKeySequence(), this);
      addAction(pUiAct);
      DynActs.push_back(pUiAct);
    }
  }

  menu.addActions(actions());
  menu.exec(globalPos);

  for (auto pAct : DynActs)
  {
    removeAction(pAct);
    delete pAct;
  }
}

void DisassemblyView::OnUiActionTriggered(medusa::Action::SPType spAction)
{
  if (!spAction->IsCompatible())
    return;
  spAction->Do();
}

void DisassemblyView::paintBackground(QPainter& p)
{
  // Draw background
  medusa::UserConfiguration UserCfg;
  QColor AddrColor = QColor(QString::fromStdString(UserCfg.GetOption("color.background_address")));
  QColor CodeColor = QColor(QString::fromStdString(UserCfg.GetOption("color.background_listing")));
  QColor CursColor = QColor(Qt::black);

  QRect AddrRect = viewport()->rect();
  QRect CodeRect = viewport()->rect();

  AddrRect.setWidth((_addrLen - horizontalScrollBar()->value()) * _wChar);
  CodeRect.setX((_addrLen - horizontalScrollBar()->value()) * _wChar);

  p.fillRect(AddrRect, AddrColor);
  p.fillRect(CodeRect, CodeColor);
}

void DisassemblyView::paintSelection(QPainter& p)
{
  if (m_SelectionBegin == m_SelectionEnd)
    return;

  // Check if the selection is visible
  medusa::Address FirstAddr;
  if (!m_PrintData.GetFirstAddress(FirstAddr))
    return;
  if (m_SelectionEnd.m_Address < FirstAddr)
    return;
  medusa::Address LastAddr;
  if (!m_PrintData.GetLastAddress(LastAddr))
    return;
  if (m_SelectionBegin.m_Address > LastAddr)
    return;

  medusa::UserConfiguration UserCfg;
  QColor slctColor = QColor(QString::fromStdString(UserCfg.GetOption("color.selection")));

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

  medusa::UserConfiguration UserCfg;
  QColor MnClr(QString::fromStdString(UserCfg.GetOption("color.instruction_mnemonic")));
  QColor PfClr(QString::fromStdString(UserCfg.GetOption("color.instruction_mnemonic_prefix")));
  QColor SfClr(QString::fromStdString(UserCfg.GetOption("color.instruction_mnemonic_suffix")));
  QColor KwClr(QString::fromStdString(UserCfg.GetOption("color.keyword")));
  QColor ImClr(QString::fromStdString(UserCfg.GetOption("color.instruction_immediate")));
  QColor OpClr(QString::fromStdString(UserCfg.GetOption("color.operator")));
  QColor RgClr(QString::fromStdString(UserCfg.GetOption("color.instruction_register")));
  QColor LbClr(QString::fromStdString(UserCfg.GetOption("color.label")));
  QColor SzClr(QString::fromStdString(UserCfg.GetOption("color.string")));
  QColor CmClr(QString::fromStdString(UserCfg.GetOption("color.comment")));
  QColor DfClr(Qt::black);

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
      auto MarkSz = rMark.GetSize();
      if (rMark.GetType() != medusa::Mark::UnprintableType)
      {
        switch (rMark.GetType())
        {
        case medusa::Mark::MnemonicType:       MarkClr = MnClr; break;
        case medusa::Mark::MnemonicPrefixType: MarkClr = PfClr; break;
        case medusa::Mark::MnemonicSuffixType: MarkClr = SfClr; break;
        case medusa::Mark::KeywordType:        MarkClr = KwClr; break;
        case medusa::Mark::ImmediateType:      MarkClr = ImClr; break;
        case medusa::Mark::OperatorType:       MarkClr = OpClr; break;
        case medusa::Mark::RegisterType:       MarkClr = RgClr; break;
        case medusa::Mark::LabelType:          MarkClr = LbClr; break;
        case medusa::Mark::StringType:         MarkClr = SzClr; break;
        case medusa::Mark::CommentType:        MarkClr = CmClr; break;
        default:                               MarkClr = DfClr; break;
        };
        p.setPen(MarkClr);
        QString Text = QString::fromUtf8(rText.substr(TextOff, MarkSz).c_str());
        p.drawText(static_cast<int>(TextOff * _wChar), Line, Text);
      }
      TextOff += MarkSz;
    }
    Line += _hChar;
  });
}

void DisassemblyView::paintCursor(QPainter& p)
{
  medusa::UserConfiguration UserCfg;
  QColor codeColor = QColor(QString::fromStdString(UserCfg.GetOption("color.background_listing")));

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
  medusa::Address SrcAddr, DstAddr;

  if (!convertMouseToAddress(evt, SrcAddr)) return;

  auto spInsn = std::dynamic_pointer_cast<medusa::Instruction const>(m_rDoc.GetCell(SrcAddr));
  if (spInsn == nullptr)
    return;

  auto spArch = medusa::ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
  if (spArch == nullptr)
    return;

  auto Index = GetSelectionIndex();
  if (Index == 0xff)
    return;
  if (!spInsn->GetOperandReference(m_rDoc, Index, spArch->CurrentAddress(SrcAddr, *spInsn), DstAddr))
    return;
  goTo(DstAddr);
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
    auto Start = m_SelectionBegin;
    auto Last  = m_SelectionEnd;

    if (Start.m_Address > Last.m_Address)
      std::swap(Start, Last);

    medusa::PrintData Print;
    medusa::FormatDisassembly FmtDisasm(m_rCore, Print);

    FmtDisasm(std::make_pair(Start.m_Address, Last.m_Address), m_FormatFlags);
    // TODO trim it!
    //auto& rLines = Print.GetTextLines();
    //rLines.erase();
    //rLines.erase();
    QApplication::clipboard()->setText(QString::fromStdString(Print.GetTexts()));
  }

  emit viewUpdated();

  QAbstractScrollArea::keyPressEvent(evt);
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

void DisassemblyView::actionEvent(QActionEvent * evt)
{
  QAbstractScrollArea::actionEvent(evt);
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

void DisassemblyView::_UpdateActions(void)
{
  auto ActionsMap = medusa::Action::GetMap();
  AddUiActions(ActionsMap);

  medusa::UserConfiguration UserCfg;

  for (auto const& rActionPair : ActionsMap)
  {
    auto spAction = rActionPair.second(*_core, this);

    std::string Opt;
    if (!UserCfg.GetOption(rActionPair.first, Opt))
    {
      medusa::Log::Write("ui_qt") << "failed to get option for " << spAction->GetName() << medusa::LogEnd;
      continue;
    }
    QKeySequence KeySeq(QString::fromStdString(Opt), QKeySequence::PortableText);
    medusa::Log::Write("ui_qt") << "bind " << rActionPair.first << " with \"" << KeySeq.toString().toStdString() << "\"" << medusa::LogEnd;

    auto pUiAction = new UiAction(this, spAction, KeySeq, this);
    addAction(pUiAction);
  }
}
