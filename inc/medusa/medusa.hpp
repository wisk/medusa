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
#include <string>
#include <tuple>


MEDUSA_NAMESPACE_BEGIN

//! Medusa is a main class, it's able to handle almost all sub-classes.
class MEDUSA_EXPORT Medusa
{
public:
                                  Medusa();
                                 ~Medusa();

  static std::string              GetVersion();

  bool                            AddTask(Task* pTask);
  void                            WaitForTasks();

  bool                            Start(
    BinaryStream::SPType spBinaryStream,
    Database::SPType spDatabase,
    Loader::SPType spLoader,
    Architecture::VSPType spArchitectures,
    OperatingSystem::SPType spOperatingSystem,
    bool StartAnalyzer = true);

  using Filter = std::tuple<std::string, std::string>;
  using AskDatabaseFunctionType = std::function<bool (
    Path& rDatabasePath,
    std::list<Filter> const& rExtensionFilter
    )>;

  static bool IgnoreDatabasePath(
    Path& rDatabasePath,
    std::list<Filter> const& rExtensionFilter);

  using ModuleSelectorFunctionType = std::function<bool (
    BinaryStream::SPType& rspBinStrm,
    Database::SPType& rspDatabase,
    Loader::SPType& rspLoader,
    Architecture::VSPType& rspArchitectures,
    OperatingSystem::SPType& rspOperatingSystem
    )>;

  static bool DefaultModuleSelector(
    BinaryStream::SPType& rspBinStrm,
    Database::SPType& rspDatabase,
    Loader::SPType& rspLoader,
    Architecture::VSPType& rspArchitectures,
    OperatingSystem::SPType& rspOperatingSystem);

  using FunctionType = std::function<bool ()>;

  bool NewDocument(
    BinaryStream::SPType spBinStrm,
    bool StartAnalyzer = true,
    AskDatabaseFunctionType AskDatabase = IgnoreDatabasePath,
    ModuleSelectorFunctionType ModuleSelector = DefaultModuleSelector);
  bool OpenDocument(AskDatabaseFunctionType AskDatabase);
  bool CloseDocument();

                                  //! This method returns the current document.
  Document&       GetDocument()       { return m_Document; }
  Document const& GetDocument() const { return m_Document; }

  /*! This method starts the analyze.
  * \param spArch is the selected Architecture.
  * \param rAddr is the start address of disassembling.
  * \param Mode allows to tell which architecture mode should be used
  */
  void  Analyze(Address const& rAddr, Architecture::SPType spArch = nullptr, u8 Mode = 0);

  /*! This method builds a control flow graph from an address.
  * \param rAddr is the start address.
  * \param rCfg is the filled control flow graph.
  */
  bool BuildControlFlowGraph(Address const& rAddr, Graph& rCfg);

  bool FormatCell(
    Address const& rAddress,
    Cell const&    rCell,
    PrintData&     rPrintData) const;

  bool FormatMultiCell(
    Address const&   rAddress,
    MultiCell const& rMultiCell,
    PrintData&       rPrintData) const;

  bool FormatGraph(
    Graph const& rGraph,
    GraphData&   rGraphData) const;

                                  //! This method makes a fully filled Address if possible. @see Address
  Address MakeAddress(OffsetType Offset);
  Address MakeAddress(BaseType Base, OffsetType Offset);
  Address MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, OffsetType Offset);
  Address MakeAddress(Loader::SPType pLoader, Architecture::SPType pArch, BaseType Base, OffsetType Offset);

  bool CreateFunction(Address const& rAddr);
  bool CreateUtf8String(Address const& rAddr);
  bool CreateUtf16String(Address const& rAddr);
  void FindFunctionAddressFromAddress(Address::Vector& rFunctionAddress, Address const& rAddress) const;

  bool AddTask(std::string const& rTaskName);
  bool AddTask(std::string const& rTaskName, Address const& rAddr);
  bool AddTask(std::string const& rTaskName, Address const& rAddr, Architecture& rArch, u8 Mode);
private:
  TaskManager m_TaskManager;
  Document    m_Document;
  Analyzer    m_Analyzer;

  using MutexType = std::mutex;
  mutable MutexType  m_Mutex;
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_CORE_HPP
