#include "llvm_emulator.hpp"

#include "medusa/log.hpp"

// To avoid unresolved external symbol LLVMLinkInMCJIT
//#include <llvm/ExecutionEngine/MCJIT.h>

#include <llvm/ExecutionEngine/JIT.h>

MEDUSA_NAMESPACE_USE

llvm::Module*           LlvmEmulator::sm_pModule          = nullptr;
llvm::ExecutionEngine*  LlvmEmulator::sm_pExecutionEngine = nullptr;
llvm::DataLayout*       LlvmEmulator::sm_pDataLayout      = nullptr;

static void* GetMemory(u8* pCpuCtxtObj, u8* pMemCtxtObj, TBase Base, TOffset Offset, u32 AccessSizeInBit)
{
  auto pCpuCtxt = reinterpret_cast<CpuContext*>(pCpuCtxtObj);
  auto pMemCtxt = reinterpret_cast<MemoryContext*>(pMemCtxtObj);
  void* pMemory;

  u64 LinAddr;
  if (pCpuCtxt->Translate(Address(Base, Offset), LinAddr) == false)
    LinAddr = Offset; // FIXME later

  if (pMemCtxt->FindMemory(LinAddr, pMemory, AccessSizeInBit) == false)
  {
    Log::Write("emul_llvm") << "Invalid memory access: linear address: " << LinAddr << LogEnd;
    Log::Write("emul_llvm") << pMemCtxt->ToString() << LogEnd;
    return nullptr;
  }
  return pMemory;
}

static llvm::Function* s_pGetMemoryFunc = nullptr;

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
  : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt, nullptr)
  , m_Builder(llvm::getGlobalContext())
{
  m_pVarCtxt = new LlvmVariableContext(m_Builder);
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

bool LlvmEmulator::Execute(Address const& rAddress, Expression const& rExpr)
{
  assert(0 && "Not implemented");
  return false;
}

bool LlvmEmulator::Execute(Address const& rAddress, Expression::List const& rExprList)
{
  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc  = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);

  static llvm::FunctionType* s_pExecFuncType = nullptr;
  if (s_pExecFuncType == nullptr)
  {
    auto pBytePtrType = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
    std::vector<llvm::Type*> Params;
    Params.push_back(pBytePtrType);
    Params.push_back(pBytePtrType);
    Params.push_back(pBytePtrType);
    s_pExecFuncType = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), Params, false);
  }

  u64 LinAddr;
  if (m_pCpuCtxt->Translate(rAddress, LinAddr) == false)
    return nullptr;

  llvm::Function* pExecFunc = m_FunctionCache[LinAddr];
  if (pExecFunc == nullptr)
  {
    pExecFunc = llvm::Function::Create(s_pExecFuncType, llvm::GlobalValue::ExternalLinkage, std::string("fn_") + rAddress.ToString(), sm_pModule);

    auto itParam          = pExecFunc->arg_begin();
    auto pCpuCtxtParam    = itParam++;
    auto pCpuCtxtObjParam = itParam++;
    auto pMemCtxtObjParam = itParam;

    auto pBscBlk = llvm::BasicBlock::Create(llvm::getGlobalContext(), "bb", pExecFunc);
    m_Builder.SetInsertPoint(pBscBlk);
    LlvmExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, m_Builder, pCpuCtxtParam, pCpuCtxtObjParam, pMemCtxtObjParam);

    for (auto itExpr = std::begin(rExprList); itExpr != std::end(rExprList); ++itExpr)
    {
      Log::Write("emul_llvm") << "Compiling: " << (*itExpr)->ToString() << LogEnd;
      auto itBscBlk = m_BasicBlockCache.find(CurPc);
      if (itBscBlk == std::end(m_BasicBlockCache))
      {
        auto pCurExpr = (*itExpr)->Visit(&Visitor);
        delete pCurExpr;
      }
    }

    m_Builder.CreateRetVoid();
    m_FunctionCache[LinAddr] = pExecFunc;
  }

  pExecFunc->dump();
  auto pCode = reinterpret_cast<BasicBlockCode>(sm_pExecutionEngine->getPointerToFunction(pExecFunc));
  pCode(reinterpret_cast<u8*>(m_pCpuCtxt->GetContextAddress()), reinterpret_cast<u8*>(m_pCpuCtxt), reinterpret_cast<u8*>(m_pMemCtxt));

  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
  TestHook(Address(CurPc), Emulator::HookOnExecute);

  return true;
}

