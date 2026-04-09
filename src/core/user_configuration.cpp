#include "medusa/user_configuration.hpp"
#include "medusa/config.hpp"
#include <filesystem>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>

MEDUSA_NAMESPACE_BEGIN

UserConfiguration::UserConfiguration()
{
  namespace fs = std::filesystem;
  std::filesystem::path HomePath;

#ifdef WIN32
  HomePath = _wgetenv(L"APPDATA");
  if (HomePath.empty())
  {
    std::cerr << "unable to get the %APPDATA% directory\n";
    return;
  }
  HomePath /= "medusa";
#else
  HomePath = getenv("HOME");
  if (HomePath.empty())
  {
    std::cerr << "unable to get $HOME directory\n";
    return;
  }
  HomePath /= ".medusa";
#endif

  if (!fs::exists(HomePath))
    std::filesystem::create_directory(HomePath);

  m_CfgPath = HomePath /= "cfg.ini";

  if (!fs::exists(m_CfgPath))
    if (!_WriteDefaultOptions())
      std::cerr << "unable to write default configuration file\n";
}

UserConfiguration::~UserConfiguration()
{
}

bool UserConfiguration::GetOption(std::string const& rKey, std::string& rValue)
{
  try
  {
    std::lock_guard<MutexType> Lock(m_Mutex);
    auto Data = _ReadIni(m_CfgPath);
    auto it = Data.find(rKey);
    if (it != Data.end())
    {
      rValue = it->second;
      return true;
    }
  }
  catch (std::exception&) {}

  auto const& rDefOpts = _GetDefaultOptions();
  auto const itOpt = rDefOpts.find(rKey);
  if (itOpt == std::end(rDefOpts))
    return false;
  rValue = itOpt->second;
  SetOption(rKey, itOpt->second);
  return true;
}

std::string UserConfiguration::GetOption(std::string const& rKey)
{
  std::string Opt;
  if (!GetOption(rKey, Opt))
    return "";
  return Opt;
}

bool UserConfiguration::SetOption(std::string const& rKey, std::string const& rValue)
{
  try
  {
    std::lock_guard<MutexType> Lock(m_Mutex);
    auto Data = _ReadIni(m_CfgPath);
    Data[rKey] = rValue;
    return _WriteIni(m_CfgPath, Data);
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
    return false;
  }
}

std::unordered_map<std::string, std::string> const& UserConfiguration::_GetDefaultOptions() const
{
  static std::unordered_map<std::string, std::string> s_DefOpts
  {
    { "core.modules_path", MEDUSA_DEFAULT_MODULE_PATH },
    { "core.log_level", "default" },

    { "color.background_listing", "#1e1e1e" },
    { "color.background_address", "#626262" },
    { "color.background_node", "#172B4A" },
    { "color.background_node_begin", "#00368C" },
    { "color.background_node_end", "#3A346B" },
    { "color.instruction_mnemonic", "#9a86d6" },
    { "color.instruction_mnemonic_prefix", "#bd86d6" },
    { "color.instruction_mnemonic_suffix", "#ad5fb8" },
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
    { "action.add_graph_view", "Ctrl+F" },
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
    { "action.symbolic_analyze", "Shift+A" },
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

bool UserConfiguration::_WriteDefaultOptions()
{
  if (std::filesystem::exists(m_CfgPath))
    return false;

  try
  {
    std::lock_guard<MutexType> Lock(m_Mutex);
    auto const& rDefOpts = _GetDefaultOptions();
    std::unordered_map<std::string, std::string> Data(rDefOpts.begin(), rDefOpts.end());
    return _WriteIni(m_CfgPath, Data);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return false;
  }
}

std::unordered_map<std::string, std::string> UserConfiguration::_ReadIni(std::filesystem::path const& rPath)
{
  std::unordered_map<std::string, std::string> Result;
  std::ifstream File(rPath);
  if (!File.is_open())
    return Result;

  std::string Line, Section;
  while (std::getline(File, Line))
  {
    // trim whitespace
    auto start = Line.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) continue;
    Line = Line.substr(start);
    auto end = Line.find_last_not_of(" \t\r\n");
    if (end != std::string::npos) Line.resize(end + 1);

    if (Line.empty() || Line[0] == ';' || Line[0] == '#')
      continue;

    if (Line.front() == '[' && Line.back() == ']')
    {
      Section = Line.substr(1, Line.size() - 2);
      continue;
    }

    auto eq = Line.find('=');
    if (eq == std::string::npos)
      continue;

    auto Key = Line.substr(0, eq);
    auto Val = Line.substr(eq + 1);

    // trim key/value
    while (!Key.empty() && (Key.back() == ' ' || Key.back() == '\t')) Key.pop_back();
    auto vs = Val.find_first_not_of(" \t");
    if (vs != std::string::npos) Val = Val.substr(vs);

    std::string FullKey = Section.empty() ? Key : Section + "." + Key;
    Result[FullKey] = Val;
  }
  return Result;
}

bool UserConfiguration::_WriteIni(std::filesystem::path const& rPath,
  std::unordered_map<std::string, std::string> const& rData)
{
  // Group by section (part before the dot)
  std::map<std::string, std::map<std::string, std::string>> Sections;
  for (auto const& [FullKey, Val] : rData)
  {
    auto dot = FullKey.find('.');
    if (dot != std::string::npos)
      Sections[FullKey.substr(0, dot)][FullKey.substr(dot + 1)] = Val;
    else
      Sections[""][FullKey] = Val;
  }

  std::ofstream File(rPath);
  if (!File.is_open())
    return false;

  for (auto const& [Section, Keys] : Sections)
  {
    if (!Section.empty())
      File << "[" << Section << "]\n";
    for (auto const& [Key, Val] : Keys)
      File << Key << "=" << Val << "\n";
  }
  return true;
}

MEDUSA_NAMESPACE_END
