#include "medusa/disassembly_view.hpp"

#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_USE;

void FormatDisassembly::operator()(Address const& rAddress, u32 Flags, u16 LinesNo)
{
  m_rPrintData.Clear();
  m_rPrintData.PrependAddress(Flags & ShowAddress ? true : false);

  auto& rDoc = m_rCore.GetDocument();
  Address CurAddr = rAddress;

  if (rDoc.GetStartAddress() == rAddress)
  {
    m_rPrintData(CurAddr);
    FormatHeader(CurAddr, Flags);
  }

  for (u16 i = 0; i < LinesNo; ++i)
  {
    m_rPrintData(CurAddr);

    // MemoryArea
    auto pMemArea = rDoc.GetMemoryArea(CurAddr);
    if (pMemArea != nullptr && pMemArea->GetBaseAddress() == CurAddr)
    {
      FormatMemoryArea(CurAddr, Flags);
      m_rPrintData.AppendNewLine();
    }

    // XRefs
    if (rDoc.HasCrossReferenceFrom(CurAddr))
    {
      if (Flags & AddSpaceBeforeXref)
        m_rPrintData.AppendNewLine();

      FormatXref(CurAddr, Flags);
      m_rPrintData.AppendNewLine();
    }

    // Label
    auto rLbl = rDoc.GetLabelFromAddress(CurAddr);
    if (rLbl.GetType() != Label::Unknown)
    {
      FormatLabel(CurAddr, Flags);
      m_rPrintData.AppendNewLine();
    }

    // Multicell
    if (rDoc.GetMultiCell(CurAddr) != nullptr)
    {
      FormatMultiCell(CurAddr, Flags);
      m_rPrintData.AppendNewLine();
    }

    if (rDoc.GetCell(CurAddr) != nullptr)
    {
      FormatCell(CurAddr, Flags);
      m_rPrintData.AppendNewLine();
    }

    if (!rDoc.GetNextAddress(CurAddr, CurAddr))
      return;
  }
}

void FormatDisassembly::FormatHeader(Address const& rAddress, u32 Flags)
{
  m_rPrintData.AppendComment(";; File disassembled with ").AppendComment(Medusa::GetVersion()).AppendNewLine();
  m_rPrintData.AppendComment(";; website: https://github.com/wisk/medusa ").AppendNewLine();
  // TODO: filename and hash
  m_rPrintData.AppendNewLine().AppendNewLine();
}

void FormatDisassembly::FormatAddress(Address const& rAddress, u32 Flags)
{
  if (rAddress.GetBase() != 0x0 && rAddress.GetBaseSize() != 0x0)
    m_rPrintData.AppendImmediate(rAddress.GetBase(), rAddress.GetBaseSize()).AppendOperator(":");
  m_rPrintData.AppendImmediate(rAddress.GetOffset(), rAddress.GetOffsetSize()).AppendSpace(8);
}

void FormatDisassembly::FormatCell(Address const& rAddress, u32 Flags)
{
  m_rPrintData.AppendSpace(4);
  auto pCell = m_rCore.GetCell(rAddress);
  if (pCell == nullptr)
    m_rPrintData.AppendComment(";; invalid cell!");
  else
    m_rCore.FormatCell(rAddress, *pCell, m_rPrintData);

  std::string Cmt;
  u16 CurTextWidth = static_cast<u16>(m_rPrintData.GetCurrentText().length());
  if (m_rCore.GetDocument().GetComment(rAddress, Cmt))
  {
    std::vector<std::string> CmtLines;
    boost::split(CmtLines, Cmt, boost::is_any_of("\n"));
    if (CmtLines.size() == 1)
      m_rPrintData.AppendSpace(std::max(20, CurTextWidth + 1)).AppendComment("; ").AppendComment(Cmt);
    else for (auto const& CmtLine : CmtLines)
      m_rPrintData.AppendNewLine().AppendSpace(CurTextWidth).AppendComment("; ").AppendComment(CmtLine);
  }
}

