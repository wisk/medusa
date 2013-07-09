#ifndef __MEDUSA_VIEW_HPP__
#define __MEDUSA_VIEW_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/medusa.hpp"
#include "medusa/printer.hpp"

#include <map>
#include <set>
#include <iterator>
#include <boost/thread/mutex.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT View
{
public:
  View(Medusa& rCore, Printer& rPrinter, Address::List const& rAddresses, u32 PrinterFlags);

  void Refresh(void);
  void Print(void);
  bool GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const;
  void GetDimension(u32& rWidth, u32& rHeight) const;

protected:
  void          _Prepare(void);

  Medusa&       m_rCore;
  Printer&      m_rPrinter;
  u32           m_PrinterFlags;
  Address::List m_Addresses;
  u32           m_Width, m_Height; //! In character
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_VIEW_HPP__
