#ifndef __WXMEDUSA_DISASSEMBLY_VIEW_DIALOG__
#define __WXMEDUSA_DISASSEMBLY_VIEW_DIALOG__

#include <wx/wx.h>
#include <wx/stc/stc.h>

#include <map>

#include <medusa/address.hpp>
#include <medusa/cell.hpp>
#include <medusa/multicell.hpp>
#include <medusa/memory_area.hpp>
#include <medusa/label.hpp>

//DECLARE_EVENT_TYPE(DisassemblyCommandEvent, wxANY_ID);
extern const wxEventType DisassemblyEventType;

class DisassemblyEvent : public wxCommandEvent
{
public:
  DisassemblyEvent(wxEventType CmdType = 0, int Id = 0)
    : wxCommandEvent(CmdType, Id), m_Address() {}

  DisassemblyEvent(DisassemblyEvent const& rDisasmCmdEvt)
    : wxCommandEvent(rDisasmCmdEvt), m_Address(rDisasmCmdEvt.GetAddress()) {}

  wxEvent*        Clone(void) const { return new DisassemblyEvent(*this); }

  void            SetAddress(medusa::Address const& rAddr)  { m_Address = rAddr; }
  medusa::Address GetAddress(void) const                    { return m_Address;  }

private:
  medusa::Address m_Address;
};

typedef void (wxEvtHandler::*DisassemblyEventFunction)(DisassemblyEvent&);

#define DisassemblyEventHandler(func)\
  (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)\
  wxStaticCastEvent(DisassemblyEventFunction, &func)

#define EVT_DISASM(id, fn)                                          \
  DECLARE_EVENT_TABLE_ENTRY(DisassemblyCommandEvent, id, wxID_ANY,  \
  (wxObjectEventFunction)(wxEventFunction)                          \
  (wxCommandEventFunction) wxStaticCastEvent(                       \
  DisassemblyEventFunction, &fn ), (wxObject*) NULL ),

class DisassemblyTextCtrl : public wxStyledTextCtrl
{
public:
  DisassemblyTextCtrl(
    wxWindow *pParent, wxWindowID WinId = wxID_ANY,
    wxPoint const& rPos = wxDefaultPosition, wxSize const& rSize = wxDefaultSize,
    long Style = 0, wxString const& rName = wxSTCNameStr);

  void AddDisassemblyLine(wxString const& rLine);

  void AddCell(medusa::Address const& rAddr, medusa::Cell const& rCell);
  void AddMultiCell(medusa::Address const& rAddr, medusa::MultiCell const& rMultiCell);
  void AddLabel(medusa::Address const& rAddr, medusa::Label const& rLabel);
  void AddMemoryArea(medusa::MemoryArea const& rMemArea);

  bool GoTo(medusa::Address const& rAddr);

  void DeleteAddress(medusa::Address const& rAddr);
  void ClearDisassembly(void);

  void AddLineAddress(int Line, medusa::Address const& rAddr);
  bool AddressToLine(medusa::Address const& rAddr, std::list<int>& rLineList);
  bool LineToAddress(int Line, medusa::Address& rAddr);

protected:
  DECLARE_EVENT_TABLE()
  void OnDoubleClick(wxStyledTextEvent& rEvt);
  void OnMouseRightUp(wxMouseEvent& rEvt);
  void OnContextMenu(wxContextMenuEvent& rEvt);

  // XXX: It should be possible to replace them with a boost::bimap...
  typedef std::multimap<medusa::Address, int> AddrToLineMap;
  typedef std::map<int, medusa::Address> LineToAddrMap;
  AddrToLineMap m_AddrToLineMap;
  LineToAddrMap m_LineToAddrMap;
};

#endif // !__WXMEDUSA_DISASSEMBLY_VIEW_DIALOG__