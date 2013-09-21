#ifndef _MEDUSA_ANALYZER_
#define _MEDUSA_ANALYZER_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/document.hpp"
#include "medusa/architecture.hpp"
#include "medusa/control_flow_graph.hpp"

//class ControlFlowGraph;
#include <fstream>

#include <boost/thread/mutex.hpp>
#include <boost/graph/graphviz.hpp>

MEDUSA_NAMESPACE_BEGIN

//! Analyzer handles all analysis operations.
class Medusa_EXPORT Analyzer
{
public:
  class Tracker
  {
  public:
    virtual bool Track(Analyzer& rAnlz, Document& rDoc, Address const& rAddr)
    { return false; }
    bool operator()(Analyzer& rAnlz, Document& rDoc, Address const& rAddr)
    { return Track(rAnlz, rDoc, rAddr); }
  };

  Analyzer(void)
  : m_FunctionPrefix("fcn_")
  , m_LabelPrefix("lbl_")
  , m_DataPrefix("dat_")
  , m_StringPrefix("str_")
  {}

  ~Analyzer(void) { }

  //! This method disassembles code by following the execution path.
  void DisassembleFollowingExecutionPath(Document& rDoc, Address const& rEntrypoint, Architecture &rArch) const;

  //! This method finds and adds cross-references.
  void CreateXRefs(Document& rDoc, Address const& rAddr) const;

  //! This method finds string using specific patterns.
  void FindStrings(Document& rDoc, Architecture& rArch) const;

  bool MakeAsciiString(Document& rDoc, Address const& rAddr) const;
  bool MakeWindowsString(Document& rDoc, Address const& rAddr) const;

  /*! This method computes the size of a function.
   * \param rDoc contains all cells.
   * \param rFunctionAddress is the address of the function.
   * \param EndAddress is set by this method and contains the end of the function.
   * \param rFunctionLength is set by this method and contains the size of the function.
   * \param rInstructionCounter is set by this method and contains the number of instruction in the function.
   * \param LengthThreshold is the maximum size of this function, it this value is reached, this method returns false.
   * \return Returns true if the size of the function can be computed, otherwise it returns false.
   */
  bool ComputeFunctionLength(
    Document const& rDoc,
    Address const& rFunctionAddress,
    Address& EndAddress,
    u16& rFunctionLength,
    u16& rInstructionCounter,
    u32 LengthThreshold) const;

  bool CreateFunction(Document& rDoc, Address const& rAddr) const;

  bool BuildControlFlowGraph(Document& rDoc, std::string const& rLblName, ControlFlowGraph& rCfg) const;
  bool BuildControlFlowGraph(Document& rDoc, Address const& rAddr,        ControlFlowGraph& rCfg) const;

  bool FormatCell(
    Document      const& rDoc,
    BinaryStream  const& rBinStrm,
    Address       const& rAddress,
    Cell          const& rCell,
    std::string        & rStrCell,
    Cell::Mark::List   & rMarks) const;

  bool FormatMultiCell(
    Document      const& rDoc,
    BinaryStream  const& rBinStrm,
    Address       const& rAddress,
    MultiCell     const& rMultiCell,
    std::string        & rStrMultiCell,
    Cell::Mark::List   & rMarks) const;

  void TrackOperand(Document& rDoc, Address const& rStartAddress, Tracker& rTracker);
  void BacktrackOperand(Document& rDoc, Address const& rStartAddress, Tracker& rTracker);

  static bool DisassembleBasicBlock(
    Document const& rDoc,
    Architecture& rArch,
    Address const& rAddr,
    std::list<Instruction::SPtr>& rBasicBlock);

  std::string          m_FunctionPrefix; //! Function prefix
  std::string          m_LabelPrefix;    //! Label prefix
  std::string          m_DataPrefix;     //! Data prefix
  std::string          m_StringPrefix;   //! String prefix
  mutable boost::mutex m_DisasmMutex;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_DISASSEMBLER_