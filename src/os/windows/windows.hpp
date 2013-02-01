#ifndef __OS_WINDOWS__
#define __OS_WINDOWS__

#include <medusa/namespace.hpp>
#include <medusa/os.hpp>

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
  WindowsOperatingSystem(Database& rDb) : OperatingSystem(rDb) {}
  ~WindowsOperatingSystem(void);

  virtual std::string GetName(void) const;
  virtual bool InitializeCpuContext(CpuContext& rCpuCtxt) const;
  virtual bool InitializeMemoryContext(MemoryContext& rMemCtxt) const;
  virtual bool IsSupported(Loader const& rLdr, Architecture const& rArch) const;
};

extern "C" OS_WINDOWS_EXPORT OperatingSystem* GetOperatingSystem(Database& rDb);

#endif // !__OS_WINDOWS__