#ifndef __MEDUSA_DETAIL_HPP__
#define __MEDUSA_DETAIL_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include <list>
#include <string>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT ValueDetail final
{
};

/* TODO: design type
    BaseMask        = 0x0f,
    BinaryType      = 0x01,
    DecimalType     = 0x02,
    HexadecimalType = 0x03,
    FloatType       = 0x04,
    CharacterType   = 0x05,
    TagType         = 0x06,
    CompositeType   = 0x07,
    ReferenceType   = 0x08,
    RelativeType    = 0x09,
    InstrusiveType  = 0x0a,


    ModifierMask    = 0xf0,
    NotType         = 0x10,
    NegateType      = 0x20,
*/

class StructureDetail final
{
public:

private:
};

class FunctionDetail final
{
public:
  enum Attribute
  {
    kNeverReturn = 1 << 0,
    kStatic = 2 << 0,
  };

  //enum CallingConvention
  //{
  //};

  FunctionDetail(std::string const& rName, std::string const& rReturnType, std::list<std::string> const& rParameters);

  std::string const& GetName(void);
  std::string const& GetReturnType(void);
  std::list<std::string> const& GetParameters(void);

private:
  std::string m_Name;
  std::string m_ReturnType; // LATER: ditto
  std::list<std::string> m_Parameters; // LATER: use a better type than std::string
  // TODO:
  // Attributes
  // Calling convention
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_DETAIL_HPP__
