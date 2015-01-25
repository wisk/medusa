#include "medusa/module.hpp"
#include "medusa/log.hpp"

#include <algorithm>

#include <boost/filesystem.hpp>

MEDUSA_NAMESPACE_BEGIN

void ModuleManager::LoadDatabases(boost::filesystem::path const& rModPath)
{
  try
  {
    const boost::filesystem::path CurDir = rModPath;
    Module Module;

    Log::Write("core") << "Module directory: " << boost::filesystem::system_complete(CurDir) << LogEnd;

    boost::filesystem::directory_iterator End;
    for (boost::filesystem::directory_iterator It(CurDir);
      It != End; ++It)
    {
      auto const& rFilename = It->path().string();

      if (rFilename.substr(rFilename.find_last_of(".") + 1) != Module::GetExtension())
        continue;

      auto FullPath = boost::filesystem::system_complete(*It);
      Log::Write("core") << "Module: \"" << rFilename << "\" ";

      void* pMod = Module.Load(FullPath);
      if (pMod == nullptr)
      {
        Log::Write("core") << "can't be loaded" << LogEnd;
        continue;
      }

      auto pGetArchitecture = Module.Load<TGetArchitecture>(pMod, "GetArchitecture");
      if (pGetArchitecture != nullptr)
      {
        Log::Write("core") << "is an architecture" << LogEnd;

        auto pArchitecture = pGetArchitecture();
        if (pArchitecture == nullptr)
        {
          Log::Write("core") << "unable to get architecture" << LogEnd;
          continue;
        }
        Architecture::SPType ArchitecturePtr(pArchitecture);
        m_Architectures.push_back(ArchitecturePtr);
        continue;
      }

      auto pGetBinding = Module.Load<TGetBinding>(pMod, "GetBinding");
      if (pGetBinding != nullptr)
      {
        Log::Write("core") << "is a binding" << LogEnd;

        auto pBinding = pGetBinding();
        if (pBinding == nullptr)
        {
          Log::Write("core") << "unable to get binding" << LogEnd;
          continue;
        }
        m_Bindings[pBinding->GetName()] = pGetBinding;
        delete pBinding;
        
        continue;
      }

      auto pGetDatabase = Module.Load<TGetDatabase>(pMod, "GetDatabase");
      if (pGetDatabase != nullptr)
      {
        Log::Write("core") << "is a database" << LogEnd;

        auto pDatabase = pGetDatabase();
        if (pDatabase == nullptr)
        {
          Log::Write("core") << "unable to get database" << LogEnd;
          continue;
        }
        Database::SPType spDatabase(pDatabase);
        m_Databases.push_back(spDatabase);
        continue;
      }

      auto pGetEmulator = Module.Load<TGetEmulator>(pMod, "GetEmulator");
      if (pGetEmulator != nullptr)
      {
        Log::Write("core") << "is an emulator" << LogEnd;

        auto pEmulator = pGetEmulator(nullptr, nullptr, nullptr);
        if (pEmulator == nullptr)
        {
          Log::Write("core") << "unable to get emulator" << LogEnd;
          continue;
        }
        m_Emulators[pEmulator->GetName()] = pGetEmulator;
        delete pEmulator;
        continue;
      }

      auto pGetOperatingSystem = Module.Load<TGetOperatingSystem>(pMod, "GetOperatingSystem");
      if (pGetOperatingSystem != nullptr)
      {
        Log::Write("core") << "is an operating system" << LogEnd;

        auto pOperatingSystem = pGetOperatingSystem();
        if (pOperatingSystem == nullptr)
        {
          Log::Write("core") << "unable to get operating system" << LogEnd;
          continue;
        }
        OperatingSystem::SPType spOperatingSystem(pOperatingSystem);
        m_OperatingSystems.push_back(spOperatingSystem);
        continue;
      }

      Log::Write("core") << "is unknown (ignored)" << LogEnd;
    }
  }
  catch (std::exception &e)
  {
    Log::Write("core") << e.what() << LogEnd;
  }
}

