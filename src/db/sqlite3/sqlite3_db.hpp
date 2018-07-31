#ifndef DB_SQLITE3_HPP
#define DB_SQLITE3_HPP

#include <medusa/namespace.hpp>
#include <medusa/database.hpp>

#include <type_traits>

#include <sqlite_orm/sqlite_orm.h>

namespace sqlite_orm
{
  template<>
  struct type_printer<medusa::Address::Type> : public integer_printer {};

  template<>
  struct statement_binder<medusa::Address::Type> {
    int bind(sqlite3_stmt* stmt, int index, medusa::Address::Type const& value) {
      return statement_binder<medusa::u8>().bind(stmt, index, static_cast<medusa::u8>(value));
    }
  };

  template<>
  struct field_printer<medusa::Address::Type> {
    medusa::u8 operator()(medusa::Address::Type const& t) const {
      return static_cast<medusa::u8>(t);
    }
  };

  template<>
  struct row_extractor<medusa::Address::Type> {
    medusa::Address::Type extract(medusa::u8 raw_value) {
      return static_cast<medusa::Address::Type>(raw_value);
    }

    medusa::Address::Type extract(sqlite3_stmt* stmt, int column_index) {
      auto v = sqlite3_column_int(stmt, column_index);
      return this->extract(static_cast<medusa::u8>(v));
    }
  };
}

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef db_sqlite3_EXPORTS
#  define DB_SQLITE3_EXPORT __declspec(dllexport)
#else
#  define DB_SQLITE3_EXPORT __declspec(dllimport)
#endif
#else
#define DB_SQLITE3_EXPORT
#endif

namespace
{
  class MemoryAreaRelated
  {
  public:
    MemoryAreaRelated(u32 Id = 0x0, u64 Offset = 0x0) : m_Id(Id), m_Offset(Offset) {}

    u32 const& GetMemoryAreaId(void) const { return m_Id; }
    u64 const& GetMemoryAreaOffset(void) const { return m_Offset; }

    void SetMemoryAreaId(u32 Id) { m_Id = Id; }
    void SetMemoryAreaOffset(u64 Offset) { m_Offset = Offset; }

  private:
    u32 m_Id;
    u64 m_Offset;
  };

  struct FileInformationTable
  {
    std::vector<char> Data;
    std::underlying_type<EEndianness>::type Endianness;
    Tag DefaultArchitecture;
    u8 DefaultAddressingType;
    u64 ImageBase;
  };

  struct MemoryAreaTable
  {
    u32 Id;
    std::string Name;
    std::underlying_type<MemoryArea::Type>::type Type;
    std::underlying_type<MemoryArea::Access>::type Access;
    Tag ArchitectureTag;
    u8 ArchitectureMode;
    OffsetType FileOffset;
    u32 FileSize;
    std::underlying_type<Address::Type>::type BaseAddressType;
    u16 BaseAddressBase;
    u64 BaseAddressOffset;
    u8 BaseAddressBaseSize;
    u8 BaseAddressOffsetSize;
    u32 Size;
  };

  struct LabelTable : public MemoryAreaRelated
  {
    LabelTable(MemoryAreaRelated const& rMemAreaRel, Label const& rLbl)
      : MemoryAreaRelated(rMemAreaRel), Name(rLbl.GetName()), Type(rLbl.GetType()), Version(rLbl.GetVersion()) {}

    u32 const& GetMemoryAreaId(void) const { return MemoryAreaRelated::GetMemeoryAreaId(); }
    u64 const& GetMemoryAreaOffset(void) const { return MemoryAreaRelated::GetMemeoryAreaOffset(); }

    void SetMemoryAreaId(u32 Id) { MemoryAreaRelated::SetMemoryAreaId(Id); }
    void SetMemoryAreaOffset(u64 Offset) { MemoryAreaRelated::SetMemoryAreaOffset(Offset); }


    std::string Name;
    u16 Type;
    u16 Version;
  };

  struct CellDataLayoutTable : public MemoryAreaRelated
  {
    u32 Offset;
    u16 Size;
  };

  struct CellDataTable : public MemoryAreaRelated
  {
    u8 Type;
    u8 SubType;
    u16 Size;
    u16 FormatStyle;
    u8 Flags;
    u8 ArchitectureMode;
    Tag ArchitectureTag;
  };

