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

bool SociDatabase::IsCompatible(boost::filesystem::path const &rDatabasePath) const
{
  return false;
}

bool SociDatabase::Open(boost::filesystem::path const &rDatabasePath)
{
  try
  {
    m_Session.open("sqlite3", "dbname=" + rDatabasePath.string());
  }
  catch (std::exception const &e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::_PrintColumnProperties(soci::row const &row) const
{
  for (std::size_t i = 0; i < row.size(); ++i)
  {
    const soci::column_properties &props = row.get_properties(i);
    std::cerr << " index: " << i << " - name: " << props.get_name() << std::endl;
  }
  return true;
}

bool SociDatabase::_FileExists(boost::filesystem::path const &rFilePath)
{
  return boost::filesystem::exists(rFilePath);
}

bool SociDatabase::_FileRemoves(boost::filesystem::path const &rFilePath)
{
  return boost::filesystem::remove(rFilePath);
}

bool SociDatabase::_MoveAddressForward(Address const &rAddress, Address &rMovedAddress,
                                       s64 Offset) const
{
  auto MemArea = GetMemoryArea(rAddress);
  if (!MemArea->IsCellPresent(rAddress))
  {
    Log::Write("soci").Level(LogError) << "memory cell was not found" << LogEnd;
    return false;
  }
  u64 CurMemAreaOff = rAddress.GetOffset() - MemArea->GetBaseAddress().GetOffset();
  if (CurMemAreaOff + Offset < MemArea->GetSize())
  {
    if (Offset == 0)
    {
      rMovedAddress = rAddress;
    }
    else
    {
      TOffset MovedOffset = rAddress.GetOffset();
      for (; Offset; --Offset)
      {
        while (true)
        {
          CellData::SPType spCellData = std::make_shared<CellData>(CellData());
          if (GetCellData(rAddress, *spCellData))
          {
            auto CellDataLength = spCellData->GetLength();
            auto OffsetAfterMove = MovedOffset + spCellData->GetLength();
            bool is_ovf = ((MovedOffset > 0) && (CellDataLength > 0)
                           && (OffsetAfterMove < 0))
                          || ((MovedOffset < 0) && (CellDataLength < 0)
                              && (OffsetAfterMove > 0));

            if (!is_ovf)
              MovedOffset += spCellData->GetLength();
          }
          else
          {
            ++MovedOffset;
          }
          if (MemArea->IsCellPresent(MovedOffset))
            break;
          if (MovedOffset > MemArea->GetBaseAddress().GetOffset() + MemArea->GetSize())
          {
            Log::Write("soci").Level(LogError) << "offset is out of bounds" << LogEnd;
            return false;
          }
        }
      }
      rMovedAddress = MemArea->MakeAddress(MovedOffset);
    }
  }
  // if (MemArea->MoveAddressForward(rAddress, rMovedAddress, Offset))
  //   {
  // 	std::cerr << "-------------------------- MoveAddressForward : TRUE : Address "
  // 		  << rAddress.Dump()
  // 		  << "  -  Moved Address : "
  // 		  << rMovedAddress.Dump()
  // 		  << "  -  Offset : " << Offset
  // 		  << std::endl;
  // 	return true;
  //   }

  //Compute the size - current_memory_area_offset
  //  std::cerr << "-------------------------- MoveAddressForward : FALSE" << std::endl;
  return true;
}

bool SociDatabase::Create(boost::filesystem::path const &rDatabasePath, bool Force)
{
  try
  {
    if (Force)
    {
      Log::Write("db_soci") << "remove file " << rDatabasePath.string() << LogEnd;
      _FileRemoves(rDatabasePath);
    }
    if (!Force && _FileExists(rDatabasePath))
    {
      Log::Write("db_soci") << "db already exists and force is false" << LogEnd;
      return false;
    }
    if (rDatabasePath.string().empty())
    {
      Log::Write("db_soci") << "db path is empty" << LogEnd;
      return false;
    }

    m_Session.open("sqlite3", "dbname=" + rDatabasePath.string());
    _CreateTable();
  }
  catch (std::exception const &e)
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
    return true;
  }
  catch (std::exception const &e)
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
  catch (std::exception const &e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::RegisterArchitectureTag(Tag ArchitectureTag)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
    m_Session << "INSERT INTO Architecture(architecture_tag)"
            "VALUES(:tag)", soci::use(ArchitectureTag);
  }
  catch (std::exception const &e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
  }
  return true;
}

bool SociDatabase::UnregisterArchitectureTag(Tag ArchitectureTag)
{
  try
  {
    m_Session << "DELETE FROM Architecture WHERE architecture_tag = :architecture_tag"
            , soci::use(ArchitectureTag);
  }
  catch (std::exception const &e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
  }
  return true;
}

std::list <Tag> SociDatabase::GetArchitectureTags(void) const
{
  std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
  return {};
}

bool SociDatabase::SetArchitecture(Address const &rAddress, Tag ArchitectureTag, u8 Mode,
                                   Database::SetArchitectureModeType SetArchMode)
{
  //TODO:To implement
  try
  {
    switch (SetArchMode)
    {
      case ByCell:
      {
        soci::indicator indicator;
        soci::row row;
        m_Session << "Select * from CellData", soci::into(row, indicator);
        if (!m_Session.got_data())
        {
          CellData Cell;
          Address::List DeletedCell;
          Cell.SetDefaultArchitectureTag(ArchitectureTag);
          Cell.SetDefaultMode(Mode);
          SetCellData(rAddress, Cell, DeletedCell, 1);
        }
        else
        {
          m_Session << "UPDATE CellData set architecture_tag = :architecture_tag"
                  ", architecture_mode = :architecture_mode"
                  , soci::use(ArchitectureTag, "architecture_tag")
                  , soci::use(Mode, "architecture_mode");
        }
        return true;
      }
      case ByMemoryArea:
      {
        std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);
        m_Session << "UPDATE MappedMemoryArea set architecture_tag = :architecture_tag"
                ", architecture_mode = :architecture_mode", soci::use(ArchitectureTag, "architecture_tag")
                , soci::use(Mode, "architecture_mode");
        return true;
      }

      default:
        break;
    }
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "cannot set architecture: " << rErr.what() << LogEnd;
    return false;
  }

  return false;
}

