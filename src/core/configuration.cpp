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

void ConfigurationModel::InsertUint8(std::string const& rName, u8 DefaultValue)
{
  m_Values[rName] = NamedUint8(rName, DefaultValue);
}

void ConfigurationModel::InsertUint16(std::string const& rName, u16 DefaultValue)
{
  m_Values[rName] = NamedUint16(rName, DefaultValue);
}

void ConfigurationModel::InsertUint32(std::string const& rName, u32 DefaultValue)
{
  m_Values[rName] = NamedUint32(rName, DefaultValue);
}

void ConfigurationModel::InsertUint64(std::string const& rName, u64 DefaultValue)
{
  m_Values[rName] = NamedUint64(rName, DefaultValue);
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

void ConfigurationModel::SetUint8(std::string const& rName, u8 Value)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  auto pResult = boost::get<NamedUint8>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint8 for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(Value);
}

void ConfigurationModel::SetUint16(std::string const& rName, u16 Value)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  auto pResult = boost::get<NamedUint16>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint16 for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(Value);
}

void ConfigurationModel::SetUint32(std::string const& rName, u32 Value)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  auto pResult = boost::get<NamedUint32>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint32 for option: " << rName << LogEnd;
    return;
  }

  pResult->SetValue(Value);
}

void ConfigurationModel::SetUint64(std::string const& rName, u64 Value)
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return;
  }
  auto pResult = boost::get<NamedUint64>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint64 for option: " << rName << LogEnd;
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
    return bool();
  }
  auto const pResult = boost::get<NamedBool>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type bool for option: " << rName << LogEnd;
    return bool();
  }
  return pResult->GetValue();
}

u8 ConfigurationModel::GetUint8(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return u8();
  }
  auto const pResult = boost::get<NamedUint8>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint8 for option: " << rName << LogEnd;
    return u8();
  }
  return pResult->GetValue();
}

u16 ConfigurationModel::GetUint16(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return u16();
  }
  auto const pResult = boost::get<NamedUint16>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint16 for option: " << rName << LogEnd;
    return u16();
  }
  return pResult->GetValue();
}

u32 ConfigurationModel::GetUint32(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return u32();
  }
  auto const pResult = boost::get<NamedUint32>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint32 for option: " << rName << LogEnd;
    return u32();
  }
  return pResult->GetValue();
}

u64 ConfigurationModel::GetUint64(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return u64();
  }
  auto const pResult = boost::get<NamedUint64>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type uint64 for option: " << rName << LogEnd;
    return u64();
  }
  return pResult->GetValue();
}

u32 ConfigurationModel::GetEnum(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return u32();
  }
  auto const pResult = boost::get<NamedEnum>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type enum for option: " << rName << LogEnd;
    return u32();
  }

  return pResult->GetValue();
}

std::string ConfigurationModel::GetString(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return std::string();
  }
  auto const pResult = boost::get<NamedString>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type string for option: " << rName << LogEnd;
    return std::string();
  }

  return pResult->GetValue();
}

Path ConfigurationModel::GetPath(std::string const& rName) const
{
  auto itValue = m_Values.find(rName);
  if (itValue == std::end(m_Values))
  {
    Log::Write("core") << "unable to find option: " << rName << LogEnd;
    return Path();
  }
  auto const pResult = boost::get<NamedPath>(&itValue->second);
  if (pResult == nullptr)
  {
    Log::Write("core") << "invalid type path for option: " << rName << LogEnd;
    return Path();
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
