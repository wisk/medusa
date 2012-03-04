#include "configuration_dialog.hpp"

#include <boost/foreach.hpp>

BEGIN_EVENT_TABLE(ConfigurationDialog, wxDialog)
  EVT_COMBOBOX(wxID_CB_LDR, ConfigurationDialog::OnComboBoxLdrUpdated)
  EVT_COMBOBOX(wxID_CB_ARCH, ConfigurationDialog::OnComboBoxArchUpdated)
END_EVENT_TABLE()

ConfigurationDialog::ConfigurationDialog(
    wxWindow* pParent, wxWindowID Id, wxString const& rTitle,
    wxPoint const& rPos, wxSize const& rSize, long Style,
    medusa::Loader::VectorSPtr const& rLdrs, medusa::Architecture::VectorSPtr const& rArchs)
  : wxDialog(pParent, Id, rTitle, rPos, rSize, Style)
  , m_rLdrs(rLdrs), m_rArchs(rArchs)
{
  wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);
  m_pBoxSizerOpt     = new wxBoxSizer(wxVERTICAL);

  wxArrayString LdrNames;
  wxArrayString ArchNames;

  BOOST_FOREACH(medusa::Loader::SPtr Ldr, m_rLdrs)
    LdrNames.Add(Ldr->GetName());

  medusa::Architecture::SPtr spBestArch = m_rLdrs[0]->GetMainArchitecture(m_rArchs);

  BOOST_FOREACH(medusa::Architecture::SPtr Arch, m_rArchs)
  {
    if (spBestArch == Arch)
      ArchNames.Insert(spBestArch->GetName(), 0);
    else
      ArchNames.Add(Arch->GetName());
  }

  m_Cfg.Clear();

  medusa::ConfigurationModel CfgMdl;
  spBestArch->FillConfigurationModel(CfgMdl);
  m_rLdrs[0]->Configure(CfgMdl.GetConfiguration());

  OptionVisitor::ControlList CtrlList;

  OptionVisitor OptVst(this, CfgMdl.GetConfiguration(), CtrlList);
  for (medusa::ConfigurationModel::ConstIterator It = CfgMdl.Begin();
      It != CfgMdl.End(); ++It)
    boost::apply_visitor(OptVst, *It);

  m_Cfg = CfgMdl.GetConfiguration();

  OptVst.FillsControls(m_pBoxSizerOpt);

  m_pComboBoxLdr  = CreateComboBox(wxID_CB_LDR,  LdrNames);
  m_pComboBoxArch = CreateComboBox(wxID_CB_ARCH, ArchNames);

  pSizer->Add(m_pComboBoxLdr,  0, wxEXPAND | wxLEFT | wxRIGHT, 5);
  pSizer->Add(m_pComboBoxArch, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);
  pSizer->Add(m_pBoxSizerOpt,  2, wxEXPAND | wxLEFT | wxRIGHT, 5);

  wxBoxSizer* pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  pButtonSizer->Add(new wxButton(this, wxID_OK,     "OK"),     0, wxALIGN_LEFT,  5);
  pButtonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 1, wxALIGN_RIGHT, 5);

  pSizer->Add(pButtonSizer);
  SetSizer(pSizer);
  pSizer->SetSizeHints(this);
}

medusa::Loader::SPtr ConfigurationDialog::GetLoader(void) const
{
  wxString LdrName = m_pComboBoxLdr->GetValue();
  BOOST_FOREACH(medusa::Loader::SPtr spLdr, m_rLdrs)
    if (LdrName == spLdr->GetName())
      return spLdr;

  return medusa::Loader::SPtr();
}

medusa::Architecture::SPtr ConfigurationDialog::GetArchitecture(void) const
{
  wxString ArchName = m_pComboBoxArch->GetValue();
  BOOST_FOREACH(medusa::Architecture::SPtr spArch, m_rArchs)
    if (ArchName == spArch->GetName())
      return spArch;

  return medusa::Architecture::SPtr();
}

medusa::Configuration ConfigurationDialog::GetConfiguration(void) const
{
  return m_Cfg;
}

wxComboBox* ConfigurationDialog::CreateComboBox(wxWindowID Id, wxArrayString const& rChoices)
{
  wxString Choice = wxEmptyString;
  if (!rChoices.IsEmpty())
    Choice = rChoices[0];

  return new wxComboBox(
      this, Id, Choice,
      wxDefaultPosition, wxDefaultSize,
      rChoices,
      wxCB_READONLY);
}

