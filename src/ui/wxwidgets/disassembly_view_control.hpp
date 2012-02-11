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

  void ClearDisassembly(void);

protected:
  void OnDoubleClick(wxStyledTextEvent& rEvt);

  void AddLineAddress(int Line, medusa::Address const& rAddr);
  bool AddressToLine(medusa::Address const& rAddr, std::list<int>& rLineList);
  bool LineToAddress(int Line, medusa::Address& rAddr);

  // XXX: It should be possible to replace them with a boost::bimap...
  typedef std::multimap<medusa::Address, int> AddrToLineMap;
  typedef std::map<int, medusa::Address> LineToAddrMap;
  AddrToLineMap m_AddrToLineMap;
  LineToAddrMap m_LineToAddrMap;
};

#endif // !__WXMEDUSA_DISASSEMBLY_VIEW_DIALOG__