void ModuleManager::LoadModules(boost::filesystem::path const& rModPath, BinaryStream const& rBinStrm)
{
  try
  {
    const boost::filesystem::path CurDir = rModPath;
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
      Log::Write("core") << "Module: \"" << rFilename << "\" ";

      void* pMod = Module.Load(FullPath);
      if (pMod == nullptr)
      {
        Log::Write("core") << "can't be loaded" << LogEnd;
        continue;
      }

      TGetLoader pGetLoader = Module.Load<TGetLoader>(pMod, "GetLoader");
      if (pGetLoader != nullptr)
      {
        Log::Write("core") << "is a loader ";

        Loader* pLoader = pGetLoader();
        if (pLoader->IsCompatible(rBinStrm))
        {
          Loader::SPType LoaderPtr(pLoader);
          m_Loaders.push_back(LoaderPtr);
          Log::Write("core") << "(loaded)" << LogEnd;
        }
        else
        {
          Log::Write("core") << "(unloaded)" << LogEnd;
          delete pLoader;
        }
        continue;
      }

      auto pGetArchitecture = Module.Load<TGetArchitecture>(pMod, "GetArchitecture");
      if (pGetArchitecture != nullptr)
      {
        Log::Write("core") << "is an architecture" << LogEnd;

        auto pArchitecture = pGetArchitecture();
        if (pArchitecture == nullptr)
        {
          Log::Write("core") << "unable to get architecture" << LogEnd;
          continue;
        }
        Architecture::SPType ArchitecturePtr(pArchitecture);
        m_Architectures.push_back(ArchitecturePtr);
        continue;
      }

      auto pGetBinding = Module.Load<TGetBinding>(pMod, "GetBinding");
      if (pGetBinding != nullptr)
      {
        Log::Write("core") << "is a binding" << LogEnd;

        auto pBinding = pGetBinding();
        if (pBinding == nullptr)
        {
          Log::Write("core") << "unable to get binding" << LogEnd;
          continue;
        }
        m_Bindings[pBinding->GetName()] = pGetBinding;
        delete pBinding;

        continue;
      }

      auto pGetDatabase = Module.Load<TGetDatabase>(pMod, "GetDatabase");
      if (pGetDatabase != nullptr)
      {
        Log::Write("core") << "is a database" << LogEnd;

        auto pDatabase = pGetDatabase();
        if (pDatabase == nullptr)
        {
          Log::Write("core") << "unable to get database" << LogEnd;
          continue;
        }
        Database::SPType spDatabase(pDatabase);
        m_Databases.push_back(spDatabase);
        continue;
      }

      auto pGetEmulator = Module.Load<TGetEmulator>(pMod, "GetEmulator");
      if (pGetEmulator != nullptr)
      {
        Log::Write("core") << "is an emulator" << LogEnd;

        auto pEmulator = pGetEmulator(nullptr, nullptr, nullptr);
        if (pEmulator == nullptr)
        {
          Log::Write("core") << "unable to get emulator" << LogEnd;
          continue;
        }
        m_Emulators[pEmulator->GetName()] = pGetEmulator;
        delete pEmulator;
        continue;
      }

      auto pGetOperatingSystem = Module.Load<TGetOperatingSystem>(pMod, "GetOperatingSystem");
      if (pGetOperatingSystem != nullptr)
      {
        Log::Write("core") << "is an operating system" << LogEnd;

        auto pOperatingSystem = pGetOperatingSystem();
        if (pOperatingSystem == nullptr)
        {
          Log::Write("core") << "unable to get operating system" << LogEnd;
          continue;
        }
        OperatingSystem::SPType spOperatingSystem(pOperatingSystem);
        m_OperatingSystems.push_back(spOperatingSystem);
        continue;
      }

      Log::Write("core") << "is unknown (ignored)" << LogEnd;
    }
  }
  catch (std::exception &e)
  {
    Log::Write("core") << e.what() << LogEnd;
  }

  if (!m_Loaders.empty())
    std::sort(std::begin(m_Loaders), std::end(m_Loaders), [](Loader::SPType spLdr0, Loader::SPType spLdr1)
        {
          return spLdr0->GetDepth() > spLdr1->GetDepth();
        });
}

