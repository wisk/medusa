#include "soci_db.hpp"

#include <medusa/module.hpp>
#include <medusa/log.hpp>
#include <medusa/util.hpp>

#include <soci/sqlite3/soci-sqlite3.h>

SociDatabase::SociDatabase(void)
{
}

SociDatabase::~SociDatabase(void)
{
}

bool SociDatabase::_CreateTable(void)
{
  try
  {
    m_Session << "CREATE TABLE IF NOT EXISTS BinaryStream("
      "data BLOB, endianness INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS Architecture(architecture_tag INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS DefaultAddressingType(value INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS ImageBase(value BIGINT)";

    m_Session << "CREATE TABLE IF NOT EXISTS MemoryArea("
      "id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, type INTEGER, access INTEGER,"
      "architecture_tag INTEGER, architecture_mode INTEGER,"
      "file_offset BIGINT, file_size BIGINT,"
      "addressing_type INTEGER, base INTEGER, offset BIGINT, base_size INTEGER, offset_size INTEGER, size BIGINT)";

    m_Session << "CREATE TABLE IF NOT EXISTS Label("
      "name TEXT, type INTEGER, version INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";

    m_Session << "CREATE TABLE IF NOT EXISTS CellData("
      "type INTEGER, sub_type INTEGER, size INTEGER,"
      "format_style INTEGER, flags INTEGER,"
      "architecture_tag INTEGER, architecture_mode INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";

    m_Session << "CREATE TABLE IF NOT EXISTS CellLayout("
      "offset INTEGER, size INTEGER, "
      "memory_area_id INTEGER, memory_area_offset BIGINT)";

    m_Session << "CREATE TABLE IF NOT EXISTS MultiCell("
      "type INTEGER, size INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";

    m_Session << "CREATE TABLE IF NOT EXISTS CrossReference("
      "memory_area_id_from INTEGER, memory_area_offset_from BIGINT,"
      "memory_area_id_to INTEGER,   memory_area_offset_to   BIGINT,"
      "type INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS Comment("
      "data TEXT,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
  }
  catch (std::exception& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to create tables: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::_ConvertIdToAddress(u32 Id, OffsetType Offset, Address& rAddress) const
{
  try
  {
    u32 MemAreaType;
    m_Session <<
      "SELECT type "
      "FROM MemoryArea "
      "WHERE :id == id"
      , soci::into(MemAreaType)
      , soci::use(Id);
    if (!m_Session.got_data())
      return false;

    switch (static_cast<MemoryArea::Type>(MemAreaType))
    {
    case MemoryArea::PhysicalType:
    {
      u32 FileSize;
      OffsetType FileOffset;

      m_Session <<
        "SELECT file_size, file_offset "
        "FROM MemoryArea "
        "WHERE :id == id"
        , soci::into(FileSize), soci::into(FileOffset)
        , soci::use(Id);
      if (!m_Session.got_data())
        return false;
      if (Offset >= FileSize)
        return false;
      rAddress = Address(Address::PhysicalType, 0x0, FileOffset + Offset);
      break;
    }

    case MemoryArea::MappedType:
    case MemoryArea::VirtualType:
    {
      u32 MemAreaSize;
      m_Session <<
        "SELECT size "
        "FROM MemoryArea "
        "WHERE :id == id"
        , soci::into(MemAreaSize)
        , soci::use(Id);
      if (!m_Session.got_data())
        return false;
      if (Offset >= MemAreaSize)
        return false;

      m_Session <<
        "SELECT addressing_type, base, offset, base_size, offset_size "
        "FROM MemoryArea "
        "WHERE :id == id"
        , soci::into(rAddress)
        , soci::use(Id);
      if (!m_Session.got_data())
        return false;
      rAddress.SetOffset(rAddress.GetOffset() + Offset);
      break;
    }

    default:
      Log::Write("db_soci").Level(LogError) << "invalid memory area type" << LogEnd;
      return false;
    }

  }
  catch (std::exception& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to convert id to address: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::_ConvertAddressToId(Address const& rAddress, u32& rId, OffsetType& rOffset) const
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);

  try
  {
    if (rAddress.GetAddressingType() == Address::PhysicalType)
    {
      OffsetType MemoryAreaOffset;
      m_Session <<
        "SELECT id, file_offset "
        "FROM MemoryArea "
        "WHERE :type == addressing_type AND :offset >= file_offset AND :offset < (file_offset + file_size)"
        , soci::into(rId), soci::into(MemoryAreaOffset)
        , soci::use(rAddress);
      if (!m_Session.got_data())
        return false;
      rOffset = rAddress.GetOffset() - MemoryAreaOffset;
    }
    else
    {
      OffsetType MemoryAreaOffset;
      m_Session <<
        "SELECT id, offset "
        "FROM MemoryArea "
        "WHERE :addressing_type == addressing_type AND :base == base AND :offset >= offset AND :offset < (offset + size)"
        , soci::into(rId), soci::into(MemoryAreaOffset)
        , soci::use(rAddress);
      if (!m_Session.got_data())
        return false;
      rOffset = rAddress.GetOffset() - MemoryAreaOffset;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get memory area: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::_ConvertAddressToId(Address const& rAddress, u32& rId, OffsetType& rOffset, OffsetType& rMemoryAreaOffset, u32& rMemoryAreaSize) const
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);

  try
  {
    if (rAddress.GetAddressingType() == Address::PhysicalType)
    {
      m_Session <<
        "SELECT id, file_offset, file_size "
        "FROM MemoryArea "
        "WHERE :type == addressing_type AND :offset >= file_offset AND :offset < (file_offset + file_size)"
        , soci::into(rId), soci::into(rMemoryAreaOffset), soci::into(rMemoryAreaSize)
        , soci::use(rAddress);
      if (!m_Session.got_data())
        return false;
      rOffset = rAddress.GetOffset() - rMemoryAreaOffset;
    }
    else
    {
      m_Session <<
        "SELECT id, offset, size "
        "FROM MemoryArea "
        "WHERE :addressing_type == addressing_type AND :base == base AND :offset >= offset AND :offset < (offset + size)"
        , soci::into(rId), soci::into(rMemoryAreaOffset), soci::into(rMemoryAreaSize)
        , soci::use(rAddress);
      if (!m_Session.got_data())
        return false;
      rOffset = rAddress.GetOffset() - rMemoryAreaOffset;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get memory area: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::_GetNextMemoryAreaId(u32 Id, u32& rNextId) const
{
  try
  {
    BaseType Base;
    OffsetType Offset;
    m_Session <<
      "SELECT base, offset "
      "FROM MemoryArea "
      "WHERE :id == id"
      , soci::into(Base), soci::into(Offset)
      , soci::use(Id);
    if (!m_Session.got_data())
    {
      Log::Write("db_soci").Level(LogError) << "invalid id to fetch next memory area id: " << Id << LogEnd;
      return false;
    }

    m_Session <<
      "SELECT id "
      "FROM MemoryArea "
      "WHERE :base <= base AND :offset < offset "
      "ORDER BY base ASC, offset ASC LIMIT 1"
      , soci::into(rNextId)
      , soci::use(Base), soci::use(Offset);
    if (!m_Session.got_data())
      return false;
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get next memory area id: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::_GetPreviousMemoryAreaId(u32 Id, u32& rPreviousId) const
{
  try
  {
    BaseType Base;
    OffsetType Offset;
    m_Session <<
      "SELECT base, offset "
      "FROM MemoryArea "
      "WHERE :id == id"
      , soci::into(Base), soci::into(Offset)
      , soci::use(Id);
    if (!m_Session.got_data())
    {
      Log::Write("db_soci").Level(LogError) << "invalid id to fetch previous memory area id: " << Id << LogEnd;
      return false;
    }

    m_Session <<
      "SELECT id "
      "FROM MemoryArea "
      "WHERE :base >= base AND :offset >= offset "
      "ORDER BY base DESC, offset DESC LIMIT 1"
      , soci::into(rPreviousId)
      , soci::use(Base), soci::use(Offset);
    if (!m_Session.got_data())
      return false;
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get previous memory area id: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
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

bool SociDatabase::_FileExists(boost::filesystem::path const &rFilePath)
{
  return boost::filesystem::exists(rFilePath);
}

bool SociDatabase::_FileRemoves(boost::filesystem::path const &rFilePath)
{
  return boost::filesystem::remove(rFilePath);
}

bool SociDatabase::Open(boost::filesystem::path const &rDatabasePath)
{
  try
  {
    m_Session.open(soci::sqlite3, "dbname=" + rDatabasePath.string());

    m_Session <<
      "PRAGMA synchronous        = OFF; "
      "PRAGMA journal_mode       = OFF; "
      "PRAGMA locking_mode       = EXCLUSIVE; "
      "PRAGMA temp_store         = MEMORY; "
      "PRAGMA count_changes      = OFF; "
      "PRAGMA PAGE_SIZE          = 4096; "
      "PRAGMA default_cache_size = 700000; "
      "PRAGMA cache_size         = 700000; "
      "PRAGMA compile_options; ";

    // TODO(wisk): redesign this
    soci::blob DataBinStrm(m_Session);
    u32 Endianness;

    m_Session <<
      "SELECT data, endianness "
      "FROM BinaryStream "
      , soci::into(DataBinStrm), soci::into(Endianness);

    auto DataSize = static_cast<u32>(DataBinStrm.get_len());
    std::unique_ptr<u8[]> upBuf(new u8[DataSize]);
    m_spBinStrm = std::make_shared<MemoryBinaryStream>(upBuf.get(), DataSize);
    m_spBinStrm->SetEndianness(static_cast<EEndianness>(Endianness));
  }
  catch (std::exception const &e)
  {
    Log::Write("db_soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::Create(medusa::Path const &rDatabasePath, bool Force)
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

    m_Session.open(soci::sqlite3, "dbname=" + rDatabasePath.string());
    m_Session <<
      "PRAGMA synchronous        = OFF; "
      "PRAGMA journal_mode       = OFF; "
      "PRAGMA locking_mode       = EXCLUSIVE; "
      "PRAGMA temp_store         = MEMORY; "
      "PRAGMA count_changes      = OFF; "
      "PRAGMA PAGE_SIZE          = 4096; "
      "PRAGMA default_cache_size = 700000; "
      "PRAGMA cache_size         = 700000; "
      "PRAGMA compile_options; ";
    _CreateTable();
  }
  catch (std::exception const &e)
  {
    Log::Write("db_soci").Level(LogError) << e.what() << LogEnd;
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
    Log::Write("db_soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::Close(void)
{
  try
  {
    // TODO(wisk): save binary stream
    m_Session.close();
  }
  catch (std::exception const &e)
  {
    Log::Write("db_soci").Level(LogError) << e.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::RegisterArchitectureTag(Tag ArchitectureTag)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
    m_Session <<
      "INSERT INTO Architecture(architecture_tag)"
      "VALUES(:tag)"
      , soci::use(ArchitectureTag);
  }
  catch (std::exception const &e)
  {
    Log::Write("db_soci").Level(LogError) << e.what() << LogEnd;
  }
  return true;
}

bool SociDatabase::UnregisterArchitectureTag(Tag ArchitectureTag)
{
  try
  {
    m_Session <<
      "DELETE FROM Architecture "
      "WHERE architecture_tag == :architecture_tag"
      , soci::use(ArchitectureTag);
  }
  catch (std::exception const &e)
  {
    Log::Write("db_soci").Level(LogError) << e.what() << LogEnd;
  }
  return true;
}

std::list <Tag> SociDatabase::GetArchitectureTags(void) const
{
  std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
  return{};
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
      m_Session << "SELECT * FROM CellData", soci::into(row, indicator);
      if (!m_Session.got_data())
      {
        CellData Cell;
        Address::List DeletedCell;
        Cell.SetArchitectureTag(ArchitectureTag);
        Cell.SetMode(Mode);
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
      m_Session << "UPDATE MemoryArea set architecture_tag = :architecture_tag"
        ", architecture_mode = :architecture_mode", soci::use(ArchitectureTag, "architecture_tag")
        , soci::use(Mode, "architecture_mode");
      return true;
    }

    default:
      break;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "cannot set architecture: " << rErr.what() << LogEnd;
    return false;
  }

  return false;
}

bool SociDatabase::GetImageBase(ImageBaseType& rImageBase) const
{
  try
  {
    m_Session << "SELECT value FROM ImageBase", soci::into(rImageBase);
    if (!m_Session.got_data())
      return false;
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::SetImageBase(ImageBaseType ImageBase)
{
  try
  {
    m_Session << "DELETE FROM ImageBase";
    m_Session <<
      "INSERT INTO ImageBase (value) "
      "VALUES(:value)"
      , soci::use(ImageBase);
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetMemoryArea(Address const &rAddress, MemoryArea& rMemArea) const
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);

  /*
    "CREATE TABLE IF NOT EXISTS MemoryArea("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, type INTEGER, access INTEGER,"
    "architecture_tag INTEGER, architecture_mode INTEGER,"
    "file_offset BIGINT, file_size INTEGER,"
    "addressing_type INTEGER, base INTEGER, offset BIGINT, base_size INTEGER, offset_size INTEGER,"
    "size INTEGER)";
  */
  try
  {
    if (rAddress.GetAddressingType() == Address::PhysicalType)
    {
      m_Session <<
        "SELECT * "
        "FROM MemoryArea "
        "WHERE :type == addressing_type AND :offset >= file_offset AND :offset < (file_offset + file_size)"
        , soci::into(rMemArea)
        , soci::use(rAddress);
      if (!m_Session.got_data())
        return false;
    }
    else
    {
      m_Session <<
        "SELECT * "
        "FROM MemoryArea "
        "WHERE :addressing_type == addressing_type AND :base == base AND :offset >= offset AND :offset < (offset + size)"
        , soci::into(rMemArea)
        , soci::use(rAddress);
      if (!m_Session.got_data())
        return false;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get memory area: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}


void SociDatabase::ForEachMemoryArea(MemoryAreaCallback Callback) const
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);

  try
  {
    soci::rowset<soci::row> RowSet = (m_Session.prepare <<
      "SELECT * "
      "FROM MemoryArea");

    for (auto const& rRow : RowSet)
    {
      MemoryArea CurMemArea;

      auto Name = rRow.get<std::string>(1);
      auto Access = static_cast<MemoryArea::Access>(rRow.get<int>(3));
      auto ArchTag = static_cast<Tag>(rRow.get<int>(4));
      auto ArchMode = static_cast<u8>(rRow.get<int>(5));

      auto GetAddress = [&](void) -> Address
      {
        return Address(
          static_cast<Address::Type>(rRow.get<int>(8)),
          static_cast<BaseType>(rRow.get<int>(9)),
          static_cast<OffsetType>(rRow.get<long long>(10)),
          static_cast<u8>(rRow.get<int>(11)),
          static_cast<u8>(rRow.get<int>(12))
        );
      };

      switch (static_cast<MemoryArea::Type>(rRow.get<int>(2)))
      {
      case MemoryArea::VirtualType:
        CurMemArea = MemoryArea::CreateVirtual(
          Name, Access,
          GetAddress(), static_cast<u32>(rRow.get<long long>(13)),
          ArchTag, ArchMode
        );
        break;

      case MemoryArea::MappedType:
        CurMemArea = MemoryArea::CreateMapped(
          Name, Access,
          static_cast<u32>(rRow.get<long long>(6)), static_cast<u32>(rRow.get<long long>(7)),
          GetAddress(), static_cast<u32>(rRow.get<long long>(13)),
          ArchTag, ArchMode
        );
        break;

      case MemoryArea::PhysicalType:
        CurMemArea = MemoryArea::CreatePhysical(
          Name, Access,
          static_cast<u32>(rRow.get<long long>(6)), static_cast<u32>(rRow.get<long long>(7)),
          ArchTag, ArchMode
        );
        break;

      default:
        Log::Write("db_soci").Level(LogWarning) << "unknown memory area type" << LogEnd;
        continue;
      }

      Callback(CurMemArea);
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "for each memory area: " << rErr.what() << rErr.what() << LogEnd;
  }
}

bool SociDatabase::AddMemoryArea(MemoryArea const& rMemArea)
{
  std::lock_guard <std::mutex> Lock(m_MemoryAreaLock);
  try
  {
    /*
    "CREATE TABLE IF NOT EXISTS MemoryArea("
      "id INTEGER PRIMARY KEY, name TEXT, type INTEGER, access INTEGER,"
      "architecture_tag INTEGER, architecture_mode INTEGER,"
      "file_offset BIGINT, file_size INTEGER,"
      "addressing_type INTEGER, base INTEGER, offset BIGINT, base_size INTEGER, offset_size INTEGER,"
      "size INTEGER)";
    */
    m_Session <<
      "INSERT INTO MemoryArea("
      /**/"name, type, access,"
      /**/"architecture_tag, architecture_mode,"
      /**/"file_offset, file_size,"
      /**/"addressing_type, base, offset, base_size, offset_size,"
      /**/"size)"
      "VALUES("
      /**/":name, :type, :access,"
      /**/":architecture_tag, :architecture_mode,"
      /**/":file_offset, :file_size,"
      /**/":addressing_type, :base, :offset, :base_size, :offset_size,"
      /**/":size)",
      soci::use(rMemArea);
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "add memory area: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::RemoveMemoryArea(MemoryArea const& rMemArea)
{
  return false;
}

bool SociDatabase::MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress)
{
  return false;
}

bool SociDatabase::GetDefaultAddressingType(Address::Type& rAddressType) const
{
  try
  {
    u32 Value;
    m_Session <<
      "SELECT value "
      "FROM DefaultAddressingType"
      , soci::into(Value);
    if (!m_Session.got_data())
      return false;
    rAddressType = static_cast<Address::Type>(Value);
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::SetDefaultAddressingType(Address::Type AddressType)
{
  try
  {
    m_Session << "DELETE FROM DefaultAddressingType";
    m_Session <<
      "INSERT INTO DefaultAddressingType (value) "
      "VALUES(:value)"
      , soci::use(static_cast<int>(AddressType));
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

// Implemented possibilities:
// - Logical Address → Virtual Address → (Relative Address → Physical address) WIP
// - (Physical Address → Relative Address) → Virtual Address → Logical Address (might not normalized)
// - Architecture Address → ? (depends on architecture)
// - ? → Architecture Address (might not be normalized)
// See document.hpp Document::Translate
bool SociDatabase::TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const
{
  try
  {
    switch (rAddress.GetAddressingType())
    {
    case Address::PhysicalType:
    {
      Address BaseAddr;
      OffsetType FileOffset;
      m_Session <<
        "SELECT addressing_type, base, offset, base_size, offset_size "
        "FROM MemoryArea "
        "WHERE :file_offset >= file_offset AND :file_offset < (file_offset + file_size)"
        , soci::into(BaseAddr)
        , soci::use(rAddress.GetOffset());
      if (!m_Session.got_data())
        return false;
      m_Session <<
        "SELECT file_offset "
        "FROM MemoryArea "
        "WHERE :file_offset >= file_offset AND :file_offset < (file_offset + file_size)"
        , soci::into(FileOffset)
        , soci::use(rAddress.GetOffset());
      if (!m_Session.got_data())
        return false;
      rTranslatedAddress = Address(
        BaseAddr.GetAddressingType(),
        BaseAddr.GetBase(), rAddress.GetOffset() - FileOffset + BaseAddr.GetOffset(),
        BaseAddr.GetBaseSize(), rAddress.GetOffsetSize()
      );
      if (ToConvert != rTranslatedAddress.GetAddressingType())
        return TranslateAddress(rTranslatedAddress, ToConvert, rTranslatedAddress);
    }
      break;

    case Address::LinearType:
      switch (ToConvert)
      {
      case Address::PhysicalType:
      {
        OffsetType Offset, FileOffset;
        m_Session <<
          "SELECT offset, file_offset "
          "FROM MemoryArea "
          "WHERE :addressing_type == addressing_type AND :offset >= offset AND :offset < (offset + size)"
          , soci::into(Offset), soci::into(FileOffset)
          , soci::use(static_cast<u32>(rAddress.GetAddressingType())), soci::use(rAddress.GetOffset());
        if (!m_Session.got_data())
          return false;
        rTranslatedAddress = Address(
          Address::PhysicalType,
          0x0, rAddress.GetOffset() - Offset + FileOffset,
          0x0, 64);
        break;
      }

      case Address::RelativeType:
      {
        ImageBaseType ImgBase;
        if (!GetImageBase(ImgBase))
          return false;
        if (rAddress.GetOffset() < ImgBase)
          return false;
        rTranslatedAddress = Address(
          Address::RelativeType,
          0x0, rAddress.GetOffset() - ImgBase,
          0x0, rAddress.GetOffsetSize()
        );
        break;
      }

      case Address::LogicalType: // TODO(wisk):

      default:
        Log::Write("db_soci").Level(LogError) << "unknown addressing type" << LogEnd;
        return false;
      }
      break;

    case Address::RelativeType:
      switch (ToConvert)
      {
      case Address::PhysicalType:
      {
        OffsetType Offset, FileOffset;
        m_Session <<
          "SELECT offset, file_offset "
          "FROM MemoryArea "
          "WHERE :addressing_type == addressing_type AND :offset >= offset AND :offset < (offset + size)"
          , soci::into(Offset), soci::into(FileOffset)
          , soci::use(static_cast<u32>(rAddress.GetAddressingType())), soci::use(rAddress.GetOffset());
        if (!m_Session.got_data())
          return false;
        rTranslatedAddress = Address(
          Address::PhysicalType,
          0x0, rAddress.GetOffset() - Offset + FileOffset,
          0x0, 64);
        break;
      }

      case Address::LinearType:
      {
        ImageBaseType ImgBase;
        if (!GetImageBase(ImgBase))
          return false;
        rTranslatedAddress = Address(
          Address::LinearType,
          0x0, ImgBase + rAddress.GetOffset(),
          0x0, rAddress.GetOffsetSize()
        );
        if (rTranslatedAddress.GetAddressingType() != ToConvert)
          return TranslateAddress(rTranslatedAddress, ToConvert, rTranslatedAddress);
        break;
      }

      default:
        Log::Write("db_soci").Level(LogError) << "unknown addressing type" << LogEnd;
        return false;
      }
      break;

    case Address::LogicalType: // TODO(wisk):
      return false;
      break;

    default:
      Log::Write("db_soci").Level(LogError) << "unknown addressing type" << LogEnd;
      return false;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to translate address: " << rAddress << ": " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetFirstAddress(Address &rAddress) const
{
  try
  {
    u32 Id;
    m_Session <<
      "SELECT id "
      "FROM MemoryArea "
      "ORDER BY base, offset LIMIT 1"
      , soci::into(Id);
    if (!m_Session.got_data())
      return false;
    return _ConvertIdToAddress(Id, 0x0, rAddress);
  }
  catch (soci::soci_error const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get first address: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetLastAddress(Address &rAddress) const
{
  try
  {
    u32 Id;
    OffsetType Offset;
    m_Session <<
      "SELECT id, offset "
      "FROM MemoryArea "
      "ORDER BY base DESC, offset DESC LIMIT 1"
      , soci::into(Id), soci::into(Offset);
    if (!m_Session.got_data())
      return false;
    return _ConvertIdToAddress(Id, Offset != 0x0 ? Offset - 1 : 0x0, rAddress);
  }
  catch (soci::soci_error const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get last address: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::MoveAddress(Address const &rAddress, Address &rMovedAddress, s64 Displacement) const
{
  u32 Id;
  OffsetType Offset;
  OffsetType MemAreaOff;
  u32 MemAreaSize;

  try
  {
    if (!_ConvertAddressToId(rAddress, Id, Offset, MemAreaOff, MemAreaSize))
      return false;

    // First we need to start at the beginning of the cell
    OffsetType CellOffset;
    u16 CellSize;
    m_Session <<
      "SELECT offset, size "
      "FROM CellLayout "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::into(CellOffset), soci::into(CellSize)
      , soci::use(Id), soci::use(Offset);
    if (m_Session.got_data())
      Offset -= CellOffset;
    else
    {
      CellOffset = 0;
      CellSize = 0;
    }

    // Forward
    if (Displacement > 0)
    {
      soci::statement Stmt = (m_Session.prepare <<
        "SELECT size "
        "FROM CellLayout "
        "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
        , soci::into(CellSize)
        , soci::use(Id), soci::use(Offset)
        );
      do
      {
        // If we reached the end of the memory area, we must find the next one
        if (Offset + CellSize >= MemAreaSize)
        {
          // Get the next memory area id
          if (!_GetNextMemoryAreaId(Id, Id))
          {
            return false;
          }
          // Get the first offset, i.e. 0x0
          Offset = 0x0;
        }


        if (!Stmt.execute(true))
          CellSize = 1;
        Offset += CellSize;

        --Displacement;
      } while (Displacement != 0x0);
    }

    // Backward
    else
    {
      soci::statement Stmt = (m_Session.prepare <<
        "SELECT size "
        "FROM CellLayout "
        "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
        , soci::into(CellSize)
        , soci::use(Id), soci::use(Offset)
        );
      do
      {
        // If the offset is 0, we must get the previous memory area and the last offset
        if (Offset == 0x0)
        {
          // Get the previous memory area id
          if (!_GetPreviousMemoryAreaId(Id, Id))
          {
            return false;
          }

          // Get the last offset
          m_Session <<
            "SELECT size "
            "FROM MemoryArea "
            "WHERE :id == id "
            , soci::into(Offset)
            , soci::use(Id);
          if (!m_Session.got_data() || Offset == 0x0)
          {
            Log::Write("db_soci").Level(LogError) << "failed to find memory area size for id: " << Id << LogEnd;
            return false;
          }
          --Offset;
        }

        if (!Stmt.execute(true))
          CellSize = 1;

        Offset -= CellSize;
        ++Displacement;
      } while (Displacement != 0x0);
    }

    // Nothing to do if the user only requests to get the exact position of a cell
  }
  catch (soci::soci_error const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to move address: " << rErr.what() << LogEnd;
    return false;
  }

  // Convert Id/Offset to address
  return _ConvertIdToAddress(Id, Offset, rMovedAddress);
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
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    m_Session <<
      "INSERT INTO Label( name,  type,  version,  memory_area_id,  memory_area_offset)"
      "VALUES           (:name, :type, :version, :memory_area_id, :memory_area_offset)"
      , soci::use(rLabel), soci::use(Id, "memory_area_id"), soci::use(Offset, "memory_area_offset");
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "add label failed: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::RemoveLabel(Address const &rAddress)
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);
  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    m_Session <<
      "DELETE FROM Label "
      "WHERE memory_area_id == :memory_area_id AND memory_area_offset == :memory_area_offset"
      , soci::use(Id), soci::use(Offset);
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "remove label failed: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetLabel(Address const &rAddress, Label &rLabel) const
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);

  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    m_Session <<
      "SELECT name, type, version "
      "FROM Label "
      "WHERE memory_area_id == :memory_area_id AND memory_area_offset == :memory_area_offset"
      , soci::into(rLabel)
      , soci::use(Id), soci::use(Offset);
    if (!m_Session.got_data())
      return false;
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "get label failed: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetLabelAddress(Label const &rLabel, Address &rAddress) const
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);
  Address Address;

  try
  {
    u32 Id, Offset;
    m_Session <<
      "SELECT memory_area_id, memory_area_offset "
      "FROM Label "
      "WHERE :name == name AND :version == version"
      , soci::into(Id), soci::into(Offset)
      , soci::use(rLabel);
    if (!m_Session.got_data())
      return false;
    if (!_ConvertIdToAddress(Id, Offset, rAddress))
      return false;
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "get label address failed: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

void SociDatabase::ForEachLabel(LabelCallback Callback)
{
  std::lock_guard <std::recursive_mutex> Lock(m_LabelLock);
  Label Label;

  try
  {
    soci::rowset<soci::row> rowset = (m_Session.prepare <<
      "SELECT * "
      "FROM Label");

    /*
      "CREATE TABLE IF NOT EXISTS Label("
      "name TEXT, type INTEGER, version INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
    */
    for (auto it = rowset.begin(); it != rowset.end(); ++it)
    {
      soci::row const &row = *it;

      Label.SetName(row.get<std::string>(0));
      Label.SetType(row.get<int>(1));
      Label.SetVersion(row.get<int>(2));

      auto Id     = static_cast<u32>(row.get<int>(3));
      auto Offset = static_cast<OffsetType>(row.get<long long>(4));

      Address Addr;
      if (!_ConvertIdToAddress(Id, Offset, Addr))
      {
        Log::Write("db_soci").Level(LogError) << "failed to convert address label: " << Label.GetName() << LogEnd;
        continue;
      }

      Callback(Addr, Label);
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "for each label failed: " << rErr.what() << LogEnd;
  }
}

bool SociDatabase::AddCrossReference(Address const &rTo, Address const &rFrom)
{
  //TODO: To implement
  std::lock_guard <std::mutex> Lock(m_CrossReferencesLock);

  try
  {
    u32 IdTo, IdFrom;
    OffsetType OffsetTo, OffsetFrom;

    if (!_ConvertAddressToId(rTo, IdTo, OffsetTo))
      return false;
    if (!_ConvertAddressToId(rFrom, IdFrom, OffsetFrom))
      return false;

    /*
    "CREATE TABLE IF NOT EXISTS CrossReference("
      "memory_area_id_from INTEGER, memory_area_offset_from INTEGER,"
      "memory_area_id_to INTEGER,   memory_area_offset_to   INTEGER,"
      "type INTEGER)";
    */

    m_Session <<
      "INSERT INTO CrossReference("
      "memory_area_id_from, memory_area_offset_from ,"
      "memory_area_id_to,   memory_area_offset_to   ,"
      "type)"
      "VALUES("
      ":memory_area_id_from, :memory_area_offset_from ,"
      ":memory_area_id_to,   :memory_area_offset_to   ,"
      "0)"
      , soci::use(IdTo), soci::use(OffsetTo), soci::use(IdFrom), soci::use(OffsetFrom);
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "add cross reference failed: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::RemoveCrossReference(Address const &rFrom)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);

  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rFrom, Id, Offset))
      return false;

    m_Session <<
      "DELETE FROM CrossReference "
      "WHERE :memory_area_id_from == memory_area_id_from AND :memory_area_offset_from == memory_area_offset_from"
      , soci::use(Id), soci::use(Offset);
  }
  catch (soci::soci_error const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to remove cross reference: " << rErr.what() << LogEnd;
    return false;
  }
  return true;
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

MultiCell::SPType SociDatabase::GetMultiCell(Address const &rAddress) const
{
  std::lock_guard <std::recursive_mutex> Lock(m_MultiCellLock);
  MultiCell::SPType spRes;
  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    u32 Type, Size;
    /*
    "CREATE TABLE IF NOT EXISTS MultiCell("
    "type INTEGER, size INTEGER,"
    "memory_area_id INTEGER, memory_area_offset INTEGER)";
    */
    m_Session <<
      "SELECT type, size "
      "FROM MultiCell "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::into(Type), soci::into(Size)
      , soci::use(Id), soci::use(Offset);
    if (!m_Session.got_data())
      return false;

    switch (Type)
    {
    case MultiCell::ArrayType:
      break;

    case MultiCell::FunctionType:
      break;

    case MultiCell::StructType:
      break;

    default:
      break;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get multicell: " << rErr.what() << LogEnd;
    return nullptr;
  }

  return spRes;
}

bool SociDatabase::SetMultiCell(Address const &rAddress, MultiCell::SPType spMultiCell)
{
  std::lock_guard <std::recursive_mutex> Lock(m_MultiCellLock);

  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    switch (spMultiCell->GetType())
    {
    case MultiCell::ArrayType:
      break;

    case MultiCell::FunctionType:
      break;

    case MultiCell::StructType:
      break;

    default:
      break;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "add multicell failed: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::DeleteMultiCell(Address const &rAddress)
{
  std::lock_guard <std::recursive_mutex> Lock(m_MultiCellLock);

  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    u32 Type;
    m_Session <<
      "SELECT type "
      "FROM MultiCell "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::into(Type)
      , soci::use(Id), soci::use(Offset);
    if (!m_Session.got_data())
      return false;

    switch (Type)
    {
    case MultiCell::ArrayType:
      break;

    case MultiCell::FunctionType:
      break;

    case MultiCell::StructType:
      break;

    default:
      break;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "add multicell failed: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetCellData(Address const &rAddress, CellData &rCellData) const
{
  std::lock_guard <std::recursive_mutex> Lock(m_CellLock);

  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    u16 CellOffset;
    m_Session <<
      "SELECT offset "
      "FROM CellLayout "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::into(CellOffset)
      , soci::use(Id), soci::use(Offset);
    if (!m_Session.got_data())
    {
      rCellData = CellData(Cell::ValueType, ValueDetail::HexadecimalType, 1);
      return true;
    }
    Offset -= CellOffset;

    /*
    "CREATE TABLE IF NOT EXISTS CellData("
      "type INTEGER, sub_type INTEGER, size INTEGER,"
      "format_style INTEGER, flags INTEGER,"
      "architecture_tag INTEGER, architecture_mode INTEGER"
      "memory_area_id INTEGER, memory_area_offset INTEGER)"
    */
    m_Session <<
      "SELECT * "
      "FROM CellData "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::into(rCellData)
      , soci::use(Id, "memory_area_id"), soci::use(Offset, "memory_area_offset");
    if (!m_Session.got_data())
    {
      rCellData = CellData(Cell::ValueType, ValueDetail::HexadecimalType, 1);
      return true;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get cell data: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::SetCellData(Address const &rAddress, CellData const &rCellData, Address::List &rDeletedCellAddresses, bool Force)
{
  std::lock_guard <std::recursive_mutex> Lock(m_CellLock);

  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    m_Session << "BEGIN";
    /*
      "CREATE TABLE IF NOT EXISTS CellData("
      "type INTEGER, sub_type INTEGER, size INTEGER,"
      "format_style INTEGER, flags INTEGER,"
      "architecture_tag INTEGER, architecture_mode INTEGER"
      "memory_area_id INTEGER, memory_area_offset INTEGER)";
    */
    m_Session <<
      "INSERT INTO CellData( type,  sub_type,  size,  format_style,  flags,  architecture_tag,  architecture_mode,  memory_area_id,  memory_area_offset) "
      "VALUES              (:type, :sub_type, :size, :format_style, :flags, :architecture_tag, :architecture_mode, :memory_area_id, :memory_area_offset)"
      , soci::use(rCellData), soci::use(Id, "memory_area_id"), soci::use(Offset, "memory_area_offset");

    /*
    "CREATE TABLE IF NOT EXISTS CellLayout("
      "offset INTEGER, size INTEGER "
      "memory_area_id INTEGER, memory_area_offset INTEGER)";
    */
    auto Size = rCellData.GetSize();
    for (auto i = 0UL; i < Size; ++i)
    {
      m_Session <<
        "INSERT INTO CellLayout( offset,  size,  memory_area_id,  memory_area_offset) "
        "VALUES                (:offset, :size, :memory_area_id, :memory_area_offset)"
        , soci::use(i,          "offset")
        , soci::use(Size,       "size")
        , soci::use(Id,         "memory_area_id")
        , soci::use(Offset + i, "memory_area_offset");
    }
    m_Session << "COMMIT";
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to set cell data: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::DeleteCellData(Address const &rAddress)
{
  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    m_Session <<
      "DELETE FROM CellData "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::use(Id), soci::use(Offset);
  }
  catch (soci::soci_error const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to delete cell data: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetComment(Address const &rAddress, std::string &rComment) const
{
  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    m_Session <<
      "SELECT data "
      "FROM Comment as cmt "
      "WHERE :memory_area_id == cmt.memory_area_id AND :memory_area_offset == cmt.memory_area_offset"
      , soci::into(rComment)
      , soci::use(Id), soci::use(Offset);
    if (!m_Session.got_data())
      return false;
  }
  catch (soci::soci_error const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to get comment: " << rErr.what() << LogEnd;
  }

  return true;
}

bool SociDatabase::SetComment(Address const &rAddress, std::string const &rComment)
{
  try
  {
    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    
    m_Session <<
      "DELETE FROM Comment "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::use(Id), soci::use(Offset);
    m_Session <<
      "INSERT INTO Comment (data, memory_area_id, memory_area_offset) "
      "VALUES (:data, :memory_area_id, :memory_area_offset)"
      , soci::use(rComment), soci::use(Id), soci::use(Offset);
  }
  catch (soci::soci_error const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to set comment: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
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
