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

bool Medusa::IsOpened(void) const
{
  if (m_Database.GetMemoryAreas().empty() && m_FileBinStrm.GetSize() == 0x0)
    return false;

  return true;
}

void Medusa::Close(void)
{
  m_Database.RemoveAll();
  m_FileBinStrm.Close();

  m_Loaders.erase(std::begin(m_Loaders), std::end(m_Loaders));
  m_AvailableArchitectures.erase(std::begin(m_AvailableArchitectures), std::end(m_AvailableArchitectures));
  m_UsedArchitectures.erase(std::begin(m_UsedArchitectures), std::end(m_UsedArchitectures));
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
      if (pGetArchitecture != NULL)
      {
        Log::Write("core") << "is an Architecture" << LogEnd;

        Architecture* pArchitecture = pGetArchitecture();
        Architecture::SharedPtr ArchitecturePtr(pArchitecture);
        m_AvailableArchitectures.push_back(ArchitecturePtr);
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

void Medusa::Disassemble(Loader::SharedPtr spLoader, Architecture::SharedPtr spArch)
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

    m_Analyzer.DisassembleFollowingExecutionPath(m_Database, It->left, *spArch);
    m_Analyzer.CreateXRefs(m_Database);
  }

  m_Analyzer.FindStrings(m_Database, *spArch);
  m_Analyzer.FormatsAllCells(m_Database, *spArch);
}

void Medusa::DisassembleAsync(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch)
{
  boost::thread DisasmThread(&Medusa::Disassemble, this, pLoader, pArch);
}

bool Medusa::RegisterArchitecture(Architecture::SharedPtr spArch)
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

  if (FoundId == false) return false;

  spArch->UpdateId(Id);

  m_UsedArchitectures[spArch->GetTag()] = spArch;

  return true;
}

bool Medusa::UnregisterArchitecture(Architecture::SharedPtr spArch)
{
  return false;
}

bool Medusa::BuildControlFlowGraph(Address const& rAddr, ControlFlowGraph& rCfg)
{
  return m_Analyzer.BuildControlFlowGraph(m_Database, rAddr, rCfg);
}

Address::SharedPtr Medusa::MakeAddress(TOffset Offset)
{
  return MakeAddress(Loader::SharedPtr(), Architecture::SharedPtr(), 0x0, Offset);
}

Address::SharedPtr Medusa::MakeAddress(TBase Base, TOffset Offset)
{
  return MakeAddress(Loader::SharedPtr(), Architecture::SharedPtr(), Base, Offset);
}

Address::SharedPtr Medusa::MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TOffset Offset)
{
  return MakeAddress(pLoader, pArch, 0x0, Offset);
}

Address::SharedPtr Medusa::MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TBase Base, TOffset Offset)
{
  Address::SharedPtr NewAddr;

  NewAddr = m_Database.MakeAddress(Base, Offset);
  if (NewAddr)
    return NewAddr;

  NewAddr.reset(new Address(Base, Offset));
  return NewAddr;
}

MEDUSA_NAMESPACE_END