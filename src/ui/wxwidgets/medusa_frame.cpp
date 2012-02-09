#include "medusa_frame.hpp"

#include <medusa/log.hpp>

#include <wx/wfstream.h>

BEGIN_EVENT_TABLE(MedusaFrame, wxFrame)
  EVT_MENU(wxID_OPEN,                     MedusaFrame::OnOpen)
  EVT_MENU(MedusaFrame::wxID_DB_LOAD,     MedusaFrame::OnLoad)
  EVT_MENU(wxID_SAVE,                     MedusaFrame::OnSave)
  EVT_MENU(wxID_CLOSE,                    MedusaFrame::OnClose)
  EVT_MENU(wxID_ABOUT,                    MedusaFrame::OnAbout)
  EVT_MENU(wxID_EXIT,                     MedusaFrame::OnExit)
  EVT_MENU(MedusaFrame::wxID_VIEW_DISASM, MedusaFrame::OnViewDisasm)
  EVT_MENU(MedusaFrame::wxID_VIEW_LABEL,  MedusaFrame::OnViewLabel)
  EVT_MENU(MedusaFrame::wxID_VIEW_LOG,    MedusaFrame::OnViewLog)
END_EVENT_TABLE()

MedusaFrame::MedusaFrame(wxWindow* pParent, wxSize const& rSize)
  : wxFrame(pParent, -1, _("Medusa"),
  wxDefaultPosition, rSize,
  wxDEFAULT_FRAME_STYLE)
  , m_AuiNoteBook(this, wxID_ANY, wxDefaultPosition, rSize)
{
  wxMenuBar* pMenuBar = new wxMenuBar;
  SetMenuBar(pMenuBar);

  wxMenu* pFileMenu = new wxMenu;
  pFileMenu->Append(wxID_OPEN,        _("Open"));
  pFileMenu->Append(wxID_DB_LOAD,     _("Load"));
  pFileMenu->Append(wxID_SAVE,        _("Save"));
  pFileMenu->Append(wxID_CLOSE,       _("Close"));
  pFileMenu->AppendSeparator();
  pFileMenu->Append(wxID_EXIT,        _("Exit"));

  wxMenu* pViewMenu = new wxMenu;
  pViewMenu->Append(wxID_VIEW_DISASM, _("Disassembly"));
  pViewMenu->Append(wxID_VIEW_LABEL,  _("Label"));
  pViewMenu->Append(wxID_VIEW_LOG,    _("Log"));

  wxMenu* pHelpMenu = new wxMenu;
  pHelpMenu->Append(wxID_ABOUT,       _("About..."));

  pMenuBar->Append(pFileMenu,         _("File"));
  pMenuBar->Append(pViewMenu,         _("View"));
  pMenuBar->Append(pHelpMenu,         _("Help"));

  m_pDisasmTextCtrl = CreateDisassemblyTextCtrl();
  m_AuiNoteBook.AddPage(m_pDisasmTextCtrl, wxT("Disassembly view"));

  m_pLogTextCtrl = CreateLogTextCtrl();
  m_AuiNoteBook.AddPage(m_pLogTextCtrl, wxT("Log"));

  m_AuiNoteBook.Split(m_AuiNoteBook.GetPageIndex(m_pLogTextCtrl), wxBOTTOM);

  m_pLabelListCtrl = CreateLabelListCtrl();
  m_AuiNoteBook.AddPage(m_pLabelListCtrl, wxT("Labels"));

  m_AuiNoteBook.Split(m_AuiNoteBook.GetPageIndex(m_pLabelListCtrl), wxLEFT);

  m_AuiNoteBook.Update();
}

MedusaFrame::~MedusaFrame(void)
{
}

void MedusaFrame::AddLogMessage(wxString const& rMsg)
{
  m_pLogTextCtrl->AppendText(rMsg);

  if (!rMsg.EndsWith(wxT("\n")))
    m_pLogTextCtrl->AppendText(wxT("\n"));
}

void MedusaFrame::AddDisassemblyLine(wxString const& rLine)
{
  m_pDisasmTextCtrl->AddDisassemblyLine(rLine);
}

void MedusaFrame::AddLabel(wxString const& rName, wxString const& rType, wxString const& rAddress)
{
  long ItemIndex = m_pLabelListCtrl->InsertItem(0, rName);
  m_pLabelListCtrl->SetItem(ItemIndex, 1, rType);
  m_pLabelListCtrl->SetItem(ItemIndex, 2, rAddress);
}

void MedusaFrame::OnOpen(wxCommandEvent& rEvt)
{
  wxFileDialog FileDlg(this, _("Executable file"),
    wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr,
    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (FileDlg.ShowModal() == wxID_CANCEL)
    return;

  medusa::Log::Write("ui_wx") << "Opening file " << FileDlg.GetPath().ToStdWstring() << medusa::LogEnd;
  m_Core.Open(FileDlg.GetPath().ToStdWstring());

  //XXX: It could be nice if the use can select this folder.
  m_Core.LoadModules(L".");
}

void MedusaFrame::OnLoad(wxCommandEvent& rEvt)
{
}

void MedusaFrame::OnSave(wxCommandEvent& rEvt)
{
}

void MedusaFrame::OnClose(wxCommandEvent& rEvt)
{
  if (wxMessageBox(_("Are you sure ?"), _("wxMedusa"), wxYES_NO, this) == wxYES)
  {
    medusa::Log::Write("wx_ui") << "Closing file..." << medusa::LogEnd;
    m_Core.Close();
  }
}

void MedusaFrame::OnAbout(wxCommandEvent& rEvt)
{
  wxMessageBox(_("Medusa"), _("About..."));
}

void MedusaFrame::OnExit(wxCommandEvent& rEvt)
{
  Close();
}

void MedusaFrame::OnViewDisasm(wxCommandEvent& rEvt)
{
}

void MedusaFrame::OnViewLabel(wxCommandEvent& rEvt)
{
}

void MedusaFrame::OnViewLog(wxCommandEvent& rEvt)
{
}

DisassemblyTextCtrl* MedusaFrame::CreateDisassemblyTextCtrl(void)
{
  return new DisassemblyTextCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
}

wxTextCtrl* MedusaFrame::CreateLogTextCtrl(void)
{
  return new wxTextCtrl(this, wxID_ANY,
    wxEmptyString, wxDefaultPosition, wxDefaultSize,
    wxTE_MULTILINE | wxTE_READONLY | wxTE_AUTO_URL | wxTE_RICH2);
}

wxListCtrl* MedusaFrame::CreateLabelListCtrl(void)
{
  wxListCtrl* pLabelListCtrl = new wxListCtrl(
    this, wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxLC_REPORT | wxLC_AUTOARRANGE | wxLC_MASK_ALIGN);

  pLabelListCtrl->InsertColumn(0, _("Name"));
  pLabelListCtrl->InsertColumn(1, _("Type"));
  pLabelListCtrl->InsertColumn(2, _("Address"));

  return pLabelListCtrl;
}