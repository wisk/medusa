#include "medusa/module.hpp"
#include "medusa/log.hpp"

#include <boost/filesystem.hpp>

MEDUSA_NAMESPACE_BEGIN

void ModuleManager::LoadModules(std::wstring const& rModPath, Document& rDoc)
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
      std::wstring const& rFilename = It->path().wstring();

      if (rFilename.substr(rFilename.find_last_of(L".") + 1) != Module::GetExtension())
        continue;

      std::wstring FullPath = boost::filesystem::system_complete(*It).wstring();
      Log::Write("core") << "Module: \"" << rFilename << "\" ";

      void* pMod = Module.Load(FullPath);
      if (pMod == nullptr)
        continue;

      TGetLoader pGetLoader = Module.Load<TGetLoader>(pMod, "GetLoader");
      if (pGetLoader != nullptr)
      {
        Log::Write("core") << "is a loader ";

        Loader* pLoader = pGetLoader(rDoc);
        if (pLoader->IsSupported())
        {
          Loader::SharedPtr LoaderPtr(pLoader);
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

      TGetArchitecture pGetArchitecture = Module.Load<TGetArchitecture>(pMod, "GetArchitecture");
      if (pGetArchitecture != nullptr)
      {
        Log::Write("core") << "is an architecture" << LogEnd;

        Architecture* pArchitecture = pGetArchitecture();
        Architecture::SharedPtr ArchitecturePtr(pArchitecture);
        m_Architectures.push_back(ArchitecturePtr);
        continue;
      }

      TGetOperatingSystem pGetOperatingSystem = Module.Load<TGetOperatingSystem>(pMod, "GetOperatingSystem");
      if (pGetOperatingSystem != nullptr)
      {
        Log::Write("core") << "is an operating system" << LogEnd;

        OperatingSystem* pOperatingSystem = pGetOperatingSystem(rDoc);
        OperatingSystem::SharedPtr spOperatingSystem(pOperatingSystem);
        m_OperatingSystems.push_back(spOperatingSystem);
        continue;
      }

      TGetEmulator pGetEmulator = Module.Load<TGetEmulator>(pMod, "GetEmulator");
      if (pGetEmulator != nullptr)
      {
        Log::Write("core") << "is an emulator" << LogEnd;

        Emulator* pEmulator = pGetEmulator(nullptr, nullptr, nullptr);
        m_Emulators[pEmulator->GetName()] = pGetEmulator;
        delete pEmulator;
        continue;
      }

      TGetDabatase pGetDatabase = Module.Load<TGetDabatase>(pMod, "GetDatabase");
      if (pGetDatabase != nullptr)
      {
        Log::Write("core") << "is a database" << LogEnd;

        Database* pDatabase = pGetDatabase();
        Database::SharedPtr spDatabase(pDatabase);
        m_Databases.push_back(spDatabase);
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

Architecture::SharedPtr ModuleManager::GetArchitecture(Tag ArchTag) const
{
  if (ArchTag == MEDUSA_ARCH_UNK)
    ArchTag = m_DefaultArchitectureTag;

  auto itArch = m_TaggedArchitectures.find(ArchTag);
  if (itArch == std::end(m_TaggedArchitectures))
    return Architecture::SharedPtr();

  return itArch->second;
}

bool ModuleManager::RegisterArchitecture(Architecture::SharedPtr spArch)
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

bool ModuleManager::UnregisterArchitecture(Architecture::SharedPtr spArch)
{
  return false; /* Not implemented */
}

void ModuleManager::ResetArchitecture(void)
{
  m_Architectures.clear();
}

TGetEmulator ModuleManager::GetEmulator(std::string const& rEmulatorName)
{
  auto itEmulator = m_Emulators.find(rEmulatorName);
  if (itEmulator == std::end(m_Emulators))
    return nullptr;
  return itEmulator->second;
}

OperatingSystem::SharedPtr ModuleManager::GetOperatingSystem(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch) const
{
  OperatingSystem::VectorSharedPtr CompatOs;
  for (auto itOs = std::begin(m_OperatingSystems); itOs != std::end(m_OperatingSystems); ++itOs)
    if ((*itOs)->IsSupported(*spLdr, *spArch) == true)
      return *itOs;
  return OperatingSystem::SharedPtr();
}

Database::SharedPtr ModuleManager::GetDatabase(std::string const& rDatabaseName)
{
  for (auto itDb = std::begin(m_Databases); itDb != std::end(m_Databases); ++itDb)
    if ((*itDb)->GetName() == rDatabaseName)
      return *itDb;
  return Database::SharedPtr();
}

Loader::VectorSharedPtr ModuleManager::GetLoaders(void) const
{
  return m_Loaders;
}

Architecture::VectorSharedPtr ModuleManager::GetArchitectures(void) const
{
  return m_Architectures;
}

MEDUSA_NAMESPACE_END
