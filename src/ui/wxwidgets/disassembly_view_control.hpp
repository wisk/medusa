#ifndef __WXMEDUSA_DISASSEMBLY_VIEW_DIALOG__
#define __WXMEDUSA_DISASSEMBLY_VIEW_DIALOG__

#include <wx/wx.h>
#include <wx/stc/stc.h>

class DisassemblyTextCtrl : public wxStyledTextCtrl
{
public:
  DisassemblyTextCtrl(
    wxWindow *pParent, wxWindowID WinId = wxID_ANY,
    wxPoint const& rPos = wxDefaultPosition, wxSize const& rSize = wxDefaultSize,
    long Style = 0, wxString const& rName = wxSTCNameStr);

  void AddDisassemblyLine(wxString const& rLine);

  void OnClick(wxMouseEvent& rMouseEvt);

protected:
};

#endif // !__WXMEDUSA_DISASSEMBLY_VIEW_DIALOG__