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
  //std::cerr << "---------------------------- DB_Soci: IsCompatible ------------------------" << std::endl;
  return false;
}

bool SociDatabase::Open(boost::filesystem::path const& rDatabasePath)
{
  try
  {
    //std::cerr << "---------------------------- DB_Soci: Open ------------------------" << std::endl;
    m_Session.open("sqlite3", "dbname=" + rDatabasePath.string());
  }
  catch (std::exception const& e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool	SociDatabase::_PrintColumnProperties(soci::row const& row) const
{
  for (std::size_t i = 0; i < row.size(); ++i)
    {
      const soci::column_properties & props = row.get_properties(i);
      std::cerr << " index: " << i << " - name: " << props.get_name() << std::endl;
    }
  return true;
}

bool SociDatabase::_FileExists(boost::filesystem::path const& rFilePath)
{
  return boost::filesystem::exists(rFilePath);
}

bool SociDatabase::_FileRemoves(boost::filesystem::path const& rFilePath)
{
  return boost::filesystem::remove(rFilePath);
}

bool SociDatabase::_MoveAddressForward(Address const& rAddress, Address& rMovedAddress,
				       s64 Offset) const
{
  //std::cerr << "-------------------------- MoveAddressForward" << std::endl;
  //Find the correct memory area, according to the address.
  auto MemArea = GetMemoryArea(rAddress);
  if (!MemArea->IsCellPresent(rAddress))
    {
      //std::cerr << "-------------------------- MoveAddressForward : TRUE" << std::endl;
      return false;
    }
  //Compute the offset of the current memory area and check if we can move forward
  //the address
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
		      //std::cerr << "************************************** LENGTH : "
		      //				<< CellDataLength << std::endl;

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
		    return false;
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

bool SociDatabase::Create(boost::filesystem::path const& rDatabasePath, bool Force)
{
  try
  {
    //std::cerr << "------------------------- DB_Soci: Create ----------------------" << std::endl;
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
  catch (std::exception const& e)
  {
    Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    //std::cerr << "----------------- DB Soci : Error" << std::endl;
    return false;
  }

  return true;
}

bool SociDatabase::Flush(void)
{
  //std::cerr << "---------------------------- Flush" << std::endl;
  try
  {
    //m_Session.commit();
    return true;
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
    //std::cerr << "------------------------------- DB_Soci: Close ----------------------------" << std::endl;
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
  //std::cerr << "---------------------------- RegisterArchitectureTag" << std::endl;
  try
    {
      m_Session << "INSERT INTO Architecture(architecture_tag)"
	"VALUES(:tag)", soci::use(ArchitectureTag);
    }
  catch (std::exception const& e)
    {
      Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    }
  return true;
}

bool SociDatabase::UnregisterArchitectureTag(Tag ArchitectureTag)
{
  //std::cerr << "---------------------------- UnregisterArchitectureTag" << std::endl;
  try
    {
      m_Session << "DELETE FROM Architecture WHERE architecture_tag = :architecture_tag"
	, soci::use(ArchitectureTag);
    }
  catch (std::exception const& e)
    {
      Log::Write("soci").Level(LogError) << e.what() << LogEnd;
    }
  return true;
}

std::list<Tag> SociDatabase::GetArchitectureTags(void) const
{
  //std::cerr << "---------------------------- GetArchitectureTags" << std::endl;
  return{};
}

bool SociDatabase::SetArchitecture(Address const& rAddress, Tag ArchitectureTag, u8 Mode, Database::SetArchitectureModeType SetArchMode)
{
  //TODO:To implement
  std::cerr << "----------------------------- DB_Soci: SetArchitecture" << std::endl;
  switch (SetArchMode)
    {
    case ByCell:
      {
	soci::indicator indicator;
        soci::row row
        m_Session << "Select * from CellData", soci::into(row, indicator);
        if (!m_Session.got_data())
          {
	    std::cerr << "----------------------------- DB_Soci: SetArchitecture : SetCellData" << std::endl;
            CellData Cell;
            Address::List DeletedCell;
            Cell.SetDefaultArchitectureTag(ArchitectureTag);
            Cell.SetDefaultMode(Mode);
            SetCellData(rAddress, Cell, DeletedCell, 1);
          }
        else
          {
	    std::cerr << "----------------------------- DB_Soci: SetArchitecture : UPDATE" << std::endl;
            m_Session << "UPDATE CellData set architecture_tag = :architecture_tag"
	      ", architecture_mode = :architecture_mode"
	      , soci::use(ArchitectureTag, "architecture_tag")
	      , soci::use(Mode, "architecture_mode");
          }
	return true;
      }
    case ByMemoryArea:
      {
	std::cerr << "----------------------------- DB_Soci: SetArchitecture : Memory" << std::endl;
        std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
        m_Session << "UPDATE MappedMemoryArea set architecture_tag = :architecture_tag"
            ", architecture_mode = :architecture_mode", soci::use(ArchitectureTag, "architecture_tag")
            , soci::use(Mode, "architecture_mode");
        return true;
      }

  default:
    break;
  }

  return false;
}

bool SociDatabase::AddMemoryArea(MemoryArea* pMemArea)
{
  //std::cerr << "-------------------------- AddMemoryArea ----------------------"
  //	    << std::endl;
  std::lock_guard<std::mutex>	Lock(m_MemoryAreaLock);

  try
    {

      MappedMemoryArea*	pMappedMemArea = dynamic_cast<MappedMemoryArea*>(pMemArea);
      VirtualMemoryArea* pVirtualMemArea = dynamic_cast<VirtualMemoryArea*>(pMemArea);

      if (!pMappedMemArea && !pVirtualMemArea)
      {
	  //std::cerr << "------------------------------- DYN_CAST : FAIL - "
	  //		<< typeid(decltype(pMemArea)).name() << std::endl;
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
	    soci::use (*pMappedMemArea)
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
	    soci::use (*pVirtualMemArea)
	    , soci::use(static_cast<int>(add.GetAddressingType()), "addressing_type")
	    , soci::use(add.GetBase(), "base"), soci::use(add.GetOffset(), "offset")
	    , soci::use(add.GetBaseSize(), "base_size")
	    , soci::use(add.GetOffsetSize(), "offset_size");
	}

    }
  catch (soci::soci_error& rErr)
    {
      //std::cerr << rErr.what() << std::endl;
      return false;
    }

  return true;
}

void SociDatabase::ForEachMemoryArea(MemoryAreaCallback Callback) const
{
  //std::cerr << "---------------------------- ForEachMemoryArea" << std::endl;
  MappedMemoryArea	*MemArea = new MappedMemoryArea;

  std::string name;
  soci::rowset<soci::row> rowset = (m_Session.prepare << "Select name, access, virtual_size,"
				    "architecture_tag, architecture_mode,"
				    "file_offset, file_size, addressing_type, "
				    "base, offset, base_size, offset_size, id_memory_area "
				    "FROM MappedMemoryArea");

  for(auto it = rowset.begin(); it != rowset.end(); ++it)
    {
      soci::row const& row = *it;
      Address	VirtualBaseAddress;
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
      //std::cerr << "------------------------------------- FOR EACH MemoryArea : "
      //		<< MemArea->Dump() << std::endl;
    }

  rowset = (m_Session.prepare << "Select name, access, virtual_size,"
	    "architecture_tag, architecture_mode,"
	    "file_offset, file_size, addressing_type, "
	    "base, offset, base_size, offset_size, id_memory_area "
	    "FROM VirtualMemoryArea");

  for(auto it = rowset.begin(); it != rowset.end(); ++it)
    {
      soci::row const& row = *it;
      Address	VirtualBaseAddress;
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
      //std::cerr << "------------------------------------- FOR EACH MemoryArea : "
      //		<< MemArea->Dump() << std::endl;
    }
  //std::cerr << "------------------------------------- FOR EACH MemoryArea : End " << std::endl;
}

MemoryArea const* SociDatabase::GetMemoryArea(Address const& rAddress) const
{
  //std::cerr << "-------------------------- GetMemoryArea ----------------------" << std::endl;

  MappedMemoryArea *MemArea = new MappedMemoryArea;
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
      soci::row const& row = *it;
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
	  //std::cerr << "----------------------------------- GET_MEM_AREA: CELL IS PRESENT" << std::endl;
	  return MemArea;
	}
    }
  return nullptr;
}

bool SociDatabase::GetFirstAddress(Address& rAddress) const
{
  std::cerr << "---------------------------- Get First Address" << std::endl;
  Address Address;
  soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT addressing_type, "                                    
				    "base, offset, base_size, offset_size, MIN(id) "
				    "FROM Label");

  auto it = rowset.begin();
  if (m_Session.got_data() && it != rowset.end())
    {
      soci::row const& row = *it;
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
      std::cerr << "-------------------- DUMP address FirstAddress: "
		<< Address.Dump() << std::endl;
      rAddress = Address;
    }
  std::cerr << "--------------------------- FirstAddress: return FALSE";
  return false;
}

bool SociDatabase::GetLastAddress(Address& rAddress) const
{
  //std::cerr << "---------------------------- Get Last Address" << std::endl;
  return false;
}

bool SociDatabase::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  //TODO:To implement

  //std::cerr << "---------------------------- Move Address" << std::endl;
  if (Offset < 0)
    return false;
  if (Offset > 0)
    return _MoveAddressForward(rAddress, rMovedAddress, Offset);

  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;

  //std::cerr << "---------------------------- Get Nearest Address" << std::endl;
  //  return pMemArea->GetNearestAddress(rAddress, rMovedAddress);
  return false;
}

bool SociDatabase::ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const
{
  //std::cerr << "---------------------------- Convert Address to Position" << std::endl;
  return false;
}

bool SociDatabase::ConvertPositionToAddress(u32 Position, Address& rAddress) const
{
  //std::cerr << "---------------------------- Convert Position to Address" << std::endl;
  return false;
}

bool SociDatabase::AddLabel(Address const& rAddress, Label const& rLabel)
{
  //std::cerr << "------------------------------ DB_Soci: AddLabel -------------------------"
  //	    << std::endl;
  std::lock_guard<std::recursive_mutex>	Lock(m_LabelLock);

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
  return true;
}

bool SociDatabase::RemoveLabel(Address const& rAddress)
{
  //std::cerr << "----------------------- DB_Soci: RemoveLabel --------------------"
  //	    << std::endl;
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  m_Session << "Delete FROM Label WHERE label_addr = :label_addr ", soci::use(rAddress.Dump());
  return true;
}

bool SociDatabase::GetLabel(Address const& rAddress, Label& rLabel) const
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);

  // std::cerr << "----------------------- DB_Soci: GetLabel --------------------"
  // 	    << std::endl;
  Label	label;
  soci::rowset<soci::row> rowset =   (m_Session.prepare << "SELECT name, "
				      "label_type, version "
				      "FROM Label WHERE label_addr = :label_addr"
				      , soci::use(rAddress.Dump()));

  auto it = rowset.begin();
  if (m_Session.got_data())
    {
      soci::row const& row = *it;

      label.SetType(row.get<int>(1));
      label.SetVersion(row.get<int>(2));
      label.SetName(row.get<std::string>(0));
      rLabel = label;
      //      std::cerr << "--------------------------- DB_Soci: GetLabel FOUND" << std::endl;
      return true;
    }
  //  std::cerr << "--------------------------- DB_Soci: GetLabel is EMPTY" << std::endl;
  return false;
}

