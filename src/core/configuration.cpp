#include <iostream>
#include "medusa/configuration.hpp"
#include "medusa/namespace.hpp"

MEDUSA_NAMESPACE_USE

u32 Configuration::Get(std::string const& rName) const
{
  NamedValue::const_iterator It = m_NamedValue.find(rName);
  if (It == m_NamedValue.end())
  {
    std::cerr << "Unknown name: " << rName << std::endl;
    return 0;
  }

  return It->second;
}

void Configuration::Set(std::string const& rName, u32 Val)
{
  m_NamedValue[rName] = Val;
}

void ConfigurationModel::Set(char const* pName, bool DefaultValue)
{
  m_Values.push_back(NamedBool(pName, DefaultValue));
  m_Cfg.Set(pName, DefaultValue);
}

void ConfigurationModel::Set(char const* pName, ConfigurationModel::Enum const& rVal, u32 DefaultValue)
{
  m_Values.push_back(NamedEnum(pName, rVal));
  m_Cfg.Set(pName, DefaultValue);
}
