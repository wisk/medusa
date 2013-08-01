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
    Unknown    = 0x0000,     //! Undefined type.
    Data       = 0x0001,   //! This label contains data.
    Code       = 0x0002,   //! This label contains code.
    String     = 0x0003,   //! This label contains a string.
    CellMask   = 0x000f,
    Imported   = 0x0010,  //! This label is imported.
    Exported   = 0x0020,  //! This label is exported.
    SourceMask = 0x00f0,
    Global     = 0x100, //! This label is global.
    Local      = 0x200,  //! This label is local.
    AccessMask = 0x0f00
  };

  Label(std::string const& rName, u16 Type)
    : m_Name(rName)
    , m_Prefix("")
    , m_Type(Type)
  {}

  Label(std::string const& rName = "", std::string const& rPrefix = "", u16 Type = Unknown)
    : m_Name(rName)
    , m_Prefix(rPrefix)
    , m_Type(Type)
  {}

  std::string const&  GetName(void) const { return m_Name; }
  std::string         GetLabel(void) const;
  void                SetName(std::string const& rName) { m_Name = rName; }
  u16                 GetType(void) const { return m_Type; }
  void                SetType(u16 Type) { m_Type = Type; }

  bool operator<(Label const& rLabel) const
  {
    return m_Name < rLabel.m_Name;
  }

protected:
  static char ConvertToLabel(char c);

  std::string m_Name;
  std::string m_Prefix;
  u16         m_Type;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_LABEL_HPP__