#include "llvm_emulator.hpp"

#include "medusa/log.hpp"

#ifdef WIN32
# include "llvm/Support/Host.h"
#endif

#include <llvm/ExecutionEngine/MCJIT.h>

#include <llvm/Object/ObjectFile.h> // NOTE: Needed to avoid incomplete type llvm::object::ObjectFile

MEDUSA_NAMESPACE_USE

llvm::Module*           LlvmEmulator::sm_pModule          = nullptr;
llvm::ExecutionEngine*  LlvmEmulator::sm_pExecutionEngine = nullptr;
llvm::DataLayout*       LlvmEmulator::sm_pDataLayout      = nullptr;

// This function allows the JIT'ed code to read from a register
extern "C" EMUL_LLVM_EXPORT bool JitReadRegister(CpuContext* pCpuCtxt, u32 Reg, void* pVal, u32 BitSize)
{
  if (!pCpuCtxt->ReadRegister(Reg, pVal, BitSize))
  {
    Log::Write("emul_llvm").Level(LogError) << "failed to read register " << pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(Reg) << LogEnd;
    return false;
  }
  return true;
}

static llvm::Function* s_pReadRegisterFunc;

// This function allows the JIT'ed code to write into a register
extern "C" EMUL_LLVM_EXPORT bool JitWriteRegister(CpuContext* pCpuCtxt, u32 Reg, void const* pVal, u32 BitSize)
{
  if (!pCpuCtxt->WriteRegister(Reg, pVal, BitSize))
  {
    Log::Write("emul_llvm").Level(LogError) << "failed to write register " << pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(Reg) << LogEnd;
    return false;
  }
  return true;
}

static llvm::Function* s_pWriteRegisterFunc;

// This function allows the JIT'ed code to access to memory
extern "C" EMUL_LLVM_EXPORT void* JitGetMemory(u8* pCpuCtxtObj, u8* pMemCtxtObj, TBase Base, TOffset Offset, u32 BitSize)
{
  auto pCpuCtxt = reinterpret_cast<CpuContext*>(pCpuCtxtObj);
  auto pMemCtxt = reinterpret_cast<MemoryContext*>(pMemCtxtObj);
  void* pMemory;

  Address CurAddr(Base, Offset);
  u64 LinAddr;
  if (!pCpuCtxt->Translate(CurAddr, LinAddr))
  {
    Log::Write("emul_llvm").Level(LogWarning) << "unable to translate address " << CurAddr << LogEnd;
    return nullptr;
  }

  if (!pMemCtxt->FindMemory(LinAddr, pMemory, BitSize))
  {
    Log::Write("emul_llvm").Level(LogWarning) << "invalid memory access: linear address: " << LinAddr << LogEnd;
    return nullptr;
  }

  return pMemory;
}

