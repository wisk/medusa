#include "configuration_dialog.hpp"

#include <boost/foreach.hpp>

ConfigurationDialog::ConfigurationDialog(
    wxWindow* pParent, wxWindowID Id, wxString const& rTitle,
    wxPoint const& rPos, wxSize const& rSize, long Style,
    medusa::Loader::VectorSPtr const& rLdrs, medusa::Architecture::VectorSPtr const& rArchs)
  : wxDialog(pParent, Id, rTitle, rPos, rSize, Style)
  , m_rLdrs(rLdrs), m_rArchs(rArchs)
{
  wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);

  wxArrayString LdrNames;
  wxArrayString ArchNames;

  BOOST_FOREACH(medusa::Loader::SPtr Ldr, m_rLdrs)
    LdrNames.Add(Ldr->GetName());

  medusa::Architecture::SPtr pBestArch = m_rLdrs[0]->GetMainArchitecture(m_rArchs);

  BOOST_FOREACH(medusa::Architecture::SPtr Arch, m_rArchs)
  {
    if (pBestArch == Arch)
      ArchNames.Insert(pBestArch->GetName(), 0);
    else
      ArchNames.Add(Arch->GetName());
  }

  m_pComboBoxLdr  = CreateComboBox(wxID_CB_LDR,  LdrNames);
  m_pComboBoxArch = CreateComboBox(wxID_CB_ARCH, ArchNames);

  pSizer->Add(m_pComboBoxLdr,  0, wxEXPAND | wxLEFT | wxRIGHT, 5);
  pSizer->Add(m_pComboBoxArch, 1, wxEXPAND | wxLEFT | wxRIGHT, 5);

  wxBoxSizer* pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  pButtonSizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxALIGN_LEFT, 5);
  pButtonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 1, wxALIGN_RIGHT, 5);

  pSizer->Add(pButtonSizer);
  pSizer->SetSizeHints(this);
  SetSizer(pSizer);
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
  medusa::ConfigurationModel CfgMdl;

  medusa::Loader::SPtr spLdr = GetLoader();
  medusa::Architecture::SPtr spArch = GetArchitecture();

  spArch->FillConfigurationModel(CfgMdl);
  spLdr->Configure(CfgMdl.GetConfiguration());
  spArch->UseConfiguration(CfgMdl.GetConfiguration());

  return CfgMdl.GetConfiguration();
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
