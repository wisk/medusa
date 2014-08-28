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

void ConfigurationModel::InsertString(std::string const& rName, std::string const& rDefaultValue)
{
  m_Values[rName] = NamedString(rName, rDefaultValue);
}

void ConfigurationModel::InsertPath(std::string const& rName, Path const& rDefaultValue)
{
  m_Values[rName] = NamedPath(rName, rDefaultValue);
}

void ConfigurationModel::SetBoolean(std::string const& rName, bool Value)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  auto pResult = boost::get<NamedBool>(&itValue->second);
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
  auto pResult = boost::get<NamedEnum>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type enum for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(Value);
}

void ConfigurationModel::SetString(std::string const& rName, std::string const& rValue)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  auto pResult = boost::get<NamedString>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type string for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(rValue);
}

void ConfigurationModel::SetPath(std::string const& rName, Path const& rValue)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  auto pResult = boost::get<NamedPath>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type path for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(rValue);
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
  auto const pResult = boost::get<NamedBool>(&itValue->second);
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
  auto const pResult = boost::get<NamedEnum>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type enum for option: " << rName << LogEnd;
    return 0;
  }

  return pResult->GetValue();
}

std::string const& ConfigurationModel::GetString(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return 0;
  }
  auto const pResult = boost::get<NamedString>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type string for option: " << rName << LogEnd;
    return 0;
  }

  return pResult->GetValue();
}

Path const& ConfigurationModel::GetPath(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return 0;
  }
  auto const pResult = boost::get<NamedPath>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type path for option: " << rName << LogEnd;
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
