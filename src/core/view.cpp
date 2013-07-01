#include "medusa/view.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_USE;

View::View(Medusa& rCore, Printer& rPrinter, Address::List const& rAddresses)
  : m_rCore(rCore), m_rPrinter(rPrinter), m_Addresses(rAddresses)
  , m_Width(), m_Height()
{
  _Prepare();
}

void View::Refresh(void)
{
  _Prepare();
}

void View::Print(void)
{
  u32 LineNo;
  u32 yOffset = 0;

  for (auto itAddr = std::begin(m_Addresses); itAddr != std::end(m_Addresses); ++itAddr)
  {
    LineNo = m_rPrinter(*itAddr, 0, yOffset);
    if (LineNo == 0)
      return;
    yOffset += LineNo;
  }
}

bool View::GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const
{
  if (yPos >= m_Addresses.size())
    return false;

  auto itAddr = m_Addresses.begin();
  while (yPos--)
    ++itAddr;

  rAddress = *itAddr;
  return true;
}

void View::_Prepare(void)
{
  m_Height = 0;
  m_Width  = 0;
  for (auto itAddr = std::begin(m_Addresses); itAddr != std::end(m_Addresses); ++itAddr)
  {
    m_Height += m_rPrinter.GetLineHeight(*itAddr);
    m_Width   = std::max(m_Width, static_cast<u32>(m_rPrinter.GetLineWidth(*itAddr)));
  }
}

void View::GetDimension(u32& rWidth, u32& rHeight) const
{
  rWidth  = m_Width;
  rHeight = m_Height;
}