void FormatDisassembly::FormatMultiCell(Address const& rAddress, u32 Flags)
{
  m_rPrintData.AppendSpace(2);
  auto pMultiCell = m_rCore.GetMultiCell(rAddress);
  if (pMultiCell == nullptr)
    m_rPrintData.AppendComment(";; invalid multicell!");
  else
    m_rCore.FormatMultiCell(rAddress, *pMultiCell, m_rPrintData);
}

void FormatDisassembly::FormatLabel(Address const& rAddress, u32 Flags)
{
  auto Lbl = m_rCore.GetDocument().GetLabelFromAddress(rAddress);
  if (Lbl.GetType() == Label::Unknown)
    m_rPrintData.AppendComment(";; invalid label!");
  else
    m_rPrintData.AppendLabel(Lbl.GetLabel()).AppendOperator(":");
}

void FormatDisassembly::FormatXref(Address const& rAddress, u32 Flags)
{
  Address::List AddrFrom;
  std::list<std::string> AddrFromStr;
  m_rCore.GetDocument().GetCrossReferenceFrom(rAddress, AddrFrom);
  for (auto const& rAddr : AddrFrom)
    AddrFromStr.push_back(rAddr.ToString());
  std::string Buffer = "; xref: " + boost::algorithm::join(AddrFromStr, ", ");
  m_rPrintData.AppendComment(Buffer);
}

void FormatDisassembly::FormatMemoryArea(Address const& rAddress, u32 Flags)
{
  m_rPrintData.AppendNewLine();
  auto pMemArea = m_rCore.GetDocument().GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    m_rPrintData.AppendComment(";; invalid memory area!");
  else
    m_rPrintData.AppendComment(pMemArea->ToString());
}

DisassemblyView::DisassemblyView(Medusa& rCore, u32 FormatFlags, Address const& rAddress)
  : View(Document::Subscriber::DocumentUpdated, rCore.GetDocument())
  , m_rCore(rCore)
  , m_FormatFlags(FormatFlags)
  , m_Format(rCore, m_PrintData)
{
}

DisassemblyView::~DisassemblyView(void)
{

}

bool DisassemblyView::GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const
{
  std::lock_guard<MutexType> Lock(m_Mutex);

  u16 Off;
  LineData Line;
  if (!m_PrintData.GetLine(yPos, Off, Line))
    return false;

  rAddress = Line.GetAddress();
  return true;
}

void DisassemblyView::GetDimension(u32& rWidth, u32& rHeight) const
{
  rWidth  = m_PrintData.GetWidth();
  rHeight = m_PrintData.GetHeight();
}

FullDisassemblyView::FullDisassemblyView(Medusa& rCore, u32 FormatFlags, u32 Width, u32 Height, Address const& rAddress)
  : View(Document::Subscriber::DocumentUpdated, rCore.GetDocument())
  , m_rCore(rCore)
  , m_FormatFlags(FormatFlags)
  , m_Format(rCore, m_PrintData)
  , m_Top(rAddress)
  , m_Cursor(rAddress)
  , m_SelectionBegin(), m_SelectionEnd()
  , m_Width(Width), m_Height(Height)
{
}

FullDisassemblyView::~FullDisassemblyView(void)
{
}

Cell::SPtr FullDisassemblyView::GetCellFromPosition(u32 xChar, u32 yChar)
{
  return false;
}

Cell::SPtr const FullDisassemblyView::GetCellFromPosition(u32 xChar, u32 yChar) const
{
  return nullptr;
}

void FullDisassemblyView::GetDimension(u32& rWidth, u32& rHeight) const
{
  rWidth = 0; rHeight = 0;
}

void FullDisassemblyView::Resize(u32 Width, u32 Height)
{
  m_Width  = Width;
  m_Height = Height;
  Refresh();
}

void FullDisassemblyView::Refresh(void)
{
  m_Format(m_Top.m_Address, m_FormatFlags, m_Height + m_Top.m_yAddressOffset);
}

