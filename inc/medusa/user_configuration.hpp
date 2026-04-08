#ifndef MEDUSA_USER_CONFIGURATION_HPP
#define MEDUSA_USER_CONFIGURATION_HPP

#include <mutex>
#include <unordered_map>
#include <filesystem>
#include <string>

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

  static std::unordered_map<std::string, std::string> _ReadIni(std::filesystem::path const& rPath);
  static bool _WriteIni(std::filesystem::path const& rPath, std::unordered_map<std::string, std::string> const& rData);

  using MutexType = std::mutex;
  mutable MutexType m_Mutex;

  std::filesystem::path m_CfgPath;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_USER_CONFIGURATION_HPP
