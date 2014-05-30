#ifndef __MEDUSA_DETAIL_HPP__
#define __MEDUSA_DETAIL_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include <list>
#include <string>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT TypeDetail
{
public:
  enum Type
  {
    UnknownType,
    IntegerType,
    FloatType,
    PointerType,
    ReferenceType,
    StructureType,
    ClassType,
    EnumType,
  };

  TypeDetail(std::string const& rName = "", Type Type = UnknownType, u8 Size = 0);

private:
  std::string m_Name;
  Type m_Type;
  u8 m_Size; //! in bit
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
class Medusa_EXPORT ValueDetail
{
public:
  /*!
  **
  ** Value type:
  **  - {s,u}{8,16,32,64}
  **  - float not implemented
  **  - composite        → include an another element
  **  - reference        → pointer to an another element
  **  - relative         → the another element is located at the beginning of struct +
  **  field value
  **  - intrusive        → the another element is located at field value - offsetof field
  **  - constant         → enum / define value
  **  - structure offset → related to a structure
  **
  */
  enum Type
  {
    UnknownType     = 0x00,

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
    ConstantType    = 0x0b,
    StructureOffsetType = 0x0c,

    ModifierMask    = 0xf0,
    NotType         = 0x10, //! ~ operator
    NegateType      = 0x20, //! - operator
    ShiftType       = 0x30, //! 1 << operator

    DefaultType     = HexadecimalType,
  };

  ValueDetail(std::string const& rName = "", Id ValueId = Id(), Type ValueType = UnknownType, Id RefId = Id());

private:
  std::string m_Name;
  Id m_Id;
  Type m_Type;
  Id m_RefId;
};

class TypedValueDetail : public TypeDetail, ValueDetail
{
public:
  typedef std::list<TypedValueDetail> List;

  TypedValueDetail(
    std::string const& rTypeName = "", TypeDetail::Type TypeType = TypeDetail::UnknownType, u8 TypeSize = 0,
    std::string const& rValueName = "", Id ValueId = Id(), ValueDetail::Type ValueType = ValueDetail::UnknownType, Id RefId = Id());

private:
};

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

  //FunctionDetail(std::string const& rName = "", TypeDetail const& rReturnType = TypeDetail(), TypedValueDetail::List const& rParameters = TypedValueDetail::List());

  std::string const& GetName(void);
  TypeDetail const& GetReturnType(void);
  std::list<TypedValueDetail> const& GetParameters(void);

private:
  std::string m_Name;
  TypeDetail m_ReturnType;
  TypedValueDetail::List m_Parameters;
  // TODO:
  // Attributes
  // Calling convention
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_DETAIL_HPP__