bool SociDatabase::AddMemoryArea(MemoryArea *pMemArea)
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);
  try
  {

    MappedMemoryArea *pMappedMemArea = dynamic_cast<MappedMemoryArea *>(pMemArea);
    VirtualMemoryArea *pVirtualMemArea = dynamic_cast<VirtualMemoryArea *>(pMemArea);

    if (!pMappedMemArea && !pVirtualMemArea)
    {
      Log::Write("soci").Level(LogError) << "memory area is null" << LogEnd;
      return false;
    }
    Address add;
    std::string Table = "";
    std::string Query = "(name, access, virtual_size, "
            "architecture_tag, architecture_mode, file_offset, file_size,"
            "addressing_type, base, offset, base_size, offset_size)"
            "VALUES(:name, :access, :virtual_size, :architecture_tag, "
            ":architecture_mode, :file_offset, :file_size, :addressing_type, "
            ":base, :offset, :base_size, :offset_size)";

    if (nullptr != pMemArea && pMappedMemArea)
    {
      add = pMappedMemArea->GetBaseAddress();
      Table = "MappedMemoryArea";
      m_Session << "INSERT INTO " + Table + Query,
              soci::use(*pMappedMemArea)
              , soci::use(static_cast<int>(add.GetAddressingType()), "addressing_type")
              , soci::use(add.GetBase(), "base"), soci::use(add.GetOffset(), "offset")
              , soci::use(add.GetBaseSize(), "base_size")
              , soci::use(add.GetOffsetSize(), "offset_size");
    }
    else if (nullptr != pMemArea && pVirtualMemArea)
    {
      add = pVirtualMemArea->GetBaseAddress();
      Table = "VirtualMemoryArea";
      m_Session << "INSERT INTO " + Table + Query,
              soci::use(*pVirtualMemArea)
              , soci::use(static_cast<int>(add.GetAddressingType()), "addressing_type")
              , soci::use(add.GetBase(), "base"), soci::use(add.GetOffset(), "offset")
              , soci::use(add.GetBaseSize(), "base_size")
              , soci::use(add.GetOffsetSize(), "offset_size");
    }

  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "add memory area: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

void SociDatabase::ForEachMemoryArea(MemoryAreaCallback Callback) const
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);
  MappedMemoryArea *MemArea = new MappedMemoryArea;

  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare << "Select name, access, virtual_size,"
            "architecture_tag, architecture_mode,"
            "file_offset, file_size, addressing_type, "
            "base, offset, base_size, offset_size, id_memory_area "
            "FROM MappedMemoryArea");

    for (auto it = rowset.begin(); it != rowset.end(); ++it)
    {
      soci::row const &row = *it;
      Address VirtualBaseAddress;
      VirtualBaseAddress.SetAddressingType(static_cast<medusa::Address::Type>(row.get<int>(7)));
      VirtualBaseAddress.SetBase(row.get<int>(8));
      VirtualBaseAddress.SetOffset(row.get<int>(9));
      VirtualBaseAddress.SetBaseSize(row.get<int>(10));
      VirtualBaseAddress.SetOffsetSize(row.get<int>(11));

      MemArea->SetName(row.get<std::string>(0));
      MemArea->SetAccess(row.get<int>(1));
      MemArea->SetVirtualSize(row.get<int>(2));
      MemArea->SetDefaultArchitectureTag(row.get<int>(3));
      MemArea->SetDefaultArchitectureMode(row.get<int>(4));
      MemArea->SetFileOffset(row.get<int>(5));
      MemArea->SetFileSize(row.get<int>(6));
      MemArea->SetId(row.get<int>(12));
      MemArea->SetVirtualBase(VirtualBaseAddress);
      Callback(*MemArea);
    }

    rowset = (m_Session.prepare << "Select name, access, virtual_size,"
            "architecture_tag, architecture_mode,"
            "file_offset, file_size, addressing_type, "
            "base, offset, base_size, offset_size, id_memory_area "
            "FROM VirtualMemoryArea");

    for (auto it = rowset.begin(); it != rowset.end(); ++it)
    {
      soci::row const &row = *it;
      Address VirtualBaseAddress;
      VirtualBaseAddress.SetAddressingType(static_cast<medusa::Address::Type>(row.get<int>(7)));
      VirtualBaseAddress.SetBase(row.get<int>(8));
      VirtualBaseAddress.SetOffset(row.get<int>(9));
      VirtualBaseAddress.SetBaseSize(row.get<int>(10));
      VirtualBaseAddress.SetOffsetSize(row.get<int>(11));

      MemArea->SetName(row.get<std::string>(0));
      MemArea->SetAccess(row.get<int>(1));
      MemArea->SetVirtualSize(row.get<int>(2));
      MemArea->SetDefaultArchitectureTag(row.get<int>(3));
      MemArea->SetDefaultArchitectureMode(row.get<int>(4));
      MemArea->SetFileOffset(row.get<int>(5));
      MemArea->SetFileSize(row.get<int>(6));
      MemArea->SetId(row.get<int>(12));
      MemArea->SetVirtualBase(VirtualBaseAddress);
      Callback(*MemArea);
    }
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "for each memory area: " << rErr.what() << rErr.what() << LogEnd;
  }
}

