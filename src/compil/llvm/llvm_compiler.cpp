#include "llvm_compiler.hpp"

#include <medusa/log.hpp>
#include <medusa/expression_visitor.hpp>

#ifdef WIN32
# include "llvm/Support/Host.h"
#endif

#include <llvm/IR/Verifier.h>

#include <llvm/ExecutionEngine/MCJIT.h>

#include <llvm/Object/ObjectFile.h> // NOTE: Needed to avoid incomplete type llvm::object::ObjectFile

#include <llvm/Support/DynamicLibrary.h>

MEDUSA_NAMESPACE_USE

LlvmCompiler::LlvmCompiler(void)
  : m_Builder(llvm::getGlobalContext())
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
}

LlvmCompiler::~LlvmCompiler(void)
{
}

bool LlvmCompiler::Compile(std::vector<u8>& rBuffer)
{
  // Initialization
  std::string ErrMsg;
  auto& rGblCtx = llvm::getGlobalContext();
  auto pModule  = new llvm::Module("compil_llvm", rGblCtx); // TODO(wisk): can we use a std::unique_ptr here?

  // Convert medusa expressions to LLVM-IR
  u32 RetValBitSize = 0;
  std::vector<std::tuple<u32, std::string>> ParamsInfo;
  std::vector<llvm::Value*> Params;
  IdentifierToVariable Id2Var;
  for (auto const& rCodePair : m_CodeMap)
  {
    Log::Write("compil_llvm").Level(LogDebug) << "compile code: " << rCodePair.first << LogEnd;
    std::list<Expression::SPType> CompilExprs;

    // First, we need to transform expressions to be compiled
    for (auto const& rspExpr : rCodePair.second)
    {
      // Find the returned value
      if (auto spAssign = expr_cast<AssignmentExpression>(rspExpr))
        if (auto spSym = expr_cast<SymbolicExpression>(spAssign->GetDestinationExpression()))
          if (spSym->GetType() == SymbolicExpression::ReturnedValue)
            RetValBitSize = spAssign->GetSourceExpression()->GetBitSize();

      // Find parameters
      FilterVisitor FltVst([](Expression::SPType spExpr) -> Expression::SPType
      {
        if (auto spSymExpr = expr_cast<SymbolicExpression>(spExpr))
          if (spSymExpr->GetType() == SymbolicExpression::FromParameter)
            return spSymExpr;
        return nullptr;
      });
      rspExpr->Visit(&FltVst);
      for (auto const& rspParamExpr : FltVst.GetMatchedExpressions())
      {
        if (auto spSymParamExpr = expr_cast<SymbolicExpression>(rspParamExpr))
        {
          size_t ParamNo = static_cast<size_t>(spSymParamExpr->GetAddress().GetOffset());
          if (ParamsInfo.size() < ParamNo + 1)
            ParamsInfo.resize(ParamNo + 1);
          if (std::get<0>(ParamsInfo[ParamNo]) != 0)
            continue;
          ParamsInfo[ParamNo] = std::make_tuple(spSymParamExpr->GetBitSize(), spSymParamExpr->GetValue());
        }
      }

      CompilExprs.push_back(rspExpr);
    }

    // Create a function type according to SymbolicExpression values
    auto pRetType = RetValBitSize == 0 ? llvm::Type::getVoidTy(rGblCtx) : llvm::Type::getIntNTy(rGblCtx, RetValBitSize);
    std::vector<llvm::Type*> ParamsType;
    for (auto Param : ParamsInfo)
    {
      ParamsType.push_back(llvm::Type::getIntNTy(rGblCtx, std::get<0>(Param)));
    }

    // Define the current function and the first basic block
    auto pCompiledFunctionType = llvm::FunctionType::get(pRetType, ParamsType, false);
    auto pCompiledFunction     = llvm::Function::Create(pCompiledFunctionType, llvm::Function::ExternalLinkage, rCodePair.first, pModule);
    auto pCompiledBasicBlock   = llvm::BasicBlock::Create(rGblCtx, rCodePair.first + "_entry", pCompiledFunction);
    m_Builder.SetInsertPoint(pCompiledBasicBlock);

    // now we can convert them to LLVM-IR
    auto itArg = pCompiledFunction->arg_begin();
    for (auto Param : ParamsInfo)
    {
      auto pParamVal = itArg++;
      m_Vars[std::get<1>(Param)] = std::make_tuple(std::get<0>(Param), pParamVal);
    }
    LlvmExpressionVisitor LlvmExpressionVisitor(this, m_Vars, m_Builder);
    for (auto const& rspExpr : CompilExprs)
    {
      if (rspExpr->Visit(&LlvmExpressionVisitor) == nullptr)
      {
        Log::Write("compil_llvm").Level(LogError) << "failed to compile expression: " << rspExpr->ToString() << LogEnd;
        return false;
      }
    }

    if (RetValBitSize == 0)
      m_Builder.CreateRetVoid();

    pCompiledFunction->dump();
  }

  // Compilation
  auto pExecutionEngine = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(pModule))
    .setErrorStr(&ErrMsg)
    .setOptLevel(llvm::CodeGenOpt::Default)
    .setRelocationModel(llvm::Reloc::PIC_)
    .setCodeModel(llvm::CodeModel::Small)
    .create();
  auto pTargetMachine = pExecutionEngine->getTargetMachine();
  llvm::SmallString<0> StringBuffer;
  llvm::raw_svector_ostream OstreamBuffer(StringBuffer);
  llvm::legacy::PassManager PassManager;

  llvm::legacy::FunctionPassManager FunctionPassManager(pModule);
  FunctionPassManager.add(llvm::createBasicAliasAnalysisPass());
  FunctionPassManager.add(llvm::createReassociatePass());
  FunctionPassManager.add(llvm::createPromoteMemoryToRegisterPass());
  FunctionPassManager.add(llvm::createInstructionCombiningPass());
  FunctionPassManager.add(llvm::createGVNPass());
  FunctionPassManager.add(llvm::createCFGSimplificationPass());

  FunctionPassManager.doInitialization();
  for (auto& rFunc : *pModule)
    FunctionPassManager.run(rFunc);

  pTargetMachine->addPassesToEmitFile(PassManager, OstreamBuffer, llvm::TargetMachine::CGFT_ObjectFile); // NOTE(wisk): don't check against false returned value here
  if (!PassManager.run(*pModule))
  {
    Log::Write("compil_llvm").Level(LogError) << "emit code to buffer failed" << LogEnd;
    return false;
  }

  // Get object file
  llvm::MemoryBufferRef MemoryBuffer(OstreamBuffer.str(), "compiled-buffer");
  auto pObjectFile = llvm::object::ObjectFile::createObjectFile(MemoryBuffer);
  if (auto ErrorCode = pObjectFile.getError())
  {
    Log::Write("compil_llvm").Level(LogError) << "unable to get object file: " << ErrorCode.message() << LogEnd;
    return false;
  }

  // Find .text section (section which contains compiled data)
  // FIXME(wisk): some parts of the code might not be included in this section...
  llvm::StringRef TextSectionContents;
  for (auto const& rSection : (*pObjectFile)->sections())
  {
    if (!rSection.isText())
      continue;
    if (auto ErrorCode = rSection.getContents(TextSectionContents))
    {
      Log::Write("compil_llvm").Level(LogError) << "unable to get .text section data: " << ErrorCode.message() << LogEnd;
      return false;
    }
    break;
  }

  // Get compiled data
  if (TextSectionContents.empty())
  {
    Log::Write("compil_llvm").Level(LogError) << "unable to find .text section" << LogEnd;
    return false;
  }
  rBuffer.resize(TextSectionContents.size());
  rBuffer = std::vector<u8>(TextSectionContents.data(), TextSectionContents.data() + TextSectionContents.size()); // LATER(wisk): can we improve this code by moving the pointer from llvm::StringRef?

  return true;
}