bool FullDisassemblyView::MoveView(s32 xOffset, s32 yOffset)
{
  // We start to update the x offset
  s32 xNewOffset = m_Top.m_xAddressOffset + xOffset;
  m_Top.m_xAddressOffset = (xNewOffset < 0) ? 0 : xNewOffset;

  if (yOffset == 0)
    return true;

  // Is this case, we don't need to find a new address
  u16 CurLineNo = m_PrintData.GetLineNo(m_Top.m_Address);
  s32 yNewOffset = static_cast<s32>(m_Top.m_yAddressOffset) + yOffset;
  if (yNewOffset >= 0 && yNewOffset < static_cast<s32>(CurLineNo))
  {
    m_Top.m_yAddressOffset = static_cast<u16>(yNewOffset);
    return true;
  }

  auto const& rDoc = m_rCore.GetDocument();

  Address NewAddr;
  if (!rDoc.MoveAddress(m_Top.m_Address, NewAddr, yOffset))
    return false;
  m_Format(NewAddr, m_FormatFlags, m_Height);

  if (yOffset < 0)
    m_Top.m_yAddressOffset = (m_Top.m_Address == m_rDoc.GetFirstAddress()) ? 0 : m_PrintData.GetLineNo(NewAddr) - 1;
  else
    m_Top.m_yAddressOffset = 0;

  m_Top.m_Address = NewAddr;

  return true;
}

bool FullDisassemblyView::MoveCursor(s32 xOffset, s32 yOffset, bool& rInvalidateView)
{
  rInvalidateView = false;
  auto const& rDoc = m_rCore.GetDocument();

  // We start to update the x offset.
  s32 xNewOffset = m_Cursor.m_xAddressOffset + xOffset;
  m_Cursor.m_xAddressOffset = (xNewOffset < 0) ? 0 : xNewOffset;

  // If yOffset is 0, we can return now.
  if (yOffset == 0)
    return true;

  // If we move the cursor in the same cell, we can simply update y offset.
  u16 CurLineNo = m_PrintData.GetLineNo(m_Cursor.m_Address);
  if (CurLineNo)
  {
    s32 yNewOffset = static_cast<s32>(m_Cursor.m_yAddressOffset) + yOffset;
    if (yNewOffset >= 0 && yNewOffset < static_cast<s32>(CurLineNo))
    {
      m_Cursor.m_yAddressOffset = static_cast<u16>(yNewOffset);
      if (m_Cursor.m_yAddressOffset < m_Top.m_yAddressOffset)
      {
        m_Top.m_yAddressOffset = m_Cursor.m_yAddressOffset;
        rInvalidateView = true;
      }
      return true;
    }
  }

  Address FirstAddr = rDoc.GetFirstAddress();
  Address NewAddr = m_Cursor.m_Address;
  while (yOffset != 0)
  {
    u16 CurLineNo = m_PrintData.GetLineNo(NewAddr);

    // move view backward
    if (yOffset < 0)
    {
      // If the cursor is at top, we can return true now.
      if (NewAddr == FirstAddr && m_Cursor.m_yAddressOffset == 0)
        return true;

      // If the current address is out of the view, we must update it.
      if ( CurLineNo == 0
        || m_Cursor.m_yAddressOffset == 0
        || m_Cursor.m_yAddressOffset >= CurLineNo)
      {
        // Get the previous address.
        if (!rDoc.MoveAddress(NewAddr, NewAddr, -1))
          return false;

        u16 NewLineNo = 0;

        // Update the view if needed.
        if (!m_PrintData.Contains(NewAddr))
        {
          m_Top.m_Address = NewAddr;
          m_Format(NewAddr, m_FormatFlags, m_Height);
          NewLineNo = m_PrintData.GetLineNo(NewAddr);
          if (NewLineNo == 0)
            return false;
          m_Top.m_yAddressOffset = NewLineNo - 1;
          rInvalidateView = true;
        }

        if (NewLineNo == 0)
          if ((NewLineNo = m_PrintData.GetLineNo(NewAddr)) == 0)
            return false;

        m_Cursor.m_yAddressOffset = NewLineNo - 1;
      }
      else
        --m_Cursor.m_yAddressOffset;

      ++yOffset;

    }

    // move view forward
    else
    {
      u16 Off;
      if (!m_PrintData.GetLineOffset(m_Cursor.m_Address, Off))
        return false;
      Off += (m_Cursor.m_yAddressOffset - m_Top.m_yAddressOffset + 2);
      if (Off > m_Height)
      {
        u16 MoveOff = Off - m_Height;
        if (!MoveView(0, MoveOff))
          return false;
        rInvalidateView = true;
      }

      if (m_Cursor.m_yAddressOffset == CurLineNo - 1)
      {
        if (!rDoc.MoveAddress(NewAddr, NewAddr, 1))
          return false;

        // Move the view until we reach the new address.
        while (!m_PrintData.Contains(NewAddr))
        {
          if (!MoveView(0, 1))
            return false;
          rInvalidateView = true;
        }

        m_Cursor.m_yAddressOffset = 0;
      }
      else
        ++m_Cursor.m_yAddressOffset;

      --yOffset;
    }
  }

  m_Cursor.m_Address = NewAddr;

  if (yOffset == 0)
    return true;

  if (yOffset < 0)
    m_Cursor.m_yAddressOffset = (m_Cursor.m_Address == m_rDoc.GetFirstAddress()) ? 0 : m_PrintData.GetLineNo(NewAddr) - 1;
  else
    m_Cursor.m_yAddressOffset = 0;

  return true;
}

