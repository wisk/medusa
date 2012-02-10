#ifndef __WXMEDUSA_CONFIGURATION_DIALOG__
#define __WXMEDUSA_CONFIGURATION_DIALOG__

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/arrstr.h>

#include <medusa/loader.hpp>
#include <medusa/architecture.hpp>
#include <medusa/configuration.hpp>

class ConfigurationDialog : public wxDialog
{
public:
  enum Id
  {
    wxID_CB_LDR,
    wxID_CB_ARCH,
    wxID_CB_OPT
  };

  ConfigurationDialog(
      wxWindow* pParent, wxWindowID Id, wxString const& rTitle,
      wxPoint const& rPos, wxSize const& rSize, long Style,
      medusa::Loader::VectorSPtr const& rLdrs, medusa::Architecture::VectorSPtr const& rArchs);

  medusa::Loader::SPtr GetLoader(void) const;
  medusa::Architecture::SPtr GetArchitecture(void) const;
  medusa::Configuration GetConfiguration(void) const;

private:
  wxComboBox* CreateComboBox(wxWindowID Id, wxArrayString const& rChoices);

  wxComboBox* m_pComboBoxLdr;
  wxComboBox* m_pComboBoxArch;
  wxComboBox* m_pComboBoxOpt;

  medusa::Loader::VectorSPtr const& m_rLdrs;
  medusa::Architecture::VectorSPtr const& m_rArchs;
};

#endif // !__WXMEDUSA_CONFIGURATION_DIALOG__