LlvmCompiler::LlvmExpressionVisitor::LlvmExpressionVisitor(
  Compiler* pCompil,
  VarMapType& rVars,
  llvm::IRBuilder<>& rBuilder)
  : m_pCompil(pCompil)
  , m_rVars(rVars)
  , m_rBuilder(rBuilder)
  , m_NrOfValueToRead(), m_State(Unknown)
{
}

LlvmCompiler::LlvmExpressionVisitor::~LlvmExpressionVisitor(void)
{
  while (!m_ValueStack.empty())
  {
    auto pCurVal = m_ValueStack.top();
    m_ValueStack.pop();
    Log::Write("compil_llvm") << "leaked value: " << pCurVal->getName().str() << LogEnd;
  }
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  auto const& rSysName = spSysExpr->GetName();

  Log::Write("compil_llvm").Level(LogWarning) << "unhandled system expression: " << rSysName << LogEnd;
  return spSysExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  for (auto spExpr : spBindExpr->GetBoundExpressions())
  {
    if (spExpr->Visit(this) == nullptr)
      return nullptr;
  }
  return spBindExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  // _Emit test and ref
  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spTernExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spTernExpr->GetTestExpression()->Visit(this);

  // Emit the condition
  auto pCondVal = _EmitComparison(spTernExpr->GetType(), "emit_tern_cmp");
  if (pCondVal == nullptr)
    return nullptr;

  auto spTrueExpr = spTernExpr->GetTrueExpression();
  if (spTrueExpr->Visit(this) == nullptr)
    return nullptr;
  if (m_ValueStack.size() < 1)
    return nullptr;
  auto pTrueVal = m_ValueStack.top();
  m_ValueStack.pop();

  auto spFalseExpr = spTernExpr->GetFalseExpression();
  if (spFalseExpr->Visit(this) == nullptr)
    return nullptr;
  if (m_ValueStack.size() < 1)
    return nullptr;
  auto pFalseVal = m_ValueStack.top();
  m_ValueStack.pop();

  m_State = OldState;

  auto pTernVal = m_rBuilder.CreateSelect(pCondVal, pTrueVal, pFalseVal, "tern");
  m_ValueStack.push(pTernVal);
  return spTernExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  auto pBbOrig = m_rBuilder.GetInsertBlock();
  auto& rCtxt = llvm::getGlobalContext();
  auto pFunc = pBbOrig->getParent();

  auto pBbCond = llvm::BasicBlock::Create(rCtxt, "cond", pFunc);
  auto pBbThen = llvm::BasicBlock::Create(rCtxt, "then", pFunc);
  auto pBbElse = llvm::BasicBlock::Create(rCtxt, "else", pFunc);
  auto pBbMerg = llvm::BasicBlock::Create(rCtxt, "merg", pFunc);

  m_rBuilder.CreateBr(pBbCond);

  // _Emit test and ref
  m_rBuilder.SetInsertPoint(pBbCond);

  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spIfElseExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spIfElseExpr->GetTestExpression()->Visit(this);

  // Emit the condition
  auto pCondVal = _EmitComparison(spIfElseExpr->GetType(), "emit_if_else_cmp");
  if (pCondVal == nullptr)
    return nullptr;
  m_State = OldState;
  m_rBuilder.CreateCondBr(pCondVal, pBbThen, pBbElse);

  // Emit the then branch
  m_rBuilder.SetInsertPoint(pBbThen);

  auto spThenExpr = spIfElseExpr->GetThenExpression();
  OldState = m_State;
  m_State = Unknown;
  if (spThenExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbMerg);

  // Emit the else branch
  m_rBuilder.SetInsertPoint(pBbElse);

  auto spElseExpr = spIfElseExpr->GetElseExpression();
  OldState = m_State;
  m_State = Unknown;
  if (spElseExpr != nullptr)
    if (spElseExpr->Visit(this) == nullptr)
      return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbMerg);

  m_rBuilder.SetInsertPoint(pBbMerg);
  return spIfElseExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  auto pBbOrig = m_rBuilder.GetInsertBlock();
  auto& rCtxt = llvm::getGlobalContext();
  auto pFunc = pBbOrig->getParent();

  auto pBbCond = llvm::BasicBlock::Create(rCtxt, "cond", pFunc);
  auto pBbBody = llvm::BasicBlock::Create(rCtxt, "body", pFunc);
  auto pBbNext = llvm::BasicBlock::Create(rCtxt, "next", pFunc);

  m_rBuilder.CreateBr(pBbCond);

  m_rBuilder.SetInsertPoint(pBbCond);

  // _Emit test and ref
  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spWhileExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spWhileExpr->GetTestExpression()->Visit(this);
  auto pCondVal = _EmitComparison(spWhileExpr->GetType(), "emit_while_cmp");
  if (pCondVal == nullptr)
    return nullptr;
  m_State = OldState;

  // Emit the condition
  m_rBuilder.CreateCondBr(pCondVal, pBbBody, pBbNext);

  // Emit the body branch
  auto spBodyExpr = spWhileExpr->GetBodyExpression();
  m_rBuilder.SetInsertPoint(pBbBody);
  OldState = m_State;
  m_State = Unknown;
  if (spBodyExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbCond);

  m_rBuilder.SetInsertPoint(pBbNext);

  return spWhileExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
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

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
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
    Log::Write("compil_llvm") << "unhandled unary operation for " << spUnOpExpr->ToString() << LogEnd;
    return nullptr;

  case OperationExpression::OpNot:
    pUnOpVal = m_rBuilder.CreateNot(pVal);
    break;

  case OperationExpression::OpNeg:
    pUnOpVal = m_rBuilder.CreateNeg(pVal);
    break;

  case OperationExpression::OpSwap:
    pUnOpVal = _CallIntrinsic(llvm::Intrinsic::bswap, { pVal->getType() }, { pVal });
    break;

  case OperationExpression::OpBsf:
    pUnOpVal = _CallIntrinsic(llvm::Intrinsic::cttz,
    { pVal->getType() },
    { pVal, _MakeInteger(BitVector(1, 1)) });
    break;

  case OperationExpression::OpBsr:
  {
    auto pCtlz = _CallIntrinsic(llvm::Intrinsic::ctlz,
    { pVal->getType() },
    { pVal, _MakeInteger(BitVector(1, 1)) });
    // NOTE(wisk): LLVM operation ctlz gives the result from the beginning (in x86 it emits bsr op0, op1 ; xor op0, op1.bitsize - 1)
    // we have to reverse the last xor to avoid invalid result
    auto ValBitSize = pVal->getType()->getScalarSizeInBits();
    pUnOpVal = m_rBuilder.CreateXor(pCtlz, _MakeInteger(BitVector(ValBitSize, ValBitSize - 1)));
  }
  break;
  }

  if (pUnOpVal == nullptr)
    return nullptr;
  m_ValueStack.push(pUnOpVal);
  return spUnOpExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
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
  auto BinOp = spBinOpExpr->GetOperation();

  switch (BinOp)
  {
  default:
    Log::Write("compil_llvm") << "unhandled binary operation for " << spBinOpExpr->ToString() << LogEnd;
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
    pBinOpVal = m_rBuilder.CreateShl(LeftVal, RightVal, "lls");
    break;

  case OperationExpression::OpLrs:
    pBinOpVal = m_rBuilder.CreateLShr(LeftVal, RightVal, "lrs");
    break;

  case OperationExpression::OpArs:
    pBinOpVal = m_rBuilder.CreateAShr(LeftVal, RightVal, "ars");
    break;

  case OperationExpression::OpRol:
  {
    auto pLeftType = LeftVal->getType();
    auto LeftBits = pLeftType->getScalarSizeInBits();
    if (LeftBits == 0)
      break;

    auto pCnt = m_rBuilder.CreateURem(RightVal, _MakeInteger(BitVector(LeftBits, LeftBits)));
    auto pRol0 = m_rBuilder.CreateShl(LeftVal, pCnt, "rol_0");
    auto pRolSub = m_rBuilder.CreateSub(_MakeInteger(BitVector(LeftBits, LeftBits)), pCnt, "rol_sub");
    auto pRol1 = m_rBuilder.CreateLShr(LeftVal, pRolSub, "rol_1");
    pBinOpVal = m_rBuilder.CreateOr(pRol0, pRol1, "rol");
  }
  break;

  case OperationExpression::OpRor:
  {
    auto pLeftType = LeftVal->getType();
    auto LeftBits = pLeftType->getScalarSizeInBits();
    if (LeftBits == 0)
      break;

    auto pCnt = m_rBuilder.CreateURem(RightVal, _MakeInteger(BitVector(LeftBits, LeftBits)));
    auto pRor0 = m_rBuilder.CreateLShr(LeftVal, pCnt, "ror_0");
    auto pRorSub = m_rBuilder.CreateSub(_MakeInteger(BitVector(LeftBits, LeftBits)), pCnt, "rol_sub");
    auto pRor1 = m_rBuilder.CreateShl(LeftVal, pRorSub, "ror_1");
    pBinOpVal = m_rBuilder.CreateOr(pRor0, pRor1, "ror");
  }
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
  {
    pBinOpVal = m_rBuilder.CreateURem(LeftVal, RightVal, "umod");
    break;
  }

  case OperationExpression::OpSext:
    pBinOpVal = m_rBuilder.CreateSExt(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "sext");
    break;

  case OperationExpression::OpZext:
    pBinOpVal = m_rBuilder.CreateZExt(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "zext");
    break;

  case OperationExpression::OpBcast:
    pBinOpVal = m_rBuilder.CreateZExtOrTrunc(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "bcast");
    break;

  case OperationExpression::OpInsertBits:
  {
    auto spRConst = expr_cast<BitVectorExpression>(spRight);
    if (spRConst == nullptr)
      break;
    auto pShift = _MakeInteger(spRConst->GetInt().Lsb());
    auto pMask = _MakeInteger(spRConst->GetInt());
    auto pShiftedVal = m_rBuilder.CreateShl(LeftVal, pShift, "insert_bits0");
    pBinOpVal = m_rBuilder.CreateAnd(pShiftedVal, pMask, "insert_bits1");
    break;
  }

  case OperationExpression::OpExtractBits:
  {
    auto spRConst = expr_cast<BitVectorExpression>(spRight);
    if (spRConst == nullptr)
      break;
    auto pShift = _MakeInteger(spRConst->GetInt().Lsb());
    auto pMask = _MakeInteger(spRConst->GetInt());
    auto pMaskedVal = m_rBuilder.CreateAnd(LeftVal, pMask, "extract_bits0");
    pBinOpVal = m_rBuilder.CreateLShr(pMaskedVal, pShift, "extract_bits1");
    break;
  }

  case OperationExpression::OpClearBits:
  {
    pBinOpVal = m_rBuilder.CreateAnd(LeftVal, m_rBuilder.CreateNot(RightVal), "clear_bits");
    break;
  }

  case OperationExpression::OpFAdd:
  case OperationExpression::OpFSub:
  case OperationExpression::OpFMul:
  case OperationExpression::OpFDiv:
  case OperationExpression::OpFMod:
    pBinOpVal = _EmitFloatingPointBinaryOperation(static_cast<OperationExpression::Type>(BinOp), LeftVal, RightVal);
    break;
  }

  if (pBinOpVal == nullptr)
  {
    Log::Write("compil_llvm").Level(LogError) << "failed to emit expression: " << spBinOpExpr->ToString() << LogEnd;
    return nullptr;
  }

  m_ValueStack.push(pBinOpVal);
  return spBinOpExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitBitVector(BitVectorExpression::SPType spConstExpr)
{
  if (m_State != Read)
  {
    Log::Write("compil_llvm").Level(LogError) << "constant can only be read" << LogEnd;
    return nullptr;
  }
  auto pConstVal = _MakeInteger(spConstExpr->GetInt());
  if (pConstVal == nullptr)
    return nullptr;
  m_ValueStack.push(pConstVal);
  return spConstExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  auto Id        = spIdExpr->GetId();
  auto IdName    = spIdExpr->GetCpuInformation()->ConvertIdentifierToName(Id);
  auto IdBitSize = spIdExpr->GetBitSize();
  auto& rGblCtxt = llvm::getGlobalContext();
  auto pIdType   = llvm::Type::getIntNTy(rGblCtxt, IdBitSize);

  switch (m_State)
  {
  case Read:
  {
    auto pFuncType = llvm::FunctionType::get(pIdType, false);
    auto Constraints = std::string("={") + IdName + "}";
    auto pAsm = llvm::InlineAsm::get(pFuncType, "", Constraints, true);
    auto pCallAsm = m_rBuilder.CreateCall(pAsm);
    m_ValueStack.push(pCallAsm);
    break;
  }

  case Write:
  {
    if (m_ValueStack.empty())
      return nullptr;
    auto pSrc = m_ValueStack.top();
    m_ValueStack.pop();

    //auto const& rCpuInfo = *spIdExpr->GetCpuInformation();
    //auto PcId = rCpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, static_cast<u8>(IdBitSize)); // HACK(wisk): the mode should come from this instance...
    //if (PcId == Id)
    //{
    //  auto DstBitSize = pSrc->getType()->getScalarSizeInBits();
    //  m_rBuilder.CreateIndirectBr(_MakePointer(DstBitSize, pSrc));
    //}
    //else
    {
      auto pFuncType = llvm::FunctionType::get(llvm::Type::getVoidTy(rGblCtxt), { pIdType }, false);
      auto Constraints = std::string("{") + IdName + "}";
      auto pAsm = llvm::InlineAsm::get(pFuncType, "", Constraints, true);
      auto pCallAsm = m_rBuilder.CreateCall(pAsm, { pSrc });
    }
    break;
  }

  default:
    return nullptr;
  }
  return spIdExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  Log::Write("compil_llvm").Level(LogError) << "vector identifier expression are not supported" << LogEnd;
  return nullptr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  switch (m_State)
  {
  case Unknown:
  {
    switch (spVarExpr->GetType())
    {
    case VariableExpression::Alloc:
      m_rVars[spVarExpr->GetName()] = std::make_tuple(spVarExpr->GetBitSize(), m_rBuilder.CreateAlloca(_BitSizeToLlvmType(spVarExpr->GetBitSize()), nullptr, spVarExpr->GetName()));
      break;

    case VariableExpression::Free:
      m_rVars.erase(spVarExpr->GetName());
      break;

    default:
      Log::Write("compil_llvm").Level(LogError) << "unknown variable action" << LogEnd;
      return nullptr;
    }
    break;
  }

  case Read:
    if (spVarExpr->GetType() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      spVarExpr->SetBitSize(std::get<0>(itVar->second));
      m_ValueStack.push(m_rBuilder.CreateLoad(std::get<1>(itVar->second), "read_var"));
      break;
    }
    else
    {
      Log::Write("compil_llvm").Level(LogError) << "invalid state for variable reading" << LogEnd;
      return nullptr;
    }

  case Write:
    if (spVarExpr->GetType() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      spVarExpr->SetBitSize(std::get<0>(itVar->second));
      if (m_ValueStack.empty())
        return nullptr;
      auto pVal = m_ValueStack.top();
      m_ValueStack.pop();
      m_rBuilder.CreateStore(pVal, std::get<1>(itVar->second));
      break;
    }
    else
    {
      Log::Write("compil_llvm").Level(LogError) << "invalid state for variable writing" << LogEnd;
      return nullptr;
    }

  default:
    return nullptr;
  }

  return spVarExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  State OldState = m_State;
  m_State = Read;

  auto spOffsetExpr = spMemExpr->GetOffsetExpression();
  auto spBaseExpr = spMemExpr->GetBaseExpression();

  if (spBaseExpr == nullptr)
    m_ValueStack.push(_MakeInteger(BitVector(16, 0x0)));
  else if (spBaseExpr->Visit(this) == nullptr)
    return nullptr;

  if (spOffsetExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;

  if (m_ValueStack.size() < 2)
  {
    Log::Write("compil_llvm").Level(LogError) << "no value for address" << LogEnd;
    return nullptr;
  }

  auto pOffVal = m_rBuilder.CreateZExtOrBitCast(m_ValueStack.top(), _BitSizeToLlvmType(64));
  m_ValueStack.pop();
  auto pBaseVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto AccBitSize = spMemExpr->GetAccessSizeInBit();

  // TODO(wisk): handle x86 fs/gs segment register
  // using gs = 256, and fs = 257
  // http://llvm.org/docs/doxygen/html/classllvm_1_1PointerType.html
  auto pPtrVal = m_rBuilder.CreateIntToPtr(pOffVal, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), AccBitSize));

  switch (m_State)
  {
  default:
    Log::Write("compil_llvm").Level(LogError) << "unknown state for address" << LogEnd;
    return nullptr;

  case Read:
  {
    if (!spMemExpr->IsDereferencable())
    {
      return nullptr; // TODO(wisk):

      break;
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
      //m_rBuilder.CreateAdd(pPtrVal, _MakeInteger(BitVector(AccBitSize, AccBitSize / 8)), "inc_ptr");
      --m_NrOfValueToRead;
    }
    break;
  }

  case Write:
  {
    if (m_ValueStack.empty())
    {
      Log::Write("compil_llvm").Level(LogError) << "no value for address writing" << LogEnd;
      return nullptr;
    }

    do
    {
      auto pCurVal = m_ValueStack.top();
      m_ValueStack.pop();
      m_rBuilder.CreateStore(pCurVal, pPtrVal);
    } while (!m_ValueStack.empty());
    break;
  }
  }

  return spMemExpr;
}

