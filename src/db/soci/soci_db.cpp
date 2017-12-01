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
  // FIXME(wisk): wait for locks
}

bool SociDatabase::_ConfigureDatabase(void)
{
  try
  {
    m_Session <<
      "PRAGMA synchronous        = OFF; "
      "PRAGMA journal_mode       = OFF; "
      "PRAGMA locking_mode       = EXCLUSIVE; "
      "PRAGMA temp_store         = MEMORY; "
      "PRAGMA count_changes      = OFF; "
      "PRAGMA page_size          = 4096; "
      "PRAGMA default_cache_size = 700000; "
      "PRAGMA cache_size         = 700000; "
      "PRAGMA compile_options;";
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "failed to configure database: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
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
    m_Session << "CREATE INDEX label_index ON Label (memory_area_id, memory_area_offset)";

    m_Session << "CREATE TABLE IF NOT EXISTS CellData("
      "type INTEGER, sub_type INTEGER, size INTEGER,"
      "format_style INTEGER, flags INTEGER,"
      "architecture_tag INTEGER, architecture_mode INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
    m_Session << "CREATE INDEX cell_data_index ON CellData (memory_area_id, memory_area_offset)";

    m_Session << "CREATE TABLE IF NOT EXISTS CellLayout("
      "offset INTEGER, size INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
    m_Session << "CREATE INDEX cell_layout_index ON CellLayout (memory_area_id, memory_area_offset)";

    m_Session << "CREATE TABLE IF NOT EXISTS MultiCell("
      "type INTEGER, size INTEGER, graphviz STRING,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
    m_Session << "CREATE INDEX multicell_index ON MultiCell (memory_area_id, memory_area_offset)";

    m_Session << "CREATE TABLE IF NOT EXISTS Function("
      "instruction_count INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
    m_Session << "CREATE INDEX function_index ON Function (memory_area_id, memory_area_offset)";

    m_Session << "CREATE TABLE IF NOT EXISTS CrossReference("
      "memory_area_id_to   INTEGER, memory_area_offset_to   BIGINT,"
      "memory_area_id_from INTEGER, memory_area_offset_from BIGINT,"
      "type INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS Comment("
      "data TEXT,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
    m_Session << "CREATE INDEX comment_index ON Comment (memory_area_id, memory_area_offset)";
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
    u64 Size;
    m_Session <<
      "SELECT base, offset, size "
      "FROM MemoryArea "
      "WHERE :id == id"
      , soci::into(Base), soci::into(Offset), soci::into(Size)
      , soci::use(Id);
    if (!m_Session.got_data())
    {
      Log::Write("db_soci").Level(LogError) << "invalid id to fetch next memory area id: " << Id << LogEnd;
      return false;
    }

    m_Session <<
      "SELECT id "
      "FROM MemoryArea "
      "WHERE :base <= base AND :offset <= offset "
      "ORDER BY base ASC, offset ASC LIMIT 1"
      , soci::into(rNextId)
      , soci::use(Base), soci::use(Offset + Size);
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
      "WHERE :base >= base AND :offset > offset "
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

bool SociDatabase::_AddCellDataToCache(u32 MemoryAreaId, OffsetType MemoryAreaOffset, CellData const& rCellData)
{
  if (m_CellDataCache.size() > 0x2000)
  {
    if (!_FlushCellDataCache())
      return false;
  }
  else
  {
    for (u16 i = 0; i < rCellData.GetSize(); ++i)
    {
      auto MemAreaPair = std::make_pair(MemoryAreaId, MemoryAreaOffset + i);
      if (m_CellLayoutCache.find(MemAreaPair) != std::end(m_CellLayoutCache))
      {
        if (!_FlushCellDataCache())
        { }
          return false;
      }
    }
  }

  auto MemAreaPair = std::make_pair(MemoryAreaId, MemoryAreaOffset);
  m_CellDataCache.push_back(std::make_tuple(MemoryAreaId, MemoryAreaOffset, rCellData));

  return true;
}

bool SociDatabase::_FlushCellDataCacheIfRequired(u32 MemoryAreaId, OffsetType MemoryAreaOffset, u16 CellSize) const
{
  if (m_CellDataCache.size() > 0x2000)
  {
    if (!_FlushCellDataCache())
      return false;
  }
  else
  {
    for (u16 i = 0; i < CellSize; ++i)
    {
      auto MemAreaPair = std::make_pair(MemoryAreaId, MemoryAreaOffset + i);
      if (m_CellLayoutCache.find(MemAreaPair) != std::end(m_CellLayoutCache))
      {
        if (!_FlushCellDataCache())
        {
          return false;
        }
        return true;
      }
    }
  }
  return true;
}

bool SociDatabase::_FlushCellDataCache(void) const
{
  u8          CellType;
  u8          CellSubType;
  u16         CellSize;
  u16         CellFmtStyle;
  u8          CellFlags;
  Tag         CellArchTag;
  u8          CellArchMode;

  u32         MemAreaId;
  OffsetType  MemAreaOff;
  OffsetType  CellOff;

  try
  {
    soci::statement CellDataStmt = (m_Session.prepare <<
      "INSERT INTO CellData( type,  sub_type,  size,  format_style,  flags,  architecture_tag,  architecture_mode,  memory_area_id,  memory_area_offset) "
      "VALUES              (:type, :sub_type, :size, :format_style, :flags, :architecture_tag, :architecture_mode, :memory_area_id, :memory_area_offset)"
      , soci::use(CellType, "type"), soci::use(CellSubType, "sub_type"), soci::use(CellSize, "size"), soci::use(CellFmtStyle, "format_style")
      , soci::use(CellFlags, "flags"), soci::use(CellArchTag, "architecture_tag"), soci::use(CellArchMode, "architecture_mode")
      , soci::use(MemAreaId, "memory_area_id"), soci::use(MemAreaOff, "memory_area_offset")
      );
    soci::statement CellLayoutStmt = (m_Session.prepare <<
      "INSERT INTO CellLayout( offset,  size,  memory_area_id,  memory_area_offset) "
      "VALUES                (:offset, :size, :memory_area_id, :memory_area_offset)"
      , soci::use(CellOff,    "offset")
      , soci::use(CellSize,   "size")
      , soci::use(MemAreaId,  "memory_area_id")
      , soci::use(MemAreaOff, "memory_area_offset")
      );

    m_Session << "BEGIN";
    for (auto const& AddrCellDataPair : m_CellDataCache)
    {
      CellType     = std::get<2>(AddrCellDataPair).GetType();
      CellSubType  = std::get<2>(AddrCellDataPair).GetSubType();
      CellSize     = std::get<2>(AddrCellDataPair).GetSize();
      CellFmtStyle = std::get<2>(AddrCellDataPair).GetFormatStyle();
      CellFlags    = std::get<2>(AddrCellDataPair).GetFlags();
      CellArchTag  = std::get<2>(AddrCellDataPair).GetArchitectureTag();
      CellArchMode = std::get<2>(AddrCellDataPair).GetMode();
      MemAreaId    = std::get<0>(AddrCellDataPair);
      MemAreaOff   = std::get<1>(AddrCellDataPair);
      CellDataStmt.execute(true);

      for (CellOff = 0; CellOff < CellSize; ++CellOff)
      {
        CellLayoutStmt.execute(true);
        ++MemAreaOff;
      }
    }
    m_Session << "COMMIT";
    m_CellDataCache.clear();
  }
  catch (std::exception const& rErr)
  {
    m_Session << "ROLLBACK";
    Log::Write("db_soci").Level(LogError) << "error while flushing cell data: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::_GetCellDataFromCache(u32 MemoryAreaId, OffsetType MemoryOffsetType, CellData& rCellData) const
{
  //auto const& itCellData = m_CellDataCache.find(std::make_pair(MemoryAreaId, MemoryOffsetType));
  //if (itCellData == std::end(m_CellDataCache))
  //  return false;
  //rCellData = itCellData->second;
  //return true;
  return false;
}

bool SociDatabase::_AddLabelToCache(u32 MemoryAreaId, OffsetType MemoryAreaOffset, Label const & rLabel)
{
  auto MemAreaPair = std::make_pair(MemoryAreaId, MemoryAreaOffset);
  auto itLabelPair = m_LabelCache.find(MemAreaPair);
  if (itLabelPair != std::end(m_LabelCache))
    if (!_FlushLabelCache())
      return false;
  m_LabelCache[MemAreaPair] = rLabel;
  return true;
}

bool SociDatabase::_FlushLabelCache(void) const
{
  std::string LabelName;
  u16 LabelType;
  u16 LabelVersion;

  u32 MemAreaId;
  OffsetType MemAreaOff;

  try
  {
    /*
      Label("
      "name TEXT, type INTEGER, version INTEGER,"
      "memory_area_id INTEGER, memory_area_offset BIGINT)";
    */
    soci::statement LblStmt = (m_Session.prepare <<
      "INSERT INTO Label( name, type,   version,  memory_area_id,  memory_area_offset) "
      "VALUES           (:name, :type, :version, :memory_area_id, :memory_area_offset)"
      , soci::use(LabelName, "name"), soci::use(LabelType, "type"), soci::use(LabelVersion, "version")
      , soci::use(MemAreaId, "memory_area_id"), soci::use(MemAreaOff, "memory_area_offset")
      );

    m_Session << "BEGIN";
    for (auto const& AddrLblPair : m_LabelCache)
    {
      LabelName    = AddrLblPair.second.GetName();
      LabelType    = AddrLblPair.second.GetType();
      LabelVersion = AddrLblPair.second.GetVersion();
      MemAreaId    = AddrLblPair.first.first;
      MemAreaOff   = AddrLblPair.first.second;
      LblStmt.execute(true);
    }
    m_Session << "COMMIT";
    m_LabelCache.clear();
  }
  catch (std::exception const& rErr)
  {
    m_Session << "ROLLBACK";
    Log::Write("db_soci").Level(LogError) << "error while flushing label: " << rErr.what() << LogEnd;
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
    std::lock_guard<std::mutex> Lock(m_Lock);

    m_Session.open(soci::sqlite3, "dbname=" + rDatabasePath.string());

    _ConfigureDatabase();

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
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }
  return true;
}

bool SociDatabase::Create(medusa::Path const &rDatabasePath, bool Force)
{

  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

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
    _ConfigureDatabase();
    _CreateTable();
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::Flush(void)
{

  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    _FlushCellDataCache();
    _FlushLabelCache();

    return true;
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::Close(void)
{

  try
  {
    Flush();

    std::lock_guard<std::mutex> Lock(m_Lock);
    // TODO(wisk): save binary stream

    m_Session.close();
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::RegisterArchitectureTag(Tag ArchitectureTag)
{

  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    m_Session <<
      "INSERT INTO Architecture(architecture_tag)"
      "VALUES(:tag)"
      , soci::use(ArchitectureTag);
  }
  catch (std::exception const &rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::UnregisterArchitectureTag(Tag ArchitectureTag)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    m_Session <<
      "DELETE FROM Architecture "
      "WHERE architecture_tag == :architecture_tag"
      , soci::use(ArchitectureTag);
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

std::list <Tag> SociDatabase::GetArchitectureTags(void) const
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << rErr.what() << LogEnd;
    return {};
  }

  return{};
}

bool SociDatabase::SetArchitecture(Address const &rAddress, Tag ArchitectureTag, u8 Mode,
  Database::SetArchitectureModeType SetArchMode)
{
  try
  {
    switch (SetArchMode)
    {
    case ByCell:
    {
      std::lock_guard<std::mutex> Lock(m_Lock);

      soci::indicator indicator;
      soci::row row;
      m_Session << "SELECT type FROM CellData", soci::into(row, indicator);
      if (!m_Session.got_data())
      {
        m_Lock.unlock();
        CellData Cell;
        Address::Vector DeletedCell;
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
      break;
    }
    case ByMemoryArea:
    {
      std::lock_guard<std::mutex> Lock(m_Lock);

      m_Session << "UPDATE MemoryArea set architecture_tag = :architecture_tag"
        ", architecture_mode = :architecture_mode", soci::use(ArchitectureTag, "architecture_tag")
        , soci::use(Mode, "architecture_mode");
      break;
    }

    default:
      Log::Write("db_soci").Level(LogError) << "unknown set architecture tag type" << LogEnd;
      return false;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "cannot set architecture: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::GetImageBase(ImageBaseType& rImageBase) const
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

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
    std::lock_guard<std::mutex> Lock(m_Lock);

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
    std::lock_guard<std::mutex> Lock(m_Lock);

    for (auto const& rCurMemArea : m_MemoryAreaCache)
    {
      auto const& rCurMemAreaAddr = rCurMemArea.GetBaseAddress();
      auto const Size = rAddress.GetAddressingType() == Address::PhysicalType ? rCurMemArea.GetFileSize() : rCurMemArea.GetSize();
      if (rAddress.IsBetween(Size, rCurMemAreaAddr))
      {
        rMemArea = rCurMemArea;
        return true;
      }
    }

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
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    MemoryArea MemArea;
    soci::statement Stmt = (m_Session.prepare <<
      "SELECT * "
      "FROM MemoryArea",
      soci::into(MemArea)
      );
    if (!Stmt.execute(true))
      return;
    do
    {
      m_Lock.unlock();
      Callback(MemArea);
      m_Lock.lock();
    } while (Stmt.fetch());
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "for each memory area: " << rErr.what() << rErr.what() << LogEnd;
  }
}

bool SociDatabase::AddMemoryArea(MemoryArea const& rMemArea)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

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

    m_MemoryAreaCache.push_back(rMemArea);
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
    std::lock_guard<std::mutex> Lock(m_Lock);

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
    std::lock_guard<std::mutex> Lock(m_Lock);

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
      std::lock_guard<std::mutex> Lock(m_Lock);

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
        std::lock_guard<std::mutex> Lock(m_Lock);

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
        std::lock_guard<std::mutex> Lock(m_Lock);

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
    std::lock_guard<std::mutex> Lock(m_Lock);

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
    std::lock_guard<std::mutex> Lock(m_Lock);

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

  std::lock_guard<std::mutex> Lock(m_Lock);

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
        if (!Stmt.execute(true))
          CellSize = 1;
        Offset += CellSize;

        // If we reached the end of the memory area, we must find the next one
        if (Offset + CellSize > MemAreaSize)
        {
          // Get the next memory area id
          if (!_GetNextMemoryAreaId(Id, Id))
          {
            return false;
          }
          // Get the first offset, i.e. 0x0
          Offset = 0x0;
        }

        --Displacement;
      } while (Displacement != 0x0);
    }

    // Backward
    else if (Displacement < 0)
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

        if ((Offset + CellSize) <= 0)
          Offset = 0;
        else
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
  std::lock_guard<std::mutex> Lock(m_Lock);
  return false;
}

bool SociDatabase::ConvertPositionToAddress(u32 Position, Address &rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_Lock);
  return false;
}

bool SociDatabase::AddLabel(Address const &rAddress, Label const &rLabel)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    if (!_FlushLabelCache())
      return false;

    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;
    return _AddLabelToCache(Id, Offset, rLabel);
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
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    if (!_FlushLabelCache())
      return false;

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
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    if (!_FlushLabelCache())
      return false;

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
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    if (!_FlushLabelCache())
      return false;

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
  std::string LabelName;
  u16 LabelType, LabelVersion;
  u32 Id;
  OffsetType Offset;

  std::lock_guard<std::mutex> Lock(m_Lock);
  try
  {
    _FlushLabelCache();

    soci::statement Stmt = (m_Session.prepare <<
      "SELECT name, type, version, memory_area_id, memory_area_offset "
      "FROM Label"
      , soci::into(LabelName), soci::into(LabelType), soci::into(LabelVersion), soci::into(Id), soci::into(Offset)
      );
    if (!Stmt.execute(true))
      return;

    /*
    "CREATE TABLE IF NOT EXISTS Label("
    "name TEXT, type INTEGER, version INTEGER,"
    "memory_area_id INTEGER, memory_area_offset BIGINT)";
    */
    do
    {
      Label Label(LabelName, LabelType, LabelVersion);
      Address Addr;
      if (!_ConvertIdToAddress(Id, Offset, Addr))
      {
        Log::Write("db_soci").Level(LogError) << "failed to convert address label: " << Label.GetName() << LogEnd;
        continue;
      }

      m_Lock.unlock();
      try
      {
        Callback(Addr, Label);
      }
      catch (...)
      {
        m_Lock.lock();
        throw;
      }
      m_Lock.lock();
    } while (Stmt.fetch());
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "for each label failed: " << rErr.what() << LogEnd;
  }
}

bool SociDatabase::AddCrossReference(Address const &rTo, Address const &rFrom)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    u32 IdTo, IdFrom;
    OffsetType OffsetTo, OffsetFrom;

    if (!_ConvertAddressToId(rTo, IdTo, OffsetTo))
      return false;
    if (!_ConvertAddressToId(rFrom, IdFrom, OffsetFrom))
      return false;

    /*
    "CREATE TABLE IF NOT EXISTS CrossReference("
      "memory_area_id_to   INTEGER, memory_area_offset_to   INTEGER,"
      "memory_area_id_from INTEGER, memory_area_offset_from INTEGER,"
      "type INTEGER)";
    */

    m_Session <<
      "INSERT INTO CrossReference("
      "memory_area_id_to,   memory_area_offset_to   ,"
      "memory_area_id_from, memory_area_offset_from ,"
      "type)"
      "VALUES("
      ":memory_area_id_to,   :memory_area_offset_to  ,"
      ":memory_area_id_from, :memory_area_offset_from,"
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
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

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

bool SociDatabase::GetCrossReferenceFrom(Address const &rTo, Address::Vector &rFrom) const
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    u32 IdTo, IdFrom;
    OffsetType OffsetTo, OffsetFrom;
    if (!_ConvertAddressToId(rTo, IdTo, OffsetTo))
      return false;
    soci::statement Stmt = (m_Session.prepare <<
      "SELECT memory_area_id_from, memory_area_offset_from "
      "FROM CrossReference "
      "WHERE :memory_area_id_to == memory_area_id_to AND :memory_area_offset_to == memory_area_offset_to"
      , soci::into(IdFrom), soci::into(OffsetFrom)
      , soci::use(IdTo),    soci::use(OffsetTo)
    );
    if (!Stmt.execute(true))
      return false;
    do
    {
      Address From;
      if (!_ConvertIdToAddress(IdFrom, OffsetFrom, From))
      {
        Log::Write("db_soci").Level(LogError) << "failed to convert: " << IdFrom << " " << OffsetFrom << LogEnd;
        return false;
      }
      rFrom.push_back(From);
    } while (Stmt.fetch());
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "get xref from: " << rErr.what() << LogEnd;
    return false;
  }

  return rFrom.empty() ? false : true;
}

bool SociDatabase::GetCrossReferenceTo(Address const &rFrom, Address::Vector &rTo) const
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    u32 IdTo, IdFrom;
    OffsetType OffsetTo, OffsetFrom;
    if (!_ConvertAddressToId(rFrom, IdFrom, OffsetFrom))
      return false;
    soci::statement Stmt = (m_Session.prepare <<
      "SELECT memory_area_id_to, memory_area_offset_to "
      "FROM CrossReference "
      "WHERE :memory_area_id_from == memory_area_id_from AND :memory_area_offset_from == memory_area_offset_from"
      , soci::into(IdTo),  soci::into(OffsetTo)
      , soci::use(IdFrom), soci::use(OffsetFrom)
      );
    if (!Stmt.execute(true))
      return false;
    do
    {
      Address To;
      if (!_ConvertIdToAddress(IdTo, OffsetTo, To))
      {
        Log::Write("db_soci").Level(LogError) << "failed to convert: " << IdTo << " " << OffsetTo << LogEnd;
        return false;
      }
      rTo.push_back(To);
    } while (Stmt.fetch());
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "get xref to: " << rErr.what() << LogEnd;
    return false;
  }

  return rTo.empty() ? false : true;
}

