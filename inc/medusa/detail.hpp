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
  enum DetailKind
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

  Detail(DetailKind Kind) : m_Kind(Kind), m_IsValid(true) {}
  virtual ~Detail(void) {}

  DetailKind GetKind(void) const { return m_Kind;  }
  bool IsValid(void) const { return m_IsValid; }
  virtual std::string GetName(void) const { return ""; }
  virtual u32         GetSize(void) const { return 0;  }

  virtual std::string Dump(void) const { return "<unknown>"; }

protected:
  mutable bool m_IsValid;

private:
  DetailKind m_Kind;
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
    StructureType,
    ClassType,
    EnumType,
    TypedefType,
  };

  TypeDetail(std::string const& rName = "", Type Type = UnknownType, u8 BitSize = 0);

  virtual std::string GetName(void) const;
  virtual Type        GetType(void) const;
  virtual u32         GetSize(void) const;
  u32                 GetBitSize(void) const;

  virtual std::string Dump(void) const;

private:
  std::string m_Name;
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

  ValueDetail(std::string const& rName = "", Id ValueId = Id(), Type ValueType = UnknownType, Id RefId = Id());

  virtual std::string GetName(void)  const;
  virtual std::string Dump(void)     const;
  Id                  GetId(void)    const;
  Type                GetType(void)  const;
  Id                  GetRefId(void) const;

private:
  std::string m_Name;
  Id          m_Id;
  Type        m_Type;
  Id          m_RefId;
};

class Medusa_EXPORT TypedValueDetail : public Detail
{
public:
  typedef std::list<TypedValueDetail> List;

  TypedValueDetail(
    std::string const& rTypeName = "", TypeDetail::Type TypeType = TypeDetail::UnknownType, u8 TypeSize = 0,
    std::string const& rValueName = "", Id ValueId = Id(), ValueDetail::Type ValueType = ValueDetail::UnknownType, Id RefId = Id());
  TypedValueDetail(TypeDetail const& rType, ValueDetail const& rValue) : Detail(Detail::TypedValue), m_Type(rType), m_Value(rValue) {}

  virtual std::string GetName(void) const { return m_Value.GetName(); }
  virtual u32 GetSize(void) const { return m_Type.GetSize(); }
  virtual std::string Dump(void) const;

  TypeDetail  const& GetType(void)  const;
  ValueDetail const& GetValue(void) const;

protected:
  TypeDetail  m_Type;
  ValueDetail m_Value;
};

//class Medusa_EXPORT UnionDetail : public Detail
//{
//public:
//  typedef std::function<TypedValueDetail(Detail const* pParentDetail)> SelecterType;
//  UnionDetail(TypedValueDetail::List const& rTypedValues, SelecterType Selecter);
//
//private:
//};

class Medusa_EXPORT StaticArrayDetail : public Detail
{
public:
  StaticArrayDetail(TypedValueDetail const& rEntry, u32 NumberOfEntry);

  virtual std::string GetName(void) const;
  virtual u32         GetSize(void) const;

  virtual std::string Dump(void) const;

  TypedValueDetail const& GetEntry(void) const;
  u32 GetNumberOfEntry(void) const;

private:
  TypedValueDetail m_Entry;
  u32 m_NumberOfEntry;
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
class Medusa_EXPORT StructureDetail : public Detail
{
public:
  StructureDetail(std::string const& rName = "", u32 Alignment = 0);
  virtual ~StructureDetail(void);

  virtual std::string GetName(void) const;
  virtual std::string Dump(void) const;

  Detail* GetFieldByName(std::string const& rFieldName);
  Detail* GetFieldByOffset(u32 Offset);

  StructureDetail& AddField(Detail* pField);

  void ForEachField(std::function <bool (u32 Offset, Detail const& rField)> Callback) const;

private:
  bool _DetermineNextOffset(u32& rNextOffset) const;

  std::string m_Name;
  u32 m_Alignment;
  std::unordered_map<std::string, u32> m_NameToOffset;
  std::map<u32, Detail*> m_OffsetToField;
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

  virtual std::string                GetName(void)       const;
  virtual std::string                Dump(void)          const;
  TypeDetail                  const& GetReturnType(void) const;
  std::list<TypedValueDetail> const& GetParameters(void) const;

private:
  std::string            m_Name;
  TypeDetail             m_ReturnType;
  TypedValueDetail::List m_Parameters;
  // TODO:
  // Attributes
  // Calling convention
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_DETAIL_HPP
