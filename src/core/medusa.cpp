#include "medusa/medusa.hpp"
#include "medusa/module.hpp"
#include "medusa/xref.hpp"
#include "medusa/log.hpp"

#include <cstring>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <boost/filesystem.hpp>

MEDUSA_NAMESPACE_BEGIN

Medusa::Medusa(void)
  : m_FileBinStrm()
  , m_Document(m_FileBinStrm)
  , m_AvailableArchitectures()
  , m_Loaders()
  , m_Analyzer()
{
}

Medusa::Medusa(std::wstring const& rFilePath)
  : m_FileBinStrm(rFilePath)
  , m_Document(m_FileBinStrm)
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
  if (m_FileBinStrm.GetSize() == 0x0)
    return false;

  return true;
}

void Medusa::Close(void)
{
  m_Document.RemoveAll();
  m_FileBinStrm.Close();

  m_Loaders.erase(std::begin(m_Loaders), std::end(m_Loaders));
  m_AvailableArchitectures.erase(std::begin(m_AvailableArchitectures), std::end(m_AvailableArchitectures));
  m_CompatibleOperatingSystems.erase(std::begin(m_CompatibleOperatingSystems), std::end(m_CompatibleOperatingSystems));
  m_Analyzer.ResetArchitecture();
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

        Loader* pLoader = pGetLoader(m_Document);
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
        m_AvailableArchitectures.push_back(ArchitecturePtr);
        continue;
      }

      TGetOperatingSystem pGetOperatingSystem = Module.Load<TGetOperatingSystem>(pMod, "GetOperatingSystem");
      if (pGetOperatingSystem != nullptr)
      {
        Log::Write("core") << "is an operating system" << LogEnd;

        OperatingSystem* pOperatingSystem = pGetOperatingSystem(m_Document);
        OperatingSystem::SharedPtr spOperatingSystem(pOperatingSystem);
        m_CompatibleOperatingSystems.push_back(spOperatingSystem);
        continue;
      }

      TGetEmulator pGetEmulator = Module.Load<TGetEmulator>(pMod, "GetEmulator");
      if (pGetEmulator != nullptr)
      {
        Log::Write("core") << "is an emulator" << LogEnd;

        Emulator* pEmulator = pGetEmulator(nullptr, nullptr, nullptr, nullptr);
        m_Emulators[pEmulator->GetName()] = pGetEmulator;
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

void Medusa::Disassemble(Architecture::SharedPtr spArch, Address const& rAddr)
{
  boost::lock_guard<MutexType> Lock(m_Mutex);
  m_Analyzer.DisassembleFollowingExecutionPath(m_Document, rAddr, *spArch);
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
  for (Document::TIterator It = m_Document.Begin(); It != m_Document.End(); ++It)
    (*It)->SetEndianness(m_FileBinStrm.GetEndianness());
}

void Medusa::Start(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs)
{
  ConfigureEndianness(spArch);

  /* Add start label */
  m_Document.AddLabel(spLdr->GetEntryPoint(), Label("start", Label::Code | Label::Global | Label::Exported));

  /* Disassemble all symbols if possible */
  Document::LabelBimapType Labels = m_Document.GetLabels();
  for (auto itLbl = Labels.begin(); itLbl != Labels.end(); ++itLbl)
  {
    if (!(itLbl->right.GetType() & Label::Code) && itLbl->right.GetType() & Label::Imported)
      continue;

    m_Analyzer.DisassembleFollowingExecutionPath(m_Document, itLbl->left, *spArch);
    m_Analyzer.CreateFunction(m_Document, itLbl->left);
  }

  /* Find all strings */
  m_Analyzer.FindStrings(m_Document, *spArch);

  /* Analyze all functions */
  if (spOs)
  {
    auto MCells = m_Document.GetMultiCells();
    for (auto itMCell = std::begin(MCells); itMCell != std::end(MCells); ++itMCell)
    {
      if (itMCell->second->GetType() != MultiCell::FunctionType)
        continue;
      spOs->AnalyzeFunction(itMCell->first, m_Analyzer);
    }
  }
}

void Medusa::StartAsync(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs)
{
  // FIXME: there is a race condition here, we must keep the thread instance in order to stop it if needed
  boost::thread StartThread(&Medusa::Start, this, spLdr, spArch, spOs);
}


void Medusa::Analyze(Architecture::SharedPtr spArch, Address const& rAddr)
{
  m_Analyzer.DisassembleFollowingExecutionPath(m_Document, rAddr, *spArch);
  //m_Analyzer.CreateXRefs(m_Document); /* LATER: Optimize this, we don't need to re-analyze the whole stuff */
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
  return m_Analyzer.BuildControlFlowGraph(m_Document, rAddr, rCfg);
}

Cell* Medusa::GetCell(Address const& rAddr)
{
  return m_Analyzer.GetCell(m_Document, rAddr);
}

Cell const* Medusa::GetCell(Address const& rAddr) const
{
  return m_Analyzer.GetCell(m_Document, rAddr);
}

bool Medusa::FormatCell(
  Address       const& rAddress,
  Cell          const& rCell,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  auto pMemArea = m_Document.GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;
  return m_Analyzer.FormatCell(m_Document, pMemArea->GetBinaryStream(), rAddress, rCell, rStrCell, rMarks);
}

MultiCell* Medusa::GetMultiCell(Address const& rAddr)
{
  return m_Analyzer.GetMultiCell(m_Document, rAddr);
}

MultiCell const* Medusa::GetMultiCell(Address const& rAddr) const
{
  return m_Analyzer.GetMultiCell(m_Document, rAddr);
}

bool Medusa::FormatMultiCell(
  Address       const& rAddress,
  MultiCell     const& rMultiCell,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  auto pMemArea = m_Document.GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;
  return m_Analyzer.FormatMultiCell(m_Document, pMemArea->GetBinaryStream(), rAddress, rMultiCell, rStrMultiCell, rMarks);
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
  Address NewAddr = m_Document.MakeAddress(Base, Offset);
  if (NewAddr.GetAddressingType() == Address::UnknownType)
    return NewAddr;

  return Address(Base, Offset);
}

bool Medusa::CreateFunction(Address const& rAddr)
{
  if (m_Analyzer.CreateFunction(m_Document, rAddr))
  {
    if (m_spOperatingSystem)
      m_spOperatingSystem->AnalyzeFunction(rAddr, m_Analyzer);
    return true;
  }
  return false;
}

void Medusa::FindFunctionAddressFromAddress(Address::List& rFunctionAddress, Address const& rAddress) const
{
  m_Document.FindFunctionAddressFromAddress(rFunctionAddress, rAddress);
}

void Medusa::TrackOperand(Address const& rStartAddress, Analyzer::Tracker& rTracker)
{
  m_Analyzer.TrackOperand(m_Document, rStartAddress, rTracker);
}

void Medusa::BacktrackOperand(Address const& rStartAddress, Analyzer::Tracker& rTracker)
{
  m_Analyzer.BacktrackOperand(m_Document, rStartAddress, rTracker);
}

MEDUSA_NAMESPACE_END