#ifndef __MEDUSA_LABEL_HPP__
#define __MEDUSA_LABEL_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include <string>
#include <cstring>
#include <memory>

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
    Unknown    = 0x0000, //! Undefined type.
    Data       = 0x0001, //! This label contains data.
    Code       = 0x0002, //! This label contains code.
    String     = 0x0003, //! This label contains a string.
    CellMask   = 0x000f,
    Imported   = 0x0010, //! This label is imported.
    Exported   = 0x0020, //! This label is exported.
    SourceMask = 0x00f0,
    Global     = 0x0100, //! This label is global.
    Local      = 0x0200, //! This label is local.
    AccessMask = 0x0f00
  };

  Label(std::string const& rName = "", u16 Type = Unknown);
  ~Label(void);

  std::string Dump(void) const;

  std::string GetName(void) const { return m_spName.get(); }
  u16         GetType(void) const { return m_Type; }
  void        SetType(u16 Type) { m_Type = Type; }
  std::string GetLabel(void) const;

  bool operator<(Label const& rLabel) const
  {
    return strcmp(m_spName.get(), rLabel.m_spName.get()) < 0;
  }

  bool operator==(Label const& rLabel) const
  {
    return !strcmp(m_spName.get(), rLabel.m_spName.get()) && m_Type == rLabel.m_Type;
  }

  bool operator!=(Label const& rLabel) const
  {
    return !(*this == rLabel);
  }

protected:
  static char ConvertToLabel(char c);

  std::shared_ptr<char> m_spName;
  u16                   m_NameLength;
  u16                   m_Type;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_LABEL_HPP__