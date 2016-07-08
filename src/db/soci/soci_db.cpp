#include "soci_db.hpp"

#include <medusa/module.hpp>
#include <medusa/log.hpp>
#include <medusa/util.hpp>

SociDatabase::SociDatabase(void)
{
}

SociDatabase::~SociDatabase(void)
{
}

std::string SociDatabase::GetName(void) const
{
  return "SOCI";
}

std::string SociDatabase::GetExtension(void) const
{
  return ".msd";
}

bool SociDatabase::IsCompatible(boost::filesystem::path const& rDatabasePath) const
{
  return false;
}

bool SociDatabase::Open(boost::filesystem::path const& rDatabasePath)
{
  try
  {
    m_Session.open("sqlite3", "dbname=" + rDatabasePath.string());
  }
  catch (std::exception const& e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::Create(boost::filesystem::path const& rDatabasePath, bool Force)
{
  try
  {
    m_Session.open("sqlite3", "dbname=" + rDatabasePath.string());
  }
  catch (std::exception const& e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::Flush(void)
{
  try
  {
    m_Session.commit();
  }
  catch (std::exception const& e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::Close(void)
{
  try
  {
    m_Session.close();
  }
  catch (std::exception const& e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::RegisterArchitectureTag(Tag ArchitectureTag)
{
  return false;
}

bool SociDatabase::UnregisterArchitectureTag(Tag ArchitectureTag)
{
  return false;
}

std::list<Tag> SociDatabase::GetArchitectureTags(void) const
{
  return{};
}

bool SociDatabase::SetArchitecture(Address const& rAddress, Tag ArchitectureTag, u8 Mode, Database::SetArchitectureModeType SetArchMode)
{
  switch (SetArchMode)
  {
  case ByCell:
  {
    break;
  }

  case ByMemoryArea:
  {
    break;
  }

  default:
    break;
  }

  return false;
}

bool SociDatabase::AddMemoryArea(MemoryArea* pMemArea)
{
  return false;
}

void SociDatabase::ForEachMemoryArea(MemoryAreaCallback Callback) const
{
}

MemoryArea const* SociDatabase::GetMemoryArea(Address const& rAddress) const
{
  return nullptr;
}

bool SociDatabase::GetFirstAddress(Address& rAddress) const
{
  return false;
}

bool SociDatabase::GetLastAddress(Address& rAddress) const
{
  return false;
}

bool SociDatabase::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  return false;
}
bool SociDatabase::ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const
{
  return false;
}

bool SociDatabase::ConvertPositionToAddress(u32 Position, Address& rAddress) const
{
  return false;
}

bool SociDatabase::AddLabel(Address const& rAddress, Label const& rLabel)
{
  return false;
}

bool SociDatabase::RemoveLabel(Address const& rAddress)
{
  return false;
}

bool SociDatabase::GetLabel(Address const& rAddress, Label& rLabel) const
{
  return false;
}

bool SociDatabase::GetLabelAddress(Label const& rLabel, Address& rAddress) const
{
  return false;
}

void SociDatabase::ForEachLabel(LabelCallback Callback)
{
}

bool SociDatabase::AddCrossReference(Address const& rTo, Address const& rFrom)
{
  return false;
}

bool SociDatabase::RemoveCrossReference(Address const& rFrom)
{
  return false;
}

bool SociDatabase::RemoveCrossReferences(void)
{
  return false;
}

bool SociDatabase::HasCrossReferenceFrom(Address const& rTo) const
{
  return false;
}

bool SociDatabase::GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const
{
  return false;
}

bool SociDatabase::HasCrossReferenceTo(Address const& rFrom) const
{
  return false;
}

bool SociDatabase::GetCrossReferenceTo(Address const& rFrom, Address::List& rToList) const
{
  return false;
}

bool SociDatabase::AddMultiCell(Address const& rAddress, MultiCell const& rMultiCell)
{
  return false;
}

bool SociDatabase::RemoveMultiCell(Address const& rAddress)
{
  return false;
}

bool SociDatabase::GetMultiCell(Address const& rAddress, MultiCell& rMultiCell) const
{
  return false;
}

bool SociDatabase::GetCellData(Address const& rAddress, CellData& rCellData)
{
  return false;
}

bool SociDatabase::SetCellData(Address const& rAddress, CellData const& rCellData, Address::List& rDeletedCellAddresses, bool Force)
{
  return false;
}

bool SociDatabase::DeleteCellData(Address const& rAddress)
{
  return false;
}

bool SociDatabase::GetComment(Address const& rAddress, std::string& rComment) const
{
  return false;
}

bool SociDatabase::SetComment(Address const& rAddress, std::string const& rComment)
{
  return false;
}

bool SociDatabase::GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const
{
  return false;
}

bool SociDatabase::SetValueDetail(Id ConstId, ValueDetail const& rConstDtl)
{
  return false;
}

bool SociDatabase::GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const
{
  return false;
}

bool SociDatabase::SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl)
{
  return false;
}

bool SociDatabase::GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const
{
  return false;
}

bool SociDatabase::SetStructureDetail(Id StructId, StructureDetail const& rStructDtl)
{
  return false;
}

bool SociDatabase::RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const
{
  return false;
}

bool SociDatabase::BindDetailId(Address const& rAddress, u8 Index, Id DtlId)
{
  return false;
}

bool SociDatabase::UnbindDetailId(Address const& rAddress, u8 Index)
{
  return false;
}