bool SociDatabase::GetLabelAddress(Label const& rLabel, Address& rAddress) const
{
  //std::cerr << "---------------------------- GetLabelAddress" << std::endl;
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  Address Address;

  soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT addressing_type, "
				    "base, offset, base_size, offset_size "
				    "FROM Label WHERE name=:name",
				    soci::use(rLabel.GetName(), "name"));

  auto it = rowset.begin();
  if (m_Session.got_data())
    {
      soci::row const& row = *it;

      Address.SetAddressingType(static_cast<medusa::Address::Type>(row.get<int>(0)));
      Address.SetBase(row.get<int>(1));
      Address.SetOffset(row.get<int>(2));
      Address.SetBaseSize(row.get<int>(3));
      Address.SetOffsetSize(row.get<int>(4));
      rAddress = Address;
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
  return false;
}

void SociDatabase::ForEachLabel(LabelCallback Callback)
{
  //TODO:To implement
  //std::cerr << "---------------------------- ForEachLabel" << std::endl;
  // Address Address;
  // soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT addressing_type, "
  // 					  "base, offset, base_size, offset_size "
  // 					  "FROM Label");

  // for (auto it = rowset.begin(); it != rowset.end(); ++it)
  //   {
  //     soci::row const& row = *it;

  //     int AddressingType = row.get<int>(0);
  //     u16 Base = row.get<int>(1);
  //     u32 Offset = row.get<int>(2);
  //     u8  BaseSize = row.get<int>(3);
  //     u8  OffsetSize = row.get<int>(4);

  //     Address.SetAddressingType(static_cast<medusa::Address::Type>(AddressingType));
  //     Address.SetBase(Base);
  //     Address.SetOffset(Offset);
  //     Address.SetBaseSize(BaseSize);
  //     Address.SetOffsetSize(OffsetSize);
  //     //std::cerr << "-------------------- ------------- ----- FOR EACHLABEL: Row: "
  //     //  		<< address.Dump() << std::endl;
  //   }
  //std::cerr << "-------------------- ------------- ----- FOR EACHLABEL: END: " << std::endl;
}

