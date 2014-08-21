#include "medusa/expression.hpp"
#include "medusa/extend.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

MEDUSA_NAMESPACE_USE

void Track::Context::TrackId(u32 Id, Address const& rCurAddr)
{
  m_TrackedId[Id] = rCurAddr;
}

bool Track::Context::GetTrackAddress(u32 RegId, Address& rTrackedAddress)
{
  auto itTrackedId = m_TrackedId.find(RegId);
  if (itTrackedId == std::end(m_TrackedId))
    return false;
  rTrackedAddress = itTrackedId->second;
  return true;
}

// system expression //////////////////////////////////////////////////////////

SystemExpression::SystemExpression(std::string const& rName)
: Expression(Expression::Unknown), m_Name(rName)
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

Expression* SystemExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitSystem(this);
}

// bind expression ////////////////////////////////////////////////////////////

BindExpression::BindExpression(Expression::List const& rExprs)
: Expression(Expression::Bind), m_Expressions(rExprs)
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

Expression* BindExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitBind(this);
}

bool BindExpression::UpdateChild(Expression* pOldExpr, Expression* pNewExpr)
{
  return false; // TODO
}

// condition expression ///////////////////////////////////////////////////////

ConditionExpression::ConditionExpression(Expression::Kind ExprKind, Type CondType, Expression *pRefExpr, Expression *pTestExpr)
: Expression(ExprKind), m_Type(CondType), m_pRefExpr(pRefExpr), m_pTestExpr(pTestExpr)
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
  if (m_pRefExpr == nullptr || m_pTestExpr == nullptr || m_Type >= (sizeof(s_StrCond) / sizeof(*s_StrCond)))
    return "";

  return (boost::format("(%1% %2% %3%)") % m_pRefExpr->ToString() % s_StrCond[m_Type] % m_pTestExpr->ToString()).str();
}

TernaryConditionExpression::TernaryConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr)
: ConditionExpression(Expression::TernaryCond, CondType, pRefExpr, pTestExpr), m_pTrueExpr(pTrueExpr), m_pFalseExpr(pFalseExpr)
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

Expression* TernaryConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitTernaryCondition(this);
}

bool TernaryConditionExpression::UpdateChild(Expression* pOldExpr, Expression* pNewExpr)
{
  return false; // TODO
}

IfElseConditionExpression::IfElseConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr)
: ConditionExpression(Expression::IfElseCond, CondType, pRefExpr, pTestExpr), m_pThenExpr(pThenExpr), m_pElseExpr(pElseExpr)
{
}

IfElseConditionExpression::~IfElseConditionExpression(void)
{
  delete m_pThenExpr;
  delete m_pElseExpr;
}

std::string IfElseConditionExpression::ToString(void) const
{
  return (boost::format("if %1% { %2% } else { %3% }") % ConditionExpression::ToString() % m_pThenExpr->ToString() % m_pElseExpr->ToString()).str();
}

Expression *IfElseConditionExpression::Clone(void) const
{
  return new IfElseConditionExpression(m_Type, m_pRefExpr->Clone(), m_pTestExpr->Clone(), m_pThenExpr->Clone(), m_pElseExpr->Clone());
}

Expression* IfElseConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitIfElseCondition(this);
}

bool IfElseConditionExpression::UpdateChild(Expression* pOldExpr, Expression* pNewExpr)
{
  return false; // TODO
}

WhileConditionExpression::WhileConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr)
: ConditionExpression(Expression::WhileCond, CondType, pRefExpr, pTestExpr), m_pBodyExpr(pBodyExpr)
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

Expression* WhileConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitWhileCondition(this);
}

bool WhileConditionExpression::UpdateChild(Expression* pOldExpr, Expression* pNewExpr)
{
  return false; // TODO
}

// operation expression ///////////////////////////////////////////////////////

AssignmentExpression::AssignmentExpression(Expression *pDstExpr, Expression *pSrcExpr)
: Expression(Expression::Assign), m_pDstExpr(pDstExpr), m_pSrcExpr(pSrcExpr)
{
}

AssignmentExpression::~AssignmentExpression(void)
{
  delete m_pDstExpr;
  delete m_pSrcExpr;
}

std::string AssignmentExpression::ToString(void) const
{
  return (boost::format("(%1% = %2%)") % m_pDstExpr->ToString() % m_pSrcExpr->ToString()).str();
}

Expression *AssignmentExpression::Clone(void) const
{
  return new AssignmentExpression(m_pDstExpr->Clone(), m_pSrcExpr->Clone());
}

