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
    Log::Write("core").Level(LogDebug) << "TIP input: " << pExpr->ToString() << LogEnd;
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

  if (!m_spResExpr->UpdateChild(spRes, spAssignTrkExpr->GetSourceExpression()))
    return false;

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

TrackedIdMerger::TrackedIdMerger(Expression::SPType spToBeMerged, Expression::VSPType const& rExprsPool)
: m_spMergedExpr(spToBeMerged), m_rExprsPool(rExprsPool)
{

}

bool TrackedIdMerger::_RunOnce(void)
{
  FilterVisitor FindTrkId([&](Expression::SPType spExpr) -> Expression::SPType
  {
    auto spAssignExpr = expr_cast<AssignmentExpression>(spExpr);
    if (spAssignExpr == nullptr)
      return nullptr;
    auto spTrkId = expr_cast<TrackedIdentifierExpression>(spAssignExpr->GetDestinationExpression());
    if (spTrkId == nullptr)
      return nullptr;
    return spAssignExpr;
  });
  for (auto const spExpr : m_rExprsPool)
    spExpr->Visit(&FindTrkId);
  auto TrkInfoExprs = FindTrkId.GetMatchedExpressions();
  if (TrkInfoExprs.empty())
    return false;
  class MergeTrackId : public ExpressionVisitor
  {
  public:
    MergeTrackId(Expression::SPType& rspToBeMerge, Expression::LSPType const& rTrkInfoExprs, bool& rDirty)
      : m_rTrkInfoExprs(rTrkInfoExprs), m_rspToBeMerged(rspToBeMerge), m_rDirty(rDirty) {}

    Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
    {
      for (auto spTrkInfo : m_rTrkInfoExprs)
      {
        auto spAssignExpr = expr_cast<AssignmentExpression>(spTrkInfo);
        if (spAssignExpr == nullptr)
          continue;
        auto spDstTrkId = expr_cast<TrackedIdentifierExpression>(spAssignExpr->GetDestinationExpression());
        if (spDstTrkId == nullptr)
          continue;
        if (spTrkIdExpr->GetCurrentAddress() != spDstTrkId->GetCurrentAddress() || spTrkIdExpr->GetId() != spDstTrkId->GetId())
          continue;
        if (m_rspToBeMerged->UpdateChild(spTrkIdExpr, spAssignExpr->GetSourceExpression()))
          m_rDirty = true;
      }
      return nullptr;
    }

  private:
    Expression::SPType& m_rspToBeMerged;
    Expression::LSPType const& m_rTrkInfoExprs;
    bool& m_rDirty;
  };
  bool Dirty = false;
  MergeTrackId MTI(m_spMergedExpr, TrkInfoExprs, Dirty);
  m_spMergedExpr->Visit(&MTI);
  m_IsDone = Dirty ? false : true;
  return true;
}

bool TrackedIdMerger::_Finalize(void)
{
  return true;
}

NormalizeExpression::NormalizeExpression(Expression::SPType spExpr) : m_spExpr(spExpr)
{
}

bool NormalizeExpression::_RunOnce(void)
{
  return false;
  //// TODO: handle all cases and associativity/commutativity
  //class SwapVisitor : public ExpressionVisitor
  //{
  //public:
  //  SwapVisitor(void) : m_IsDirty(false) {}
  //  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
  //  {
  //    ExpressionVisitor::VisitBinaryOperation(spBinOpExpr);

  //    if (spBinOpExpr->GetLeftExpression()->IsKindOf(Expression::BinOp))
  //    {
  //      m_IsDirty = true;
  //      spBinOpExpr->SwapExpressions();
  //    }

  //    if (spBinOpExpr->GetLeftExpression()->IsKindOf(Expression::Const))
  //    {
  //      if (spBinOpExpr->GetRightExpression()->IsKindOf(Expression::Id))
  //      {
  //        m_IsDirty = true;
  //        spBinOpExpr->SwapExpressions();
  //        return nullptr;
  //      }

  //      auto spRBinOpExpr = expr_cast<BinaryOperationExpression>(spBinOpExpr->GetRightExpression());
  //      if (spRBinOpExpr != nullptr && spRBinOpExpr->GetLeftExpression()->IsKindOf(Expression::Id))
  //      {
  //        m_IsDirty = true;
  //        spBinOpExpr->SwapLeftExpressions(spRBinOpExpr);
  //        return nullptr;
  //      }
  //    }
  //    return nullptr;
  //  }

  //  bool IsDirty(void) const { return m_IsDirty; }

  //private:
  //  bool m_IsDirty;
  //};

  //SwapVisitor SwpVst;
  //m_spExpr->Visit(&SwpVst);
  //if (!SwpVst.IsDirty())
  //  m_IsDone = true;
  //return true;
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
  u32 Bit = std::max(spLConstExpr->GetBitSize(), spRConstExpr->GetBitSize()); // NOTE: cast to the larger type

  switch (spBinOpExpr->GetOperation())
  {
  default:
    return false;

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