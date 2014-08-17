#include "medusa/expression.hpp"
#include "medusa/extend.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

MEDUSA_NAMESPACE_USE

void Taint::Context::TaintId(u32 Id, Address const& rCurAddr)
{
  m_TaintedId[Id] = rCurAddr;
}

bool Taint::Context::GetTaintAddress(u32 RegId, Address& rTaintedAddress)
{
  auto itTaintedId = m_TaintedId.find(RegId);
  if (itTaintedId == std::end(m_TaintedId))
    return false;
  rTaintedAddress = itTaintedId->second;
  return true;
}

// expression visitor /////////////////////////////////////////////////////////

Expression* ExpressionVisitor::VisitSystem(std::string const& rName)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitBind(Expression::List const& rExprList)
{
  for (auto const pCurExpr : rExprList)
    pCurExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr)
{
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitTernaryCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pTrueExpr, Expression const* pFalseExpr)
{
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  pTrueExpr->Visit(this);
  pFalseExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)
{
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  pThenExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr)
{
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  pThenExpr->Visit(this);
  pElseExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitWhileCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr)
{
  pRefExpr->Visit(this);
  pTestExpr->Visit(this);
  pBodyExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  pLeftExpr->Visit(this);
  pRightExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitConstant(u32 Type, u64 Value)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitTaintedIdentifier(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitSymbolic(u32 Type, std::string const& rValue)
{
  return nullptr;
}

Expression* CloneVisitor::VisitSystem(std::string const& rName)
{
  return new SystemExpression(rName);
}

Expression* CloneVisitor::VisitBind(Expression::List const& rExprList)
{
  return Expr::MakeBind(rExprList);
}

Expression* CloneVisitor::VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr)
{
  return Expr::MakeCond(
    static_cast<ConditionExpression::Type>(Type),
    pRefExpr->Visit(this),
    pTestExpr->Visit(this));
}

Expression* CloneVisitor::VisitTernaryCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pTrueExpr, Expression const* pFalseExpr)
{
  return Expr::MakeTernaryCond(
    static_cast<ConditionExpression::Type>(Type),
    pRefExpr->Visit(this),
    pTestExpr->Visit(this),
    pTrueExpr->Visit(this),
    pFalseExpr->Visit(this));
}

Expression* CloneVisitor::VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)
{
  return Expr::MakeIfCond(
    static_cast<ConditionExpression::Type>(Type),
    pRefExpr->Visit(this),
    pTestExpr->Visit(this),
    pThenExpr->Visit(this));
}

Expression* CloneVisitor::VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr)
{
  return Expr::MakeIfElseCond(
    static_cast<ConditionExpression::Type>(Type),
    pRefExpr->Visit(this),
    pTestExpr->Visit(this),
    pThenExpr->Visit(this),
    pElseExpr->Visit(this));
}

Expression* CloneVisitor::VisitWhileCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr)
{
  return Expr::MakeWhileCond(
    static_cast<ConditionExpression::Type>(Type),
    pRefExpr->Visit(this),
    pTestExpr->Visit(this),
    pBodyExpr->Visit(this));
}

Expression* CloneVisitor::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  return Expr::MakeOp(
    static_cast<OperationExpression::Type>(Type),
    pLeftExpr->Visit(this),
    pRightExpr->Visit(this));
}

Expression* CloneVisitor::VisitConstant(u32 Type, u64 Value)
{
  return Expr::MakeConst(
    static_cast<ConstantExpression::Type>(Type),
    Value);
}

Expression* CloneVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  return Expr::MakeId(Id, pCpuInfo);
}

Expression* CloneVisitor::VisitTaintedIdentifier(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr)
{
  return new TaintedIdentifierExpression(Id, pCpuInfo, rCurAddr);
}

Expression* CloneVisitor::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  return Expr::MakeMem(
    AccessSizeInBit,
    pBaseExpr != nullptr ? pBaseExpr->Visit(this) : nullptr,
    pOffsetExpr->Visit(this),
    Deref);
}

