#include "medusa/detail.hpp"

MEDUSA_NAMESPACE_USE;

TypeDetail::TypeDetail(std::string const& rName, TypeDetail::Type Type, u8 Size)
  : m_Name(rName), m_Type(Type), m_Size(Size)
{
}

std::string const &TypeDetail::GetName(void) const
{
  return m_Name;
}

TypeDetail::Type TypeDetail::GetType(void) const
{
  return m_Type;
}

u8 TypeDetail::GetSize(void) const
{
  return m_Size;
}

ValueDetail::ValueDetail(std::string const& rName, Id ValueId, Type ValueType, Id RefId)
  : m_Name(rName), m_Id(ValueId), m_Type(ValueType), m_RefId(RefId)
{
}

std::string const& ValueDetail::GetName(void) const
{
  return m_Name;
}

Id ValueDetail::GetId(void) const
{
  return m_Id;
}

ValueDetail::Type ValueDetail::GetType(void) const
{
  return m_Type;
}

Id ValueDetail::GetRefId(void) const
{
  return m_RefId;
}

TypedValueDetail::TypedValueDetail(
  std::string const& rTypeName, TypeDetail::Type TypeType, u8 TypeSize,
  std::string const& rValueName, Id ValueId, ValueDetail::Type ValueType, Id RefId)
  : m_Type(rTypeName, TypeType, TypeSize)
  , m_Value(rValueName, ValueId, ValueType, RefId)
{
}

TypeDetail const& TypedValueDetail::GetType(void) const
{
  return m_Type;
}

ValueDetail const& TypedValueDetail::GetValue(void) const
{
  return m_Value;
}

FunctionDetail::FunctionDetail(std::string const& rName, TypeDetail const& rReturnType, TypedValueDetail::List const& rParameters)
  : m_Name(rName), m_ReturnType(rReturnType), m_Parameters(rParameters)
{
}

std::string const& FunctionDetail::GetName(void) const
{
  return m_Name;
}

TypeDetail const& FunctionDetail::GetReturnType(void) const
{
  return m_ReturnType;
}

std::list<TypedValueDetail> const& FunctionDetail::GetParameters(void) const
{
  return m_Parameters;
}