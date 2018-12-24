#include "medusa/module.hpp"
#include "medusa/log.hpp"

#include <algorithm>

#include <boost/filesystem.hpp>

MEDUSA_NAMESPACE_BEGIN

bool ModuleManager::_LoadModule(Path const& rModulePath, void* const pMod, Module& rModule)
{
  auto pGetArchitecture = rModule.Load<TGetArchitecture>(pMod, "GetArchitecture");
  if (pGetArchitecture != nullptr)
  {
    auto pArch = pGetArchitecture();
    if (pArch == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to load " << rModulePath << LogEnd;
      return false;
    }
    if (m_Architectures.find(pArch->GetTag()) != std::end(m_Architectures))
    {
      Log::Write("core").Level(LogWarning) << "module " << pArch->GetName() << " is already loaded" << LogEnd;
      delete pArch;
      return true;
    }
    m_Architectures[pArch->GetTag()] = Architecture::SPType(pArch);
    Log::Write("core").Level(LogInfo) << "architecture module " << pArch->GetName() << " loaded" << LogEnd;
    return true;
  }

  auto pGetOperatingSystem = rModule.Load<TGetOperatingSystem>(pMod, "GetOperatingSystem");
  if (pGetOperatingSystem != nullptr)
  {
    auto pOs = pGetOperatingSystem();
    if (pOs == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to load " << rModulePath << LogEnd;
      return false;
    }
    if (m_OperatingSystems.find(pOs->GetName()) != std::end(m_OperatingSystems))
    {
      Log::Write("core").Level(LogWarning) << "module " << pOs->GetName() << " is already loaded" << LogEnd;
      delete pOs;
      return true;
    }
    m_OperatingSystems[pOs->GetName()] = OperatingSystem::SPType(pOs);
    return true;
  }

  auto pGetLoader = rModule.Load<TGetLoader>(pMod, "GetLoader");
  if (pGetLoader != nullptr)
  {
    auto pLdr = pGetLoader();
    if (pLdr == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to load " << rModulePath << LogEnd;
      return false;
    }
    if (std::find(std::begin(m_Loaders), std::end(m_Loaders), pGetLoader) != std::end(m_Loaders))
    {
      Log::Write("core").Level(LogWarning) << "module " << pLdr->GetName() << " is already loaded" << LogEnd;
      delete pLdr;
      return true;
    }
    m_Loaders.push_back(pGetLoader);
    delete pLdr;
    return true;
  }

  auto pGetBinding = rModule.Load<TGetBinding>(pMod, "GetBinding");
  if (pGetBinding != nullptr)
  {
    // HACK(wisk): instantiate a dummy module to get its name
    auto pBind = pGetBinding();
    if (pBind == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to load " << rModulePath << LogEnd;
      return false;
    }
    if (m_Bindings.find(pBind->GetName()) != std::end(m_Bindings))
    {
      Log::Write("core").Level(LogWarning) << "module " << pBind->GetName() << " is already loaded" << LogEnd;
      delete pBind;
      return true;
    }
    m_Bindings[pBind->GetName()] = pGetBinding;
    Log::Write("core").Level(LogInfo) << "binding module " << pBind->GetName() << " loaded" << LogEnd;
    delete pBind;
    return true;
  }

  auto pGetCompiler = rModule.Load<TGetCompiler>(pMod, "GetCompiler");
  if (pGetCompiler != nullptr)
  {
    // HACK(wisk): instantiate a dummy module to get its name
    auto pComp = pGetCompiler();
    if (pComp == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to load " << rModulePath << LogEnd;
      return false;
    }
    if (m_Compilers.find(pComp->GetName()) != std::end(m_Compilers))
    {
      Log::Write("core").Level(LogWarning) << "module " << pComp->GetName() << " is already loaded" << LogEnd;
      delete pComp;
      return true;
    }
    m_Compilers[pComp->GetName()] = pGetCompiler;
    Log::Write("core").Level(LogInfo) << "compiler module " << pComp->GetName() << " loaded" << LogEnd;
    delete pComp;
    return true;
  }

  auto pGetDatabase = rModule.Load<TGetDatabase>(pMod, "GetDatabase");
  if (pGetDatabase != nullptr)
  {
    auto pDb = pGetDatabase();
    if (pDb == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to load " << rModulePath << LogEnd;
      return false;
    }
    if (m_Databases.find(pDb->GetName()) != std::end(m_Databases))
    {
      Log::Write("core").Level(LogWarning) << "module " << pDb->GetName() << " is already loaded" << LogEnd;
      delete pDb;
      return true;
    }
    m_Databases[pDb->GetName()] = pGetDatabase;
    Log::Write("core").Level(LogInfo) << "database module " << pDb->GetName() << " loaded" << LogEnd;
    delete pDb;
    return true;
  }

  auto pGetEmulator = rModule.Load<TGetEmulator>(pMod, "GetEmulator");
  if (pGetEmulator != nullptr)
  {
    auto pEmul = pGetEmulator();
    if (pEmul == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to load " << rModulePath << LogEnd;
      return false;
    }
    if (m_Emulators.find(pEmul->GetName()) != std::end(m_Emulators))
    {
      Log::Write("core").Level(LogWarning) << "module " << pEmul->GetName() << " is already loaded" << LogEnd;
      return true;
    }
    m_Emulators[pEmul->GetName()] = pGetEmulator;
    Log::Write("core").Level(LogInfo) << "emulator module " << pEmul->GetName() << " loaded" << LogEnd;
    delete pEmul;
    return true;
  }

  Log::Write("core").Level(LogWarning) << "module " << rModulePath << " is unknown" << LogEnd;
  return false;
}

