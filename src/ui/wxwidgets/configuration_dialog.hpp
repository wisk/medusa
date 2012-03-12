#ifndef __WXMEDUSA_CONFIGURATION_DIALOG__
#define __WXMEDUSA_CONFIGURATION_DIALOG__

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/arrstr.h>

#include <medusa/log.hpp>
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

  medusa::Loader::SPtr       GetLoader(void) const;
  medusa::Architecture::SPtr GetArchitecture(void) const;
  medusa::Configuration      GetConfiguration(void) const;

protected:
  DECLARE_EVENT_TABLE()

private:
  wxComboBox* CreateComboBox(wxWindowID Id, wxArrayString const& rChoices);

  void OnComboBoxLdrUpdated(wxCommandEvent& rEvt);
  void OnComboBoxArchUpdated(wxCommandEvent& rEvt);
  void OnComboBoxOptUpdated(wxCommandEvent& rEvt);

  wxComboBox* m_pComboBoxLdr;
  wxComboBox* m_pComboBoxArch;
  wxBoxSizer* m_pBoxSizerOpt;

  medusa::Loader::VectorSPtr const& m_rLdrs;
  medusa::Architecture::VectorSPtr const& m_rArchs;
  medusa::Configuration m_Cfg;

  class OptionVisitor : public boost::static_visitor<>
  {
  public:
    typedef std::list<wxSizer*> ControlList;

    OptionVisitor(wxWindow* pParent, medusa::Configuration& rCfg, ControlList& rOptCtrlList)
      : m_pParent(pParent), m_rCfg(rCfg), m_rOptCtrlList(rOptCtrlList) {}

    void operator()(medusa::ConfigurationModel::NamedBool const& rBool);
    void operator()(medusa::ConfigurationModel::NamedEnum const& rEnum);

    void FillsControls(wxBoxSizer* pBoxSizer);

  private:
    wxWindow* m_pParent;
    medusa::Configuration& m_rCfg;
    ControlList& m_rOptCtrlList;
  };
};

#endif // !__WXMEDUSA_CONFIGURATION_DIALOG__