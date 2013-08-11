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
  Log::Write("emul_llvm") << "Executing: " << rExpr.ToString() << LogEnd;
  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc  = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);

  auto itBscBlk = m_BasicBlockCache.find(CurPc);
  if (itBscBlk == std::end(m_BasicBlockCache))
  {
    LlvmExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, m_Builder);
    auto pCurExpr = rExpr.Visit(&Visitor);

    delete pCurExpr;
  }

  TestHook(Address(CurPc), Emulator::HookOnExecute);
  return true;
}

bool LlvmEmulator::Execute(Expression::List const& rExprList)
{
  std::for_each(std::begin(rExprList), std::end(rExprList), [&](Expression const* pExpr)
  {
    Execute(*pExpr);
  });
  return false;
}

LlvmEmulator::LlvmExpressionVisitor::LlvmExpressionVisitor(HookAddressHashMap const& Hooks, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, llvm::IRBuilder<>& rBuilder)
  : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_pVarCtxt(pVarCtxt)
  , m_rBuilder(rBuilder), m_pBscBlk(llvm::BasicBlock::Create(llvm::getGlobalContext()))
{
  m_rBuilder.SetInsertPoint(m_pBscBlk);
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitBind(Expression::List const& rExprList)
{
  std::for_each(std::begin(rExprList), std::end(rExprList), [&](Expression const* pExpr)
  {
    pExpr->Visit(this);
  });
  return nullptr;
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
  pLeftExpr->Visit(this);
  pRightExpr->Visit(this);

  llvm::Value* pLeftValue = nullptr;
  llvm::Value* pRightValue = nullptr;

  if (!m_ValueStack.empty())
  {
    pRightValue = m_ValueStack.top();
    m_ValueStack.pop();
  }

  if (!m_ValueStack.empty())
  {
    pLeftValue = m_ValueStack.top();
    m_ValueStack.pop();
  }

#define LLVM_OP_CASE(op_type)\
  case OperationExpression::Op##op_type: m_rBuilder.Create##op_type(pLeftValue, pRightValue);

  switch (Type)
  {
    LLVM_OP_CASE(Add ); break;
    LLVM_OP_CASE(Sub ); break;
    LLVM_OP_CASE(Mul ); break;
    LLVM_OP_CASE(UDiv); break;
    LLVM_OP_CASE(SDiv); break;
    LLVM_OP_CASE(And ); break;
    LLVM_OP_CASE(Or  ); break;
    LLVM_OP_CASE(Xor ); break;

  default:
    break;
  }

#undef LLVM_OP_CASE

  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitConstant(u32 Type, u64 Value)
{
  auto pValue = MakeInteger(Type, Value);
  m_ValueStack.push(pValue);
  return nullptr;
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  u32 IdSize = pCpuInfo->GetSizeOfRegisterInBit(Id);
  if (IdSize == 0x0)
    return nullptr;

  void* pIdAddr = m_pCpuCtxt->GetRegisterAddress(Id);
  if (pIdAddr == nullptr)
    return nullptr;

  auto pIdValue = MakePointer(IdSize, pIdAddr);
  if (pIdValue == nullptr)
    return nullptr;

  m_ValueStack.push(pIdValue);

  return nullptr;
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitVariable(u32 SizeInBit, std::string const& rName)
{
  return nullptr; // TODO
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::MakeInteger(u32 Bits, u64 Value) const
{
  return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(Bits, Value));
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::MakePointer(u32 Bits, void* pPointer) const
{
  //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
  llvm::Constant* pConstInt = llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), reinterpret_cast<u64>(pPointer));
  return llvm::ConstantExpr::getIntToPtr(pConstInt, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));
}