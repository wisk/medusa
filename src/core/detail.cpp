#include "medusa/detail.hpp"

MEDUSA_NAMESPACE_USE;

Detail::Detail(Detail::Kind Kind, Detail::SPType spParent, std::string const& rName)
  : m_IsValid(true), m_Kind(Kind), m_spParent(spParent), m_Name(rName)
{
  m_Id = rName.empty() ? Id() : Sha1(rName);
}

TypeDetail::TypeDetail(std::string const& rName, TypeDetail::Type Type, u32 BitSize)
  : Detail(Detail::Type, nullptr, rName), m_Type(Type), m_BitSize(BitSize)
{
}

std::string TypeDetail::Dump(void) const
{
  return "<type>(" + m_Name + ")";
}

TypeDetail::TypeDetail(Detail::Kind Kind, std::string const& rName, Type Type)
  : Detail(Kind, nullptr, rName), m_Type(Type), m_BitSize(0)
{
}

TypeDetail::Type TypeDetail::GetType(void) const
{
  return m_Type;
}

u32 TypeDetail::GetSize(void) const
{
  return m_BitSize / 8; // TODO: handle 80-bit value
}

u32 TypeDetail::GetBitSize(void) const
{
  return m_BitSize;
}

ValueDetail::ValueDetail(std::string const& rName, ValueDetail::Type ValueType, Id RefId)
  : Detail(Detail::Value, nullptr, rName), m_Type(ValueType), m_RefId(RefId)
{
}

std::string ValueDetail::Dump(void) const
{
  return "<value>(" + m_Name + ")";
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
  std::string const& rTypeName, TypeDetail::Type Type, u8 Size,
  std::string const& rValueName, ValueDetail::Type ValueType, Id RefId)
  : Detail(Detail::TypedValue, nullptr, rTypeName + " " + rValueName)
  , m_spType(std::make_shared<TypeDetail>(rTypeName, Type, Size))
  , m_Value(rValueName, ValueType, RefId)
{
}

TypedValueDetail::TypedValueDetail(TypeDetail::SPType spType, ValueDetail const& rValue)
  : Detail(Detail::TypedValue, nullptr, spType->GetName() + " " + rValue.GetName()), m_spType(spType), m_Value(rValue)
{
}

u32 TypedValueDetail::GetSize(void) const
{
  return m_spType->GetSize();
}

std::string TypedValueDetail::Dump(void) const
{
  return m_spType->Dump() + " " + m_Value.Dump();
}

TypeDetail::SPType const& TypedValueDetail::GetType(void) const
{
  return m_spType;
}

ValueDetail const& TypedValueDetail::GetValue(void) const
{
  return m_Value;
}

StaticArrayDetail::StaticArrayDetail(TypeDetail::SPType spElementType, u32 NumberOfElements)
  : TypeDetail(Detail::StaticArray, spElementType->GetName() + "[]", TypeDetail::ArrayType)
  , m_spElementType(spElementType), m_NumberOfElements(NumberOfElements)
{
}

u32 StaticArrayDetail::GetSize(void) const
{
  u64 ArrSz = m_spElementType->GetSize() * m_NumberOfElements;
  if (ArrSz > 0xffffffff)
  {
    m_IsValid = false;
    return 0;
  }
  return static_cast<u32>(ArrSz);
}

u32 StaticArrayDetail::GetBitSize(void) const
{
  u64 ArrSz = m_spElementType->GetBitSize() * m_NumberOfElements;
  if (ArrSz > 0xffffffff)
  {
    m_IsValid = false;
    return 0;
  }
  return static_cast<u32>(ArrSz);
}

std::string StaticArrayDetail::Dump(void) const
{
  std::ostringstream Res;
  for (u32 Count = 0; Count < m_NumberOfElements; ++Count)
    Res << "<static_array>(" << m_spElementType->Dump() << ")[" << Count << "]";
  return Res.str();
}

TypeDetail::SPType const& StaticArrayDetail::GetElementType(void) const
{
  return m_spElementType;
}

u32 StaticArrayDetail::GetNumberOfElements(void) const
{
  return m_NumberOfElements;
}

