#ifndef MEDUSA_LOADER_HPP
#define MEDUSA_LOADER_HPP

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/endian.hpp"
#include "medusa/document.hpp"
#include "medusa/configuration.hpp"

#include "medusa/export.hpp"

#include "medusa/architecture.hpp"

#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Loader : public IsConfigurable
{
public:
  typedef std::shared_ptr<Loader> SPType;
  typedef std::vector<SPType>    VSPType;

  virtual             ~Loader(void) {}

  // Generic methods
  virtual std::string GetName(void) const = 0;
  virtual u8          GetDepth(void) const = 0;

  // Only binary stream is needed
  virtual bool                     IsCompatible(BinaryStream const& rBinStrm) const = 0;
  virtual std::string              GetDetailedName(BinaryStream const& rBinStrm) const { return GetName(); }
  virtual std::string              GetSystemName(BinaryStream const& rBinStrm) const { return ""; }
  virtual std::vector<std::string> GetUsedArchitectures(BinaryStream const& rBinStrm) const = 0;

  // Document is required
  virtual bool        Map(Document& rDoc) const = 0;
  virtual bool        Map(Document& rDoc, Address const& rImgBase) const = 0;
  virtual void        Analyze(Document& rDoc) const = 0;
};

typedef Loader* (*TGetLoader)(void);

MEDUSA_NAMESPACE_END

#endif // MEDUSA_LOADER_HPP
