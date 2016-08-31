#ifndef DB_SOCI_HPP
#define DB_SOCI_HPP

#include <medusa/namespace.hpp>
#include <medusa/database.hpp>
#include <medusa/memory_area.hpp>

#include <boost/bimap.hpp>

#include <fstream>
#include <set>
#include <unordered_map>
#include <mutex>
#include <list>
#include <tuple>
#include <atomic>

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "soci_serializer.hpp"

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef db_soci_EXPORTS
#  define DB_SOCI_EXPORT __declspec(dllexport)
#else
#  define DB_SOCI_EXPORT __declspec(dllimport)
#endif
#else
#define DB_SOCI_EXPORT
#endif

namespace soci
{
  template<>
  struct type_conversion<BinaryStream>
  {
    typedef values base_type;

    static void from_base(values const& val, indicator ind, BinaryStream& rBinaryStream)
    {
      if (ind == i_null)
    	{
    	  throw soci_error("BinaryStream: Null value not allowed for this type");
    	}
      rBinaryStream.SetEndianness(val.get<EEndianness>("endianness"));
    }

    static void to_base(BinaryStream const& rBinaryStream, values& val, indicator& ind)
    {
      //      val.set("path", rBinaryStream.GetPath());
      val.set("size", rBinaryStream.GetSize());
      val.set("sha1", rBinaryStream.GetSha1());
      ind = i_ok;
    }
  };

  template<>
  struct type_conversion<Address>
  {
    typedef values base_type;

    static void from_base(values const& val, indicator ind, Address& rAddress)
    {
      if (ind == i_null)
    	{
    	  throw soci_error("Address: Null value not allowed for this type");
    	}
      rAddress.SetAddressingType(val.get<medusa::Address::Type>("addressing_type"));
      rAddress.SetBase(val.get<u16>("base"));
      rAddress.SetOffset(val.get<u32>("offset"));
      rAddress.SetBaseSize(val.get<u8>("base_size"));
      rAddress.SetOffsetSize(val.get<u8>("offset_size"));
    }

    static void to_base(Address const& rAddress, values& val, indicator& ind)
    {
      val.set("addressing_type", static_cast<u32>(rAddress.GetAddressingType()));
      val.set("base", rAddress.GetBase());
      val.set("offset", rAddress.GetOffset());
      val.set("base_size", rAddress.GetBaseSize());
      val.set("offset_size", rAddress.GetOffsetSize());
      val.set("label_addr", rAddress.Dump());
      ind = i_ok;
    }
  };
  
  template<>
  struct type_conversion<CellData::SPType>
  {
    typedef values base_type;
    
    static void from_base(values const& val, indicator ind, CellData::SPType spCellData)
    {
      if (ind == i_null)
	throw soci_error("CellData: Null value not allowed for this type");

      spCellData->Type() = val.get<u8>("type");
      spCellData->SubType() = val.get<u8>("sub_type");
      spCellData->Length() = val.get<u16>("length");
      spCellData->FormatStyle() = val.get<u16>("format_style");
      spCellData->Flags() = val.get<u8>("flags");
      spCellData->SetDefaultMode(val.get<u8>("architecture_mode"));
      spCellData->SetDefaultArchitectureTag(val.get<Tag>("architecture_tag"));
    }

    static void to_base(CellData::SPType const& spCellData, values& val, indicator& ind)
    {
      val.set("type", spCellData->GetType());
      val.set("sub_type", spCellData->GetSubType());
      val.set("length", spCellData->GetLength());
      val.set("format_style", spCellData->GetFormatStyle());
      val.set("flags", spCellData->Flags());
      val.set("architecture_tag", spCellData->GetArchitectureTag());
      val.set("architecture_mode", spCellData->GetMode());
    }
  };

  template<>
  struct type_conversion<std::pair<Address::SPType, Address::SPType> >
  {
    static void from_base(values const& val, indicator ind, std::pair<Address::SPType,
			  Address::SPType>& rPairAddress)
    {
      if (ind == i_null)
	throw soci_error("CrossReference : Null value not allowed for this type");
      type_conversion<Address>::from_base(val, ind, *(rPairAddress.first));
      type_conversion<Address>::from_base(val, ind, *(rPairAddress.second));
    }

    static void to_base(std::pair<Address::SPType, Address::SPType> const& rPairAddress, values& val, indicator& ind)
    {
      type_conversion<Address>::to_base(*(rPairAddress.first), val, ind);
      type_conversion<Address>::to_base(*(rPairAddress.second), val, ind);
    }
    
  };
  
  template<>
  struct type_conversion<Label>
  {
    typedef values base_type;

    static void from_base(values const& val, indicator ind, Label& rLabel)
    {
      if (ind == i_null)
       	throw soci_error("Label: Null value not allowed for this type");

      rLabel.SetName(val.get<std::string>("name"));
      rLabel.SetNameLength(val.get<u16>("name_length"));
      rLabel.SetType(val.get<u16>("label_type"));
      rLabel.SetVersion(val.get<u32>("version"));
    }

