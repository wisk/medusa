#ifndef OS_UNIX_HPP
#define OS_UNIX_HPP

#include <medusa/namespace.hpp>
#include <medusa/os.hpp>

#if defined(_WIN32) || defined(WIN32)
# ifdef os_unix_EXPORTS
#  define OS_UNIX_EXPORT __declspec(dllexport)
# else
#  define OS_UNIX_EXPORT __declspec(dllimport)
# endif
#else
# define OS_UNIX_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class UnixOperatingSystem : public OperatingSystem
{
public:
  virtual std::string GetName(void) const;
  virtual bool InitializeCpuContext(Document const& rDoc, CpuContext& rCpuCtxt) const;
  virtual bool InitializeMemoryContext(Document const& rDoc, MemoryContext& rMemCtxt) const;

  virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const;

  virtual bool AnalyzeFunction(Document& rDoc, Address const& rAddress);
  virtual Expression::List ExecuteSymbol(Document& rDoc, Address const& rSymAddr);

  virtual bool ProvideDetails(Document& rDoc) const;
  virtual bool GetValueDetail(Id ValueId, ValueDetail& rValDtl) const;
  virtual bool GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const;
  virtual bool GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const;
};

extern "C" OS_UNIX_EXPORT OperatingSystem* GetOperatingSystem(void);

#endif // !OS_UNIX_HPP
