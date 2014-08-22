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

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::SPtr spExpr)
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
  for (Expression::SPtr spExpr : rExprList)
  {
    auto spCurExpr = spExpr->Visit(&Visitor);
    if (spCurExpr == nullptr)
      return false;

    auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
    auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
    u64 CurPc = 0;
    m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
    TestHook(Address(CurPc), Emulator::HookOnExecute);
  }
  return true;
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitSystem(SystemExpression::SPtr spSysExpr)
{
  return nullptr; // TODO
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(BindExpression::SPtr spBindExpr)
{
  Expression::List SmplExprList;
  for (Expression::SPtr spExpr : spBindExpr->GetBoundExpressions())
    SmplExprList.push_back(spExpr->Visit(this));
  return Expr::MakeBind(SmplExprList);
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPtr spTernExpr)
{
  auto spRef = spTernExpr->GetReferenceExpression()->Visit(this);
  auto spTest = spTernExpr->GetReferenceExpression()->Visit(this);

  if (spRef == nullptr || spTest == nullptr)
    return nullptr;

  union BisignedU64
  {
    BisignedU64(void) : u(0x0) {}
    u64 u;
    s64 s;
  } Ref, Test;
  spRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref.u, true);
  spTest->Read(m_pCpuCtxt, m_pMemCtxt, Test.u, true);

  bool Cond = false;
  switch (spTernExpr->GetType())
  {
  case ConditionExpression::CondEq: Cond = (Ref.u == Test.u) ? true : false; break;
  case ConditionExpression::CondNe: Cond = (Ref.u != Test.u) ? true : false; break;
  case ConditionExpression::CondUgt:Cond = (Ref.u >  Test.u) ? true : false; break;
  case ConditionExpression::CondUge:Cond = (Ref.u >= Test.u) ? true : false; break;
  case ConditionExpression::CondUlt:Cond = (Ref.u <  Test.u) ? true : false; break;
  case ConditionExpression::CondUle:Cond = (Ref.u <= Test.u) ? true : false; break;
  case ConditionExpression::CondSgt:Cond = (Ref.s >  Test.s) ? true : false; break;
  case ConditionExpression::CondSge:Cond = (Ref.s >= Test.s) ? true : false; break;
  case ConditionExpression::CondSlt:Cond = (Ref.s <  Test.s) ? true : false; break;
  case ConditionExpression::CondSle:Cond = (Ref.s <= Test.s) ? true : false; break;
  }

  return (Cond ? spTernExpr->GetTrueExpression()->Clone() : spTernExpr->GetFalseExpression()->Clone());
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPtr spIfElseExpr)
{
  auto spRef = spIfElseExpr->GetReferenceExpression()->Visit(this);
  auto spTest = spIfElseExpr->GetReferenceExpression()->Visit(this);

  if (spRef == nullptr || spTest == nullptr)
    return nullptr;

  union BisignedU64
  {
    BisignedU64(void) : u(0x0) {}
    u64 u;
    s64 s;
  } Ref, Test;
  spRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref.u, true);
  spTest->Read(m_pCpuCtxt, m_pMemCtxt, Test.u, true);

  bool Cond = false;
  switch (spIfElseExpr->GetType())
  {
  case ConditionExpression::CondEq: Cond = (Ref.u == Test.u) ? true : false; break;
  case ConditionExpression::CondNe: Cond = (Ref.u != Test.u) ? true : false; break;
  case ConditionExpression::CondUgt:Cond = (Ref.u >  Test.u) ? true : false; break;
  case ConditionExpression::CondUge:Cond = (Ref.u >= Test.u) ? true : false; break;
  case ConditionExpression::CondUlt:Cond = (Ref.u <  Test.u) ? true : false; break;
  case ConditionExpression::CondUle:Cond = (Ref.u <= Test.u) ? true : false; break;
  case ConditionExpression::CondSgt:Cond = (Ref.s >  Test.s) ? true : false; break;
  case ConditionExpression::CondSge:Cond = (Ref.s >= Test.s) ? true : false; break;
  case ConditionExpression::CondSlt:Cond = (Ref.s <  Test.s) ? true : false; break;
  case ConditionExpression::CondSle:Cond = (Ref.s <= Test.s) ? true : false; break;
  }

  auto spExpr = (Cond ? spIfElseExpr->GetThenExpression()->Clone() : (spIfElseExpr->GetElseExpression() ? spIfElseExpr->GetElseExpression()->Clone() : nullptr));
  if (spExpr != nullptr)
    auto pStmtExpr = spExpr->Visit(this);

  return spExpr;
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPtr spWhileExpr)
{
  auto spRef = spWhileExpr->GetReferenceExpression()->Visit(this);
  auto spTest = spWhileExpr->GetReferenceExpression()->Visit(this);

  if (spRef == nullptr || spTest == nullptr)
    return nullptr;

  union BisignedU64
  {
    u64 u;
    s64 s;
  } Ref, Test;
  spRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref.u, true);
  spTest->Read(m_pCpuCtxt, m_pMemCtxt, Test.u, true);

  bool Cond = false;
  switch (spWhileExpr->GetType())
  {
  case ConditionExpression::CondEq: Cond = (Ref.u == Test.u) ? true : false; break;
  case ConditionExpression::CondNe: Cond = (Ref.u != Test.u) ? true : false; break;
  case ConditionExpression::CondUgt:Cond = (Ref.u >  Test.u) ? true : false; break;
  case ConditionExpression::CondUge:Cond = (Ref.u >= Test.u) ? true : false; break;
  case ConditionExpression::CondUlt:Cond = (Ref.u <  Test.u) ? true : false; break;
  case ConditionExpression::CondUle:Cond = (Ref.u <= Test.u) ? true : false; break;
  case ConditionExpression::CondSgt:Cond = (Ref.s >  Test.s) ? true : false; break;
  case ConditionExpression::CondSge:Cond = (Ref.s >= Test.s) ? true : false; break;
  case ConditionExpression::CondSlt:Cond = (Ref.s <  Test.s) ? true : false; break;
  case ConditionExpression::CondSle:Cond = (Ref.s <= Test.s) ? true : false; break;
  }

  auto spExpr = Cond == true ? spWhileExpr->GetBodyExpression()->Clone() : nullptr;
  if (spExpr != nullptr)
    spExpr->Visit(this);

  return spExpr;
}

