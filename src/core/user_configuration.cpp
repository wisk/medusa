#include "medusa/user_configuration.hpp"
#include "medusa/log.hpp"
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
    Log::Write("core") << "unable to get the %APPDATA% directory" << LogEnd;
    return;
  }
  HomePath /= "medusa";
#else
  HomePath = getenv("HOME");
  if (HomePath.empty())
  {
    Log::Write("core") << "unable to get $HOME directory" << LogEnd;
    return;
  }
  HomePath /= ".medusa";
#endif

  if (!fs::exists(HomePath))
    boost::filesystem::create_directory(HomePath);

  m_CfgPath = HomePath /= "cfg.ini";

  if (!fs::exists(m_CfgPath))
    if (!_WriteDefaultOptions())
      Log::Write("core") << "unable to write default configuration file" << LogEnd;
}

UserConfiguration::~UserConfiguration(void)
{
}


// TODO: handle return value
bool UserConfiguration::GetOption(std::string const& rKey, std::string& rValue) const
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
  catch (std::exception& e)
  {
    Log::Write("core") << "exception: " << e.what() << LogEnd;
    return false;
  }
}

std::string UserConfiguration::GetOption(std::string const& rKey) const
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
    Log::Write("core") << "exception: " << e.what() << LogEnd;
    return false;
  }
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

    PropTree.put("core.modules_path", ".");

    PropTree.put("color.background_listing", "#1e1e1e");
    PropTree.put("color.background_address", "#626262");
    PropTree.put("color.background_node", "#172B4A");
    PropTree.put("color.background_node_begin", "#00368C");
    PropTree.put("color.background_node_end", "#3A346B");
    PropTree.put("color.instruction_mnemonic", "#9a86d6");
    PropTree.put("color.instruction_register", "#00aa7f");
    PropTree.put("color.instruction_immediate", "#ffaa00");
    PropTree.put("color.comment", "#55aa00");
    PropTree.put("color.selection", "#760000");
    PropTree.put("color.operator", "#ff0000");
    PropTree.put("color.keyword", "#55aaff");
    PropTree.put("color.label", "#aaaa7f");
    PropTree.put("color.string", "#bd72ff");

    PropTree.put("action.add_disassembly_view", "Ctrl+D");
    PropTree.put("action.add_semantic_view", "Ctrl+S");
    PropTree.put("action.add_control_flow_graph_view", "Ctrl+F");
    PropTree.put("action.show_label_dialog", "N");
    PropTree.put("action.show_comment_dialog", ";");
    PropTree.put("action.change_value_size", "S, V");
    PropTree.put("action.to_character", "S, C");
    PropTree.put("action.to_reference", "S, R");
    PropTree.put("action.to_binary", "S, B");
    PropTree.put("action.to_decimal", "S, D");
    PropTree.put("action.to_hexadecimal", "S, H");
    PropTree.put("action.not_value", "~");
    PropTree.put("action.negate_value", "-");
    PropTree.put("action.reset_value", "Del");
    PropTree.put("action.undefine", "U");
    PropTree.put("action.to_word", "D, W");
    PropTree.put("action.to_dword", "D, D");
    PropTree.put("action.to_qword", "D, Q");
    PropTree.put("action.analyze", "A");
    PropTree.put("action.create_function", "F");
    PropTree.put("action.to_utf8_string", "D, S, 1");
    PropTree.put("action.to_utf16_name", "D, S, 2");
    PropTree.put("action.go_to_previous_address", "Alt+Left");
    PropTree.put("action.go_to_next_address", "Alt+Right");

    PropTree.put("font.listing", "DejaVu Sans Mono,10,-1,5,50,0,0,0,0,0");

    pt::ini_parser::write_ini(m_CfgPath.string(), PropTree);
    return true;
  }
  catch (std::exception& e)
  {
    Log::Write("core") << e.what() << LogEnd;
    return false;
  }
}

MEDUSA_NAMESPACE_END
