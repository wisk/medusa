#include "medusa/screen.hpp"
#include <algorithm>

MEDUSA_NAMESPACE_USE

Screen::Screen(Medusa& rCore, u16 Width, u16 Height, Address const& rAddress)
  : m_rCore(rCore)
  , m_Width(Width), m_Height(Height)
  , m_xPosition(), m_yPosition()
  , m_xOffset(), m_yOffset()
  , m_AddressOffset()
  , m_NumberOfAddress(rCore.GetDatabase().GetNumberOfAddress())
  , m_CurrentAddress(rAddress)
  , m_CurrentAddressLines()
{

}

Cell* Screen::GetCellFromPosition(u16 xChar, u16 yChar)
{
  View::LineInformation LineInfo;
  if (m_rCore.GetDatabase().GetView().GetLineInformation(yChar, LineInfo) == false)
    return nullptr;

  return m_rCore.GetCell(LineInfo.GetAddress());
}

Cell const* Screen::GetCellFromPosition(u16 xChar, u16 yChar) const
{
  View::LineInformation LineInfo;
  if (m_rCore.GetDatabase().GetView().GetLineInformation(yChar, LineInfo) == false)
    return nullptr;

  return m_rCore.GetCell(LineInfo.GetAddress());
}

void Screen::GetDimension(u16& rWidth, u16& rHeight) const
{
  rWidth  = 100;
  rHeight = m_NumberOfAddress;
}

void Screen::Resize(u16 Width, u16 Height)
{
  m_Width  = Width;
  m_Height = Height;
}

void Screen::Print(Printer& rPrinter)
{
  auto& rDatabase = m_rCore.GetDatabase();

  Address CurAddr;
  if (rDatabase.GetNearestAddress(m_CurrentAddress, CurAddr) == false)
    return;
  m_CurrentAddress = CurAddr;

  u16 CurLineNo = 0;
  while (true)
  {
    CurLineNo += rPrinter(CurAddr, m_xOffset, m_yOffset);
    if (CurLineNo > m_Height)
      break;
    if (!rDatabase.GetNextAddress(CurAddr, CurAddr))
      break;
  }

}

void Screen::Scroll(u16 xOffset, u16 yOffset)
{
  m_xPosition += xOffset;
  m_yPosition += yOffset;
  m_CurrentAddress += yOffset;
}

void Screen::GetScrollValues(u16& rxOffset, u16& ryOffset) const
{
  rxOffset = 0;
  ryOffset = 0;
}