Expression::SPType LlvmCompiler::LlvmExpressionVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  switch (m_State)
  {
  case Read:
    switch (spSymExpr->GetType())
    {
    case SymbolicExpression::FromParameter:
    {
      auto itVar = m_rVars.find(spSymExpr->GetValue());
      if (itVar == std::end(m_rVars))
        return nullptr;
      m_ValueStack.push(std::get<1>(itVar->second));
      return spSymExpr;
    }

    default:
      break;
    }

  case Write:
  {
    if (m_ValueStack.empty())
      return nullptr;

    switch (spSymExpr->GetType())
    {
    case SymbolicExpression::ReturnedValue:
    {
      auto pRetVal = m_ValueStack.top();
      m_ValueStack.pop();
      m_rBuilder.CreateRet(pRetVal);
      return spSymExpr;
    }

    default:
      break;
    }

    if (spSymExpr->GetValue() == "instruction_pointer")
    {
      auto pDstVal = m_ValueStack.top();
      m_ValueStack.pop();
      auto DstBitSize = pDstVal->getType()->getScalarSizeInBits();
      m_rBuilder.CreateIndirectBr(_MakePointer(DstBitSize, m_ValueStack.top()));
      m_ValueStack.pop();
      return spSymExpr;
    }
    break;
  }

  default:
    break;
  }

  if (spSymExpr->GetExpression() == nullptr)
    return spSymExpr;
  return spSymExpr->GetExpression()->Visit(this);
}

