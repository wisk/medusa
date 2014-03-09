#ifndef _MEDUSA_CONFIGURATION_HPP_
#define _MEDUSA_CONFIGURATION_HPP_

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

#include <map>
#include <list>
#include <string>
#include <boost/variant.hpp>

MEDUSA_NAMESPACE_BEGIN

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

//! Configuration class allows to store generically information for one module
class Medusa_EXPORT Configuration
{
public:
  typedef u32 Value;
  typedef std::map<std::string, Value> NamedValue;

  Configuration(void) {}
  ~Configuration(void) {}

  u32         Get(std::string const& rName) const;
  bool        IsSet(std::string const& rName) const;
  void        Set(std::string const& rName, u32 Val);
  void        Clear(void);
  std::string ToString(void) const;

private:
  NamedValue m_NamedValue;
};

//! ConfigurationModel class defines available information to configure both Architecture and Loader.
class Medusa_EXPORT ConfigurationModel
{
public:
  template<typename ValueType>
  class NamedValue
  {
  public:
    NamedValue(char const* pName, ValueType Value) : m_pName(pName), m_Value(Value) {}

    char const* GetName(void) const { return m_pName; }
    ValueType const& GetValue(void) const { return m_Value; }

  private:
    char const* m_pName;
    ValueType m_Value;
  };

  // Boolean type
  typedef NamedValue<bool            > NamedBool;

  // Enum type
  typedef std::pair <char const*, u32> NamedField;
  typedef std::list <NamedField      > Enum;
  typedef NamedValue<Enum            > NamedEnum;

  // Variant
  typedef boost::variant < NamedBool,   NamedEnum > VariantNamedValue;
  typedef std::list      < VariantNamedValue      > VariantNamedValueList;

  typedef VariantNamedValueList::const_iterator ConstIterator;

  void           Set(char const* pName, bool DefaultValue = false);
  void           Set(char const* pName, Enum const& rVal, u32 DefaultValue = 0);
  ConstIterator  Begin(void) const { return m_Values.begin(); }
  ConstIterator  End(void)   const { return m_Values.end();   }

  Configuration& GetConfiguration(void) { return m_Cfg; }
  Configuration const& GetConfiguration(void) const { return m_Cfg; }

private:
  VariantNamedValueList m_Values;
  Configuration         m_Cfg;
};

class Medusa_EXPORT ConfigurationManager
{
public:
  ConfigurationManager(void);
  ~ConfigurationManager(void);

  enum ConfigurationType
  {
    UnknownType,
    DatabaseType,
    LoaderType,
    ArchitectureType,
    OperatingSystemType,
    NumberOfType,
  };

  Configuration* GetConfiguration(ConfigurationType Type);
  Configuration const* GetConfiguration(ConfigurationType Type) const;

private:
  Configuration m_AllCfg[NumberOfType];
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CONFIGURATION_HPP_