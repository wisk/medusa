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

  virtual            ~Loader(void) {}

  virtual std::string GetName(void) const = 0;
  virtual u8          GetDepth(void) const = 0;
  virtual bool        IsCompatible(BinaryStream const& rBinStrm) = 0;
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs) = 0;
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase) = 0;
  virtual void        FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const = 0;
};

typedef Loader* (*TGetLoader)(void);

MEDUSA_NAMESPACE_END

#endif // MEDUSA_LOADER_HPP
