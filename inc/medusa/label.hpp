#ifndef __MEDUSA_LABEL_HPP__
#define __MEDUSA_LABEL_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include <string>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Label is a string associated with an Address.
class Medusa_EXPORT Label
{
public:
  enum Type
  {
    LabelUnknown  = 0,    //! Undefined type.
    LabelData     = 0x1,  //! This label contains data.
    LabelCode     = 0x2,  //! This label contains code.
    LabelString   = 0x3,  //! This label contains a string.
    LabelImported = 0x10, //! This label is imported.
    LabelExported = 0x20  //! This label is exported.
  };

  Label(std::string const& rName, u8 Type)
    : m_Name(rName)
    , m_Prefix("")
    , m_Type(Type)
  {}

  Label(std::string const& rName, std::string const& rPrefix, u8 Type)
    : m_Name(rName)
    , m_Prefix(rPrefix)
    , m_Type(Type)
  {}

  std::string const&  GetName(void) const { return m_Name; }
  std::string         GetLabel(void);
  void                SetName(std::string const& rName) { m_Name = rName; }
  u8                  GetType(void) const { return m_Type; }
  void                SetType(u8 Type) { m_Type = Type; }

  bool operator<(Label const& rLabel) const
  {
    return m_Name < rLabel.m_Name;
  }

protected:
  static char ConvertToLabel(char c);

  std::string m_Name;
  std::string m_Prefix;
  u8          m_Type;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_LABEL_HPP__