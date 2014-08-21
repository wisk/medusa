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

bool InterpreterEmulator::Execute(Address const& rAddress, Expression& rExpr)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);
  auto pCurExpr = rExpr.Visit(&Visitor);

  if (pCurExpr == nullptr)
    return false;

  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc  = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
  TestHook(Address(CurPc), Emulator::HookOnExecute);
  delete pCurExpr;
  return true;
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::List const& rExprList)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);
  for (Expression* pExpr : rExprList)
  {
    auto pCurExpr = pExpr->Visit(&Visitor);
    if (pCurExpr == nullptr)
      return false;

    auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
    auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
    u64 CurPc = 0;
    m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
    TestHook(Address(CurPc), Emulator::HookOnExecute);
    delete pCurExpr;
  }
  return true;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitSystem(SystemExpression* pSysExpr)
{
  return nullptr; // TODO
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(BindExpression* pBindExpr)
{
  Expression::List SmplExprList;
  for (Expression* pExpr : pBindExpr->GetBoundExpressions())
    SmplExprList.push_back(pExpr->Visit(this));
  return new BindExpression(SmplExprList);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression* pTernExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pTernExpr->GetTestExpression()->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTernExpr->GetReferenceExpression()->Visit(this));

  if (pRef == nullptr || pTest == nullptr)
  {
    delete pRef;
    delete pTest;
    return nullptr;
  }

  union BisignedU64
  {
    BisignedU64(void) : u(0x0) {}
    u64 u;
    s64 s;
  } Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref.u, true);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, Test.u, true);
  delete pRef;
  delete pTest;

  bool Cond = false;
  switch (pTernExpr->GetType())
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

  return (Cond ? pTernExpr->GetTrueExpression()->Clone() : pTernExpr->GetFalseExpression()->Clone());
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression* pIfElseExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pIfElseExpr->GetReferenceExpression()->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pIfElseExpr->GetReferenceExpression()->Visit(this));

  if (pRef == nullptr || pTest == nullptr)
  {
    delete pRef;
    delete pTest;
    return nullptr;
  }

  union BisignedU64
  {
    BisignedU64(void) : u(0x0) {}
    u64 u;
    s64 s;
  } Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref.u, true);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, Test.u, true);
  delete pRef;
  delete pTest;

  bool Cond = false;
  switch (pIfElseExpr->GetType())
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

  auto pExpr = (Cond ? pIfElseExpr->GetThenExpression()->Clone() : (pIfElseExpr->GetElseExpression() ? pIfElseExpr->GetElseExpression()->Clone() : nullptr));
  if (pExpr != nullptr)
  {
    auto pStmtExpr = pExpr->Visit(this);
    delete pStmtExpr;
  }
  return pExpr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitWhileCondition(WhileConditionExpression* pWhileExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pWhileExpr->GetReferenceExpression()->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pWhileExpr->GetTestExpression()->Visit(this));

  if (pRef == nullptr || pTest == nullptr)
  {
    delete pRef;
    delete pTest;
    return nullptr;
  }

  union BisignedU64
  {
    u64 u;
    s64 s;
  } Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref.u, true);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, Test.u, true);
  delete pRef;
  delete pTest;

  bool Cond = false;
  switch (pWhileExpr->GetType())
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

  auto pExpr = Cond == true ? pWhileExpr->GetBodyExpression()->Clone() : nullptr;
  if (pExpr != nullptr)
  {
    auto pStmtExpr = pExpr->Visit(this);
    delete pStmtExpr;
  }
  return pExpr;
}

