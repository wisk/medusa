#ifndef __MEDUSA_FRAME__
#define __MEDUSA_FRAME__

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/listctrl.h>

#include <medusa/medusa.hpp>
#include <medusa/event_handler.hpp>
#include <medusa/address.hpp>
#include <medusa/label.hpp>

#include "disassembly_view_control.hpp"

class wxMedusaEvent : public wxEvent
{
public:
  wxMedusaEvent(wxMedusaEvent const& rEvt)
    : wxEvent(rEvt.m_id, rEvt.m_eventType)
    , m_Address(rEvt.m_Address)
  {}

  wxMedusaEvent(int WinId = 0, wxEventType EvtType = wxEVT_NULL)
    : wxEvent(WinId, EvtType)
  {}

  virtual wxEvent *Clone(void) const
  { return new wxMedusaEvent(*this); };

  medusa::Address const& GetAddress(void) const                   { return m_Address;  }
  void                   SetAddress(medusa::Address const& rAddr) { m_Address = rAddr; }

  DECLARE_DYNAMIC_CLASS(wxMedusaEvent)

private:
  medusa::Address m_Address;
};

class MedusaNotifier : public medusa::EventHandler
{
public:
  MedusaNotifier(wxFrame* pParent) : m_pParent(pParent) {}

  virtual bool OnCellUpdated(EventHandler::UpdatedCell const& rUpdatedCell);

private:
  wxFrame* m_pParent;
};

class MedusaFrame : public wxFrame
{
public:
  enum Id
  {
    wxID_DB_LOAD = wxID_HIGHEST,
    wxID_DISASM,
    wxID_LABEL,
    wxID_LOG,
    wxID_VIEW_DISASM,
    wxID_VIEW_LABEL,
    wxID_VIEW_LOG,
    wxID_APPEND_LOG
  };

  MedusaFrame(wxWindow* pParent, wxSize const& rSize);
  ~MedusaFrame(void);

  void AddLogMessage(wxString const& rMsg);
  void AddLabel(medusa::Address const& rAddr, medusa::Label const& rLbl);

protected:

  // wxWidgets event handlers
  DECLARE_EVENT_TABLE()
  void OnOpen(wxCommandEvent& rEvt);
  void OnLoad(wxCommandEvent& rEvt);
  void OnSave(wxCommandEvent& rEvt);
  void OnClose(wxCommandEvent& rEvt);
  void OnAbout(wxCommandEvent& rEvt);
  void OnExit(wxCommandEvent& rEvt);

  void OnViewDisasm(wxCommandEvent& rEvt);
  void OnViewLabel(wxCommandEvent& rEvt);
  void OnViewLog(wxCommandEvent& rEvt);

  void OnLabelActivated(wxListEvent& rEvt);

  void OnDisasmMouseRightUp(wxMouseEvent& rEvt);
  void OnDisasmContextMenu(wxContextMenuEvent& rEvt);

  void OnAppendLog(wxCommandEvent& rEvt);

  void OnCellUpdated(wxMedusaEvent& rEvt);

  void DoDisasmContextMenu(wxPoint Point);

private:
  DisassemblyTextCtrl* CreateDisassemblyTextCtrl(void);
  wxTextCtrl*          CreateLogTextCtrl(void);
  wxListCtrl*          CreateLabelListCtrl(void);

  wxAuiNotebook        m_AuiNoteBook;

  DisassemblyTextCtrl* m_pDisasmTextCtrl;
  wxTextCtrl*          m_pLogTextCtrl;
  wxListCtrl*          m_pLabelListCtrl;

  medusa::Medusa       m_Core;
};

#endif // !__MEDUSA_FRAME__