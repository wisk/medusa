#ifndef __TEXT_DB_HPP__
#define __TEXT_DB_HPP__

#include <medusa/namespace.hpp>
#include <medusa/database.hpp>

#include <fstream>

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
  TextDatabase(void);
  virtual ~TextDatabase(void);

  virtual std::string GetName(void) const;
  virtual std::string GetExtension(void) const;
  virtual bool IsCompatible(std::wstring const& rDatabasePath) const;

  virtual bool Open(std::wstring const& rFilePath);
  virtual bool Create(std::wstring const& rDatabasePath);
  virtual bool Close(void);

  virtual bool SaveDocument(Document const& rDoc);
  virtual bool SaveConfiguration(Configuration const& rCfg);
  virtual bool SaveMemoryArea(MemoryArea const& rMemArea);
  virtual bool SaveMultiCell(Address const& rAddress, MultiCell const& rMultiCell);
  virtual bool SaveCell(Address const& rAddress, Cell const& rCell);
  virtual bool SaveLabel(Address const& rAddress, Label const& rLabel);
  virtual bool SaveXRef(Address const& rSrcAddr, Address const& rDstAddr);

  virtual bool LoadDocument(Document& rDoc);
  virtual bool LoadConfiguration(Configuration& rCfg);
  virtual bool LoadMemoryArea(MemoryArea& rMemArea);
  virtual bool LoadMultiCell(Address const& rAddress, MultiCell& rMultiCell);
  virtual bool LoadCell(Address const& rAddress, Cell& rCell);
  virtual bool LoadLabel(Address const& rAddress, Label& rLabel);
  virtual bool LoadXRef(Address& rSrcAddr, Address& rDstAddr);

private:
  std::fstream m_TextFile;
};

extern "C" DB_TEXT_EXPORT Database* GetDatabase(void);

#endif // __TEXT_DB_HPP__