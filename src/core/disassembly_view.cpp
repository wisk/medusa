#include "medusa/disassembly_view.hpp"

#include <algorithm>

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_USE;

DisassemblyView::DisassemblyView(Medusa& rCore, Printer* pPrinter, u32 PrinterFlags, Address::List const& rAddresses)
  : View(Document::Subscriber::DocumentUpdated, rCore.GetDocument())
  , m_rCore(rCore)
  , m_pPrinter(pPrinter), m_PrinterFlags(PrinterFlags)
  , m_Addresses(rAddresses)
  , m_Width(), m_Height()
{
  _Prepare();
}

DisassemblyView::~DisassemblyView(void)
{
  boost::mutex::scoped_lock Lock(m_Mutex);
  delete m_pPrinter;
}

void DisassemblyView::Refresh(void)
{
  boost::mutex::scoped_lock Lock(m_Mutex);

  _Prepare();
}

void DisassemblyView::Print(void)
{
  u32 LineNo;
  u32 yOffset = 0;

  boost::mutex::scoped_lock Lock(m_Mutex);

  for (auto itAddr = std::begin(m_Addresses); itAddr != std::end(m_Addresses); ++itAddr)
  {
    LineNo = (*m_pPrinter)(*itAddr, 0, yOffset, m_PrinterFlags);
    if (LineNo == 0)
      return;
    yOffset += LineNo;
  }
}

bool DisassemblyView::GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const
{
  boost::mutex::scoped_lock Lock(m_Mutex);

  if (yPos >= m_Addresses.size())
    return false;

  auto itAddr = m_Addresses.begin();
  while (yPos--)
    ++itAddr;

  rAddress = *itAddr;
  return true;
}

void DisassemblyView::_Prepare(void)
{
  m_Height = 0;
  m_Width  = 0;
  for (auto itAddr = std::begin(m_Addresses); itAddr != std::end(m_Addresses); ++itAddr)
  {
    m_Height += m_pPrinter->GetLineHeight(*itAddr, m_PrinterFlags);
    m_Width   = std::max(m_Width, static_cast<u32>(m_pPrinter->GetLineWidth(*itAddr, m_PrinterFlags)));
  }
}

void DisassemblyView::GetDimension(u32& rWidth, u32& rHeight) const
{
  rWidth  = m_Width;
  rHeight = m_Height;
}

FullDisassemblyView::FullDisassemblyView(Medusa& rCore, Printer* pPrinter, u32 PrinterFlags, u32 Width, u32 Height, Address const& rAddress)
  : View(Document::Subscriber::DocumentUpdated, rCore.GetDocument())
  , m_rCore(rCore)
  , m_pPrinter(pPrinter), m_PrinterFlags(PrinterFlags)
  , m_CursorAddress(rAddress), m_CursorOffset()
  , m_Offset(), m_Width(Width), m_Height(Height)
{
  _Prepare(rAddress);
}

FullDisassemblyView::~FullDisassemblyView(void)
{
  boost::mutex::scoped_lock Lock(m_Mutex);
  delete m_pPrinter;
}

Cell::SPtr FullDisassemblyView::GetCellFromPosition(u32 xChar, u32 yChar)
{
  Address CellAddr;

  if (GetAddressFromPosition(CellAddr, xChar, yChar) == false)
    return nullptr;

  return m_rCore.GetCell(CellAddr);
}

Cell::SPtr const FullDisassemblyView::GetCellFromPosition(u32 xChar, u32 yChar) const
{
  Address CellAddr;

  if (GetAddressFromPosition(CellAddr, xChar, yChar) == false)
    return nullptr;

  return m_rCore.GetCell(CellAddr);
}

void FullDisassemblyView::GetDimension(u32& rWidth, u32& rHeight) const
{
  rWidth  = m_Width;
  rHeight = m_Height;
}

void FullDisassemblyView::Refresh(void)
{
  boost::mutex::scoped_lock Lock(m_Mutex);

  if (m_VisiblesAddresses.empty())
    return;
  auto FirstAddr = *m_VisiblesAddresses.begin();
  _Prepare(FirstAddr);
}

void FullDisassemblyView::Resize(u32 Width, u32 Height)
{
  m_Width  = Width;
  m_Height = Height;
}

