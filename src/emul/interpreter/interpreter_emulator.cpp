#include "interpreter_emulator.hpp"

MEDUSA_NAMESPACE_USE

InterpreterEmulator::InterpreterEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt) : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
{
}

InterpreterEmulator::~InterpreterEmulator(void)
{
}

void InterpreterEmulator::ReadRegister(u32 Register, void* pValue, u32 ValueSize) const
{
  m_pCpuCtxt->ReadRegister(Register, pValue, ValueSize);
}

void InterpreterEmulator::WriteRegister(u32 Register, void const* pValue, u32 ValueSize)
{
  m_pCpuCtxt->WriteRegister(Register, pValue, ValueSize);
}

void InterpreterEmulator::Execute(Expression const& rExpr)
{
  InterpreterExpressionVisitor Visitor(m_pCpuCtxt, m_pMemCtxt);
  rExpr.Visit(&Visitor);
}

void InterpreterEmulator::Execute(Expression::List const& rExprList)
{
  InterpreterExpressionVisitor Visitor(m_pCpuCtxt, m_pMemCtxt);
  for (auto itExpr = std::begin(rExprList); itExpr != std::end(rExprList); ++itExpr)
  {
    (*itExpr)->Visit(&Visitor);
  }
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
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  return nullptr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)
{
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  pThenExpr->Visit(this);
  return nullptr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr)
{
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  pThenExpr->Visit(this);
  pElseExpr->Visit(this);
  return nullptr;
}

Expression* InterpreterEmulator::InterpreterExpressionVisitor::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  auto pLeft  = dynamic_cast<ContextExpression *>(pLeftExpr->Visit(this));
  auto pRight = dynamic_cast<ContextExpression *>(pRightExpr->Visit(this));

  if (pLeft == nullptr || pRight == nullptr) return nullptr;

  u64 Left, Right;
  pLeft->Read(m_pCpuCtxt, m_pMemCtxt, Left);
  pRight->Read(m_pCpuCtxt, m_pMemCtxt, Right);

  switch (Type)
  {
  case OperationExpression::OpAdd: Left +=  Right; break;
  case OperationExpression::OpSub: Left -=  Right; break;
  case OperationExpression::OpMul: Left *=  Right; break;
  case OperationExpression::OpDiv: Left /=  Right; break;
  case OperationExpression::OpAnd: Left &=  Right; break;
  case OperationExpression::OpOr:  Left |=  Right; break;
  case OperationExpression::OpXor: Left ^=  Right; break;
  case OperationExpression::OpLls: Left <<= Right; break;
  case OperationExpression::OpLrs: Left >>= Right; break;
  case OperationExpression::OpArs: Left = static_cast<s64>(Left) >> Right; break;
  case OperationExpression::OpAff: pLeft->Write(m_pCpuCtxt, m_pMemCtxt, Right); break;
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
  if (pBaseExpr == nullptr)
    return new MemoryExpression(AccessSizeInBit, nullptr, pOffsetExpr->Clone(), Deref);
  return new MemoryExpression(AccessSizeInBit, pBaseExpr->Clone(), pOffsetExpr->Clone(), Deref);
}
