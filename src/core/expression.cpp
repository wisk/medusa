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

Expression::SPtr SystemExpression::Clone(void) const
{
  return std::make_shared<SystemExpression>(m_Name);
}

Expression::SPtr SystemExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitSystem(shared_from_this());
}

// bind expression ////////////////////////////////////////////////////////////

BindExpression::BindExpression(Expression::List const& rExprs)
: m_Expressions(rExprs)
{
}

BindExpression::~BindExpression(void)
{
}

std::string BindExpression::ToString(void) const
{
  std::list<std::string> ExprsStrList;

  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [&](Expression::SPtr spExpr)
  {
    ExprsStrList.push_back(spExpr->ToString());
  });

  return boost::algorithm::join(ExprsStrList, "; ");
}

Expression::SPtr BindExpression::Clone(void) const
{
  Expression::List ExprListCloned;
  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [&](Expression::SPtr spExpr)
  {
    ExprListCloned.push_back(spExpr->Clone());
  });

  return std::make_shared<BindExpression>(ExprListCloned);
}

Expression::SPtr BindExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitBind(std::static_pointer_cast<BindExpression>(shared_from_this()));
}

bool BindExpression::UpdateChild(Expression::SPtr spOldExpr, Expression::SPtr spNewExpr)
{
  auto itExpr = std::find(std::begin(m_Expressions), std::end(m_Expressions), spOldExpr);
  if (itExpr != std::end(m_Expressions))
  {
    m_Expressions.erase(itExpr);
    // TODO: we may want to keep the order...
    m_Expressions.push_back(spNewExpr);
    return true;
  }

  for (auto spExpr : m_Expressions)
  {
    if (spExpr->UpdateChild(spOldExpr, spNewExpr))
      return true;
  }

  return false;
}

// condition expression ///////////////////////////////////////////////////////

ConditionExpression::ConditionExpression(Type CondType, Expression::SPtr spRefExpr, Expression::SPtr spTestExpr)
: m_Type(CondType), m_spRefExpr(spRefExpr), m_spTestExpr(spTestExpr)
{
}

ConditionExpression::~ConditionExpression(void)
{
}

std::string ConditionExpression::ToString(void) const
{
  static const char *s_StrCond[] = { "???", "==", "!=", "u>", "u>=", "u<", "u<=", "s>", "s>=", "s<", "s<=" };
  if (m_spRefExpr == nullptr || m_spTestExpr == nullptr || m_Type >= (sizeof(s_StrCond) / sizeof(*s_StrCond)))
    return "";

  return (boost::format("(%1% %2% %3%)") % m_spRefExpr->ToString() % s_StrCond[m_Type] % m_spTestExpr->ToString()).str();
}

TernaryConditionExpression::TernaryConditionExpression(Type CondType, Expression::SPtr spRefExpr, Expression::SPtr spTestExpr, Expression::SPtr spTrueExpr, Expression::SPtr spFalseExpr)
: ConditionExpression(CondType, spRefExpr, spTestExpr), m_spTrueExpr(spTrueExpr), m_spFalseExpr(spFalseExpr)
{
}

TernaryConditionExpression::~TernaryConditionExpression(void)
{
}

std::string TernaryConditionExpression::ToString(void) const
{
  std::string Result = ConditionExpression::ToString();
  Result += " ? (";
  Result += m_spTrueExpr->ToString();

  Result += ") : ";
  Result += m_spFalseExpr->ToString();

  Result += ")";

  return Result;
}

Expression::SPtr TernaryConditionExpression::Clone(void) const
{
  return std::make_shared<TernaryConditionExpression>(
    m_Type,
    m_spRefExpr->Clone(),
    m_spTestExpr->Clone(),
    m_spTrueExpr->Clone(),
    m_spFalseExpr->Clone());
}

Expression::SPtr TernaryConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitTernaryCondition(std::static_pointer_cast<TernaryConditionExpression>(shared_from_this()));
}

bool TernaryConditionExpression::UpdateChild(Expression::SPtr spOldExpr, Expression::SPtr spNewExpr)
{
  if (m_spRefExpr == spOldExpr)
  {
    m_spRefExpr = spNewExpr;
    return true;
  }

  if (m_spTestExpr == spOldExpr)
  {
    m_spTestExpr = spNewExpr;
    return true;
  }

  if (m_spTrueExpr == spOldExpr)
  {
    m_spTrueExpr = spNewExpr;
    return true;
  }

  if (m_spFalseExpr == spOldExpr)
  {
    m_spFalseExpr = spNewExpr;
    return true;
  }

  if (m_spRefExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spTestExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spTrueExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spFalseExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  return false;
}

IfElseConditionExpression::IfElseConditionExpression(Type CondType, Expression::SPtr spRefExpr, Expression::SPtr spTestExpr, Expression::SPtr spThenExpr, Expression::SPtr spElseExpr)
: ConditionExpression(CondType, spRefExpr, spTestExpr), m_spThenExpr(spThenExpr), m_spElseExpr(spElseExpr)
{
}

IfElseConditionExpression::~IfElseConditionExpression(void)
{
}

std::string IfElseConditionExpression::ToString(void) const
{
  if (m_spElseExpr == nullptr)
    return (boost::format("if %1% { %2% }") % ConditionExpression::ToString() % m_spThenExpr->ToString()).str();
  return (boost::format("if %1% { %2% } else { %3% }") % ConditionExpression::ToString() % m_spThenExpr->ToString() % m_spElseExpr->ToString()).str();
}

Expression::SPtr IfElseConditionExpression::Clone(void) const
{
  return Expr::MakeIfElseCond(
    m_Type,
    m_spRefExpr->Clone(), m_spTestExpr->Clone(),
    m_spThenExpr->Clone(), m_spElseExpr != nullptr ? m_spThenExpr->Clone() : nullptr);
}

Expression::SPtr IfElseConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitIfElseCondition(std::static_pointer_cast<IfElseConditionExpression>(shared_from_this()));
}