void FullDisassemblyView::Print(void)
{
  boost::mutex::scoped_lock Lock(m_Mutex);

  if (m_VisiblesAddresses.empty())
    return;

  u32 LineNo;
  u32 yOffset = 0;

  m_Width = 0;

  for (auto itAddr = std::begin(m_VisiblesAddresses); itAddr != std::end(m_VisiblesAddresses);)
  {
    LineNo = (*m_pPrinter)(*itAddr, m_Offset, yOffset, m_PrinterFlags);
    m_Width = std::max(m_Width, static_cast<u32>(m_pPrinter->GetLineWidth(*itAddr, m_PrinterFlags)));
    if (LineNo == 0)
      return;
    yOffset += LineNo;
    while (LineNo-- && itAddr != std::end(m_VisiblesAddresses))
      ++itAddr;
  }
}

bool FullDisassemblyView::Scroll(s32 xOffset, s32 yOffset)
{
  s32 NewOffset = m_Offset + xOffset;
  if (NewOffset < 0)
    m_Offset = 0;
  else
    m_Offset = NewOffset;

  if (yOffset)
  {
    boost::mutex::scoped_lock Lock(m_Mutex);

    Address NewAddress = m_VisiblesAddresses.front();
    if (m_rCore.GetDocument().MoveAddress(NewAddress, NewAddress, yOffset) == false)
      return false;

    _Prepare(NewAddress);
  }

  return true;
}

bool FullDisassemblyView::MoveCursor(s32 xOffset, s32 yOffset)
{
  {
    boost::mutex::scoped_lock Lock(m_Mutex);

    if (m_rDoc.MoveAddress(m_CursorAddress, m_CursorAddress, yOffset) == false)
      return false;
  }

  m_CursorOffset += xOffset;

  return EnsureCursorIsVisible();
}

bool FullDisassemblyView::SetCursor(u32 xOffset, u32 yOffset)
{
  if (xOffset != -1)
    m_CursorOffset = xOffset;

  if (yOffset != -1)
  {
    boost::mutex::scoped_lock Lock(m_Mutex);
    auto itAddr = std::begin(m_VisiblesAddresses);
    std::advance(itAddr, yOffset);
    if (itAddr == std::end(m_VisiblesAddresses))
      return false;

    m_CursorAddress = *itAddr;
  }

  return EnsureCursorIsVisible();
}

bool FullDisassemblyView::GoTo(Address const& rAddress)
{
  boost::mutex::scoped_lock Lock(m_Mutex);
  m_CursorAddress = rAddress;
  _Prepare(rAddress);
  return m_VisiblesAddresses.empty() ? false : true;
}

void FullDisassemblyView::_Prepare(Address const& rAddress)
{
  auto const& rDoc = m_rCore.GetDocument();
  u32 NumberOfAddress = m_Height;
  Address CurrentAddress;

  if (m_VisiblesAddresses.empty() == false)
    m_VisiblesAddresses.clear();

  if (NumberOfAddress == 0)
    return;

  if (rDoc.GetNearestAddress(rAddress, CurrentAddress) == false)
    return;

  while (NumberOfAddress--)
  {
    u32 NumberOfLine = m_pPrinter->GetLineHeight(CurrentAddress, m_PrinterFlags);

    if (NumberOfLine == 0)
      continue;

    while (NumberOfLine--)
      m_VisiblesAddresses.push_back(CurrentAddress);

    if (rDoc.GetNextAddress(CurrentAddress, CurrentAddress) == false)
      return;
  }
}

bool FullDisassemblyView::GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const
{
  boost::mutex::scoped_lock Lock(m_Mutex);
  if (yPos >= m_VisiblesAddresses.size())
    return false;

  auto itAddr = m_VisiblesAddresses.begin();
  while (yPos--)
    ++itAddr;

  rAddress = *itAddr;
  return true;
}

bool FullDisassemblyView::EnsureCursorIsVisible(void)
{
  boost::mutex::scoped_lock Lock(m_Mutex);
  if (m_CursorAddress >= m_VisiblesAddresses.front() && m_CursorAddress <= m_VisiblesAddresses.back())
    return true;

  _Prepare(m_CursorAddress);
  return m_VisiblesAddresses.empty() ? false : true;
}