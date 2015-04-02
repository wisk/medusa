#include "medusa/user_configuration.hpp"
#include <boost/filesystem.hpp>

MEDUSA_NAMESPACE_BEGIN

UserConfiguration::UserConfiguration(void)
{
  namespace fs = boost::filesystem;
  fs::path HomePath;

#ifdef WIN32
  HomePath = getenv("APPDATA");
  if (HomePath.empty())
  {
    std::cerr << "unable to get the %APPDATA% directory" << std::endl;
    return;
  }
  HomePath /= "medusa";
#else
  HomePath = getenv("HOME");
  if (HomePath.empty())
  {
    std::cerr << "unable to get $HOME directory" << std::endl;
    return;
  }
  HomePath /= ".medusa";
#endif

  if (!fs::exists(HomePath))
    boost::filesystem::create_directory(HomePath);

  m_CfgPath = HomePath /= "cfg.ini";

  if (!fs::exists(m_CfgPath))
    if (!_WriteDefaultOptions())
      std::cerr << "unable to write default configuration file" << std::endl;
}

UserConfiguration::~UserConfiguration(void)
{
}


// TODO: handle return value
bool UserConfiguration::GetOption(std::string const& rKey, std::string& rValue)
{
  try
  {
    std::lock_guard<MutexType> Lock(m_Mutex);
    namespace pt = boost::property_tree;
    pt::ptree PropTree;
    pt::ini_parser::read_ini(m_CfgPath.string(), PropTree);
    rValue = PropTree.get<std::string>(rKey);
    return true;
  }
  catch (std::exception&)
  {
    auto const& rDefOpts = _GetDefaultOptions();
    auto const itOpt = rDefOpts.find(rKey);
    if (itOpt == std::end(rDefOpts))
      return false;
    if (!SetOption(rKey, itOpt->second))
      return false;
    return true;
  }
}

std::string UserConfiguration::GetOption(std::string const& rKey)
{
  std::string Opt;
  if (!GetOption(rKey, Opt))
    return "";
  return Opt;
}

// TODO: handle return value
bool UserConfiguration::SetOption(std::string const& rKey, std::string const& rValue)
{
  try
  {
    std::lock_guard<MutexType> Lock(m_Mutex);
    namespace pt = boost::property_tree;
    pt::ptree PropTree;
    pt::ini_parser::read_ini(m_CfgPath.string(), PropTree);
    PropTree.put(rKey, rValue);
    pt::ini_parser::write_ini(m_CfgPath.string(), PropTree);
    return true;
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << std::endl;
    return false;
  }
}

std::unordered_map<std::string, std::string> const& UserConfiguration::_GetDefaultOptions(void) const
{
  static std::unordered_map<std::string, std::string> s_DefOpts
  {
    { "core.modules_path", "." },
    { "core.log_level", "default" },

    { "color.background_listing", "#1e1e1e" },
    { "color.background_address", "#626262" },
    { "color.background_node", "#172B4A" },
    { "color.background_node_begin", "#00368C" },
    { "color.background_node_end", "#3A346B" },
    { "color.instruction_mnemonic", "#9a86d6" },
    { "color.instruction_register", "#00aa7f" },
    { "color.instruction_immediate", "#ffaa00" },
    { "color.comment", "#55aa00" },
    { "color.selection", "#760000" },
    { "color.operator", "#ff0000" },
    { "color.keyword", "#55aaff" },
    { "color.label", "#aaaa7f" },
    { "color.string", "#bd72ff" },

    { "action.add_disassembly_view", "Ctrl+D" },
    { "action.add_semantic_view", "Ctrl+S" },
    { "action.add_control_flow_graph_view", "Ctrl+F" },
    { "action.show_label_dialog", "N" },
    { "action.show_comment_dialog", "," },
    { "action.change_value_size", "S, V" },
    { "action.to_character", "S, C" },
    { "action.to_reference", "S, R" },
    { "action.to_binary", "S, B" },
    { "action.to_decimal", "S, D" },
    { "action.to_hexadecimal", "S, H" },
    { "action.not_value", "~" },
    { "action.negate_value", "-" },
    { "action.reset_value", "Del" },
    { "action.undefine", "U" },
    { "action.to_word", "D, W" },
    { "action.to_dword", "D, D" },
    { "action.to_qword", "D, Q" },
    { "action.analyze", "A" },
    { "action.create_function", "F" },
    { "action.to_utf8_string", "D, S, 1" },
    { "action.to_utf16_name", "D, S, 2" },
    { "action.go_to_previous_address", "Alt+Left" },
    { "action.go_to_next_address", "Alt+Right" },

    { "font.listing", "DejaVu Sans Mono,10,-1,5,50,0,0,0,0,0" },
  };

  return s_DefOpts;
}

bool UserConfiguration::_WriteDefaultOption(std::string const& rKey)
{
  auto const& rDefOpts = _GetDefaultOptions();
  auto const itOpt = rDefOpts.find(rKey);
  if (itOpt == std::end(rDefOpts))
    return false;
  return SetOption(rKey, itOpt->second);
}

bool UserConfiguration::_WriteDefaultOptions(void)
{
  if (boost::filesystem::exists(m_CfgPath))
    return false;

  try
  {
    std::lock_guard<MutexType> Lock(m_Mutex);
    namespace pt = boost::property_tree;
    pt::ptree PropTree;

    auto const& rDefOpts = _GetDefaultOptions();
    for (auto const& rOptPair : rDefOpts)
      PropTree.put(rOptPair.first, rOptPair.second);

    pt::ini_parser::write_ini(m_CfgPath.string(), PropTree);
    return true;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

MEDUSA_NAMESPACE_END
