#ifndef _MEDUSA_
#define _MEDUSA_

#include "medusa/namespace.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/architecture.hpp"
#include "medusa/loader.hpp"
#include "medusa/database.hpp"
#include "medusa/analyzer.hpp"

#include <vector>
#include <iostream>
#include <iomanip>

#include <boost/shared_ptr.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Medusa is a main class, it's able to handle almost all sub-classes.
class Medusa_EXPORT Medusa
{
public:
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
                                  //! This methods loads all modules.
  void                            LoadModules(std::wstring const& rModulesPath);

  bool                            RegisterArchitecture(Architecture::SharedPtr spArch);

  bool                            UnregisterArchitecture(Architecture::SharedPtr spArch);

                                  /*! This method starts the disassembling.
                                   * \param pLoader is the selected Loader.
                                   * \param pArch is the selected Architecture.
                                   */
  void                            Disassemble(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch);

  void                            DisassembleAsync(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch);

                                  /*! This method builds a control flow graph from an address.
                                   * \param rAddr is the start address.
                                   * \param rCfg is the filled control flow graph.
                                   */
  bool                            BuildControlFlowGraph(Address const& rAddr, ControlFlowGraph& rCfg);

                                  //! This method returns the current database.
  Database&                       GetDatabase(void)       { return m_Database; }
  Database const&                 GetDatabase(void) const { return m_Database; }

                                  //! This method makes a fully filled Address if possible. @see Address
  Address::SharedPtr              MakeAddress(TOffset Offset);
  Address::SharedPtr              MakeAddress(TBase Base, TOffset Offset);
  Address::SharedPtr              MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TOffset Offset);
  Address::SharedPtr              MakeAddress(Loader::SharedPtr pLoader, Architecture::SharedPtr pArch, TBase Base, TOffset Offset);

private:
  FileBinaryStream              m_FileBinStrm;
  Database                      m_Database;
  Architecture::VectorSharedPtr m_AvailableArchitectures;
  Architecture::TagMap          m_UsedArchitectures;
  Loader::VectorSharedPtr       m_Loaders;
  Analyzer                      m_Analyzer; /* don't shorten this word :) */
  u32                           m_ArchIdPool;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_