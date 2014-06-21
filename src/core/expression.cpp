#include "medusa/expression.hpp"
#include "medusa/extend.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

MEDUSA_NAMESPACE_USE

/* Usually the main operation is located in the first expression */
Expression* ExpressionVisitor_FindOperation::VisitBind(Expression::List const& rExprList)
{
  if (rExprList.size())
  {
    auto pExpr = *rExprList.begin();

    auto pOperExpr = dynamic_cast<OperationExpression const*>(pExpr);
    if (pOperExpr != nullptr)
      return pOperExpr->Clone();

    pOperExpr = static_cast<OperationExpression const*>(pExpr->Visit(this));
    if (pOperExpr != nullptr)
      return pOperExpr->Clone();
  }

  return nullptr;
}

/* We should have the following form op0 = op0 <operation> op1 or op0 <operation> op1 */
Expression* ExpressionVisitor_FindOperation::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  if (Type != OperationExpression::OpAff)
    return new OperationExpression(static_cast<OperationExpression::Type>(Type), pLeftExpr->Clone(), pRightExpr->Clone());

  auto pOperExpr = dynamic_cast<OperationExpression const*>(pRightExpr);
  if (pOperExpr != nullptr)
    return pOperExpr->Clone();

  return nullptr;
}

/* Usually the main operation is located in the first expression */
Expression* ExpressionVisitor_FindDestination::VisitBind(Expression::List const& rExprList)
{
  if (rExprList.size())
  {
    auto pExpr = (*rExprList.begin())->Visit(this);
    if (pExpr != nullptr)
      return pExpr;
  }

  return nullptr;
}

Expression* ExpressionVisitor_ContainIdentifier::VisitBind(Expression::List const& rExprList)
{
  for (auto itSem = std::begin(rExprList); itSem != std::end(rExprList); ++itSem)
  {
    (*itSem)->Visit(this);
  }
  return nullptr;
}

Expression* ExpressionVisitor_ContainIdentifier::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  pLeftExpr->Visit(this);
  pRightExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor_ContainIdentifier::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  if (Id == m_Id)
    m_Result = true;
  return nullptr;
}

Expression* ExpressionVisitor_ContainIdentifier::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  pOffsetExpr->Visit(this);
  return nullptr;
}

/* We should have the following form op0 = op0 <operation> op1 */
Expression* ExpressionVisitor_FindDestination::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  return pLeftExpr->Clone();
}

BindExpression::BindExpression(Expression::List const& rExprs)
  : m_Expressions(rExprs)
{
}

BindExpression::~BindExpression(void)
{
  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [](Expression *pExpr)
  {
    delete pExpr;
  });
  m_Expressions.clear();
}

std::string BindExpression::ToString(void) const
{
  std::list<std::string> ExprsStrList;

  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [&](Expression *pExpr)
  {
    ExprsStrList.push_back(pExpr->ToString());
  });

  return boost::algorithm::join(ExprsStrList, "; ");
}

Expression *BindExpression::Clone(void) const
{
  Expression::List ExprListCloned;
  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [&](Expression *pExpr)
  {
    ExprListCloned.push_back(pExpr->Clone());
  });

  return new BindExpression(ExprListCloned);
}

ConditionExpression::ConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr)
    : m_Type(CondType), m_pRefExpr(pRefExpr), m_pTestExpr(pTestExpr)
{
}

ConditionExpression::~ConditionExpression(void)
{
  delete m_pRefExpr;
  delete m_pTestExpr;
}

std::string ConditionExpression::ToString(void) const
{
  static const char *s_StrCond[] = { "???", "==", "!=", "u>", "u>=", "u<", "u<=", "s>", "s>=", "s<", "s<=" };
  if (m_pRefExpr == nullptr || m_pTestExpr == nullptr || m_Type >=(sizeof(s_StrCond) / sizeof(*s_StrCond)))
    return "";

  return (boost::format("(%1% %2% %3%)") % m_pRefExpr->ToString() % s_StrCond[m_Type] % m_pTestExpr->ToString()).str();
}

Expression *ConditionExpression::Clone(void) const
{
  return new ConditionExpression(m_Type, m_pRefExpr->Clone(), m_pTestExpr->Clone());
}

