#include "interpreter_emulator.hpp"
#include <medusa/log.hpp>
#include <medusa/extend.hpp>
#include <medusa/bits.hpp>

MEDUSA_NAMESPACE_USE

InterpreterEmulator::InterpreterEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
: Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
{
}

InterpreterEmulator::~InterpreterEmulator(void)
{
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::SPType spExpr)
{
  if (auto spSys = expr_cast<SystemExpression>(spExpr))
  {
    if (spSys->GetName() == "dump_insn")
    {
      if (m_InsnCb)
        m_InsnCb(m_pCpuCtxt, m_pMemCtxt, spSys->GetAddress());
      return true;
    }

    if (spSys->GetName() == "check_exec_hook")
    {
      auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
      auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc);
      u64 CurPc = 0;
      m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
      TestHook(Address(CurPc), Emulator::HookOnExecute);
    }
  }

  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);
  auto spCurExpr = spExpr->Visit(&Visitor);

  if (spCurExpr == nullptr)
    return false;

  return true;
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::LSPType const& rExprList)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);

  for (Expression::SPType spExpr : rExprList)
  {
    if (auto spSys = expr_cast<SystemExpression>(spExpr))
    {
      if (spSys->GetName() == "dump_insn")
      {
        if (m_InsnCb)
          m_InsnCb(m_pCpuCtxt, m_pMemCtxt, spSys->GetAddress());
        continue;
      }

      if (spSys->GetName() == "check_exec_hook")
      {
        auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
        auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc);
        u64 CurPc = 0;
        if (!m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz))
          return false;
        TestHook(Address(CurPc), Emulator::HookOnExecute);
        continue;
      }
    }

    auto spCurExpr = spExpr->Visit(&Visitor);
    if (spCurExpr == nullptr)
    {
      std::cout << m_pCpuCtxt->ToString() << std::endl;
      std::cout << spExpr->ToString() << std::endl;
      return false;
    }

//#ifdef _DEBUG
//    // DEBUG
//    std::cout << "cur: " << spExpr->ToString() << std::endl;
//    std::cout << "res: " << spCurExpr->ToString() << std::endl;
//    std::cout << m_pCpuCtxt->ToString() << std::endl;
//#endif
  }

  return true;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr; // TODO
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  Expression::LSPType SmplExprList;
  for (Expression::SPType spExpr : spBindExpr->GetBoundExpressions())
    SmplExprList.push_back(spExpr->Visit(this));
  return Expr::MakeBind(SmplExprList);
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  bool Cond;

  if (!_EvaluateCondition(spTernExpr, Cond))
    return nullptr;

  return (Cond ? spTernExpr->GetTrueExpression()->Clone() : spTernExpr->GetFalseExpression()->Clone());
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  bool Cond;

  if (!_EvaluateCondition(spIfElseExpr, Cond))
    return nullptr;

  auto spExpr = (Cond ? spIfElseExpr->GetThenExpression()->Clone() : (spIfElseExpr->GetElseExpression() ? spIfElseExpr->GetElseExpression()->Clone() : Expr::MakeBoolean(false)));
  if (spExpr != nullptr)
    return spExpr->Visit(this);

  return spExpr;
}

// FIXME:
Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return nullptr;
}

