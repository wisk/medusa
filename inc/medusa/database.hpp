#ifndef MEDUSA_DATABASE_HPP
#define MEDUSA_DATABASE_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include "medusa/binary_stream.hpp"
#include "medusa/configuration.hpp"
#include "medusa/memory_area.hpp"
#include "medusa/multicell.hpp"
#include "medusa/cell.hpp"
#include "medusa/label.hpp"
#include "medusa/xref.hpp"
#include "medusa/detail.hpp"

#include <boost/filesystem/path.hpp>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Database : public IsConfigurable
{
public:
  enum SetArchitectureModeType
  {
    ByCell,
    ByMemoryArea,
  };

  typedef std::shared_ptr<Database> SPType;
  typedef std::vector<SPType> VSPType;

  typedef std::function<void (MemoryArea const& rMemoryArea)>                MemoryAreaCallback;
  typedef std::function<void (Address const& rAddress, Label const& rLabel)> LabelCallback;

  Database(void);
  virtual ~Database(void);

  virtual std::string GetName(void) const;
  virtual std::string GetExtension(void) const;
  virtual bool IsCompatible(boost::filesystem::path const& rDatabasePath) const;

  virtual bool Open(boost::filesystem::path const& rDatabasePath);
  virtual bool Create(boost::filesystem::path const& rDatabasePath, bool Force);
  virtual bool Flush(void);
  virtual bool Close(void);

  // BinaryStream
  Database& SetBinaryStream(BinaryStream::SPType spBinStrm);
  BinaryStream& GetBinaryStream(void);

  // Architecture
  virtual bool RegisterArchitectureTag(Tag ArchitectureTag) = 0;
  virtual bool UnregisterArchitectureTag(Tag ArchitectureTag) = 0;
  virtual std::list<Tag> GetArchitectureTags(void) const = 0;

  // Operating system
  Database& SetOperatingSystemName(std::string const& rOperatingSystemName);
  std::string GetOperatingSystemName(void) const;

  // Image base
  virtual bool GetImageBase(ImageBaseType& rImageBase) const = 0;
  virtual bool SetImageBase(ImageBaseType ImageBase) = 0;

  // MemoryArea
  virtual bool GetMemoryArea(Address const& rAddress, MemoryArea& rMemArea) const = 0;
  virtual void ForEachMemoryArea(MemoryAreaCallback Callback) const = 0;
  virtual bool AddMemoryArea(MemoryArea const& rMemArea) = 0;
  virtual bool RemoveMemoryArea(MemoryArea const& rMemArea) = 0;
  virtual bool MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress) = 0;

  virtual bool SetArchitecture(Address const& rAddress, Tag TagArch, u8 Mode, SetArchitectureModeType SetArchMode) = 0;

  // Address
  virtual bool GetDefaultAddressingType(Address::Type& rAddressType) const = 0;
  virtual bool SetDefaultAddressingType(Address::Type AddressType) = 0;
  virtual bool TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const = 0;
  virtual bool GetFirstAddress(Address& rAddress) const = 0;
  virtual bool GetLastAddress(Address& rAddress)  const = 0;
  virtual bool MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const = 0;
  virtual bool ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const = 0;
  virtual bool ConvertPositionToAddress(u32 Position, Address& rAddress) const = 0;

  // Label
  virtual bool AddLabel(Address const& rAddress, Label const& rLbl) = 0;
  virtual bool RemoveLabel(Address const& rAddress) = 0;

  virtual bool GetLabel(Address const& rAddress, Label& rLabel) const = 0;
  virtual bool GetLabelAddress(Label const& rLabel, Address& rAddress) const = 0;
  virtual void ForEachLabel(LabelCallback Callback) = 0;

  // CrossRef
  virtual bool AddCrossReference(Address const& rTo, Address const& rFrom) = 0;
  virtual bool RemoveCrossReference(Address const& rFrom) = 0;
  virtual bool GetCrossReferenceFrom(Address const& rTo, Address::Vector& rFrom) const = 0;
  virtual bool GetCrossReferenceTo(Address const& rFrom, Address::Vector& rTo) const = 0;

  // Cell (data)
  virtual bool GetCellData(Address const& rAddress, CellData& rCellData) const = 0;
  virtual bool SetCellData(Address const& rAddress, CellData const& rCellData, Address::Vector& rDeletedCellAddresses, bool Force) = 0;
  virtual bool DeleteCellData(Address const& rAddress) = 0;

  // MultiCell
  virtual MultiCell::SPType GetMultiCell(Address const& rAddress) const = 0;
  virtual bool              SetMultiCell(Address const& rAddress, MultiCell::SPType spMultiCell) = 0;
  virtual bool              DeleteMultiCell(Address const& rAddress) = 0;

  // Comment
  virtual bool GetComment(Address const& rAddress, std::string& rComment) const = 0;
  virtual bool SetComment(Address const& rAddress, std::string const& rComment) = 0;

  // Detail
  virtual bool GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const = 0;
  virtual bool SetValueDetail(Id ConstId, ValueDetail const& rConstDtl) = 0;

  virtual bool GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const = 0;
  virtual bool SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl) = 0;

  virtual bool GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const = 0;
  virtual bool SetStructureDetail(Id StructId, StructureDetail const& rStructDtl) = 0;

  virtual bool RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const = 0;
  virtual bool BindDetailId(Address const& rAddress, u8 Index, Id DtlId) = 0;
  virtual bool UnbindDetailId(Address const& rAddress, u8 Index) = 0;

protected:
  BinaryStream::SPType m_spBinStrm;
  std::string m_OsName;
};

typedef Database* (*TGetDatabase)(void);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_DATABASE_HPP
