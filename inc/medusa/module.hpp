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
#include <unordered_map>

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
  ModuleManager(void) {}
  ~ModuleManager(void) {}
  ModuleManager(ModuleManager const&);
  ModuleManager& operator=(ModuleManager const&);

  bool _LoadModule(Path const& rModulePath, void* const pModule, Module& rModule);

public:
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

  void InitializeModules(Path const& rModPath);

  bool ConvertArchitectureNameToTag(std::string const& rArchitectureName, TagType& rArchitectureTag) const;
  bool ConvertArchitectureNameToTagAndMode(std::string const& rArchitectureName, TagType& rArchitectureTag, u8& rArchitectureMode) const;

  Architecture::SPType    GetArchitecture(TagType ArchTag) const;
  Architecture::VSPType   GetArchitectures(Loader::SPType const spLoader, BinaryStream const& rBinaryStream) const;
  OperatingSystem::SPType GetOperatingSystem(std::string const& rOperatingSystemName) const;

  Loader::SPType          MakeLoader(std::string const& rLoaderName) const;
  Loader::VSPType         MakeAllLoaders(void) const;
  Loader::VSPType         MakeAllLoaders(BinaryStream::SPType spBinStrm) const;
  Database::SPType        MakeDatabase(std::string const& rDatabaseName) const;
  Database::VSPType       MakeAllDatabases(void) const;
  Binding::SPType         MakeBinding(std::string const& rBindingName) const;
  Emulator::SPType        MakeEmulator(std::string const& rEmulatorName) const;
  Compiler::SPType        MakeCompiler(std::string const& rCompilerName) const;

private:
  typedef std::mutex MutexType;
  MutexType m_Mutex;


  // Context free modules
  using ArchitectureMapType = std::unordered_map<TagType, Architecture::SPType>;
  ArchitectureMapType m_Architectures;
  using OperatingSystemMapType = std::unordered_map<std::string, OperatingSystem::SPType>;
  OperatingSystemMapType m_OperatingSystems;

  // Context non-free modules
  using LoaderMakerVectorType = std::vector<TGetLoader>;
  LoaderMakerVectorType m_Loaders;
  using DatabaseMakerMapType = std::unordered_map<std::string, TGetDatabase>;
  DatabaseMakerMapType m_Databases;
  using BindingMakerMapType = std::unordered_map<std::string, TGetBinding>;
  BindingMakerMapType m_Bindings;
  using EmulatorMapType = std::unordered_map<std::string, TGetEmulator>;
  EmulatorMapType m_Emulators;
  using CompilerMapType = std::unordered_map<std::string, TGetCompiler>;
  CompilerMapType m_Compilers;
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_MODULE_HPP
