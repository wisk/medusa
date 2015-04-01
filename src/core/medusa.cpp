#include "medusa/medusa.hpp"
#include "medusa/version.hpp"
#include "medusa/module.hpp"
#include "medusa/xref.hpp"
#include "medusa/log.hpp"
#include "medusa/user_configuration.hpp"

#include <cstring>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <boost/filesystem/operations.hpp>

MEDUSA_NAMESPACE_BEGIN

Medusa::Medusa(void)
  : m_TaskManager([] (Task const* pTask) { Log::Write("core") << "Task \"" << pTask->GetName() << "\" is done" << LogEnd; })
  , m_Document()
  , m_Analyzer()
{
  m_TaskManager.Start();
  Log::Write("core") << GetVersion() << LogEnd;
}

Medusa::~Medusa(void)
{
  m_TaskManager.Stop();
}

std::string Medusa::GetVersion(void)
{
#ifdef _DEBUG
  char const* pBuildType = "debug";
#else
  char const* pBuildType = "release";
#endif
  char const* pBuildDate = __DATE__;
  char const* pBuildTime = __TIME__;
  return std::move((boost::format("Medusa %1%.%2%.%3% (%4%) %5% - %6%")
    % Version::Major
    % Version::Minor
    % Version::Patch
    % pBuildType
    % pBuildDate
    % pBuildTime).str());
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
  BinaryStream::SPType spBinaryStream,
  Database::SPType spDatabase,
  Loader::SPType spLoader,
  Architecture::VSPType spArchitectures,
  OperatingSystem::SPType spOperatingSystem)
{
  if (spArchitectures.empty())
    return false;

  for (auto const& rArch : spArchitectures)
  {
    if (!ModuleManager::Instance().RegisterArchitecture(rArch))
      Log::Write("core") << "unable to register architecture " << rArch->GetName() << " to module manager" << LogEnd;
    if (!spDatabase->RegisterArchitectureTag(rArch->GetTag()))
      Log::Write("core") << "unable to register architecture " << rArch->GetName() << " to database" << LogEnd;
  }

  spDatabase->SetBinaryStream(spBinaryStream);

  /* Set the endianness for the binary stream */
  spBinaryStream->SetEndianness(spArchitectures.front()->GetEndianness()); // LATER: does it always true?

  /* Set the database to the document */
  m_Document.Use(spDatabase);

  /* Map the file to the document */
  spLoader->Map(m_Document, spArchitectures); // Should it be async?

  /* Try to define functions and structures if possible */
  if (spOperatingSystem)
  {
    spDatabase->SetOperatingSystemName(spOperatingSystem->GetName());
    spOperatingSystem->ProvideDetails(m_Document);
  }

  /* Disassemble the file with the default analyzer */
  //AddTask(m_Analyzer.CreateDisassembleAllFunctionsTask(m_Document));

  /* Analyze the stack for each functions */
  //AddTask(m_Analyzer.CreateAnalyzeStackAllFunctionsTask(m_Document));

  ///* Find all strings using the previous analyze */
  //AddTask(m_Analyzer.CreateFindAllStringTask(m_Document));

  /* Analyze all functions */
  if (spOperatingSystem)
  {
    auto MCells = m_Document.GetMultiCells();
    for (auto const &MCell : MCells)
    {
      if (MCell.second->GetType() != MultiCell::FunctionType)
        continue;
      spOperatingSystem->AnalyzeFunction(m_Document, MCell.first);
    }
  }

  return true;
}

bool Medusa::IgnoreDatabasePath(
  Path& rDatabasePath,
  std::list<Filter> const& rExtensionFilter)
{
  rDatabasePath = boost::filesystem::unique_path();
  return true;
}

bool Medusa::DefaultModuleSelector(BinaryStream::SPType spBinStrm, Database::SPType& rspDatabase, Loader::SPType& rspLoader,
                                   Architecture::VSPType& rspArchitectures, OperatingSystem::SPType& rspOperatingSystem)
{
  auto& rModMgr = ModuleManager::Instance();
  auto AllDbs = rModMgr.GetDatabases();
  if (AllDbs.empty())
    return false;
  rspDatabase =  AllDbs.front();
  auto AllLdrs = rModMgr.GetLoaders();
  if (AllLdrs.empty())
    return false;
  rspLoader = AllLdrs.front();
  rspArchitectures = rModMgr.GetArchitectures();
  rspLoader->FilterAndConfigureArchitectures(rspArchitectures);
  if (rspArchitectures.empty())
    return false;
  rspOperatingSystem = rModMgr.GetOperatingSystem(rspLoader, rspArchitectures.front());
  return true;
}

