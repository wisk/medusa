#include "interpreter_emulator.hpp"
#include <medusa/log.hpp>

MEDUSA_NAMESPACE_USE

InterpreterEmulator::InterpreterEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt) : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
{
}

InterpreterEmulator::~InterpreterEmulator(void)
{
}

bool InterpreterEmulator::Execute(Expression const& rExpr)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);
  if (rExpr.Visit(&Visitor) == nullptr)
    return false;

  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
  TestHook(Address(CurPc), Emulator::HookOnExecute);
  return true;
}

bool InterpreterEmulator::Execute(Expression::List const& rExprList)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt);
  for (auto itExpr = std::begin(rExprList); itExpr != std::end(rExprList); ++itExpr)
  {
    //Log::Write("interpreter") << "* exec: " << (*itExpr)->ToString() << LogEnd;
    if ((*itExpr)->Visit(&Visitor) == false)
      return false;

    auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
    auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
    u64 CurPc = 0;
    m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
    TestHook(Address(CurPc), Emulator::HookOnExecute);
  }
  return true;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(Expression::List const& rExprList)
{
  Expression::List SmplExprList;
  for (auto itExpr = std::begin(rExprList); itExpr != std::end(rExprList); ++itExpr)
    SmplExprList.push_back((*itExpr)->Visit(this));
  return new BindExpression(SmplExprList);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

  if (pRef == nullptr || pTest == nullptr) return nullptr;

  u64 Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, Test);

  bool Cond = false;
  switch (Type)
  {
  case ConditionExpression::CondEq: Cond = (Ref == Test) ? true : false; break;
  case ConditionExpression::CondNe: Cond = (Ref != Test) ? true : false; break;
  case ConditionExpression::CondUgt:Cond = (Ref >  Test) ? true : false; break;
  case ConditionExpression::CondUge:Cond = (Ref >= Test) ? true : false; break;
  case ConditionExpression::CondUlt:Cond = (Ref <  Test) ? true : false; break;
  case ConditionExpression::CondUle:Cond = (Ref <= Test) ? true : false; break;
  case ConditionExpression::CondSgt:Cond = (Ref >  Test) ? true : false; break; //
  case ConditionExpression::CondSge:Cond = (Ref >= Test) ? true : false; break; // TODO:
  case ConditionExpression::CondSlt:Cond = (Ref <  Test) ? true : false; break; // Handle signed expression
  case ConditionExpression::CondSle:Cond = (Ref <= Test) ? true : false; break; //
  }

  return new ConstantExpression(ConstantExpression::Const1Bit, Cond);
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

  if (pRef == nullptr || pTest == nullptr) return nullptr;

  u64 Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, Test);

  bool Cond = false;
  switch (Type)
  {
  case ConditionExpression::CondEq: Cond = (Ref == Test) ? true : false; break;
  case ConditionExpression::CondNe: Cond = (Ref != Test) ? true : false; break;
  case ConditionExpression::CondUgt:Cond = (Ref >  Test) ? true : false; break;
  case ConditionExpression::CondUge:Cond = (Ref >= Test) ? true : false; break;
  case ConditionExpression::CondUlt:Cond = (Ref <  Test) ? true : false; break;
  case ConditionExpression::CondUle:Cond = (Ref <= Test) ? true : false; break;
  case ConditionExpression::CondSgt:Cond = (Ref >  Test) ? true : false; break; //
  case ConditionExpression::CondSge:Cond = (Ref >= Test) ? true : false; break; // TODO:
  case ConditionExpression::CondSlt:Cond = (Ref <  Test) ? true : false; break; // Handle signed expression
  case ConditionExpression::CondSle:Cond = (Ref <= Test) ? true : false; break; //
  }

  auto pExpr = Cond == true ? pThenExpr->Clone() : nullptr;
  if (pExpr != nullptr)
    pExpr->Visit(this);
  return pExpr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr)
{
  auto pRef  = dynamic_cast<ContextExpression *>(pRefExpr->Visit(this));
  auto pTest = dynamic_cast<ContextExpression *>(pTestExpr->Visit(this));

  if (pRef == nullptr || pTest == nullptr) return nullptr;

  u64 Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, Ref);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, Test);

  bool Cond = false;
  switch (Type)
  {
  case ConditionExpression::CondEq: Cond = (Ref == Test) ? true : false;
  case ConditionExpression::CondNe: Cond = (Ref != Test) ? true : false;
  case ConditionExpression::CondUgt:Cond = (Ref >  Test) ? true : false;
  case ConditionExpression::CondUge:Cond = (Ref >= Test) ? true : false;
  case ConditionExpression::CondUlt:Cond = (Ref <  Test) ? true : false;
  case ConditionExpression::CondUle:Cond = (Ref <= Test) ? true : false;
  case ConditionExpression::CondSgt:Cond = (Ref >  Test) ? true : false; //
  case ConditionExpression::CondSge:Cond = (Ref >= Test) ? true : false; // TODO:
  case ConditionExpression::CondSlt:Cond = (Ref <  Test) ? true : false; // Handle signed expression
  case ConditionExpression::CondSle:Cond = (Ref <= Test) ? true : false; //
  }

  auto pExpr = Cond == true ? pThenExpr->Clone() : pElseExpr->Clone();
  if (pExpr != nullptr)
    pExpr->Visit(this);
  return pExpr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  auto pLeft  = dynamic_cast<ContextExpression *>(pLeftExpr->Visit(this));
  auto pRight = dynamic_cast<ContextExpression *>(pRightExpr->Visit(this));

  if (pLeft == nullptr || pRight == nullptr) return nullptr;

  u64 Left = 0, Right;
  if (Type != OperationExpression::OpAff) /* OpAff doesn't require us to read left operand */
    if (pLeft ->Read(m_pCpuCtxt, m_pMemCtxt, Left) == false)
      return nullptr;

  if (pRight->Read(m_pCpuCtxt, m_pMemCtxt, Right) == false)
    return nullptr;

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

  switch (Type)
  {
  case OperationExpression::OpAdd:  Left +=  Right; break;
  case OperationExpression::OpSub:  Left -=  Right; break;
  case OperationExpression::OpMul:  Left *=  Right; break;
  case OperationExpression::OpDiv:  Left /=  Right; break;
  case OperationExpression::OpAnd:  Left &=  Right; break;
  case OperationExpression::OpOr:   Left |=  Right; break;
  case OperationExpression::OpXor:  Left ^=  Right; break;
  case OperationExpression::OpLls:  Left <<= Right; break;
  case OperationExpression::OpLrs:  Left >>= Right; break;
  case OperationExpression::OpArs:  Left = static_cast<s64>(Left) >> Right; break;
  case OperationExpression::OpAff:  pLeft->Write(m_pCpuCtxt, m_pMemCtxt, Right); break;
  case OperationExpression::OpXchg:
    pLeft ->Write(m_pCpuCtxt, m_pMemCtxt, Right);
    pRight->Write(m_pCpuCtxt, m_pMemCtxt, Left );
    break;
  default: assert(0);
  }

  delete pLeft;
  delete pRight;
  return new ConstantExpression(0, Left);
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