static llvm::Function* s_pGetMemoryFunc = nullptr;

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
  : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
  , m_Builder(llvm::getGlobalContext())
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  auto& rCtxt = llvm::getGlobalContext();
  std::string ErrStr;

  if (sm_pModule == nullptr)
  {
    sm_pModule = new llvm::Module("medusa-emulator-llvm", rCtxt);

  // c'mon... https://the-ravi-programming-language.readthedocs.org/en/latest/llvm-notes.html
#ifdef _WIN32
    auto Triple = llvm::sys::getProcessTriple();
    sm_pModule->setTargetTriple(Triple + "-elf");
#endif
  }
  if (sm_pExecutionEngine == nullptr)
  {
    sm_pExecutionEngine = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(sm_pModule))
      .setErrorStr(&ErrStr)
      .create();

    static EventListener s_EvtLst;
    sm_pExecutionEngine->RegisterJITEventListener(&s_EvtLst);

    //auto pChkStkType = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), false);
    //llvm::IRBuilder<> ChkStkBld(llvm::getGlobalContext());
    //auto pChkStk = llvm::Function::Create(pChkStkType, llvm::GlobalValue::ExternalLinkage, "__chkstk", sm_pModule);
    //sm_pExecutionEngine->addGlobalMapping(pChkStk, (void*)0x1337);
  }
  if (sm_pExecutionEngine == nullptr)
    Log::Write("emul_llvm") << "Error: " << ErrStr << LogEnd;
  if (sm_pDataLayout == nullptr)
    sm_pDataLayout = new llvm::DataLayout(sm_pModule);

  // Initialize ReadRegister function type
  if (s_pReadRegisterFunc == nullptr)
  {
    std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt),   // pCpuCtxt
      llvm::Type::getInt32Ty(rCtxt),    // Reg
      llvm::Type::getInt8PtrTy(rCtxt), // pVal
      llvm::Type::getInt32Ty(rCtxt)   // BitSize
    };
    auto pReadRegisterFuncType = llvm::FunctionType::get(llvm::Type::getInt1Ty(rCtxt), Params, false);
    s_pReadRegisterFunc = llvm::Function::Create(pReadRegisterFuncType, llvm::GlobalValue::ExternalLinkage, "JitReadRegister", sm_pModule);
    sm_pExecutionEngine->addGlobalMapping(s_pReadRegisterFunc, (void*)JitReadRegister);
  }

  // Initialize WriteRegister function type
  if (s_pWriteRegisterFunc == nullptr)
  {
    std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt),   // pCpuCtxt
      llvm::Type::getInt32Ty(rCtxt),    // Reg
      llvm::Type::getInt8PtrTy(rCtxt), // pVal
      llvm::Type::getInt32Ty(rCtxt)   // BitSize
    };
    auto pWriteRegisterFuncType = llvm::FunctionType::get(llvm::Type::getInt8Ty(rCtxt), Params, false);
    s_pWriteRegisterFunc = llvm::Function::Create(pWriteRegisterFuncType, llvm::GlobalValue::ExternalLinkage, "JitWriteRegister", sm_pModule);
    sm_pExecutionEngine->addGlobalMapping(s_pWriteRegisterFunc, (void*)JitWriteRegister);
  }

  // Initialize GetMemory function type
  if (s_pGetMemoryFunc == nullptr)
  {
    std::vector<llvm::Type*> Params;
    Params.push_back(llvm::Type::getInt8PtrTy(rCtxt));
    Params.push_back(llvm::Type::getInt8PtrTy(rCtxt));
    Params.push_back(llvm::Type::getInt16Ty(rCtxt));
    Params.push_back(llvm::Type::getInt64Ty(rCtxt));
    Params.push_back(llvm::Type::getInt32Ty(rCtxt));
    auto pGetMemoryFuncType = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(rCtxt), Params, false);
    s_pGetMemoryFunc = llvm::Function::Create(pGetMemoryFuncType, llvm::GlobalValue::ExternalLinkage, "JitGetMemory", sm_pModule);
    sm_pExecutionEngine->addGlobalMapping(s_pGetMemoryFunc, (void*)JitGetMemory);
  }

  // Add passes to optimize the generated code
  llvm::FunctionPassManager FuncPassMgr(sm_pModule);
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

