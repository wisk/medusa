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
  : m_TaskManager([] (Task const* pTask) { Log::Write("core") << "Task \"" << pTask->GetName() << "\" is done" << LogEnd; })
  , m_FileBinStrm()
  , m_Document(m_FileBinStrm)
  , m_Analyzer()
{
  m_TaskManager.Start();
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

void Medusa::UnloadModules(void)
{
  ModuleManager::Instance().UnloadModules();
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

void Medusa::Start(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs, Database::SharedPtr spDb)
{
  ConfigureEndianness(spArch);

  spLdr->Map(m_Document);
  u8 Mode = spArch->GetDefaultMode(m_Document.GetAddressFromLabelName("start"));
  AddTask(m_Analyzer.CreateDisassembleAllFunctionsTask(m_Document, *spArch, Mode));

  /* Find all strings */
  AddTask(m_Analyzer.CreateFindAllStringTask(m_Document));

  /* Analyze all functions */
  //if (spOs)
  //{
  //  auto MCells = m_Document.GetMultiCells();
  //  for (auto itMCell = std::begin(MCells); itMCell != std::end(MCells); ++itMCell)
  //  {
  //    if (itMCell->second->GetType() != MultiCell::FunctionType)
  //      continue;
  //    spOs->AnalyzeFunction(itMCell->first, m_Analyzer);
  //  }
  //}
}

void Medusa::Analyze(Address const& rAddr, Architecture::SharedPtr spArch, u8 Mode)
{
  Cell::SPtr spCell = nullptr;

  if (Mode == 0)
  {
    spCell = GetCell(rAddr);
    if (spCell == nullptr)
      return;
    Mode = spCell->GetData()->GetMode();
  }

  if (spArch == nullptr)
  {
    if (spCell == nullptr)
    {
      spCell = GetCell(rAddr);
      if (spCell == nullptr)
        return;
    }

    spArch = ModuleManager::Instance().GetArchitecture(spCell->GetArchitectureTag());
    if (spArch == nullptr)
      return;
  }

  if (Mode == 0)
    Mode = spArch->GetDefaultMode(rAddr);

  AddTask(m_Analyzer.CreateDisassembleFunctionTask(m_Document, rAddr, *spArch, Mode));
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
