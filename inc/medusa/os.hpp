#ifndef _MEDUSA_OS_
#define _MEDUSA_OS_

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/exception.hpp"
#include "medusa/types.hpp"
#include "medusa/endian.hpp"
#include "medusa/configuration.hpp"
#include "medusa/address.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/document.hpp"
#include "medusa/context.hpp"
#include "medusa/loader.hpp"
#include "medusa/architecture.hpp"
#include "medusa/analyzer.hpp"

#include <string>
#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT OperatingSystem
{
public:
  typedef boost::shared_ptr<OperatingSystem> SharedPtr;
  typedef std::vector<SharedPtr>             VectorSharedPtr;

  virtual ~OperatingSystem(void) {}

  virtual std::string GetName(void) const = 0;
  virtual bool InitializeCpuContext(Document const& rDoc, CpuContext& rCpuCtxt) const = 0;
  virtual bool InitializeMemoryContext(Document const& rDoc, MemoryContext& rMemCtxt) const = 0;
  virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const = 0;
  virtual bool ProvideDetails(Document& rDoc) const = 0;
};

typedef OperatingSystem* (*TGetOperatingSystem)(void);

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_OS_
