#ifndef __MEDUSA_DISASSEMBLY_VIEW_HPP__
#define __MEDUSA_DISASSEMBLY_VIEW_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/medusa.hpp"
#include "medusa/view.hpp"
#include "medusa/printer.hpp"

#include <map>
#include <set>
#include <iterator>
#include <boost/thread/mutex.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT DisassemblyView : public View
{
public:
  DisassemblyView(Medusa& rCore, Printer* pPrinter, u32 PrinterFlags, Address::List const& rAddresses);
  virtual ~DisassemblyView(void);

  void Refresh(void);
  void Print(void);
  bool GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const;
  void GetDimension(u32& rWidth, u32& rHeight) const;

protected:
  typedef boost::mutex MutexType;

  void          _Prepare(void);

  mutable MutexType m_Mutex;
  Medusa&           m_rCore;
  Printer*          m_pPrinter;
  u32               m_PrinterFlags;
  Address::List     m_Addresses;
  u32               m_Width, m_Height; //! In character
};

class Medusa_EXPORT FullDisassemblyView : public View
{
public:
  FullDisassemblyView(Medusa& rCore, Printer* pPrinter, u32 PrinterFlags, u32 Width, u32 Height, Address const& rAddress);
  virtual ~FullDisassemblyView(void);

  Cell::SPtr       GetCellFromPosition(u32 xChar, u32 yChar);
  Cell::SPtr const GetCellFromPosition(u32 xChar, u32 yChar) const;
  void             GetDimension(u32& rWidth, u32& rHeight) const;
  void             Refresh(void);
  void             Resize(u32 Width, u32 Height);
  void             Print(void);
  bool             Scroll(s32 xOffset, s32 yOffset);
  bool             Move(u32 xPosition, u32 yPosition);
  bool             GoTo(Address const& rAddress);
  bool             GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const;

protected:
  typedef boost::mutex MutexType;

  void        _Prepare(Address const& rAddress); //! Determine visible addresses

  mutable MutexType m_Mutex;
  Medusa&           m_rCore;
  Printer*          m_pPrinter;
  u32               m_PrinterFlags;
  Address::List     m_VisiblesAddresses;  //! All visibles addresses
  u32               m_Width, m_Height;    //! In character
  u32               m_xOffset, m_yOffset; //! Relative to Address
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_DISASSEMBLY_VIEW_HPP__
