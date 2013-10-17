#include "medusa/medusa.hpp"
#include "medusa/module.hpp"
#include "medusa/xref.hpp"
#include "medusa/log.hpp"

#include <cstring>
#include <cstdlib>
#include <list>
#include <algorithm>

MEDUSA_NAMESPACE_BEGIN

Medusa::Medusa(void)
  : m_FileBinStrm()
  , m_Document(m_FileBinStrm)
  , m_Analyzer()
{
  m_TaskManager.Start();
}

Medusa::Medusa(std::wstring const& rFilePath)
  : m_FileBinStrm(rFilePath)
  , m_Document(m_FileBinStrm)
  , m_Analyzer()
{
}

Medusa::~Medusa(void)
{
  m_TaskManager.Stop();
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
}

void Medusa::LoadModules(std::wstring const& rModulesPath)
{
  ModuleManager::Instance().LoadModules(rModulesPath, m_Document);
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
  auto spArch = ModuleManager::Instance().GetArchitecture(pCell->GetArchitectureTag());
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
}

void Medusa::AddTask(Task* pTask)
{
  m_TaskManager.AddTask(pTask);
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

    AddTask(m_Analyzer.CreateDisassembleFunctionTask(m_Document, itLbl->left, *spArch));
  }

  /* Find all strings */
  AddTask(m_Analyzer.CreateFindAllStringTask(m_Document));

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
  AddTask(m_Analyzer.CreateDisassembleFunctionTask(m_Document, rAddr, *spArch));
}

void Medusa::AnalyzeAsync(Address const& rAddr)
{
  auto pCell = GetCell(rAddr);
  if (pCell == nullptr) return;
  auto spArch = ModuleManager::Instance().GetArchitecture(pCell->GetArchitectureTag());
  if (!spArch) return;
  boost::thread AnlzThread(&Medusa::Analyze, this, spArch, rAddr);
}

void Medusa::AnalyzeAsync(Architecture::SharedPtr spArch, Address const& rAddr)
{
  boost::thread DisasmThread(&Medusa::Analyze, this, spArch, rAddr);
}

bool Medusa::BuildControlFlowGraph(Address const& rAddr, ControlFlowGraph& rCfg) const
{
  return m_Analyzer.BuildControlFlowGraph(m_Document, rAddr, rCfg);
}

Cell::SPtr Medusa::GetCell(Address const& rAddr)
{
  return m_Document.GetCell(rAddr);
}

Cell::SPtr const Medusa::GetCell(Address const& rAddr) const
{
  return m_Document.GetCell(rAddr);
}

bool Medusa::FormatCell(
  Address       const& rAddress,
  Cell          const& rCell,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  return m_Analyzer.FormatCell(m_Document, m_Document.GetFileBinaryStream(), rAddress, rCell, rStrCell, rMarks);
}

MultiCell* Medusa::GetMultiCell(Address const& rAddr)
{
  return m_Document.GetMultiCell(rAddr);
}

MultiCell const* Medusa::GetMultiCell(Address const& rAddr) const
{
  return m_Document.GetMultiCell(rAddr);
}

bool Medusa::FormatMultiCell(
  Address       const& rAddress,
  MultiCell     const& rMultiCell,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  return m_Analyzer.FormatMultiCell(m_Document, m_Document.GetFileBinaryStream(), rAddress, rMultiCell, rStrMultiCell, rMarks);
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
    //if (m_spOperatingSystem)
    //  m_spOperatingSystem->AnalyzeFunction(rAddr, m_Analyzer);
    return true;
  }
  return false;
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