bool IfElseConditionExpression::UpdateChild(Expression::SPtr spOldExpr, Expression::SPtr spNewExpr)
{
  if (m_spRefExpr == spOldExpr)
  {
    m_spRefExpr = spNewExpr;
    return true;
  }

  if (m_spTestExpr == spOldExpr)
  {
    m_spTestExpr = spNewExpr;
    return true;
  }

  if (m_spThenExpr == spOldExpr)
  {
    m_spThenExpr = spNewExpr;
    return true;
  }

  // NOTE: No need to check m_spElseExpr since spOldExpr cannot be nullptr

  if (m_spElseExpr == spOldExpr)
  {
    m_spElseExpr = spNewExpr;
    return true;
  }

  if (m_spRefExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spTestExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spThenExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spElseExpr != nullptr && m_spElseExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  return false;
}

WhileConditionExpression::WhileConditionExpression(Type CondType, Expression::SPtr spRefExpr, Expression::SPtr spTestExpr, Expression::SPtr spBodyExpr)
: ConditionExpression(CondType, spRefExpr, spTestExpr), m_spBodyExpr(spBodyExpr)
{
}

WhileConditionExpression::~WhileConditionExpression(void)
{
}

std::string WhileConditionExpression::ToString(void) const
{
  return (boost::format("while %1% { %2% }") % ConditionExpression::ToString() % m_spBodyExpr->ToString()).str();
}

Expression::SPtr WhileConditionExpression::Clone(void) const
{
  return std::make_shared<WhileConditionExpression>(m_Type, m_spRefExpr->Clone(), m_spTestExpr->Clone(), m_spBodyExpr->Clone());
}

Expression::SPtr WhileConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitWhileCondition(std::static_pointer_cast<WhileConditionExpression>(shared_from_this()));
}

bool WhileConditionExpression::UpdateChild(Expression::SPtr spOldExpr, Expression::SPtr spNewExpr)
{
  if (m_spRefExpr == spOldExpr)
  {
    m_spRefExpr = spNewExpr;
    return true;
  }

  if (m_spTestExpr == spOldExpr)
  {
    m_spTestExpr = spNewExpr;
    return true;
  }

  if (m_spBodyExpr == spOldExpr)
  {
    m_spBodyExpr = spNewExpr;
    return true;
  }

  if (m_spRefExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spTestExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spBodyExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  return false;
}

// operation expression ///////////////////////////////////////////////////////

AssignmentExpression::AssignmentExpression(Expression::SPtr spDstExpr, Expression::SPtr spSrcExpr)
: m_spDstExpr(spDstExpr), m_spSrcExpr(spSrcExpr)
{
}

AssignmentExpression::~AssignmentExpression(void)
{
}

std::string AssignmentExpression::ToString(void) const
{
  return (boost::format("(%1% = %2%)") % m_spDstExpr->ToString() % m_spSrcExpr->ToString()).str();
}

Expression::SPtr AssignmentExpression::Clone(void) const
{
  return std::make_shared<AssignmentExpression>(m_spDstExpr->Clone(), m_spSrcExpr->Clone());
}

Expression::SPtr AssignmentExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitAssignment(std::static_pointer_cast<AssignmentExpression>(shared_from_this()));
}

bool AssignmentExpression::UpdateChild(Expression::SPtr spOldExpr, Expression::SPtr spNewExpr)
{
  if (m_spDstExpr == spOldExpr)
  {
    m_spDstExpr = spNewExpr;
    return true;
  }

  if (m_spSrcExpr == spOldExpr)
  {
    m_spSrcExpr = spNewExpr;
    return true;
  }

  if (m_spDstExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spSrcExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  return false;
}

OperationExpression::OperationExpression(Type OpType, Expression::SPtr spLeftExpr, Expression::SPtr spRightExpr)
: m_OpType(OpType), m_spLeftExpr(spLeftExpr), m_spRightExpr(spRightExpr)
{
  assert(this != spLeftExpr.get());
  assert(this != spRightExpr.get());
}

OperationExpression::~OperationExpression(void)
{
}

std::string OperationExpression::ToString(void) const
{
  static const char *s_StrOp[] = { "???", "↔", "&", "|", "^", "<<", ">>", ">>(s)", "+", "-", "*", "/" };

  if (m_spLeftExpr == nullptr || m_spRightExpr == nullptr)


    return "";

  auto LeftStr = m_spLeftExpr->ToString();

  auto RightStr = m_spRightExpr->ToString();


  if (LeftStr.empty() || RightStr.empty())
    return "";

  if (m_OpType == OpSext)
    return (boost::format("sext_%2%(%1%)") % LeftStr % RightStr).str();

  if (m_OpType >= (sizeof(s_StrOp) / sizeof(*s_StrOp)))
    return "";

  return (boost::format("(%1% %2% %3%)") % LeftStr % s_StrOp[m_OpType] % RightStr).str();
}

Expression::SPtr OperationExpression::Clone(void) const
{
  return std::make_shared<OperationExpression>(static_cast<Type>(m_OpType), m_spLeftExpr->Clone(), m_spRightExpr->Clone());
}

Expression::SPtr OperationExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitOperation(std::static_pointer_cast<OperationExpression>(shared_from_this()));
}

