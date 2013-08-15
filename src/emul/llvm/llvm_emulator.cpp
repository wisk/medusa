#include "llvm_emulator.hpp"

#include "medusa/log.hpp"

// To avoid unresolved external symbol LLVMLinkInMCJIT
//#include <llvm/ExecutionEngine/MCJIT.h>

#include <llvm/ExecutionEngine/JIT.h>

MEDUSA_NAMESPACE_USE

  llvm::Module*           LlvmEmulator::sm_pModule = nullptr;
llvm::ExecutionEngine*  LlvmEmulator::sm_pExecutionEngine = nullptr;
llvm::DataLayout*       LlvmEmulator::sm_pDataLayout = nullptr;

static void* GetMemory(u8* pMemCtxtObj, TBase Base, TOffset Offset, u32 AccessSizeInBit)
{
  auto pMemCtxt = reinterpret_cast<MemoryContext*>(pMemCtxtObj);
  void* pMemory;
  if (pMemCtxt->FindMemory(Base, Offset, pMemory, AccessSizeInBit) == false)
    return nullptr;
  return pMemory;
}

static llvm::Function* s_pGetMemoryFunc = nullptr;

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
    sm_pExecutionEngine = llvm::EngineBuilder(sm_pModule).setUseMCJIT(false).setErrorStr(&ErrStr).create();
  if (sm_pExecutionEngine == nullptr)
    Log::Write("emul_llvm") << "Error: " << ErrStr << LogEnd;
  if (sm_pDataLayout == nullptr)
    sm_pDataLayout = new llvm::DataLayout(sm_pModule);


  if (s_pGetMemoryFunc == nullptr)
  {
    auto& rCtxt = llvm::getGlobalContext();
    std::vector<llvm::Type*> Params;
    Params.push_back(llvm::Type::getInt8PtrTy(rCtxt));
    Params.push_back(llvm::Type::getInt16Ty(rCtxt));
    Params.push_back(llvm::Type::getInt64Ty(rCtxt));
    Params.push_back(llvm::Type::getInt32Ty(rCtxt));
    auto pGetMemoryFuncType = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(rCtxt), Params, false);
    s_pGetMemoryFunc = llvm::Function::Create(pGetMemoryFuncType, llvm::GlobalValue::ExternalLinkage, "GetMemory", sm_pModule);

    sm_pExecutionEngine->addGlobalMapping(s_pGetMemoryFunc, GetMemory);
  }

  llvm::FunctionPassManager FuncPassMgr(sm_pModule);
  FuncPassMgr.add(new llvm::DataLayout(*sm_pExecutionEngine->getDataLayout()));
  FuncPassMgr.add(llvm::createBasicAliasAnalysisPass());
  FuncPassMgr.add(llvm::createInstructionCombiningPass());
  FuncPassMgr.add(llvm::createReassociatePass());
  FuncPassMgr.add(llvm::createGVNPass());
  FuncPassMgr.add(llvm::createCFGSimplificationPass());
  FuncPassMgr.add(llvm::createPromoteMemoryToRegisterPass());
  FuncPassMgr.add(llvm::createDeadCodeEliminationPass());
  FuncPassMgr.doInitialization();
}

LlvmEmulator::~LlvmEmulator(void)
{
}

bool LlvmEmulator::Execute(Expression const& rExpr)
{
  return false;
}

bool LlvmEmulator::Execute(Expression::List const& rExprList)
{
  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc  = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);

  auto pBytePtrType = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
  std::vector<llvm::Type*> Params;
  Params.push_back(pBytePtrType);
  Params.push_back(pBytePtrType);

  auto pExecFuncTy = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), Params, false);
  auto pExecFunc = llvm::Function::Create(pExecFuncTy, llvm::GlobalValue::ExternalLinkage, "exec", sm_pModule);

  auto itParam = pExecFunc->arg_begin();
  auto pCpuCtxtParam = itParam++;
  auto pMemCtxtParam = itParam;

  auto pBscBlk = llvm::BasicBlock::Create(llvm::getGlobalContext(), "bb", pExecFunc);
  m_Builder.SetInsertPoint(pBscBlk);
  LlvmExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, m_Builder, pCpuCtxtParam, pMemCtxtParam);

  for (auto itExpr = std::begin(rExprList); itExpr != std::end(rExprList); ++itExpr)
  {
    Log::Write("emul_llvm") << "Executing: " << (*itExpr)->ToString() << LogEnd;
    auto itBscBlk = m_BasicBlockCache.find(CurPc);
    if (itBscBlk == std::end(m_BasicBlockCache))
    {
      auto pCurExpr = (*itExpr)->Visit(&Visitor);

      delete pCurExpr;
    }
  }

  m_Builder.CreateRetVoid();

  auto pCode = reinterpret_cast<BasicBlockCode>(sm_pExecutionEngine->getPointerToFunction(pExecFunc));
  pExecFunc->dump();

  pCode(reinterpret_cast<u8*>(m_pCpuCtxt->GetContextAddress()), reinterpret_cast<u8*>(m_pMemCtxt));


  TestHook(Address(CurPc), Emulator::HookOnExecute);

  return true;
}

