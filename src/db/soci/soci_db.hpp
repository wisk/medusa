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
  struct type_conversion<Address>
  {
    typedef values base_type;

    static void from_base(values const& val, indicator ind, Address& rAddress)
    {
      if (ind == i_null)
      {
        throw soci_error("Address: null value not allowed for this type");
      }
      rAddress.SetAddressingType(static_cast<Address::Type>(val.get<int>      ("addressing_type")));
      rAddress.SetBase          (static_cast<BaseType>     (val.get<int>      ("base")           ));
      rAddress.SetOffset        (static_cast<OffsetType>   (val.get<long long>("offset")         ));
      rAddress.SetBaseSize      (static_cast<u8>           (val.get<int>      ("base_size")      ));
      rAddress.SetOffsetSize    (static_cast<u8>           (val.get<int>      ("offset_size")    ));
    }

    static void to_base(Address const& rAddress, values& val, indicator& ind)
    {
      val.set("addressing_type", static_cast<u32>(rAddress.GetAddressingType()));
      val.set("base",                             rAddress.GetBase()           );
      val.set("offset",                           rAddress.GetOffset()         );
      val.set("base_size",                        rAddress.GetBaseSize()       );
      val.set("offset_size",                      rAddress.GetOffsetSize()     );
      ind = i_ok;
    }
  };

  template<>
  struct type_conversion<CellData>
  {
    typedef values base_type;

    static void from_base(values const& val, indicator ind, CellData& cd)
    {
      if (ind == i_null)
      {
        cd = CellData(Cell::ValueType, ValueDetail::HexadecimalType, 1);
        return;
      }

      cd = CellData(
        static_cast<u8>( val.get<int>("type")),             static_cast<u8>(val.get<int>("sub_type")), static_cast<u16>(val.get<int>("size")),
        static_cast<u8>( val.get<int>("format_style")),     static_cast<u8>(val.get<int>("flags")),
        static_cast<Tag>(val.get<int>("architecture_tag")), static_cast<u8>(val.get<int>("architecture_mode"))
      );
    }

    static void to_base(CellData const& cd, values& val, indicator& ind)
    {
      val.set("type",              cd.GetType()           );
      val.set("sub_type",          cd.GetSubType()        );
      val.set("size",              cd.GetSize()           );
      val.set("format_style",      cd.GetFormatStyle()    );
      val.set("flags",             cd.GetFlags()          );
      val.set("architecture_tag",  cd.GetArchitectureTag());
      val.set("architecture_mode", cd.GetMode()           );
      ind = i_ok;
    }
  };

  template<>
  struct type_conversion<std::pair<Address, Address> >
  {
    static void from_base(values const& val, indicator ind, std::pair<Address, Address>& rPairAddress)
    {
      if (ind == i_null)
        throw soci_error("CrossReference : null value not allowed for this type");

      type_conversion<Address>::from_base(val, ind, rPairAddress.first );
      type_conversion<Address>::from_base(val, ind, rPairAddress.second);
    }

    static void to_base(std::pair<Address, Address> const& rPairAddress, values& val, indicator& ind)
    {
      type_conversion<Address>::to_base(rPairAddress.first,  val, ind);
      type_conversion<Address>::to_base(rPairAddress.second, val, ind);
    }

  };

  template<>
  struct type_conversion<Label>
  {
    typedef values base_type;

    static void from_base(values const& val, indicator ind, Label& rLabel)
    {
      if (ind == i_null)
        throw soci_error("Label: null value not allowed for this type");

      rLabel = Label(
        val.get<std::string>("name"),
        val.get<int>        ("type"),
        val.get<int>        ("version")
      );
    }

    static void to_base(Label const& rLabel, values& val, indicator& ind)
    {
      val.set("name",    rLabel.GetName() );
      val.set("type",    rLabel.GetType() );
      val.set("version", rLabel.GetVersion());
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
        throw soci_error("MemoryArea: null value not allowed for this type");

      switch (val.get<int>("type"))
      {
      case MemoryArea::UnknownType:
      default:
        break;

      case MemoryArea::VirtualType:
      {
        Address BaseAddress;
        type_conversion<Address>::from_base(val, ind, BaseAddress);
        rMemArea = MemoryArea::CreateVirtual(
          val.get<std::string>("name"),     static_cast<MemoryArea::Access>(val.get<int>("access")),
          BaseAddress,                      val.get<long long>("size"),
          val.get<int>("architecture_tag"), val.get<int>("architecture_mode")
        );
        rMemArea.SetId(val.get<int>("id"));
        return;
      }

      case MemoryArea::MappedType:
      {
        Address BaseAddress;
        type_conversion<Address>::from_base(val, ind, BaseAddress);
        rMemArea = MemoryArea::CreateMapped(
          val.get<std::string>("name"),      static_cast<MemoryArea::Access>(val.get<int>("access")),
          val.get<long long>("file_offset"), val.get<long long>("file_size"),
          BaseAddress,                       val.get<long long>("size"),
          val.get<int>("architecture_tag"),  val.get<int>("architecture_mode")
        );
        rMemArea.SetId(val.get<int>("id"));
        return;
      }

      case MemoryArea::PhysicalType:
      {
        rMemArea = MemoryArea::CreatePhysical(
          val.get<std::string>("name"),      static_cast<MemoryArea::Access>(val.get<int>("access")),
          val.get<long long>("file_offset"), val.get<long long>("file_size"),
          val.get<int>("architecture_tag"),  val.get<int>("architecture_mode")
        );
        rMemArea.SetId(val.get<int>("id"));
        return;
      }

      }
      throw soci_error("MemoryArea: unknown type");
    }

    static void to_base(MemoryArea& rMemArea, values& val, indicator& ind)
    {
      val.set("name", rMemArea.GetName());
      val.set("type", static_cast<u32>(rMemArea.GetType()));
      val.set("access", static_cast<u32>(rMemArea.GetAccess()));
      val.set("architecture_tag", rMemArea.GetArchitectureTag());
      val.set("architecture_mode", rMemArea.GetArchitectureMode());
      val.set("file_offset", rMemArea.GetFileOffset());
      val.set("file_size", rMemArea.GetFileSize());
      type_conversion<Address>::to_base(rMemArea.GetBaseAddress(), val, ind);
      if (ind != i_ok)
        return;
      val.set("size", rMemArea.GetSize());
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
}

class SociDatabase : public medusa::Database
{
public:
  SociDatabase(void);
  virtual ~SociDatabase(void);

private:
  bool _CreateTable(void);
  bool _ConvertIdToAddress(u32 Id, OffsetType Offset, Address& rAddress) const;
  bool _ConvertAddressToId(Address const& rAddress, u32& rId, OffsetType& rOffset) const;
  bool _ConvertAddressToId(Address const& rAddress, u32& rId, OffsetType& rOffset, OffsetType& rMemoryAreaOffset, u32& rMemoryAreaSize) const;
  bool _GetNextMemoryAreaId(u32 Id, u32& rNextId) const;
  bool _GetPreviousMemoryAreaId(u32 Id, u32& rPreviousId) const;

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
  virtual bool RemoveCrossReferences(void);

  virtual bool HasCrossReferenceFrom(Address const& rTo) const;
  virtual bool GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const;

  virtual bool HasCrossReferenceTo(Address const& rFrom) const;
  virtual bool GetCrossReferenceTo(Address const& rFrom, Address::List& rToList) const;

  // MultiCell
  virtual MultiCell::SPType GetMultiCell(Address const& rAddress) const;
  virtual bool              SetMultiCell(Address const& rAddress, MultiCell::SPType spMultiCell);
  virtual bool              DeleteMultiCell(Address const& rAddress);

  // Cell (data)
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