Expression* AssignmentExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitAssignment(this);
}

bool AssignmentExpression::UpdateChild(Expression* pOldExpr, Expression* pNewExpr)
{
  return false; // TODO
}

OperationExpression::OperationExpression(Type OpType, Expression *pLeftExpr, Expression *pRightExpr)
: Expression(Expression::Op), m_OpType(OpType), m_pLeftExpr(pLeftExpr), m_pRightExpr(pRightExpr)
{
}

OperationExpression::~OperationExpression(void)
{
  delete m_pLeftExpr;
  delete m_pRightExpr;
}

std::string OperationExpression::ToString(void) const
{
  static const char *s_StrOp[] = { "???", "↔", "&", "|", "^", "<<", ">>", ">>(s)", "+", "-", "*", "/" };

  if (m_pLeftExpr == nullptr || m_pRightExpr == nullptr)
    return "";

  auto LeftStr = m_pLeftExpr->ToString();
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

Expression* OperationExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitOperation(this);
}

bool OperationExpression::UpdateChild(Expression* pOldExpr, Expression* pNewExpr)
{
  if (m_pLeftExpr == pOldExpr)
  {
    m_pLeftExpr = pNewExpr;
    return true;
  }
  if (m_pRightExpr == pOldExpr)
  {
    m_pRightExpr = pNewExpr;
    return true;
  }

  return false;
}

// constant expression ////////////////////////////////////////////////////////

ConstantExpression::ConstantExpression(u32 ConstType, u64 Value)
: ContextExpression(Expression::Const)
, m_ConstType(ConstType), m_Value(
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

Expression* ConstantExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitConstant(this);
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
  case Const8Bit:  m_Value = medusa::SignExtend<s64, 8>(m_Value); break;
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

IdentifierExpression::IdentifierExpression(u32 Id, CpuInformation const* pCpuInfo)
: ContextExpression(Expression::Id), m_Id(Id), m_pCpuInfo(pCpuInfo) {}

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

Expression* IdentifierExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitIdentifier(this);
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

TrackedIdentifierExpression::TrackedIdentifierExpression(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr)
: Expression(Expression::TrackedId)
, m_Id(Id), m_pCpuInfo(pCpuInfo), m_CurAddr(rCurAddr) {}

TrackedIdentifierExpression::~TrackedIdentifierExpression(void)
{
}

std::string TrackedIdentifierExpression::ToString(void) const
{
  auto pIdName = m_pCpuInfo->ConvertIdentifierToName(m_Id);
  if (pIdName == 0) return "";

  return (boost::format("Id%d(%s)[%s]") % m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) % pIdName % m_CurAddr.ToString()).str();
}

Expression *TrackedIdentifierExpression::Clone(void) const
{
  return new TrackedIdentifierExpression(m_Id, m_pCpuInfo, m_CurAddr);
}

u32 TrackedIdentifierExpression::GetSizeInBit(void) const
{
  return m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);
}

Expression* TrackedIdentifierExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitTrackedIdentifier(this);
}

// memory expression //////////////////////////////////////////////////////////

MemoryExpression::MemoryExpression(u32 AccessSize, Expression *pExprBase, Expression *pExprOffset, bool Dereference)
: ContextExpression(Expression::Mem)
, m_AccessSizeInBit(AccessSize), m_pExprBase(pExprBase), m_pExprOffset(pExprOffset), m_Dereference(Dereference)
{
  assert(pExprOffset != nullptr);
}

MemoryExpression::~MemoryExpression(void)
{
  delete m_pExprBase;
  delete m_pExprOffset;
}

std::string MemoryExpression::ToString(void) const
{
  auto const pMemType = m_Dereference ? "Mem" : "Addr";
  if (m_pExprBase == nullptr)
    return (boost::format("%s%d(%s)") % pMemType % m_AccessSizeInBit % m_pExprOffset->ToString()).str();

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

Expression* MemoryExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitMemory(this);
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
  auto pOffExpr = dynamic_cast<ContextExpression *>(m_pExprOffset);
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

bool MemoryExpression::UpdateChild(Expression* pOldExpr, Expression* pNewExpr)
{
  return false; // TODO
}

// symbolic expression ////////////////////////////////////////////////////////

SymbolicExpression::SymbolicExpression(SymbolicExpression::Type SymType, std::string const& rValue)
: Expression(Expression::Sym), m_Type(SymType), m_Value(rValue) {}

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

bool SymbolicExpression::SignExtend(u32 NewSizeInBit)
{
  return false;
}

Expression* SymbolicExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitSymbolic(this);
}

