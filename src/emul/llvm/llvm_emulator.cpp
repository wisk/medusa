#include "llvm_emulator.hpp"

#include "medusa/log.hpp"

// To avoid unresolved external symbol LLVMLinkInMCJIT
#include <llvm/ExecutionEngine/MCJIT.h>

MEDUSA_NAMESPACE_USE

llvm::Module*           LlvmEmulator::sm_pModule = nullptr;
llvm::ExecutionEngine*  LlvmEmulator::sm_pExecutionEngine = nullptr;
llvm::DataLayout*       LlvmEmulator::sm_pDataLayout = nullptr;

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt, VariableContext *pVarCtxt)
: Emulator(pCpuInfo, pCpuCtxt, pMemCtxt, pVarCtxt)
, m_Builder(llvm::getGlobalContext())
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::LLVMContext& rCtxt = llvm::getGlobalContext();
  std::string ErrStr;

    if (sm_pModule == nullptr)
      sm_pModule = new llvm::Module("medusa-emulator-llvm", rCtxt);
    if (sm_pExecutionEngine == nullptr)
      sm_pExecutionEngine = llvm::EngineBuilder(sm_pModule).setUseMCJIT(true).setErrorStr(&ErrStr).create();
    if (sm_pExecutionEngine == nullptr)
      Log::Write("emul_llvm") << "Error: " << ErrStr << LogEnd;
    if (sm_pDataLayout == nullptr)
      sm_pDataLayout = new llvm::DataLayout(sm_pModule);

    llvm::FunctionPassManager FuncPassMgr(sm_pModule);
    FuncPassMgr.add(new llvm::DataLayout(*sm_pExecutionEngine->getDataLayout()));
    FuncPassMgr.add(llvm::createBasicAliasAnalysisPass());
    FuncPassMgr.add(llvm::createInstructionCombiningPass());
    FuncPassMgr.add(llvm::createReassociatePass());
    FuncPassMgr.add(llvm::createGVNPass());
    FuncPassMgr.add(llvm::createCFGSimplificationPass());
    FuncPassMgr.add(llvm::createPromoteMemoryToRegisterPass());
    FuncPassMgr.doInitialization();
}

LlvmEmulator::~LlvmEmulator(void)
{
}

bool LlvmEmulator::Execute(Expression const& rExpr)
{
  LlvmExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt);
  auto pCurExpr = rExpr.Visit(&Visitor);

  if (pCurExpr == nullptr)
    return false;

  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc  = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
  TestHook(Address(CurPc), Emulator::HookOnExecute);
  delete pCurExpr;
  return true;
}

bool LlvmEmulator::Execute(Expression::List const& rExprList)
{
  return false;
}

LlvmEmulator::LlvmExpressionVisitor::LlvmExpressionVisitor(HookAddressHashMap const& Hooks, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt)
  : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_pVarCtxt(pVarCtxt)
{
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitBind(Expression::List const& rExprList)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitWhileCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitConstant(u32 Type, u64 Value)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitVariable(u32 SizeInBit, std::string const& rName)
{
  return nullptr; // TODO
}