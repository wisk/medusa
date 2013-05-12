#include "stack_analyzer.hpp"

Expression* ExpressionVisitor_FindOperations::VisitBind(Expression::List const& rExprList)
{
  for (auto itSem = std::begin(rExprList); itSem != std::end(rExprList); ++itSem)
    (*itSem)->Visit(this);
  return nullptr;
}

Expression* ExpressionVisitor_FindOperations::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  // this test matches register initialization with a constant
  if (Type == OperationExpression::OpAff && dynamic_cast<ConstantExpression const*>(pRightExpr))
    return nullptr;

  // in this test, we try to match these cases: id₀ = addr[ id₁ ± off ] or id₀ = id₁ or id₀ ±= off
  //auto pDstExpr = dynamic_cast<IdentifierExpression const*>(pRightExpr);
  //if (pDstExpr == nullptr)
  //  return nullptr;

  // id₀ = addr[ id₁ ± off ]
  std::unique_ptr<OperationExpression const> pSrcExprAddr(dynamic_cast<OperationExpression const*>(pLeftExpr->Visit(this)));
  if (pSrcExprAddr != nullptr)
  {
    // we make sure that the current operation is either add or sub
    if (pSrcExprAddr->GetOperation() != OperationExpression::OpAdd || pSrcExprAddr->GetOperation() != OperationExpression::OpSub)
      return nullptr;

    auto pSrcId  = dynamic_cast<IdentifierExpression const*>(pSrcExprAddr->GetLeftExpression());
    auto pSrcOff = dynamic_cast<ConstantExpression const*>(pSrcExprAddr->GetRightExpression());

    if (pSrcId == nullptr || pSrcOff == nullptr)
      return nullptr;

    // if the source id is not tracked, we leave
    auto itRegOff = std::find(std::begin(m_rRegisterOffsetList), std::end(m_rRegisterOffsetList), pSrcId->GetId());
    if (itRegOff == std::end(m_rRegisterOffsetList))
      return nullptr;

    auto Off = static_cast<s64>(pSrcOff->GetConstant());
    if (pSrcExprAddr->GetOperation() == OperationExpression::OpSub)
      Off = -Off;

    m_CurrentRegisterOffset = RegisterOffset(pSrcId->GetId(), Off + itRegOff->m_Offset);
    return nullptr;
  }

  // id₀ = id₁
  auto pSrcExprId = dynamic_cast<IdentifierExpression const*>(pRightExpr);
  if (pSrcExprId != nullptr)
  {
    // if the source id is not tracked, we leave
    auto itRegOff = std::find(std::begin(m_rRegisterOffsetList), std::end(m_rRegisterOffsetList), pSrcExprId->GetId());
    if (itRegOff == std::end(m_rRegisterOffsetList))
      return nullptr;

    m_CurrentRegisterOffset = RegisterOffset(pSrcExprId->GetId(), itRegOff->m_Offset);
    return nullptr;
  }

  // id₀ ±= off
  auto pSrcExprOprt = dynamic_cast<OperationExpression const*>(pRightExpr);
  if (pSrcExprOprt != nullptr)
  {
    auto pSrcId = dynamic_cast<IdentifierExpression const*>(pSrcExprOprt->GetLeftExpression());
    auto pSrcConst = dynamic_cast<ConstantExpression const*>(pSrcExprOprt->GetRightExpression());

    if (pSrcId == nullptr || pSrcConst == nullptr)
      return nullptr;

    auto Off = static_cast<s64>(pSrcConst->GetConstant());
    if (pSrcExprOprt->GetOperation() == OperationExpression::OpSub)
      Off = -Off;

    auto itRegOff = std::find(std::begin(m_rRegisterOffsetList), std::end(m_rRegisterOffsetList), pSrcId->GetId());
    if (itRegOff == std::end(m_rRegisterOffsetList))
      return nullptr;

    auto pDstExprId = dynamic_cast<IdentifierExpression const*>(pLeftExpr);
    if (pDstExprId != nullptr && pSrcId->GetId() != pDstExprId->GetId())
      m_CurrentRegisterOffset = RegisterOffset(pDstExprId->GetId(), Off + itRegOff->m_Offset);
    else
      m_CurrentRegisterOffset = RegisterOffset(pSrcId->GetId(), Off + itRegOff->m_Offset);

    return nullptr;
  }

  return nullptr;
}

Expression* ExpressionVisitor_FindOperations::VisitConstant(u32 Type, u64 Value)
{
  return nullptr;
}

Expression* ExpressionVisitor_FindOperations::VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo)
{
  return nullptr;
}

Expression* ExpressionVisitor_FindOperations::VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)
{
  if (Deref == true)
    return nullptr;

  return pOffsetExpr->Clone();
}

X86StackAnalyzerTracker::X86StackAnalyzerTracker(CpuInformation const* pCpuInfo) : m_pCpuInfo(pCpuInfo)
{
  m_RegisterOffsetList.push_back(ExpressionVisitor_FindOperations::RegisterOffset(pCpuInfo->GetRegisterByType(CpuInformation::StackPointerRegister)));
}

bool X86StackAnalyzerTracker::Track(Analyzer& rAnlz, Database& rDb, Address const& rAddr)
{
  auto pInsn = dynamic_cast<Instruction*>(rAnlz.GetCell(rDb, rDb.GetFileBinaryStream(), rAddr));
  if (pInsn == nullptr)
    return false;
  if (pInsn->GetOperationType() == Instruction::OpRet)
    return false;

  ExpressionVisitor_FindOperations fo(m_RegisterOffsetList);
  auto Sem = pInsn->GetSemantic();
  for (auto itSem = std::begin(Sem); itSem != std::end(Sem); ++itSem)
  {
    (*itSem)->Visit(&fo);
  }
  auto& rCurRegOff = fo.GetCurrentRegisterOffset();
  if (rCurRegOff.m_Id != 0)
  {
    if (pInsn->GetOperationType() == Instruction::OpCall)
    {
      rCurRegOff.m_Offset += m_pCpuInfo->GetSizeOfRegisterInBit(rCurRegOff.m_Id) / 8;
    }
    auto itRegOff = std::find(std::begin(m_RegisterOffsetList), std::end(m_RegisterOffsetList), rCurRegOff.m_Id);
    if (itRegOff == std::end(m_RegisterOffsetList))
      m_RegisterOffsetList.push_back(rCurRegOff);
    else
      itRegOff->m_Offset = rCurRegOff.m_Offset;
  }

  pInsn->Comment() += (boost::format("[stk: %s:%x]") % m_pCpuInfo->ConvertIdentifierToName(rCurRegOff.m_Id) % rCurRegOff.m_Offset).str();
  return true;
}