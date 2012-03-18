#include "medusa_frame.hpp"
#include "configuration_dialog.hpp"

#include <boost/foreach.hpp>

#include <medusa/log.hpp>
#include <iostream>

/* Custom event */
IMPLEMENT_DYNAMIC_CLASS(wxMedusaEvent, wxEvent)

//const wxEventType MedusaOnCellUpdatedEvent = wxNewEventType();
wxDEFINE_EVENT(MedusaOnCellUpdatedEvent, wxMedusaEvent);

typedef void (wxEvtHandler::*wxMedusaEventHandler)(wxMedusaEvent&);

#define EVT_MEDUSA(evt, fn) \
  wx__DECLARE_EVT0(evt, &fn)

/* Medusa frame event tables */
BEGIN_EVENT_TABLE(MedusaFrame, wxFrame)

  // Menu events
  EVT_MENU(wxID_OPEN,                     MedusaFrame::OnOpen)
  EVT_MENU(MedusaFrame::wxID_DB_LOAD,     MedusaFrame::OnLoad)
  EVT_MENU(wxID_SAVE,                     MedusaFrame::OnSave)
  EVT_MENU(wxID_CLOSE,                    MedusaFrame::OnClose)
  EVT_MENU(wxID_ABOUT,                    MedusaFrame::OnAbout)
  EVT_MENU(wxID_EXIT,                     MedusaFrame::OnExit)
  EVT_MENU(MedusaFrame::wxID_VIEW_DISASM, MedusaFrame::OnViewDisasm)
  EVT_MENU(MedusaFrame::wxID_VIEW_LABEL,  MedusaFrame::OnViewLabel)
  EVT_MENU(MedusaFrame::wxID_VIEW_LOG,    MedusaFrame::OnViewLog)

  // List events
  EVT_LIST_ITEM_ACTIVATED(MedusaFrame::wxID_LABEL, MedusaFrame::OnLabelActivated)

  // Mouse events
  EVT_RIGHT_UP(MedusaFrame::OnDisasmMouseRightUp)

  // Command events
  EVT_COMMAND(MedusaFrame::wxID_APPEND_LOG, wxEVT_COMMAND_TEXT_UPDATED, MedusaFrame::OnAppendLog)

  // Context events
  EVT_CONTEXT_MENU(MedusaFrame::OnDisasmContextMenu)

  // Medusa events
  EVT_MEDUSA(MedusaOnCellUpdatedEvent, MedusaFrame::OnCellUpdated)
END_EVENT_TABLE()

bool MedusaNotifier::OnCellUpdated(EventHandler::UpdatedCell const& rUpdatedCell)
{
  if (rUpdatedCell.GetModifiedAddresses().size())
    BOOST_FOREACH(medusa::Address const& rAddr, rUpdatedCell.GetModifiedAddresses())
    {
      wxMedusaEvent* pEvt = new wxMedusaEvent(m_pParent->GetId(), MedusaOnCellUpdatedEvent);
      pEvt->SetAddress(rAddr);
      wxQueueEvent(m_pParent->GetEventHandler(), pEvt);
    }

  return true;
}

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
  // LATER: wxPostEvent ?
  OnClose(wxCommandEvent());
}

void MedusaFrame::AddLogMessage(wxString const& rMsg)
{
  wxCommandEvent Evt(wxEVT_COMMAND_TEXT_UPDATED, wxID_APPEND_LOG);
  Evt.SetString(rMsg);
  m_pLogTextCtrl->GetEventHandler()->AddPendingEvent(Evt);
}

void MedusaFrame::AddLabel(medusa::Address const& rAddr, medusa::Label const& rLbl)
{
  wxString LabelName = rLbl.GetName();
  wxString LabelTypeName = "";
  switch (rLbl.GetType())
  {
  case medusa::Label::LabelCode:    LabelTypeName = "code";   break;
  case medusa::Label::LabelData:    LabelTypeName = "data";   break;
  case medusa::Label::LabelString:  LabelTypeName = "string"; break;
  default:                                                    break;
  }

  long ItemIndex = m_pLabelListCtrl->InsertItem(0, LabelName);
  m_pLabelListCtrl->SetItem(ItemIndex, 1, LabelTypeName);
  m_pLabelListCtrl->SetItem(ItemIndex, 2, rAddr.ToString());
}

