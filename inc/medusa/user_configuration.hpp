#ifndef __MEDUSA_USER_CONFIGURATION_HPP__
#define __MEDUSA_USER_CONFIGURATION_HPP__

#include <mutex>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/path.hpp>

#include <medusa/namespace.hpp>
#include <medusa/types.hpp>
#include <medusa/export.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT UserConfiguration
{
public:
  UserConfiguration(void);
  ~UserConfiguration(void);

  bool GetOption(std::string const& rKey, std::string& rValue) const;
  bool SetOption(std::string const& rKey, std::string const& rValue);

private:
  bool _WriteDefaultOptions(void);

  typedef std::mutex MutexType;
  mutable MutexType m_Mutex;

  boost::filesystem::path m_CfgPath;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_USER_CONFIGURATION_HPP__