// TODO: double-check this method
Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitAssignment(AssignmentExpression* pAssignExpr)
{
  auto pVstDstExpr = dynamic_cast<ContextExpression *>(pAssignExpr->GetDestinationExpression()->Visit(this));
  auto pVstSrcExpr = dynamic_cast<ContextExpression *>(pAssignExpr->GetSourceExpression()->Visit(this));

  if (pVstDstExpr == nullptr || pVstSrcExpr == nullptr)
  {
    delete pVstDstExpr;
    delete pVstSrcExpr;
    return nullptr;
  }

  u64 Src = 0;
  if (!pVstSrcExpr->Read(m_pCpuCtxt, m_pMemCtxt, Src))
  {
    delete pVstDstExpr;
    delete pVstSrcExpr;
    return nullptr;
  }

  Address LeftAddress, RightAddress;
  if (pVstDstExpr->GetAddress(m_pCpuCtxt, m_pMemCtxt, LeftAddress) == true)
  {
    auto itHook = m_rHooks.find(LeftAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (pVstSrcExpr->GetAddress(m_pCpuCtxt, m_pMemCtxt, RightAddress) == true)
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (!pVstDstExpr->Write(m_pCpuCtxt, m_pMemCtxt, Src))
  {
    delete pVstDstExpr;
    delete pVstSrcExpr;
    return nullptr;
  }

  return nullptr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitOperation(OperationExpression* pOpExpr)
{
  auto pLeft  = dynamic_cast<ContextExpression *>(pOpExpr->GetLeftExpression()->Visit(this));
  auto pRight = dynamic_cast<ContextExpression *>(pOpExpr->GetRightExpression()->Visit(this));

  if (pLeft == nullptr || pRight == nullptr)
  {
    delete pLeft;
    delete pRight;
    return nullptr;
  }

  u64 Left = 0, Right = 0;
  if (pRight->Read(m_pCpuCtxt, m_pMemCtxt, Right) == false)
  {
    delete pLeft;
    delete pRight;
    return nullptr;
  }

  Address LeftAddress, RightAddress;
  if (pLeft->GetAddress(m_pCpuCtxt, m_pMemCtxt, LeftAddress) == true)
  {
    auto itHook = m_rHooks.find(LeftAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (pRight->GetAddress(m_pCpuCtxt, m_pMemCtxt, RightAddress) == true)
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  u64 SignedLeft = 0;
  pLeft->Read(m_pCpuCtxt, m_pMemCtxt, SignedLeft, true);

  switch (pOpExpr->GetOperation())
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
    pLeft ->Write(m_pCpuCtxt, m_pMemCtxt, Right);
    pRight->Write(m_pCpuCtxt, m_pMemCtxt, Left );
    break;
  case OperationExpression::OpSext:
    {
      // FIXME: Handle error case
      u64 Value = 0;
      pLeft->Read(m_pCpuCtxt, m_pMemCtxt, Value, true);
      auto pReadValue = new ConstantExpression(static_cast<u32>(Right * 8), Value);
      pReadValue->SignExtend(pLeft->GetSizeInBit());
      delete pLeft;
      delete pRight;
      return pReadValue;
    }
  default: assert(0);
  }

  u32 Bit = pLeft->GetSizeInBit();
  delete pLeft;
  delete pRight;
  return new ConstantExpression(Bit, Left);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitConstant(ConstantExpression* pConstExpr)
{
  return pConstExpr->Clone();
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIdentifier(IdentifierExpression* pIdExpr)
{
  return pIdExpr->Clone();
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr)
{
  return pTrkIdExpr->Clone();
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitMemory(MemoryExpression* pMemExpr)
{
  Expression* pBaseExprVisited = nullptr;
  if (pMemExpr->GetBaseExpression() != nullptr)
    pBaseExprVisited = pMemExpr->GetBaseExpression()->Visit(this);

  auto pOffsetExprVisited = pMemExpr->GetOffsetExpression()->Visit(this);

  return new MemoryExpression(pMemExpr->GetAccessSizeInBit(), pBaseExprVisited, pOffsetExprVisited, pMemExpr->IsDereferencable());
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitSymbolic(SymbolicExpression* pSymExpr)
{
  // TODO:
  return nullptr;
}