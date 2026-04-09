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
  std::string GetName() const override;
  bool IsSupported(Loader const& rLdr, Architecture const& rArch) const override;

  bool InitializeContext(
    Document const& rDoc,
    CpuContext& rCpuCtxt, MemoryContext& rMemCtxt,
    std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir) const override;

  bool AnalyzeFunction(Document& rDoc, Address const& rAddress) override;
  Expression::VSPType ExecuteSymbol(Document& rDoc, Address const& rSymAddr) override;

  bool ProvideDetails(Document& rDoc) const override;
  bool GetValueDetail(Id ValueId, ValueDetail& rValDtl) const override;
  bool GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const override;
  bool GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const override;

  bool GetDefaultCallingConvention(Document const& rDoc, std::string& rCallingConvention, Address const& rAddress) const override;
};

extern "C" OS_UNIX_EXPORT OperatingSystem* GetOperatingSystem();

#endif // !OS_UNIX_HPP
