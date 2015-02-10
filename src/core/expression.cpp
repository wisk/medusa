#include "medusa/expression.hpp"
#include "medusa/extend.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/reversed.hpp>

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

Expression::SPType SystemExpression::Clone(void) const
{
  return std::make_shared<SystemExpression>(m_Name);
}

Expression::SPType SystemExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitSystem(shared_from_this());
}

// bind expression ////////////////////////////////////////////////////////////

BindExpression::BindExpression(Expression::LSPType const& rExprs)
: m_Expressions(rExprs)
{
}

BindExpression::~BindExpression(void)
{
}

std::string BindExpression::ToString(void) const
{
  std::list<std::string> ExprsStrList;

  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [&](Expression::SPType spExpr)
  {
    ExprsStrList.push_back(spExpr->ToString());
  });

  return boost::algorithm::join(ExprsStrList, "; ");
}

Expression::SPType BindExpression::Clone(void) const
{
  Expression::LSPType ExprListCloned;
  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [&](Expression::SPType spExpr)
  {
    ExprListCloned.push_back(spExpr->Clone());
  });

  return std::make_shared<BindExpression>(ExprListCloned);
}

Expression::SPType BindExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitBind(std::static_pointer_cast<BindExpression>(shared_from_this()));
}

bool BindExpression::UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr)
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

ConditionExpression::ConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr)
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

TernaryConditionExpression::TernaryConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr)
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

Expression::SPType TernaryConditionExpression::Clone(void) const
{
  return std::make_shared<TernaryConditionExpression>(
    m_Type,
    m_spRefExpr->Clone(),
    m_spTestExpr->Clone(),
    m_spTrueExpr->Clone(),
    m_spFalseExpr->Clone());
}

Expression::SPType TernaryConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitTernaryCondition(std::static_pointer_cast<TernaryConditionExpression>(shared_from_this()));
}

bool TernaryConditionExpression::UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr)
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

IfElseConditionExpression::IfElseConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr)
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

Expression::SPType IfElseConditionExpression::Clone(void) const
{
  return Expr::MakeIfElseCond(
    m_Type,
    m_spRefExpr->Clone(), m_spTestExpr->Clone(),
    m_spThenExpr->Clone(), m_spElseExpr != nullptr ? m_spThenExpr->Clone() : nullptr);
}

Expression::SPType IfElseConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitIfElseCondition(std::static_pointer_cast<IfElseConditionExpression>(shared_from_this()));
}

bool IfElseConditionExpression::UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr)
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

WhileConditionExpression::WhileConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr)
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

Expression::SPType WhileConditionExpression::Clone(void) const
{
  return std::make_shared<WhileConditionExpression>(m_Type, m_spRefExpr->Clone(), m_spTestExpr->Clone(), m_spBodyExpr->Clone());
}

Expression::SPType WhileConditionExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitWhileCondition(std::static_pointer_cast<WhileConditionExpression>(shared_from_this()));
}

bool WhileConditionExpression::UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr)
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

AssignmentExpression::AssignmentExpression(Expression::SPType spDstExpr, Expression::SPType spSrcExpr)
: m_spDstExpr(spDstExpr), m_spSrcExpr(spSrcExpr)
{
  assert(spDstExpr != nullptr && "Destination expression is null");
  assert(spSrcExpr != nullptr && "Source expression is null");
}

AssignmentExpression::~AssignmentExpression(void)
{
}

std::string AssignmentExpression::ToString(void) const
{
  return (boost::format("(%1% = %2%)") % m_spDstExpr->ToString() % m_spSrcExpr->ToString()).str();
}

Expression::SPType AssignmentExpression::Clone(void) const
{
  return std::make_shared<AssignmentExpression>(m_spDstExpr->Clone(), m_spSrcExpr->Clone());
}

Expression::SPType AssignmentExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitAssignment(std::static_pointer_cast<AssignmentExpression>(shared_from_this()));
}

bool AssignmentExpression::UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr)
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