    static void to_base(Label const& rLabel, values& val, indicator& ind)
    {
      val.set("name", rLabel.GetName());
      val.set("name_length", rLabel.GetNameLength());
      val.set("label_type", rLabel.GetType());
      val.set("label", rLabel.GetLabel());
      ind = i_ok;
    }
  };

  template<>
  struct type_conversion<MemoryArea>
  {
    typedef values base_type;
    
    static void from_base(values const& val, indicator ind, MemoryArea& rMemArea)
    {
      if (ind == i_null)
    	throw soci_error("MemoryArea: Null value not allowed for this type");
      
      rMemArea.SetDefaultArchitectureTag(val.get<u32>("architecture_tag"));
      rMemArea.SetDefaultArchitectureMode(val.get<u8>("architecture_mode"));
      rMemArea.SetName(val.get<std::string>("name"));
      rMemArea.SetAccess(val.get<u32>("access"));
      // SetCellData
    }

    static void to_base(MemoryArea& rMemArea, values& val, indicator& ind)
    {
      val.set("name", rMemArea.GetName());
      val.set("access", rMemArea.GetAccess());
      val.set("size", rMemArea.GetSize());
      val.set("architecture_tag", rMemArea.GetArchitectureTag());
      val.set("architecture_mode", rMemArea.GetArchitectureMode());
      val.set("file_offset", rMemArea.GetFileOffset());
      val.set("file_size", rMemArea.GetFileSize());
      //GetBaseAddress
      //GetCellData
      ind = i_ok;
    }
  };

  template<>
  struct type_conversion<MappedMemoryArea>
  {
    typedef values base_type;
    
    static void from_base(values const& val, indicator ind, MappedMemoryArea& rMemArea)
    {
      if (ind == i_null)
    	throw soci_error("MappedMemoryArea: Null value not allowed for this type");
      
      rMemArea.SetDefaultArchitectureTag(val.get<u32>("architecture_tag"));
      rMemArea.SetDefaultArchitectureMode(val.get<u8>("architecture_mode"));
      rMemArea.SetName(val.get<std::string>("name"));
      rMemArea.SetAccess(val.get<u32>("access"));
      rMemArea.SetFileOffset(val.get<TOffset>("file_offset"));
      rMemArea.SetFileSize(val.get<u32>("file_size"));
      rMemArea.SetVirtualSize(val.get<u32>("virtual_size"));
      rMemArea.SetId(val.get<u32>("id"));
      // Get Address virtual base

      // SetCellData
    }

    static void to_base(MappedMemoryArea& rMemArea, values& val, indicator& ind)
    {
      val.set("name", rMemArea.GetName());
      val.set("access", rMemArea.GetAccess());
      val.set("virtual_size", rMemArea.GetSize());
      val.set("architecture_tag", rMemArea.GetArchitectureTag());
      val.set("architecture_mode", rMemArea.GetArchitectureMode());
      val.set("file_offset", rMemArea.GetFileOffset());
      val.set("file_size", rMemArea.GetFileSize());
      val.set("id", rMemArea.GetId());
      //GetBaseAddress
      //GetCellData
      ind = i_ok;
    }
  };

  template<>
  struct type_conversion<MultiCell>
  {
    static void from_base(values const& val, indicator ind, MultiCell& rMultiCell)
    {
      ;
    }

    static void to_base(MultiCell& rMultiCell, values& val, indicator& ind)
    {
      ;
    }
  };

  template<>
  struct type_conversion<VirtualMemoryArea>
  {
    typedef values base_type;
    
    static void from_base(values const& val, indicator ind, VirtualMemoryArea& rMemArea)
    {
      if (ind == i_null)
    	throw soci_error("VirtualMemoryArea: Null value not allowed for this type");
      
      rMemArea.SetDefaultArchitectureTag(val.get<u32>("architecture_tag"));
      rMemArea.SetDefaultArchitectureMode(val.get<u8>("architecture_mode"));
      rMemArea.SetVirtualSize(val.get<u32>("virtual_size"));
      rMemArea.SetId(val.get<u32>("id"));
      // SetCellData
    }

    static void to_base(VirtualMemoryArea& rMemArea, values& val, indicator& ind)
    {
      val.set("name", rMemArea.GetName());
      val.set("access", rMemArea.GetAccess());
      val.set("virtual_size", rMemArea.GetSize());
      val.set("architecture_tag", rMemArea.GetArchitectureTag());
      val.set("architecture_mode", rMemArea.GetArchitectureMode());
      val.set("file_offset", rMemArea.GetFileOffset());
      val.set("file_size", rMemArea.GetFileSize());
      val.set("id", rMemArea.GetId());
      //GetBaseAddress
      //GetCellData
      ind = i_ok;
    }
  };
}

class SociDatabase : public medusa::Database
{
public:
  SociDatabase(void);
  virtual ~SociDatabase(void);

