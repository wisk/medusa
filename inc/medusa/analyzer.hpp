#ifndef _MEDUSA_ANALYZER_
#define _MEDUSA_ANALYZER_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/database.hpp"
#include "medusa/architecture.hpp"
#include "medusa/control_flow_graph.hpp"

#include <boost/thread/mutex.hpp>

MEDUSA_NAMESPACE_BEGIN

//! Analyzer handles all analysis operations.
class Medusa_EXPORT Analyzer
{
public:
  Analyzer(void)
  : m_FunctionPrefix("fcn_")
  , m_LabelPrefix("lbl_")
  , m_DataPrefix("dat_")
  , m_StringPrefix("str_")
  {}

  ~Analyzer(void) {}

  //! This method disassembles code by following the execution path.
  void DisassembleFollowingExecutionPath(Database& rDb, Address const& rEntrypoint, Architecture &rArch) const;

  //! This method finds and adds cross-references.
  void CreateXRefs(Database& rDb) const;

  //! This method finds string using specific patterns.
  void FindStrings(Database& rDb, Architecture& rArch) const;

  /*! This method computes the size of a function.
   * \param rDb contains all cells.
   * \param rFunctionAddress is the address of the function.
   * \param EndAddress is set by this method and contains the end of the function.
   * \param rFunctionLength is set by this method and contains the size of the function.
   * \param rInstructionCounter is set by this method and contains the number of instruction in the function.
   * \param LengthThreshold is the maximum size of this function, it this value is reached, this method returns false.
   * \return Returns true if the size of the function can be computed, otherwise it returns false.
   */
  bool ComputeFunctionLength(
    Database const& rDb,
    Address const& rFunctionAddress,
    Address& EndAddress,
    u16& rFunctionLength,
    u16& rInstructionCounter,
    u32 LengthThreshold) const;

  bool BuildControlFlowGraph(Database& rDb, std::string const& rLblName, ControlFlowGraph& rCfg);
  bool BuildControlFlowGraph(Database& rDb, Address const& rAddr,        ControlFlowGraph& rCfg);

private:
  static bool DisassembleBasicBlock(
      Database const& rDb,
      Architecture& rArch,
      Address const& rAddr,
      std::list<Instruction*>& rBasicBlock);

  std::string  m_FunctionPrefix; //! Function prefix
  std::string  m_LabelPrefix;    //! Label prefix
  std::string  m_DataPrefix;     //! Data prefix
  std::string  m_StringPrefix;   //! String prefix
  mutable boost::mutex m_DisasmMutex;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_DISASSEMBLER_