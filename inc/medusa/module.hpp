#ifndef _MEDUSA_MODULE_
#define _MEDUSA_MODULE_

#include "medusa/architecture.hpp"
#include "medusa/database.hpp"
#include "medusa/emulation.hpp"
#include "medusa/loader.hpp"
#include "medusa/os.hpp"

#include "medusa/exception.hpp"
#include "medusa/export.hpp"
#include <map>

MEDUSA_NAMESPACE_BEGIN

//! Module is only used to load medusa modules.
class Module
{
public:
  Module(void) {}
  ~Module(void) {}

  void* Load(std::wstring const& ModulePath)
  {
    return ImplLoadLibrary(ModulePath);
  }

  static wchar_t const* GetExtension(void);

  template<typename FuncType>
  FuncType Load(std::wstring const& ModulePath, std::string const& FunctionName)
  {
    void* pModule = ImplLoadLibrary(ModulePath);
    if (pModule == nullptr)
      throw Exception_System(L"LoadLibraryW/dlopen");

    return reinterpret_cast<FuncType>(ImplGetFunctionAddress(pModule, FunctionName));
  }

  template<typename FuncType>
  FuncType Load(void* pModule, std::string const& FunctionName)
  {
    return reinterpret_cast<FuncType>(ImplGetFunctionAddress(pModule, FunctionName));
  }

private:
  typedef std::map< std::wstring, void* > TModuleMap;

  void* ImplLoadLibrary(std::wstring const& LibraryPath);
  void* ImplGetFunctionAddress(void* pModule, std::string const& FunctionName);

  TModuleMap m_ModuleMap;
};

class Medusa_EXPORT ModuleManager
{
private:
  ModuleManager(void) : m_ArchIdPool(0x0), m_DefaultArchitectureTag(MEDUSA_ARCH_UNK) {}
  ~ModuleManager(void) {}
  ModuleManager(ModuleManager const&);
  ModuleManager& operator=(ModuleManager const&);

public:
  typedef std::map<std::string, TGetEmulator> EmulatorMap;

  static ModuleManager& Instance(void)
  {
    static ModuleManager ModMgr;

    return ModMgr;
  }

  void LoadModules(std::wstring const& rModPath); // TODO: since we can't afford to have binstrm, we should change this method name
  void LoadModules(std::wstring const& rModPath, BinaryStream const& rBinStrm);
  void UnloadModules(void);

  // Architecture
  Architecture::SharedPtr       GetArchitecture(Tag ArchTag) const;
  Architecture::SharedPtr       FindArchitecture(Tag ArchTag) const;
  bool                          RegisterArchitecture(Architecture::SharedPtr spArch);
  bool                          UnregisterArchitecture(Architecture::SharedPtr spArch);
  void                          ResetArchitecture(void);

  TGetEmulator                  GetEmulator(std::string const& rEmulatorName);
  OperatingSystem::SharedPtr    GetOperatingSystem(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch) const;
  Database::SharedPtr           GetDatabase(std::string const& rDatabaseName);
  Database::VectorSharedPtr     GetDatabases(void) const;

  Loader::VectorSharedPtr       GetLoaders(void) const;
  Architecture::VectorSharedPtr GetArchitectures(void) const;

private:
  typedef boost::mutex MutexType;
  MutexType                        m_Mutex;

  u32                              m_ArchIdPool;
  Tag                              m_DefaultArchitectureTag;
  Architecture::TagMap             m_TaggedArchitectures;

  Loader::VectorSharedPtr          m_Loaders;
  Architecture::VectorSharedPtr    m_Architectures;
  Database::VectorSharedPtr        m_Databases;
  OperatingSystem::VectorSharedPtr m_OperatingSystems;
  EmulatorMap                      m_Emulators;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_MODULE_