// TODO: double-check this method
Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  auto spDst = spAssignExpr->GetDestinationExpression()->Visit(this);
  auto spSrc = spAssignExpr->GetSourceExpression()->Visit(this);

  if (spDst == nullptr || spSrc == nullptr)
    return nullptr;

  Expression::DataContainerType Data;
  spDst->Prepare(Data);
  if (!spSrc->Read(m_pCpuCtxt, m_pMemCtxt, Data))
    return nullptr;

  // TODO: handle size of access
  Address LeftAddress, RightAddress;
  if (spDst->GetAddress(m_pCpuCtxt, m_pMemCtxt, LeftAddress))
  {
    auto itHook = m_rHooks.find(LeftAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt, LeftAddress);
  }

  if (spSrc->GetAddress(m_pCpuCtxt, m_pMemCtxt, RightAddress))
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt, RightAddress);
  }

  if (!spDst->Write(m_pCpuCtxt, m_pMemCtxt, Data))
    return nullptr;

  return spSrc;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExpr = spUnOpExpr->GetExpression()->Visit(this);

  if (spExpr == nullptr)
    return nullptr;

  // TODO: handle size of access
  Address ExprAddr;
  if (spExpr->GetAddress(m_pCpuCtxt, m_pMemCtxt, ExprAddr) == true)
  {
    auto itHook = m_rHooks.find(ExprAddr);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt, ExprAddr);
  }

  u8 Op = spUnOpExpr->GetOperation();

  switch (spExpr->GetSizeInBit())
  {
  case  1: // TODO: _DoBinaryOperation<bool>?
  case  8: return _DoUnaryOperation<s8>(Op,  spExpr);
  case 16: return _DoUnaryOperation<s16>(Op, spExpr);
  case 32: return _DoUnaryOperation<s32>(Op, spExpr);
  case 64: return _DoUnaryOperation<s64>(Op, spExpr);
  default:
    Log::Write("emul_interpreter") << "unhandled bit size for unary operation" << LogEnd;
    return nullptr;
  }
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLeft = spBinOpExpr->GetLeftExpression()->Visit(this);
  auto spRight = spBinOpExpr->GetRightExpression()->Visit(this);

  if (spLeft == nullptr || spRight == nullptr)
    return nullptr;

  // TODO: handle size of access
  Address LeftAddress, RightAddress;
  if (spLeft->GetAddress(m_pCpuCtxt, m_pMemCtxt, LeftAddress) == true)
  {
    auto itHook = m_rHooks.find(LeftAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt, LeftAddress);
  }

  if (spRight->GetAddress(m_pCpuCtxt, m_pMemCtxt, RightAddress) == true)
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt, RightAddress);
  }

  u8 Op = spBinOpExpr->GetOperation();

  switch (Op)
  {
  case OperationExpression::OpSext:
  {
    Expression::DataContainerType DataLeft, DataRight;
    DataLeft.resize(1);
    DataRight.resize(1);
    if (!spLeft->Read(m_pCpuCtxt, m_pMemCtxt, DataLeft))
      return nullptr;
    if (!spRight->Read(m_pCpuCtxt, m_pMemCtxt, DataRight))
      return nullptr;
    s64 Left = std::get<1>(DataLeft.front()).convert_to<u64>();
    u32 LeftBitSize = std::get<0>(DataLeft.front());
    auto Right = std::get<1>(DataRight.front()).convert_to<u32>();
    u64 Result = 0;

    switch (LeftBitSize)
    {
    case  8: Result = static_cast<s64>(SignExtend<s64, 8>(Left)); break;
    case 16: Result = static_cast<s64>(SignExtend<s64, 16>(Left)); break;
    case 32: Result = static_cast<s64>(SignExtend<s64, 32>(Left)); break;
    case 64: Result = Left; break;
    default:
      Log::Write("emul_interpreter") << "unhandled bit size for sign extend operation" << LogEnd;
      return nullptr;
    }

    return Expr::MakeConst(Right, Result);
  }

  case OperationExpression::OpZext:
  {
    Expression::DataContainerType DataLeft, DataRight;
    DataLeft.resize(1);
    DataRight.resize(1);
    if (!spLeft->Read(m_pCpuCtxt, m_pMemCtxt, DataLeft))
      return nullptr;
    if (!spRight->Read(m_pCpuCtxt, m_pMemCtxt, DataRight))
      return nullptr;
    u64 Left = std::get<1>(DataLeft.front()).convert_to<u64>();
    u32 LeftBitSize = std::get<0>(DataLeft.front());
    auto Right = std::get<1>(DataRight.front()).convert_to<u32>();
    u64 Result = 0;

    switch (LeftBitSize)
    {
    case  8: Result = Left & 0xff; break;
    case 16: Result = Left & 0xffff; break;
    case 32: Result = Left & 0xffffffff; break;
    case 64: Result = Left; break;
    default:
      Log::Write("emul_interpreter") << "unhandled bit size for zero extend operation" << LogEnd;
      return nullptr;
    }

    return Expr::MakeConst(Right, Result);
  }

  case OperationExpression::OpXchg:
  {
    Expression::DataContainerType DataLeft, DataRight;
    DataLeft.resize(1);
    DataRight.resize(1);
    if (!spLeft->Read(m_pCpuCtxt, m_pMemCtxt, DataLeft))
      return nullptr;
    if (!spRight->Read(m_pCpuCtxt, m_pMemCtxt, DataRight))
      return nullptr;
    auto Left = std::get<1>(DataLeft.front()).convert_to<u64>();
    auto Right = std::get<1>(DataRight.front()).convert_to<u32>();

    if (std::get<0>(DataLeft.front()) != std::get<0>(DataRight.front()))
    {
      Log::Write("emul_interpreter") << "mismatch size while exchanging data" << LogEnd;
      return nullptr;
    }

    if (!spLeft->Write(m_pCpuCtxt, m_pMemCtxt, DataRight))
      return nullptr;
    if (!spRight->Write(m_pCpuCtxt, m_pMemCtxt, DataLeft))
      return nullptr;

    return spBinOpExpr;
  }
  } // end of switch (Op)

  auto Bit = std::max(spLeft->GetSizeInBit(), spRight->GetSizeInBit());
  switch (Bit)
  {
  case  1: // TODO: _DoBinaryOperation<bool>?
  case  8: return _DoBinaryOperation<s8> (Op, spLeft, spRight);
  case 16: return _DoBinaryOperation<s16>(Op, spLeft, spRight);
  case 32: return _DoBinaryOperation<s32>(Op, spLeft, spRight);
  case 64: return _DoBinaryOperation<s64>(Op, spLeft, spRight);
  default:
    Log::Write("emul_interpreter") << "unhandled bit size for binary operation" << LogEnd;
    return nullptr;
  }
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitConstant(ConstantExpression::SPType pConstExpr)
{
  return pConstExpr->Clone();
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType pIdExpr)
{
  return pIdExpr->Clone();
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return spVecIdExpr->Clone();
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType pTrkIdExpr)
{
  return pTrkIdExpr->Clone();
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitMemory(MemoryExpression::SPType pMemExpr)
{
  Expression::SPType pBaseExprVisited = nullptr;
  if (pMemExpr->GetBaseExpression() != nullptr)
    pBaseExprVisited = pMemExpr->GetBaseExpression()->Visit(this);

  auto pOffsetExprVisited = pMemExpr->GetOffsetExpression()->Visit(this);

  return Expr::MakeMem(pMemExpr->GetAccessSizeInBit(), pBaseExprVisited, pOffsetExprVisited, pMemExpr->IsDereferencable());
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitSymbolic(SymbolicExpression::SPType pSymExpr)
{
  // TODO:
  return nullptr;
}

bool InterpreterEmulator::InterpreterExpressionVisitor::_EvaluateCondition(ConditionExpression::SPType spCondExpr, bool& rResult)
{
  u8 Op = spCondExpr->GetType();
  auto
    spRefExpr = spCondExpr->GetReferenceExpression()->Visit(this),
    spTestExpr = spCondExpr->GetTestExpression()->Visit(this);

  if (spRefExpr == nullptr || spTestExpr == nullptr)
    return false;
  auto Bit = std::max(spRefExpr->GetSizeInBit(), spTestExpr->GetSizeInBit());

  switch (Bit)
  {
  case  1:
  case  8: return _DoComparison<u8 >(Op, spRefExpr, spTestExpr, rResult);
  case 16: return _DoComparison<u16>(Op, spRefExpr, spTestExpr, rResult);
  case 32: return _DoComparison<u32>(Op, spRefExpr, spTestExpr, rResult);
  case 64: return _DoComparison<u64>(Op, spRefExpr, spTestExpr, rResult);
  default: return false;
  }
}

template<typename _Type>
bool InterpreterEmulator::InterpreterExpressionVisitor::_DoComparison(u8 Op, Expression::SPType spRefExpr, Expression::SPType spTestExpr, bool& rResult)
{
  Expression::DataContainerType RefData, TestData;

  RefData.resize(1);
  TestData.resize(1);

  if (!spRefExpr->Read(m_pCpuCtxt, m_pMemCtxt, RefData))
    return false;
  if (!spTestExpr->Read(m_pCpuCtxt, m_pMemCtxt, TestData))
    return false;

  // FIXME: vector condition is not supported
  if (RefData.size() != 1 || TestData.size() != 1)
    return false;

  auto
    URef = std::get<1>(RefData.front()).convert_to<typename std::make_unsigned<_Type>::type>(),
    UTest = std::get<1>(TestData.front()).convert_to<typename std::make_unsigned<_Type>::type>();

  auto
    SRef = std::get<1>(RefData.front()).convert_to<typename std::make_signed<_Type>::type>(),
    STest = std::get<1>(TestData.front()).convert_to<typename std::make_signed<_Type>::type>();

  switch (Op)
  {
  default:
    return false;

  case ConditionExpression::CondEq:
    rResult = URef == UTest;
    break;

  case ConditionExpression::CondNe:
    rResult = URef != UTest;
    break;

  case ConditionExpression::CondUgt:
    rResult = URef > UTest;
    break;

  case ConditionExpression::CondUge:
    rResult = URef >= UTest;
    break;

  case ConditionExpression::CondUlt:
    rResult = URef < UTest;
    break;

  case ConditionExpression::CondUle:
    rResult = URef <= UTest;
    break;

  case ConditionExpression::CondSgt:
    rResult = SRef > STest;
    break;

  case ConditionExpression::CondSge:
    rResult = SRef >= STest;
    break;

  case ConditionExpression::CondSlt:
    rResult = SRef < STest;
    break;

  case ConditionExpression::CondSle:
    rResult = SRef <= STest;
    break;
  }

  return true;
}

template<typename _Type>
Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::_DoUnaryOperation(u8 Op, Expression::SPType spExpr)
{
  Expression::DataContainerType Data;

  spExpr->Prepare(Data);
  // TODO: handle vectorize operation
  if (Data.size() != 1)
    return nullptr;
  if (!spExpr->Read(m_pCpuCtxt, m_pMemCtxt, Data))
    return nullptr;

  auto Bit = spExpr->GetSizeInBit();

  typename std::make_unsigned<_Type>::type
    Value = std::get<1>(Data.front()).convert_to<_Type>(),
    Result = 0;

  switch (Op)
  {
  case OperationExpression::OpNot:
    Result = ~Value;
    break;

  case OperationExpression::OpNeg:
    Result = ~Value + 1;
    break;

  case OperationExpression::OpSwap:
    Result = Value;
    EndianSwap(Result);
    break;

  case OperationExpression::OpBsf:
  {
    Result = CountTrailingZero(Value);
    if (Result == 32)
      Result = 0;
    break;
  }

  case OperationExpression::OpBsr:
  {
    Result = CountLeadingZero(Value);
    if (Result == 32)
      Result = 0;
    break;
  }

  default:
    return nullptr;
  }

  return Expr::MakeConst(Bit, Result);
}

template<typename _Type>
Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::_DoBinaryOperation(u8 Op, Expression::SPType spLeftExpr, Expression::SPType spRightExpr)
{
  static_assert(std::is_signed<_Type>::value, "only signed value");

  Expression::DataContainerType LeftData, RightData;

  spRightExpr->Prepare(LeftData);
  // TODO: handle vectorize operation
  if (LeftData.size() != 1)
    return nullptr;
  if (!spLeftExpr->Read(m_pCpuCtxt, m_pMemCtxt, LeftData))
    return nullptr;

  spLeftExpr->Prepare(RightData);
  // TODO: handle vectorize operation
  if (RightData.size() != 1)
    return nullptr;
  if (!spRightExpr->Read(m_pCpuCtxt, m_pMemCtxt, RightData))
    return nullptr;

  auto Bit = std::max(spLeftExpr->GetSizeInBit(), spRightExpr->GetSizeInBit());

  _Type
    Left   = std::get<1>(LeftData.front()).convert_to<_Type>(),
    Right  = std::get<1>(RightData.front()).convert_to<_Type>(),
    Result = 0;

  switch (Op)
  {
  case OperationExpression::OpAdd:
    Result = Left + Right;
    break;

  case OperationExpression::OpSub:
    Result = Left - Right;
    break;

  case OperationExpression::OpMul:
    Result = Left * Right;
    break;

  case OperationExpression::OpUDiv:
  case OperationExpression::OpSDiv:
    if (Right == 0)
      return nullptr;
    Result = Left / Right;
    break;

  case OperationExpression::OpSMod:
  case OperationExpression::OpUMod:
    if (Right == 0)
      return nullptr;
    Result = Left % Right;
    break;

  case OperationExpression::OpAnd:
    Result = Left & Right;
    break;

  case OperationExpression::OpOr:
    Result = Left | Right;
    break;

  case OperationExpression::OpXor:
    Result = Left ^ Right;
    break;

  case OperationExpression::OpLls:
    Result = Left << Right;
    break;

  case OperationExpression::OpLrs:
    Result = static_cast<typename std::make_unsigned<_Type>::type>(Left) >> Right;
    break;

  case OperationExpression::OpArs:
    Result = Left >> Right;
    break;

  case OperationExpression::OpSext:
    Log::Write("emul_interpreter") << "unhandled operation sign extend" << LogEnd;
    return nullptr;

  case OperationExpression::OpZext:
    Log::Write("emul_interpreter") << "unhandled operation zero extend" << LogEnd;
    return nullptr;

  case OperationExpression::OpXchg:
    Log::Write("emul_interpreter") << "unhandled operation exchange" << LogEnd;
    break;

  case OperationExpression::OpInsertBits:
    Result = (Left & Right) << CountTrailingZero(Right);
    break;

  case OperationExpression::OpExtractBits:
    Result = (Left & Right) >> CountTrailingZero(Right);
    break;

  default:
    Log::Write("emul_interpreter") << "unknown operation" << LogEnd;
    return nullptr;
  }

  return Expr::MakeConst(Bit, Result);
}