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
    pt::ini_parser::write_ini(m_CfgPath.string(), PropTree);
    return true;
  }
  catch (std::exception& e)
  {
    Log::Write("core") << "exception: " << e.what() << LogEnd;
    return false;
  }
}

MEDUSA_NAMESPACE_END