// TODO: double-check this method
Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitAssignment(AssignmentExpression::SPtr spAssignExpr)
{
  auto spDst = spAssignExpr->GetDestinationExpression()->Visit(this);
  auto spSrc = spAssignExpr->GetSourceExpression()->Visit(this);

  if (spDst == nullptr || spSrc == nullptr)
    return nullptr;

  u64 Src = 0;
  if (!spSrc->Read(m_pCpuCtxt, m_pMemCtxt, Src))
    return nullptr;

  Address LeftAddress, RightAddress;
  if (spDst->GetAddress(m_pCpuCtxt, m_pMemCtxt, LeftAddress) == true)
  {
    auto itHook = m_rHooks.find(LeftAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (spSrc->GetAddress(m_pCpuCtxt, m_pMemCtxt, RightAddress) == true)
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (!spDst->Write(m_pCpuCtxt, m_pMemCtxt, Src))
    return nullptr;

  return nullptr;
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitOperation(OperationExpression::SPtr spOpExpr)
{
  auto spLeft = spOpExpr->GetLeftExpression();
  auto spRight = spOpExpr->GetRightExpression();

  if (spLeft == nullptr || spRight == nullptr)
    return nullptr;

  u64 Left = 0, Right = 0;
  if (spRight->Read(m_pCpuCtxt, m_pMemCtxt, Right) == false)
    return nullptr;

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

  u64 SignedLeft = 0;
  spLeft->Read(m_pCpuCtxt, m_pMemCtxt, SignedLeft, true);

  switch (spOpExpr->GetOperation())
  {
  case OperationExpression::OpAdd:   Left +=  Right; break;
  case OperationExpression::OpSub:   Left -=  Right; break;
  case OperationExpression::OpMul:   Left = static_cast<s64>(SignedLeft) * Right; break;
  case OperationExpression::OpUDiv:
  case OperationExpression::OpSDiv:  Left /=  Right; break;
  case OperationExpression::OpAnd:   Left &=  Right; break;
  case OperationExpression::OpOr:    Left |=  Right; break;
  case OperationExpression::OpXor:   Left ^=  Right; break;
  case OperationExpression::OpLls:   Left <<= Right; break;
  case OperationExpression::OpLrs:   Left >>= Right; break;
  case OperationExpression::OpArs:   Left = static_cast<s64>(SignedLeft) >> Right; break;
  case OperationExpression::OpXchg:
    spLeft ->Write(m_pCpuCtxt, m_pMemCtxt, Right);
    spRight->Write(m_pCpuCtxt, m_pMemCtxt, Left );
    break;
  case OperationExpression::OpSext:
    {
      // FIXME: Handle error case
      u64 Value = 0;
      spLeft->Read(m_pCpuCtxt, m_pMemCtxt, Value, true);
      auto spReadValue = Expr::MakeConst(static_cast<u32>(Right * 8), Value);
      spReadValue->SignExtend(spLeft->GetSizeInBit());
      return spReadValue;
    }
  default: assert(0);
  }

  u32 Bit = spLeft->GetSizeInBit();
  return Expr::MakeConst(Bit, Left);
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitConstant(ConstantExpression::SPtr pConstExpr)
{
  return pConstExpr->Clone();
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitIdentifier(IdentifierExpression::SPtr pIdExpr)
{
  return pIdExpr->Clone();
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr pTrkIdExpr)
{
  return pTrkIdExpr->Clone();
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitMemory(MemoryExpression::SPtr pMemExpr)
{
  Expression::SPtr pBaseExprVisited = nullptr;
  if (pMemExpr->GetBaseExpression() != nullptr)
    pBaseExprVisited = pMemExpr->GetBaseExpression()->Visit(this);

  auto pOffsetExprVisited = pMemExpr->GetOffsetExpression()->Visit(this);

  return Expr::MakeMem(pMemExpr->GetAccessSizeInBit(), pBaseExprVisited, pOffsetExprVisited, pMemExpr->IsDereferencable());
}

Expression::SPtr InterpreterEmulator::InterpreterExpressionVisitor::VisitSymbolic(SymbolicExpression::SPtr pSymExpr)
{
  // TODO:
  return nullptr;
}