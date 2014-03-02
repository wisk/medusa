#ifndef __TEXT_DB_HPP__
#define __TEXT_DB_HPP__

#include <medusa/namespace.hpp>
#include <medusa/database.hpp>
#include <medusa/memory_area.hpp>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
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
  typedef std::set<MemoryArea*, MemoryArea::Compare> MemoryAreaSetType;
  typedef boost::bimap<Address, Label>               LabelBimapType;
  typedef std::unordered_map<Address, MultiCell>     MultiCellMapType;
  typedef std::unordered_map<Address, CellData>      CellDataMapType;
  typedef std::unordered_map<Address, std::string>   CommentMapType;

  TextDatabase(void);
  virtual ~TextDatabase(void);

  virtual std::string GetName(void) const;
  virtual std::string GetExtension(void) const;
  virtual bool IsCompatible(std::wstring const& rDatabasePath) const;

  virtual bool Open(std::wstring const& rFilePath);
  virtual bool Create(std::wstring const& rDatabasePath, bool Force);
  virtual bool Flush(void);
  virtual bool Close(void);

  // BinaryStream
  //virtual FileBinaryStream const& GetFileBinaryStream(void) const;

  // MemoryArea
  virtual bool AddMemoryArea(MemoryArea* pMemArea);
  //virtual RemoveMemoryArea // not implemented
  //virtual MoveMemoryArea // not implemented

  virtual MemoryArea const* GetMemoryArea(Address const& rAddress) const;

  // Label
  virtual bool AddLabel(Address const& rAddress, Label const& rLbl);
  virtual bool RemoveLabel(Address const& rAddress);

  virtual bool GetLabel(Address const& rAddress, Label& rLbl) const;
  virtual bool GetLabelAddress(Label const& rLabel, Address& rAddress) const;

  virtual void ForEachLabel(std::function<void (Address const& rAddress, Label const& rLabel)> LabelPredicat);

  // CrossRef
  virtual bool AddCrossReference(Address const& rTo, Address const& rFrom);
  virtual bool RemoveCrossReference(Address const& rFrom);
  virtual bool RemoveCrossReferences(void);

  virtual bool HasCrossReferenceFrom(Address const& rTo) const;
  virtual bool GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const;

  virtual bool HasCrossReferenceTo(Address const& rFrom) const;
  virtual bool GetCrossReferenceTo(Address const& rFrom, Address& rTo) const;

  // MultiCell
  virtual bool AddMultiCell(Address const& rAddress, MultiCell const& rMultiCell);
  virtual bool RemoveMultiCell(Address const& rAddress);

  virtual bool GetMultiCell(Address const& rAddress, MultiCell& rMultiCell) const;

  // Cell (data)
  virtual bool GetCellData(Address const& rAddress, CellData& rCellData);
  virtual bool SetCellData(Address const& rAddress, CellData const& rCellData);

  // Comment
  virtual bool GetComment(Address const& rAddress, std::string& rComment) const;
  virtual bool SetComment(Address const& rAddress, std::string const& rComment);

private:
  std::fstream       m_TextFile;

  MemoryAreaSetType  m_MemoryAreas;
  mutable std::mutex m_MemoryAreaLock;

  LabelBimapType     m_LabelMap;
  mutable std::recursive_mutex m_LabelLock;

  XRefs              m_CrossReferences;
  mutable std::mutex m_CrossReferencesLock;

  MultiCellMapType   m_MultiCells;
  mutable std::mutex m_MultiCellsLock;

  CommentMapType     m_Comments;
  mutable std::mutex m_CommentsMutex;
};

extern "C" DB_TEXT_EXPORT Database* GetDatabase(void);

#endif // __TEXT_DB_HPP__