#include "medusa/database.hpp"

MEDUSA_NAMESPACE_BEGIN

Database::Database(void)
{
}

Database::~Database(void)
{
}

std::string Database::GetName(void) const
{
  return "Invalid";
}

std::string Database::GetExtension(void) const
{
  return ".mdb"; // generic name
}

bool Database::IsCompatible(std::wstring const& rDatabasePath) const
{
  return false;
}

bool Database::Open(std::wstring const& rFilePath)
{
  return false;
}

bool Database::Create(std::wstring const& rDatabasePath)
{
  return false;
}

bool Database::Flush(void)
{
  return false;
}

bool Database::Close(void)
{
  return false;
}

//FileBinaryStream const& Database::GetFileBinaryStream(void) const
//{
//}

bool Database::AddMemoryArea(MemoryArea* pMemArea)
{
  return false;
}

MemoryArea const* Database::GetMemoryArea(Address const& rAddress) const
{
  return nullptr;
}

bool Database::AddLabel(Address const& rAddress, Label const& rLbl)
{
  return false;
}

bool Database::RemoveLabel(Address const& rAddress)
{
  return false;
}

bool Database::HasLabel(Address const& rAddress) const
{
  return false;
}

bool Database::GetLabel(Address const& rAddress, Label& rLbl) const
{
  return false;
}

bool Database::GetLabelAddress(std::string const& rName, Address& rAddress) const
{
  return false;
}

bool Database::AddCrossReference(Address const& rTo, Address const& rFrom)
{
  return false;
}

bool Database::RemoveCrossReference(Address const& rFrom)
{
  return false;
}

bool Database::RemoveCrossReferences(void)
{
  return false;
}

bool Database::HasCrossReferenceFrom(Address const& rTo) const
{
  return false;
}

bool Database::GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const
{
  return false;
}

bool Database::HasCrossReferenceTo(Address const& rFrom) const
{
  return false;
}

bool Database::GetCrossReferenceTo(Address const& rFrom, Address& rTo) const
{
  return false;
}

bool Database::AddMultiCell(Address const& rAddress, MultiCell const& rMultiCell)
{
  return false;
}

bool Database::RemoveMultiCell(Address const& rAddress)
{
  return false;
}

bool Database::GetMultiCell(Address const& rAddress, MultiCell& rMultiCell) const
{
  return false;
}

bool Database::GetCellData(Address const& rAddress, CellData& rCellData)
{
  return false;
}

bool Database::SetCellData(Address const& rAddress, CellData const& rCellData)
{
  return false;
}

MEDUSA_NAMESPACE_END