bool SociDatabase::AddCrossReference(Address const& rTo, Address const& rFrom)
{
  //TODO: To implement
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);

  //std::cerr << "----------------------------------- DB_Soci: AddCrossReference --------------------" << std::endl;
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

  return true;
}

bool SociDatabase::RemoveCrossReference(Address const& rFrom)
{
  //std::cerr << "---------------------------- RemoveCrossReference" << std::endl;
  return false;
}

bool SociDatabase::RemoveCrossReferences(void)
{
  //std::cerr << "---------------------------- RemoveCrossReferences" << std::endl;
  return false;
}

bool SociDatabase::HasCrossReferenceFrom(Address const& rTo) const
{
  //std::cerr << "---------------------------- HasCrossReferenceFrom" << std::endl;
  return false;
}

bool SociDatabase::GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const
{
  //std::cerr << "---------------------------- GetCrossReferenceFrom" << std::endl;
  return false;
}

bool SociDatabase::HasCrossReferenceTo(Address const& rFrom) const
{
  //std::cerr << "---------------------------- HasCrossReferenceTo" << std::endl;
  return false;
}

bool SociDatabase::GetCrossReferenceTo(Address const& rFrom, Address::List& rToList) const
{
  //std::cerr << "---------------------------- GetCrossReferenceTo" << std::endl;
  return false;
}