Expression* CloneVisitor::VisitSymbolic(u32 Type, std::string const& rValue)
{
  return Expr::MakeSym(
    static_cast<SymbolicExpression::Type>(Type),
    rValue);
}

TaintVisitor::TaintVisitor(Address const& rCurAddr, Taint::Context& rCtxt)
  : m_CurAddr(rCurAddr), m_rCtxt(rCtxt), m_pTaintedExpr(nullptr), m_IsAssigned(false)
{
}

Expression* TaintVisitor::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  m_IsAssigned = true;
  auto pTaintedLeftExpr = pLeftExpr->Visit(this);
  m_IsAssigned = false;
  auto pTaintedRightExpr = pRightExpr->Visit(this);

  return Expr::MakeOp(
    static_cast<OperationExpression::Type>(Type),
    pTaintedLeftExpr,
    pTaintedRightExpr);
}

Expression* TaintVisitor::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  Address TaintedAddress;

  if (m_IsAssigned)
  {
    TaintedAddress = m_CurAddr;
    m_rCtxt.TaintId(Id, m_CurAddr);
  }
  else
  {
    if (!m_rCtxt.GetTaintAddress(Id, TaintedAddress))
      return nullptr;
  }

  return new TaintedIdentifierExpression(Id, pCpuInfo, TaintedAddress);
}

// system expression //////////////////////////////////////////////////////////

SystemExpression::SystemExpression(std::string const& rName)
  : m_Name(rName)
{
}

SystemExpression::~SystemExpression(void)
{
}

std::string SystemExpression::ToString(void) const
{
  return m_Name;
}

Expression *SystemExpression::Clone(void) const
{
  return new SystemExpression(m_Name);
}

// bind expression ////////////////////////////////////////////////////////////

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

// condition expression ///////////////////////////////////////////////////////

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

TernaryConditionExpression::TernaryConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr)
  : ConditionExpression(CondType, pRefExpr, pTestExpr), m_pTrueExpr(pTrueExpr), m_pFalseExpr(pFalseExpr)
{
}

TernaryConditionExpression::~TernaryConditionExpression(void)
{
  delete m_pTrueExpr;
  delete m_pFalseExpr;
}

std::string TernaryConditionExpression::ToString(void) const
{
  std::string Result = ConditionExpression::ToString();
  Result += " ? (";
  Result += m_pTrueExpr->ToString();
  Result += ") : ";
  Result += m_pFalseExpr->ToString();
  Result += ")";

  return Result;
}

Expression *TernaryConditionExpression::Clone(void) const
{
  return new TernaryConditionExpression(m_Type, m_pRefExpr->Clone(), m_pTestExpr->Clone(), m_pTrueExpr->Clone(), m_pFalseExpr->Clone());
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

// operation expression ///////////////////////////////////////////////////////

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

// constant expression ////////////////////////////////////////////////////////

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

bool ConstantExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend) const
{
  rValue = m_Value;
  return true;
}

bool ConstantExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend)
{
  assert(0);
  return false;
}

bool ConstantExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
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

// identifier expression //////////////////////////////////////////////////////

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

bool IdentifierExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend) const
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

bool IdentifierExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend)
{
  return pCpuCtxt->WriteRegister(m_Id, &Value, m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) / 8);
}

bool IdentifierExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
{
  return false;
}

TaintedIdentifierExpression::~TaintedIdentifierExpression(void)
{
}

std::string TaintedIdentifierExpression::ToString(void) const
{
  auto pIdName = m_pCpuInfo->ConvertIdentifierToName(m_Id);
  if (pIdName == 0) return "";

  return (boost::format("Id%d(%s)[%s]") % m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) % pIdName % m_CurAddr.ToString()).str();
}

Expression *TaintedIdentifierExpression::Clone(void) const
{
  return new TaintedIdentifierExpression(m_Id, m_pCpuInfo, m_CurAddr);
}