  struct MultiCellTable : public MemoryAreaRelated
  {
    std::underlying_type<MultiCell::Type>::type Type;
    u16 Size;
    std::string GraphViz;
  };

  struct FunctionTable : public MemoryAreaRelated
  {
    u32 InstructionCount;
  };

  struct CrossReferenceTable
  {
    CrossReferenceTable(MemoryAreaRelated const& rTo, MemoryAreaRelated const& rFrom)
      : MemoryAreaIdTo(rTo.GetMemoryAreaId()), MemoryAreaOffsetTo(rTo.GetMemoryAreaOffset())
      , MemoryAreaIdFrom(rFrom.GetMemoryAreaId()), MemoryAreaOffsetFrom(rFrom.GetMemoryAreaOffset())
    {}

    MemoryAreaRelated GetTo(void) const { return { MemoryAreaIdTo, MemoryAreaOffsetTo }; }
    MemoryAreaRelated GetFrom(void) const { return { MemoryAreaIdFrom, MemoryAreaOffsetFrom }; }

    u32 MemoryAreaIdTo;
    u64 MemoryAreaOffsetTo;
    u32 MemoryAreaIdFrom;
    u64 MemoryAreaOffsetFrom;
  };

  struct CommentTable : public MemoryAreaRelated
  {
    std::string Text;
  };

  inline auto MakeStorage(std::string const& rDatabasePath)
  {
    using namespace sqlite_orm;
    return make_storage(rDatabasePath
      , make_table("FileInformation"
        , make_column("data", &FileInformationTable::Data)
        , make_column("endianness", &FileInformationTable::Endianness)
        , make_column("default_architecture", &FileInformationTable::DefaultAddressingType)
        , make_column("default_addressing_type", &FileInformationTable::DefaultAddressingType)
        , make_column("image_base", &FileInformationTable::ImageBase)
      )
      , make_table("MemoryArea"
        , make_column("id", &MemoryAreaTable::Id, autoincrement(), primary_key())
        , make_column("name", &MemoryAreaTable::Name)
        , make_column("type", &MemoryAreaTable::Type)
        , make_column("access", &MemoryAreaTable::Access)
        , make_column("architecture_tag", &MemoryAreaTable::ArchitectureTag)
        , make_column("architecture_mode", &MemoryAreaTable::ArchitectureMode)
        , make_column("file_offset", &MemoryAreaTable::FileOffset)
        , make_column("file_size", &MemoryAreaTable::FileSize)
        , make_column("base_address_type", &MemoryAreaTable::BaseAddressType)
        , make_column("base_address_base", &MemoryAreaTable::BaseAddressBase)
        , make_column("base_address_offset", &MemoryAreaTable::BaseAddressOffset)
        , make_column("base_address_base_size", &MemoryAreaTable::BaseAddressBaseSize)
        , make_column("base_address_offset_size", &MemoryAreaTable::BaseAddressOffsetSize)
        , make_column("size", &MemoryAreaTable::Size)
      )

      , make_table("Label"
        , make_column("memory_area_id", &LabelTable::GetMemoryAreaId, &LabelTable::SetMemoryAreaId)
        , make_column("memory_area_offset", &LabelTable::GetMemoryAreaOffset, &LabelTable::SetMemoryAreaOffset)
        , make_column("name", &LabelTable::Name)
        , make_column("type", &LabelTable::Type)
        , make_column("version", &LabelTable::Version)
      )
      , make_table("CellLayout"
        , make_column("memory_area_id", &CellDataLayoutTable::GetMemoryAreaId, &CellDataLayoutTable::SetMemoryAreaId)
        , make_column("memory_area_offset", &CellDataLayoutTable::GetMemoryAreaOffset, &CellDataLayoutTable::SetMemoryAreaOffset)
        , make_column("offset", &CellDataLayoutTable::Offset)
        , make_column("size", &CellDataLayoutTable::Size)
      )
      , make_table("CellData"
        , make_column("memory_area_id", &CellDataTable::GetMemoryAreaId, &CellDataTable::SetMemoryAreaId)
        , make_column("memory_area_offset", &CellDataTable::GetMemoryAreaOffset, &CellDataTable::SetMemoryAreaOffset)
        , make_column("type", &CellDataTable::Type)
        , make_column("sub_type", &CellDataTable::SubType)
        , make_column("size", &CellDataTable::Size)
        , make_column("format_style", &CellDataTable::FormatStyle)
        , make_column("flags", &CellDataTable::Flags)
        , make_column("architecture_tag", &CellDataTable::ArchitectureTag)
        , make_column("architecture_mode", &CellDataTable::ArchitectureMode)
      )
      , make_table("MultiCell"
        , make_column("memory_area_id", &MultiCellTable::GetMemoryAreaId, &MultiCellTable::SetMemoryAreaId)
        , make_column("memory_area_offset", &MultiCellTable::GetMemoryAreaOffset, &MultiCellTable::SetMemoryAreaOffset)
        , make_column("type", &MultiCellTable::Type)
        , make_column("size", &MultiCellTable::Size)
        , make_column("graphviz", &MultiCellTable::GraphViz)
      )
      , make_table("Function"
        , make_column("memory_area_id", &FunctionTable::GetMemoryAreaId, &FunctionTable::SetMemoryAreaId)
        , make_column("memory_area_offset", &FunctionTable::GetMemoryAreaOffset, &FunctionTable::SetMemoryAreaOffset)
        , make_column("instruction_count", &FunctionTable::InstructionCount)
      )
      , make_table("CrossReference"
        , make_column("memory_area_id_to", &CrossReferenceTable::MemoryAreaIdTo)
        , make_column("memory_area_offset_to", &CrossReferenceTable::MemoryAreaOffsetTo)
        , make_column("memory_area_id_from", &CrossReferenceTable::MemoryAreaIdFrom)
        , make_column("memory_area_offset_from", &CrossReferenceTable::MemoryAreaOffsetFrom)
      )
      , make_table("CommentTable"
        , make_column("memory_area_id", &CommentTable::GetMemoryAreaId, &CommentTable::SetMemoryAreaId)
        , make_column("memory_area_offset", &CommentTable::GetMemoryAreaOffset, &CommentTable::SetMemoryAreaOffset)
        , make_column("text", &CommentTable::Text)
      )
    );
  }
}