MemoryArea const *SociDatabase::GetMemoryArea(Address const &rAddress) const
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);
  MappedMemoryArea *MemArea = new MappedMemoryArea;

  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare << "Select name, access, virtual_size,"
            "architecture_tag, architecture_mode,"
            "file_offset, file_size, addressing_type, "
            "base, offset, base_size, offset_size, id_memory_area "
            "FROM MappedMemoryArea WHERE :offset_addr >= offset "
            "AND :offset_addr < (offset + virtual_size)"
            , soci::use(rAddress.GetOffset(), "offset_addr"));

    auto it = rowset.begin();
    if (m_Session.got_data())
    {
      soci::row const &row = *it;
      Address VirtualBaseAddress;

      VirtualBaseAddress.SetAddressingType(static_cast<medusa::Address::Type>(row.get<int>(7)));
      VirtualBaseAddress.SetBase(row.get<int>(8));
      VirtualBaseAddress.SetOffset(row.get<int>(9));
      VirtualBaseAddress.SetBaseSize(row.get<int>(10));
      VirtualBaseAddress.SetOffsetSize(row.get<int>(11));

      MemArea->SetName(row.get<std::string>(0));
      MemArea->SetAccess(row.get<int>(1));
      MemArea->SetVirtualSize(row.get<int>(2));
      MemArea->SetDefaultArchitectureTag(row.get<int>(3));
      MemArea->SetDefaultArchitectureMode(row.get<int>(4));
      MemArea->SetFileOffset(row.get<int>(5));
      MemArea->SetFileSize(row.get<int>(6));
      MemArea->SetId(row.get<int>(12));
      MemArea->SetVirtualBase(VirtualBaseAddress);
      if ((VirtualBaseAddress.GetBase() == rAddress.GetBase())
          && VirtualBaseAddress.IsBetween(MemArea->GetSize(), rAddress.GetOffset()))
      {
        return MemArea;
      }
    }
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "memory area is null" << LogEnd;
    return nullptr;
  }
  return nullptr;
}