IfConditionExpression::IfConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr)
    : ConditionExpression(CondType, pRefExpr, pTestExpr), m_pThenExpr(pThenExpr)
{
}

IfConditionExpression::IfConditionExpression(ConditionExpression* pCondExpr, Expression *pThenExpr) // FIXME: memleak
  : ConditionExpression(
    pCondExpr->GetType(),
    pCondExpr->GetReferenceExpression()->Clone(),
    pCondExpr->GetTestExpression()->Clone()),
  m_pThenExpr(pThenExpr)
{
  delete pCondExpr;
}

IfConditionExpression::~IfConditionExpression(void)
{
  delete m_pThenExpr;
}

std::string IfConditionExpression::ToString(void) const
{
  return (boost::format("if %1% { %2% }") % ConditionExpression::ToString() % m_pThenExpr->ToString()).str();
}

Expression *IfConditionExpression::Clone(void) const
{
  return new IfConditionExpression(m_Type, m_pRefExpr->Clone(), m_pTestExpr->Clone(), m_pThenExpr->Clone());
}

IfElseConditionExpression::IfElseConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr)
  : IfConditionExpression(CondType, pRefExpr, pTestExpr, pThenExpr), m_pElseExpr(pElseExpr)
{
}

IfElseConditionExpression::IfElseConditionExpression(ConditionExpression* pCondExpr, Expression *pThenExpr, Expression *pElseExpr) // FIXME: memleak
  : IfConditionExpression(pCondExpr, pThenExpr), m_pElseExpr(pElseExpr)
{
}

IfElseConditionExpression::~IfElseConditionExpression(void)
{
  delete m_pElseExpr;
}

std::string IfElseConditionExpression::ToString(void) const
{
  return (boost::format("%1% else { %2% }") % IfConditionExpression::ToString() % m_pElseExpr->ToString()).str();
}

Expression *IfElseConditionExpression::Clone(void) const
{
  return new IfElseConditionExpression(m_Type, m_pRefExpr->Clone(), m_pTestExpr->Clone(), m_pThenExpr->Clone(), m_pElseExpr->Clone());
}

WhileConditionExpression::WhileConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr)
  : ConditionExpression(CondType, pRefExpr, pTestExpr), m_pBodyExpr(pBodyExpr)
{
}

WhileConditionExpression::WhileConditionExpression(ConditionExpression* pCondExpr, Expression *pBodyExpr) // FIXME: memleak
  : ConditionExpression(*pCondExpr), m_pBodyExpr(pBodyExpr)
{
}

WhileConditionExpression::~WhileConditionExpression(void)
{
  delete m_pBodyExpr;
}

std::string WhileConditionExpression::ToString(void) const
{
  return (boost::format("while %1% { %2% }") % ConditionExpression::ToString() % m_pBodyExpr->ToString()).str();
}

Expression *WhileConditionExpression::Clone(void) const
{
  return new WhileConditionExpression(m_Type, m_pRefExpr->Clone(), m_pTestExpr->Clone(), m_pBodyExpr->Clone());
}

OperationExpression::OperationExpression(Type OpType, Expression *pLeftExpr, Expression *pRightExpr)
    : m_OpType(OpType), m_pLeftExpr(pLeftExpr), m_pRightExpr(pRightExpr)
{
}

OperationExpression::~OperationExpression(void)
{
  delete m_pLeftExpr;
  delete m_pRightExpr;
}

std::string OperationExpression::ToString(void) const
{
  static const char *s_StrOp[] = { "???", "=", "↔", "&", "|", "^", "<<", ">>", ">>(s)", "+", "-", "*", "/" };

  if (m_pLeftExpr == nullptr || m_pRightExpr == nullptr)
    return "";

  auto LeftStr  = m_pLeftExpr->ToString();
  auto RightStr = m_pRightExpr->ToString();

  if (LeftStr.empty() || RightStr.empty())
    return "";

  if (m_OpType == OpSext)
    return (boost::format("sext_%2%(%1%)") % LeftStr % RightStr).str();

  if (m_OpType >= (sizeof(s_StrOp) / sizeof(*s_StrOp)))
    return "";

  return (boost::format("(%1% %2% %3%)") % LeftStr % s_StrOp[m_OpType] % RightStr).str();
}

