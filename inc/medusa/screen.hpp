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
  Screen(Medusa& rCore, u16 Width, u16 Height);

  Cell*       GetCellFromPosition(u16 xChar, u16 yChar);
  Cell const* GetCellFromPosition(u16 xChar, u16 yChar) const;
  void        GetMaximumDimension(u16& rWidth, u16& rHeight) const;
  void        GetDimension(u16& rWidth, u16& rHeight) const;
  void        Resize(u16 Width, u16 Height);
  void        Print(Printer& rPrinter) const;
  void        Scroll(u16 xOffset, u16 yOffset);

private:
  Medusa& m_rCore;
  u16 m_Width, m_Height;
  u16 m_xOffset, m_yOffset;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SCREEN_HPP__