LlvmEmulator::LlvmExpressionVisitor::LlvmExpressionVisitor(HookAddressHashMap const& Hooks, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, llvm::IRBuilder<>& rBuilder, llvm::Value* pCpuCtxtParam, llvm::Value* pMemCtxtParam)
  : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_pVarCtxt(pVarCtxt)
  , m_rBuilder(rBuilder)
  , m_pCpuCtxtParam(pCpuCtxtParam), m_pMemCtxtParam(pMemCtxtParam)
{
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

  llvm::Value* pLeftPointer  = nullptr;
  llvm::Value* pRightPointer = nullptr;
  llvm::Value* pLeftValue    = nullptr;
  llvm::Value* pRightValue   = nullptr;

  if (!m_ValueStack.empty())
  {
    pRightPointer = std::get<0>(m_ValueStack.top());
    pRightValue = std::get<1>(m_ValueStack.top());

    m_ValueStack.pop();
  }

  if (!m_ValueStack.empty())
  {
    pLeftPointer = std::get<0>(m_ValueStack.top());
    pLeftValue = std::get<1>(m_ValueStack.top());

    m_ValueStack.pop();
  }

  llvm::Value* pResult = nullptr;

#define LLVM_OP_CASE(op_type)\
  case OperationExpression::Op##op_type:\
  if (pLeftValue == nullptr || pRightValue == nullptr)\
  break;\
  pResult = m_rBuilder.Create##op_type(pLeftValue, pRightValue);

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

  case OperationExpression::OpAff:
    pRightValue->dump();
    pLeftPointer->dump();
    if (pRightValue == nullptr || pLeftPointer == nullptr)
      break;
    m_rBuilder.CreateStore(pRightValue, pLeftPointer, true);
    break;

  default:
    break;
  }

  if (pResult)
    m_ValueStack.push(std::make_tuple(nullptr, pResult));

#undef LLVM_OP_CASE

  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitConstant(u32 Type, u64 Value)
{
  auto pValue = MakeInteger(Type, Value);
  m_ValueStack.push(std::make_tuple(nullptr, pValue));
  return nullptr;
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  u32 IdSize = pCpuInfo->GetSizeOfRegisterInBit(Id);
  if (IdSize == 0x0)
    return nullptr;

  u16 CtxtOff = m_pCpuCtxt->GetRegisterOffset(Id);
  if (CtxtOff == -1)
    return nullptr;

  auto pIdPtr = MakePointer(IdSize, m_pCpuCtxtParam, CtxtOff);
  if (pIdPtr == nullptr)
    return nullptr;

  auto pIdValue = m_rBuilder.CreateLoad(pIdPtr, false);

  m_ValueStack.push(std::make_tuple(pIdPtr, pIdValue));

  return nullptr;
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  pOffsetExpr->Visit(this);
  if (m_ValueStack.empty())
    return nullptr;
  auto pOffVal = std::get<1>(m_ValueStack.top());
  pOffVal = m_rBuilder.CreateZExt(pOffVal, llvm::Type::getInt64Ty(llvm::getGlobalContext()));
  m_ValueStack.pop();

  llvm::Value* pBaseVal = nullptr;
  if (pBaseExpr == nullptr)
  {
    pBaseVal = MakeInteger(16, 0x0);
  }
  else
  {
    pBaseExpr->Visit(this);
    if (m_ValueStack.empty())
    {
      pBaseVal = MakeInteger(16, 0x0);
    }
    else
    {
      pBaseVal = std::get<1>(m_ValueStack.top());
      m_ValueStack.pop();
    }
  }

  auto pAccessSizeInBitVal = MakeInteger(32, AccessSizeInBit);
  auto pCallVal = m_rBuilder.CreateCall4(s_pGetMemoryFunc, m_pMemCtxtParam, pBaseVal, pOffVal, pAccessSizeInBitVal);
  auto pPtrVal = m_rBuilder.CreateBitCast(pCallVal, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), AccessSizeInBit));
  auto pVal = m_rBuilder.CreateLoad(pPtrVal, false);
  m_ValueStack.push(std::make_tuple(pPtrVal, pVal));

  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitVariable(u32 SizeInBit, std::string const& rName)
{
  // HACK
  if (SizeInBit == 0)
    SizeInBit = 32;
  llvm::Value* pVar = nullptr;
  auto itVar = m_Variables.find(rName);
  if (itVar != std::end(m_Variables))
  {
    pVar = itVar->second;
  }
  else
  {
    auto pAlloca = m_rBuilder.CreateAlloca(llvm::Type::getIntNTy(llvm::getGlobalContext(), SizeInBit));
    pVar = m_rBuilder.CreateBitCast(pAlloca, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), SizeInBit));
  }

  if (pVar)
    m_ValueStack.push(std::make_tuple(pVar, nullptr));

  return nullptr; // TODO
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::MakeInteger(u32 Bits, u64 Value) const
{
  return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(Bits, Value));
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::MakePointer(u32 Bits, void* pPointer, s32 Offset) const
{
  //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
  auto pConstInt = llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), reinterpret_cast<u64>(pPointer));
  auto pPtr = llvm::ConstantExpr::getIntToPtr(pConstInt, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));

  if (Offset == 0x0)
    return pPtr;

  return m_rBuilder.CreateGEP(pPtr, MakeInteger(32, Offset));
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::MakePointer(u32 Bits, llvm::Value* pPointerValue, s32 Offset) const
{
  if (Offset != 0x0)
  {
    //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
    pPointerValue = m_rBuilder.CreateGEP(pPointerValue, MakeInteger(32, Offset));
  }

  return m_rBuilder.CreateBitCast(pPointerValue, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));
}