OperationExpression::OperationExpression(Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr)
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
  static const char *s_StrOp[] = { "???", "↔", "&", "|", "^", "<<", ">>", ">>(s)", "+", "-", "*", "/(s)", "/(u)" };

  if (m_spLeftExpr == nullptr || m_spRightExpr == nullptr)


    return "";

  auto LeftStr = m_spLeftExpr->ToString();

  auto RightStr = m_spRightExpr->ToString();


  if (LeftStr.empty() || RightStr.empty())
    return "";

  if (m_OpType == OpSext)
    return (boost::format("sext<%2%>(%1%)") % LeftStr % RightStr).str();

  if (m_OpType >= (sizeof(s_StrOp) / sizeof(*s_StrOp)))
    return "";

  return (boost::format("(%1% %2% %3%)") % LeftStr % s_StrOp[m_OpType] % RightStr).str();
}

Expression::SPType OperationExpression::Clone(void) const
{
  return std::make_shared<OperationExpression>(static_cast<Type>(m_OpType), m_spLeftExpr->Clone(), m_spRightExpr->Clone());
}

u32 OperationExpression::GetSizeInBit(void) const
{
  return std::max(m_spLeftExpr->GetSizeInBit(), m_spRightExpr->GetSizeInBit());
}

Expression::SPType OperationExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitOperation(std::static_pointer_cast<OperationExpression>(shared_from_this()));
}

bool OperationExpression::UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr)
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

void OperationExpression::SwapLeftExpressions(OperationExpression::SPType spOpExpr)
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

Expression::SPType ConstantExpression::Clone(void) const
{
  return std::make_shared<ConstantExpression>(m_ConstType, m_Value);
}

Expression::SPType ConstantExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitConstant(std::static_pointer_cast<ConstantExpression>(shared_from_this()));
}

bool ConstantExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const
{
  if (rData.size() != 1)
    return false;

  rData.front() = std::make_tuple(m_ConstType, m_Value);
  return true;
}

bool ConstantExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData)
{
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

Expression::SPType IdentifierExpression::Clone(void) const
{
  return std::make_shared<IdentifierExpression>(m_Id, m_pCpuInfo);

}

u32 IdentifierExpression::GetSizeInBit(void) const
{
  return m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);

}

Expression::SPType IdentifierExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitIdentifier(std::static_pointer_cast<IdentifierExpression>(shared_from_this()));
}

bool IdentifierExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const
{
  if (rData.size() != 1)
    return false;

  u32 RegSize = m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);

  u64 Value = 0;
  if (!pCpuCtxt->ReadRegister(m_Id, &Value, RegSize / 8))
    return false;

  rData.front() = std::make_tuple(RegSize, Value);
  return true;
}

bool IdentifierExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData)
{
  auto DataValue = rData.front();

  // FIXME: it seems to cause issue with movsx instruction
  u32 RegSize = m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);
  if (RegSize != std::get<0>(DataValue))
    return false;

  u64 RegVal = std::get<1>(DataValue).convert_to<u64>();
  if (!pCpuCtxt->WriteRegister(m_Id, &RegVal, RegSize / 8))
    return false;

  rData.pop_front();
  return true;
}

bool IdentifierExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
{
  return false;
}

VectorIdentifierExpression::VectorIdentifierExpression(std::vector<u32> const& rVecId, CpuInformation const* pCpuInfo)
  : m_VecId(rVecId), m_pCpuInfo(pCpuInfo)
{
}

std::string VectorIdentifierExpression::ToString(void) const
{
  std::vector<std::string> VecStr;
  VecStr.reserve(m_VecId.size());
  for (auto Id : m_VecId)
  {
    auto pCurIdStr = m_pCpuInfo->ConvertIdentifierToName(Id);
    if (pCurIdStr == nullptr)
      return "";
    VecStr.push_back(pCurIdStr);
  }
  return "{ " + boost::algorithm::join(VecStr, ", ") + " }";
}

Expression::SPType VectorIdentifierExpression::Clone(void) const
{
  return std::make_shared<VectorIdentifierExpression>(m_VecId, m_pCpuInfo);
}

u32 VectorIdentifierExpression::GetSizeInBit(void) const
{
  u32 SizeInBit = 0;
  for (auto Id : m_VecId)
  {
    SizeInBit += m_pCpuInfo->GetSizeOfRegisterInBit(Id);
  }
  return SizeInBit;
}

