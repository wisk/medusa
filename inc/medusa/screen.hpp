#ifndef __MEDUSA_SCREEN_HPP__
#define __MEDUSA_SCREEN_HPP__

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/printer.hpp"
#include "medusa/medusa.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Screen
{
public:
  Screen(Medusa& rCore, u16 Width, u16 Height, Address const& rAddress);

  Cell*       GetCellFromPosition(u16 xChar, u16 yChar);
  Cell const* GetCellFromPosition(u16 xChar, u16 yChar) const;
  void        GetDimension(u16& rWidth, u16& rHeight) const;
  void        Resize(u16 Width, u16 Height);
  void        Print(Printer& rPrinter);
  void        Scroll(u16 xOffset, u16 yOffset);
  void        GetScrollValues(u16& xOffset, u16& yOffset) const;

private:
  Medusa& m_rCore;
  u16     m_Width, m_Height;
  u16     m_xPosition, m_yPosition;
  u16     m_xOffset, m_yOffset;
  u16     m_AddressOffset;
  u16     m_NumberOfAddress;
  Address m_CurrentAddress;
  u16     m_CurrentAddressLines;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SCREEN_HPP__
