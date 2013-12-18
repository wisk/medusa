#ifndef _MEDUSA_ANALYZER_
#define _MEDUSA_ANALYZER_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/document.hpp"
#include "medusa/architecture.hpp"
#include "medusa/control_flow_graph.hpp"
#include "medusa/task.hpp"

#include <fstream>
#include <string>

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

private:
  class DisassembleTask : public Task
  {
  public:
    DisassembleTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode);
    virtual ~DisassembleTask(void);

    virtual std::string GetName(void) const;
    virtual void Run(void);

  protected:
    bool Disassemble(Address const& rAddr);
    bool DisassembleBasicBlock(Address const& rAddr, std::list<Instruction::SPtr>& rBasicBlock);
    bool CreateCrossReferences(Address const& rAddr);
    bool CreateFunction(Address const& rAddr);

    /*! This method computes the size of a function.
    * \param rFuncAddr is the function address.
    * \param EndAddress is set by this method and contains the end of the function.
    * \param rFunctionLength is set by this method and contains the size of the function.
    * \param rInstructionCounter is set by this method and contains the number of instruction in the function.
    * \param LengthThreshold is the maximum size of this function, it this value is reached, this method returns false.
    * \return Returns true if the size of the function can be computed, otherwise it returns false.
    */
    bool ComputeFunctionLength(Address const& rFuncAddr, Address& rEndAddress, u16& rFunctionLength, u16& rInstructionCounter, u32 LengthThreshold) const;

    Document&     m_rDoc;
    Address       m_Addr;
    Architecture& m_rArch;
    u8            m_Mode;
  };

  class DisassembleFunctionTask : public DisassembleTask
  {
  public:
    DisassembleFunctionTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode);
    virtual ~DisassembleFunctionTask(void);

    virtual std::string GetName(void) const;
    virtual void Run(void);
  };

  class DisassembleAllFunctionsTask : public DisassembleFunctionTask
  {
  public:
    DisassembleAllFunctionsTask(Document& rDoc, Architecture& rArch, u8 Mode);
    ~DisassembleAllFunctionsTask(void);

    virtual std::string GetName(void) const;
    virtual void Run(void);
  };

  class FindAllStringTask : public Task
  {
  public:
    FindAllStringTask(Document& rDoc);
    ~FindAllStringTask(void);

    virtual std::string GetName(void) const;
    virtual void Run(void);

  protected:
    Document& m_rDoc;
  };

public:

  Analyzer(void)
  : m_FunctionPrefix("fcn_")
  , m_LabelPrefix("lbl_")
  , m_DataPrefix("dat_")
  , m_StringPrefix("str_")
  {}

  ~Analyzer(void) { }

  Task* CreateDisassembleTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode) const
  { return new DisassembleTask(rDoc, rAddr, rArch, Mode); }
  Task* CreateDisassembleFunctionTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode) const
  { return new DisassembleFunctionTask(rDoc, rAddr, rArch, Mode); }
  Task* CreateDisassembleAllFunctionsTask(Document& rDoc, Architecture& rArch, u8 Mode) const
  { return new DisassembleAllFunctionsTask(rDoc, rArch, Mode); }
  Task* CreateFindAllStringTask(Document& rDoc) const
  { return new FindAllStringTask(rDoc); }


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

  bool BuildControlFlowGraph(Document const& rDoc, std::string const& rLblName, ControlFlowGraph& rCfg) const;
  bool BuildControlFlowGraph(Document const& rDoc, Address const& rAddr,        ControlFlowGraph& rCfg) const;

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

  std::string          m_FunctionPrefix; //! Function prefix
  std::string          m_LabelPrefix;    //! Label prefix
  std::string          m_DataPrefix;     //! Data prefix
  std::string          m_StringPrefix;   //! String prefix
  mutable boost::mutex m_DisasmMutex;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_DISASSEMBLER_
