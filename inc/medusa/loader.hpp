#ifndef _MEDUSA_LOADER_
#define _MEDUSA_LOADER_

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

class Medusa_EXPORT Loader : public IsConfigurable
{
public:
  typedef boost::shared_ptr<Loader> SharedPtr;
  typedef std::vector<SharedPtr>    VectorSharedPtr;

  virtual            ~Loader(void) {}

  virtual std::string GetName(void) const = 0;
  virtual u8          GetDepth(void) const = 0;
  virtual bool        IsCompatible(BinaryStream const& rBinStrm) = 0;
  virtual void        Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs) = 0;
  virtual void        FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const = 0;
};

typedef Loader* (*TGetLoader)(void);

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_LOADER_
