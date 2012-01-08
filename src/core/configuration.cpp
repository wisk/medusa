#include <iostream>
#include "medusa/configuration.hpp"
#include "medusa/namespace.hpp"
#include "medusa/log.hpp"

MEDUSA_NAMESPACE_USE

u32 Configuration::Get(std::string const& rName) const
{
  NamedValue::const_iterator It = m_NamedValue.find(rName);
  if (It == m_NamedValue.end())
  {
    Log::Write("core") << "Unknown name: " << rName << LogEnd;
    return 0;
  }

  return It->second;
}

bool Configuration::IsSet(std::string const& rName) const
{
  NamedValue::const_iterator It = m_NamedValue.find(rName);
  return It != m_NamedValue.end() ? true : false;
}

void Configuration::Set(std::string const& rName, u32 Val)
{
  m_NamedValue[rName] = Val;
}

void Configuration::Clear(void)
{
  m_NamedValue.erase(m_NamedValue.begin(), m_NamedValue.end());
}

void ConfigurationModel::Set(char const* pName, bool DefaultValue)
{
  m_Values.push_back(NamedBool(pName, DefaultValue));
  if (!m_Cfg.IsSet(pName))
    m_Cfg.Set(pName, DefaultValue);
}

void ConfigurationModel::Set(char const* pName, ConfigurationModel::Enum const& rVal, u32 DefaultValue)
{
  m_Values.push_back(NamedEnum(pName, rVal));
  m_Cfg.Set(pName, DefaultValue);
}