Expression *OperationExpression::Clone(void) const
{
  return new OperationExpression(static_cast<Type>(m_OpType), m_pLeftExpr->Clone(), m_pRightExpr->Clone());
}

ConstantExpression::ConstantExpression(u32 ConstType, u64 Value)
  : m_ConstType(ConstType), m_Value(
    ConstType == ConstUnknownBit ||
    ConstType == Const64Bit ?
    Value : (Value & ((1ULL << m_ConstType) - 1)))
{
}

std::string ConstantExpression::ToString(void) const
{
  std::ostringstream Buffer;
  if (m_ConstType != ConstUnknownBit)
    Buffer << "int" << m_ConstType << "(";

  auto TmpValue = m_Value;
  //if (m_Value < 0)
  //{
  //  Buffer << "-";
  //  TmpValue = (~m_Value) + 1;
  //}
  Buffer << "0x" << (boost::format("%x") % TmpValue).str();
  if (m_ConstType != ConstUnknownBit)
    Buffer << ")";
  return Buffer.str();
}

Expression *ConstantExpression::Clone(void) const
{
  return new ConstantExpression(m_ConstType, m_Value);
}

bool ConstantExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue, bool SignExtend) const
{
  rValue = m_Value;
  return true;
}

bool ConstantExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value, bool SignExtend)
{
  assert(0);
  return false;
}

bool ConstantExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const
{
  return false;
}

bool ConstantExpression::SignExtend(u32 NewSizeInBit)
{
  switch (NewSizeInBit)
  {
  case Const8Bit:  m_Value = medusa::SignExtend<s64,  8>(m_Value); break;
  case Const16Bit: m_Value = medusa::SignExtend<s64, 16>(m_Value); break;
  case Const32Bit: m_Value = medusa::SignExtend<s64, 32>(m_Value); break;
  case Const64Bit:                                                 break;
  default: return false;
  }

  switch (m_ConstType)
  {
  case Const8Bit:  m_Value &= 0x000000ff; break;
  case Const16Bit: m_Value &= 0x0000ffff; break;
  case Const32Bit: m_Value &= 0xffffffff; break;
  case Const64Bit:                        break;
  default: return false;
  }

  return true;
}

std::string IdentifierExpression::ToString(void) const
{
  auto pIdName = m_pCpuInfo->ConvertIdentifierToName(m_Id);
  if (pIdName == 0) return "";

  return (boost::format("Id%d(%s)") % m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) % pIdName).str();
}

Expression *IdentifierExpression::Clone(void) const
{
  return new IdentifierExpression(m_Id, m_pCpuInfo);
}

u32 IdentifierExpression::GetSizeInBit(void) const
{
  return m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);
}

bool IdentifierExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue, bool SignExtend) const
{
  rValue = 0;
  u32 RegSize = m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) / 8;
  if (!pCpuCtxt->ReadRegister(m_Id, &rValue, RegSize))
    return false;
  if (SignExtend) switch (RegSize)
  {
    case 1:
      rValue = medusa::SignExtend<s64, 8>(rValue);
      break;
    case 2:
      rValue = medusa::SignExtend<s64, 16>(rValue);
      break;
    case 4:
      rValue = medusa::SignExtend<s64, 32>(rValue);
      break;
  }

  return true;
}

bool IdentifierExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value, bool SignExtend)
{
  return pCpuCtxt->WriteRegister(m_Id, &Value, m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) / 8);
}

bool IdentifierExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const
{
  return false;
}

MemoryExpression::~MemoryExpression(void)
{
  delete m_pExprBase;
  delete m_pExprOffset;
}

std::string MemoryExpression::ToString(void) const
{
  std::string MemType = m_Dereference ? "Mem" : "Addr";
  if (m_pExprBase == nullptr)
    return (boost::format("%s%d(%s)")  % MemType % m_AccessSizeInBit % m_pExprOffset->ToString()).str();

  return (boost::format("%s%d(%s:%s)") % MemType % m_AccessSizeInBit % m_pExprBase->ToString() % m_pExprOffset->ToString()).str();
}