bool SociDatabase::AddMultiCell(Address const& rAddress, MultiCell const& rMultiCell)
{
  //TODO: To implement
  //std::cerr << "---------------------------- AddMultiCell" << std::endl;
  //FIXME(lemme): we don't need blob to store the label
  soci::blob blob(m_Session);
  MultiCell MultiCell = rMultiCell;
  blob.write(0, reinterpret_cast<char*>(&MultiCell), sizeof(MultiCell));
  m_Session << "INSERT INTO MultiCell("
    "dump, MultiCell) "
    "VALUES(:dump, :multi_cell)", soci::use(rAddress.Dump()), soci::use(blob);
  return true;
}

bool SociDatabase::RemoveMultiCell(Address const& rAddress)
{
  //std::cerr << "---------------------------- RemoveMultiCell" << std::endl;
  return false;
}

bool SociDatabase::GetMultiCell(Address const& rAddress, MultiCell& rMultiCell) const
{
  //std::cerr << "---------------------------- GetMultiCell" << std::endl;
  //FIXME(lemme): we don't need blob to store the label
  soci::blob blob(m_Session);
  m_Session << "SELECT multi_cell WHERE name = :name", soci::use(rAddress.Dump())
    , soci::into(blob);
  if (!m_Session.got_data())
    {
      //std::cerr << "------------------------ GetMultiCell : EMPTY" << std::endl;
      return false;
    }
  blob.read(0, reinterpret_cast<char*>(&rMultiCell), sizeof(MultiCell));
  return true;
}

