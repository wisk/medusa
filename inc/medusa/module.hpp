#ifndef MEDUSA_MODULE_HPP
#define MEDUSA_MODULE_HPP

#include "medusa/architecture.hpp"
#include "medusa/binding.hpp"
#include "medusa/compilation.hpp"
#include "medusa/database.hpp"
#include "medusa/emulation.hpp"
#include "medusa/loader.hpp"
#include "medusa/os.hpp"

#include "medusa/exception.hpp"
#include "medusa/export.hpp"
#include <map>

MEDUSA_NAMESPACE_BEGIN

//! Module is only used to load medusa modules.
class MEDUSA_EXPORT Module
{
public:
  Module(void) {}
  ~Module(void) {}

  void* Load(Path const& ModulePath)
  {
    return ImplLoadLibrary(ModulePath);
  }

  static char const* GetExtension(void);

  template<typename FuncType>
  FuncType Load(Path const& ModulePath, std::string const& FunctionName)
  {
    void* pModule = ImplLoadLibrary(ModulePath);
    if (pModule == nullptr)
      throw Exception_System("LoadLibraryW/dlopen");

    return reinterpret_cast<FuncType>(ImplGetFunctionAddress(pModule, FunctionName));
  }

  template<typename FuncType>
  FuncType Load(void* pModule, std::string const& FunctionName)
  {
    return reinterpret_cast<FuncType>(ImplGetFunctionAddress(pModule, FunctionName));
  }

private:
  typedef std::map< boost::filesystem::path, void* > TModuleMap;

  void* ImplLoadLibrary(boost::filesystem::path const& LibraryPath);
#ifdef __MINGW32__
  FARPROC WINAPI Module::ImplGetFunctionAddress(void* pModule, std::string const& FunctionName)
#else
  void* ImplGetFunctionAddress(void* pModule, std::string const& FunctionName);
#endif

  TModuleMap m_ModuleMap;
};

template<typename ExportedFunctionType>
struct ModuleTraits
{
  static inline char const* GetPrefix(void);
  static inline char const* GetExportedFunctionName(void);
};

#define DECL_MODULE_TRAITS(mod_name, func_type, prefix, func_name)\
  typedef ModuleTraits<func_type> mod_name;\
  template<> inline char const* mod_name::GetPrefix(void) { return prefix; }\
  template<> inline char const* mod_name::GetExportedFunctionName(void) { return func_name; }

DECL_MODULE_TRAITS(ArchitectureModuleTraits,    TGetArchitecture,    "arch_",   "GetArchitecture")
DECL_MODULE_TRAITS(BindingModuleTraits,         TGetBinding,         "bind_",   "GetBinding")
DECL_MODULE_TRAITS(CompilerModuleTraits,        TGetCompiler,        "compil_", "GetCompiler")
DECL_MODULE_TRAITS(DatabaseModuleTraits,        TGetDatabase,        "db_",     "GetDatabase")
DECL_MODULE_TRAITS(EmulatorModuleTraits,        TGetEmulator,        "emul_",   "GetEmulator")
DECL_MODULE_TRAITS(LoaderModuleTraits,          TGetLoader,          "ldr_",    "GetLoader")
DECL_MODULE_TRAITS(OperatingSystemModuleTraits, TGetOperatingSystem, "os_",     "GetOperatingSystem")

#undef DECL_MODULE_TRAITS

class MEDUSA_EXPORT ModuleManager
{
private:
  ModuleManager(void) : m_ArchIdPool(0x0), m_DefaultArchitectureTag(MEDUSA_ARCH_UNK) {}
  ~ModuleManager(void) {}
  ModuleManager(ModuleManager const&);
  ModuleManager& operator=(ModuleManager const&);

  template<typename ModuleType>
  bool _DoLoad(
    Path const& rModulePath,
    ModuleType pModule,
    std::string const MsgModule,
    std::string const MsgError,
    std::function<void(ModuleType)> pFct);

  bool _DoLoadModule(Path const& rModulePath, void* const pModule, Module& rModule);

public:
  typedef std::map<std::string, TGetBinding>  BindingMap;
  typedef std::map<std::string, TGetEmulator> EmulatorMap;


  static ModuleManager& Instance(void)
  {
    static std::mutex		mutex;
    std::lock_guard<std::mutex> lock(mutex);
    static ModuleManager ModMgr;

    return ModMgr;
  }

  template<typename ExportedFunctionType>
  ExportedFunctionType LoadModule(Path const& rModPath, std::string rModName)
  {
    typedef ModuleTraits<ExportedFunctionType> ModuleType;
    Module Mod;
    std::string FullModName = ModuleType::GetPrefix();
    FullModName += rModName;
    FullModName += ".";
    FullModName += Module::GetExtension();
    auto pModHandle = Mod.Load(rModPath / FullModName);
    if (pModHandle == nullptr)
    {
      FullModName = "lib";
      FullModName += ModuleType::GetPrefix();
      FullModName += rModName;
      FullModName += ".";
      FullModName += Module::GetExtension();
      pModHandle = Mod.Load(rModPath / FullModName);
      if (pModHandle == nullptr)
        return nullptr;
    }

    return (ExportedFunctionType)Mod.Load<ExportedFunctionType>(pModHandle, ModuleType::GetExportedFunctionName());
  }

  void LoadDatabases(boost::filesystem::path const& rModPath); // TODO: since we can't afford to have binstrm, we should change this method name
  void LoadModules(boost::filesystem::path const& rModPath, BinaryStream const& rBinStrm);
  void UnloadModules(void);

  // Architecture
  Architecture::SPType    GetArchitecture(Tag ArchTag) const;
  Architecture::SPType    FindArchitecture(Tag ArchTag) const;
  bool                    RegisterArchitecture(Architecture::SPType spArch);
  bool                    UnregisterArchitecture(Architecture::SPType spArch);
  void                    ResetArchitecture(void);

  TGetBinding             GetBinding(std::string const& rBindingName);
  TGetEmulator            GetEmulator(std::string const& rEmulatorName);
  OperatingSystem::SPType GetOperatingSystem(Loader::SPType spLdr, Architecture::SPType spArch) const;
  OperatingSystem::SPType GetOperatingSystem(std::string const& rOperatingSystemName) const;
  Database::SPType        GetDatabase(std::string const& rDatabaseName);
  Compiler::SPType        GetCompiler(std::string const& rCompilerName);
  Database::VSPType       GetDatabases(void) const;

  Loader::VSPType         GetLoaders(void) const;
  Architecture::VSPType   GetArchitectures(void) const;

private:
  typedef std::mutex MutexType;
  MutexType                m_Mutex;

  u32                      m_ArchIdPool;
  Tag                      m_DefaultArchitectureTag;
  Architecture::TagMap     m_TaggedArchitectures;

  Loader::VSPType          m_Loaders;
  Architecture::VSPType    m_Architectures;
  Database::VSPType        m_Databases;
  Compiler::VSPType        m_Compilers;
  OperatingSystem::VSPType m_OperatingSystems;
  BindingMap               m_Bindings;
  EmulatorMap              m_Emulators;
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_MODULE_HPP