void MedusaFrame::OnOpen(wxCommandEvent& rEvt)
{
  wxFileDialog FileDlg(this, _("Executable file"),
    wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr,
    wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (FileDlg.ShowModal() == wxID_CANCEL)
    return;

  if (m_Core.IsOpened())
  {
    medusa::Log::Write("wx_ui") << "Closing file..." << medusa::LogEnd;
    m_pDisasmTextCtrl->ClearDisassembly();
    m_pLabelListCtrl->DeleteAllItems();
    m_Core.Close();
  }

  medusa::Log::Write("ui_wx") << "Opening file " << FileDlg.GetPath().ToStdWstring() << medusa::LogEnd;
  m_Core.Open(FileDlg.GetPath().ToStdWstring());

  //XXX: It could be nice if the use can select this folder.
  m_Core.LoadModules(L".");

  //XXX: delete?
  m_Core.GetDatabase().StartsEventHandling(new MedusaNotifier(this));

  ConfigurationDialog CfgDlg(
      this, wxID_ANY, _("Modules configuration"),
      wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
      m_Core.GetSupportedLoaders(), m_Core.GetArchitectures()
      );
  if (CfgDlg.ShowModal() == wxID_CANCEL)
    return;

  medusa::Loader::SPtr spLdr        = CfgDlg.GetLoader();
  medusa::Architecture::SPtr spArch = CfgDlg.GetArchitecture();
  medusa::Configuration Cfg         = CfgDlg.GetConfiguration();

  spArch->UseConfiguration(Cfg);

  medusa::Log::Write("ui_wx")
    << "Load with " << spLdr->GetName()
    << " and disassemble with " << spArch->GetName()
    << medusa::LogEnd;

  spLdr->Map();
  medusa::Log::Write("ui_wx") << "Mapping..." << medusa::LogEnd;

  medusa::Log::Write("ui_wx") << "Disassembling..." << medusa::LogEnd;
  m_Core.DisassembleAsync(spLdr, spArch);

  medusa::Database::TLabelMap const& rLabels = m_Core.GetDatabase().GetLabels();
  for (medusa::Database::TLabelMap::const_iterator It = rLabels.begin();
      It != rLabels.end(); ++It)
      AddLabel(It->left, It->right);

  m_pLabelListCtrl->SetColumnWidth(0, wxLIST_AUTOSIZE);
  m_pLabelListCtrl->SetColumnWidth(1, wxLIST_AUTOSIZE);
  m_pLabelListCtrl->SetColumnWidth(2, wxLIST_AUTOSIZE);

  //for (medusa::Database::TConstIterator itMemArea = m_Core.GetDatabase().Begin();
  //    itMemArea != m_Core.GetDatabase().End(); ++itMemArea)
  //{
  //  m_pDisasmTextCtrl->AddMemoryArea(*itMemArea);
  //}
}

void MedusaFrame::OnLoad(wxCommandEvent& rEvt)
{
}

void MedusaFrame::OnSave(wxCommandEvent& rEvt)
{
}

void MedusaFrame::OnClose(wxCommandEvent& rEvt)
{
  wxASSERT(wxIsMainThread() && "Not called from main thread");

  if (wxMessageBox(_("Are you sure ?"), _("wxMedusa"), wxYES_NO, this) == wxNO)
    return;

  medusa::Log::Write("wx_ui") << "Closing file..." << medusa::LogEnd;
  m_pDisasmTextCtrl->ClearDisassembly();
  m_pLabelListCtrl->DeleteAllItems();
  m_Core.GetDatabase().StopsEventHandling();
  m_Core.Close();
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

void MedusaFrame::OnLabelActivated(wxListEvent& rEvt)
{
  medusa::Address Addr = m_Core.GetDatabase().GetAddressFromLabelName(rEvt.GetText().ToStdString());
  m_pDisasmTextCtrl->GoTo(Addr);
}

void MedusaFrame::OnDisasmMouseRightUp(wxMouseEvent& rEvt)
{
  wxPoint pt = rEvt.GetPosition();
  DoDisasmContextMenu(wxPoint(pt.x, pt.y));
}

void MedusaFrame::OnAppendLog(wxCommandEvent& rEvt)
{
  wxASSERT(wxIsMainThread() && "Not called from main thread");
  m_pLogTextCtrl->AppendText(rEvt.GetString());
}

void MedusaFrame::OnDisasmContextMenu(wxContextMenuEvent& rEvt)
{
  wxPoint pt = rEvt.GetPosition();
  wxPoint ScreenPoint = ScreenToClient(pt);

  wxHitTest ht = HitTest(pt);
  if (ht != wxHT_WINDOW_INSIDE)
    ScreenPoint = m_pDisasmTextCtrl->PointFromPosition(m_pDisasmTextCtrl->GetCurrentPos());

  DoDisasmContextMenu(ScreenPoint);
}

void MedusaFrame::OnCellUpdated(wxMedusaEvent& rEvt)
{
  wxASSERT(wxIsMainThread() && "Not called from main thread");

  auto pCell = m_Core.GetDatabase().RetrieveCell(rEvt.GetAddress());

  if (pCell == nullptr) return;

  // Print label if exists
  medusa::Label Lbl = m_Core.GetDatabase().GetLabelFromAddress(rEvt.GetAddress());
  if (Lbl.GetType() != medusa::Label::LabelUnknown)
    m_pDisasmTextCtrl->AddLabel(rEvt.GetAddress(), Lbl);

  // Print multicell if exists
  medusa::MultiCell* pMultiCell = m_Core.GetDatabase().RetrieveMultiCell(rEvt.GetAddress());
  if (pMultiCell)
  {
    m_pDisasmTextCtrl->AddMultiCell(rEvt.GetAddress(), *pMultiCell);
  }

  // Print xrefs if exist
  //medusa::Address::List RefAddrList;
  //m_Core.GetDatabase().GetXRefs().From(rEvt.GetAddress(), RefAddrList);

  //if (RefAddrList.size())
  //{
  //  wxString RefLine = ";; xref:";

  //  BOOST_FOREACH(medusa::Address Addr, RefAddrList)
  //    RefLine += wxString(" ") + Addr.ToString();

  //  AddDisassemblyLine(RefLine);
  //}

  m_pDisasmTextCtrl->AddCell(rEvt.GetAddress(), *pCell);
}

void MedusaFrame::DoDisasmContextMenu(wxPoint Point)
{
  static const char *CellStr[] =
  { "Cell", "Instruction", "Value", "Character" };

  static const char *MultiCellStr[] =
  { "Unknown", "Function", "String", "Struct", "Array" };

  int Pos  = m_pDisasmTextCtrl->PositionFromPoint(Point);
  int Line = m_pDisasmTextCtrl->LineFromPosition(Pos);
  medusa::Address Addr;

  medusa::Cell      const* pCell      = m_Core.GetDatabase().RetrieveCell(Addr);
  medusa::MultiCell const* pMultiCell = m_Core.GetDatabase().RetrieveMultiCell(Addr);
  medusa::Label            Lbl        = m_Core.GetDatabase().GetLabelFromAddress(Addr);

  if (pCell == NULL) return;
  if (pCell->GetType() >= sizeof(CellStr) / sizeof(*CellStr)) return;

  medusa::Log::Write("ui_wx") << Addr.ToString() << medusa::LogEnd;
  wxMenu *pMenu = new wxMenu;
  pMenu->Append(wxID_ANY, CellStr[pCell->GetType()]);

  if (pMultiCell && pMultiCell->GetType() < sizeof(MultiCellStr) / sizeof(MultiCellStr[0]))
    pMenu->Append(wxID_ANY, MultiCellStr[pMultiCell->GetType()]);

  if (Lbl.GetType() != medusa::Label::LabelUnknown)
    pMenu->Append(wxID_ANY, "Rename label");

  PopupMenu(pMenu, Point.x, Point.y);
}

DisassemblyTextCtrl* MedusaFrame::CreateDisassemblyTextCtrl(void)
{
  return new DisassemblyTextCtrl(this, wxID_DISASM, wxDefaultPosition, wxDefaultSize, wxNO_FULL_REPAINT_ON_RESIZE);
}

wxTextCtrl* MedusaFrame::CreateLogTextCtrl(void)
{
  return new wxTextCtrl(this, wxID_LOG,
    wxEmptyString, wxDefaultPosition, wxDefaultSize,
    wxTE_MULTILINE | wxTE_READONLY | wxTE_AUTO_URL | wxTE_RICH2);
}

wxListCtrl* MedusaFrame::CreateLabelListCtrl(void)
{
  wxListCtrl* pLabelListCtrl = new wxListCtrl(
    this, wxID_LABEL,
    wxDefaultPosition, wxDefaultSize,
    wxLC_REPORT | wxLC_AUTOARRANGE | wxLC_MASK_ALIGN);

  pLabelListCtrl->InsertColumn(0, _("Name"));
  pLabelListCtrl->InsertColumn(1, _("Type"));
  pLabelListCtrl->InsertColumn(2, _("Address"));

  return pLabelListCtrl;
}