Expression::SPType VectorIdentifierExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitVectorIdentifier(std::static_pointer_cast<VectorIdentifierExpression>(shared_from_this()));
}

void VectorIdentifierExpression::Prepare(DataContainerType& rData) const
{
  rData.resize(2);
}

bool VectorIdentifierExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const
{
  // FIXME: we ignore the initial size of rData here
  rData.clear();

  for (auto Id : m_VecId)
  {
    u32 RegSize = m_pCpuInfo->GetSizeOfRegisterInBit(Id);
    u64 RegValue = 0;
    if (!pCpuCtxt->ReadRegister(Id, &RegValue, RegSize / 8))
      return false;
    rData.push_front(std::make_tuple(RegSize, RegValue));
  }
  return true;
}

bool VectorIdentifierExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData)
{
  for (auto Id : boost::adaptors::reverse(m_VecId))
  {
    if (rData.empty())
      return false;
    u32 RegSize = m_pCpuInfo->GetSizeOfRegisterInBit(Id);
    auto DataValue = rData.front();
    if (!pCpuCtxt->WriteRegister(Id, &std::get<1>(DataValue), RegSize / 8))
      return false;
    rData.pop_front();
  }
  return true;
}

bool VectorIdentifierExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
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

Expression::SPType TrackedIdentifierExpression::Clone(void) const
{
  return std::make_shared<TrackedIdentifierExpression>(m_Id, m_pCpuInfo, m_CurAddr);

}

u32 TrackedIdentifierExpression::GetSizeInBit(void) const
{
  return m_pCpuInfo->GetSizeOfRegisterInBit(m_Id);

}

Expression::SPType TrackedIdentifierExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitTrackedIdentifier(std::static_pointer_cast<TrackedIdentifierExpression>(shared_from_this()));
}

// memory expression //////////////////////////////////////////////////////////

MemoryExpression::MemoryExpression(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference)
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

Expression::SPType MemoryExpression::Clone(void) const
{
  if (m_spBaseExpr == nullptr)
    return std::make_shared<MemoryExpression>(m_AccessSizeInBit, nullptr, m_spOffExpr->Clone(), m_Dereference);

  return std::make_shared<MemoryExpression>(m_AccessSizeInBit, m_spBaseExpr->Clone(), m_spOffExpr->Clone(), m_Dereference);
}

u32 MemoryExpression::GetSizeInBit(void) const
{
  return m_AccessSizeInBit;
}

Expression::SPType MemoryExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitMemory(std::static_pointer_cast<MemoryExpression>(shared_from_this()));
}

bool MemoryExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const
{
  Address DstAddr;
  if (GetAddress(pCpuCtxt, pMemCtxt, DstAddr) == false)
    return false;

  u64 LinAddr = 0;
  if (pCpuCtxt->Translate(DstAddr, LinAddr) == false)
    LinAddr = DstAddr.GetOffset();

  if (m_Dereference == true)
  {
    for (auto& rDataValue : rData)
    {
      u64 MemVal = 0;
      if (!pMemCtxt->ReadMemory(LinAddr, &MemVal, m_AccessSizeInBit / 8))
        return false;
      rDataValue = std::make_tuple(m_AccessSizeInBit, MemVal);
    }
  }
  else
  {
    if (rData.size() != 1)
      return false;
    rData.front() = std::make_tuple(m_AccessSizeInBit, LinAddr);
  }


  return true;
}

bool MemoryExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData)
{
  // When we dereference this expression, we actually have to write stuff in memory
  if (m_Dereference)
  {
    Address DstAddr;
    if (GetAddress(pCpuCtxt, pMemCtxt, DstAddr) == false)
      return false;
    u64 LinAddr = 0;
    if (pCpuCtxt->Translate(DstAddr, LinAddr) == false)
      LinAddr = DstAddr.GetOffset();

    for (auto const& rDataValue : rData)
    {
      if (!pMemCtxt->WriteMemory(LinAddr, &std::get<1>(rDataValue), std::get<0>(rDataValue) / 8))
        return false;
      LinAddr += std::get<0>(rDataValue) / 8;
    }
  }
  // but if it's just an addressing operation, we have to make sure the address is moved
  // TODO: however, this kind of operation could modify both base and offset.
  // At this time we only modify the offset value if it's a register (otherwise it has to fail)
  else
  {
    // We only handle one data
    if (rData.size() != 1)
      return false;

    auto spRegOff = expr_cast<IdentifierExpression>(m_spOffExpr);
    if (spRegOff == nullptr)
      return false;

    //auto DataSize = std::get<0>(rData.front());
    auto DataVal = std::get<1>(rData.front());
    u64 DataToWrite = DataVal.convert_to<u64>(); // TODO: is it mandatory?
    if (!pCpuCtxt->WriteRegister(spRegOff->GetId(), &DataToWrite, spRegOff->GetSizeInBit() / 8))
      return false;
  }
  return true;
}

