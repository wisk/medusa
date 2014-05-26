#include "medusa/detail.hpp"

MEDUSA_NAMESPACE_USE;

TypeDetail::TypeDetail(std::string const& rName, TypeDetail::Type Type, u8 Size)
  : m_Name(rName), m_Type(Type), m_Size(Size)
{
}

ValueDetail::ValueDetail(std::string const& rName, Id ValueId, Type ValueType, Id RefId)
  : m_Name(rName), m_Id(ValueId), m_RefId(RefId)
{
}

TypedValueDetail::TypedValueDetail(
  std::string const& rTypeName, TypeDetail::Type TypeType, u8 TypeSize,
  std::string const& rValueName, Id ValueId, ValueDetail::Type ValueType, Id RefId)
  : TypeDetail(rTypeName, TypeType, TypeSize)
  , ValueDetail(rValueName, ValueId, ValueType, RefId)
{
}

FunctionDetail::FunctionDetail(std::string const& rName, TypeDetail const& rReturnType, TypedValueDetail::List const& rParameters)
  : m_Name(rName), m_ReturnType(rReturnType), m_Parameters(rParameters)
{
}