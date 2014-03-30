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
  , m_Document()
  , m_Analyzer()
{
  m_TaskManager.Start();
}

Medusa::~Medusa(void)
{
  m_TaskManager.Stop();
}

void Medusa::AddTask(Task* pTask)
{
  m_TaskManager.AddTask(pTask);
}

void Medusa::WaitForTasks(void)
{
  m_TaskManager.Wait();
}

bool Medusa::Start(
  BinaryStream::SharedPtr spBinaryStream,
  Database::SharedPtr spDatabase,
  Loader::SharedPtr spLoader,
  Architecture::VectorSharedPtr spArchitectures,
  OperatingSystem::SharedPtr spOperatingSystem)
{
  if (spArchitectures.empty())
    return false;

  for (auto itArch = std::begin(spArchitectures), itEnd = std::end(spArchitectures); itArch != itEnd; ++itArch)
  {
    if (!ModuleManager::Instance().RegisterArchitecture(*itArch))
      Log::Write("core") << "unable to register architecture " << (*itArch)->GetName() << " to module manager" << LogEnd;
    if (!spDatabase->RegisterArchitectureTag((*itArch)->GetTag()))
      Log::Write("core") << "unable to register architecture " << (*itArch)->GetName() << " to database" << LogEnd;
  }

  spDatabase->SetBinaryStream(spBinaryStream);

  /* Set the endianness for the binary stream */
  spBinaryStream->SetEndianness(spArchitectures.front()->GetEndianness()); // LATER: does it always true?

  /* Set the database to the document */
  m_Document.Use(spDatabase);

  /* Map the file to the document */
  spLoader->Map(m_Document, spArchitectures); // Should it be async?

  /* Disassemble the file with the default analyzer */
  AddTask(m_Analyzer.CreateDisassembleAllFunctionsTask(m_Document));

  /* Find all strings using the previous analyze */
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

  return true;
}

bool Medusa::NewDocument(
  fs::path const& rFilePath,
  Medusa::AskDatabaseFunctionType AskDatabase,
  Medusa::ModuleSelectorFunctionType ModuleSelector,
  Medusa::FunctionType BeforeStart,
  Medusa::FunctionType AfterStart)
{
  try
  {
    BinaryStream::SharedPtr spFileBinStrm = std::make_shared<FileBinaryStream>(rFilePath.wstring());
    Log::Write("core") << "opening \"" << rFilePath.string() << "\"" << LogEnd;
    auto& rModMgr = ModuleManager::Instance();
    rModMgr.UnloadModules(); // Make sure we don't have loaded modules in memory
    rModMgr.LoadModules(L".", *spFileBinStrm); // TODO: Let the user select the folder

    auto const& AllLdrs = rModMgr.GetLoaders();
    if (AllLdrs.empty())
    {
      Log::Write("core") << "there is not supported loader" << LogEnd;
      return false;
    }

    auto const& AllArchs = rModMgr.GetArchitectures();
    if (AllArchs.empty())
    {
      Log::Write("core") << "there is not supported architecture" << LogEnd;
      return false;
    }

    Database::SharedPtr spCurDb;
    Loader::SharedPtr spCurLdr;
    Architecture::VectorSharedPtr spCurArchs;
    OperatingSystem::SharedPtr spCurOs;

    if (!ModuleSelector(spFileBinStrm, spCurDb, spCurLdr, spCurArchs, spCurOs))
      return false;

    if (spCurDb == nullptr)
    {
      Log::Write("core") << "there's no available database" << LogEnd;
      return false;
    }

    if (spCurLdr == nullptr)
    {
      Log::Write("core") << "there's no available loader" << LogEnd;
      return false;
    }

    bool Force = false;
    fs::path DbPath = rFilePath.string();
    DbPath += spCurDb->GetExtension().c_str();
    while (!spCurDb->Create(DbPath.wstring(), Force))
    {
      Log::Write("core") << "unable to create database file \"" << DbPath.string() << "\"" << LogEnd;
      fs::path NewDbPath = DbPath;
      std::list<Filter> ExtList;
      ExtList.push_back(std::make_tuple(spCurDb->GetName(), spCurDb->GetExtension()));
      if (!AskDatabase(NewDbPath, ExtList))
        return false;
      if (NewDbPath.empty())
        return false;
      if (NewDbPath == DbPath)
        Force = true;
      DbPath = std::move(NewDbPath);
    }

    if (!BeforeStart())
      return false;
    if (!Start(spFileBinStrm, spCurDb, spCurLdr, spCurArchs, spCurOs))
      return false;
    if (!AfterStart())
      return false;

    return true;
  }
  catch (Exception &e)
  {
    Log::Write("core") << "exception: \"" << e.What() << "\"" << LogEnd;
  }
  return false;
}

