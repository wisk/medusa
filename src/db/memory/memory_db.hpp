#ifndef DB_MEMORY_HPP
#define DB_MEMORY_HPP

#include <medusa/namespace.hpp>
#include <medusa/database.hpp>

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

struct PairHash
{
  template<typename T1, typename T2>
  size_t operator()(std::pair<T1, T2> const& p) const
  {
    size_t h1 = std::hash<T1>{}(p.first);
    size_t h2 = std::hash<T2>{}(p.second);
    return h1 ^ (h2 * 0x9e3779b97f4a7c15 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

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
  ~MemoryDatabase(void) override;

  std::string GetName(void) const override;
  std::string GetExtension(void) const override;
  bool IsCompatible(Path const& rDatabasePath) const override;

  bool Open(Path const& rFilePath) override;
  bool Create(Path const& rDatabasePath, bool Force) override;
  bool Flush(void) override;
  bool Close(void) override;

  // BinaryStream
  //virtual FileBinaryStream const& GetFileBinaryStream(void) const;

  // Architecture
  bool RegisterArchitectureTag(Tag ArchitectureTag) override;
  bool UnregisterArchitectureTag(Tag ArchitectureTag) override;
  std::vector<Tag> GetArchitectureTags(void) const override;

  bool SetArchitecture(Address const& rAddress, Tag ArchitectureTag, u8 Mode, SetArchitectureModeType SetArchMode) override;

  // Image base
  bool GetImageBase(ImageBaseType& rImageBase) const override;
  bool SetImageBase(ImageBaseType ImageBase) override;

  // MemoryArea
  bool GetMemoryArea(Address const& rAddress, MemoryArea& rMemArea) const override;
  void ForEachMemoryArea(MemoryAreaCallback Callback) const override;
  bool AddMemoryArea(MemoryArea const& rMemArea) override;
  bool RemoveMemoryArea(MemoryArea const& rMemArea) override;
  bool MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress) override;


  // Address
  bool GetDefaultAddressingType(Address::Type& rAddressType) const override;
  bool SetDefaultAddressingType(Address::Type AddressType) override;
  bool TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const override;
  bool GetFirstAddress(Address& rAddress) const override;
  bool GetLastAddress(Address& rAddress) const override;
  bool MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const override;
  bool ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const override;
  bool ConvertPositionToAddress(u32 Position, Address& rAddress) const override;

  // Label
  bool AddLabel(Address const& rAddress, Label const& rLbl) override;
  bool RemoveLabel(Address const& rAddress) override;

  bool GetLabel(Address const& rAddress, Label& rLbl) const override;
  bool GetLabelAddress(Label const& rLabel, Address& rAddress) const override;

  void ForEachLabel(LabelCallback Callback) override;

  // CrossRef
  bool AddCrossReference(Address const& rTo, Address const& rFrom) override;
  bool RemoveCrossReference(Address const& rFrom) override;
  bool GetCrossReferenceFrom(Address const& rTo, Address::Vector& rFrom) const override;
  bool GetCrossReferenceTo(Address const& rFrom, Address::Vector& rTo) const override;

  // MultiCell
  MultiCell::SPType GetMultiCell(Address const& rAddress) const override;
  bool              SetMultiCell(Address const& rAddress, MultiCell::SPType spMultiCell) override;
  bool              DeleteMultiCell(Address const& rAddress) override;

  // Cell (data)
  bool GetCellData(Address const& rAddress, CellData& rCellData) const override;
  bool SetCellData(Address const& rAddress, CellData const& rCellData, Address::Vector& rDeletedCellAddresses, bool Force) override;
  bool DeleteCellData(Address const& rAddress) override;

  // Comment
  bool GetComment(Address const& rAddress, std::string& rComment) const override;
  bool SetComment(Address const& rAddress, std::string const& rComment) override;

  // Detail
  bool GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const override;
  bool SetValueDetail(Id ConstId, ValueDetail const& rConstDtl) override;

  bool GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const override;
  bool SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl) override;

  bool GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const override;
  bool SetStructureDetail(Id StructId, StructureDetail const& rStructDtl) override;

  bool RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const override;
  bool BindDetailId(Address const& rAddress, u8 Index, Id DtlId) override;
  bool UnbindDetailId(Address const& rAddress, u8 Index) override;

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

  std::unordered_map<MemoryAreaRelatedType, Label, PairHash> m_LabelsMap;
  std::unordered_map<std::string, MemoryAreaRelatedType> m_ReverseLabelsMap;

  std::unordered_map<MemoryAreaRelatedType, std::vector<MemoryAreaRelatedType>, PairHash> m_CrossReferencesToFrom;
  std::unordered_map<MemoryAreaRelatedType, std::vector<MemoryAreaRelatedType>, PairHash> m_CrossReferencesFromTo;

  std::unordered_map<MemoryAreaRelatedType, MultiCell::SPType, PairHash> m_MultiCellsMap;

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
  std::unordered_map<MemoryAreaRelatedType, CellData, PairHash> m_CellsMap;

  std::unordered_map<MemoryAreaRelatedType, std::string, PairHash> m_CommentsMap;

  std::unordered_map<Id, StructureDetail> m_StructureDetailsMap;
  std::unordered_map<Id, ValueDetail> m_ValueDetailsMap;
  std::unordered_map<Id, FunctionDetail> m_FunctionDetailsMap;

  // Maps address (via MemoryAreaRelatedType) + index to detail ID
  using DetailBindingKey = std::pair<MemoryAreaRelatedType, u8>;
  struct DetailBindingKeyHash {
    size_t operator()(DetailBindingKey const& k) const {
      size_t h1 = PairHash{}(k.first);
      size_t h2 = std::hash<u8>{}(k.second);
      return h1 ^ (h2 * 0x9e3779b97f4a7c15 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
  };
  std::unordered_map<DetailBindingKey, Id, DetailBindingKeyHash> m_DetailBindingsMap;
};

extern "C" DB_MEMORY_EXPORT Database* GetDatabase(void);

#endif // !DB_MEMORY_HPP
