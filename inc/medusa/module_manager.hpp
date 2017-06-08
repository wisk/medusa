#ifndef MEDUSA_MODULE_MANAGER_HPP
#define MEDUSA_MODULE_MANAGER_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/module.hpp"

#include <map>
#include <mutex>

MEDUSA_NAMESPACE_BEGIN

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
  ExportedFunctionType ModuleManager::LoadModule(Path const& rModPath, std::string rModName)
  {
    typedef ModuleTraits<ExportedFunctionType> ModuleType;
    Module Mod;
    std::string FullModName = ModuleType::GetPrefix();
    FullModName += rModName;
    FullModName += ".";
    FullModName += Module::GetExtension();

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

#endif // !MEDUSA_MODULE_MANAGER_HPP
