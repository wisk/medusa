#include "medusa/medusa.hpp"
#include "medusa/module.hpp"
#include "medusa/xref.hpp"
#include "medusa/log.hpp"

#include <cstring>
#include <cstdlib>
#include <list>
#include <boost/filesystem.hpp>

MEDUSA_NAMESPACE_BEGIN

Medusa::Medusa(void)
  : m_FileBinStrm()
  , m_Database(m_FileBinStrm)
  , m_AvailableArchitectures()
  , m_Loaders()
  , m_Analyzer()
{
}

Medusa::Medusa(std::wstring const& rFilePath)
  : m_FileBinStrm(rFilePath)
  , m_Database(m_FileBinStrm)
  , m_AvailableArchitectures()
  , m_Loaders()
  , m_Analyzer()
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
  m_CompatibleOperatingSystems.erase(std::begin(m_CompatibleOperatingSystems), std::end(m_CompatibleOperatingSystems));
}

OperatingSystem::VectorSharedPtr Medusa::GetCompatibleOperatingSystems(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch) const
{
  OperatingSystem::VectorSharedPtr CompatOs;
  for (auto itOs = std::begin(m_CompatibleOperatingSystems); itOs != std::end(m_CompatibleOperatingSystems); ++itOs)
    if ((*itOs)->IsSupported(*spLdr, *spArch) == true)
      CompatOs.push_back(*itOs);
  return CompatOs;
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
      if (pMod == nullptr)
        continue;

      TGetLoader pGetLoader = Module.Load<TGetLoader>(pMod, "GetLoader");
      if (pGetLoader != nullptr)
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
      if (pGetArchitecture != nullptr)
      {
        Log::Write("core") << "is an Architecture" << LogEnd;

        Architecture* pArchitecture = pGetArchitecture();
        Architecture::SharedPtr ArchitecturePtr(pArchitecture);
        m_AvailableArchitectures.push_back(ArchitecturePtr);
        continue;
      }

      TGetOperatingSystem pGetOperatingSystem = Module.Load<TGetOperatingSystem>(pMod, "GetOperatingSystem");
      if (pGetOperatingSystem != nullptr)
      {
        Log::Write("core") << "is an operating system" << LogEnd;

        OperatingSystem* pOperatingSystem = pGetOperatingSystem(m_Database);
        OperatingSystem::SharedPtr spOperatingSystem(pOperatingSystem);
        m_CompatibleOperatingSystems.push_back(spOperatingSystem);
        continue;
      }

      TGetEmulator pGetEmulator = Module.Load<TGetEmulator>(pMod, "GetEmulator");
      if (pGetEmulator != nullptr)
      {
        Log::Write("core") << "is an Emulator" << LogEnd;

        Emulator* pEmulator = pGetEmulator(nullptr, nullptr, nullptr);
        m_Emulators[pEmulator->GetName()] = pGetEmulator;
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

void Medusa::Disassemble(Architecture::SharedPtr spArch, Address const& rAddr)
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
  m_Analyzer.DisassembleFollowingExecutionPath(m_Database, rAddr, *spArch);
}

void Medusa::DisassembleAsync(Address const& rAddr)
{
  auto pCell = GetCell(rAddr);
  if (pCell == nullptr) return;
  auto spArch = m_Analyzer.GetArchitecture(pCell->GetArchitectureTag());
  if (!spArch) return;
  boost::thread DisasmThread(&Medusa::Disassemble, this, spArch, rAddr);
}

void Medusa::DisassembleAsync(Architecture::SharedPtr spArch, Address const& rAddr)
{
  boost::thread DisasmThread(&Medusa::Disassemble, this, spArch, rAddr);
}

void Medusa::ConfigureEndianness(Architecture::SharedPtr spArch)
{
  /* Configure endianness of memory area */
  m_FileBinStrm.SetEndianness(spArch->GetEndianness());
  for (Database::TIterator It = m_Database.Begin(); It != m_Database.End(); ++It)
    (*It)->SetEndianness(m_FileBinStrm.GetEndianness());
}

void Medusa::Start(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch)
{
  ConfigureEndianness(spArch);

  /* Add start label */
  m_Database.AddLabel(spLdr->GetEntryPoint(), Label("start", Label::LabelCode));

  /* Disassemble all symbols if possible */
  Database::TLabelMap const& rLabels = m_Database.GetLabels();
  for (Database::TLabelMap::const_iterator It = rLabels.begin();
    It != rLabels.end(); ++It)
  {
    if (It->right.GetType() != Label::LabelCode)
      continue;

    m_Analyzer.DisassembleFollowingExecutionPath(m_Database, It->left, *spArch);
    m_Analyzer.CreateXRefs(m_Database);
  }

  /* Find all strings */
  m_Analyzer.FindStrings(m_Database, *spArch);
}

void Medusa::StartAsync(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch)
{
  boost::thread StartThread(&Medusa::Start, this, spLdr, spArch);
}


void Medusa::Analyze(Architecture::SharedPtr spArch, Address const& rAddr)
{
  m_Analyzer.DisassembleFollowingExecutionPath(m_Database, rAddr, *spArch);
  m_Analyzer.CreateXRefs(m_Database); /* LATER: Optimize this, we don't need to re-analyze the whole stuff */
}

void Medusa::AnalyzeAsync(Address const& rAddr)
{
  auto pCell = GetCell(rAddr);
  if (pCell == nullptr) return;
  auto spArch = m_Analyzer.GetArchitecture(pCell->GetArchitectureTag());
  if (!spArch) return;
  boost::thread AnlzThread(&Medusa::Analyze, this, spArch, rAddr);
}

void Medusa::AnalyzeAsync(Architecture::SharedPtr spArch, Address const& rAddr)
{
  boost::thread DisasmThread(&Medusa::Analyze, this, spArch, rAddr);
}

bool Medusa::RegisterArchitecture(Architecture::SharedPtr spArch)
{
  return m_Analyzer.RegisterArchitecture(spArch);
}

bool Medusa::UnregisterArchitecture(Architecture::SharedPtr spArch)
{
  return m_Analyzer.UnregisterArchitecture(spArch);
}

bool Medusa::BuildControlFlowGraph(Address const& rAddr, ControlFlowGraph& rCfg)
{
  return m_Analyzer.BuildControlFlowGraph(m_Database, rAddr, rCfg);
}

Cell* Medusa::GetCell(Address const& rAddr)
{
  return m_Analyzer.GetCell(m_Database, m_FileBinStrm, rAddr);
}

Cell const* Medusa::GetCell(Address const& rAddr) const
{
  return m_Analyzer.GetCell(m_Database, m_FileBinStrm, rAddr);
}

MultiCell* Medusa::GetMultiCell(Address const& rAddr)
{
  return m_Analyzer.GetMultiCell(m_Database, m_FileBinStrm, rAddr);
}

MultiCell const* Medusa::GetMultiCell(Address const& rAddr) const
{
  return m_Analyzer.GetMultiCell(m_Database, m_FileBinStrm, rAddr);
}

Address Medusa::MakeAddress(TOffset Offset)
{
  return MakeAddress(Loader::SharedPtr(), Architecture::SharedPtr(), 0x0, Offset);
}

Address Medusa::MakeAddress(TBase Base, TOffset Offset)
{
  return MakeAddress(Loader::SharedPtr(), Architecture::SharedPtr(), Base, Offset);
}

Address Medusa::MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TOffset Offset)
{
  return MakeAddress(pLoader, pArch, 0x0, Offset);
}

Address Medusa::MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TBase Base, TOffset Offset)
{
  Address NewAddr = m_Database.MakeAddress(Base, Offset);
  if (NewAddr.GetAddressingType() == Address::UnknownType)
    return NewAddr;

  return Address(Base, Offset);
}

void Medusa::FindFunctionAddressFromAddress(Address::List& rFunctionAddress, Address const& rAddress) const
{
  auto MCells = m_Database.GetMultiCells();
  for (auto itMc = std::begin(MCells); itMc != std::end(MCells); ++itMc)
  {
    auto pFunction = dynamic_cast<Function const*>(itMc->second);
    if (pFunction == nullptr)
      continue;

    if (pFunction->Contains(rAddress) == false)
      continue;

    for (auto itMc = std::begin(MCells); itMc != std::end(MCells); ++itMc)
      if (itMc->second == pFunction)
      {
        rFunctionAddress.push_back(itMc->first);
        break;
      }
  }
}

MEDUSA_NAMESPACE_END