Expression *MemoryExpression::Clone(void) const
{
  if (m_pExprBase == nullptr)
    return new MemoryExpression(m_AccessSizeInBit, nullptr, m_pExprOffset->Clone(), m_Dereference);

  return new MemoryExpression(m_AccessSizeInBit, m_pExprBase->Clone(), m_pExprOffset->Clone(), m_Dereference);
}

u32 MemoryExpression::GetSizeInBit(void) const
{
  return m_AccessSizeInBit;
}

bool MemoryExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue, bool SignExtend) const
{
  Address DstAddr;
  if (GetAddress(pCpuCtxt, pMemCtxt, pVarCtxt, DstAddr) == false)
    return false;

  u64 LinAddr = 0;
  if (pCpuCtxt->Translate(DstAddr, LinAddr) == false)
    LinAddr = DstAddr.GetOffset();

  if (m_Dereference == true)
    return pMemCtxt->ReadMemory(LinAddr, &rValue, m_AccessSizeInBit / 8);

  rValue = LinAddr;
  return true;
}

bool MemoryExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value, bool SignExtend)
{
  assert(m_Dereference == true);
  Address DstAddr;
  if (GetAddress(pCpuCtxt, pMemCtxt, pVarCtxt, DstAddr) == false)
    return false;
  u64 LinAddr = 0;
  if (pCpuCtxt->Translate(DstAddr, LinAddr) == false)
    LinAddr = DstAddr.GetOffset();
  return pMemCtxt->WriteMemory(LinAddr, &Value, m_AccessSizeInBit / 8);
}

bool MemoryExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const
{
  u64 Base = 0, Offset = 0;
  auto pBaseExpr = dynamic_cast<ContextExpression *>(m_pExprBase);
  auto pOffExpr  = dynamic_cast<ContextExpression *>(m_pExprOffset);
  if (pOffExpr == nullptr)
    return false;

  if (pBaseExpr != nullptr)
    if (pBaseExpr->Read(pCpuCtxt, pMemCtxt, pVarCtxt, Base) == false)
      return false;
  if (pOffExpr->Read(pCpuCtxt, pMemCtxt, pVarCtxt, Offset) == false)
    return false;
  rAddress = Address(static_cast<u16>(Base), Offset);
  return true;
}

std::string VariableExpression::ToString(void) const
{
  if (m_Type == 0)
    return m_Name;

  return (boost::format("var%d %s") % m_Type % m_Name).str();
}

Expression* VariableExpression::Clone(void) const
{
  return new VariableExpression(m_Type, m_Name);
}

u32 VariableExpression::GetSizeInBit(void) const
{
  return m_Type;
}

bool VariableExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue, bool SignExtend) const
{
  return pVarCtxt->ReadVariable(m_Name, rValue);
}

bool VariableExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value, bool SignExtend)
{
  return pVarCtxt->WriteVariable(m_Name, Value);
}

bool VariableExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const
{
  return false;
}

Expression* Expr::MakeConst(u32 ConstType, u64 Value)
{
  return new ConstantExpression(ConstType, Value);
}

Expression* Expr::MakeId(u32 Id, CpuInformation const* pCpuInfo)
{
  return new IdentifierExpression(Id, pCpuInfo);
}

Expression* Expr::MakeMem(u32 AccessSize, Expression *pExprBase, Expression *pExprOffset, bool Dereference)
{
  return new MemoryExpression(AccessSize, pExprBase, pExprOffset, Dereference);
}

Expression* Expr::MakeCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr)
{
  return new ConditionExpression(CondType, pRefExpr, pTestExpr);
}

Expression* Expr::MakeIfCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr)
{
  return new IfConditionExpression(CondType, pRefExpr, pTestExpr, pThenExpr);
}

Expression* Expr::MakeIfElseCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr)
{
  return new IfElseConditionExpression(CondType, pRefExpr, pTestExpr, pThenExpr, pElseExpr);
}

Expression* Expr::MakeWhileCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr)
{
  return new WhileConditionExpression(CondType, pRefExpr, pTestExpr, pBodyExpr);
}

Expression* Expr::MakeOp(OperationExpression::Type OpType, Expression *pLeftExpr, Expression *pRightExpr)
{
  return new OperationExpression(OpType, pLeftExpr, pRightExpr);
}

Expression* Expr::MakeBind(Expression::List const& rExprs)
{
  return new BindExpression(rExprs);
}