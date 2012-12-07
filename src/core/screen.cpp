#include "medusa/screen.hpp"
#include <algorithm>

MEDUSA_NAMESPACE_USE

Screen::Screen(Medusa& rCore, u16 Width, u16 Height)
  : m_rCore(rCore)
  , m_Width(Width), m_Height(Height)
  , m_xOffset(), m_yOffset()
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

void Screen::GetMaximumDimension(u16& rWidth, u16& rHeight) const
{
  rWidth  = std::max(m_Width, static_cast<u16>(100));
  rHeight = static_cast<u16>(m_rCore.GetDatabase().GetView().GetNumberOfLine());
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

void Screen::Print(Printer& rPrinter) const
{
  const View::LineInformation BadLine;
  for (auto LineIndex = m_yOffset; LineIndex < m_Height; ++LineIndex)
  {
    View::LineInformation LineInfo;
    if (!m_rCore.GetDatabase().GetView().GetLineInformation(LineIndex, LineInfo))
    {
      rPrinter(BadLine, m_xOffset);
      continue;
    }

    rPrinter(LineInfo, m_xOffset);
  }
}

void Screen::Scroll(u16 xOffset, u16 yOffset)
{
  /* Overflow check */
  if ((xOffset + m_xOffset < xOffset) || (yOffset + m_yOffset < yOffset))
    return;

  u16 MaxWidth;
  u16 MaxHeight;

  GetMaximumDimension(MaxWidth, MaxHeight);

  u16 xLimit = MaxWidth - xOffset;
  u16 yLimit = MaxHeight - yOffset;
  m_xOffset = std::min(xLimit, static_cast<u16>(xOffset + m_xOffset));
  m_yOffset = std::min(yLimit, static_cast<u16>(yOffset + m_yOffset));
  std::cout << "** " << m_yOffset << " ** " << MaxHeight << " ** " << std::endl;
}
