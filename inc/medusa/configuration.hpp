#ifndef MEDUSA_CONFIGURATION_HPP
#define MEDUSA_CONFIGURATION_HPP

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

#include <list>
#include <string>
#include <unordered_map>
#include <variant>

MEDUSA_NAMESPACE_BEGIN

namespace Configuration
{
  using NamedField = std::pair<std::string, u32>;
  using Enum       = std::list<NamedField>;

  template<typename ConfigType, typename ValueType>
  class NamedValue
  {
  public:
    NamedValue(std::string const& rName = "", ConfigType Value = ConfigType()) : m_Name(rName), m_Value(Value) {}

    std::string const& GetName() const { return m_Name; }
    ValueType GetValue() const { return m_Value; }
    ConfigType const& GetConfigurationValue() const { return m_Value; }
    void SetValue(ValueType Value) { m_Value = Value; }

  private:
    std::string m_Name;
    ConfigType m_Value;
  };

  template<> class NamedValue<Configuration::Enum, u32>
  {
  public:
    NamedValue(std::string const& rName = "", Configuration::Enum Value = Configuration::Enum()) : m_Name(rName), m_Value(Value) {}

    std::string const& GetName() const { return m_Name; }
    Enum const& GetConfigurationValue() const { return m_Value; }
    u32 GetValue() const
    {
      for (auto ValuePair : m_Value)
        if (ValuePair.first == "")
          return ValuePair.second;
      return 0;
    }

    void SetValue(u32 Value)
    {
      for (auto ValuePair : m_Value)
      {
        if (ValuePair.first == "")
        {
          ValuePair.second = Value;
          return;
        }
      }
    }

  private:
    std::string m_Name;
    Enum m_Value;
  };
}

//! ConfigurationModel class defines available information to configure both Architecture and Loader.
class MEDUSA_EXPORT ConfigurationModel
{
public:
  // Boolean type
  using NamedBool = Configuration::NamedValue<bool, bool>;

  // Integer type
  using NamedUint8  = Configuration::NamedValue<u8,  u8>;
  using NamedUint16 = Configuration::NamedValue<u16, u16>;
  using NamedUint32 = Configuration::NamedValue<u32, u32>;
  using NamedUint64 = Configuration::NamedValue<u64, u64>;

  // Enum type
  using NamedEnum = Configuration::NamedValue<Configuration::Enum, u32>;

  // String type
  using NamedString = Configuration::NamedValue<std::string, std::string>;

  // Path
  using NamedPath = Configuration::NamedValue<Path, Path>;

  // Variant
  using VariantNamedValue = std::variant<
    NamedBool,
    NamedUint8, NamedUint16, NamedUint32, NamedUint64,
    NamedEnum, NamedString, NamedPath
  >;
  using VariantNamedValueMapType = std::unordered_map<std::string, VariantNamedValue>;

  using ConstIterator = VariantNamedValueMapType::const_iterator;

  void InsertBoolean(std::string const& rName, bool DefaultValue = false);
  void InsertUint8(std::string const& rName, u8 DefaultValue = 0);
  void InsertUint16(std::string const& rName, u16 DefaultValue = 0);
  void InsertUint32(std::string const& rName, u32 DefaultValue = 0);
  void InsertUint64(std::string const& rName, u64 DefaultValue = 0);
  void InsertEnum(std::string const& rName, Configuration::Enum const& rValue, u32 DefaultValue = 0);
  void InsertString(std::string const& rName, std::string const& rDefaultValue = "");
  void InsertPath(std::string const& rName, Path const& rDefaultValue = "");

  void SetBoolean(std::string const& rName, bool Value = false);
  void SetUint8(std::string const& rName, u8 Value = 0);
  void SetUint16(std::string const& rName, u16 Value = 0);
  void SetUint32(std::string const& rName, u32 Value = 0);
  void SetUint64(std::string const& rName, u64 Value = 0);
  void SetEnum(std::string const& rName, u32 Value = 0);
  void SetString(std::string const& rName, std::string const& Value = "");
  void SetPath(std::string const& rName, Path const& rValue = "");

  bool IsSet(std::string const& rName) const;

  bool        GetBoolean(std::string const& rName) const;
  u8          GetUint8(std::string const& rName) const;
  u16         GetUint16(std::string const& rName) const;
  u32         GetUint32(std::string const& rName) const;
  u64         GetUint64(std::string const& rName) const;
  u32         GetEnum(std::string const& rName) const;
  std::string GetString(std::string const& rName) const;
  Path        GetPath(std::string const& rName) const;

  ConstIterator  Begin() const { return std::begin(m_Values); }
  ConstIterator  End()   const { return std::end(m_Values); }

private:
  VariantNamedValueMapType m_Values;
};

class MEDUSA_EXPORT IsConfigurable
{
public:
  ConfigurationModel& GetConfigurationModel();
  ConfigurationModel const& GetConfigurationModel() const;

protected:
  ConfigurationModel m_CfgMdl;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CONFIGURATION_HPP
