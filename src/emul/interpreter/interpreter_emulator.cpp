#include "interpreter_emulator.hpp"
#include <medusa/log.hpp>

MEDUSA_NAMESPACE_USE

InterpreterEmulator::InterpreterEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
: Emulator(pCpuInfo, pCpuCtxt, pMemCtxt, new VariableContext)
{
}

InterpreterEmulator::~InterpreterEmulator(void)
{
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression const& rExpr)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt);
  auto pCurExpr = rExpr.Visit(&Visitor);

  if (pCurExpr == nullptr)
    return false;

  auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
  auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
  u64 CurPc  = 0;
  m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
  TestHook(Address(CurPc), Emulator::HookOnExecute);
  delete pCurExpr;
  return true;
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::List const& rExprList)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt);
  for (auto itExpr = std::begin(rExprList); itExpr != std::end(rExprList); ++itExpr)
  {
    Log::Write("emul_interpreter") << (*itExpr)->ToString() << LogEnd;
    auto pCurExpr = (*itExpr)->Visit(&Visitor);
    if (pCurExpr == nullptr)
      return false;

    auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
    auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc) / 8;
    u64 CurPc = 0;
    m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
    TestHook(Address(CurPc), Emulator::HookOnExecute);
    delete pCurExpr;
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
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Ref);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Test);
  delete pRef;
  delete pTest;

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
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Ref);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Test);
  delete pRef;
  delete pTest;

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

  u64 Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Ref);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Test);
  delete pRef;
  delete pTest;

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

  u64 Ref, Test;
  pRef->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Ref);
  pTest->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Test);
  delete pRef;
  delete pTest;

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

  u64 Left = 0, Right;
  if (Type != OperationExpression::OpAff) /* OpAff doesn't require us to read left operand */
    if (pLeft ->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Left) == false)
    {
      delete pLeft;
      delete pRight;
      return nullptr;
    }

  if (pRight->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Right) == false)
  {
    delete pLeft;
    delete pRight;
    return nullptr;
  }

  Address LeftAddress, RightAddress;
  if (pLeft->GetAddress(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, LeftAddress) == true)
  {
    auto itHook = m_rHooks.find(LeftAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnWrite)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  if (pRight->GetAddress(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, RightAddress) == true)
  {
    auto itHook = m_rHooks.find(RightAddress);
    if (itHook != std::end(m_rHooks) && itHook->second.m_Type & Emulator::HookOnRead)
      itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt);
  }

  switch (Type)
  {
  case OperationExpression::OpAdd:   Left +=  Right; break;
  case OperationExpression::OpSub:   Left -=  Right; break;
  case OperationExpression::OpMul:   Left *=  Right; break;
  case OperationExpression::OpUDiv:
  case OperationExpression::OpSDiv:  Left /=  Right; break;
  case OperationExpression::OpAnd:   Left &=  Right; break;
  case OperationExpression::OpOr:    Left |=  Right; break;
  case OperationExpression::OpXor:   Left ^=  Right; break;
  case OperationExpression::OpLls:   Left <<= Right; break;
  case OperationExpression::OpLrs:   Left >>= Right; break;
  case OperationExpression::OpArs:   Left = static_cast<s64>(Left) >> Right; break;
  case OperationExpression::OpAff:   pLeft->Write(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Right); break;
  case OperationExpression::OpXchg:
    pLeft ->Write(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Right);
    pRight->Write(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Left );
    break;
  case OperationExpression::OpSext:
    {
      // FIXME: Handle error case
      u64 Value = 0;
      pLeft->Read(m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt, Value);
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

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitVariable(u32 Type, std::string const& rName)
{
  if (Type && m_pVarCtxt->AllocateVariable(Type, rName) == false)
    return nullptr;
  return new VariableExpression(Type, rName);
}