llvm::Value* LlvmCompiler::LlvmExpressionVisitor::_MakeInteger(BitVector const& rInt) const
{
  // TODO(KS): Handle integer larger than 64-bit
  if (rInt.GetBitSize() > 64)
  {
    // Skip "0x"
    return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(rInt.GetBitSize(), rInt.ToString().c_str() + 2, 16));
    Log::Write("compil_llvm").Level(LogError) << "unsupported int size " << rInt.GetBitSize() << LogEnd;
    return nullptr;
  }
  return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(rInt.GetBitSize(), rInt.ConvertTo<u64>()));
}

llvm::Value* LlvmCompiler::LlvmExpressionVisitor::_MakePointer(u32 Bits, void* pPointer, s32 Offset) const
{
  //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
  auto pConstInt = llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), reinterpret_cast<u64>(pPointer));
  auto pPtr = llvm::ConstantExpr::getIntToPtr(pConstInt, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));

  if (Offset == 0x0)
    return pPtr;

  return m_rBuilder.CreateGEP(pPtr, _MakeInteger(BitVector(Offset)));
}

llvm::Value* LlvmCompiler::LlvmExpressionVisitor::_MakePointer(u32 Bits, llvm::Value* pPointerValue, s32 Offset) const
{
  if (Offset != 0x0)
  {
    //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
    pPointerValue = m_rBuilder.CreateGEP(pPointerValue, _MakeInteger(BitVector(Offset)));
  }

  return m_rBuilder.CreateBitCast(pPointerValue, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));
}

