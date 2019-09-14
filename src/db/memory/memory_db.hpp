#ifndef DB_MEMORY_HPP
#define DB_MEMORY_HPP

#include <medusa/namespace.hpp>
#include <medusa/database.hpp>

#include <boost/functional/hash.hpp>

#include <type_traits>
#include <vector>
#include <functional>


MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef db_memory_EXPORTS
#  define DB_MEMORY_EXPORT __declspec(dllexport)
#else
#  define DB_MEMORY_EXPORT __declspec(dllimport)
#endif
#else
#define DB_MEMORY_EXPORT
#endif

class MemoryDatabase : public medusa::Database
{
private:
  bool _ConvertIdToAddress(int Id, OffsetType Offset, u16 Size, Address& rAddress) const;
  bool _ConvertAddressToId(Address const& rAddress, int& rId, OffsetType& rOffset) const;
  bool _ConvertAddressToId(Address const& rAddress, int& rId, OffsetType& rOffset, OffsetType& rMemoryAreaOffset, u32& rMemoryAreaSize) const;
  bool _GetNextMemoryAreaId(MemoryArea const& rMemoryArea, MemoryArea& rNextMemoryArea) const;
  bool _GetPreviousMemoryAreaId(MemoryArea const& rMemoryArea, MemoryArea& rPreviousMemoryArea) const;

public:
  MemoryDatabase(void);
  virtual ~MemoryDatabase(void);

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
  virtual std::vector<Tag> GetArchitectureTags(void) const;

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
  struct MemoryAreaSorter
  {
    bool operator()(MemoryArea const& rLhs, MemoryArea const& rRhs) const
    {
      return rLhs.GetBaseAddress() < rRhs.GetBaseAddress();
    }
  };

  std::vector<Tag> m_ArchitectureTags;

  mutable std::mutex m_Lock;

  Address::Type m_DefaultAddressingType;
  ImageBaseType m_ImageBase;

  std::set<MemoryArea, MemoryAreaSorter> m_MemoryAreas;

  using MemoryAreaRelatedType = std::pair<int, OffsetType>;
  
  std::unordered_map<MemoryAreaRelatedType, Label, boost::hash<MemoryAreaRelatedType>> m_LabelsMap;
  std::unordered_map<std::string, MemoryAreaRelatedType> m_ReverseLabelsMap;

  std::unordered_map<MemoryAreaRelatedType, std::vector<MemoryAreaRelatedType>, boost::hash<MemoryAreaRelatedType>> m_CrossReferencesToFrom;
  std::unordered_map<MemoryAreaRelatedType, std::vector<MemoryAreaRelatedType>, boost::hash<MemoryAreaRelatedType>> m_CrossReferencesFromTo;

  std::unordered_map<MemoryAreaRelatedType, MultiCell::SPType, boost::hash<MemoryAreaRelatedType>> m_MultiCellsMap;

  class CellLayout
  {
  public:
    CellLayout(void) : m_Offset(), m_Size() {}
    bool IsEmpty(void) const { return m_Size == 0x0; }
    void Clear(void) { m_Offset = 0x0; m_Size = 0x0; }
    void Set(u16 CellOffset, u16 CellSize) { m_Offset = CellOffset; m_Size = CellSize; }
    u16 GetOffset(void) const { return m_Offset; }
    u16 GetSize(void) const { return m_Size; }
    u16 GetNextCellOffset(void) const { return m_Size - m_Offset; }

  private:
    u16 m_Offset;
    u16 m_Size;
  };
  using CellLayoutVectorType = std::vector<CellLayout>;
  std::unordered_map<int, CellLayoutVectorType> m_CellLayoutMap;
  std::unordered_map<MemoryAreaRelatedType, CellData, boost::hash<MemoryAreaRelatedType>> m_CellsMap;

  std::unordered_map<MemoryAreaRelatedType, std::string, boost::hash<MemoryAreaRelatedType>> m_CommentsMap;
};

extern "C" DB_MEMORY_EXPORT Database* GetDatabase(void);

#endif // !DB_MEMORY_HPP