void ModuleManager::UnloadModules(void)
{
  m_DefaultArchitectureTag = MEDUSA_ARCH_UNK;
  m_Loaders.clear();
  m_Architectures.clear();
  m_Databases.clear();
  m_OperatingSystems.clear();
  m_Emulators.clear();
}

Architecture::SPType ModuleManager::GetArchitecture(Tag ArchTag) const
{
  if (ArchTag == MEDUSA_ARCH_UNK)
    ArchTag = m_DefaultArchitectureTag;

  auto itArch = m_TaggedArchitectures.find(ArchTag);
  if (itArch == std::end(m_TaggedArchitectures))
    return Architecture::SPType();

  return itArch->second;
}

Architecture::SPType ModuleManager::FindArchitecture(Tag ArchTag) const
{
  if (ArchTag == MEDUSA_ARCH_UNK)
    return nullptr;

  auto const& rAllArchs = GetArchitectures();
  for (auto itArch = std::begin(rAllArchs), itEnd = std::end(rAllArchs); itArch != itEnd; ++itArch)
    if (MEDUSA_CMP_TAG((*itArch)->GetTag(), ArchTag))
      return *itArch;
  return nullptr;
}

bool ModuleManager::RegisterArchitecture(Architecture::SPType spArch)
{
  u8 Id = 0;
  bool FoundId = false;

  for (u8 i = 0; i < 32; ++i)
    if (!(m_ArchIdPool & (1 << i)))
    {
      m_ArchIdPool |= (1 << i);
      Id = i;
      FoundId = true;
      break;
    }

    if (FoundId == false)
      return false;

    spArch->UpdateId(Id);

    m_TaggedArchitectures[spArch->GetTag()] = spArch;

    if (m_DefaultArchitectureTag == MEDUSA_ARCH_UNK)
      m_DefaultArchitectureTag = spArch->GetTag();

    return true;
}

bool ModuleManager::UnregisterArchitecture(Architecture::SPType spArch)
{
  return false; /* Not implemented */
}

void ModuleManager::ResetArchitecture(void)
{
  m_Architectures.clear();
}

TGetBinding ModuleManager::GetBinding(std::string const& rBindingName)
{
  auto itBinding = m_Bindings.find(rBindingName);
  if (itBinding == std::end(m_Bindings))
    return nullptr;
  return itBinding->second;
}

TGetEmulator ModuleManager::GetEmulator(std::string const& rEmulatorName)
{
  auto itEmulator = m_Emulators.find(rEmulatorName);
  if (itEmulator == std::end(m_Emulators))
    return nullptr;
  return itEmulator->second;
}

OperatingSystem::SPType ModuleManager::GetOperatingSystem(std::string const& rOperatingSystemName) const
{
  for (auto spOs : m_OperatingSystems)
    if (spOs->GetName() == rOperatingSystemName)
      return spOs;
  return nullptr;
}

OperatingSystem::SPType ModuleManager::GetOperatingSystem(Loader::SPType spLdr, Architecture::SPType spArch) const
{
  OperatingSystem::VSPType CompatOs;
  for (auto itOs = std::begin(m_OperatingSystems); itOs != std::end(m_OperatingSystems); ++itOs)
    if ((*itOs)->IsSupported(*spLdr, *spArch) == true)
      return *itOs;
  return OperatingSystem::SPType();
}

Database::SPType ModuleManager::GetDatabase(std::string const& rDatabaseName)
{
  for (auto itDb = std::begin(m_Databases); itDb != std::end(m_Databases); ++itDb)
    if ((*itDb)->GetName() == rDatabaseName)
      return *itDb;
  return Database::SPType();
}

Database::VSPType ModuleManager::GetDatabases(void) const
{
  return m_Databases;
}

Loader::VSPType ModuleManager::GetLoaders(void) const
{
  return m_Loaders;
}

Architecture::VSPType ModuleManager::GetArchitectures(void) const
{
  return m_Architectures;
}

MEDUSA_NAMESPACE_END
