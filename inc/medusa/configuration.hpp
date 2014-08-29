#ifndef _MEDUSA_CONFIGURATION_HPP_
#define _MEDUSA_CONFIGURATION_HPP_

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

#include <list>
#include <string>
#include <unordered_map>
#include <boost/variant.hpp>

MEDUSA_NAMESPACE_BEGIN

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

namespace Configuration
{
  typedef std::pair <std::string, u32> NamedField;
  typedef std::list <NamedField      > Enum;

  template<typename ConfigType, typename ValueType>
  class NamedValue
  {
  public:
    NamedValue(std::string const& rName = "", ConfigType Value = ConfigType()) : m_Name(rName), m_Value(Value) {}

    std::string const& GetName(void) const { return m_Name; }
    ValueType GetValue(void) const { return m_Value; }
    ConfigType const& GetConfigurationValue(void) const { return m_Value; }
    void SetValue(ValueType Value) { m_Value = Value; }

  private:
    std::string m_Name;
    ConfigType m_Value;
  };

  template<> class NamedValue<Configuration::Enum, u32>
  {
  public:
    NamedValue(std::string const& rName = "", Configuration::Enum Value = Configuration::Enum()) : m_Name(rName), m_Value(Value) {}

    std::string const& GetName(void) const { return m_Name; }
    Enum const& GetConfigurationValue(void) const { return m_Value; }
    u32 GetValue(void) const
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
class Medusa_EXPORT ConfigurationModel
{
public:
  // Boolean type
  typedef Configuration::NamedValue<bool, bool> NamedBool;

  // Integer type
  typedef Configuration::NamedValue<u8,  u8>  NamedUint8;
  typedef Configuration::NamedValue<u16, u16> NamedUint16;
  typedef Configuration::NamedValue<u32, u32> NamedUint32;
  typedef Configuration::NamedValue<u64, u64> NamedUint64;

  // Enum type
  typedef Configuration::NamedValue<Configuration::Enum, u32> NamedEnum;

  // String type
  typedef Configuration::NamedValue<std::string, std::string> NamedString;

  // Path
  typedef Configuration::NamedValue<Path, Path> NamedPath;

  // Variant
  typedef boost::variant<
    NamedBool,
    NamedUint8, NamedUint16, NamedUint32, NamedUint64,
    NamedEnum, NamedString, NamedPath
  > VariantNamedValue;
  typedef std::unordered_map< std::string, VariantNamedValue> VariantNamedValueMapType;

  typedef VariantNamedValueMapType::const_iterator ConstIterator;

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

  ConstIterator  Begin(void) const { return std::begin(m_Values); }
  ConstIterator  End(void)   const { return std::end(m_Values); }

private:
  VariantNamedValueMapType m_Values;
};

class Medusa_EXPORT IsConfigurable
{
public:
  ConfigurationModel& GetConfigurationModel(void);
  ConfigurationModel const& GetConfigurationModel(void) const;

protected:
  ConfigurationModel m_CfgMdl;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CONFIGURATION_HPP_
