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
private:
  // Move these classes into analyzer.cpp
  class MakeFunctionTask : public Task
  {
  public:
    MakeFunctionTask(Document& rDoc, Address const& rFuncAddr);
    virtual ~MakeFunctionTask(void);

    virtual std::string GetName(void) const;
    virtual void Run(void);

  protected:
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

    Document& m_rDoc;
    Address   m_Addr;
  };

  class DisassembleTask : public MakeFunctionTask
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

  class DisassembleAllFunctionsTask : public Task
  {
  public:
    DisassembleAllFunctionsTask(Document& rDoc);
    ~DisassembleAllFunctionsTask(void);

    virtual std::string GetName(void) const;
    virtual void Run(void);

  private:
    Document& m_rDoc;
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

  class StackAnalyzerTask : public Task
  {
  public:
    StackAnalyzerTask(Document& rDoc, Address const& rFuncAddr);
    ~StackAnalyzerTask(void);

    virtual std::string GetName(void) const;
    virtual void Run(void);

  protected:
    Document& m_rDoc;
    Address m_FuncAddr;
  };

public:

  Analyzer(void)
  : m_FunctionPrefix("fcn_")
  , m_LabelPrefix("lbl_")
  , m_DataPrefix("dat_")
  , m_StringPrefix("str_")
  {}

  ~Analyzer(void) { }

  Task* CreateMakeFunctionTask(Document& rDoc, Address const& rAddr) const
  { return new MakeFunctionTask(rDoc, rAddr); }
  Task* CreateDisassembleTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode) const
  { return new DisassembleTask(rDoc, rAddr, rArch, Mode); }
  Task* CreateDisassembleFunctionTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode) const
  { return new DisassembleFunctionTask(rDoc, rAddr, rArch, Mode); }
  Task* CreateFunctionTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode) const
  { return new DisassembleAllFunctionsTask(rDoc); }
  Task* CreateFindAllStringTask(Document& rDoc) const
  { return new FindAllStringTask(rDoc); }
  Task* CreateDisassembleAllFunctionsTask(Document& rDoc) const
  { return new DisassembleAllFunctionsTask(rDoc); }
  Task* CreateStackAnalyzerTask(Document& rDoc, Address const& rFuncAddr) const
  { return new StackAnalyzerTask(rDoc, rFuncAddr); }

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

  bool BuildControlFlowGraph(Document const& rDoc, std::string const& rLblName, ControlFlowGraph& rCfg) const;
  bool BuildControlFlowGraph(Document const& rDoc, Address const& rAddr,        ControlFlowGraph& rCfg) const;

  bool FormatCell(
    Document      const& rDoc,
    Address       const& rAddress,
    Cell          const& rCell,
    PrintData          & rPrintData) const;

  bool FormatMultiCell(
    Document      const& rDoc,
    Address       const& rAddress,
    MultiCell     const& rMultiCell,
    PrintData          & rPrintData) const;

  std::string          m_FunctionPrefix; //! Function prefix
  std::string          m_LabelPrefix;    //! Label prefix
  std::string          m_DataPrefix;     //! Data prefix
  std::string          m_StringPrefix;   //! String prefix
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_DISASSEMBLER_
