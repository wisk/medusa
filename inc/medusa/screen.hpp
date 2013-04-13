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
  Screen(Medusa& rCore, Printer& rPrinter, u32 Width, u32 Height, Address const& rAddress);

  Cell*       GetCellFromPosition(u32 xChar, u32 yChar);
  Cell const* GetCellFromPosition(u32 xChar, u32 yChar) const;
  void        GetDimension(u32& rWidth, u32& rHeight) const;
  void        Refresh(void);
  void        Resize(u32 Width, u32 Height);
  void        Print(void);
  bool        Scroll(s32 xOffset, s32 yOffset);
  bool        Move(u32 xPosition, u32 yPosition);
  bool        GoTo(Address const& rAddress);
  bool        GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const;

private:
  void        _Prepare(Address const& rAddress); //! Determine visible addresses

  Medusa&       m_rCore;
  Printer&      m_rPrinter;
  Address::List m_VisiblesAddresses;  //! All visibles addresses
  u32           m_Width, m_Height;    //! In character
  u32           m_xOffset, m_yOffset; //! Relative to Address
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SCREEN_HPP__
