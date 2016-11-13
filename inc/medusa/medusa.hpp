#ifndef MEDUSA_CORE_HPP
#define MEDUSA_CORE_HPP

#include "medusa/namespace.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/task.hpp"
#include "medusa/architecture.hpp"
#include "medusa/loader.hpp"
#include "medusa/os.hpp"
#include "medusa/emulation.hpp"
#include "medusa/database.hpp"
#include "medusa/document.hpp"
#include "medusa/analyzer.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

MEDUSA_NAMESPACE_BEGIN

//! Medusa is a main class, it's able to handle almost all sub-classes.
class MEDUSA_EXPORT Medusa
{
public:
                                  Medusa(void);
                                 ~Medusa(void);

  static std::string              GetVersion(void);

  bool                            AddTask(Task* pTask);
  void                            WaitForTasks(void);

  bool                            Start(
    BinaryStream::SPType spBinaryStream,
    Database::SPType spDatabase,
    Loader::SPType spLoader,
    Architecture::VSPType spArchitectures,
    OperatingSystem::SPType spOperatingSystem,
    bool StartAnalyzer = true);

  typedef std::tuple<std::string, std::string> Filter;
  typedef std::function<bool (
    Path& rDatabasePath,
    std::list<Filter> const& rExtensionFilter
    )> AskDatabaseFunctionType;

  static bool IgnoreDatabasePath(
    Path& rDatabasePath,
    std::list<Filter> const& rExtensionFilter);

  typedef std::function<bool (
    BinaryStream::SPType& rspBinStrm,
    Database::SPType& rspDatabase,
    Loader::SPType& rspLoader,
    Architecture::VSPType& rspArchitectures,
    OperatingSystem::SPType& rspOperatingSystem
    )> ModuleSelectorFunctionType;

  typedef std::function<bool (void)> FunctionType;

  static bool DefaultModuleSelector(
    BinaryStream::SPType spBinStrm,
    Database::SPType& rspDatabase,
    Loader::SPType& rspLoader,
    Architecture::VSPType& rspArchitectures,
    OperatingSystem::SPType& rspOperatingSystem
    );

  bool                            NewDocument(
    BinaryStream::SPType spBinStrm,
    bool StartAnalyzer = true,
    AskDatabaseFunctionType AskDatabase = IgnoreDatabasePath,
    ModuleSelectorFunctionType ModuleSelector = DefaultModuleSelector,
    FunctionType BeforeStart = [](){ return true; },
    FunctionType AfterStart  = [](){ return true; });
  bool                            OpenDocument(AskDatabaseFunctionType AskDatabase);
  bool                            CloseDocument(void);

                                  //! This method returns the current document.
  Document&                       GetDocument(void)       { return m_Document; }
  Document const&                 GetDocument(void) const { return m_Document; }

                                  /*! This method starts the analyze.
                                   * \param spArch is the selected Architecture.
                                   * \param rAddr is the start address of disassembling.
                                   * \param Mode allows to tell which architecture mode should be used
                                   */
  void                            Analyze(Address const& rAddr, Architecture::SPType spArch = nullptr, u8 Mode = 0);

                                  /*! This method builds a control flow graph from an address.
                                   * \param rAddr is the start address.
                                   * \param rCfg is the filled control flow graph.
                                   */
  bool                            BuildControlFlowGraph(Address const& rAddr, ControlFlowGraph& rCfg);

  bool FormatCell(
    Address       const& rAddress,
    Cell          const& rCell,
    PrintData          & rPrintData) const;

  bool              FormatMultiCell(
    Address       const& rAddress,
    MultiCell     const& rMultiCell,
    PrintData          & rPrintData) const;

  bool FormatGraph(
    Graph const& rGraph,
    GraphData& rGraphData) const;

                                  //! This method makes a fully filled Address if possible. @see Address
  Address                         MakeAddress(OffsetType Offset);
  Address                         MakeAddress(BaseType Base, OffsetType Offset);
  Address                         MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, OffsetType Offset);
  Address                         MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, BaseType Base, OffsetType Offset);

  bool                            CreateFunction(Address const& rAddr);
  bool                            CreateUtf8String(Address const& rAddr);
  bool                            CreateUtf16String(Address const& rAddr);
  void                            FindFunctionAddressFromAddress(Address::Vector& rFunctionAddress, Address const& rAddress) const;

  bool                            AddTask(std::string const& rTaskName);
  bool                            AddTask(std::string const& rTaskName, Address const& rAddr);
  bool                            AddTask(std::string const& rTaskName, Address const& rAddr, Architecture& rArch, u8 Mode);
private:
  TaskManager        m_TaskManager;
  Document           m_Document;

  Analyzer           m_Analyzer;

  typedef std::mutex MutexType;
  mutable MutexType  m_Mutex;
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_CORE_HPP
