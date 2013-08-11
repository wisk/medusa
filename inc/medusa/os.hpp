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

  OperatingSystem(Document& rDoc) : m_rDoc(rDoc) {}
  virtual ~OperatingSystem(void);

  virtual std::string GetName(void) const = 0;
  virtual bool InitializeCpuContext(CpuContext& rCpuCtxt) const = 0;
  virtual bool InitializeMemoryContext(MemoryContext& rMemCtxt) const = 0;
  virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const = 0;
  virtual void AnalyzeFunction(Address const& rFuncAddr, Analyzer& rAnlz) const = 0;

protected:
  Document& m_rDoc;
};

typedef OperatingSystem* (*TGetOperatingSystem)(Document&);

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_OS_