#include "medusa/expression_simplifier.hpp"
#include "medusa/expression_visitor.hpp"

MEDUSA_NAMESPACE_BEGIN

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

TrackedIdPropagation::TrackedIdPropagation(Expression::LSPType & rExprs, u32 Id) : m_rExprs(rExprs), m_spResExpr(nullptr)
{
  // Find the track id expression
  auto FindTrkIdExpr = [&](Expression::SPType spExpr) -> Expression::SPType
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

    Expression::SPType operator()(Expression::SPType spExpr)
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

Expression::SPType TrackedIdPropagation::__FindTrackedIdExpression(u32 Id, Address const& rAddr)
{
  auto FindTrkByIdAddr = [&](Expression::SPType spExpr) -> Expression::SPType
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

NormalizeExpression::NormalizeExpression(Expression::SPType spExpr) : m_spExpr(spExpr)
{
}

bool NormalizeExpression::_RunOnce(void)
{
  // TODO: handle all cases and associativity/commutativity
  class SwapVisitor : public ExpressionVisitor
  {
  public:
    SwapVisitor(void) : m_IsDirty(false) {}
    virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
    {
      ExpressionVisitor::VisitBinaryOperation(spBinOpExpr);

      if (spBinOpExpr->GetLeftExpression()->IsKindOf(Expression::BinOp))
      {
        m_IsDirty = true;
        spBinOpExpr->SwapExpressions();
      }

      if (spBinOpExpr->GetLeftExpression()->IsKindOf(Expression::Const))
      {
        if (spBinOpExpr->GetRightExpression()->IsKindOf(Expression::Id))
        {
          m_IsDirty = true;
          spBinOpExpr->SwapExpressions();
          return nullptr;
        }

        auto spRBinOpExpr = expr_cast<BinaryOperationExpression>(spBinOpExpr->GetRightExpression());
        if (spRBinOpExpr != nullptr && spRBinOpExpr->GetLeftExpression()->IsKindOf(Expression::Id))
        {
          m_IsDirty = true;
          spBinOpExpr->SwapLeftExpressions(spRBinOpExpr);
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

ConstantPropagation::ConstantPropagation(Expression::SPType spExpr) : m_spExpr(spExpr)
{
}

bool ConstantPropagation::_RunOnce(void)
{
  auto FindOpWithConst = [](Expression::SPType spExpr) -> Expression::SPType
  {
    auto spBinOpExpr = expr_cast<BinaryOperationExpression>(spExpr);
    if (spBinOpExpr == nullptr)
      return nullptr;
    if (spBinOpExpr->GetLeftExpression()->GetClassKind() != Expression::Const ||
      spBinOpExpr->GetRightExpression()->GetClassKind() != Expression::Const)
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
  auto spBinOpExpr = expr_cast<BinaryOperationExpression>(Exprs.front());
  if (spBinOpExpr == nullptr)
    return false;
  auto spLConstExpr = expr_cast<ConstantExpression>(spBinOpExpr->GetLeftExpression());
  auto spRConstExpr = expr_cast<ConstantExpression>(spBinOpExpr->GetRightExpression());

  if (spLConstExpr == nullptr || spRConstExpr == nullptr)
    return false;

  u64 Res = 0;
  u32 Bit = std::max(spLConstExpr->GetSizeInBit(), spRConstExpr->GetSizeInBit()); // NOTE: cast to the larger type

  switch (spBinOpExpr->GetOperation())
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
  return m_spExpr->UpdateChild(spBinOpExpr, spConstExpr);
}

bool ConstantPropagation::_Finalize(void)
{
  return true;
}

MEDUSA_NAMESPACE_END