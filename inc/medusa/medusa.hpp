#ifndef _MEDUSA_
#define _MEDUSA_

#include "medusa/namespace.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/architecture.hpp"
#include "medusa/loader.hpp"
#include "medusa/os.hpp"
#include "medusa/emulation.hpp"
#include "medusa/database.hpp"
#include "medusa/analyzer.hpp"

#include <vector>
#include <iostream>
#include <iomanip>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Medusa is a main class, it's able to handle almost all sub-classes.
class Medusa_EXPORT Medusa
{
public:
  typedef std::map<std::string, TGetEmulator> EmulatorMap;

                                  Medusa(void);
                                  Medusa(std::wstring const& rFilePath);
                                 ~Medusa(void);

                                  /*! This method opens a file for being disassembled.
                                   * It returns nothing but could throws exception @see Exception
                                   * \param rFilePath is the path to the file.
                                   */
  void                            Open(std::wstring const& rFilePath);

                                  //! This method returns true if a file is opened, otherwise it returns false.
  bool                            IsOpened(void) const;

                                  //! This method closes the current disassembled file and cleans all resources.
  void                            Close(void);

                                  //! This method returns available architectures. @see Architecture
  Architecture::VectorSharedPtr&  GetAvailableArchitectures(void) { return m_AvailableArchitectures; }
                                  //! This method returns available architectures. @see Architecture
  Architecture::VectorSharedPtr const& GetAvailableArchitectures(void) const { return m_AvailableArchitectures; }
                                  //! This method returns available loaders. @see Loader
  Loader::VectorSharedPtr const&  GetSupportedLoaders(void) const { return m_Loaders; }
                                  //! This method returns compatible operating system. @see OperatingSystem
  OperatingSystem::VectorSharedPtr GetCompatibleOperatingSystems(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch) const;
                                  //! This method returns available emulators. @see Emulator
  EmulatorMap const&              GetEmulators(void) const { return m_Emulators; }
                                  //! This methods loads all modules.
  void                            LoadModules(std::wstring const& rModulesPath);

  bool                            RegisterArchitecture(Architecture::SharedPtr spArch);

  bool                            UnregisterArchitecture(Architecture::SharedPtr spArch);

  void                            ConfigureEndianness(Architecture::SharedPtr spArch);

  void                            Start(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs);
  void                            StartAsync(Loader::SharedPtr spLdr, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs);

  void                            Disassemble(Architecture::SharedPtr spArch, Address const& rAddr);
  void                            DisassembleAsync(Address const& rAddr);
  void                            DisassembleAsync(Architecture::SharedPtr spArch, Address const& rAddr);

                                  /*! This method starts the analyze.
                                   * \param spArch is the selected Architecture.
                                   * \param rAddr is the start address of disassembling.
                                   */
  void                            Analyze(Architecture::SharedPtr spArch, Address const& rAddr);
  void                            AnalyzeAsync(Address const& rAddr);
  void                            AnalyzeAsync(Architecture::SharedPtr spArch, Address const& rAddr);

                                  /*! This method builds a control flow graph from an address.
                                   * \param rAddr is the start address.
                                   * \param rCfg is the filled control flow graph.
                                   */
  bool                            BuildControlFlowGraph(Address const& rAddr, ControlFlowGraph& rCfg);

  Cell*                           GetCell(Address const& rAddr);
  Cell const*                     GetCell(Address const& rAddr) const;

  MultiCell*                      GetMultiCell(Address const& rAddr);
  MultiCell const*                GetMultiCell(Address const& rAddr) const;

                                  //! This method returns the current database.
  Database&                       GetDatabase(void)       { return m_Database; }
  Database const&                 GetDatabase(void) const { return m_Database; }

                                  //! This method makes a fully filled Address if possible. @see Address
  Address                         MakeAddress(TOffset Offset);
  Address                         MakeAddress(TBase Base, TOffset Offset);
  Address                         MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TOffset Offset);
  Address                         MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TBase Base, TOffset Offset);

  bool                            CreateFunction(Address const& rAddr);
  void                            FindFunctionAddressFromAddress(Address::List& rFunctionAddress, Address const& rAddress) const;
  void                            DumpControlFlowGraph(Function const& rFunc, std::string const& rFilename) const
  { m_Analyzer.DumpControlFlowGraph(rFilename, rFunc.GetControlFlowGraph(), m_Database, m_FileBinStrm); }

  void TrackOperand(Address const& rStartAddress, Analyzer::Tracker& rTracker);
  void BacktrackOperand(Address const& rStartAddress, Analyzer::Tracker& rTracker);

private:
  FileBinaryStream                 m_FileBinStrm;
  Database                         m_Database;
  Architecture::VectorSharedPtr    m_AvailableArchitectures;
  Loader::VectorSharedPtr          m_Loaders;
  OperatingSystem::VectorSharedPtr m_CompatibleOperatingSystems;
  Analyzer                         m_Analyzer; /* don't shorten this word :) */
  typedef boost::mutex             MutexType;
  mutable MutexType                m_Mutex;
  EmulatorMap                      m_Emulators;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_