LlvmEmulator::LlvmVariableContext::LlvmVariableContext(llvm::IRBuilder<>& rBuilder) : m_rBuilder(rBuilder)
{
}

bool LlvmEmulator::LlvmVariableContext::ReadVariable(std::string const& rVariableName, u64& rValue) const
{
  return false;
}

bool LlvmEmulator::LlvmVariableContext::WriteVariable(std::string const& rVariableName, u64 Value, bool SignExtend)
{
  return false;
}

bool LlvmEmulator::LlvmVariableContext::AllocateVariable(u32 Type, std::string const& rVariableName)
{
  FreeVariable(rVariableName);

  auto pAlloca = m_rBuilder.CreateAlloca(llvm::Type::getIntNTy(llvm::getGlobalContext(), Type), 0, rVariableName);
  auto pVarVal = m_rBuilder.CreateBitCast(pAlloca, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), Type));

  m_Variables[rVariableName] = VariableInformation(Type, pVarVal);
  return true;
}

std::string LlvmEmulator::LlvmVariableContext::ToString(void) const
{
  return "";
}

LlvmEmulator::LlvmExpressionVisitor::LlvmExpressionVisitor(
  HookAddressHashMap const& Hooks
  , CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt
  , llvm::IRBuilder<>& rBuilder, llvm::Value* pCpuCtxtParam, llvm::Value* pCpuCtxtObjParam, llvm::Value* pMemCtxtObjParam)
  : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_pVarCtxt(pVarCtxt)
  , m_rBuilder(rBuilder), m_pCpuCtxtParam(pCpuCtxtParam), m_pCpuCtxtObjParam(pCpuCtxtObjParam), m_pMemCtxtObjParam(pMemCtxtObjParam)
{
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitBind(Expression::List const& rExprList)
{
  std::for_each(std::begin(rExprList), std::end(rExprList), [&](Expression const* pExpr)
  { pExpr->Visit(this); });
  return nullptr;
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr)
{
  llvm::Value* pCondVal = nullptr,* pRefVal = nullptr,* pTestVal = nullptr;

  pRefExpr->Visit(this);
  pTestExpr->Visit(this);

  if (m_ValueStack.size() < 2)
  {
    assert(0 && "Unable to JIT condition expression");
    return nullptr;
  }

  pTestVal = std::get<1>(m_ValueStack.top());
  m_ValueStack.pop();
  pRefVal = std::get<1>(m_ValueStack.top());
  m_ValueStack.pop();

  switch (Type)
  {
  case ConditionExpression::CondEq:  pCondVal = m_rBuilder.CreateICmpEQ(pRefVal, pTestVal); break;
  case ConditionExpression::CondNe:  pCondVal = m_rBuilder.CreateICmpNE(pRefVal, pTestVal); break;
  case ConditionExpression::CondSge: pCondVal = m_rBuilder.CreateICmpSGE(pRefVal, pTestVal); break;
  case ConditionExpression::CondSgt: pCondVal = m_rBuilder.CreateICmpSGT(pRefVal, pTestVal); break;
  case ConditionExpression::CondSle: pCondVal = m_rBuilder.CreateICmpSLE(pRefVal, pTestVal); break;
  case ConditionExpression::CondSlt: pCondVal = m_rBuilder.CreateICmpSLT(pRefVal, pTestVal); break;
  case ConditionExpression::CondUge: pCondVal = m_rBuilder.CreateICmpUGE(pRefVal, pTestVal); break;
  case ConditionExpression::CondUgt: pCondVal = m_rBuilder.CreateICmpUGT(pRefVal, pTestVal); break;
  case ConditionExpression::CondUle: pCondVal = m_rBuilder.CreateICmpULE(pRefVal, pTestVal); break;
  case ConditionExpression::CondUlt: pCondVal = m_rBuilder.CreateICmpULT(pRefVal, pTestVal); break;

  case ConditionExpression::CondUnk:
  default:
    return nullptr;
  }

  m_ValueStack.push(std::make_tuple(nullptr, pCondVal));

  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)
{
  VisitCondition(Type, pRefExpr, pTestExpr);
  if (m_ValueStack.empty())
  {
    assert(0 && "Unable to JIT condition expression");
    return nullptr;
  }

  auto pCondVal = std::get<1>(m_ValueStack.top());
  m_ValueStack.pop();

  auto pFunc = m_rBuilder.GetInsertBlock()->getParent();
  auto pThenBasicBlock   = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", pFunc);
  auto pMergedBasicBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merged", pFunc);
  auto pIfCondVal        = m_rBuilder.CreateCondBr(pCondVal, pThenBasicBlock, pMergedBasicBlock);

  m_rBuilder.SetInsertPoint(pThenBasicBlock);
  LlvmExpressionVisitor ThenVisitor(m_rHooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, m_rBuilder, m_pCpuCtxtParam, m_pCpuCtxtObjParam, m_pMemCtxtObjParam);
  pThenExpr->Visit(&ThenVisitor);
  ThenVisitor.ClearValues();
  m_rBuilder.CreateBr(pMergedBasicBlock);

  m_rBuilder.SetInsertPoint(pMergedBasicBlock);

  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr)
{
  VisitCondition(Type, pRefExpr, pTestExpr);
  if (m_ValueStack.empty())
  {
    assert(0 && "Unable to JIT condition expression");
    return nullptr;
  }

  auto pCondVal = std::get<1>(m_ValueStack.top());
  m_ValueStack.pop();

  auto pFunc = m_rBuilder.GetInsertBlock()->getParent();
  auto pMergedBasicBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merged", pFunc);
  auto pThenBasicBlock   = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then",   pFunc);
  auto pElseBasicBlock   = llvm::BasicBlock::Create(llvm::getGlobalContext(), "else",   pFunc);
  auto pIfCondVal        = m_rBuilder.CreateCondBr(pCondVal, pThenBasicBlock, pElseBasicBlock);

  m_rBuilder.SetInsertPoint(pThenBasicBlock);
  LlvmExpressionVisitor ThenVisitor(m_rHooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, m_rBuilder, m_pCpuCtxtParam, m_pCpuCtxtObjParam, m_pMemCtxtObjParam);
  pThenExpr->Visit(&ThenVisitor);
  m_rBuilder.CreateBr(pMergedBasicBlock);

  m_rBuilder.SetInsertPoint(pElseBasicBlock);
  LlvmExpressionVisitor ElseVisitor(m_rHooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, m_rBuilder, m_pCpuCtxtParam, m_pCpuCtxtObjParam, m_pMemCtxtObjParam);
  pElseExpr->Visit(&ElseVisitor);
  m_rBuilder.CreateBr(pMergedBasicBlock);

  m_rBuilder.SetInsertPoint(pMergedBasicBlock);

  m_ValueStack.push(std::make_tuple(nullptr, pIfCondVal));

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

  assert(m_ValueStack.size() >= 2 && "Not enough value for operation");

  pRightPointer = std::get<0>(m_ValueStack.top());
  pRightValue = std::get<1>(m_ValueStack.top());
  m_ValueStack.pop();

  pLeftPointer = std::get<0>(m_ValueStack.top());
  pLeftValue = std::get<1>(m_ValueStack.top());
  m_ValueStack.pop();

  llvm::Value* pResult = nullptr;

#define LLVM_OP_CASE(op_type)\
  case OperationExpression::Op##op_type:\
  if (pLeftValue == nullptr || pRightValue == nullptr)\
  {\
  assert(0 && "Error while JIT operation " #op_type);\
  break;\
  }\
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
    if (pLeftPointer == nullptr || pRightValue == nullptr)
    {
      assert(0 && "Error while JIT affectation");
      break;
    }
    pResult = m_rBuilder.CreateStore(pRightValue, pLeftPointer, true);
    break;

  default:
    break;
  }

  if (pResult)
  {
    m_ValueStack.push(std::make_tuple(nullptr, pResult));
  }

#undef LLVM_OP_CASE

  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitConstant(u32 Type, u64 Value)
{
  auto pValue = MakeInteger(Type, Value);
  assert(pValue && "Invalid integer");
  m_ValueStack.push(std::make_tuple(nullptr, pValue));
  return nullptr;
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  u32 IdSize = pCpuInfo->GetSizeOfRegisterInBit(Id);
  if (IdSize == 0x0)
  {
    assert(0 && "Invalid identifier");
    return nullptr;
  }

  if (IdSize == 1)
  {
    std::string IdName = m_pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(Id);
    //if (m_pVarCtxt->GetVariable(IdName) == nullptr)
    VisitVariable(1, IdName);
    VisitVariable(0, IdName);
  }

  u16 CtxtOff = m_pCpuCtxt->GetRegisterOffset(Id);
  if (CtxtOff == -1)
  {
    assert(0 && "Invalid register offset");
    return nullptr;
  }

  auto pIdPtr = MakePointer(IdSize, m_pCpuCtxtParam, CtxtOff);
  if (pIdPtr == nullptr)
  {
    assert(0 && "Unable to make pointer to register");
    return nullptr;
  }

  auto pIdValue = m_rBuilder.CreateLoad(pIdPtr, false);
  assert(pIdValue && "Invalid identifier value");

  m_ValueStack.push(std::make_tuple(pIdPtr, pIdValue));

  return nullptr;
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  pOffsetExpr->Visit(this);
  if (m_ValueStack.empty())
  {
    assert(0 && "Unable to JIT offset calculation");
    return nullptr;
  }
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

  assert(pBaseVal && "Invalid base value");

  auto pAccessSizeInBitVal = MakeInteger(32, AccessSizeInBit);
  auto pCallVal = m_rBuilder.CreateCall5(s_pGetMemoryFunc, m_pCpuCtxtObjParam, m_pMemCtxtObjParam, pBaseVal, pOffVal, pAccessSizeInBitVal);
  auto pPtrVal = m_rBuilder.CreateBitCast(pCallVal, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), AccessSizeInBit));
  auto pVal = m_rBuilder.CreateLoad(pPtrVal, false);
  m_ValueStack.push(std::make_tuple(pPtrVal, pVal));

  return nullptr; // TODO
}

Expression* LlvmEmulator::LlvmExpressionVisitor::VisitVariable(u32 SizeInBit, std::string const& rName)
{
  if (SizeInBit)
  {
    m_pVarCtxt->AllocateVariable(SizeInBit, rName);
    return nullptr;
  }

  auto pVarPtrVal = reinterpret_cast<llvm::Value*>(m_pVarCtxt->GetVariable(rName));
  if (pVarPtrVal == nullptr)
  {
    assert(0 && "Unknown variable");
    return nullptr;
  }

  auto pVarVal = m_rBuilder.CreateLoad(pVarPtrVal);

  m_ValueStack.push(std::make_tuple(pVarPtrVal, pVarVal));

  return nullptr; // TODO
}

void LlvmEmulator::LlvmExpressionVisitor::ClearValues(void)
{
  while (!m_ValueStack.empty())
    m_ValueStack.pop();
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