//XXX: Could be optimize with wxComboBox::SetSelection
void ConfigurationDialog::OnComboBoxLdrUpdated(wxCommandEvent& rEvt)
{
  medusa::Log::Write("wx_ui") << rEvt.GetString() << medusa::LogEnd;

  medusa::Loader::SPtr spCurLdr;
  BOOST_FOREACH(medusa::Loader::SPtr spLdr, m_rLdrs)
    if (spLdr->GetName() == rEvt.GetString())
    {
      spCurLdr = spLdr;
      break;
    }

  if (spCurLdr == medusa::Loader::SPtr()) return;

  medusa::Architecture::SPtr spBestArch = spCurLdr->GetMainArchitecture(m_rArchs);

  wxArrayString ArchNames;

  BOOST_FOREACH(medusa::Architecture::SPtr Arch, m_rArchs)
  {
    if (spBestArch == Arch)
      ArchNames.Insert(spBestArch->GetName(), 0);
    else
      ArchNames.Add(Arch->GetName());
  }

  wxComboBox* pNewComboBoxArch = CreateComboBox(wxID_CB_ARCH, ArchNames);
  GetSizer()->Replace(m_pComboBoxArch, pNewComboBoxArch);
  delete m_pComboBoxArch;
  m_pComboBoxArch = pNewComboBoxArch;
  GetSizer()->Layout();
}

void ConfigurationDialog::OnComboBoxArchUpdated(wxCommandEvent& rEvt)
{
  medusa::Log::Write("wx_ui") << rEvt.GetString() << medusa::LogEnd;

  wxString LdrStr = m_pComboBoxLdr->GetValue();
  medusa::Loader::SPtr spCurLdr;
  BOOST_FOREACH(medusa::Loader::SPtr spLdr, m_rLdrs)
    if (spLdr->GetName() == LdrStr)
    {
      spCurLdr = spLdr;
      break;
    }

  if (spCurLdr == medusa::Loader::SPtr()) return;

  medusa::Architecture::SPtr spCurArch;
  BOOST_FOREACH(medusa::Architecture::SPtr spArch, m_rArchs)
    if (spArch->GetName() == rEvt.GetString())
    {
      spCurArch = spArch;
      break;
    }

  if (spCurArch == medusa::Architecture::SPtr()) return;

  m_Cfg.Clear();

  medusa::ConfigurationModel CfgMdl;
  spCurArch->FillConfigurationModel(CfgMdl);
  spCurLdr->Configure(CfgMdl.GetConfiguration());

  OptionVisitor::ControlList CtrlList;

  OptionVisitor OptVst(this, CfgMdl.GetConfiguration(), CtrlList);
  for (medusa::ConfigurationModel::ConstIterator It = CfgMdl.Begin();
      It != CfgMdl.End(); ++It)
    boost::apply_visitor(OptVst, *It);

  OptVst.FillsControls(m_pBoxSizerOpt);
  GetSizer()->SetSizeHints(this);
  m_Cfg = CfgMdl.GetConfiguration();
}

void ConfigurationDialog::OnComboBoxOptUpdated(wxCommandEvent& rEvt)
{
}

void ConfigurationDialog::OptionVisitor::operator()(medusa::ConfigurationModel::NamedBool const& rBool)
{
  wxCheckBox* pCheckBox = new wxCheckBox(m_pParent, wxID_ANY, rBool.GetName());
  wxBoxSizer* pBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  pBoxSizer->Add(pCheckBox);
  m_rOptCtrlList.push_back(pBoxSizer);
}

void ConfigurationDialog::OptionVisitor::operator()(medusa::ConfigurationModel::NamedEnum const& rEnum)
{
  wxStaticText* pStaticText = new wxStaticText(m_pParent, wxID_ANY, rEnum.GetName());
  wxString Choice = wxEmptyString;

  wxArrayString EnumStrings;
  for (medusa::ConfigurationModel::Enum::const_iterator It = rEnum.GetValue().begin();
      It != rEnum.GetValue().end(); ++It)
  {
    if (It->second == m_rCfg.Get(rEnum.GetName()))
      EnumStrings.Insert(It->first, 0);
    else
      EnumStrings.Add(It->first);
  }

  if (!EnumStrings.IsEmpty())
    Choice = EnumStrings[0];

  wxComboBox* pComboBox = new wxComboBox(
      m_pParent, wxID_ANY,
      Choice, wxDefaultPosition, wxDefaultSize, EnumStrings,
      wxCB_READONLY);

  wxBoxSizer* pBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  pBoxSizer->Add(pStaticText, 0, wxEXPAND | wxALL, 5);
  pBoxSizer->Add(pComboBox, 0, wxEXPAND | wxALL, 5);

  m_rOptCtrlList.push_back(pBoxSizer);
}

void ConfigurationDialog::OptionVisitor::FillsControls(wxBoxSizer* pBoxSizer)
{
  pBoxSizer->Clear(true);
  BOOST_FOREACH(wxSizer* pCtrl, m_rOptCtrlList)
    pBoxSizer->Add(pCtrl, 0, wxEXPAND | wxALL, 0);
}