// expression visitor /////////////////////////////////////////////////////////

Expression* ExpressionVisitor::VisitSystem(SystemExpression* pSysExpr)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitBind(BindExpression* pBindExpr)
{
  auto& rExprs = pBindExpr->GetBoundExpressions();
  for (auto pExpr : rExprs)
    pExpr->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression* pTernExpr)
{
  pTernExpr->GetReferenceExpression()->Visit(this);
  pTernExpr->GetTestExpression()->Visit(this);
  pTernExpr->GetTrueExpression()->Visit(this);
  pTernExpr->GetFalseExpression()->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression* pIfElseExpr)
{
  pIfElseExpr->GetReferenceExpression()->Visit(this);
  pIfElseExpr->GetTestExpression()->Visit(this);
  pIfElseExpr->GetThenExpression()->Visit(this);
  pIfElseExpr->GetElseExpression()->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitWhileCondition(WhileConditionExpression* pWhileExpr)
{
  pWhileExpr->GetReferenceExpression()->Visit(this);
  pWhileExpr->GetTestExpression()->Visit(this);
  pWhileExpr->GetBodyExpression()->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitAssignment(AssignmentExpression* pAssignExpr)
{
  pAssignExpr->GetDestinationExpression()->Visit(this);
  pAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitOperation(OperationExpression* pOpExpr)
{
  pOpExpr->GetLeftExpression()->Visit(this);
  pOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitConstant(ConstantExpression* pConstExpr)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitIdentifier(IdentifierExpression* pIdExpr)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr)
{
  return nullptr;
}

Expression* ExpressionVisitor::VisitMemory(MemoryExpression* pMemExpr)
{
  if (pMemExpr->GetBaseExpression() != nullptr)
    pMemExpr->GetBaseExpression()->Visit(this);
  pMemExpr->GetOffsetExpression()->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor::VisitSymbolic(SymbolicExpression* pSymExpr)
{
  return nullptr;
}

Expression* CloneVisitor::VisitSystem(SystemExpression* pSysExpr)
{
  return pSysExpr->Clone();
}

Expression* CloneVisitor::VisitBind(BindExpression* pBindExpr)
{
  Expression::List Exprs;

  for (auto pExpr : pBindExpr->GetBoundExpressions())
    Exprs.push_back(pExpr->Visit(this));

  return Expr::MakeBind(Exprs);
}

Expression* CloneVisitor::VisitTernaryCondition(TernaryConditionExpression* pTernExpr)
{
  return Expr::MakeTernaryCond(
    pTernExpr->GetType(),
    pTernExpr->GetReferenceExpression()->Visit(this),
    pTernExpr->GetTestExpression()->Visit(this),
    pTernExpr->GetTrueExpression()->Visit(this),
    pTernExpr->GetFalseExpression()->Visit(this));
}

Expression* CloneVisitor::VisitIfElseCondition(IfElseConditionExpression* pIfElseExpr)
{
  return Expr::MakeIfElseCond(
    pIfElseExpr->GetType(),
    pIfElseExpr->GetReferenceExpression()->Visit(this),
    pIfElseExpr->GetTestExpression()->Visit(this),
    pIfElseExpr->GetThenExpression()->Visit(this),
    pIfElseExpr->GetElseExpression()->Visit(this));
}

Expression* CloneVisitor::VisitWhileCondition(WhileConditionExpression* pWhileExpr)
{
  return Expr::MakeWhileCond(
    pWhileExpr->GetType(),
    pWhileExpr->GetReferenceExpression(),
    pWhileExpr->GetTestExpression(),
    pWhileExpr->GetBodyExpression());
}

Expression* CloneVisitor::VisitAssignment(AssignmentExpression* pAssignExpr)
{
  return Expr::MakeAssign(
    pAssignExpr->GetDestinationExpression()->Visit(this),
    pAssignExpr->GetSourceExpression()->Visit(this));
}

Expression* CloneVisitor::VisitOperation(OperationExpression* pOpExpr)
{
  return Expr::MakeOp(
    static_cast<OperationExpression::Type>(pOpExpr->GetOperation()),
    pOpExpr->GetLeftExpression()->Visit(this),
    pOpExpr->GetRightExpression()->Visit(this));
}

Expression* CloneVisitor::VisitConstant(ConstantExpression* pConstExpr)
{
  return Expr::MakeConst(
    pConstExpr->GetType(),
    pConstExpr->GetConstant());
}

Expression* CloneVisitor::VisitIdentifier(IdentifierExpression* pIdExpr)
{
  return Expr::MakeId(
    pIdExpr->GetId(),
    pIdExpr->GetCpuInformation());
}

Expression* CloneVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr)
{
  return new TrackedIdentifierExpression(
    pTrkIdExpr->GetId(),
    pTrkIdExpr->GetCpuInformation(),
    pTrkIdExpr->GetCurrentAddress());
}

Expression* CloneVisitor::VisitMemory(MemoryExpression* pMemExpr)
{
  auto pBaseExpr = pMemExpr->GetBaseExpression() ? pMemExpr->GetBaseExpression()->Visit(this) : nullptr;

  return Expr::MakeMem(
    pMemExpr->GetAccessSizeInBit(),
    pBaseExpr,
    pMemExpr->GetOffsetExpression()->Visit(this),
    pMemExpr->IsDereferencable());
}

Expression* CloneVisitor::VisitSymbolic(SymbolicExpression* pSymExpr)
{
  return Expr::MakeSym(
    pSymExpr->GetType(),
    pSymExpr->GetValue());
}

Expression* FilterVisitor::VisitSystem(SystemExpression* pSysExpr)
{
  _Evaluate(pSysExpr);
  return nullptr;
}

Expression* FilterVisitor::VisitBind(BindExpression* pBindExpr)
{
  _Evaluate(pBindExpr);
  if (_IsDone())
    return nullptr;

  for (auto pExpr : pBindExpr->GetBoundExpressions())
  {
    pExpr->Visit(this);

    if (_IsDone())
      return nullptr;
  }

  return nullptr;
}

Expression* FilterVisitor::VisitTernaryCondition(TernaryConditionExpression* pTernExpr)
{
  _Evaluate(pTernExpr);
  if (_IsDone())
    return nullptr;

  pTernExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pTernExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pTernExpr->GetTrueExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pTernExpr->GetFalseExpression()->Visit(this);
  return nullptr;
}

Expression* FilterVisitor::VisitIfElseCondition(IfElseConditionExpression* pIfElseExpr)
{
  _Evaluate(pIfElseExpr);
  if (_IsDone())
    return nullptr;

  pIfElseExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;;

  pIfElseExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pIfElseExpr->GetThenExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  if (pIfElseExpr->GetElseExpression() != nullptr)
    pIfElseExpr->GetElseExpression()->Visit(this);
  return nullptr;
}

Expression* FilterVisitor::VisitWhileCondition(WhileConditionExpression* pWhileExpr)
{
  _Evaluate(pWhileExpr);
  if (_IsDone())
    return nullptr;

  pWhileExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pWhileExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pWhileExpr->GetBodyExpression()->Visit(this);
  return nullptr;
}

Expression* FilterVisitor::VisitAssignment(AssignmentExpression* pAssignExpr)
{
  _Evaluate(pAssignExpr);
  if (_IsDone())
    return nullptr;

  pAssignExpr->GetDestinationExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression* FilterVisitor::VisitOperation(OperationExpression* pOpExpr)
{
  _Evaluate(pOpExpr);
  if (_IsDone())
    return nullptr;

  pOpExpr->GetLeftExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression* FilterVisitor::VisitConstant(ConstantExpression* pConstExpr)
{
  _Evaluate(pConstExpr);
  return nullptr;
}

Expression* FilterVisitor::VisitIdentifier(IdentifierExpression* pIdExpr)
{
  _Evaluate(pIdExpr);
  return nullptr;
}

Expression* FilterVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr)
{
  _Evaluate(pTrkIdExpr);
  return nullptr;
}

Expression* FilterVisitor::VisitMemory(MemoryExpression* pMemExpr)
{
  _Evaluate(pMemExpr);
  if (_IsDone())
    return nullptr;

  if (pMemExpr->GetBaseExpression() != nullptr)
    pMemExpr->GetBaseExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  pMemExpr->GetOffsetExpression()->Visit(this);
  return nullptr;
}

Expression* FilterVisitor::VisitSymbolic(SymbolicExpression* pSymExpr)
{
  if (m_ExprMatcher(pSymExpr))
    m_MatchedExprs.push_back(pSymExpr);
  return nullptr;
}

void FilterVisitor::_Evaluate(Expression* pExpr)
{
  _AddExpression(m_ExprMatcher(pExpr));
}

void FilterVisitor::_AddExpression(Expression* pExpr)
{
  if (pExpr != nullptr)
    m_MatchedExprs.push_back(pExpr);
}

bool FilterVisitor::_IsDone(void) const
{
  if (m_NbrOfResult == 0)
    return false;
  return m_MatchedExprs.size() >= m_NbrOfResult;
}

TrackVisitor::TrackVisitor(Address const& rCurAddr, Track::Context& rCtxt)
: m_CurAddr(rCurAddr), m_rCtxt(rCtxt), m_pTrackedExpr(nullptr), m_IsAssigned(false)
{
}

Expression* TrackVisitor::VisitAssignment(AssignmentExpression* pAssignExpr)
{
  // NOTE: We must to visit the right side first, since the left side could
  // modify the Track context
  auto pTrkSrcExpr = pAssignExpr->GetSourceExpression()->Visit(this);

  m_IsAssigned = true;
  auto pTrkDstExpr = pAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  return Expr::MakeAssign(pTrkDstExpr, pTrkSrcExpr);
}

Expression* TrackVisitor::VisitIdentifier(IdentifierExpression* pIdExpr)
{
  Address TrackedAddress;

  if (m_IsAssigned)
  {
    TrackedAddress = m_CurAddr;
    m_rCtxt.TrackId(pIdExpr->GetId(), m_CurAddr);
  }
  else
  {
    if (!m_rCtxt.GetTrackAddress(pIdExpr->GetId(), TrackedAddress))
      return CloneVisitor::VisitIdentifier(pIdExpr);
  }

  return new TrackedIdentifierExpression(pIdExpr->GetId(), pIdExpr->GetCpuInformation(), TrackedAddress);
}

Expression* TrackVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr)
{
  Address TrackedAddress;

  if (m_IsAssigned)
  {
    TrackedAddress = m_CurAddr;
    m_rCtxt.TrackId(pTrkIdExpr->GetId(), m_CurAddr);
  }
  else
  {
    if (!m_rCtxt.GetTrackAddress(pTrkIdExpr->GetId(), TrackedAddress))
      return Expr::MakeId(pTrkIdExpr->GetId(), pTrkIdExpr->GetCpuInformation()); // TODO: Do we really need to remove the track?
  }

  return new TrackedIdentifierExpression(pTrkIdExpr->GetId(), pTrkIdExpr->GetCpuInformation(), TrackedAddress);
}

Expression* BackTrackVisitor::VisitAssignment(AssignmentExpression* pAssignExpr)
{
  m_IsAssigned = true;
  pAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  // TODO: can we have 2 OpAff in the same expression?
  pAssignExpr->GetSourceExpression()->Visit(this);
  m_TrackSource = false;

  return nullptr;
}

Expression* BackTrackVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr)
{
  if (m_IsAssigned)
  {
    if (m_rBtCtxt.IsTracked(std::make_tuple(pTrkIdExpr->GetId(), pTrkIdExpr->GetCurrentAddress())))
    {
      m_Result = true;
      m_TrackSource = true;
    }
  }

  else if (!m_IsAssigned && m_TrackSource)
  {
    m_rBtCtxt.TrackId(std::make_tuple(pTrkIdExpr->GetId(), pTrkIdExpr->GetCurrentAddress()));
  }

  return nullptr;
}

// expression simplifier //////////////////////////////////////////////////////

bool ExpressionSimplifier::Execute(void)
{
  do
  {
    if (!_RunOnce())
      return false;
  }
  while (!m_IsDone);

  return true;
}

TrackedIdPropagation::TrackedIdPropagation(Expression::List& rExprs, u32 Id) : ExpressionSimplifier(rExprs), m_pResExpr(nullptr)
{
  // Find the track id expression
  auto FindTrkIdExpr = [&](Expression* pExpr) -> Expression*
  {
    if (pExpr->GetKind() != Expression::Assign)
      return nullptr;
    auto pDstExpr = static_cast<AssignmentExpression*>(pExpr)->GetDestinationExpression();
    if (pDstExpr->GetKind() != Expression::TrackedId)
      return nullptr;
    return pDstExpr;
  };

  for (auto itExpr = m_rExprs.begin(); itExpr != m_rExprs.end(); ++itExpr)
  {
    //DEBUG
    std::string s = (*itExpr)->ToString();
    Log::Write("dbg") << "(tip) tracked expr: " << s << LogEnd;

    FilterVisitor FltVst(FindTrkIdExpr, false);
    (*itExpr)->Visit(&FltVst);
    auto rFltExprs = FltVst.GetMatchedExpressions();

    if (rFltExprs.size() != 1)
      continue;

    m_pResExpr = static_cast<AssignmentExpression*>(*itExpr);
    Log::Write("dbg") << "(tip) ref expr: " << m_pResExpr->ToString() << LogEnd;
    break;
  }
}

bool TrackedIdPropagation::_RunOnce(void)
{
  if (m_pResExpr == nullptr)
    return false;

  // This functor is used to retrieve the source of a specific tracked id
  struct FindTrkSrc
  {
    FindTrkSrc(void) : m_IsAssigned(false) {}

    Expression* operator()(Expression* pExpr)
    {
      switch (pExpr->GetKind())
      {
      case Expression::Assign:
        m_IsAssigned = true;
        break;

      case Expression::TrackedId:
        if (m_IsAssigned)
        {
          m_IsAssigned = false;
          break;
        }
        return pExpr;

      default:
        m_IsAssigned = false;
        break;
      }

      return nullptr;
    }

    bool m_IsAssigned;
  };

  //DEBUG
  std::string s = m_pResExpr->ToString();

  FindTrkSrc Matcher;
  FilterVisitor FindTrkSrcVst(Matcher, 1);

  m_pResExpr->Visit(&FindTrkSrcVst);
  auto rExprs = FindTrkSrcVst.GetMatchedExpressions();

  // No more TrackedIdExpr
  if (rExprs.empty())
  {
    m_IsDone = true;
    return true;
  }

  assert(rExprs.size() == 1);
  auto pRes = static_cast<TrackedIdentifierExpression*>(rExprs.front());
  assert(pRes->GetKind() == Expression::TrackedId);

  Log::Write("dbg") << "(tip) found tracked id in result: " << pRes->ToString() << LogEnd;

  auto pTrkExpr = __FindTrackedIdExpression(pRes->GetId(), pRes->GetCurrentAddress());
  if (pTrkExpr == nullptr)
    return false;

  Log::Write("dbg") << "(tip) found id in list: " << pTrkExpr->ToString() << LogEnd;
  //<PARENT>->UpdateChild(pRes, pTrkExpr->Clone());
  //Log::Write("dbg") << "(tip) update result: " << m_pResExpr->ToString() << LogEnd;

  return false;
}

bool TrackedIdPropagation::_Finalize(void)
{
  if (m_pResExpr == nullptr)
    return false;
  for (auto pExpr : m_rExprs)
    delete pExpr;
  m_rExprs.clear();
  m_rExprs.push_back(m_pResExpr);
  return true;
}

Expression* TrackedIdPropagation::__FindTrackedIdExpression(u32 Id, Address const& rAddr)
{
  auto FindTrkByIdAddr = [&](Expression* pExpr) -> Expression*
  {
    if (pExpr->GetKind() != Expression::Assign)
      return nullptr;
    auto pAssignExpr = static_cast<AssignmentExpression*>(pExpr);

    if (pAssignExpr->GetDestinationExpression()->GetKind() != Expression::TrackedId)
      return nullptr;

    auto pTrkId = static_cast<TrackedIdentifierExpression*>(pAssignExpr->GetDestinationExpression());
    if (pTrkId->GetId() != Id || pTrkId->GetCurrentAddress() != rAddr)
      return nullptr;

    return pExpr;
  };

  for (auto pExpr : m_rExprs)
  {
    //DEBUG
    std::string s = pExpr->ToString();
    Log::Write("dbg") << "(tip) search tracked id in: " << s << LogEnd;

    FilterVisitor FindTrkByIdAddrVst(FindTrkByIdAddr);
    pExpr->Visit(&FindTrkByIdAddrVst);

    auto Exprs = FindTrkByIdAddrVst.GetMatchedExpressions();
    if (Exprs.size() != 1)
      continue;

    return Exprs.front();
  }

  return nullptr;
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

Expression* Expr::MakeTernaryCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr)
{
  return new TernaryConditionExpression(CondType, pRefExpr, pTestExpr, pTrueExpr, pFalseExpr);
}

Expression* Expr::MakeIfElseCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr)
{
  return new IfElseConditionExpression(CondType, pRefExpr, pTestExpr, pThenExpr, pElseExpr);
}

Expression* Expr::MakeWhileCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr)
{
  return new WhileConditionExpression(CondType, pRefExpr, pTestExpr, pBodyExpr);
}

Expression* Expr::MakeAssign(Expression *pDstExpr, Expression *pSrcExpr)
{
  return new AssignmentExpression(pDstExpr, pSrcExpr);
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