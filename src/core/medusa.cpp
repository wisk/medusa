#include "medusa/medusa.hpp"
#include "medusa/config.hpp"
#include "medusa/module.hpp"
#include "medusa/xref.hpp"
#include "medusa/log.hpp"
#include "medusa/user_configuration.hpp"

#include <cstring>
#include <cstdlib>
#include <list>
#include <algorithm>
#include <boost/filesystem/operations.hpp>
#include <fmt/format.h>

MEDUSA_NAMESPACE_BEGIN

Medusa::Medusa(void)
  : m_TaskManager([] (Task const* pTask) { })
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
  return fmt::format("Medusa %1%.%2%.%3% (%4%) %5% - %6%"
    , Version::Major
    , Version::Minor
    , Version::Patch
    , pBuildType
    , pBuildDate
    , pBuildTime);
}

bool Medusa::AddTask(Task* pTask)
{
  return m_TaskManager.AddTask(pTask);
}

void Medusa::WaitForTasks(void)
{
  m_TaskManager.Wait();
}

bool Medusa::IgnoreDatabasePath(
        Path& rDatabasePath,
  std::list<Filter> const& rExtensionFilter)
{
  rDatabasePath = boost::filesystem::unique_path();
  return true;
}

bool Medusa::DefaultModuleSelector(
  BinaryStream::SPType& rspBinStrm,
  Database::SPType& rspDatabase,
  Loader::SPType& rspLoader,
  Architecture::VSPType& rspArchitectures,
  OperatingSystem::SPType& rspOperatingSystem)
{
  auto const& rModMngr = ModuleManager::Instance();
  auto ArchNames = rspLoader->GetUsedArchitectures(*rspBinStrm);
  for (auto ArchName : ArchNames)
  {
    TagType ArchTag;
    u8 ArchMode;
    if (!rModMngr.ConvertArchitectureNameToTagAndMode(ArchName, ArchTag, ArchMode))
      return false;
    rspArchitectures.push_back(rModMngr.GetArchitecture(ArchTag));
  }

  auto const& rSysName = rspLoader->GetSystemName(*rspBinStrm);
  if (!rSysName.empty())
    rspOperatingSystem = rModMngr.GetOperatingSystem(rSysName);

  return true;
}

