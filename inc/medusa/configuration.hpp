#ifndef _MEDUSA_CONFIGURATION_HPP_
#define _MEDUSA_CONFIGURATION_HPP_

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

#include <unordered_map>
#include <string>
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
      for (auto itVal = std::begin(m_Value), itEnd = std::end(m_Value); itVal != itEnd; ++itVal)
        if (itVal->first == "")
          return itVal->second;
      return 0;
    }

    void SetValue(u32 Value)
    {
      for (auto itVal = std::begin(m_Value), itEnd = std::end(m_Value); itVal != itEnd; ++itVal)
      {
        if (itVal->first == "")
        {
          itVal->second = Value;
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

  // Enum type
  typedef Configuration::NamedValue<Configuration::Enum, u32> NamedEnum;

  // Variant
  typedef boost::variant    < NamedBool,   NamedEnum        > VariantNamedValue;
  typedef std::unordered_map< std::string, VariantNamedValue> VariantNamedValueMapType;

  typedef VariantNamedValueMapType::const_iterator ConstIterator;

  void           InsertBoolean(std::string const& rName, bool DefaultValue = false);
  void           InsertEnum(std::string const& rName, Configuration::Enum const& rVal, u32 DefaultValue = 0);

  void           SetBoolean(std::string const& rName, bool Value = false);
  void           SetEnum(std::string const& rName, u32 Value = 0);

  bool           IsSet(std::string const& rName) const;

  bool           GetBoolean(std::string const& rName) const;
  u32            GetEnum(std::string const& rName) const;

  ConstIterator  Begin(void) const { return std::begin(m_Values); }
  ConstIterator  End(void)   const { return std::end(m_Values); }

private:
  VariantNamedValueMapType m_Values;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CONFIGURATION_HPP_