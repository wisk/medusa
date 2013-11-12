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
  bool             MoveCursor(s32 xOffset, s32 yOffset); //! Relative to the view
  bool             SetCursor(u32 x, u32 y);              //! Absolute to the view
  bool             GoTo(Address const& rAddress);
  bool             GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const;
  bool             EnsureCursorIsVisible(void);

  void             BeginSelection(u32 x, u32 y);        //! Absolute to the view
  void             EndSelection(u32 x, u32 y);          //! Absolute to the view
  void             ResetSelection(void);

  Address          GetCursorAddress(void)         const { return m_Cursor.m_Address;         }
  Address          GetSelectionFirstAddress(void) const { return m_SelectionBegin.m_Address; }
  Address          GetSelectionLastAddress(void)  const { return m_SelectionEnd.m_Address;   }

protected:
  typedef boost::mutex MutexType;

  struct TextPosition
  {
    TextPosition(Address const& rAddress = Address(), u16 xAddressOffset = 0, u16 yAddressOffset = 0)
      : m_Address(rAddress), m_xAddressOffset(xAddressOffset), m_yAddressOffset(yAddressOffset)
    {}

    bool operator==(TextPosition const& rTxtPos)
    {
      return m_Address        == rTxtPos.m_Address 
        &&   m_xAddressOffset == rTxtPos.m_xAddressOffset
        &&   m_yAddressOffset == rTxtPos.m_yAddressOffset;
    }

    Address m_Address;
    u16     m_xAddressOffset; //! x offset relative to address
    u16     m_yAddressOffset; //! y offset relative to address
  };

  bool        _ConvertViewOffsetToAddressOffset(TextPosition& rTxtPos, u32 x, u32 y) const;
  bool        _ConvertAddressOffsetToViewOffset(TextPosition const& rTxtPos, u32& x, u32& y) const;

  void        _Prepare(Address const& rAddress); //! Determine visible addresses

  mutable MutexType m_Mutex;
  Medusa&           m_rCore;
  Printer*          m_pPrinter;
  u32               m_PrinterFlags;
  Address::Vector   m_VisiblesAddresses;  //! All visibles addresses
  u32               m_Offset;             //! Start of address printing
  TextPosition      m_Cursor;             //! Address position
  TextPosition      m_SelectionBegin;     //! Beginning of the selection
  TextPosition      m_SelectionEnd;       //! Ending of the selection
  u32               m_Width, m_Height;    //! In character
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_DISASSEMBLY_VIEW_HPP__
