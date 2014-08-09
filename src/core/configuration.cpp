#include <iostream>
#include "medusa/configuration.hpp"
#include "medusa/namespace.hpp"
#include "medusa/log.hpp"

#include <boost/foreach.hpp>
#include <sstream>

MEDUSA_NAMESPACE_USE

void ConfigurationModel::InsertBoolean(std::string const& rName, bool DefaultValue)
{
  m_Values[rName] = NamedBool(rName, DefaultValue);
}

void ConfigurationModel::InsertEnum(std::string const& rName, Configuration::Enum const& rVal, u32 DefaultValue)
{
  Configuration::Enum Values = rVal;
  Values.push_back(std::make_pair("", DefaultValue));
  m_Values[rName] = NamedEnum(rName, Values);
}

void ConfigurationModel::SetBoolean(std::string const& rName, bool Value)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  NamedBool* pResult = boost::get<NamedBool>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type bool for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(Value);
}

void ConfigurationModel::SetEnum(std::string const& rName, u32 Value)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  NamedEnum* pResult = boost::get<NamedEnum>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type enum for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(Value);
}

bool ConfigurationModel::IsSet(std::string const& rName) const
{
  return m_Values.find(rName) != std::end(m_Values);
}

bool ConfigurationModel::GetBoolean(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return false;
  }
  NamedBool const* pResult = boost::get<NamedBool>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type bool for option: " << rName << LogEnd;
    return false;
  }
  return pResult->GetValue();
}

u32 ConfigurationModel::GetEnum(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return 0;
  }
  NamedEnum const* pResult = boost::get<NamedEnum>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type enum for option: " << rName << LogEnd;
    return 0;
  }

  return pResult->GetValue();
}

ConfigurationModel& IsConfigurable::GetConfigurationModel(void)
{
  return m_CfgMdl;
}

ConfigurationModel const& IsConfigurable::GetConfigurationModel(void) const
{
  return m_CfgMdl;
}