bool SociDatabase::GetCellData(Address const& rAddress, CellData& rCellData)
{
  //  std::cerr << "---------------------------- GetCellData: NON CONST" << std::endl;
  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    {
      //  std::cerr << "---------------------------- GetCellData: FALSE MemoryArea is ABSENT" << std::endl;
      return false;
    }
  //  auto spCellData = pMemArea->GetCellData(rAddress.GetOffset());
  auto spCellData = std::make_shared<CellData>(Cell::ValueType, ValueDetail::HexadecimalType, 1);
  if (spCellData == nullptr)
    {
      // std::cerr << "---------------------------- GetCellData: FALSE" << std::endl;
      return false;
    }
  soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT type,"
  				    "sub_type, length, format_style, flags,"
  				    " architecture_tag,"
  				    "architecture_mode FROM CellData WHERE id_memory_area = :id_memory_area"
  				    , soci::use(pMemArea->GetId(), "id_memory_area"));

  soci::rowset<soci::row>::const_iterator it = rowset.begin();
  if (m_Session.got_data())
    {
      soci::row const& row = *it;

      spCellData->Type() = row.get<int>(0);
      spCellData->SubType() = row.get<int>(1);
      spCellData->Length() = row.get<int>(2);
      spCellData->FormatStyle() = row.get<int>(3);
      spCellData->Flags() = row.get<int>(4);
      spCellData->SetDefaultArchitectureTag(row.get<int>(5));
      spCellData->SetDefaultMode(row.get<int>(6));
    }
  rCellData = *spCellData;
  //    std::cerr << "---------------------------- GetCellData: TRUE - Mode : " << (int)rCellData.GetMode() << std::endl;
  return true;
}

bool SociDatabase::GetCellData(Address const& rAddress, CellData& rCellData) const
{
  //std::cerr << "---------------------------- GetCellData: CONST" << std::endl;
  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    {
      //std::cerr << "---------------------------- GetCellData: FALSE" << std::endl;
      return false;
    }

  auto spCellData = std::make_shared<CellData>(Cell::ValueType, ValueDetail::HexadecimalType, 1);
 if (spCellData == nullptr)
    {
      //std::cerr << "---------------------------- GetCellData: FALSE" << std::endl;
      return false;
    }

  soci::rowset<soci::row> rowset = (m_Session.prepare << "SELECT type,"
  				    "sub_type, length, format_style, flags,"
  				    " architecture_tag,"
  				    "architecture_mode FROM CellData WHERE label_addr = :label_addr AND id = :offset"
  				    , soci::use(pMemArea->GetBaseAddress().Dump(), "label_addr")
				    , soci::use(rAddress.GetOffset() - pMemArea->GetBaseAddress().GetOffset(), "offset"));

  soci::rowset<soci::row>::const_iterator it = rowset.begin();
  if (m_Session.got_data())
    {
      soci::row const& row = *it;

      spCellData->Type() = row.get<int>(0);
      spCellData->SubType() = row.get<int>(1);
      spCellData->Length() = row.get<int>(2);
      spCellData->FormatStyle() = row.get<int>(3);
      spCellData->Flags() = row.get<int>(4);
      spCellData->SetDefaultArchitectureTag(row.get<int>(5));
      spCellData->SetDefaultMode(row.get<int>(6));
    }
  rCellData = *spCellData;
  //std::cerr << "---------------------------- GetCellData: TRUE - Length : " << rCellData.GetLength() << std::endl;
  return true;
}