bool Medusa::NewDocument(
  BinaryStream::SPType spBinStrm,
  bool StartAnalyzer,
  Medusa::AskDatabaseFunctionType AskDatabase,
  Medusa::ModuleSelectorFunctionType ModuleSelector)
{
  try
  {
    auto& rModMgr = ModuleManager::Instance();
    UserConfiguration UserCfg;
    auto ModPath = UserCfg.GetOption("core.modules_path");
    if (ModPath.empty())
      ModPath = ".";
    rModMgr.InitializeModules(ModPath);

    // Get all loaders and remove which are not needed
    auto AllLdrs = rModMgr.MakeAllLoaders();
    if (AllLdrs.empty())
    {
      Log::Write("core").Level(LogError) << "there is not supported loader" << LogEnd;
      return false;
    }
    AllLdrs.erase(std::remove_if(std::begin(AllLdrs), std::end(AllLdrs), [&spBinStrm](Loader::SPType spLdr)
    {
      return spLdr->IsCompatible(*spBinStrm) ? false : true;
    }), std::end(AllLdrs));

    Database::SPType spDb;
    Loader::SPType spLdr;
    Architecture::VSPType vspArchs;
    OperatingSystem::SPType spOs;
    if (!ModuleSelector(spBinStrm, spDb, spLdr, vspArchs, spOs))
      return false;

    bool Force = true;
    Path DbPath;
    std::list<Filter> ExtList;
    if (!AskDatabase(DbPath, ExtList))
    {
      Log::Write("core").Level(LogError) << "failed to ask for database path" << LogEnd;
      return false;
    }
    if (DbPath.empty())
    {
      Log::Write("core").Level(LogError) << "path to database is empty" << LogEnd;
      return false;
    }
    if (!spDb->Create(DbPath, Force))
    {
      Log::Write("core").Level(LogError) << "failed to create database to: " << DbPath.string() << LogEnd;
      return false;
    }

    /* Bind a binary stream to the database */
    spDb->SetBinaryStream(spBinStrm);

    /* Bind the database to the document */
    if (!m_Document.Open(spDb))
      return false;

    /* Save used architectures */
    for (auto const spArch : vspArchs)
    {
      if (!spDb->RegisterArchitectureTag(spArch->GetTag()))
        return false;
    }

    /* Map memory areas */
    if (!spLdr->Map(m_Document))
      return false;

    /* Create symbols, functions, ... */
    spLdr->Analyze(m_Document);

    if (StartAnalyzer)
    {
      /* Disassemble the file with the default analyzer */
      AddTask(m_Analyzer.CreateTask("disassemble all functions", m_Document));

      /* Analyze the stack for each functions */
      //AddTask(m_Analyzer.CreateAnalyzeStackAllFunctionsTask(m_Document));

      /* Find all strings using the previous analyze */
      AddTask(m_Analyzer.CreateTask("find all strings", m_Document));
    }

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
  return false;
}

bool Medusa::CloseDocument(void)
{
  m_Document.Close();
  return true;
}

void Medusa::Analyze(Address const& rAddr, Architecture::SPType spArch, u8 Mode)
{
  Cell::SPType spCell = nullptr;
  if (Mode == 0)
  {
    spCell = m_Document.GetCell(rAddr);
    if (spCell == nullptr)
      return;
    Mode = spCell->GetData()->GetMode();
  }

  if (spArch == nullptr)
  {
    if (spCell == nullptr)
    {
      spCell = m_Document.GetCell(rAddr);
      if (spCell == nullptr)
        return;
    }
    spArch = ModuleManager::Instance().GetArchitecture(spCell->GetArchitectureTag());
    if (spArch == nullptr)
      return;
  }

  if (Mode == 0)
    Mode = spArch->GetDefaultMode(rAddr);

  AddTask(m_Analyzer.CreateTask("disassemble with", m_Document, rAddr, *spArch, Mode));
}

bool Medusa::BuildControlFlowGraph(Address const& rAddr, Graph& rCfg)
{
  return m_Analyzer.BuildControlFlowGraph(m_Document, rAddr, rCfg);
}

bool Medusa::FormatCell(
  Address       const& rAddress,
  Cell          const& rCell,
  PrintData          & rPrintData) const
{
  return m_Analyzer.FormatCell(m_Document, rAddress, rCell, rPrintData);
}

bool Medusa::FormatMultiCell(
  Address       const& rAddress,
  MultiCell     const& rMultiCell,
  PrintData          & rPrintData) const
{
  return m_Analyzer.FormatMultiCell(m_Document, rAddress, rMultiCell, rPrintData);
}

bool Medusa::FormatGraph(
  Graph const& rGraph,
  GraphData& rGraphData) const
{
  return m_Analyzer.FormatGraph(m_Document, rGraph, rGraphData);
}

Address Medusa::MakeAddress(OffsetType Offset)
{
  return MakeAddress(Loader::SPType(), Architecture::SPType(), 0x0, Offset);
}

Address Medusa::MakeAddress(BaseType Base, OffsetType Offset)
{
  return MakeAddress(Loader::SPType(), Architecture::SPType(), Base, Offset);
}

Address Medusa::MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, OffsetType Offset)
{
  return MakeAddress(pLoader, pArch, 0x0, Offset);
}

Address Medusa::MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, BaseType Base, OffsetType Offset)
{
  Address NewAddr = m_Document.MakeAddress(Base, Offset);
  if (NewAddr.GetAddressingType() != Address::UnknownType)
    return NewAddr;

  return Address(Base, Offset);
}

bool Medusa::CreateFunction(Address const& rAddr)
{
  return AddTask(m_Analyzer.CreateTask("create function", m_Document, rAddr));
}

bool Medusa::CreateUtf8String(Address const& rAddr)
{
  return AddTask(m_Analyzer.CreateTask("create utf-8 string", m_Document, rAddr));
}

bool Medusa::CreateUtf16String(Address const& rAddr)
{
  return AddTask(m_Analyzer.CreateTask("create utf-16 string", m_Document, rAddr));
}

bool Medusa::AddTask(std::string const& rTaskName)
{
  return AddTask(m_Analyzer.CreateTask(rTaskName, m_Document));
}

bool Medusa::AddTask(std::string const& rTaskName, Address const& rAddr)
{
  return AddTask(m_Analyzer.CreateTask(rTaskName, m_Document, rAddr));
}

bool Medusa::AddTask(std::string const& rTaskName, Address const& rAddr, Architecture& rArch, u8 Mode)
{
  return AddTask(m_Analyzer.CreateTask(rTaskName, m_Document, rAddr, rArch, Mode));
}

MEDUSA_NAMESPACE_END
