#include "medusa/medusa.hpp"
#include "medusa/module.hpp"
#include "medusa/xref.hpp"
#include "medusa/log.hpp"

#include <cstring>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <list>

MEDUSA_NAMESPACE_BEGIN

Medusa::Medusa(void)
  : m_FileBinStrm()
  , m_Database(m_FileBinStrm)
{
}

Medusa::Medusa(std::wstring const& rFilePath)
  : m_FileBinStrm(rFilePath)
  , m_Database(m_FileBinStrm)
{
}

Medusa::~Medusa(void)
{
}

void Medusa::Open(std::wstring const& rFilePath)
{
  m_FileBinStrm.Open(rFilePath);
}

void Medusa::Close(void)
{
  m_Database.RemoveAll();
  m_FileBinStrm.Close();

  m_Loaders.erase(m_Loaders.begin(), m_Loaders.end());

  m_Architectures.erase(m_Architectures.begin(), m_Architectures.end());
}

void Medusa::Load(Serialize& rSrlz)
{
  SerializeEntity::SPtr spRoot = rSrlz.GetRoot();

  SerializeEntity::SPtrList::const_iterator It = spRoot->BeginSubEntities();

  /* Loader */
  if ((*It)->GetName() != "ldr")
    throw Exception(L"Database is corrupted (ldr is missing)");

  /* Architecture */
  ++It;
  for (; It != spRoot->EndSubEntities() && (*It)->GetName() == "arch"; ++It)
  {
  }

  if (It == spRoot->EndSubEntities())
    throw Exception(L"Database is corrupted (db is missing)");

  m_Database.Load(*It);

  rSrlz.Load();
}

void Medusa::Save(Serialize& rSrlz)
{
  SerializeEntity::SPtr spRoot = rSrlz.GetRoot();

  spRoot->CreateSubEntity("ldr")->AddField("name", "unimplemented");

  for (Architecture::VectorSPtr::const_iterator It = m_Architectures.begin();
    It != m_Architectures.end(); ++It)
    spRoot->CreateSubEntity("arch")->AddField("name", (*It)->GetName());

  SerializeEntity::SPtr spDatabase = m_Database.Save();

  if (!spDatabase)
    throw Exception(L"Error while saving file");

  spRoot->AddSubEntity(m_Database.Save());

  rSrlz.Save();
}

void Medusa::LoadModules(std::wstring const& rModulesPath)
{
  try
  {
    const boost::filesystem::path CurDir = rModulesPath;
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
      if (pMod == NULL)
        continue;

      TGetLoader pGetLoader = Module.Load<TGetLoader>(pMod, "GetLoader");
      if (pGetLoader != NULL)
      {
        Log::Write("core") << "is a loader ";

        Loader* pLoader = pGetLoader(m_Database);
        if (pLoader->IsSupported())
        {
          Loader::SPtr LoaderPtr(pLoader);
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
      if (pGetArchitecture != NULL)
      {
        Log::Write("core") << "is an Architecture" << LogEnd;

        Architecture* pArchitecture = pGetArchitecture();
        Architecture::SPtr ArchitecturePtr(pArchitecture);
        m_Architectures.push_back(ArchitecturePtr);
        continue;
      }

      TGetSerialize pGetSerialize = Module.Load<TGetSerialize>(pMod, "GetSerialize");
      if (pGetSerialize != NULL)
      {
        Log::Write("core") << "is a serialize" << LogEnd;

        Serialize* pSerialize = pGetSerialize();
        Serialize::SPtr spSerialize(pSerialize);
        m_Serializes.push_back(spSerialize);
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

void Medusa::Disassemble(Loader::SPtr spLoader, Architecture::SPtr spArch)
{
  m_FileBinStrm.SetEndianness(spArch->GetEndianness());
  for (Database::TIterator It = m_Database.Begin(); It != m_Database.End(); ++It)
    (*It)->SetEndianness(m_FileBinStrm.GetEndianness());

  Address EntryPoint = spLoader->GetEntryPoint();
  m_Database.AddLabel(EntryPoint, Label("start", Label::LabelCode));

  Database::TLabelMap const& rLabels = m_Database.GetLabels();
  for (Database::TLabelMap::const_iterator It = rLabels.begin();
    It != rLabels.end(); ++It)
  {
    if (It->right.GetType() != Label::LabelCode)
      continue;

    m_Disasm.FollowExecutionPath(m_Database, It->left, *spArch);
    m_Disasm.CreateXRefs(m_Database);
  }

  m_Disasm.FindStrings(m_Database);
  m_Disasm.FormatsAllCells(m_Database, *spArch);
}

Address::SPtr Medusa::MakeAddress(TOffset Offset)
{
  return MakeAddress(Loader::SPtr(), Architecture::SPtr(), 0x0, Offset);
}

Address::SPtr Medusa::MakeAddress(TBase Base, TOffset Offset)
{
  return MakeAddress(Loader::SPtr(), Architecture::SPtr(), Base, Offset);
}

Address::SPtr Medusa::MakeAddress(Loader::SPtr pLoader, Architecture::SPtr pArch, TOffset Offset)
{
  return MakeAddress(pLoader, pArch, 0x0, Offset);
}

Address::SPtr Medusa::MakeAddress(Loader::SPtr pLoader, Architecture::SPtr pArch, TBase Base, TOffset Offset)
{
  Address::SPtr NewAddr;

  NewAddr = m_Database.MakeAddress(Base, Offset);
  if (NewAddr)
    return NewAddr;

  NewAddr.reset(new Address(Base, Offset));
  return NewAddr;
}

MEDUSA_NAMESPACE_END