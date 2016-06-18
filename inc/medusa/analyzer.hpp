#ifndef MEDUSA_ANALYZER_HPP
#define MEDUSA_ANALYZER_HPP

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

class AnalyzerPass
{
public:
  AnalyzerPass(std::string const& rName, Document& rDoc, Address const& rAddr)
    : m_Name(rName), m_rDoc(rDoc), m_Addr(rAddr)
  {}

  std::string const& GetName(void) const { return m_Name; }

protected:
  std::string m_Name;
  Document& m_rDoc;
  Address m_Addr;
};

class AnalyzerTask : public Task
{
public:
  typedef std::function<void(Document& rDoc)> TaskFunctionType;

  AnalyzerTask(std::string const& rTaskName, Document& rDoc, TaskFunctionType TaskFunc)
    : m_Name(rTaskName), m_rDoc(rDoc), m_TaskFunc(TaskFunc) {}

  virtual std::string GetName(void) const { return m_Name; }
  virtual void Run(void) { m_TaskFunc(m_rDoc); }

private:
  std::string m_Name;
  Document& m_rDoc;
  TaskFunctionType m_TaskFunc;
};

class AnalyzerTaskAddress : public Task
{
public:
  typedef std::function<void(Document& rDoc, Address const& rAddr)> TaskFunctionType;

  AnalyzerTaskAddress(std::string const& rTaskName, Document& rDoc, Address const& rAddr, TaskFunctionType TaskFunc)
    : m_Name(rTaskName), m_rDoc(rDoc), m_Addr(rAddr), m_TaskFunc(TaskFunc) {}

  virtual std::string GetName(void) const { return m_Name; }
  virtual void Run(void) { m_TaskFunc(m_rDoc, m_Addr); }

  void SetAddress(Address const& rAddr) { m_Addr = rAddr; }

private:
  std::string m_Name;
  Document& m_rDoc;
  Address m_Addr;
  TaskFunctionType m_TaskFunc;
};

class AnalyzerDisassemble : public AnalyzerPass
{
public:
  AnalyzerDisassemble(Document& rDoc, Address const& rAddr) : AnalyzerPass("disassemble", rDoc, rAddr) {}

  bool DisassembleOneInstruction(void);
  bool Disassemble(void);
  bool DisassembleBasicBlock(std::list<Instruction::SPType>& rBasicBlock);

  bool DisassembleWith(Architecture& rArch, u8 Mode);
  bool DisassembleBasicBlockWith(Architecture& rArch, u8 Mode, std::list<Instruction::SPType>& rBasicBlock);

  bool BuildControlFlowGraph(ControlFlowGraph& rCfg);

  bool DisassembleUsingSymbolicExecution(void);
};

class AnalyzerInstruction : public AnalyzerPass
{
public:
  AnalyzerInstruction(Document& rDoc, Address const& rAddr) : AnalyzerPass("instruction", rDoc, rAddr) {}

  bool FindCrossReference(void);
  bool FindString(void);
};

class AnalyzerBasicBlock : public AnalyzerPass
{
public:
  AnalyzerBasicBlock(Document& rDoc, Address const& rAddr) : AnalyzerPass("basic block", rDoc, rAddr) {}
};

class AnalyzerFunction : public AnalyzerPass
{
public:
  AnalyzerFunction(Document& rDoc, Address const& rAddr) : AnalyzerPass("function", rDoc, rAddr) {}

  bool CreateFunction(void);
  bool ComputeFunctionLength(u16& rFunctionLength, u16& rInstructionCounter, u32 LengthThreshold = 0x10000);
  //bool DetermineCallingConvention();
  //bool AnalyzeStack();
};

class AnalyzerString : public AnalyzerPass
{
public:
  AnalyzerString(Document& rDoc, Address const& rAddr) : AnalyzerPass("string", rDoc, rAddr) {}

  bool CreateUtf8String(void);
  bool CreateUtf16String(void);
  //bool DetermineStringType();
};

//! Analyzer handles all analysis operations.
class MEDUSA_EXPORT Analyzer
{
public:

  Task* CreateTask(std::string const& rTaskName, Document& rDoc);
  Task* CreateTask(std::string const& rTaskName, Document& rDoc, Address const& rAddr);
  Task* CreateTask(std::string const& rTaskName, Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode);

  bool BuildControlFlowGraph(Document& rDoc, std::string const& rLblName, ControlFlowGraph& rCfg) const;
  bool BuildControlFlowGraph(Document& rDoc, Address const& rAddr,        ControlFlowGraph& rCfg) const;

  bool FormatCell(Document const& rDoc, Address const& rAddress, Cell const& rCell, PrintData &rPrintData) const;
  bool FormatMultiCell(Document const& rDoc, Address const& rAddress, MultiCell const& rMultiCell, PrintData& rPrintData) const;
  bool FormatGraph(Document const& rDoc, Graph const& rGraph, GraphData& rGraphData) const;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_DISASSEMBLER_HPP