MemoryExpression::~MemoryExpression(void)
{
  delete m_pExprBase;
  delete m_pExprOffset;
}

// memory expression //////////////////////////////////////////////////////////

std::string MemoryExpression::ToString(void) const
{
  auto const pMemType = m_Dereference ? "Mem" : "Addr";
  if (m_pExprBase == nullptr)
    return (boost::format("%s%d(%s)")  % pMemType % m_AccessSizeInBit % m_pExprOffset->ToString()).str();

  return (boost::format("%s%d(%s:%s)") % pMemType % m_AccessSizeInBit % m_pExprBase->ToString() % m_pExprOffset->ToString()).str();
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

bool MemoryExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend) const
{
  Address DstAddr;
  if (GetAddress(pCpuCtxt, pMemCtxt, DstAddr) == false)
    return false;

  u64 LinAddr = 0;
  if (pCpuCtxt->Translate(DstAddr, LinAddr) == false)
    LinAddr = DstAddr.GetOffset();

  if (m_Dereference == true)
    return pMemCtxt->ReadMemory(LinAddr, &rValue, m_AccessSizeInBit / 8);

  rValue = LinAddr;
  return true;
}

bool MemoryExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend)
{
  assert(m_Dereference == true);
  Address DstAddr;
  if (GetAddress(pCpuCtxt, pMemCtxt, DstAddr) == false)
    return false;
  u64 LinAddr = 0;
  if (pCpuCtxt->Translate(DstAddr, LinAddr) == false)
    LinAddr = DstAddr.GetOffset();
  return pMemCtxt->WriteMemory(LinAddr, &Value, m_AccessSizeInBit / 8);
}

bool MemoryExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
{
  u64 Base = 0, Offset = 0;
  auto pBaseExpr = dynamic_cast<ContextExpression *>(m_pExprBase);
  auto pOffExpr  = dynamic_cast<ContextExpression *>(m_pExprOffset);
  if (pOffExpr == nullptr)
    return false;

  if (pBaseExpr != nullptr)
    if (pBaseExpr->Read(pCpuCtxt, pMemCtxt, Base) == false)
      return false;
  if (pOffExpr->Read(pCpuCtxt, pMemCtxt, Offset) == false)
    return false;
  rAddress = Address(static_cast<u16>(Base), Offset);
  return true;
}

// symbolic expression ////////////////////////////////////////////////////////

std::string SymbolicExpression::ToString(void) const
{
  static char const* TypeToStr[] = { "unknown", "retval", "parm", "undef" };
  if (m_Type > Undefined)
    return "<invalid symbolic expression>";
  return (boost::format("sym(%1, %2)") % TypeToStr[m_Type] % m_Value).str();
}

Expression* SymbolicExpression::Clone(void) const
{
  return new SymbolicExpression(m_Type, m_Value);
}

u32 SymbolicExpression::GetSizeInBit(void) const
{
  return 0;
}

Expression* SymbolicExpression::Visit(ExpressionVisitor *pVisitor) const
{
  return pVisitor->VisitSymbolic(m_Type, m_Value);
}

bool SymbolicExpression::SignExtend(u32 NewSizeInBit)
{
  return false;
}

// helper /////////////////////////////////////////////////////////////////////

Expression* Expr::MakeConst(u32 ConstType, u64 Value)
{
  return new ConstantExpression(ConstType, Value);
}

Expression* Expr::MakeBoolean(bool Value)
{
  return new ConstantExpression(ConstantExpression::Const1Bit, Value ? 1 : 0);
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

Expression* Expr::MakeTernaryCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr)
{
  return new TernaryConditionExpression(CondType, pRefExpr, pTestExpr, pTrueExpr, pFalseExpr);
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

Expression* Expr::MakeSym(SymbolicExpression::Type SymType, std::string const& rValue)
{
  return new SymbolicExpression(SymType, rValue);
}