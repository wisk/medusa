#ifndef _MEDUSA_MULTICELL_
#define _MEDUSA_MULTICELL_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/serialize.hpp"
#include "medusa/address.hpp"

#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

MEDUSA_NAMESPACE_BEGIN

//! MultiCell is a group of cell.
class Medusa_EXPORT MultiCell : public SerializeAccess
{
public:
  typedef std::map<Address, MultiCell*> Map;

  enum Type
  {
    UnknownType,
    FunctionType,
    StringType,
    StructType,
    ArrayType
  };

  MultiCell(u8 Type = UnknownType, u16 Size = 0x0)
    : m_Type(Type)
    , m_Size(Size)
  {}

  //! This method returns the size of multicell.
  u16 GetSize(void) const { return m_Size; }

  //! This method returns the multicell type.
  u8 GetType(void) const { return m_Type; }

  //! This method returns the string form.
  std::string const& ToString(void) const { return m_Buffer; }

  //! This method allows to update the string form.
  void UpdateString(std::string const& Buffer) { m_Buffer = Buffer; }

  //! This method tells if the ui have to display cell contained in multicell.
  virtual bool DisplayCell(void) const { return false; }

  virtual void                  Load(SerializeEntity::SPtr spSrlzEtt);
  virtual SerializeEntity::SPtr Save(void);

protected:
  u8          m_Type;
  u16         m_Size;
  std::string m_Buffer;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_MULTICELL_