class Sqlite3Database : public medusa::Database
{
private:


public:
  Sqlite3Database(void);
  virtual ~Sqlite3Database(void);

private:
  bool _ConfigureDatabase(void);
  bool _CreateTable(void);
  bool _ConvertIdToAddress(u32 Id, OffsetType Offset, Address& rAddress) const;
  bool _ConvertAddressToId(Address const& rAddress, u32& rId, OffsetType& rOffset) const;
  bool _ConvertAddressToId(Address const& rAddress, u32& rId, OffsetType& rOffset, OffsetType& rMemoryAreaOffset, u32& rMemoryAreaSize) const;
  bool _GetNextMemoryAreaId(u32 Id, u32& rNextId) const;
  bool _GetPreviousMemoryAreaId(u32 Id, u32& rPreviousId) const;

  bool _AddCellDataToCache(u32 MemoryAreaId, OffsetType MemoryAreaOffset, CellData const& rCellData);
  bool _FlushCellDataCacheIfRequired(u32 MemoryAreaId, OffsetType MemoryAreaOffset, u16 CellSize) const;
  bool _FlushCellDataCache(void) const;
  bool _GetCellDataFromCache(u32 MemoryAreaId, OffsetType MemoryOffsetType, CellData& rCellData) const;

  bool _AddLabelToCache(u32 MemoryAreaId, OffsetType MemoryAreaOffset, Label const& rLabel);
  bool _FlushLabelCache(void) const;

public:
  virtual std::string GetName(void) const;
  virtual std::string GetExtension(void) const;
  virtual bool IsCompatible(boost::filesystem::path const& rDatabasePath) const;

  virtual bool Open(Path const& rFilePath);
  virtual bool Create(Path const& rDatabasePath, bool Force);
  virtual bool Flush(void);
  virtual bool Close(void);

  // BinaryStream
  //virtual FileBinaryStream const& GetFileBinaryStream(void) const;

  // Architecture
  virtual bool RegisterArchitectureTag(Tag ArchitectureTag);
  virtual bool UnregisterArchitectureTag(Tag ArchitectureTag);
  virtual std::list<Tag> GetArchitectureTags(void) const;