bool LlvmEmulator::Execute(Address const& rAddress, Expression::LSPType const& rExprList)
{
  // Define method(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
  static llvm::FunctionType* s_pExecFuncType = nullptr;
  if (s_pExecFuncType == nullptr)
  {
    auto pBytePtrType = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
    std::vector<llvm::Type*> Params;
    Params.push_back(pBytePtrType);
    Params.push_back(pBytePtrType);
    s_pExecFuncType = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), Params, false);
  }

  u64 LinAddr;
  if (!m_pCpuCtxt->Translate(rAddress, LinAddr))
    return nullptr;

  // Check if the code to be executed is already JIT'ed
  llvm::Function* pExecFunc = m_FunctionCache[LinAddr];
  if (pExecFunc == nullptr)
  {
    // Create a new LLVM function
    pExecFunc = llvm::Function::Create(s_pExecFuncType, llvm::GlobalValue::InternalLinkage, std::string("fn_") + rAddress.ToString(), sm_pModule);

    // Expose its parameters: CpuCtxt and MemCtxt
    auto itParam = pExecFunc->arg_begin();
    auto pCpuCtxtObjParam = itParam++;
    auto pMemCtxtObjParam = itParam;

    // Insert a new basic block
    auto pBscBlk = llvm::BasicBlock::Create(llvm::getGlobalContext(), "bb", pExecFunc);
    m_Builder.SetInsertPoint(pBscBlk);

    // This visitor will now emit code into the basic block using the LLVM builder
    LlvmExpressionVisitor Visitor(
      sm_pModule,
      m_Hooks,
      m_pCpuCtxt, m_pMemCtxt,
      m_Vars,
      m_Builder,
      pCpuCtxtObjParam, pMemCtxtObjParam);

    // Emit the code for each expression
    for (auto spExpr : rExprList)
    {
      Log::Write("emul_llvm") << "Compiling: " << spExpr->ToString() << LogEnd;
      std::cout << spExpr->ToString() << std::endl;
      if (spExpr->Visit(&Visitor) == nullptr)
      {
        Log::Write("emul_llvm").Level(LogError) << "failed to JIT expression at " << rAddress << LogEnd;
        return false;
      }
    }

    m_Builder.CreateRetVoid();
    m_FunctionCache[LinAddr] = pExecFunc;
  }

  pExecFunc->dump();

  // Call the JIT'ed code
  // NOTE: We must call this method explicitly since it seems getFunctionAddress won't call it
  // -elf windows hack?
  sm_pExecutionEngine->finalizeObject();
  auto pCode = (BasicBlockCode)sm_pExecutionEngine->getFunctionAddress(pExecFunc->getName());
  if (pCode == nullptr)
  {
    Log::Write("emul_llvm").Level(LogError) << "failed to JIT code for function " << pExecFunc->getName().data() << LogEnd;
    return false;
  }
  pCode(reinterpret_cast<u8*>(m_pCpuCtxt), reinterpret_cast<u8*>(m_pMemCtxt));

  return true;
}

LlvmEmulator::EventListener::EventListener(void)
{
}

LlvmEmulator::EventListener::~EventListener(void)
{
}

void LlvmEmulator::EventListener::NotifyObjectEmitted(llvm::object::ObjectFile const& rObj, llvm::RuntimeDyld::LoadedObjectInfo const& rLdObjInfo)
{
  Log::Write("emul_llvm").Level(LogDebug) << "object emitted" << LogEnd;
}

void LlvmEmulator::EventListener::NotifyFreeingObject(llvm::object::ObjectFile const& rObj)
{
  Log::Write("emul_llvm").Level(LogDebug) << "object freed" << LogEnd;
}

LlvmEmulator::LlvmExpressionVisitor::LlvmExpressionVisitor(
  llvm::Module* pMod,
  HookAddressHashMap const& rHooks,
  CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, std::unordered_map<std::string, llvm::Value*>& rVars,
  llvm::IRBuilder<>& rBuilder,
  llvm::Value* pCpuCtxtObjParam, llvm::Value* pMemCtxtObjParam)
  : m_pMod(pMod)
  , m_rHooks(rHooks)
  , m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_rVars(rVars)
  , m_rBuilder(rBuilder)
  , m_NrOfValueToRead(), m_State(Unknown)
  , m_pCpuCtxtObjParam(pCpuCtxtObjParam), m_pMemCtxtObjParam(pMemCtxtObjParam)
{
}

