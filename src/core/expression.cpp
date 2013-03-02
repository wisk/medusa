#include "medusa/expression.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

MEDUSA_NAMESPACE_USE

BindExpression::~BindExpression(void)
{
  std::for_each(std::begin(m_Expressions), std::end(m_Expressions), [](Expression *pExpr)
  {
    delete pExpr;
  });
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
    return (boost::format("se%2%(%1%)") % LeftStr % RightStr).str();

  if (m_OpType >= (sizeof(s_StrOp) / sizeof(*s_StrOp)))
    return "";

  return (boost::format("%1% %2% %3%") % LeftStr % s_StrOp[m_OpType] % RightStr).str();
}

Expression *OperationExpression::Clone(void) const
{
  return new OperationExpression(static_cast<Type>(m_OpType), m_pLeftExpr->Clone(), m_pRightExpr->Clone());
}

std::string ConstantExpression::ToString(void) const
{
  std::ostringstream Buffer;
  if (m_ConstType != ConstUnknownBit)
    Buffer << "int" << m_ConstType << "(";

  auto TmpValue = m_Value;
  if (m_Value < 0)
  {
    Buffer << "-";
    TmpValue = (~m_Value) + 1;
  }
  Buffer << "0x" << (boost::format("%x") % TmpValue).str();
  if (m_ConstType != ConstUnknownBit)
    Buffer << ")";
  return Buffer.str();
}

Expression *ConstantExpression::Clone(void) const
{
  return new ConstantExpression(m_ConstType, m_Value);
}

bool ConstantExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue) const
{
  rValue = m_Value;
  return true;
}

bool ConstantExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value)
{
  assert(0);
  return false;
}

bool ConstantExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
{
  return false;
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

bool IdentifierExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue) const
{
  rValue = 0;
  return pCpuCtxt->ReadRegister(m_Id, &rValue, m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) / 8);
}

bool IdentifierExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value)
{
  return pCpuCtxt->WriteRegister(m_Id, &Value, m_pCpuInfo->GetSizeOfRegisterInBit(m_Id) / 8);
}

bool IdentifierExpression::GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const
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
    return new MemoryExpression(m_AccessSizeInBit, nullptr, m_pExprOffset->Clone());

  return new MemoryExpression(m_AccessSizeInBit, m_pExprBase->Clone(), m_pExprOffset->Clone());
}

u32 MemoryExpression::GetSizeInBit(void) const
{
  return m_AccessSizeInBit;
}

bool MemoryExpression::Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue) const
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

bool MemoryExpression::Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value)
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