bool OperationExpression::UpdateChild(Expression::SPtr spOldExpr, Expression::SPtr spNewExpr)
{
  if (m_spLeftExpr == spOldExpr)
  {
    m_spLeftExpr = spNewExpr;
    return true;
  }
  if (m_spRightExpr == spOldExpr)
  {
    m_spRightExpr = spNewExpr;
    return true;
  }

  if (m_spLeftExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;
  if (m_spRightExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  return false;
}

void OperationExpression::SwapLeftExpressions(OperationExpression::SPtr spOpExpr)
{
  m_spLeftExpr.swap(spOpExpr->m_spLeftExpr);
  if (m_OpType == OpSub && spOpExpr->m_OpType == OpSub) // TODO: handle operator precedence
    spOpExpr->m_OpType = OpAdd;
  assert(this != m_spLeftExpr.get());
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

Expression::SPtr ConstantExpression::Clone(void) const
{
  return std::make_shared<ConstantExpression>(m_ConstType, m_Value);
}

Expression::SPtr ConstantExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitConstant(std::static_pointer_cast<ConstantExpression>(shared_from_this()));
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
: m_Id(Id), m_pCpuInfo(pCpuInfo) {}


std::string IdentifierExpression::ToString(void) const
{
  auto pIdName = m_pCpuInfo->ConvertIdentifierToName(m_Id);

  if (pIdName == 0) return "";

  return (boost::format("Id%d(%s)") % m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) % pIdName).str();

}

Expression::SPtr IdentifierExpression::Clone(void) const
{
  return std::make_shared<IdentifierExpression>(m_Id, m_pCpuInfo);

}

u32 IdentifierExpression::GetSizeInBit(void) const
{
  return m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);

}

Expression::SPtr IdentifierExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitIdentifier(std::static_pointer_cast<IdentifierExpression>(shared_from_this()));
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
: m_Id(Id), m_pCpuInfo(pCpuInfo), m_CurAddr(rCurAddr) {}


TrackedIdentifierExpression::~TrackedIdentifierExpression(void)
{
}

std::string TrackedIdentifierExpression::ToString(void) const
{
  auto pIdName = m_pCpuInfo->ConvertIdentifierToName(m_Id);

  if (pIdName == 0) return "";

  return (boost::format("Id%d(%s)[%s]") % m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) % pIdName % m_CurAddr.ToString()).str();

}

Expression::SPtr TrackedIdentifierExpression::Clone(void) const
{
  return std::make_shared<TrackedIdentifierExpression>(m_Id, m_pCpuInfo, m_CurAddr);

}

u32 TrackedIdentifierExpression::GetSizeInBit(void) const
{
  return m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);

}

Expression::SPtr TrackedIdentifierExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitTrackedIdentifier(std::static_pointer_cast<TrackedIdentifierExpression>(shared_from_this()));
}

// memory expression //////////////////////////////////////////////////////////

MemoryExpression::MemoryExpression(u32 AccessSize, Expression::SPtr spExprBase, Expression::SPtr spExprOffset, bool Dereference)
: m_AccessSizeInBit(AccessSize), m_spBaseExpr(spExprBase), m_spOffExpr(spExprOffset), m_Dereference(Dereference)
{
  assert(spExprOffset != nullptr);
}

MemoryExpression::~MemoryExpression(void)
{
}

std::string MemoryExpression::ToString(void) const
{
  auto const pMemType = m_Dereference ? "Mem" : "Addr";
  if (m_spBaseExpr == nullptr)
    return (boost::format("%s%d(%s)") % pMemType % m_AccessSizeInBit % m_spOffExpr->ToString()).str();

  return (boost::format("%s%d(%s:%s)") % pMemType % m_AccessSizeInBit % m_spBaseExpr->ToString() % m_spOffExpr->ToString()).str();
}

Expression::SPtr MemoryExpression::Clone(void) const
{
  if (m_spBaseExpr == nullptr)
    return std::make_shared<MemoryExpression>(m_AccessSizeInBit, nullptr, m_spOffExpr->Clone(), m_Dereference);

  return std::make_shared<MemoryExpression>(m_AccessSizeInBit, m_spBaseExpr->Clone(), m_spOffExpr->Clone(), m_Dereference);
}

u32 MemoryExpression::GetSizeInBit(void) const
{
  return m_AccessSizeInBit;
}

Expression::SPtr MemoryExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitMemory(std::static_pointer_cast<MemoryExpression>(shared_from_this()));
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

  if (m_spBaseExpr != nullptr)
    if (m_spBaseExpr->Read(pCpuCtxt, pMemCtxt, Base) == false)
      return false;
  if (m_spOffExpr->Read(pCpuCtxt, pMemCtxt, Offset) == false)
    return false;
  rAddress = Address(static_cast<u16>(Base), Offset);
  return true;
}

bool MemoryExpression::UpdateChild(Expression::SPtr spOldExpr, Expression::SPtr spNewExpr)
{
  if (m_spBaseExpr == spOldExpr)
  {
    m_spBaseExpr = spNewExpr;
    return true;
  }

  if (m_spOffExpr == spOldExpr)
  {
    m_spOffExpr = spNewExpr;
    return true;
  }

  if (m_spBaseExpr != nullptr && m_spBaseExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  if (m_spOffExpr->UpdateChild(spOldExpr, spNewExpr))
    return true;

  return false;
}

// symbolic expression ////////////////////////////////////////////////////////

SymbolicExpression::SymbolicExpression(SymbolicExpression::Type SymType, std::string const& rValue)
: m_Type(SymType), m_Value(rValue) {}

std::string SymbolicExpression::ToString(void) const
{
  static char const* TypeToStr[] = { "unknown", "retval", "parm", "undef" };
  if (m_Type > Undefined)
    return "<invalid symbolic expression>";
  return (boost::format("sym(%1, %2)") % TypeToStr[m_Type] % m_Value).str();
}

Expression::SPtr SymbolicExpression::Clone(void) const
{
  return std::make_shared<SymbolicExpression>(m_Type, m_Value);
}

u32 SymbolicExpression::GetSizeInBit(void) const
{
  return 0;
}

bool SymbolicExpression::SignExtend(u32 NewSizeInBit)
{
  return false;
}

Expression::SPtr SymbolicExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitSymbolic(std::static_pointer_cast<SymbolicExpression>(shared_from_this()));
}

