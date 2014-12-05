#ifndef MEDUSA_OS_HPP
#define MEDUSA_OS_HPP

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

class Medusa_EXPORT OperatingSystem : public IsConfigurable
{
public:
  typedef std::shared_ptr<OperatingSystem> SPType;
  typedef std::vector<SPType>              VSPType;

  virtual ~OperatingSystem(void) {}

  virtual std::string GetName(void) const = 0;
  virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const = 0;

  virtual bool InitializeContext(
    Document const& rDoc,
    CpuContext& rCpuCtxt, MemoryContext& rMemCtxt,
    std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir) const = 0;

  virtual bool AnalyzeFunction(Document& rDoc, Address const& rAddress) = 0;
  virtual Expression::List ExecuteSymbol(Document& rDoc, Address const& rSymAddr) = 0;

  virtual bool ProvideDetails(Document& rDoc) const = 0;
  virtual bool GetValueDetail(Id ValueId, ValueDetail& rValDtl) const = 0;
  virtual bool GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const = 0;
  virtual bool GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const = 0;
};

typedef OperatingSystem* (*TGetOperatingSystem)(void);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_OS_HPP