bool Medusa::OpenDocument(AskDatabaseFunctionType AskDatabase)
{
  auto& rModMgr = ModuleManager::Instance();
  try
  {
    rModMgr.UnloadModules();
    rModMgr.LoadModules(L".");

    auto const& AllDbs = rModMgr.GetDatabases();
    std::list<Filter> ExtList;
    for (auto itDb = std::begin(AllDbs), itEnd = std::end(AllDbs); itDb != itEnd; ++itDb)
      ExtList.push_back(std::make_tuple((*itDb)->GetName(), (*itDb)->GetExtension()));

    fs::path DbPath;
    if (!AskDatabase(DbPath, ExtList))
      return false;

    Database::SharedPtr spDb;
    for (auto itDb = std::begin(AllDbs), itEnd = std::end(AllDbs); itDb != itEnd; ++itDb)
      if ((*itDb)->IsCompatible(DbPath.wstring()))
      {
        spDb = *itDb;
        break;
      }

    if (spDb == nullptr)
    {
      Log::Write("core") << "unable to find a suitable database" << LogEnd;
      return false;
    }

    if (!spDb->Open(DbPath.wstring()))
    {
      Log::Write("core") << "unable to open database" << LogEnd;
      return false;
    }

    rModMgr.UnloadModules();
    rModMgr.LoadModules(L".", *spDb->GetBinaryStream());

    Log::Write("core") << "opening database \"" << DbPath.string() << "\"" << LogEnd;

    m_Document.Use(spDb);

    auto const& ArchTags = spDb->GetArchitectureTags();
    auto const& AllArchs = rModMgr.GetArchitectures();
    for (auto itArchTag = std::begin(ArchTags), itEnd = std::end(ArchTags); itArchTag != itEnd; ++itArchTag)
    {
      for (auto itArch = std::begin(AllArchs), itArchEnd = std::end(AllArchs); itArch != itArchEnd; ++itArch)
      {
        if (MEDUSA_CMP_TAG((*itArch)->GetTag(), (*itArchTag)))
        {
          rModMgr.RegisterArchitecture(*itArch);
          break;
        }
      }
    }

    return true;
  }
  catch (Exception &e)
  {
    Log::Write("core") << "exception: \"" << e.What() << "\"" << LogEnd;
  }
  return false;
}

bool Medusa::CloseDocument(void)
{
  return false;
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

  AddTask(m_Analyzer.CreateDisassembleTask(m_Document, rAddr, *spArch, Mode));
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
  return m_Analyzer.FormatCell(m_Document, m_Document.GetBinaryStream(), rAddress, rCell, rStrCell, rMarks);
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
  return m_Analyzer.FormatMultiCell(m_Document, m_Document.GetBinaryStream(), rAddress, rMultiCell, rStrMultiCell, rMarks);
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
  //if (m_Analyzer.CreateFunction(m_Document, rAddr))
  //{
  //  //if (m_spOperatingSystem)
  //  //  m_spOperatingSystem->AnalyzeFunction(rAddr, m_Analyzer);
  //  return true;
  //}
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