bool SociDatabase::GetFirstAddress(Address &rAddress) const
{
  //return false;
  Address Address;

  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT addressing_type, "
            "base, offset, base_size, offset_size, MIN(id) "
            "FROM Label");

    auto it = rowset.begin();
    if (m_Session.got_data() && it != rowset.end())
    {
      soci::row const &row = *it;
      if (row.get_indicator(0) != soci::i_null)
        Address.SetAddressingType(static_cast<medusa::Address::Type>(row.get<int>(0)));
      if (row.get_indicator(1) != soci::i_null)
        Address.SetBase(row.get<int>(1));
      if (row.get_indicator(2) != soci::i_null)
        Address.SetOffset(row.get<int>(2));
      if (row.get_indicator(3) != soci::i_null)
        Address.SetBaseSize(row.get<int>(3));
      if (row.get_indicator(4) != soci::i_null)
        Address.SetOffsetSize(row.get<int>(4));
      rAddress = Address;
      return true;
    }
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "cannot find first address" << LogEnd;
    return false;
  }

  return false;
}

bool SociDatabase::GetLastAddress(Address &rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  return false;
}

bool SociDatabase::MoveAddress(Address const &rAddress, Address &rMovedAddress, s64 Offset) const
{
  //TODO:To implement
  //  return false;
  if (Offset < 0)
    return false;
  if (Offset > 0)
    return _MoveAddressForward(rAddress, rMovedAddress, Offset);

  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;

  return false;
}

bool SociDatabase::ConvertAddressToPosition(Address const &rAddress, u32 &rPosition) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  return false;
}

bool SociDatabase::ConvertPositionToAddress(u32 Position, Address &rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  return false;
}

bool SociDatabase::AddLabel(Address const &rAddress, Label const &rLabel)
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);

  try
  {
    m_Session << "INSERT INTO Label(name, name_length, label_type, version, label_addr,"
            "addressing_type, base, offset, base_size, offset_size)"
            "VALUES(:name, :name_length, :label_type, :version, :label_addr,"
            ":addressing_type, :base, :offset, :base_size, :offset_size)"
            , soci::use(rLabel.GetName(), "name")
            , soci::use(rLabel.GetNameLength(), "name_length")
            , soci::use(rLabel.GetType(), "label_type")
            , soci::use(rLabel.GetVersion(), "version")
            , soci::use(rAddress.Dump(), "label_addr")
            , soci::use(static_cast<int>(rAddress.GetAddressingType()), "addressing_type")
            , soci::use(rAddress.GetBase(), "base")
            , soci::use(rAddress.GetOffset(), "offset")
            , soci::use(rAddress.GetBaseSize(), "base_size")
            , soci::use(rAddress.GetOffsetSize(), "offset_size");
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "add label failed" << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::RemoveLabel(Address const &rAddress)
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);
  try
  {
    m_Session << "DELETE FROM Label WHERE label_addr = :label_addr ", soci::use(rAddress.Dump());
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "remove label failed" << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetLabel(Address const &rAddress, Label &rLabel) const
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);

  try
  {
    Label label;
    soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT name, "
            "label_type, version "
            "FROM Label WHERE label_addr = :label_addr"
            , soci::use(rAddress.Dump()));

    auto it = rowset.begin();
    if (m_Session.got_data())
    {
      soci::row const &row = *it;

      label.SetType(row.get<int>(1));
      label.SetVersion(row.get<int>(2));
      label.SetName(row.get<std::string>(0));
      rLabel = label;
      return true;
    }
  }
  catch (soci::soci_error &rErr)
  {
    std::cerr << "Error: GetLabel: " << rErr.what() << std::endl;
    return false;
  }
  return false;
}

