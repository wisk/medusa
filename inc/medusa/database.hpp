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

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Database : public IsConfigurable
{
public:
  enum SetArchitectureModeType
  {
    ByCell,
    ByMemoryArea,
  };

  using SPType = std::shared_ptr<Database>;
  using VSPType = std::vector<SPType>;

  using MemoryAreaCallback = std::function<void (MemoryArea const& rMemoryArea)>;
  using LabelCallback = std::function<void (Address const& rAddress, Label const& rLabel)>;

  Database();
  virtual ~Database();

  virtual std::string GetName() const;
  virtual std::string GetExtension() const;
  [[nodiscard]] virtual bool IsCompatible(Path const& rDatabasePath) const;

  [[nodiscard]] virtual bool Open(Path const& rDatabasePath);
  [[nodiscard]] virtual bool Create(Path const& rDatabasePath, bool Force);
  [[nodiscard]] virtual bool Flush();
  [[nodiscard]] virtual bool Close();

  // BinaryStream
  Database& SetBinaryStream(BinaryStream::SPType spBinStrm);
  BinaryStream& GetBinaryStream();

  // Architecture
  [[nodiscard]] virtual bool RegisterArchitectureTag(Tag ArchitectureTag) = 0;
  [[nodiscard]] virtual bool UnregisterArchitectureTag(Tag ArchitectureTag) = 0;
  virtual std::vector<Tag> GetArchitectureTags() const = 0;

  // Operating system
  Database& SetOperatingSystemName(std::string const& rOperatingSystemName);
  std::string GetOperatingSystemName() const;

  // Image base
  [[nodiscard]] virtual bool GetImageBase(ImageBaseType& rImageBase) const = 0;
  [[nodiscard]] virtual bool SetImageBase(ImageBaseType ImageBase) = 0;

  // MemoryArea
  [[nodiscard]] virtual bool GetMemoryArea(Address const& rAddress, MemoryArea& rMemArea) const = 0;
  virtual void ForEachMemoryArea(MemoryAreaCallback Callback) const = 0;
  [[nodiscard]] virtual bool AddMemoryArea(MemoryArea const& rMemArea) = 0;
  [[nodiscard]] virtual bool RemoveMemoryArea(MemoryArea const& rMemArea) = 0;
  [[nodiscard]] virtual bool MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress) = 0;

  [[nodiscard]] virtual bool SetArchitecture(Address const& rAddress, Tag TagArch, u8 Mode, SetArchitectureModeType SetArchMode) = 0;

  // Address
  [[nodiscard]] virtual bool GetDefaultAddressingType(Address::Type& rAddressType) const = 0;
  [[nodiscard]] virtual bool SetDefaultAddressingType(Address::Type AddressType) = 0;
  [[nodiscard]] virtual bool TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const = 0;
  [[nodiscard]] virtual bool GetFirstAddress(Address& rAddress) const = 0;
  [[nodiscard]] virtual bool GetLastAddress(Address& rAddress)  const = 0;
  [[nodiscard]] virtual bool MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const = 0;
  [[nodiscard]] virtual bool ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const = 0;
  [[nodiscard]] virtual bool ConvertPositionToAddress(u32 Position, Address& rAddress) const = 0;

  // Label
  [[nodiscard]] virtual bool AddLabel(Address const& rAddress, Label const& rLbl) = 0;
  [[nodiscard]] virtual bool RemoveLabel(Address const& rAddress) = 0;

  [[nodiscard]] virtual bool GetLabel(Address const& rAddress, Label& rLabel) const = 0;
  [[nodiscard]] virtual bool GetLabelAddress(Label const& rLabel, Address& rAddress) const = 0;
  virtual void ForEachLabel(LabelCallback Callback) = 0;

  // CrossRef
  [[nodiscard]] virtual bool AddCrossReference(Address const& rTo, Address const& rFrom) = 0;
  [[nodiscard]] virtual bool RemoveCrossReference(Address const& rFrom) = 0;
  [[nodiscard]] virtual bool GetCrossReferenceFrom(Address const& rTo, Address::Vector& rFrom) const = 0;
  [[nodiscard]] virtual bool GetCrossReferenceTo(Address const& rFrom, Address::Vector& rTo) const = 0;

  // Cell (data)
  [[nodiscard]] virtual bool GetCellData(Address const& rAddress, CellData& rCellData) const = 0;
  [[nodiscard]] virtual bool SetCellData(Address const& rAddress, CellData const& rCellData, Address::Vector& rDeletedCellAddresses, bool Force) = 0;
  [[nodiscard]] virtual bool DeleteCellData(Address const& rAddress) = 0;

  // MultiCell
  virtual MultiCell::SPType GetMultiCell(Address const& rAddress) const = 0;
  [[nodiscard]] virtual bool              SetMultiCell(Address const& rAddress, MultiCell::SPType spMultiCell) = 0;
  [[nodiscard]] virtual bool              DeleteMultiCell(Address const& rAddress) = 0;

  // Comment
  [[nodiscard]] virtual bool GetComment(Address const& rAddress, std::string& rComment) const = 0;
  [[nodiscard]] virtual bool SetComment(Address const& rAddress, std::string const& rComment) = 0;

  // Detail
  [[nodiscard]] virtual bool GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const = 0;
  [[nodiscard]] virtual bool SetValueDetail(Id ConstId, ValueDetail const& rConstDtl) = 0;

  [[nodiscard]] virtual bool GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const = 0;
  [[nodiscard]] virtual bool SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl) = 0;

  [[nodiscard]] virtual bool GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const = 0;
  [[nodiscard]] virtual bool SetStructureDetail(Id StructId, StructureDetail const& rStructDtl) = 0;

  [[nodiscard]] virtual bool RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const = 0;
  [[nodiscard]] virtual bool BindDetailId(Address const& rAddress, u8 Index, Id DtlId) = 0;
  [[nodiscard]] virtual bool UnbindDetailId(Address const& rAddress, u8 Index) = 0;

protected:
  BinaryStream::SPType m_spBinStrm;
  std::string m_OsName;
};

using TGetDatabase = Database* (*)();

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_DATABASE_HPP