// expression visitor /////////////////////////////////////////////////////////

Expression::SPtr ExpressionVisitor::VisitSystem(SystemExpression::SPtr spSysExpr)
{
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitBind(BindExpression::SPtr spBindExpr)
{
  auto& rExprs = spBindExpr->GetBoundExpressions();
  for (auto pExpr : rExprs)
    pExpr->Visit(this);
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPtr spTernExpr)
{
  spTernExpr->GetReferenceExpression()->Visit(this);
  spTernExpr->GetTestExpression()->Visit(this);
  spTernExpr->GetTrueExpression()->Visit(this);
  spTernExpr->GetFalseExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPtr spIfElseExpr)
{
  spIfElseExpr->GetReferenceExpression()->Visit(this);
  spIfElseExpr->GetTestExpression()->Visit(this);
  spIfElseExpr->GetThenExpression()->Visit(this);
  if (spIfElseExpr->GetElseExpression() != nullptr)
    spIfElseExpr->GetElseExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPtr spWhileExpr)
{
  spWhileExpr->GetReferenceExpression()->Visit(this);
  spWhileExpr->GetTestExpression()->Visit(this);
  spWhileExpr->GetBodyExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitAssignment(AssignmentExpression::SPtr spAssignExpr)
{
  spAssignExpr->GetDestinationExpression()->Visit(this);
  spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitOperation(OperationExpression::SPtr spOpExpr)
{
  spOpExpr->GetLeftExpression()->Visit(this);
  spOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitConstant(ConstantExpression::SPtr spConstExpr)
{
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitIdentifier(IdentifierExpression::SPtr spIdExpr)
{
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr spTrkIdExpr)
{
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitMemory(MemoryExpression::SPtr spMemExpr)
{
  if (spMemExpr->GetBaseExpression() != nullptr)
    spMemExpr->GetBaseExpression()->Visit(this);
  spMemExpr->GetOffsetExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr ExpressionVisitor::VisitSymbolic(SymbolicExpression::SPtr spSymExpr)
{
  return nullptr;
}

Expression::SPtr CloneVisitor::VisitSystem(SystemExpression::SPtr spSysExpr)
{
  return spSysExpr->Clone();
}

Expression::SPtr CloneVisitor::VisitBind(BindExpression::SPtr spBindExpr)
{
  Expression::List Exprs;

  for (auto spExpr : spBindExpr->GetBoundExpressions())
    Exprs.push_back(spExpr->Visit(this));

  return Expr::MakeBind(Exprs);
}

Expression::SPtr CloneVisitor::VisitTernaryCondition(TernaryConditionExpression::SPtr spTernExpr)
{
  return Expr::MakeTernaryCond(
    spTernExpr->GetType(),
    spTernExpr->GetReferenceExpression()->Visit(this),
    spTernExpr->GetTestExpression()->Visit(this),
    spTernExpr->GetTrueExpression()->Visit(this),
    spTernExpr->GetFalseExpression()->Visit(this));
}

Expression::SPtr CloneVisitor::VisitIfElseCondition(IfElseConditionExpression::SPtr spIfElseExpr)
{
  auto spElseExpr = spIfElseExpr->GetElseExpression() != nullptr ? spIfElseExpr->GetElseExpression()->Visit(this) : nullptr;
  return Expr::MakeIfElseCond(
    spIfElseExpr->GetType(),
    spIfElseExpr->GetReferenceExpression()->Visit(this),
    spIfElseExpr->GetTestExpression()->Visit(this),
    spIfElseExpr->GetThenExpression()->Visit(this),
    spElseExpr);
}

Expression::SPtr CloneVisitor::VisitWhileCondition(WhileConditionExpression::SPtr spWhileExpr)
{
  return Expr::MakeWhileCond(
    spWhileExpr->GetType(),
    spWhileExpr->GetReferenceExpression(),
    spWhileExpr->GetTestExpression(),
    spWhileExpr->GetBodyExpression());
}

Expression::SPtr CloneVisitor::VisitAssignment(AssignmentExpression::SPtr spAssignExpr)
{
  return Expr::MakeAssign(
    spAssignExpr->GetDestinationExpression()->Visit(this),
    spAssignExpr->GetSourceExpression()->Visit(this));
}

Expression::SPtr CloneVisitor::VisitOperation(OperationExpression::SPtr spOpExpr)
{
  return Expr::MakeOp(
    static_cast<OperationExpression::Type>(spOpExpr->GetOperation()),
    spOpExpr->GetLeftExpression()->Visit(this),
    spOpExpr->GetRightExpression()->Visit(this));
}

Expression::SPtr CloneVisitor::VisitConstant(ConstantExpression::SPtr spConstExpr)
{
  return Expr::MakeConst(
    spConstExpr->GetType(),
    spConstExpr->GetConstant());
}

Expression::SPtr CloneVisitor::VisitIdentifier(IdentifierExpression::SPtr spIdExpr)
{
  return Expr::MakeId(
    spIdExpr->GetId(),
    spIdExpr->GetCpuInformation());
}

Expression::SPtr CloneVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr spTrkIdExpr)
{
  return std::make_shared<TrackedIdentifierExpression>(
    spTrkIdExpr->GetId(),
    spTrkIdExpr->GetCpuInformation(),
    spTrkIdExpr->GetCurrentAddress());
}

Expression::SPtr CloneVisitor::VisitMemory(MemoryExpression::SPtr spMemExpr)
{
  auto spBaseExpr = spMemExpr->GetBaseExpression() ? spMemExpr->GetBaseExpression()->Visit(this) : nullptr;

  return Expr::MakeMem(
    spMemExpr->GetAccessSizeInBit(),
    spBaseExpr,
    spMemExpr->GetOffsetExpression()->Visit(this),
    spMemExpr->IsDereferencable());
}

Expression::SPtr CloneVisitor::VisitSymbolic(SymbolicExpression::SPtr spSymExpr)
{
  return Expr::MakeSym(
    spSymExpr->GetType(),
    spSymExpr->GetValue());
}

Expression::SPtr FilterVisitor::VisitSystem(SystemExpression::SPtr spSysExpr)
{
  _Evaluate(spSysExpr);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitBind(BindExpression::SPtr spBindExpr)
{
  _Evaluate(spBindExpr);
  if (_IsDone())
    return nullptr;

  for (auto pExpr : spBindExpr->GetBoundExpressions())
  {
    pExpr->Visit(this);

    if (_IsDone())
      return nullptr;
  }

  return nullptr;
}

Expression::SPtr FilterVisitor::VisitTernaryCondition(TernaryConditionExpression::SPtr spTernExpr)
{
  _Evaluate(spTernExpr);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetTrueExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spTernExpr->GetFalseExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitIfElseCondition(IfElseConditionExpression::SPtr spIfElseExpr)
{
  _Evaluate(spIfElseExpr);
  if (_IsDone())
    return nullptr;

  spIfElseExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;;

  spIfElseExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spIfElseExpr->GetThenExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  if (spIfElseExpr->GetElseExpression() != nullptr)
    spIfElseExpr->GetElseExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitWhileCondition(WhileConditionExpression::SPtr spWhileExpr)
{
  _Evaluate(spWhileExpr);
  if (_IsDone())
    return nullptr;

  spWhileExpr->GetReferenceExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spWhileExpr->GetTestExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spWhileExpr->GetBodyExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitAssignment(AssignmentExpression::SPtr spAssignExpr)
{
  _Evaluate(spAssignExpr);
  if (_IsDone())
    return nullptr;

  spAssignExpr->GetDestinationExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitOperation(OperationExpression::SPtr spOpExpr)
{
  _Evaluate(spOpExpr);
  if (_IsDone())
    return nullptr;

  spOpExpr->GetLeftExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitConstant(ConstantExpression::SPtr spConstExpr)
{
  _Evaluate(spConstExpr);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitIdentifier(IdentifierExpression::SPtr spIdExpr)
{
  _Evaluate(spIdExpr);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr spTrkIdExpr)
{
  _Evaluate(spTrkIdExpr);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitMemory(MemoryExpression::SPtr spMemExpr)
{
  _Evaluate(spMemExpr);
  if (_IsDone())
    return nullptr;

  if (spMemExpr->GetBaseExpression() != nullptr)
    spMemExpr->GetBaseExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  spMemExpr->GetOffsetExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr FilterVisitor::VisitSymbolic(SymbolicExpression::SPtr spSymExpr)
{
  if (m_ExprMatcher(spSymExpr))
    m_MatchedExprs.push_back(spSymExpr);
  return nullptr;
}

void FilterVisitor::_Evaluate(Expression::SPtr spExpr)
{
  _AddExpression(m_ExprMatcher(spExpr));
}

void FilterVisitor::_AddExpression(Expression::SPtr spExpr)
{
  if (spExpr != nullptr)
    m_MatchedExprs.push_back(spExpr);
}

bool FilterVisitor::_IsDone(void) const
{
  if (m_NbrOfResult == 0)
    return false;
  return m_MatchedExprs.size() >= m_NbrOfResult;
}

TrackVisitor::TrackVisitor(Address const& rCurAddr, Track::Context& rCtxt)
: m_CurAddr(rCurAddr), m_rCtxt(rCtxt), m_spTrackedExpr(nullptr), m_IsAssigned(false)
{
}

Expression::SPtr TrackVisitor::VisitAssignment(AssignmentExpression::SPtr spAssignExpr)
{
  // NOTE: We must to visit the right side first, since the left side could
  // modify the Track context
  auto pTrkSrcExpr = spAssignExpr->GetSourceExpression()->Visit(this);

  m_IsAssigned = true;
  auto spTrkDstExpr = spAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  return Expr::MakeAssign(spTrkDstExpr, pTrkSrcExpr);
}

Expression::SPtr TrackVisitor::VisitIdentifier(IdentifierExpression::SPtr spIdExpr)
{
  Address TrackedAddress;

  if (m_IsAssigned)
  {
    TrackedAddress = m_CurAddr;
    m_rCtxt.TrackId(spIdExpr->GetId(), m_CurAddr);
  }
  else
  {
    if (!m_rCtxt.GetTrackAddress(spIdExpr->GetId(), TrackedAddress)
      || m_CurAddr == TrackedAddress)
      return CloneVisitor::VisitIdentifier(spIdExpr);
  }

  return std::make_shared<TrackedIdentifierExpression>(spIdExpr->GetId(), spIdExpr->GetCpuInformation(), TrackedAddress);
}

Expression::SPtr TrackVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr spTrkIdExpr)
{
  Address TrackedAddress;

  if (m_IsAssigned)
  {
    TrackedAddress = m_CurAddr;
    m_rCtxt.TrackId(spTrkIdExpr->GetId(), m_CurAddr);
  }
  else
  {
    if (!m_rCtxt.GetTrackAddress(spTrkIdExpr->GetId(), TrackedAddress))
      return Expr::MakeId(spTrkIdExpr->GetId(), spTrkIdExpr->GetCpuInformation()); // TODO: Do we really need to remove the track?
  }

  return std::make_shared<TrackedIdentifierExpression>(spTrkIdExpr->GetId(), spTrkIdExpr->GetCpuInformation(), TrackedAddress);
}

Expression::SPtr BackTrackVisitor::VisitAssignment(AssignmentExpression::SPtr spAssignExpr)
{
  m_IsAssigned = true;
  spAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  // TODO: can we have 2 OpAff in the same expression?
  spAssignExpr->GetSourceExpression()->Visit(this);
  m_TrackSource = false;

  return nullptr;
}

Expression::SPtr BackTrackVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr spTrkIdExpr)
{
  if (m_IsAssigned)
  {
    if (m_rBtCtxt.IsTracked(std::make_tuple(spTrkIdExpr->GetId(), spTrkIdExpr->GetCurrentAddress())))
    {
      m_Result = true;
      m_TrackSource = true;
    }
  }

  else if (!m_IsAssigned && m_TrackSource)
  {
    m_rBtCtxt.TrackId(std::make_tuple(spTrkIdExpr->GetId(), spTrkIdExpr->GetCurrentAddress()));
  }

  return nullptr;
}

EvaluateVisitor::EvaluateVisitor(Document const& rDoc, u32 PcBaseId, u32 PcOffId, Address const& rCurAddr)
: m_rDoc(rDoc), m_PcBaseId(PcBaseId), m_PcOffId(PcOffId), m_rCurAddr(rCurAddr)
, m_spResExpr(nullptr), m_IsSymbolic(false)
{
}

Expression::SPtr EvaluateVisitor::VisitSystem(SystemExpression::SPtr spSysExpr)
{
  return nullptr;
}

Expression::SPtr EvaluateVisitor::VisitBind(BindExpression::SPtr spBindExpr)
{
  return nullptr;
}

Expression::SPtr EvaluateVisitor::VisitTernaryCondition(TernaryConditionExpression::SPtr spTernExpr)
{
  return nullptr;
}

Expression::SPtr EvaluateVisitor::VisitIfElseCondition(IfElseConditionExpression::SPtr spIfElseExpr)
{
  return nullptr;
}

Expression::SPtr EvaluateVisitor::VisitWhileCondition(WhileConditionExpression::SPtr spWhileExpr)
{
  return nullptr;
}

Expression::SPtr EvaluateVisitor::VisitAssignment(AssignmentExpression::SPtr spAssignExpr)
{
  m_spResExpr = spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPtr EvaluateVisitor::VisitOperation(OperationExpression::SPtr spOpExpr)
{
  auto spLExpr = expr_cast<ConstantExpression>(spOpExpr->GetLeftExpression()->Visit(this));
  auto spRExpr = expr_cast<ConstantExpression>(spOpExpr->GetRightExpression()->Visit(this));

  if (spLExpr == nullptr || spRExpr == nullptr)
    return nullptr;

  u32 Bit = std::max(spLExpr->GetSizeInBit(), spRExpr->GetSizeInBit());
  u64 Result = 0;

  switch (spOpExpr->GetOperation())
  {
  default:
    return nullptr;

  //case OpXchg:
    //return nullptr;

  case OperationExpression::OpAnd:
    Result = spLExpr->GetConstant() & spRExpr->GetConstant();
    break;

  case OperationExpression::OpOr:
    Result = spLExpr->GetConstant() | spRExpr->GetConstant();
    break;

  case OperationExpression::OpXor:
    Result = spLExpr->GetConstant() ^ spRExpr->GetConstant();
    break;

  case OperationExpression::OpLls:
    Result = spLExpr->GetConstant() << spRExpr->GetConstant();
    break;

  case OperationExpression::OpLrs:
    Result = spLExpr->GetConstant() >> spRExpr->GetConstant();
    break;

  //case OperationExpression::OpArs: // TODO: required sign extend

  case OperationExpression::OpAdd:
    Result = spLExpr->GetConstant() + spRExpr->GetConstant();
    break;

  case OperationExpression::OpSub:
    Result = spLExpr->GetConstant() - spRExpr->GetConstant();
    break;

  case OperationExpression::OpMul:
    Result = spLExpr->GetConstant() * spRExpr->GetConstant();
    break;

  //case OperationExpression::OpSDiv:

  case OperationExpression::OpUDiv:
    Result = spLExpr->GetConstant() / spRExpr->GetConstant();
    break;

  //case OperationExpression::OpSext:
  }

  return Expr::MakeConst(Bit, Result);
}

Expression::SPtr EvaluateVisitor::VisitConstant(ConstantExpression::SPtr spConstExpr)
{
  return spConstExpr;
}

Expression::SPtr EvaluateVisitor::VisitIdentifier(IdentifierExpression::SPtr spIdExpr)
{
  if (m_PcBaseId == spIdExpr->GetId())
    return Expr::MakeConst(m_rCurAddr.GetBaseSize(), m_rCurAddr.GetBase());
  if (m_PcOffId == spIdExpr->GetId())
    return Expr::MakeConst(m_rCurAddr.GetOffsetSize(), m_rCurAddr.GetOffset());
  return nullptr;
}

Expression::SPtr EvaluateVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr spTrkIdExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

template<typename T>
bool ReadType(BinaryStream const& rBinStrm, TOffset FileOff, u64& rValue)
{
  T Value;
  if (!rBinStrm.Read(FileOff, Value))
    return false;
  rValue = Value;
  return true;
};

Expression::SPtr EvaluateVisitor::VisitMemory(MemoryExpression::SPtr spMemExpr)
{
  u16 Base = 0;
  Expression::SPtr spBaseConst;

  if (spMemExpr->GetBaseExpression() != nullptr)
  {
    spBaseConst = expr_cast<ConstantExpression>(spMemExpr->GetBaseExpression()->Visit(this));
    if (spBaseConst == nullptr)
    {
      m_IsSymbolic = true;
      return nullptr;
    }
  }

  auto spOffConst = expr_cast<ConstantExpression>(spMemExpr->GetOffsetExpression()->Visit(this));
  if (spOffConst == nullptr)
  {
    m_IsSymbolic = true;
    return nullptr;
  }

  Address CurAddr(Base, spOffConst->GetConstant());

  // HACK: Since we can't perform what an executable loader do (like import resolving)
  // We have to keep this label unvisited to avoid to read a meaningless value.
  auto Lbl = m_rDoc.GetLabelFromAddress(CurAddr);
  if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
  {
    return Expr::MakeMem(spMemExpr->GetAccessSizeInBit(), spBaseConst, spOffConst, spMemExpr->IsDereferencable());
  }

  TOffset FileOff;
  if (!m_rDoc.ConvertAddressToFileOffset(CurAddr, FileOff))
    return nullptr;
  auto const& rBinStrm = m_rDoc.GetBinaryStream();
  u64 Value;

  switch (spMemExpr->GetAccessSizeInBit())
  {
  case 8:
    if (!ReadType<u8>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 16:
    if (!ReadType<u16>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 32:
    if (!ReadType<u32>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  case 64:
    if (!ReadType<u64>(rBinStrm, FileOff, Value))
      return nullptr;
    break;

  default:
    return nullptr;
  }

  return Expr::MakeConst(spMemExpr->GetAccessSizeInBit(), Value);
}

Expression::SPtr EvaluateVisitor::VisitSymbolic(SymbolicExpression::SPtr spSymExpr)
{
  m_IsSymbolic = true;
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

TrackedIdPropagation::TrackedIdPropagation(Expression::List& rExprs, u32 Id) : m_rExprs(rExprs), m_spResExpr(nullptr)
{
  // Find the track id expression
  auto FindTrkIdExpr = [&](Expression::SPtr spExpr) -> Expression::SPtr
  {
    auto spAssign = expr_cast<AssignmentExpression>(spExpr);
    if (spAssign == nullptr)
      return nullptr;
    auto spDstExpr = spAssign->GetDestinationExpression();
    if (spDstExpr->GetClassKind() != Expression::TrackedId)
      return nullptr;
    return spDstExpr;
  };

  for (auto pExpr : m_rExprs)
  {
    Log::Write("core") << "TIP input: " << pExpr->ToString() << LogEnd;
  }

  for (auto itExpr = m_rExprs.begin(); itExpr != m_rExprs.end(); ++itExpr)
  {
    FilterVisitor FltVst(FindTrkIdExpr, 1);
    (*itExpr)->Visit(&FltVst);
    auto rFltExprs = FltVst.GetMatchedExpressions();

    if (rFltExprs.size() != 1)
      continue;

    m_spResExpr = expr_cast<AssignmentExpression>(*itExpr);

    break;
  }
}

bool TrackedIdPropagation::_RunOnce(void)
{
  if (m_spResExpr == nullptr)
    return false;

  // this functor is used to retrieve the source of a specific tracked id
  struct FindTrkSrc
  {
    FindTrkSrc(void) : m_IsAssigned(false) {}

    Expression::SPtr operator()(Expression::SPtr spExpr)
    {
      switch (spExpr->GetClassKind())
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
        return spExpr;

      default:
        m_IsAssigned = false;
        break;
      }

      return nullptr;
    }

    bool m_IsAssigned;
  };

  FindTrkSrc Matcher;
  FilterVisitor FindTrkSrcVst(Matcher, 1);

  m_spResExpr->Visit(&FindTrkSrcVst);

  auto rExprs = FindTrkSrcVst.GetMatchedExpressions();

  // If there's no more tracked id, it means we've finish
  if (rExprs.empty())
  {
    m_IsDone = true;
    return true;
  }

  assert(rExprs.size() == 1);
  auto spRes = expr_cast<TrackedIdentifierExpression>(rExprs.front());
  assert(spRes->GetClassKind() == Expression::TrackedId);

  auto spAssignTrkExpr = expr_cast<AssignmentExpression>(__FindTrackedIdExpression(spRes->GetId(), spRes->GetCurrentAddress()));
  if (spAssignTrkExpr == nullptr)
    return false;

  m_spResExpr->UpdateChild(spRes, spAssignTrkExpr->GetSourceExpression());

  return true;
}

bool TrackedIdPropagation::_Finalize(void)
{
  if (m_spResExpr == nullptr)
    return false;

  m_rExprs.clear();
  m_rExprs.push_back(m_spResExpr);

  return true;
}

Expression::SPtr TrackedIdPropagation::__FindTrackedIdExpression(u32 Id, Address const& rAddr)
{
  auto FindTrkByIdAddr = [&](Expression::SPtr spExpr) -> Expression::SPtr
  {
    auto spAssignExpr = expr_cast<AssignmentExpression>(spExpr);
    if (spAssignExpr == nullptr)
      return nullptr;

    auto spTrkId = expr_cast<TrackedIdentifierExpression>(spAssignExpr->GetDestinationExpression());
    if (spTrkId == nullptr)
      return nullptr;

    if (spTrkId->GetId() != Id || spTrkId->GetCurrentAddress() != rAddr)
      return nullptr;

    return spExpr;
  };

  for (auto pExpr : m_rExprs)
  {
    FilterVisitor FindTrkByIdAddrVst(FindTrkByIdAddr);
    pExpr->Visit(&FindTrkByIdAddrVst);

    auto Exprs = FindTrkByIdAddrVst.GetMatchedExpressions();
    if (Exprs.size() != 1)
      continue;

    return Exprs.front();
  }

  return nullptr;
}

NormalizeExpression::NormalizeExpression(Expression::SPtr spExpr) : m_spExpr(spExpr)
{
}

bool NormalizeExpression::_RunOnce(void)
{
  // TODO: handle all cases and associativity/commutativity
  class SwapVisitor : public ExpressionVisitor
  {
  public:
    SwapVisitor(void) : m_IsDirty(false) {}
    virtual Expression::SPtr VisitOperation(OperationExpression::SPtr spOpExpr)
    {
      ExpressionVisitor::VisitOperation(spOpExpr);

      if (spOpExpr->GetLeftExpression()->IsKindOf(Expression::Op))
      {
        m_IsDirty = true;
        spOpExpr->SwapExpressions();
      }

      if (spOpExpr->GetLeftExpression()->IsKindOf(Expression::Const))
      {
        if (spOpExpr->GetRightExpression()->IsKindOf(Expression::Id))
        {
          m_IsDirty = true;
          spOpExpr->SwapExpressions();
          return nullptr;
        }

        auto spROpExpr = expr_cast<OperationExpression>(spOpExpr->GetRightExpression());
        if (spROpExpr != nullptr && spROpExpr->GetLeftExpression()->IsKindOf(Expression::Id))
        {
          m_IsDirty = true;
          spOpExpr->SwapLeftExpressions(spROpExpr);
          return nullptr;
        }
      }
      return nullptr;
    }

    bool IsDirty(void) const { return m_IsDirty; }

  private:
    bool m_IsDirty;
  };

  SwapVisitor SwpVst;
  m_spExpr->Visit(&SwpVst);
  if (!SwpVst.IsDirty())
    m_IsDone = true;
  return true;
}

bool NormalizeExpression::_Finalize(void)
{
  return true;
}

ConstantPropagation::ConstantPropagation(Expression::SPtr spExpr) : m_spExpr(spExpr)
{
}

bool ConstantPropagation::_RunOnce(void)
{
  auto FindOpWithConst = [](Expression::SPtr spExpr) -> Expression::SPtr
  {
    auto spOpExpr = expr_cast<OperationExpression>(spExpr);
    if (spOpExpr == nullptr)
      return nullptr;
    if (spOpExpr->GetLeftExpression()->GetClassKind() != Expression::Const ||
      spOpExpr->GetRightExpression()->GetClassKind() != Expression::Const)
      return nullptr;

    return spExpr;
  };

  FilterVisitor FltVst(FindOpWithConst, 1);
  m_spExpr->Visit(&FltVst);
  auto Exprs = FltVst.GetMatchedExpressions();
  if (Exprs.empty())
  {
    m_IsDone = true;
    return true;
  }
  auto spOpExpr = expr_cast<OperationExpression>(Exprs.front());
  if (spOpExpr == nullptr)
    return false;
  auto spLConstExpr = expr_cast<ConstantExpression>(spOpExpr->GetLeftExpression());
  auto spRConstExpr = expr_cast<ConstantExpression>(spOpExpr->GetRightExpression());

  if (spLConstExpr == nullptr || spRConstExpr == nullptr)
    return false;

  u64 Res = 0;
  u32 Bit = std::max(spLConstExpr->GetSizeInBit(), spRConstExpr->GetSizeInBit()); // NOTE: cast to the larger type

  switch (spOpExpr->GetOperation())
  {
  default:
    return false;

  case OperationExpression::OpAdd:
    Res = spLConstExpr->GetConstant() + spRConstExpr->GetConstant();
    break;

  case OperationExpression::OpSub:
    Res = spLConstExpr->GetConstant() - spRConstExpr->GetConstant();
    break;

    // TODO: handle all operations...
  }

  auto spConstExpr = Expr::MakeConst(Bit, Res);
  return m_spExpr->UpdateChild(spOpExpr, spConstExpr);
}

bool ConstantPropagation::_Finalize(void)
{
  return true;
}

// helper /////////////////////////////////////////////////////////////////////

Expression::SPtr Expr::MakeConst(u32 ConstType, u64 Value)
{
  return std::make_shared<ConstantExpression>(ConstType, Value);
}

Expression::SPtr Expr::MakeBoolean(bool Value)
{
  return std::make_shared<ConstantExpression>(ConstantExpression::Const1Bit, Value ? 1 : 0);
}

Expression::SPtr Expr::MakeId(u32 Id, CpuInformation const* pCpuInfo)
{
  return std::make_shared<IdentifierExpression>(Id, pCpuInfo);
}

Expression::SPtr Expr::MakeMem(u32 AccessSize, Expression::SPtr spExprBase, Expression::SPtr spExprOffset, bool Dereference)
{
  return std::make_shared<MemoryExpression>(AccessSize, spExprBase, spExprOffset, Dereference);
}

Expression::SPtr Expr::MakeTernaryCond(ConditionExpression::Type CondType, Expression::SPtr spRefExpr, Expression::SPtr spTestExpr, Expression::SPtr spTrueExpr, Expression::SPtr spFalseExpr)
{
  return std::make_shared<TernaryConditionExpression>(CondType, spRefExpr, spTestExpr, spTrueExpr, spFalseExpr);
}

Expression::SPtr Expr::MakeIfElseCond(ConditionExpression::Type CondType, Expression::SPtr spRefExpr, Expression::SPtr spTestExpr, Expression::SPtr spThenExpr, Expression::SPtr spElseExpr)
{
  return std::make_shared<IfElseConditionExpression>(CondType, spRefExpr, spTestExpr, spThenExpr, spElseExpr);
}

Expression::SPtr Expr::MakeWhileCond(ConditionExpression::Type CondType, Expression::SPtr spRefExpr, Expression::SPtr spTestExpr, Expression::SPtr spBodyExpr)
{
  return std::make_shared<WhileConditionExpression>(CondType, spRefExpr, spTestExpr, spBodyExpr);
}

Expression::SPtr Expr::MakeAssign(Expression::SPtr spDstExpr, Expression::SPtr spSrcExpr)
{
  return std::make_shared<AssignmentExpression>(spDstExpr, spSrcExpr);
}

Expression::SPtr Expr::MakeOp(OperationExpression::Type OpType, Expression::SPtr spLeftExpr, Expression::SPtr spRightExpr)
{
  return std::make_shared<OperationExpression>(OpType, spLeftExpr, spRightExpr);
}

Expression::SPtr Expr::MakeBind(Expression::List const& rExprs)
{
  return std::make_shared<BindExpression>(rExprs);
}

Expression::SPtr Expr::MakeSym(SymbolicExpression::Type SymType, std::string const& rValue)
{
  return std::make_shared<SymbolicExpression>(SymType, rValue);
}