MultiCell::SPType SociDatabase::GetMultiCell(Address const &rAddress) const
{
  MultiCell::SPType spRes;

  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return nullptr;
    u32 Type, Size;
    std::string GraphViz;
    /*
    "CREATE TABLE IF NOT EXISTS MultiCell("
    "type INTEGER, size INTEGER, graphviz STRING"
    "memory_area_id INTEGER, memory_area_offset INTEGER)";
    */
    m_Session <<
      "SELECT type, size, graphviz "
      "FROM MultiCell "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::into(Type), soci::into(Size), soci::into(GraphViz)
      , soci::use(Id), soci::use(Offset);
    if (!m_Session.got_data())
      return nullptr;

    switch (Type)
    {
    case MultiCell::ArrayType:
      break;

    /*
    "CREATE TABLE IF NOT EXISTS Function("
    "size INTEGER, instruction_count INTEGER,"
    "memory_area_id INTEGER, memory_area_offset BIGINT)";
    */
    case MultiCell::FunctionType:
    {
      u32 InstructionCount;
      m_Session <<
        "SELECT instruction_count "
        "FROM Function "
        "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
        , soci::into(InstructionCount), soci::into(InstructionCount);
      spRes = std::make_shared<Function>(Size, InstructionCount);
      break;
    }

    case MultiCell::StructType:
      break;

    default:
      Log::Write("db_soci").Level(LogError) << "unknown multicell type" << LogEnd;
      break;
    }

    if (!GraphViz.empty())
    {
      Graph MultiCellGraph;
      if (!Graph::FromGraphViz(MultiCellGraph, GraphViz))
      {
        Log::Write("db_soci").Level(LogError) << "failed to parse GraphViz code" << LogEnd;
      }
      spRes->SetGraph(std::make_shared<Graph>(MultiCellGraph)); // TODO(wisk): make FromGraphViz returns a shared_ptr
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
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    std::string GraphViz;
    auto spGraph = spMultiCell->GetGraph();
    if (spGraph != nullptr)
    {
      if (!spGraph->ToGraphViz(GraphViz))
        Log::Write("db_soci").Level(LogDebug) << "no graph attached to multicell" << LogEnd;
    }

    /*
    "CREATE TABLE IF NOT EXISTS MultiCell("
    "type INTEGER, size INTEGER, graphviz STRING"
    "memory_area_id INTEGER, memory_area_offset INTEGER)";
    */
    m_Session << "INSERT INTO MultiCell(type, size, graphviz, memory_area_id, memory_area_offset) "
      "VALUES(:type, :size, :graphviz, :memory_area_id, :memory_area_offset)"
      , soci::use(static_cast<int>(spMultiCell->GetType())), soci::use(spMultiCell->GetSize()), soci::use(GraphViz)
      , soci::use(Id), soci::use(Offset);

    switch (spMultiCell->GetType())
    {
    case MultiCell::ArrayType:
      break;

    case MultiCell::FunctionType:
    {
      auto spFunc = std::static_pointer_cast<Function>(spMultiCell);
      if (spFunc == nullptr)
      {
        Log::Write("db_soci").Level(LogError) << "invalid function" << LogEnd;
        return false;
      }
      m_Session << "INSERT INTO Function(instruction_count, memory_area_id, memory_area_offset) "
        "VALUES(:instruction_count, :memory_area_id, :memory_area_offset)"
        , soci::use(spFunc->GetInstructionCount()), soci::use(Id), soci::use(Offset);
      break;
    }

    case MultiCell::StructType:
      break;

    default:
      break;
    }
  }
  catch (std::exception const& rErr)
  {
    Log::Write("db_soci").Level(LogError) << "set multicell failed: " << rErr.what() << LogEnd;
    return false;
  }

  return true;
}

