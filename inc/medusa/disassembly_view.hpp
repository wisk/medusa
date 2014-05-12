#ifndef __MEDUSA_DISASSEMBLY_VIEW_HPP__
#define __MEDUSA_DISASSEMBLY_VIEW_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/medusa.hpp"
#include "medusa/view.hpp"
#include "medusa/cell_text.hpp"

#include <map>
#include <set>
#include <iterator>
#include <boost/thread/mutex.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT FormatDisassembly
{
public:
  enum Flags
  {
    ShowAddress        = 1 << 0,
    AddSpaceBeforeXref = 1 << 1
  };

  FormatDisassembly(Medusa const& rCore, PrintData& rPrintData) : m_rCore(rCore), m_rPrintData(rPrintData) {}
  void operator()(Address::List const& rAddresses, u32 Flags);
  void operator()(Address const& rAddress, u32 Flags, u16 LinesNo);

private:
  void FormatHeader    (Address const& rAddress, u32 Flags);
  void FormatAddress   (Address const& rAddress, u32 Flags);
  void FormatCell      (Address const& rAddress, u32 Flags);
  void FormatMultiCell (Address const& rAddress, u32 Flags);
  void FormatLabel     (Address const& rAddress, u32 Flags);
  void FormatXref      (Address const& rAddress, u32 Flags);
  void FormatMemoryArea(Address const& rAddress, u32 Flags);
  void FormatEmpty     (Address const& rAddress, u32 Flags);

  Medusa const& m_rCore;
  PrintData&    m_rPrintData;
};

class Medusa_EXPORT DisassemblyView : public View
{
public:
  DisassemblyView(Medusa& rCore, u32 FormatFlags, Address const& rAddress);
  virtual ~DisassemblyView(void);

  bool GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const;
  void GetDimension(u32& rWidth, u32& rHeight) const;

protected:
  typedef std::mutex MutexType;

  mutable MutexType m_Mutex;
  Medusa&           m_rCore;
  u32               m_FormatFlags;
  FormatDisassembly m_Format;
  PrintData         m_PrintData;
};

class Medusa_EXPORT FullDisassemblyView : public View
{
public:
  FullDisassemblyView(Medusa& rCore, u32 FormatFlags, u32 Width, u32 Height, Address const& rAddress);
  virtual ~FullDisassemblyView(void);

  Cell::SPtr       GetCellFromPosition(u32 xChar, u32 yChar);
  Cell::SPtr const GetCellFromPosition(u32 xChar, u32 yChar) const;
  void             GetDimension(u32& rWidth, u32& rHeight) const;

  void             Resize(u32 Width, u32 Height);
  void             Refresh(void);

  bool             MoveView(s32 xOffset, s32 yOffset);   //! Relative to the view
  bool             MoveCursor(s32 xOffset, s32 yOffset, bool& rInvalidateView); //! Relative to the view
  bool             SetCursor(u32 x, u32 y);              //! Absolute to the view

  bool             GoTo(Address const& rAddress);
  bool             GetAddressFromPosition(Address& rAddress, u32 xPos, u32 yPos) const;

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

  mutable MutexType m_Mutex;
  Medusa&           m_rCore;
  u32               m_FormatFlags;
  FormatDisassembly m_Format;
  PrintData         m_PrintData;

  TextPosition      m_Top;                //! Top screen
  TextPosition      m_Cursor;             //! Address position
  TextPosition      m_SelectionBegin;     //! Beginning of the selection
  TextPosition      m_SelectionEnd;       //! Ending of the selection
  u32               m_Width;
  u32               m_Height;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_DISASSEMBLY_VIEW_HPP__