bool Medusa::NewDocument(
  BinaryStream::SPType spBinStrm,
  Medusa::AskDatabaseFunctionType AskDatabase,
  Medusa::ModuleSelectorFunctionType ModuleSelector,
  Medusa::FunctionType BeforeStart,
  Medusa::FunctionType AfterStart)
{
  try
  {
    auto& rModMgr = ModuleManager::Instance();
    rModMgr.UnloadModules(); // Make sure we don't have loaded modules in memory
    UserConfiguration UserCfg;
    auto ModPath = UserCfg.GetOption("core.modules_path");
    if (ModPath.empty())
      ModPath = ".";
    rModMgr.LoadModules(ModPath, *spBinStrm);

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

    Database::SPType spCurDb;
    Loader::SPType spCurLdr;
    Architecture::VSPType spCurArchs;
    OperatingSystem::SPType spCurOs;

    if (!ModuleSelector(spBinStrm, spCurDb, spCurLdr, spCurArchs, spCurOs))
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
    Path DbPath = spBinStrm->GetPath();
    DbPath += spCurDb->GetExtension().c_str();
    while (!spCurDb->Create(DbPath, Force))
    {
      Log::Write("core") << "unable to create database file \"" << DbPath.string() << "\"" << LogEnd;
      Path NewDbPath = DbPath;
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
    if (!Start(spBinStrm, spCurDb, spCurLdr, spCurArchs, spCurOs))
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
    UserConfiguration UserCfg;
    auto ModPath = UserCfg.GetOption("core.modules_path");
    if (ModPath.empty())
      ModPath = ".";
    rModMgr.LoadDatabases(ModPath);

    auto const& AllDbs = rModMgr.GetDatabases();
    std::list<Filter> ExtList;
    for (auto itDb = std::begin(AllDbs), itEnd = std::end(AllDbs); itDb != itEnd; ++itDb)
      ExtList.push_back(std::make_tuple((*itDb)->GetName(), (*itDb)->GetExtension()));

    Path DbPath;
    if (!AskDatabase(DbPath, ExtList))
      return false;

    Database::SPType spDb;
    for (auto itDb = std::begin(AllDbs), itEnd = std::end(AllDbs); itDb != itEnd; ++itDb)
      if ((*itDb)->IsCompatible(DbPath))
      {
        spDb = *itDb;
        break;
      }

    if (spDb == nullptr)
    {
      Log::Write("core") << "unable to find a suitable database" << LogEnd;
      return false;
    }

    if (!spDb->Open(DbPath))
    {
      Log::Write("core") << "unable to open database" << LogEnd;
      return false;
    }

    rModMgr.UnloadModules();
    ModPath = UserCfg.GetOption("core.modules_path");
    if (ModPath.empty())
      ModPath = ".";
    rModMgr.LoadDatabases(ModPath);
    rModMgr.LoadModules(ModPath, *spDb->GetBinaryStream());

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
  m_Document.RemoveAll();
  return true;
}

void Medusa::Analyze(Address const& rAddr, Architecture::SPType spArch, u8 Mode)
{
  Cell::SPType spCell = nullptr;

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

Cell::SPType Medusa::GetCell(Address const& rAddr)
{
  return m_Document.GetCell(rAddr);
}

Cell::SPType const Medusa::GetCell(Address const& rAddr) const
{
  return m_Document.GetCell(rAddr);
}

bool Medusa::FormatCell(
  Address       const& rAddress,
  Cell          const& rCell,
  PrintData          & rPrintData) const
{
  return m_Analyzer.FormatCell(m_Document, rAddress, rCell, rPrintData);
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
  PrintData          & rPrintData) const
{
  return m_Analyzer.FormatMultiCell(m_Document, rAddress, rMultiCell, rPrintData);
}

Address Medusa::MakeAddress(TOffset Offset)
{
  return MakeAddress(Loader::SPType(), Architecture::SPType(), 0x0, Offset);
}

Address Medusa::MakeAddress(TBase Base, TOffset Offset)
{
  return MakeAddress(Loader::SPType(), Architecture::SPType(), Base, Offset);
}

Address Medusa::MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, TOffset Offset)
{
  return MakeAddress(pLoader, pArch, 0x0, Offset);
}

Address Medusa::MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, TBase Base, TOffset Offset)
{
  Address NewAddr = m_Document.MakeAddress(Base, Offset);
  if (NewAddr.GetAddressingType() == Address::UnknownType)
    return NewAddr;

  return Address(Base, Offset);
}

bool Medusa::CreateFunction(Address const& rAddr)
{
  AddTask(m_Analyzer.CreateMakeFunctionTask(m_Document, rAddr));
  return true;
}

MEDUSA_NAMESPACE_END
