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
  rWidth  = m_Width;
  rHeight = m_Height;
}

void Screen::Resize(u16 Width, u16 Height)
{
  m_Width  = Width;
  m_Height = Height;
}

void Screen::Print(Printer& rPrinter)
{
  m_CurrentAddressLines = rPrinter(m_CurrentAddress, m_xOffset, m_yOffset);

  for (u16 AddressIndex = m_AddressOffset; AddressIndex < m_Height;
    AddressIndex += rPrinter(m_CurrentAddress + AddressIndex, m_xOffset, m_yOffset))
  {}
}

void Screen::Scroll(u16 xOffset, u16 yOffset)
{
  m_xOffset += xOffset;
  m_yOffset += yOffset;


}

void Screen::GetScrollValues(u16& rxOffset, u16& ryOffset) const
{
  rxOffset = m_xOffset;
  ryOffset = m_yOffset;
}