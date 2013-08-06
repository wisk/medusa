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

bool Database::Close(void)
{
  return false;
}

bool Database::SaveDocument(Document const& rDoc)
{
  return false;
}

bool Database::SaveMemoryArea(MemoryArea const& rMemArea)
{
  return false;
}

bool Database::SaveMultiCell(Address const& rAddress, MultiCell const& rMultiCell)
{
  return false;
}

bool Database::SaveCell(Address const& rAddress, Cell const& rCell)
{
  return false;
}

bool Database::SaveLabel(Address const& rAddress, Label const& rLabel)
{
  return false;
}

bool Database::SaveXRef(Address const& rSrcAddr, Address const& rDstAddr)
{
  return false;
}

bool Database::LoadDocument(Document& rDoc)
{
  return false;
}

bool Database::LoadMemoryArea(MemoryArea& rMemArea)
{
  return false;
}

bool Database::LoadMultiCell(Address const& rAddress, MultiCell& rMultiCell)
{
  return false;
}

bool Database::LoadCell(Address const& rAddress, Cell& rCell)
{
  return false;
}

bool Database::LoadLabel(Address const& rAddress, Label& rLabel)
{
  return false;
}

bool Database::LoadXRef(Address& rSrcAddr, Address& rDstAddr)
{
  return false;
}

MEDUSA_NAMESPACE_END