bool SociDatabase::GetLabelAddress(Label const &rLabel, Address &rAddress) const
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);
  Address Address;

  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT addressing_type, "
            "base, offset, base_size, offset_size "
            "FROM Label WHERE name=:name",
            soci::use(rLabel.GetName(), "name"));

    auto it = rowset.begin();
    if (m_Session.got_data())
    {
      soci::row const &row = *it;

      Address.SetAddressingType(static_cast<medusa::Address::Type>(row.get<int>(0)));
      Address.SetBase(row.get<int>(1));
      Address.SetOffset(row.get<int>(2));
      Address.SetBaseSize(row.get<int>(3));
      Address.SetOffsetSize(row.get<int>(4));
      rAddress = Address;
    }
  }
  catch (soci::soci_error &rErr)
  {
    std::cerr << "Error: GetLabelAddress: " << rErr.what() << std::endl;
    return false;
  }

  // m_Session << "SELECT addressing_type"
  //   //" base, offset, base_size, offset_size"
  //   "FROM Label WHERE name=:name",
  //   soci::use(rLabel.GetName(), "name"), soci::into(address, ind);
  // if (m_Session.got_data())
  //   {
  //     switch (ind)
  // 	{
  // 	case soci::i_ok:
  // 	  {
  // 	    rAddress = address;
  // 	    std::cerr << "-------------------- DB_Soci, LabelAddress : "
  // 		      << rAddress.Dump() << std::endl;
  // 	    return true;
  // 	  }
  // 	default:
  // 	  return false;
  // 	}
  //   }
  return true;
}

void SociDatabase::ForEachLabel(LabelCallback Callback)
{
  //TODO:To implement
  //  std::cerr << "---------------------------- ForEachLabel" << std::endl;
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);
  Address Address;
  Label Label;

  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT addressing_type, "
            "base, offset, base_size, offset_size,"
            "name, label_type, version"
            " FROM Label");

    for (auto it = rowset.begin(); it != rowset.end(); ++it)
    {
      soci::row const &row = *it;

      int AddressingType = row.get<int>(0);
      u16 Base = row.get<int>(1);
      u32 Offset = row.get<int>(2);
      u8 BaseSize = row.get<int>(3);
      u8 OffsetSize = row.get<int>(4);

      Label.SetType(row.get<int>(6));
      Label.SetVersion(row.get<int>(7));
      Label.SetName(row.get<std::string>(5));

      Address.SetAddressingType(static_cast<medusa::Address::Type>(AddressingType));
      Address.SetBase(Base);
      Address.SetOffset(Offset);
      Address.SetBaseSize(BaseSize);
      Address.SetOffsetSize(OffsetSize);
      Callback(Address, Label);
    }
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "for each label failed" << LogEnd;
  }
}

bool SociDatabase::AddCrossReference(Address const &rTo, Address const &rFrom)
{
  //TODO: To implement
  std::lock_guard <std::mutex> Lock(m_CrossReferencesLock);

  try
  {
    m_Session << "INSERT INTO CrossReference("
            "type_to INTEGER, sub_type_to INTEGER, length_to INTEGER, format_style_to INTEGER, "
            "architecture_tag_to INTEGER, architecture_mode_to INTEGER)"
            "type_from INTEGER, sub_type_from INTEGER, length_from INTEGER,"
            "format_style_from INTEGER, architecture_tag_from INTEGER, "
            "architecture_mode_from INTEGER,"
            "VALUES("
            ":type_to, sub_type_to, :length_to, :format_style_to, "
            ":architecture_tag_to, :architecture_mode_to)"
            ":type_from, :sub_type_from, :length_from,"
            ":format_style_from, :architecture_tag_from, "
            ":architecture_mode_from)", soci::use(rTo), soci::use(rFrom);
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "add cross reference failed" << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::RemoveCrossReference(Address const &rFrom)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return false;
}

bool SociDatabase::RemoveCrossReferences(void)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return false;
}

bool SociDatabase::HasCrossReferenceFrom(Address const &rTo) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return false;
}

bool SociDatabase::GetCrossReferenceFrom(Address const &rTo, Address::List &rFromList) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return false;
}

bool SociDatabase::HasCrossReferenceTo(Address const &rFrom) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return false;
}

bool SociDatabase::GetCrossReferenceTo(Address const &rFrom, Address::List &rToList) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return false;
}