bool MemoryExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
{
  Expression::DataContainerType BaseData(1), OffsetData(1);

  if (m_spBaseExpr != nullptr)
    if (m_spBaseExpr->Read(pCpuCtxt, pMemCtxt, BaseData) == false)
      return false;
  if (m_spOffExpr->Read(pCpuCtxt, pMemCtxt, OffsetData) == false)
    return false;

  TBase Base = BaseData.size() != 1 ? 0x0 : std::get<1>(BaseData.front()).convert_to<TBase>();
  if (OffsetData.size() != 1)
    return false;
  TOffset OffsetValue = std::get<1>(OffsetData.front()).convert_to<TOffset>();
  rAddress = Address(Base, OffsetValue);
  return true;
}

Expression::SPType MemoryExpression::ToAddress(void) const
{
  return Expr::MakeMem(m_AccessSizeInBit,
    m_spBaseExpr,
    m_spOffExpr,
    false);
}

bool MemoryExpression::UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr)
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

Expression::SPType SymbolicExpression::Clone(void) const
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

Expression::SPType SymbolicExpression::Visit(ExpressionVisitor* pVisitor)
{
  return pVisitor->VisitSymbolic(std::static_pointer_cast<SymbolicExpression>(shared_from_this()));
}

// helper /////////////////////////////////////////////////////////////////////

Expression::SPType Expr::MakeConst(u32 ConstType, u64 Value)
{
  return std::make_shared<ConstantExpression>(ConstType, Value);
}

Expression::SPType Expr::MakeBoolean(bool Value)
{
  return std::make_shared<ConstantExpression>(ConstantExpression::Const1Bit, Value ? 1 : 0);
}

Expression::SPType Expr::MakeId(u32 Id, CpuInformation const* pCpuInfo)
{
  return std::make_shared<IdentifierExpression>(Id, pCpuInfo);
}

Expression::SPType Expr::MakeVecId(std::vector<u32> const& rVecId, CpuInformation const* pCpuInfo)
{
  return std::make_shared<VectorIdentifierExpression>(rVecId, pCpuInfo);
}

Expression::SPType Expr::MakeMem(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference)
{
  return std::make_shared<MemoryExpression>(AccessSize, spExprBase, spExprOffset, Dereference);
}

Expression::SPType Expr::MakeTernaryCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr)
{
  return std::make_shared<TernaryConditionExpression>(CondType, spRefExpr, spTestExpr, spTrueExpr, spFalseExpr);
}

Expression::SPType Expr::MakeIfElseCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr)
{
  return std::make_shared<IfElseConditionExpression>(CondType, spRefExpr, spTestExpr, spThenExpr, spElseExpr);
}

Expression::SPType Expr::MakeWhileCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr)
{
  return std::make_shared<WhileConditionExpression>(CondType, spRefExpr, spTestExpr, spBodyExpr);
}

Expression::SPType Expr::MakeAssign(Expression::SPType spDstExpr, Expression::SPType spSrcExpr)
{
  return std::make_shared<AssignmentExpression>(spDstExpr, spSrcExpr);
}

Expression::SPType Expr::MakeOp(OperationExpression::Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr)
{
  return std::make_shared<OperationExpression>(OpType, spLeftExpr, spRightExpr);
}

Expression::SPType Expr::MakeBind(Expression::LSPType const& rExprs)
{
  return std::make_shared<BindExpression>(rExprs);
}

Expression::SPType Expr::MakeSym(SymbolicExpression::Type SymType, std::string const& rValue)
{
  return std::make_shared<SymbolicExpression>(SymType, rValue);
}