llvm::Type* LlvmCompiler::LlvmExpressionVisitor::_BitVectorToLlvmType(BitVector const& rInt) const
{
  return _BitSizeToLlvmType(rInt.GetBitSize());
}

llvm::Type* LlvmCompiler::LlvmExpressionVisitor::_BitSizeToLlvmType(u16 BitSize) const
{
  return llvm::Type::getIntNTy(llvm::getGlobalContext(), BitSize);
}

llvm::Value* LlvmCompiler::LlvmExpressionVisitor::_CallIntrinsic(
  llvm::Intrinsic::ID IntrId,
  std::vector<llvm::Type*> const& rTypes,
  std::vector<llvm::Value*> const& rArgs) const
{
  auto pModule = m_rBuilder.GetInsertBlock()->getParent()->getParent();
  auto pIntrFunc = llvm::Intrinsic::getDeclaration(pModule, IntrId, rTypes);
  auto pCallIntr = m_rBuilder.CreateCall(pIntrFunc, rArgs);
  return pCallIntr;
}

llvm::Value* LlvmCompiler::LlvmExpressionVisitor::_EmitComparison(u8 CondOp, char const* pCmpName)
{
  if (m_ValueStack.size() < 2)
  {
    Log::Write("compil_llvm").Level(LogError) << "no enough values to do comparison" << LogEnd;
    return nullptr;
  }

  auto RefVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto TestVal = m_ValueStack.top();
  m_ValueStack.pop();

  llvm::Value* pCmpVal = nullptr;

  switch (CondOp)
  {
  case ConditionExpression::CondEq:
    pCmpVal = m_rBuilder.CreateICmpEQ(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondNe:
    pCmpVal = m_rBuilder.CreateICmpNE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUgt:
    pCmpVal = m_rBuilder.CreateICmpUGT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUge:
    pCmpVal = m_rBuilder.CreateICmpUGE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUlt:
    pCmpVal = m_rBuilder.CreateICmpULT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUle:
    pCmpVal = m_rBuilder.CreateICmpULE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSgt:
    pCmpVal = m_rBuilder.CreateICmpSGT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSge:
    pCmpVal = m_rBuilder.CreateICmpSGE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSlt:
    pCmpVal = m_rBuilder.CreateICmpSLT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSle:
    pCmpVal = m_rBuilder.CreateICmpSLE(TestVal, RefVal, pCmpName);
    break;

  default:
    Log::Write("compil_llvm") << "unknown comparison" << LogEnd;
    return nullptr;
  }

  return pCmpVal;
}

llvm::Value* LlvmCompiler::LlvmExpressionVisitor::_EmitFloatingPointBinaryOperation(OperationExpression::Type FOpType, llvm::Value* pLeftVal, llvm::Value* pRightVal) const
{
  auto pLeftType = pLeftVal->getType();
  auto LeftBits = pLeftType->getScalarSizeInBits();
  auto pRightType = pRightVal->getType();
  auto RightBits = pRightType->getScalarSizeInBits();

  if (LeftBits != RightBits)
  {
    Log::Write("compil_llvm").Level(LogError) << "mismatch type for floating point operations" << LogEnd;
    return nullptr;
  }

  llvm::Type* pFloatType = nullptr;
  switch (LeftBits)
  {
  case 32:
    pFloatType = llvm::Type::getFloatTy(llvm::getGlobalContext());
    break;

  case 64:
    pFloatType = llvm::Type::getDoubleTy(llvm::getGlobalContext());
    break;

  default:
    Log::Write("compil_llvm").Level(LogError) << "unhandle floating point size: " << LeftBits << LogEnd;
    return nullptr;
  }

  auto pLeftFloat = m_rBuilder.CreateCast(llvm::Instruction::BitCast, pLeftVal, pFloatType, "leftval_to_flp");
  auto pRightFloat = m_rBuilder.CreateCast(llvm::Instruction::BitCast, pRightVal, pFloatType, "rightval_to_flp");

  llvm::Value* pFloatOpVal = nullptr;
  switch (FOpType)
  {
  case OperationExpression::OpFAdd: pFloatOpVal = m_rBuilder.CreateFAdd(pLeftFloat, pRightFloat, "fadd"); break;
  case OperationExpression::OpFSub: pFloatOpVal = m_rBuilder.CreateFSub(pLeftFloat, pRightFloat, "fsub"); break;
  case OperationExpression::OpFMul: pFloatOpVal = m_rBuilder.CreateFMul(pLeftFloat, pRightFloat, "fmul"); break;
  case OperationExpression::OpFDiv: pFloatOpVal = m_rBuilder.CreateFDiv(pLeftFloat, pRightFloat, "fdiv"); break;
  case OperationExpression::OpFMod: pFloatOpVal = m_rBuilder.CreateFRem(pLeftFloat, pRightFloat, "fmod"); break;

  default:
    Log::Write("compil_llvm").Level(LogError) << "unhandle floating point operation" << LogEnd;
    return nullptr;
  }

  return m_rBuilder.CreateCast(llvm::Instruction::BitCast, pFloatOpVal, pLeftType, "res_to_bv");
}