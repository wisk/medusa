#ifndef DB_TEXT_HPP
#define DB_TEXT_HPP

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

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef db_text_EXPORTS
#  define DB_TEXT_EXPORT __declspec(dllexport)
#else
#  define DB_TEXT_EXPORT __declspec(dllimport)
#endif
#else
#define DB_TEXT_EXPORT
#endif

class TextDatabase : public medusa::Database
{
public:
  typedef std::set<MemoryArea*, MemoryArea::Compare>   MemoryAreaSetType;
  typedef boost::bimap<Address, Label>                 LabelBimapType;
  typedef std::unordered_map<Address, MultiCell>       MultiCellMapType;
  typedef std::unordered_map<Address, CellData>        CellDataMapType;
  typedef std::unordered_map<Address, std::string>     CommentMapType;
  typedef std::map<Id, ValueDetail>                    ValueDetailMapType;
  typedef std::map<Id, StructureDetail>                StructureDetailMapType;
  typedef std::map<Id, FunctionDetail>                 FunctioNDetailMapType;
  typedef std::unordered_map<Address, std::vector<Id>> IdMapType;

  TextDatabase(void);
  virtual ~TextDatabase(void);

  virtual std::string GetName(void) const;
  virtual std::string GetExtension(void) const;
  virtual bool IsCompatible(boost::filesystem::path const& rDatabasePath) const;

  virtual bool Open(boost::filesystem::path const& rFilePath);
  virtual bool Create(boost::filesystem::path const& rDatabasePath, bool Force);
  virtual bool Flush(void);
  virtual bool Close(void);

  // BinaryStream
  //virtual FileBinaryStream const& GetFileBinaryStream(void) const;

  // Architecture
  virtual bool RegisterArchitectureTag(Tag ArchitectureTag);
  virtual bool UnregisterArchitectureTag(Tag ArchitectureTag);
  virtual std::list<Tag> GetArchitectureTags(void) const;

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
  static bool _FileExists(boost::filesystem::path const& rFilePath);
  static bool _FileRemoves(boost::filesystem::path const& rFilePath);
  static bool _FileCanCreate(boost::filesystem::path const& rFilePath);

  bool _MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  bool _MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;

  boost::filesystem::path m_DatabasePath;

  std::list<Tag>     m_ArchitectureTags;
  mutable std::mutex m_ArchitectureTagLock;

  MemoryAreaSetType  m_MemoryAreas;
  mutable std::mutex m_MemoryAreaLock;

  LabelBimapType                    m_LabelMap;
  std::unordered_map<Address, bool> m_VisitedLabels;
  std::atomic<bool>                 m_DirtyLabels, m_IsIteratingLabels;
  mutable std::recursive_mutex      m_LabelLock;

  XRefs              m_CrossReferences;
  mutable std::mutex m_CrossReferencesLock;

  MultiCellMapType   m_MultiCells;
  mutable std::mutex m_MultiCellsLock;

  CommentMapType     m_Comments;
  mutable std::mutex m_CommentsMutex;

  ValueDetailMapType     m_ValuesDetail;
  StructureDetailMapType m_StructuresDetail;
  FunctioNDetailMapType  m_FunctionsDetail;
  IdMapType              m_Ids;
  mutable std::mutex     m_DetailMutex;
};

extern "C" DB_TEXT_EXPORT Database* GetDatabase(void);

#endif // !DB_TEXT_HPP
