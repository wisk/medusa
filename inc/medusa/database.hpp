#ifndef __MEDUSA_DATABASE_HPP__
#define __MEDUSA_DATABASE_HPP__

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include "medusa/document.hpp"
#include "medusa/memory_area.hpp"
#include "medusa/multicell.hpp"
#include "medusa/cell.hpp"
#include "medusa/label.hpp"
#include "medusa/xref.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Database
{
public:
  typedef boost::shared_ptr<Database> SharedPtr;
  typedef std::vector<SharedPtr> VectorSharedPtr;

  Database(void);
  virtual ~Database(void);

  virtual std::string GetName(void) const;
  virtual std::string GetExtension(void) const;
  virtual bool IsCompatible(std::wstring const& rDatabasePath) const;

  virtual bool Open(std::wstring const& rFilePath);
  virtual bool Create(std::wstring const& rDatabasePath);
  virtual bool Close(void);

  virtual bool SaveDocument(Document const& rDoc);
  virtual bool SaveMemoryArea(MemoryArea const& rMemArea);
  virtual bool SaveMultiCell(Address const& rAddress, MultiCell const& rMultiCell);
  virtual bool SaveCell(Address const& rAddress, Cell const& rCell);
  virtual bool SaveLabel(Address const& rAddress, Label const& rLabel);
  virtual bool SaveXRef(Address const& rSrcAddr, Address const& rDstAddr);

  virtual bool LoadDocument(Document& rDoc);
  virtual bool LoadMemoryArea(MemoryArea& rMemArea);
  virtual bool LoadMultiCell(Address const& rAddress, MultiCell& rMultiCell);
  virtual bool LoadCell(Address const& rAddress, Cell& rCell);
  virtual bool LoadLabel(Address const& rAddress, Label& rLabel);
  virtual bool LoadXRef(Address& rSrcAddr, Address& rDstAddr);
};

typedef Database* (*TGetDabatase)(void);

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_DATABASE_HPP__
