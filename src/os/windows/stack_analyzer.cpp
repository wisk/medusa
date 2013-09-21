#include "stack_analyzer.hpp"

Expression* ExpressionVisitor_FindOperations::VisitBind(Expression::List const& rExprList)
{
  for (auto itSem = std::begin(rExprList); itSem != std::end(rExprList); ++itSem)
    (*itSem)->Visit(this);
  return nullptr;
}

/*! in this test, we try to match these cases: id₀ = addr[ id₁ ± off ] or id₀ = id₁ or id₀ ±= off */
Expression* ExpressionVisitor_FindOperations::VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
{
  if (Type != OperationExpression::OpAff)
    return nullptr;

  auto pDstExprId  = dynamic_cast<IdentifierExpression const*>(pLeftExpr);
  auto pDstExprVar = dynamic_cast<VariableExpression   const*>(pLeftExpr);

  /*! id₀ = addr[ id₁ ± off ] */
  std::unique_ptr<OperationExpression const> upSrcExprOprt(dynamic_cast<OperationExpression const*>(pRightExpr->Visit(this)));
  if (upSrcExprOprt != nullptr)
  {
    // we make sure that the current operation is either add or sub
    if (upSrcExprOprt->GetOperation() != OperationExpression::OpAdd && upSrcExprOprt->GetOperation() != OperationExpression::OpSub)
      return nullptr;

    auto pSrcId  = dynamic_cast<IdentifierExpression const*>(upSrcExprOprt->GetLeftExpression() );
    auto pSrcOff = dynamic_cast<ConstantExpression   const*>(upSrcExprOprt->GetRightExpression());

    if (pSrcId == nullptr || pSrcOff == nullptr)
      return nullptr;

    // if the source id is not tracked, we leave
    auto itRegOff = std::find(std::begin(m_rRegisterOffsetList), std::end(m_rRegisterOffsetList), pSrcId->GetId());
    if (itRegOff == std::end(m_rRegisterOffsetList))
    {
      if (pDstExprId != nullptr)
      {
        // If we modify a tracked register with a unknown source, we remove it
        auto itDstRegOff = std::find(std::begin(m_rRegisterOffsetList), std::end(m_rRegisterOffsetList), pDstExprId->GetId());
        if (itDstRegOff != std::end(m_rRegisterOffsetList))
          itDstRegOff->m_Tracked = false;
      }
      return nullptr;
    }

    s64 Off = 0;
    if (pSrcOff != nullptr)
      Off = static_cast<s64>(pSrcOff->GetConstant());
    if (upSrcExprOprt->GetOperation() == OperationExpression::OpSub)
      Off = -Off;

    // LATER: If the destination is not an identifier, we can't track it at this time
    if (pDstExprId == nullptr)
      return nullptr;

    m_CurrentRegisterOffset = RegisterOffset(pDstExprId->GetId(), Off + itRegOff->m_Offset);
    return nullptr;
  }


  /*! id₀ = addr[ id₁ ] */
  std::unique_ptr<IdentifierExpression const> upSrcExprId(dynamic_cast<IdentifierExpression const*>(pRightExpr->Visit(this)));

  /*! id₀ = id₁ */
  auto pSrcExprId = dynamic_cast<IdentifierExpression const*>(pRightExpr);
  if (pSrcExprId == nullptr)
    pSrcExprId = upSrcExprId.get();
  if (pSrcExprId != nullptr)
  {
    // if the source id is not tracked, we leave
    RegisterOffset RegOff;
    if (FindRegisterOffsetByIdentifier(RegOff, pSrcExprId->GetId()) == false)
      return nullptr;

    // LATER: If the destination is not an identifier, we can't track it at this time
    if (pDstExprId == nullptr)
      return nullptr;

    bool Tracked = true;

    // If we modify a tracked register with a unknown source, we remove it
    RegisterOffset RegOffSrc;
    if (FindRegisterOffsetByIdentifier(RegOffSrc, pSrcExprId->GetId()) == false)
      Tracked = false;

    m_CurrentRegisterOffset = RegisterOffset(pDstExprId->GetId(), RegOff.m_Offset, Tracked);
    return nullptr;
  }

  /*! id₀ = var */
  auto pSrcExprVar = dynamic_cast<VariableExpression const*>(pRightExpr);
  if (pDstExprId != nullptr && pSrcExprVar != nullptr)
  {
    m_CurrentRegisterOffset.m_Id = pDstExprId->GetId();
    return nullptr;
  }

  /*! id₀ ±= off */   /*! id₀ = id₁ ± id₂ */
  auto pSrcExprOprt = dynamic_cast<OperationExpression const*>(pRightExpr);
  if (pSrcExprOprt != nullptr &&
      (pSrcExprOprt->GetOperation() == OperationExpression::OpAdd
    || pSrcExprOprt->GetOperation() == OperationExpression::OpSub))
  {
    auto pSrcId1   = dynamic_cast<IdentifierExpression const*>(pSrcExprOprt->GetLeftExpression ());
    auto pSrcId2   = dynamic_cast<IdentifierExpression const*>(pSrcExprOprt->GetRightExpression());
    auto pSrcConst = dynamic_cast<ConstantExpression   const*>(pSrcExprOprt->GetRightExpression());

    s64 NewOffset = 0;

    bool Tracked = false;

    if (pSrcId1 != nullptr && pSrcConst != nullptr)
    {
      RegisterOffset RegOff;
      if (FindRegisterOffsetByIdentifier(RegOff, pSrcId1->GetId()) == false)
        return nullptr;

      auto ConstOff = static_cast<s64>(pSrcConst->GetConstant());

      NewOffset = pSrcExprOprt->GetOperation() == OperationExpression::OpAdd
        ? RegOff.m_Offset + ConstOff
        : RegOff.m_Offset - ConstOff;

      Tracked = RegOff.m_Tracked;
    }

    else if (pSrcId1 != nullptr && pSrcId2 != nullptr)
    {
      RegisterOffset RegOff1, RegOff2;
      if (FindRegisterOffsetByIdentifier(RegOff1, pSrcId1->GetId()) == false || FindRegisterOffsetByIdentifier(RegOff2, pSrcId2->GetId()) == false)
        return nullptr;

      if (RegOff1.m_Tracked == true || RegOff2.m_Tracked == true)
        Tracked = true;

      NewOffset = pSrcExprOprt->GetOperation() == OperationExpression::OpAdd
        ? RegOff1.m_Offset + RegOff2.m_Offset
        : RegOff1.m_Offset - RegOff2.m_Offset;
    }

    if (pDstExprId != nullptr)
      m_CurrentRegisterOffset = RegisterOffset(pDstExprId->GetId(), NewOffset, Tracked);
    else
    {
      m_CurrentRegisterOffset.m_Offset = NewOffset;
      m_CurrentRegisterOffset.m_Tracked = Tracked;
    }

    return nullptr;
  }

  /* id₀ = imm */
  auto pSrcExprConst = dynamic_cast<ConstantExpression const*>(pRightExpr);
  if (pDstExprId != nullptr && pSrcExprConst != nullptr)
  {
    auto Off = static_cast<s64>(pSrcExprConst->GetConstant());
    auto itRegOff = std::find(std::begin(m_rRegisterOffsetList), std::end(m_rRegisterOffsetList), pDstExprId->GetId());

    bool Tracked = itRegOff != std::end(m_rRegisterOffsetList);
    if (Tracked)
    {
      itRegOff->m_Tracked = false;
      itRegOff->m_Offset = Off;
    }
    else if (UpdateRegisterOffset(RegisterOffset(pDstExprId->GetId(), Off, false)) == false)
      m_rRegisterOffsetList.push_back(RegisterOffset(pDstExprId->GetId(), Off, false));

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

bool ExpressionVisitor_FindOperations::FindRegisterOffsetByIdentifier(RegisterOffset& rRegOff, u32 Id)
{
  for (auto itRegOff = std::begin(m_rRegisterOffsetList); itRegOff != std::end(m_rRegisterOffsetList); ++itRegOff)
  {
    if (m_pCpuInfo->IsRegisterAliased(Id, itRegOff->m_Id) == false)
      continue;

    // LATER: Modify offset accordingly...
    rRegOff = *itRegOff;
    return true;
  }

  return false;
}

bool ExpressionVisitor_FindOperations::UpdateRegisterOffset(RegisterOffset const& rRegOff)
{
  for (auto itRegOff = std::begin(m_rRegisterOffsetList); itRegOff != std::end(m_rRegisterOffsetList); ++itRegOff)
  {
    if (m_pCpuInfo->IsRegisterAliased(rRegOff.m_Id, itRegOff->m_Id) == false)
      continue;

    // LATER: Modify offset accordingly...
    *itRegOff = rRegOff;
    return true;
  }

  return false;
}

std::string ExpressionVisitor_FindOperations::ToString(void) const
{
  std::string Result = "";
  for (auto itRegOff = std::begin(m_rRegisterOffsetList); itRegOff != std::end(m_rRegisterOffsetList); ++itRegOff)
  {
    std::string Register = m_pCpuInfo->ConvertIdentifierToName(itRegOff->m_Id);
    std::string Offset   = (itRegOff->m_Offset < 0 ? (boost::format("-0x%x") % -itRegOff->m_Offset) : (boost::format("0x%x") % itRegOff->m_Offset)).str();
    std::string Tracked  = itRegOff->m_Tracked ? "true" : "false";

    Result += (boost::format(" [reg: %s, off: %s, trk: %s]") % Register % Offset % Tracked).str();
  }
  return Result;
}

X86StackAnalyzerTracker::X86StackAnalyzerTracker(CpuInformation const* pCpuInfo) : m_pCpuInfo(pCpuInfo)
{
  m_RegisterOffsetList.push_back(ExpressionVisitor_FindOperations::RegisterOffset(pCpuInfo->GetRegisterByType(CpuInformation::StackPointerRegister)));
}

bool X86StackAnalyzerTracker::Track(Analyzer& rAnlz, Document& rDoc, Address const& rAddr)
{
  auto spInsn = std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(rAddr));
  if (spInsn == nullptr)
    return false;
  if (spInsn->GetOperationType() == Instruction::OpRet)
  {
    // FIXME: We assume the first entry to be the stack register (esp/rsp)
    auto const& rCurRegOff = m_RegisterOffsetList.front();
    if (rCurRegOff.m_Offset != 0)
      spInsn->Comment() += " [stk anlz failed]";
    else
      spInsn->Comment() += " [stk anlz succeed]";
    return false;
  }

  ExpressionVisitor_FindOperations fo(m_RegisterOffsetList, m_pCpuInfo);
  auto Sem = spInsn->GetSemantic();
  for (auto itSem = std::begin(Sem); itSem != std::end(Sem); ++itSem)
  {
    (*itSem)->Visit(&fo);
  }
  auto& rCurRegOff = fo.GetCurrentRegisterOffset();
  if (rCurRegOff.m_Id != 0)
  {
    if (spInsn->GetOperationType() == Instruction::OpCall)
    {
      rCurRegOff.m_Offset += m_pCpuInfo->GetSizeOfRegisterInBit(rCurRegOff.m_Id) / 8;
    }

    if (fo.UpdateRegisterOffset(rCurRegOff) == false)
      m_RegisterOffsetList.push_back(rCurRegOff);
  }

  spInsn->Comment() += fo.ToString();
  return true;
}