void ModuleManager::InitializeModules(Path const& rModPath)
{
  try
  {
    auto CurDir = rModPath;
    Module Module;

    Log::Write("core") << "Module directory: " << boost::filesystem::system_complete(CurDir) << LogEnd;

    boost::filesystem::directory_iterator End;
    for (boost::filesystem::directory_iterator It(CurDir);
      It != End; ++It)
    {
      auto const& rFilename = It->path().string();

      if (rFilename.substr(rFilename.find_last_of(".") + 1) != Module::GetExtension())
        continue;

      auto FullPath = boost::filesystem::system_complete(*It).string();
      void* pMod = Module.Load(FullPath);

      Log::Write("core") << "Module: \"" << rFilename << "\" ";
      if (pMod == nullptr)
      {
        Log::Write("core") << "can't be loaded" << LogEnd;
        continue;
      }

      _LoadModule(rFilename, pMod, Module);
    }
  }
  catch (std::exception &e)
  {
    Log::Write("core") << e.what() << LogEnd;
  }
}

bool ModuleManager::ConvertArchitectureNameToTag(std::string const& rArchitectureName, TagType& rArchitectureTag) const
{
  for (auto const itArch : m_Architectures)
  {
    if (itArch.second->GetName() == rArchitectureName)
    {
      rArchitectureTag = itArch.first;
      return true;
    }
  }
  return false;
}

bool ModuleManager::ConvertArchitectureNameToTagAndMode(std::string const& rArchitectureName, TagType& rArchitectureTag, u8& rArchitectureMode) const
{
  auto LimitOff = rArchitectureName.find('/');
  if (LimitOff == std::string::npos)
  {
    rArchitectureMode = 0x0;
    return ConvertArchitectureNameToTag(rArchitectureName, rArchitectureTag);
  }
  auto const& rArchName = rArchitectureName.substr(0x0, LimitOff);
  auto const& rArchMode = rArchitectureName.substr(LimitOff + 1);
  for (auto const itArch : m_Architectures)
  {
    if (itArch.second->GetName() == rArchName)
    {
      rArchitectureTag = itArch.first;
      rArchitectureMode = itArch.second->GetModeByName(rArchMode);
      return true;
    }
  }
  return false;
}

Architecture::SPType ModuleManager::GetArchitecture(TagType ArchTag) const
{
  auto itArch = m_Architectures.find(ArchTag);
  if (itArch == std::end(m_Architectures))
    return nullptr;
  return itArch->second;
}

