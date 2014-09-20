#ifndef MEDUSA_DETAIL_HPP
#define MEDUSA_DETAIL_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/binary_stream.hpp"

#include <list>
#include <string>
#include <unordered_map>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Detail
{
public:
  enum Kind
  {
    Unknown,
    Type,
    Value,
    TypedValue,
    Union,
    StaticArray,
    DynamicArray,
    Structure,
    Function,
  };

  typedef std::shared_ptr<Detail> SPType;

  Detail(Kind Kind = Unknown, SPType spParent = nullptr, std::string const& rName = "");
  virtual ~Detail(void) {}

  bool IsValid(void) const { return m_IsValid; }

  Kind   GetKind(void)   const { return m_Kind;  }
  SPType GetParent(void) const { return m_spParent; }

  virtual std::string const& GetName(void) const { return m_Name; }
  Id GetId(void) const { return m_Id; }

  virtual std::string Dump(void) const { return "<unknown>"; }

protected:
  mutable bool m_IsValid;
  std::string  m_Name;

private:
  Kind   m_Kind;
  SPType m_spParent;
  Id     m_Id;
};

class Medusa_EXPORT TypeDetail : public Detail
{
public:
  enum Type
  {
    UnknownType,
    VoidType,
    IntegerType,
    FloatType,
    PointerType,
    ReferenceType,
    ArrayType,
    StructureType,
    ClassType,
    EnumType,
    TypedefType,
  };

  typedef std::shared_ptr<TypeDetail> SPType;

  TypeDetail(std::string const& rName = "", Type Type = UnknownType, u32 BitSize = 0);

  virtual Type        GetType(void) const;
  virtual u32         GetSize(void) const;
  u32                 GetBitSize(void) const;

  virtual std::string Dump(void) const;

protected:
  TypeDetail(Kind Kind, std::string const& rName, Type Type);

private:
  Type        m_Type;
  u8          m_BitSize;
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
class Medusa_EXPORT ValueDetail : public Detail
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

  ValueDetail(std::string const& rName = "", Type ValueType = UnknownType, Id RefId = Id());

  virtual std::string Dump(void)     const;
  Type                GetType(void)  const;
  Id                  GetRefId(void) const;

private:
  Type        m_Type;
  Id          m_RefId;
};

class Medusa_EXPORT TypedValueDetail : public Detail
{
public:
  typedef std::list<TypedValueDetail> List;

  TypedValueDetail(
    std::string const& rTypeName = "", TypeDetail::Type Type = TypeDetail::UnknownType, u8 Size = 0,
    std::string const& rValueName = "", ValueDetail::Type ValueType = ValueDetail::UnknownType, Id RefId = Id());
  TypedValueDetail(TypeDetail::SPType spType, ValueDetail const& rValue);

  virtual u32 GetSize(void) const;
  virtual std::string Dump(void) const;

  TypeDetail::SPType const& GetType(void)  const;
  ValueDetail        const& GetValue(void) const;

protected:
  TypeDetail::SPType m_spType;
  ValueDetail        m_Value;
};

//class Medusa_EXPORT UnionDetail : public Detail
//{
//public:
//  typedef std::function<TypedValueDetail(Detail const* pParentDetail)> SelecterType;
//  UnionDetail(TypedValueDetail::List const& rTypedValues, SelecterType Selecter);
//
//private:
//};

class Medusa_EXPORT StaticArrayDetail : public TypeDetail
{
public:
  StaticArrayDetail(TypeDetail::SPType spElementType, u32 NumberOfElements);

  virtual u32 GetSize(void) const;
  virtual std::string Dump(void) const;

  TypeDetail::SPType const& GetElementType(void) const;
  u32 GetNumberOfElements(void) const;

private:
  TypeDetail::SPType m_spElementType;
  u32 m_NumberOfElements;
};

//class Medusa_EXPORT DynamicArrayDetail : public Detail
//{
//public:
//  typedef std::function<u32(Detail const* pParentDetail)> SizerType;
//  ArrayDetail(TypedValueDetail const& rTypedValue, SizerType Sizer);
//
//private:
//
//};

// TODO: don't copy
// TODO: use shared_ptr
class Medusa_EXPORT StructureDetail : public TypeDetail
{
public:
  StructureDetail(std::string const& rName = "", u32 Alignment = 0);

  virtual u32 GetSize(void) const;
  virtual std::string Dump(void) const;

  bool GetFieldByName(std::string const& rFieldName, TypedValueDetail& rField);
  bool GetFieldByOffset(u32 Offset, TypedValueDetail& rField);

  StructureDetail& AddField(TypeDetail::SPType spFieldType, std::string const& rFieldName, ValueDetail::Type FieldType = ValueDetail::UnknownType, Id FieldRefId = Id());

  void ForEachField(std::function <bool (u32 Offset, TypedValueDetail const& rField)> Callback) const;

private:
  bool _DetermineNextOffset(u32& rNextOffset) const;

  u32 m_Alignment;
  std::unordered_map<std::string, u32> m_NameToOffset;
  std::map<u32, TypedValueDetail> m_OffsetToField;
};

class Medusa_EXPORT FunctionDetail : public Detail
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

  FunctionDetail(std::string const& rName = "",
    TypeDetail const& rReturnType = TypeDetail(),
    TypedValueDetail::List const& rParameters = TypedValueDetail::List());

  virtual std::string                Dump(void)          const;
  TypeDetail                  const& GetReturnType(void) const;
  std::list<TypedValueDetail> const& GetParameters(void) const;

private:
  TypeDetail             m_ReturnType;
  TypedValueDetail::List m_Parameters;
  // TODO:
  // Attributes
  // Calling convention
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_DETAIL_HPP