bool SociDatabase::SetCellData(Address const& rAddress, CellData const& rCellData, Address::List& rDeletedCellAddresses, bool Force)
{
  //  std::cerr << "---------------------------- SetCellData" << std::endl;
  MemoryArea* pCurMemArea = const_cast<MemoryArea*>(GetMemoryArea(rAddress));
  //std::cerr << " * COUNT *" << std::endl;
  if (!pCurMemArea || !pCurMemArea->IsCellPresent(rAddress))
    {
      //  std::cerr << "---------------------------- SetCellData: CurMemArea is EMPTY or "
      //	<< "Cell is not present"
      //	<< std::endl;
      return false;
    }
  //std::cerr << pCurMemArea->Dump() << std::endl;
  size_t CellOffset = static_cast<size_t>(rAddress.GetOffset() - pCurMemArea->GetBaseAddress().GetOffset());
  CellData::SPType spCellData = std::make_shared<CellData>(rCellData);
  int Length = rCellData.GetLength();
  Tag   ArchTag;
  int   Mode;

  m_Session << "SELECT architecture_tag, architecture_mode FROM CellData WHERE architecture_mode != 1"
      , soci::into(ArchTag), soci::into(Mode);
  if (!m_Session.got_data())
    {
      //      std::cerr << "--------------------------- m_Session.got_data() == FALSE: "
      //   << spCellData->GetArchitectureTag() << " - " << (int)spCellData->GetMode() << std::endl;
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
    , soci::use(CellOffset, "id"), soci::use (spCellData->GetType(), "type")
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
  //  std::cerr << " * SetCellData *  ======== Length : " << (int)spCellData->GetLength() << std::endl;
  //std::cerr << " * OK *" << std::endl;
  return true;
  // return pCurMemArea->SetCellData(rAddress.GetOffset(), spCellData, rDeletedCellAddresses,
  // 				  Force);
}

bool SociDatabase::DeleteCellData(Address const& rAddress)
{
  //std::cerr << "---------------------------- DeleteCellData" << std::endl;
  return false;
}

bool SociDatabase::GetComment(Address const& rAddress, std::string& rComment) const
{
  //std::cerr << "---------------------------- GetComment" << std::endl;
  return false;
}

bool SociDatabase::SetComment(Address const& rAddress, std::string const& rComment)
{
  //std::cerr << "---------------------------- SetComment" << std::endl;
  return false;
}

bool SociDatabase::GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const
{
  //std::cerr << "---------------------------- GetValueDetail" << std::endl;
  return false;
}

bool SociDatabase::SetValueDetail(Id ConstId, ValueDetail const& rConstDtl)
{
  //std::cerr << "---------------------------- SetValueDetail" << std::endl;
  return false;
}

bool SociDatabase::GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const
{
  //std::cerr << "---------------------------- GetFunctionDetail" << std::endl;
  return false;
}

bool SociDatabase::SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl)
{
  //std::cerr << "---------------------------- SetFunctionDetail" << std::endl;
  return false;
}

bool SociDatabase::GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const
{
  //std::cerr << "---------------------------- GetStructureDetail" << std::endl;
  return false;
}

bool SociDatabase::SetStructureDetail(Id StructId, StructureDetail const& rStructDtl)
{
  //std::cerr << "---------------------------- SetStructureDetail" << std::endl;
  return false;
}

bool SociDatabase::RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const
{
  //std::cerr << "---------------------------- RetrieveDetailId" << std::endl;
  return false;
}

bool SociDatabase::BindDetailId(Address const& rAddress, u8 Index, Id DtlId)
{
  //std::cerr << "---------------------------- BindDetailId" << std::endl;
  return false;
}

bool SociDatabase::UnbindDetailId(Address const& rAddress, u8 Index)
{
  //std::cerr << "---------------------------- UnbindDetailId" << std::endl;
  return false;
}
