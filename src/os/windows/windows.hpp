#ifndef OS_WINDOWS_HPP
#define OS_WINDOWS_HPP

#include <medusa/namespace.hpp>
#include <medusa/os.hpp>

#include "sqlite3.h"

#if defined(_WIN32) || defined(WIN32)
#ifdef os_windows_EXPORTS
#  define OS_WINDOWS_EXPORT __declspec(dllexport)
#else
#  define OS_WINDOWS_EXPORT __declspec(dllimport)
#endif
#else
#define OS_WINDOWS_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class WindowsOperatingSystem : public OperatingSystem
{
public:
  WindowsOperatingSystem(void);
  virtual ~WindowsOperatingSystem(void);

  virtual std::string GetName(void) const;
  virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const;

  virtual bool InitializeContext(
    Document const& rDoc,
    CpuContext& rCpuCtxt, MemoryContext& rMemCtxt,
    std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir) const;

  virtual bool AnalyzeFunction(Document& rDoc, Address const& rAddress);
  virtual Expression::VSPType ExecuteSymbol(Document& rDoc, Address const& rSymAddr);

  virtual bool ProvideDetails(Document& rDoc) const;
  virtual bool GetValueDetail(Id ValueId, ValueDetail& rValDtl) const;
  virtual bool GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const;
  virtual bool GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const;

  virtual bool GetDefaultCallingConvention(Document const& rDoc, std::string& rCallingConvention, Address const& rAddress) const;

private:
  bool _OpenDatabaseIfNeeded(void) const;
  bool _GetTypeFromDatabase(std::string const& rTypeId, TypeDetail& rTpDtl) const;

  mutable sqlite3* m_Database;
};

extern "C" OS_WINDOWS_EXPORT OperatingSystem* GetOperatingSystem(void);

#endif // !OS_WINDOWS_HPP