bool SociDatabase::AddMultiCell(Address const &rAddress, MultiCell const &rMultiCell)
{
  //TODO: To implement
  //FIXME(lemme): we don't need blob to store the label
  std::lock_guard <std::recursive_mutex> Lock(m_MultiCellLock);
  try
  {
    soci::blob blob(m_Session);
    MultiCell MultiCell = rMultiCell;
    blob.write(0, reinterpret_cast<char *>(&MultiCell), sizeof(MultiCell));
    m_Session << "INSERT INTO MultiCell("
            "dump, MultiCell) "
            "VALUES(:dump, :multi_cell)", soci::use(rAddress.Dump()), soci::use(blob);
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "add multicell failed" << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::RemoveMultiCell(Address const &rAddress)
{
  std::lock_guard <std::recursive_mutex> Lock(m_MultiCellLock);
  return false;
}

bool SociDatabase::GetMultiCell(Address const &rAddress, MultiCell &rMultiCell) const
{
  //FIXME(lemme): we don't need blob to store the label
  std::lock_guard <std::recursive_mutex> Lock(m_MultiCellLock);
  try
  {
    soci::blob blob(m_Session);
    m_Session << "SELECT multi_cell WHERE name = :name", soci::use(rAddress.Dump())
            , soci::into(blob);
    if (!m_Session.got_data())
    {
      return false;
    }
    blob.read(0, reinterpret_cast<char *>(&rMultiCell), sizeof(MultiCell));
  }
  catch (soci::soci_error& rErr)
  {
    Log::Write("soci").Level(LogError) << "get multicell failed" << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::GetCellData(Address const &rAddress, CellData &rCellData)
{
  std::lock_guard <std::recursive_mutex> Lock(m_CellLock);
  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
  {
    Log::Write("soci").Level(LogError) << "memory area is null" << LogEnd;
    return false;
  }
  auto spCellData = std::make_shared<CellData>(Cell::ValueType, ValueDetail::HexadecimalType, 1);
  if (spCellData == nullptr)
  {
    Log::Write("soci").Level(LogError) << "celldata is null" << LogEnd;
    return false;
  }
  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT type,"
            "sub_type, length, format_style, flags,"
            " architecture_tag,"
            "architecture_mode FROM CellData WHERE id_memory_area = :id_memory_area"
            , soci::use(pMemArea->GetId(), "id_memory_area"));

    soci::rowset<soci::row>::const_iterator it = rowset.begin();
    if (m_Session.got_data())
    {
      soci::row const &row = *it;

      spCellData->Type() = row.get<int>(0);
      spCellData->SubType() = row.get<int>(1);
      spCellData->Length() = row.get<int>(2);
      spCellData->FormatStyle() = row.get<int>(3);
      spCellData->Flags() = row.get<int>(4);
      spCellData->SetDefaultArchitectureTag(row.get<int>(5));
      spCellData->SetDefaultMode(row.get<int>(6));
    }
    rCellData = *spCellData;
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "get cell data failed" << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::GetCellData(Address const &rAddress, CellData &rCellData) const
{
  std::lock_guard <std::recursive_mutex> Lock(m_CellLock);
  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
  {
    Log::Write("soci").Level(LogError) << "memory area is null" << LogEnd;
    return false;
  }

  auto spCellData = std::make_shared<CellData>(Cell::ValueType, ValueDetail::HexadecimalType, 1);
  if (spCellData == nullptr)
  {
    Log::Write("soci").Level(LogError) << "celldata is null" << LogEnd;
    return false;
  }

  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT type,"
            "sub_type, length, format_style, flags,"
            " architecture_tag,"
            "architecture_mode FROM CellData WHERE label_addr = :label_addr AND id = :offset"
            , soci::use(pMemArea->GetBaseAddress().Dump(), "label_addr")
            , soci::use(rAddress.GetOffset() - pMemArea->GetBaseAddress().GetOffset(), "offset"));

    soci::rowset<soci::row>::const_iterator it = rowset.begin();
    if (m_Session.got_data())
    {
      soci::row const &row = *it;

      spCellData->Type() = row.get<int>(0);
      spCellData->SubType() = row.get<int>(1);
      spCellData->Length() = row.get<int>(2);
      spCellData->FormatStyle() = row.get<int>(3);
      spCellData->Flags() = row.get<int>(4);
      spCellData->SetDefaultArchitectureTag(row.get<int>(5));
      spCellData->SetDefaultMode(row.get<int>(6));
    }
    rCellData = *spCellData;
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "memory area is null" << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::SetCellData(Address const &rAddress, CellData const &rCellData, Address::List &rDeletedCellAddresses,
                               bool Force)
{
  std::lock_guard <std::recursive_mutex> Lock(m_CellLock);
  MemoryArea *pCurMemArea = const_cast<MemoryArea *>(GetMemoryArea(rAddress));
  if (!pCurMemArea || !pCurMemArea->IsCellPresent(rAddress))
  {
    Log::Write("soci").Level(LogError) << "memory area is null or memory cell was not found" << LogEnd;
    return false;
  }
  size_t CellOffset = static_cast<size_t>(rAddress.GetOffset() - pCurMemArea->GetBaseAddress().GetOffset());
  CellData::SPType spCellData = std::make_shared<CellData>(rCellData);
  int Length = rCellData.GetLength();
  Tag ArchTag;
  int Mode;

  try
  {
    m_Session << "SELECT architecture_tag, architecture_mode FROM CellData WHERE architecture_mode != 1"
            , soci::into(ArchTag), soci::into(Mode);
    if (!m_Session.got_data())
    {
      ArchTag = spCellData->GetArchitectureTag();
      Mode = spCellData->GetMode();
    }
    m_Session << "INSERT INTO CellData(id, type, sub_type, length, format_style, flags,"
            "architecture_tag,"
            "architecture_mode, label_addr, address_base, base_offset, address_base_size,"
            " address_offset_size, id_memory_area)"
            "VALUES(:id, :type, :sub_type, :length, :format_style, :flags,"
            ":architecture_tag, :architecture_mode, :label_addr, :address_base, "
            ":base_offset, :address_base_size"
            ", :address_offset_size, :id_memory_area)"
            , soci::use(CellOffset, "id"), soci::use(spCellData->GetType(), "type")
            , soci::use(spCellData->GetSubType(), "sub_type")
            , soci::use(spCellData->GetLength(), "length")
            , soci::use(spCellData->GetFormatStyle(), "format_style")
            , soci::use(spCellData->Flags(), "flags")
            , soci::use(ArchTag, "architecture_tag")
            , soci::use(Mode, "architecture_mode")
            , soci::use(pCurMemArea->GetBaseAddress().Dump(), "label_addr")
            , soci::use(pCurMemArea->GetBaseAddress().GetBase(), "address_base")
            , soci::use(pCurMemArea->GetBaseAddress().GetOffset(), "base_offset")
            , soci::use(pCurMemArea->GetBaseAddress().GetBaseSize(), "address_base_size")
            , soci::use(pCurMemArea->GetBaseAddress().GetOffsetSize(), "address_offset_size")
            , soci::use(pCurMemArea->GetId(), "id_memory_area");

    for (auto i = 0; i < Length; ++i)
    {
      m_Session << "INSERT INTO CellDataLayout(id_memory_area, cell_offset) "
              "VALUES(:id_memory_area, :cell_offset)"
              , soci::use(pCurMemArea->GetId(), "id_memory_area")
              , soci::use(i, "cell_offset");
    }
  }
  catch (soci::soci_error &rErr)
  {
    Log::Write("soci").Level(LogError) << "set cell data failed" << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::DeleteCellData(Address const &rAddress)
{
  return false;
}

bool SociDatabase::GetComment(Address const &rAddress, std::string &rComment) const
{
  return false;
}

bool SociDatabase::SetComment(Address const &rAddress, std::string const &rComment)
{
  return false;
}

bool SociDatabase::GetValueDetail(Id ConstId, ValueDetail &rConstDtl) const
{
  return false;
}

bool SociDatabase::SetValueDetail(Id ConstId, ValueDetail const &rConstDtl)
{
  return false;
}

bool SociDatabase::GetFunctionDetail(Id FuncId, FunctionDetail &rFuncDtl) const
{
  return false;
}

bool SociDatabase::SetFunctionDetail(Id FuncId, FunctionDetail const &rFuncDtl)
{
  return false;
}

bool SociDatabase::GetStructureDetail(Id StructId, StructureDetail &rStructDtl) const
{
  return false;
}

bool SociDatabase::SetStructureDetail(Id StructId, StructureDetail const &rStructDtl)
{
  return false;
}

bool SociDatabase::RetrieveDetailId(Address const &rAddress, u8 Index, Id &rDtlId) const
{
  return false;
}

bool SociDatabase::BindDetailId(Address const &rAddress, u8 Index, Id DtlId)
{
  return false;
}

bool SociDatabase::UnbindDetailId(Address const &rAddress, u8 Index)
{
  return false;
}
