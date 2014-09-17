#include "medusa/detail.hpp"

MEDUSA_NAMESPACE_USE;

TypeDetail::TypeDetail(std::string const& rName, TypeDetail::Type Type, u8 Size)
  : Detail(Detail::Type), m_Name(rName), m_Type(Type), m_BitSize(Size)
{
}

std::string TypeDetail::GetName(void) const
{
  return m_Name;
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

ValueDetail::ValueDetail(std::string const& rName, Id ValueId, Type ValueType, Id RefId)
  : Detail(Detail::Value), m_Name(rName), m_Id(ValueId), m_Type(ValueType), m_RefId(RefId)
{
}

std::string ValueDetail::GetName(void) const
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
  : Detail(Detail::TypedValue)
  , m_Type(rTypeName, TypeType, TypeSize)
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

StructureDetail::StructureDetail(std::string const& rName, u32 Alignment)
: Detail(Detail::Structure), m_Name(rName), m_Alignment(Alignment)
{
}

StructureDetail::~StructureDetail(void)
{
  for (auto const& Field : m_OffsetToField)
  {
    delete Field.second;
  }
}

std::string StructureDetail::GetName(void) const
{
  return m_Name;
}

Detail* StructureDetail::GetFieldByName(std::string const& rFieldName)
{
  auto itOffset = m_NameToOffset.find(rFieldName);
  if (itOffset == std::end(m_NameToOffset))
    return nullptr;
  return GetFieldByOffset(itOffset->second);
}
Detail* StructureDetail::GetFieldByOffset(u32 Offset)
{
  auto itField = m_OffsetToField.find(Offset);
  if (itField == std::end(m_OffsetToField))
    return nullptr;
  return itField->second;
}

StructureDetail& StructureDetail::AddField(Detail* pField)
{
  auto itOffset = m_NameToOffset.find(pField->GetName());
  if (itOffset != std::end(m_NameToOffset))
    return *this; // TODO:

  u32 NextOffset = 0;
  if (!_DetermineNextOffset(NextOffset))
    return *this; // TODO:

  auto itField = m_OffsetToField.find(NextOffset);
  if (itField != std::end(m_OffsetToField))
    return *this; // TODO:

  m_NameToOffset[pField->GetName()] = NextOffset;
  m_OffsetToField[NextOffset] = pField;

  return *this;
}

void StructureDetail::ForEachField(std::function <bool(u32 Offset, Detail const& rField)> Callback) const
{
  for (auto const& rField : m_OffsetToField)
  {
    if (!Callback(rField.first, *rField.second))
      return;
  }
}

std::string StructureDetail::Dump(void) const
{
  std::ostringstream Res;

  ForEachField([&](u32 Offset, Detail const& rField)
  {
    Res << "offset: " << std::hex << std::setw(8) << std::setfill('0') << Offset << " " << rField.GetName() << "\n";
    return true;
  });

  return Res.str();
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
  auto const pField = itLastField->second;
  u32 LastFieldSize = pField->GetSize();
  if (LastFieldSize == 0)
    return false;
  rNextOffset = LastOffset + LastFieldSize;
  // TODO: implement alignment
  return true;
}

FunctionDetail::FunctionDetail(std::string const& rName, TypeDetail const& rReturnType, TypedValueDetail::List const& rParameters)
  : Detail(Detail::Function), m_Name(rName), m_ReturnType(rReturnType), m_Parameters(rParameters)
{
}

std::string FunctionDetail::GetName(void) const
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