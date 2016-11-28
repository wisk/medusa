#ifndef MEDUSA_USER_CONFIGURATION_HPP
#define MEDUSA_USER_CONFIGURATION_HPP

#include <mutex>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/path.hpp>

#include <medusa/namespace.hpp>
#include <medusa/types.hpp>
#include <medusa/export.hpp>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT UserConfiguration
{
public:
  UserConfiguration(void);
  ~UserConfiguration(void);

  bool GetOption(std::string const& rKey, std::string& rValue);
  std::string GetOption(std::string const& rKey);
  bool SetOption(std::string const& rKey, std::string const& rValue);

private:
  std::unordered_map<std::string, std::string> const& _GetDefaultOptions(void) const;

  bool _WriteDefaultOption(std::string const& rKey);
  bool _WriteDefaultOptions(void);

  typedef std::mutex MutexType;
  mutable MutexType m_Mutex;

  boost::filesystem::path m_CfgPath;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_USER_CONFIGURATION_HPP