bool FullDisassemblyView::SetCursor(u32 xPosition, u32 yPosition)
{
  if (m_Width < xPosition)
    return false;
  if (m_Height < yPosition)
    return false;

  u16 Off;
  LineData Line;
  if (!m_PrintData.GetLine(yPosition + m_Top.m_yAddressOffset, Off, Line))
    return false;

  m_Cursor.m_Address = Line.GetAddress();
  m_Cursor.m_xAddressOffset = xPosition + m_Top.m_xAddressOffset;
  m_Cursor.m_yAddressOffset = Off;
  return true;
}

bool FullDisassemblyView::GoTo(Address const& rAddress)
{
  auto const& rDoc = m_rCore.GetDocument();

  Address TopAddr;
  if (!rDoc.GetNearestAddress(rAddress, TopAddr))
    return false;
  m_Top.m_Address = TopAddr;
  m_Top.m_yAddressOffset = 0;
  return true;
}

bool FullDisassemblyView::GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const
{
  LineData Line;
  if (!m_PrintData.GetLine(rAddress, yPos, Line))
    return false;
  rAddress = Line.GetAddress();
  return true;
}

void FullDisassemblyView::BeginSelection(u32 x, u32 y)
{
  _ConvertViewOffsetToAddressOffset(m_SelectionBegin, x, y);
}

void FullDisassemblyView::EndSelection(u32 x, u32 y)
{
  _ConvertViewOffsetToAddressOffset(m_SelectionEnd, x, y);
  SetCursor(x, y);
}

void FullDisassemblyView::ResetSelection(void)
{
  m_SelectionBegin = m_SelectionEnd = m_Cursor;
}

bool FullDisassemblyView::_ConvertViewOffsetToAddressOffset(TextPosition& rTxtPos, u32 x, u32 y) const
{
  u16 Off;
  LineData Line;
  if (!m_PrintData.GetLine(y, Off, Line))
    return false;
  rTxtPos = Line.GetAddress();
  rTxtPos.m_xAddressOffset = x;
  rTxtPos.m_yAddressOffset = Off;
  return true;
}

bool FullDisassemblyView::_ConvertAddressOffsetToViewOffset(TextPosition const& rTxtPos, u32& x, u32& y) const
{
  u16 Offset;
  if (!m_PrintData.GetLineOffset(rTxtPos.m_Address, Offset))
    return false;
  x = rTxtPos.m_xAddressOffset;
  y = rTxtPos.m_yAddressOffset + Offset - m_Top.m_yAddressOffset;
  return true;
}