bool SociDatabase::DeleteMultiCell(Address const &rAddress)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

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
  u32 Id;
  OffsetType Offset;

  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    if (!_FlushCellDataCache())
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

bool SociDatabase::SetCellData(Address const &rAddress, CellData const &rCellData, Address::Vector &rDeletedCellAddresses, bool Force)
{
  try
  {
    std::lock_guard<std::mutex> Lock(m_Lock);

    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    if (!_AddCellDataToCache(Id, Offset, rCellData))
      return false;

    u16 CellSize = rCellData.GetSize();
    u32 DelCellMemAreaId;
    OffsetType DelCellMemAreaOff;
    soci::statement SelectStmt = (m_Session.prepare <<
      "SELECT memory_area_id, memory_area_offset "
      "FROM CellLayout "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset >= memory_area_offset AND :memory_area_offset < (memory_area_offset + :cell_size)"
      , soci::into(DelCellMemAreaId), soci::into(DelCellMemAreaOff)
      , soci::use(Id, "memory_area_id"), soci::use(Offset, "memory_area_offset"), soci::use(CellSize, "cell_size")
      );
    if (!SelectStmt.execute(true))
      return true;
    do
    {
      Address DelCellAddr;
      if (!_ConvertIdToAddress(DelCellMemAreaId, DelCellMemAreaOff, DelCellAddr))
        return false;
      rDeletedCellAddresses.push_back(DelCellAddr);
    } while (SelectStmt.fetch());

    m_Session <<
      "DELETE FROM CellData "
      "WHERE :memory_area_id == memory_area_id AND :memory_area_offset == memory_area_offset"
      , soci::use(Id), soci::use(Offset);

    // TODO(wisk): what should we do with CellLayout?
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
    std::lock_guard<std::mutex> Lock(m_Lock);

    u32 Id;
    OffsetType Offset;
    if (!_ConvertAddressToId(rAddress, Id, Offset))
      return false;

    if (!_FlushCellDataCache())
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
    std::lock_guard<std::mutex> Lock(m_Lock);

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
    std::lock_guard<std::mutex> Lock(m_Lock);

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