LlvmEmulator::LlvmExpressionVisitor::~LlvmExpressionVisitor(void)
{
  while (!m_ValueStack.empty())
  {
    auto pCurVal = m_ValueStack.top();
    m_ValueStack.pop();
    Log::Write("emul_llvm") << "leaked value: " << pCurVal->getName().str() << LogEnd;
  }
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return spSysExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  return nullptr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  return nullptr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  // _Emit test and ref
  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spIfElseExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spIfElseExpr->GetTestExpression()->Visit(this);
  auto pCondVal = _EmitComparison(spIfElseExpr->GetType());
  m_State = OldState;

  auto pBbOrig = m_rBuilder.GetInsertBlock();
  auto& rCtxt = llvm::getGlobalContext();
  auto pFunc = pBbOrig->getParent();

  auto pBbThen = llvm::BasicBlock::Create(rCtxt, "then", pFunc);
  auto pBbElse = llvm::BasicBlock::Create(rCtxt, "else", pFunc);
  auto pBbMerg = llvm::BasicBlock::Create(rCtxt, "merged", pFunc);

  // Emit the then branch
  auto spThenExpr = spIfElseExpr->GetThenExpression();
  m_rBuilder.SetInsertPoint(pBbThen);
  OldState = m_State;
  m_State = Unknown;
  if (spThenExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbMerg);

  // Emit the else branch
  auto spElseExpr = spIfElseExpr->GetElseExpression();
  m_rBuilder.SetInsertPoint(pBbElse);
  OldState = m_State;
  m_State = Unknown;
  if (spElseExpr != nullptr)
    if (spElseExpr->Visit(this) == nullptr)
      return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbMerg);

  // Emit the condition
  m_rBuilder.SetInsertPoint(pBbOrig);
  m_rBuilder.CreateCondBr(pCondVal, pBbThen, pBbElse);

  m_rBuilder.SetInsertPoint(pBbMerg);
  return spIfElseExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return nullptr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  if (auto spDstVecId = expr_cast<VectorIdentifierExpression>(spAssignExpr->GetDestinationExpression()))
    m_NrOfValueToRead = spDstVecId->GetVector().size();
  else
    m_NrOfValueToRead = 0;

  State OldState = m_State;

  m_State = Read;
  auto spSrc = spAssignExpr->GetSourceExpression()->Visit(this);
  m_State = Write;
  auto spDst = spAssignExpr->GetDestinationExpression()->Visit(this);
  m_State = OldState;

  if (spDst == nullptr || spSrc == nullptr)
    return nullptr;

  return spAssignExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExpr = spUnOpExpr->GetExpression()->Visit(this);

  if (spExpr == nullptr)
    return nullptr;

  if (m_ValueStack.size() < 1)
    return nullptr;
  auto pVal = m_ValueStack.top();
  m_ValueStack.pop();

  llvm::Value* pUnOpVal = nullptr;

  switch (spUnOpExpr->GetOperation())
  {
  default:
    return nullptr;

    case OperationExpression::OpNot:
      pUnOpVal = m_rBuilder.CreateNot(pVal);
      break;

    case OperationExpression::OpNeg:
      pUnOpVal = m_rBuilder.CreateNeg(pVal);
      break;

    case OperationExpression::OpSwap:
      pUnOpVal = _CallIntrinsic(llvm::Intrinsic::bswap, {}, { pVal });
      break;

    case OperationExpression::OpBsf:
      pUnOpVal = _CallIntrinsic(llvm::Intrinsic::cttz, {}, { pVal });
      break;

    case OperationExpression::OpBsr:
      pUnOpVal = _CallIntrinsic(llvm::Intrinsic::ctlz, {}, { pVal });
      break;
  }

  if (pUnOpVal == nullptr)
    return nullptr;
  m_ValueStack.push(pUnOpVal);
  return spUnOpExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  if (spBinOpExpr->GetOperation() == OperationExpression::OpXchg)
  {
    State OldState = m_State;
    m_State = Read;
    auto spReadLeft = spBinOpExpr->GetLeftExpression()->Visit(this);
    auto spReadRight = spBinOpExpr->GetRightExpression()->Visit(this);
    m_State = Write;
    auto spWriteLeft = spBinOpExpr->GetLeftExpression()->Visit(this);
    auto spWriteRight = spBinOpExpr->GetRightExpression()->Visit(this);
    m_State = OldState;
    return spBinOpExpr;
  }

  auto spLeft = spBinOpExpr->GetLeftExpression()->Visit(this);
  auto spRight = spBinOpExpr->GetRightExpression()->Visit(this);

  if (spLeft == nullptr || spRight == nullptr)
    return nullptr;

  if (m_ValueStack.size() < 2)
    return nullptr;

  auto RightVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto LeftVal = m_ValueStack.top();
  m_ValueStack.pop();

  llvm::Value* pBinOpVal = nullptr;

  switch (spBinOpExpr->GetOperation())
  {
  default:
    return nullptr;

  case OperationExpression::OpAnd:
    pBinOpVal = m_rBuilder.CreateAnd(LeftVal, RightVal, "and");
    break;

  case OperationExpression::OpOr:
    pBinOpVal = m_rBuilder.CreateOr(LeftVal, RightVal, "or");
    break;

  case OperationExpression::OpXor:
    pBinOpVal = m_rBuilder.CreateXor(LeftVal, RightVal, "xor");
    break;

  case OperationExpression::OpLls:
    pBinOpVal = m_rBuilder.CreateLShr(LeftVal, RightVal, "lls");
    break;

  case OperationExpression::OpLrs:
    pBinOpVal = m_rBuilder.CreateShl(LeftVal, RightVal, "lrs");
    break;

  case OperationExpression::OpArs:
    pBinOpVal = m_rBuilder.CreateAShr(LeftVal, RightVal, "ars");
    break;

  case OperationExpression::OpRol:
    // TODO(KS):
    break;

  case OperationExpression::OpRor:
    // TODO(KS):
    break;

  case OperationExpression::OpAdd:
    pBinOpVal = m_rBuilder.CreateAdd(LeftVal, RightVal, "add");
    break;

  case OperationExpression::OpSub:
    pBinOpVal = m_rBuilder.CreateSub(LeftVal, RightVal, "sub");
    break;

  case OperationExpression::OpMul:
    pBinOpVal = m_rBuilder.CreateMul(LeftVal, RightVal, "mul");
    break;

  case OperationExpression::OpSDiv:
    pBinOpVal = m_rBuilder.CreateSDiv(LeftVal, RightVal, "sdiv");
    break;

  case OperationExpression::OpUDiv:
    pBinOpVal = m_rBuilder.CreateUDiv(LeftVal, RightVal, "udiv");
    break;

  case OperationExpression::OpSMod:
    pBinOpVal = m_rBuilder.CreateSRem(LeftVal, RightVal, "smod");
    break;

  case OperationExpression::OpUMod:
    pBinOpVal = m_rBuilder.CreateURem(LeftVal, RightVal, "rmod");
    break;

  case OperationExpression::OpSext:
    pBinOpVal = m_rBuilder.CreateSExt(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "sext");
    break;

  case OperationExpression::OpZext:
    pBinOpVal = m_rBuilder.CreateZExt(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "zext");
    break;

  case OperationExpression::OpBcast:
    pBinOpVal = m_rBuilder.CreateTruncOrBitCast(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "bcast");
    break;

  case OperationExpression::OpInsertBits:
    break;

  case OperationExpression::OpExtractBits:
    break;
  }

  if (pBinOpVal == nullptr)
    return nullptr;
  m_ValueStack.push(pBinOpVal);
  return spBinOpExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitConstant(ConstantExpression::SPType spConstExpr)
{
  if (m_State != Read)
  {
    Log::Write("emul_llvm").Level(LogError) << "constant can only be read" << LogEnd;
    return nullptr;
  }
  auto pConstVal = _MakeInteger(spConstExpr->GetConstant());
  if (pConstVal == nullptr)
    return nullptr;
  m_ValueStack.push(pConstVal);
  return spConstExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  switch (m_State)
  {
  case Read:
  {
    auto pRegVal = _EmitReadRegister(spIdExpr->GetId(), *spIdExpr->GetCpuInformation());
    if (pRegVal == nullptr)
      return nullptr;
    m_ValueStack.push(pRegVal);
    break;
  }

  case Write:
  {
    if (m_ValueStack.empty())
      return nullptr;
    auto pRegVal = m_ValueStack.top();
    m_ValueStack.pop();
    if (!_EmitWriteRegister(spIdExpr->GetId(), *spIdExpr->GetCpuInformation(), pRegVal))
      return nullptr;
    break;
  }

  default:
    return nullptr;
  }
  return spIdExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  auto const* pCpuInfo = spVecIdExpr->GetCpuInformation();
  switch (m_State)
  {
  case Read:
  {
    auto VecId = spVecIdExpr->GetVector();
    for (auto Id : VecId)
    {
      auto pRegVal = _EmitReadRegister(Id, *spVecIdExpr->GetCpuInformation());
      if (pRegVal == nullptr)
        return nullptr;
      m_ValueStack.push(pRegVal);
    }
    break;
  }

  case Write:
  {
    auto VecId = spVecIdExpr->GetVector();
    for (auto Id : VecId)
    {
      if (m_ValueStack.empty())
        return nullptr;
      auto pRegVal = m_ValueStack.top();
      m_ValueStack.pop();
      if (!_EmitWriteRegister(Id, *spVecIdExpr->GetCpuInformation(), pRegVal))
        return nullptr;
    }
    break;
  }

  default:
    return nullptr;
  }

  return spVecIdExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
{
  switch (m_State)
  {
  case Read:
  {
    auto pRegVal = _EmitReadRegister(spTrkIdExpr->GetId(), *spTrkIdExpr->GetCpuInformation());
    if (pRegVal == nullptr)
      return nullptr;
    m_ValueStack.push(pRegVal);
    break;
  }

  case Write:
  {
    if (m_ValueStack.empty())
      return nullptr;
    auto pRegVal = m_ValueStack.top();
    m_ValueStack.pop();
    if (!_EmitWriteRegister(spTrkIdExpr->GetId(), *spTrkIdExpr->GetCpuInformation(), pRegVal))
      return nullptr;
    break;
  }

  default:
    return nullptr;
  }
  return spTrkIdExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  switch (m_State)
  {
  case Unknown:
  {
    switch (spVarExpr->GetAction())
    {
    case VariableExpression::Alloc:
      m_rVars[spVarExpr->GetName()] = m_rBuilder.CreateAlloca(_BitSizeToLlvmType(spVarExpr->GetBitSize()), nullptr, spVarExpr->GetName());
      break;

    case VariableExpression::Free:
      m_rVars.erase(spVarExpr->GetName());
      break;

    default:
      Log::Write("emul_llvm").Level(LogError) << "unknown variable action" << LogEnd;
      return nullptr;
    }
    break;
  }

  case Read:
    if (spVarExpr->GetAction() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      m_ValueStack.push(m_rBuilder.CreateLoad(itVar->second, "read_var"));
      break;
    }
    else
    {
      Log::Write("emul_llvm").Level(LogError) << "invalid state for variable reading" << LogEnd;
      return nullptr;
    }

  case Write:
    if (spVarExpr->GetAction() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      if (m_ValueStack.empty())
        return nullptr;
      auto pVal = m_ValueStack.top();
      m_ValueStack.pop();
      m_rBuilder.CreateStore(pVal, itVar->second);
      break;
    }
    else
    {
      Log::Write("emul_llvm").Level(LogError) << "invalid state for variable writing" << LogEnd;
      return nullptr;
    }

  default:
    return nullptr;
  }

  return spVarExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  State OldState = m_State;
  m_State = Read;

  auto spOffsetExpr = spMemExpr->GetOffsetExpression();
  auto spBaseExpr = spMemExpr->GetBaseExpression();

  if (spBaseExpr == nullptr)
    m_ValueStack.push(_MakeInteger(IntType(16, 0x0)));
  else if (spBaseExpr->Visit(this) == nullptr)
    return nullptr;

  if (spOffsetExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;

  if (m_ValueStack.size() < 2)
  {
    Log::Write("emul_llvm").Level(LogError) << "no value for address" << LogEnd;
    return nullptr;
  }

  auto pOffVal = m_rBuilder.CreateZExtOrBitCast(m_ValueStack.top(), _BitSizeToLlvmType(64));
  m_ValueStack.pop();
  auto pBaseVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto AccBitSize = spMemExpr->GetAccessSizeInBit();
  auto pAccBitSizeVal = _MakeInteger(IntType(32, AccBitSize));

  auto pRawMemVal = m_rBuilder.CreateCall5(s_pGetMemoryFunc, m_pCpuCtxtObjParam, m_pMemCtxtObjParam, pBaseVal, pOffVal, pAccBitSizeVal, "get_memory");
  auto pPtrVal = m_rBuilder.CreateBitCast(pRawMemVal, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), AccBitSize));

  switch (m_State)
  {
  default:
    Log::Write("emul_llvm").Level(LogError) << "unknown state for address" << LogEnd;
    return nullptr;

  case Read:
  {
    if (!spMemExpr->IsDereferencable())
    {
      return nullptr;
    }

    if (m_NrOfValueToRead == 0)
    {
      auto pVal = m_rBuilder.CreateLoad(pPtrVal, "read_mem");
      m_ValueStack.push(pVal);
    }
    while (m_NrOfValueToRead != 0)
    {
      auto pVal = m_rBuilder.CreateLoad(pPtrVal, "read_mem");
      m_ValueStack.push(pVal);
      // FIXME(KS):
      //m_rBuilder.CreateAdd(pPtrVal, _MakeInteger(IntType(AccBitSize, AccBitSize / 8)), "inc_ptr");
      --m_NrOfValueToRead;
    }
    break;
  }

  case Write:
  {
    if (m_ValueStack.empty())
    {
      Log::Write("emul_llvm").Level(LogError) << "no value for address writing" << LogEnd;
      return nullptr;
    }

    //  // NOTE: Trying to write an non-deferencable address is like
    //  // changing its offset.
    //  if (!spMemExpr->IsDereferencable())
    //  {
    //    auto spOffsetExpr = spMemExpr->GetOffsetExpression()->Visit(this);
    //    if (spOffsetExpr == nullptr)
    //      return nullptr;
    //    break;
    //  }

    do
    {
      auto pCurVal = m_ValueStack.top();
      m_ValueStack.pop();
      m_rBuilder.CreateStore(pCurVal, pPtrVal);
      // FIXME(KS):
      //m_rBuilder.CreateAdd(pPtrVal, _MakeInteger(IntType(AccBitSize, AccBitSize / 8)), "inc_ptr");
    } while (!m_ValueStack.empty());
    break;
  }
  }

  return spMemExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  return nullptr;
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_MakeInteger(IntType const& rInt) const
{
  // TODO(KS): Handle integer larger than 64-bit
  if (rInt.GetBitSize() > 64)
  {
    Log::Write("emul_llvm").Level(LogError) << "unsupported int size " << rInt.GetBitSize() << LogEnd;
    return nullptr;
  }
  return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(rInt.GetBitSize(), rInt.ConvertTo<u64>()));
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_MakePointer(u32 Bits, void* pPointer, s32 Offset) const
{
  //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
  auto pConstInt = llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), reinterpret_cast<u64>(pPointer));
  auto pPtr = llvm::ConstantExpr::getIntToPtr(pConstInt, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));

  if (Offset == 0x0)
    return pPtr;

  return m_rBuilder.CreateGEP(pPtr, _MakeInteger(IntType(Offset)));
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_MakePointer(u32 Bits, llvm::Value* pPointerValue, s32 Offset) const
{
  if (Offset != 0x0)
  {
    //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
    pPointerValue = m_rBuilder.CreateGEP(pPointerValue, _MakeInteger(IntType(Offset)));
  }

  return m_rBuilder.CreateBitCast(pPointerValue, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));
}

