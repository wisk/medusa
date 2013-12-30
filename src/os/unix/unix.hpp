#ifndef __OS_UNIX__
#define __OS_UNIX__

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
  UnixOperatingSystem(Document& rDoc) : OperatingSystem(rDoc) {}
  ~UnixOperatingSystem(void) {}

  virtual std::string GetName(void) const;
  virtual bool InitializeCpuContext(CpuContext& rCpuCtxt) const;
  virtual bool InitializeMemoryContext(MemoryContext& rMemCtxt) const;
  virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const;
  virtual void AnalyzeFunction(Address const& rFuncAddr, Analyzer& rAnlz) const;
};

extern "C" OS_UNIX_EXPORT OperatingSystem* GetOperatingSystem(Document& rDoc);

#endif // !__OS_UNIX__
