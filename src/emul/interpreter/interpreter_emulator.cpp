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

bool InterpreterEmulator::Execute(Address const& rAddress, Expression const& rExpr)
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

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitSystem(std::string const& rName)
{
  return nullptr; // TODO
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(Expression::List const& rExprList)
{
  Expression::List SmplExprList;
  for (Expression* pExpr : rExprList)
    SmplExprList.push_back(pExpr->Visit(this));
  return new BindExpression(SmplExprList);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

  if (pRef == nullptr || pTest == nullptr) return nullptr;

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
  switch (Type)
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

  return new ConstantExpression(ConstantExpression::Const1Bit, Cond);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitTernaryCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pTrueExpr, Expression const* pFalseExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

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
  switch (Type)
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

  return (Cond ? pTrueExpr->Clone() : pFalseExpr->Clone());
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

  if (pRef == nullptr || pTest == nullptr) return nullptr;

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
  switch (Type)
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

  auto pExpr = Cond == true ? pThenExpr->Clone() : new ConstantExpression(ConstantExpression::Const1Bit, Cond); // what are we supposed to return?
  if (pExpr != nullptr)
  {
    auto pStmtExpr = pExpr->Visit(this);
    delete pStmtExpr;
  }
  return pExpr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

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
  switch (Type)
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

  auto pExpr = (Cond ? pThenExpr->Clone() : pElseExpr->Clone());
  if (pExpr != nullptr)
  {
    auto pStmtExpr = pExpr->Visit(this);
    delete pStmtExpr;
  }
  return pExpr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitWhileCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

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
  switch (Type)
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

  auto pExpr = Cond == true ? pBodyExpr->Clone() : nullptr;
  if (pExpr != nullptr)
  {
    auto pStmtExpr = pExpr->Visit(this);
    delete pStmtExpr;
  }
  return pExpr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  auto pLeft  = dynamic_cast<ContextExpression *>(pLeftExpr->Visit(this));
  auto pRight = dynamic_cast<ContextExpression *>(pRightExpr->Visit(this));

  if (pLeft == nullptr || pRight == nullptr)
  {
    delete pLeft;
    delete pRight;
    return nullptr;
  }

  u64 Left = 0, Right = 0;
  if (Type != OperationExpression::OpAff) /* OpAff doesn't require us to read left operand */
    if (pLeft ->Read(m_pCpuCtxt, m_pMemCtxt, Left) == false)
    {
      delete pLeft;
      delete pRight;
      return nullptr;
    }

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

  switch (Type)
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
  case OperationExpression::OpAff:   pLeft->Write(m_pCpuCtxt, m_pMemCtxt, Right); break;
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

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitConstant(u32 Type, u64 Value)
{
  return new ConstantExpression(Type, Value);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  return new IdentifierExpression(Id, pCpuInfo);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  Expression* pBaseExprVisited = nullptr;
  if (pBaseExpr != nullptr)
    pBaseExprVisited = pBaseExpr->Visit(this);

  auto pOffsetExprVisited = pOffsetExpr->Visit(this);

  return new MemoryExpression(AccessSizeInBit, pBaseExprVisited, pOffsetExprVisited, Deref);
}