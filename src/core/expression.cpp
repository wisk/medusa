#include "medusa/expression.hpp"
#include <sstream>
#include <boost/format.hpp>

MEDUSA_NAMESPACE_USE

std::string OperationExpression::ToString(void) const
{
  static const char *s_StrOp[] = { "???", "=", "&", "|", "^", "+", "-", "*", "/" };

  if (m_pLeftExpr == nullptr || m_pRightExpr == nullptr || m_OpType >= (sizeof(s_StrOp) / sizeof(*s_StrOp)))
    return "";

  auto LeftStr  = m_pLeftExpr->ToString();
  auto RightStr = m_pRightExpr->ToString();

  if (LeftStr.empty() || RightStr.empty())
    return "";

  return (boost::format("%1% %2% %3%") % LeftStr % s_StrOp[m_OpType] % RightStr).str();
}

std::string ConstantExpression::ToString(void) const
{
  // TODO: Handle m_ConstType
  return (boost::format("%#x") % m_Value).str();
}

std::string IdentifierExpression::ToString(void) const
{
  return (boost::format("Id(%d)") % m_Id).str();
}

std::string MemoryExpression::ToString(void) const
{
  switch (m_MemOpType)
  {
  case OpRead:  return (boost::format("Read(%s)")  % m_Address.ToString()).str();
  case OpWrite: return (boost::format("Write(%s)") % m_Address.ToString()).str();
  default:      return "";
  }
}