  void	_CreateTable()
  {
    m_Session << "CREATE TABLE IF NOT EXISTS BinaryStream("
      "size INTEGER, buffer BLOB)";

    m_Session << "CREATE TABLE IF NOT EXISTS Architecture(architecture_tag INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS MemoryArea("
      "name TEXT, access INTEGER, size INTEGER, architecture_tag INTEGER, "
      "architecture_mode INTEGER, file_offset INTEGER, file_size INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS MappedMemoryArea("
      "id_memory_area INTEGER PRIMARY KEY, name TEXT, access INTEGER, virtual_size INTEGER, "
      "architecture_tag INTEGER, "
      "architecture_mode INTEGER, file_offset INTEGER, file_size INTEGER,"
      "addressing_type INTEGER, base INTEGER, offset INTEGER, base_size INTEGER,"
      "offset_size INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS VirtualMemoryArea("
      "id_memory_area INTEGER PRIMARY KEY, name TEXT, access INTEGER, virtual_size INTEGER,"
      " architecture_tag INTEGER, "
      "architecture_mode INTEGER, file_offset INTEGER, file_size INTEGER,"
      "addressing_type INTEGER, base INTEGER, offset INTEGER, base_size INTEGER,"
      "offset_size INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS Label(id INTEGER PRIMARY KEY, name TEXT, "
      "name_length INTEGER, label_type INTEGER, version INTEGER, addressing_type INTEGER,"
      " base INTEGER, offset INTEGER, base_size INTEGER, offset_size INTEGER, label_addr TEXT)";

    m_Session << "CREATE TABLE IF NOT EXISTS CellData("
      "id INTEGER, type INTEGER, sub_type INTEGER, length INTEGER, format_style INTEGER, "
      "flags INTEGER, architecture_tag INTEGER, architecture_mode INTEGER, label_addr TEXT"
      ", address_base INTEGER, base_offset INTEGER, address_base_size INTEGER"
      ", address_offset_size INTEGER, id_memory_area INTEGER)";

    m_Session << "CREATE TABLE IF NOT EXISTS CellDataLayout(id_memory_area INTEGER," 
        "cell_offset INTEGER)";
        
    m_Session << "CREATE TABLE IF NOT EXISTS MultiCell("
      "dump TEXT, MultiCell BLOB)";

    m_Session << "CREATE TABLE IF NOT EXISTS CrossReference("
      "type_from INTEGER, sub_type_from INTEGER, length_from INTEGER, format_style_from INTEGER, "
      "architecture_tag_from INTEGER, architecture_mode_from INTEGER,"
      "type_to INTEGER, sub_type_to INTEGER, length_to INTEGER, format_style_to INTEGER, "
      "architecture_tag_to INTEGER, architecture_mode_to INTEGER)";
  }

  bool _PrintColumnProperties(soci::row const& row) const;
  bool _MoveAddressForward(Address const& rAddress, Address& rMovedAddress,
			   s64 Offset) const;
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

  // MemoryArea
  virtual bool AddMemoryArea(MemoryArea* pMemArea);
  //virtual RemoveMemoryArea // not implemented
  //virtual MoveMemoryArea // not implemented
  virtual void ForEachMemoryArea(MemoryAreaCallback Callback) const;

  virtual MemoryArea const* GetMemoryArea(Address const& rAddress) const;

  // Address
  virtual bool GetFirstAddress(Address& rAddress) const;
  virtual bool GetLastAddress(Address& rAddress)  const;
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
  virtual bool RemoveCrossReferences(void);

  virtual bool HasCrossReferenceFrom(Address const& rTo) const;
  virtual bool GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const;

  virtual bool HasCrossReferenceTo(Address const& rFrom) const;
  virtual bool GetCrossReferenceTo(Address const& rFrom, Address::List& rToList) const;

  // MultiCell
  virtual bool AddMultiCell(Address const& rAddress, MultiCell const& rMultiCell);
  virtual bool RemoveMultiCell(Address const& rAddress);

  virtual bool GetMultiCell(Address const& rAddress, MultiCell& rMultiCell) const;

  // Cell (data)
  virtual bool GetCellData(Address const& rAddress, CellData& rCellData);
  virtual bool GetCellData(Address const& rAddress, CellData& rCellData) const;
  virtual bool SetCellData(Address const& rAddress, CellData const& rCellData, Address::List& rDeletedCellAddresses, bool Force);
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
  mutable soci::session m_Session;
  mutable std::mutex m_MemoryAreaLock;
  mutable std::recursive_mutex m_CellLock;
  mutable std::recursive_mutex m_MultiCellLock;
  mutable std::recursive_mutex m_LabelLock;
  mutable std::mutex m_CrossReferencesLock;
  mutable std::mutex m_ArchitectureTagLock;

  static bool _FileExists(boost::filesystem::path const& rFilePath);
  static bool _FileRemoves(boost::filesystem::path const& rFilePath);
};

extern "C" DB_SOCI_EXPORT Database* GetDatabase(void);

#endif // !DB_SOCI_HPP
