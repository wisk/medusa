#include "medusa/expression_filter.hpp"

namespace medusa
{
  ExpressionPattern::ExpressionPattern(std::string const& rPatternName)
    : m_PatternName(rPatternName)
  {

  }

  void ExpressionPattern::_CaptureExpressionIfNeeded(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
  {
    if (m_PatternName.empty())
      return;
    rNamedExprs[m_PatternName] = spExpr;
  }

  ExpressionFilter::ExpressionFilter(ExpressionPattern const& rPattern)
    : m_rPattern(rPattern)
  {

  }

  bool ExpressionFilter::Execute(Expression::SPType spExpr)
  {
    return m_rPattern.Filter(m_NamedExprs, spExpr);
  }

  Expression::SPType ExpressionFilter::GetExpression(std::string const& rExprName)
  {
    auto itVarPair = m_NamedExprs.find(rExprName);
    if (itVarPair == std::end(m_NamedExprs))
      return nullptr;
    return itVarPair->second;
  }
}