llvm::Type* LlvmEmulator::LlvmExpressionVisitor::_IntTypeToLlvmType(IntType const& rInt) const
{
  return _BitSizeToLlvmType(rInt.GetBitSize());
}

llvm::Type* LlvmEmulator::LlvmExpressionVisitor::_BitSizeToLlvmType(u16 BitSize) const
{
  return llvm::Type::getIntNTy(llvm::getGlobalContext(), BitSize);
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_CallIntrinsic(
  llvm::Intrinsic::ID IntrId,
  std::vector<llvm::Type*> const& rTypes,
  std::vector<llvm::Value*> const& rArgs) const
{
  auto pModule   = m_rBuilder.GetInsertBlock()->getParent()->getParent();
  auto pIntrFunc = llvm::Intrinsic::getDeclaration(pModule, IntrId);
  auto pCallIntr = m_rBuilder.CreateCall(pIntrFunc, rArgs);
  return pCallIntr;
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_EmitComparison(u8 CondOp)
{
  if (m_ValueStack.size() < 2)
  {
    Log::Write("emul_llvm").Level(LogError) << "no enough values to do comparison" << LogEnd;
    return false;
  }

  auto TestVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto RefVal = m_ValueStack.top();
  m_ValueStack.pop();

  llvm::Value* pCmpVal = nullptr;

  switch (CondOp)
  {
  case ConditionExpression::CondEq:
    pCmpVal = m_rBuilder.CreateICmpEQ(TestVal, RefVal);
    break;

  case ConditionExpression::CondNe:
    pCmpVal = m_rBuilder.CreateICmpNE(TestVal, RefVal);
    break;

  case ConditionExpression::CondUgt:
    pCmpVal = m_rBuilder.CreateICmpUGT(TestVal, RefVal);
    break;

  case ConditionExpression::CondUge:
    pCmpVal = m_rBuilder.CreateICmpUGE(TestVal, RefVal);
    break;

  case ConditionExpression::CondUlt:
    pCmpVal = m_rBuilder.CreateICmpULT(TestVal, RefVal);
    break;

  case ConditionExpression::CondUle:
    pCmpVal = m_rBuilder.CreateICmpULE(TestVal, RefVal);
    break;

  case ConditionExpression::CondSgt:
    pCmpVal = m_rBuilder.CreateICmpSGT(TestVal, RefVal);
    break;

  case ConditionExpression::CondSge:
    pCmpVal = m_rBuilder.CreateICmpSGE(TestVal, RefVal);
    break;

  case ConditionExpression::CondSlt:
    pCmpVal = m_rBuilder.CreateICmpSLT(TestVal, RefVal);
    break;

  case ConditionExpression::CondSle:
    pCmpVal = m_rBuilder.CreateICmpSLE(TestVal, RefVal);
    break;

  default:
    Log::Write("emul_llvm") << "unknown comparison" << LogEnd;
    return nullptr;
  }

  return pCmpVal;
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_EmitReadRegister(u32 Reg, CpuInformation const& rCpuInfo)
{
  auto RegName = llvm::StringRef(rCpuInfo.ConvertIdentifierToName(Reg));

  // We need the register bit size to declare its type
  auto RegBitSize = rCpuInfo.GetSizeOfRegisterInBit(Reg);

  // Make values for function parameters
  auto pRegVal = _MakeInteger(IntType(32, Reg));
  auto pBitSize = _MakeInteger(IntType(32, RegBitSize));

  // Allocate the result on the stack
  auto pRegAlloca = m_rBuilder.CreateAlloca(_BitSizeToLlvmType(RegBitSize), nullptr, RegName + "_alloc");
  auto pRegPtrVal = m_rBuilder.CreateBitCast(pRegAlloca, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), 8));

  // Call ReadRegister wrapper
  auto pCallVal = m_rBuilder.CreateCall4(s_pReadRegisterFunc, m_pCpuCtxtObjParam, pRegVal, pRegPtrVal, pBitSize, RegName + "_read");

  // Return the result
  auto pRegResVal = m_rBuilder.CreateLoad(pRegAlloca, RegName + "_val");
  return pRegResVal;
}

bool LlvmEmulator::LlvmExpressionVisitor::_EmitWriteRegister(u32 Reg, CpuInformation const& rCpuInfo, llvm::Value* pVal)
{
  auto RegName = llvm::StringRef(rCpuInfo.ConvertIdentifierToName(Reg));

  // We need the register bit size to declare its type
  auto RegBitSize = rCpuInfo.GetSizeOfRegisterInBit(Reg);

  // Make values for function parameters
  auto pRegVal = _MakeInteger(IntType(32, Reg));
  auto pBitSize = _MakeInteger(IntType(32, RegBitSize));

  // Allocate the new value on the stack
  auto pRegAlloca = m_rBuilder.CreateAlloca(_BitSizeToLlvmType(RegBitSize), nullptr, RegName + "_alloc");
  auto pStoreReg = m_rBuilder.CreateStore(pVal, pRegAlloca);
  auto pRegPtrVal = m_rBuilder.CreateBitCast(pRegAlloca, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), 8), RegName + "_ptr");

  // Call WriteRegister wrapper
  auto pCallVal = m_rBuilder.CreateCall4(s_pWriteRegisterFunc, m_pCpuCtxtObjParam, pRegVal, pRegPtrVal, pBitSize, RegName + "_write");

  return true;
}