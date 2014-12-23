#include "interpreter_emulator.hpp"
#include <medusa/log.hpp>

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
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);
  auto spCurExpr = spExpr->Visit(&Visitor);

  if (spCurExpr == nullptr)
    return false;

  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc  = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
  TestHook(Address(CurPc), Emulator::HookOnExecute);
  return true;
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::List const& rExprList)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);
  for (Expression::SPType spExpr : rExprList)
  {
    // DEBUG
    //std::cout << spExpr->ToString() << std::endl;
    auto spCurExpr = spExpr->Visit(&Visitor);
    if (spCurExpr == nullptr)
      return false;
    // DEBUG
    //std::cout << "res:\n" << spCurExpr->ToString() << std::endl;

    auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
    auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
    u64 CurPc = 0;
    m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
    TestHook(Address(CurPc), Emulator::HookOnExecute);

    // DEBUG
    //std::cout << m_pCpuCtxt->ToString() << std::endl;
  }
  return true;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr; // TODO
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  Expression::List SmplExprList;
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

  auto spExpr = (Cond ? spIfElseExpr->GetThenExpression()->Clone() : (spIfElseExpr->GetElseExpression() ? spIfElseExpr->GetElseExpression()->Clone() : nullptr));
  if (spExpr != nullptr)
    auto pStmtExpr = spExpr->Visit(this);

  return spExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  bool Cond;

  if (!_EvaluateCondition(spWhileExpr, Cond))
    return nullptr;

  auto spExpr = Cond == true ? spWhileExpr->GetBodyExpression()->Clone() : nullptr;
  if (spExpr != nullptr)
    spExpr->Visit(this);

  return spExpr;
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
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (spSrc->GetAddress(m_pCpuCtxt, m_pMemCtxt, RightAddress))
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (!spDst->Write(m_pCpuCtxt, m_pMemCtxt, Data))
    return nullptr;

  return spSrc;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitOperation(OperationExpression::SPType spOpExpr)
{
  auto spLeft = spOpExpr->GetLeftExpression();
  auto spRight = spOpExpr->GetRightExpression();

  if (spLeft == nullptr || spRight == nullptr)
    return nullptr;

  // TODO: handle size of access
  Address LeftAddress, RightAddress;
  if (spLeft->GetAddress(m_pCpuCtxt, m_pMemCtxt, LeftAddress) == true)
  {
    auto itHook = m_rHooks.find(LeftAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (spRight->GetAddress(m_pCpuCtxt, m_pMemCtxt, RightAddress) == true)
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  u32 Bit = spLeft->GetSizeInBit();
  bool Res = false;
  u8 Op = spOpExpr->GetOperation();
  switch (Bit)
  {
  case  8: return _DoOperation<s8> (Op, spLeft, spRight);
  case 16: return _DoOperation<s16>(Op, spLeft, spRight);
  case 32: return _DoOperation<s32>(Op, spLeft, spRight);
  case 64: return _DoOperation<s64>(Op, spLeft, spRight);
  default: return nullptr;
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
  auto spRef = spCondExpr->GetReferenceExpression()->Visit(this);
  auto spTest = spCondExpr->GetTestExpression()->Visit(this);

  if (spRef == nullptr || spTest == nullptr)
    return false;

  // FIXME
  rResult = false;
  return false;
}

template<typename _Type>
Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::_DoOperation(u8 Op, Expression::SPType spLeftExpr, Expression::SPType spRightExpr)
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
    Result = Left >> Right;
    break;

  case OperationExpression::OpArs:
    Result = Left >> Right;
    break;

  case OperationExpression::OpXchg:
  case OperationExpression::OpSext:
  default:
    return nullptr;
  }

  return Expr::MakeConst(Bit, Result);
}