  virtual bool SetArchitecture(Address const& rAddress, Tag ArchitectureTag, u8 Mode, SetArchitectureModeType SetArchMode);

  // Image base
  virtual bool GetImageBase(ImageBaseType& rImageBase) const;
  virtual bool SetImageBase(ImageBaseType ImageBase);

  // MemoryArea
  virtual bool GetMemoryArea(Address const& rAddress, MemoryArea& rMemArea) const;
  virtual void ForEachMemoryArea(MemoryAreaCallback Callback) const;
  virtual bool AddMemoryArea(MemoryArea const& rMemArea);
  virtual bool RemoveMemoryArea(MemoryArea const& rMemArea);
  virtual bool MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress);


  // Address
  virtual bool GetDefaultAddressingType(Address::Type& rAddressType) const;
  virtual bool SetDefaultAddressingType(Address::Type AddressType);
  virtual bool TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const;
  virtual bool GetFirstAddress(Address& rAddress) const;
  virtual bool GetLastAddress(Address& rAddress) const;
  virtual bool MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  virtual bool ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const;
  virtual bool ConvertPositionToAddress(u32 Position, Address& rAddress) const;

  // Label
  virtual bool AddLabel(Address const& rAddress, Label const& rLbl);
  virtual bool RemoveLabel(Address const& rAddress);

  virtual bool GetLabel(Address const& rAddress, Label& rLbl) const;
  virtual bool GetLabelAddress(Label const& rLabel, Address& rAddress) const;

  virtual void ForEachLabel(LabelCallback Callback);

  // CrossRef
  virtual bool AddCrossReference(Address const& rTo, Address const& rFrom);
  virtual bool RemoveCrossReference(Address const& rFrom);
  virtual bool GetCrossReferenceFrom(Address const& rTo, Address::Vector& rFrom) const;
  virtual bool GetCrossReferenceTo(Address const& rFrom, Address::Vector& rTo) const;

  // MultiCell
  virtual MultiCell::SPType GetMultiCell(Address const& rAddress) const;
  virtual bool              SetMultiCell(Address const& rAddress, MultiCell::SPType spMultiCell);
  virtual bool              DeleteMultiCell(Address const& rAddress);

  // Cell (data)
  virtual bool GetCellData(Address const& rAddress, CellData& rCellData) const;
  virtual bool SetCellData(Address const& rAddress, CellData const& rCellData, Address::Vector& rDeletedCellAddresses, bool Force);
  virtual bool DeleteCellData(Address const& rAddress);

  // Comment
  virtual bool GetComment(Address const& rAddress, std::string& rComment) const;
  virtual bool SetComment(Address const& rAddress, std::string const& rComment);

  // Detail
  virtual bool GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const;
  virtual bool SetValueDetail(Id ConstId, ValueDetail const& rConstDtl);

  virtual bool GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const;
  virtual bool SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl);

  virtual bool GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const;
  virtual bool SetStructureDetail(Id StructId, StructureDetail const& rStructDtl);

  virtual bool RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const;
  virtual bool BindDetailId(Address const& rAddress, u8 Index, Id DtlId);
  virtual bool UnbindDetailId(Address const& rAddress, u8 Index);

private:
  mutable std::mutex m_Lock;
  using StorageType = decltype(MakeStorage(""));
  std::shared_ptr<StorageType> m_spStorage;

  typedef std::vector<MemoryArea> MemoryAreaCacheType;
  mutable MemoryAreaCacheType m_MemoryAreaCache;

  typedef std::vector<std::tuple<u32, OffsetType, CellData>> CellDataCacheType;
  mutable CellDataCacheType m_CellDataCache;

  typedef std::map<std::pair<u32, OffsetType>, bool> CellLayoutCacheType;
  CellLayoutCacheType m_CellLayoutCache;

  typedef std::map<std::pair<u32, OffsetType>, Label> LabelCacheType;
  mutable LabelCacheType m_LabelCache;

  static bool _FileExists(boost::filesystem::path const& rFilePath);
  static bool _FileRemoves(boost::filesystem::path const& rFilePath);
};

extern "C" DB_SQLITE3_EXPORT Database* GetDatabase(void);

#endif // !DB_SQLITE3_HPP