StructureDetail::StructureDetail(std::string const& rName, u32 Alignment)
  : TypeDetail(Detail::Structure, rName, TypeDetail::StructureType), m_Alignment(Alignment)
{
}

u32 StructureDetail::GetSize(void) const
{
  u32 StructSize = 0;
  // Actually, the next offset is also the structure size
  // NOTE: What about alignment?
  if (!_DetermineNextOffset(StructSize))
  {
    m_IsValid = false;
    return 0;
  }

  return StructSize;
}

std::string StructureDetail::Dump(void) const
{
  std::ostringstream Res;

  Res << "<structure>(" << m_Name << "), size: " << std::hex << GetSize() << "\n";
  ForEachField([&](u32 Offset, Detail const& rField)
  {
    Res << "+" << std::hex << std::setw(8) << std::setfill('0') << Offset << " " << rField.Dump() << "\n";
    return true;
  });

  return Res.str();
}

bool StructureDetail::GetFieldByName(std::string const& rFieldName, TypedValueDetail& rField)
{
  auto itOffset = m_NameToOffset.find(rFieldName);
  if (itOffset == std::end(m_NameToOffset))
    return false;
  return GetFieldByOffset(itOffset->second, rField);
}

bool StructureDetail::GetFieldByOffset(u32 Offset, TypedValueDetail& rField)
{
  auto itField = m_OffsetToField.find(Offset);
  if (itField == std::end(m_OffsetToField))
    return false;
  rField = itField->second;
  return true;
}

StructureDetail& StructureDetail::AddField(TypeDetail::SPType spFieldType, std::string const& rFieldName, ValueDetail::Type FieldType, Id FieldRefId)
{
  auto itOffset = m_NameToOffset.find(rFieldName);
  if (itOffset != std::end(m_NameToOffset))
  {
    m_IsValid = false;
    return *this;
  }

  u32 NextOffset = 0;
  if (!_DetermineNextOffset(NextOffset))
  {
    m_IsValid = false;
    return *this;
  }

  auto itField = m_OffsetToField.find(NextOffset);
  if (itField != std::end(m_OffsetToField))
  {
    m_IsValid = false;
    return *this;
  }

  m_NameToOffset[rFieldName] = NextOffset;

  if (spFieldType->GetKind() == Detail::Structure)
  {
    m_OffsetToField[NextOffset] = TypedValueDetail(spFieldType, ValueDetail(rFieldName, ValueDetail::CompositeType, spFieldType->GetId()));
    return *this;
  }

  m_OffsetToField[NextOffset] = TypedValueDetail(spFieldType, ValueDetail(rFieldName, FieldType, FieldRefId));

  return *this;
}

void StructureDetail::ForEachField(std::function <bool(u32 Offset, TypedValueDetail const& rField)> Callback) const
{
  for (auto const& rField : m_OffsetToField)
  {
    if (!Callback(rField.first, rField.second))
      return;
  }
}

bool StructureDetail::_DetermineNextOffset(u32& rNextOffset) const
{
  if (m_OffsetToField.empty())
  {
    rNextOffset = 0;
    return true;
  }
  auto itLastField = m_OffsetToField.end();
  --itLastField;
  u32 LastOffset = itLastField->first;
  auto const& rspField = itLastField->second;
  u32 LastFieldSize = rspField.GetType()->GetSize();
  if (LastFieldSize == 0)
    return false;
  rNextOffset = LastOffset + LastFieldSize;
  // TODO: implement alignment
  return true;
}

FunctionDetail::FunctionDetail(std::string const& rName, TypeDetail const& rReturnType, TypedValueDetail::List const& rParameters)
  : Detail(Detail::Function, nullptr, rName), m_ReturnType(rReturnType), m_Parameters(rParameters)
{
}

std::string FunctionDetail::Dump(void) const
{
  return "<function>(" + m_Name + ")";
}

TypeDetail const& FunctionDetail::GetReturnType(void) const
{
  return m_ReturnType;
}

std::list<TypedValueDetail> const& FunctionDetail::GetParameters(void) const
{
  return m_Parameters;
}