Architecture::VSPType ModuleManager::GetArchitectures(Loader::SPType const spLoader, BinaryStream const& rBinaryStream) const
{
  auto ArchNames = spLoader->GetUsedArchitectures(rBinaryStream);
  Architecture::VSPType Archs;
  for (auto ArchName : ArchNames)
  {
    TagType ArchTag;
    u8 ArchMode;
    if (!ConvertArchitectureNameToTagAndMode(ArchName, ArchTag, ArchMode))
      return {};
    auto spArch = GetArchitecture(ArchTag);
    if (spArch == nullptr)
      return {};
    Archs.push_back(spArch);
  }
  return Archs;
}

OperatingSystem::SPType ModuleManager::GetOperatingSystem(std::string const& rOperatingSystemName) const
{
  auto itOs = m_OperatingSystems.find(rOperatingSystemName);
  if (itOs == std::end(m_OperatingSystems))
    return nullptr;
  return itOs->second;
}

Loader::SPType ModuleManager::MakeLoader(std::string const& rLoaderName) const
{
  for (auto const& rLdrMaker : m_Loaders)
  {
    auto spTmpLdr = Loader::SPType(rLdrMaker());
    if (spTmpLdr == nullptr)
      continue;
    if (spTmpLdr->GetName() == rLoaderName)
      return spTmpLdr;
  }

  return nullptr;
}

Loader::VSPType ModuleManager::MakeAllLoaders(void) const
{
  Loader::VSPType Ldrs;
  for (auto const& rLdrMaker : m_Loaders)
    Ldrs.push_back(Loader::SPType(rLdrMaker()));
  std::sort(std::begin(Ldrs), std::end(Ldrs), [](Loader::SPType spLdr0, Loader::SPType spLdr1)
  {
    return spLdr0->GetDepth() > spLdr1->GetDepth();
  });
  return std::move(Ldrs);
}

Loader::VSPType ModuleManager::MakeAllLoaders(BinaryStream::SPType spBinStrm) const
{
  Loader::VSPType Ldrs;
  for (auto const& rLdrMaker : m_Loaders)
    Ldrs.push_back(Loader::SPType(rLdrMaker()));
  Ldrs.erase(std::remove_if(std::begin(Ldrs), std::end(Ldrs), [&spBinStrm](Loader::SPType spLoader) {
    return spLoader->IsCompatible(*spBinStrm) ? false : true;
  }), std::end(Ldrs));
  std::sort(std::begin(Ldrs), std::end(Ldrs), [](Loader::SPType spLdr0, Loader::SPType spLdr1)
  {
    return spLdr0->GetDepth() > spLdr1->GetDepth();
  });
  return std::move(Ldrs);
}

Database::SPType ModuleManager::MakeDatabase(std::string const& rDatabaseName) const
{
  auto itDb = m_Databases.find(rDatabaseName);
  if (itDb == std::end(m_Databases))
    return nullptr;
  auto pMakeDb = itDb->second;
  return Database::SPType(pMakeDb());
}

Database::VSPType ModuleManager::MakeAllDatabases(void) const
{
  Database::VSPType Dbs;
  for (auto const& itDb : m_Databases)
    Dbs.push_back(Database::SPType(itDb.second()));
  return Dbs;
}

Binding::SPType ModuleManager::MakeBinding(std::string const& rBindingName) const
{
  auto itBind = m_Bindings.find(rBindingName);
  if (itBind == std::end(m_Bindings))
    return nullptr;
  auto pMakeBind = itBind->second;
  return Binding::SPType(pMakeBind());
}

Emulator::SPType ModuleManager::MakeEmulator(std::string const& rEmulatorName) const
{
  auto itEmul = m_Emulators.find(rEmulatorName);
  if (itEmul == std::end(m_Emulators))
    return nullptr;
  auto pMakeEmul = itEmul->second;
  return Emulator::SPType(pMakeEmul());
}

Compiler::SPType ModuleManager::MakeCompiler(std::string const& rCompilerName) const
{
  auto itComp = m_Compilers.find(rCompilerName);
  if (itComp == std::end(m_Compilers))
    return nullptr;
  auto pMakeComp = itComp->second;
  return Compiler::SPType(pMakeComp());
}

MEDUSA_NAMESPACE_END