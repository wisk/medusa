#include "medusa/expression_visitor.hpp"

MEDUSA_NAMESPACE_BEGIN

Expression::SPType ExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  auto& rExprs = spBindExpr->GetBoundExpressions();
  for (auto pExpr : rExprs)
    pExpr->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  spTernExpr->GetReferenceExpression()->Visit(this);
  spTernExpr->GetTestExpression()->Visit(this);
  spTernExpr->GetTrueExpression()->Visit(this);
  spTernExpr->GetFalseExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  spIfElseExpr->GetReferenceExpression()->Visit(this);
  spIfElseExpr->GetTestExpression()->Visit(this);
  spIfElseExpr->GetThenExpression()->Visit(this);
  if (spIfElseExpr->GetElseExpression() != nullptr)
    spIfElseExpr->GetElseExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  spWhileExpr->GetReferenceExpression()->Visit(this);
  spWhileExpr->GetTestExpression()->Visit(this);
  spWhileExpr->GetBodyExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  spAssignExpr->GetDestinationExpression()->Visit(this);
  spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  spUnOpExpr->GetExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  spBinOpExpr->GetLeftExpression()->Visit(this);
  spBinOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitConstant(ConstantExpression::SPType spConstExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  if (spMemExpr->GetBaseExpression() != nullptr)
    spMemExpr->GetBaseExpression()->Visit(this);
  spMemExpr->GetOffsetExpression()->Visit(this);
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  return nullptr;
}

Expression::SPType CloneVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return spSysExpr->Clone();
}

Expression::SPType CloneVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  Expression::LSPType Exprs;

  for (auto spExpr : spBindExpr->GetBoundExpressions())
    Exprs.push_back(spExpr->Visit(this));

  return Expr::MakeBind(Exprs);
}

Expression::SPType CloneVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  return Expr::MakeTernaryCond(
    spTernExpr->GetType(),
    spTernExpr->GetReferenceExpression()->Visit(this),
    spTernExpr->GetTestExpression()->Visit(this),
    spTernExpr->GetTrueExpression()->Visit(this),
    spTernExpr->GetFalseExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  auto spElseExpr = spIfElseExpr->GetElseExpression() != nullptr ? spIfElseExpr->GetElseExpression()->Visit(this) : nullptr;
  return Expr::MakeIfElseCond(
    spIfElseExpr->GetType(),
    spIfElseExpr->GetReferenceExpression()->Visit(this),
    spIfElseExpr->GetTestExpression()->Visit(this),
    spIfElseExpr->GetThenExpression()->Visit(this),
    spElseExpr);
}

Expression::SPType CloneVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return Expr::MakeWhileCond(
    spWhileExpr->GetType(),
    spWhileExpr->GetReferenceExpression(),
    spWhileExpr->GetTestExpression(),
    spWhileExpr->GetBodyExpression());
}

Expression::SPType CloneVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  return Expr::MakeAssign(
    spAssignExpr->GetDestinationExpression()->Visit(this),
    spAssignExpr->GetSourceExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  return Expr::MakeUnOp(
    static_cast<OperationExpression::Type>(spUnOpExpr->GetOperation()),
    spUnOpExpr->GetExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  return Expr::MakeBinOp(
    static_cast<OperationExpression::Type>(spBinOpExpr->GetOperation()),
    spBinOpExpr->GetLeftExpression()->Visit(this),
    spBinOpExpr->GetRightExpression()->Visit(this));
}

Expression::SPType CloneVisitor::VisitConstant(ConstantExpression::SPType spConstExpr)
{
  return Expr::MakeConst(
    spConstExpr->GetType(),
    spConstExpr->GetConstant());
}

Expression::SPType CloneVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  return Expr::MakeId(
    spIdExpr->GetId(),
    spIdExpr->GetCpuInformation());
}

Expression::SPType CloneVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return Expr::MakeVecId(
    spVecIdExpr->GetVector(),
    spVecIdExpr->GetCpuInformation());
}

Expression::SPType CloneVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
{
  return std::make_shared<TrackedIdentifierExpression>(
    spTrkIdExpr->GetId(),
    spTrkIdExpr->GetCpuInformation(),
    spTrkIdExpr->GetCurrentAddress());
}

Expression::SPType CloneVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  auto spBaseExpr = spMemExpr->GetBaseExpression() ? spMemExpr->GetBaseExpression()->Visit(this) : nullptr;

  return Expr::MakeMem(
    spMemExpr->GetAccessSizeInBit(),
    spBaseExpr,
    spMemExpr->GetOffsetExpression()->Visit(this),
    spMemExpr->IsDereferencable());
}

Expression::SPType CloneVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  return Expr::MakeSym(
    spSymExpr->GetType(),
    spSymExpr->GetValue(),
    spSymExpr->GetAddress());
}

Expression::SPType FilterVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  _Evaluate(spSysExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitBind(BindExpression::SPType spBindExpr)
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

Expression::SPType FilterVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
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

Expression::SPType FilterVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
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

Expression::SPType FilterVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
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

Expression::SPType FilterVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
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

Expression::SPType FilterVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  _Evaluate(spUnOpExpr);
  if (_IsDone())
    return nullptr;

  spUnOpExpr->GetExpression()->Visit(this);
  if (_IsDone())
    return nullptr;

  return nullptr;
}

Expression::SPType FilterVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  _Evaluate(spBinOpExpr);

  if (_IsDone())
    return nullptr;
  spBinOpExpr->GetLeftExpression()->Visit(this);

  if (_IsDone())
    return nullptr;
  spBinOpExpr->GetRightExpression()->Visit(this);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitConstant(ConstantExpression::SPType spConstExpr)
{
  _Evaluate(spConstExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  _Evaluate(spIdExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  _Evaluate(spVecIdExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
{
  _Evaluate(spTrkIdExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
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

Expression::SPType FilterVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  if (m_ExprMatcher(spSymExpr))
    m_MatchedExprs.push_back(spSymExpr);
  return nullptr;
}

void FilterVisitor::_Evaluate(Expression::SPType spExpr)
{
  _AddExpression(m_ExprMatcher(spExpr));
}

void FilterVisitor::_AddExpression(Expression::SPType spExpr)
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

Expression::SPType TrackVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  // NOTE: We must to visit the right side first, since the left side could
  // modify the Track context
  auto pTrkSrcExpr = spAssignExpr->GetSourceExpression()->Visit(this);

  m_IsAssigned = true;
  auto spTrkDstExpr = spAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  return Expr::MakeAssign(spTrkDstExpr, pTrkSrcExpr);
}

Expression::SPType TrackVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
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

Expression::SPType TrackVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
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

Expression::SPType BackTrackVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  m_IsAssigned = true;
  spAssignExpr->GetDestinationExpression()->Visit(this);
  m_IsAssigned = false;

  // TODO: can we have 2 OpAff in the same expression?
  spAssignExpr->GetSourceExpression()->Visit(this);
  m_TrackSource = false;

  return nullptr;
}

Expression::SPType BackTrackVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
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

EvaluateVisitor::EvaluateVisitor(Document const& rDoc, Address const& rCurAddr, u8 Mode, bool EvalMemRef)
: m_rDoc(rDoc), m_rCurAddr(rCurAddr), m_Mode(Mode)
, m_IsSymbolic(false), m_IsRelative(false), m_IsMemoryReference(false)
, m_EvalMemRef(EvalMemRef), m_spResExpr(nullptr)
{
}

Expression::SPType EvaluateVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  m_spResExpr = spAssignExpr->GetSourceExpression()->Visit(this);
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExpr = expr_cast<ConstantExpression>(spUnOpExpr->GetExpression()->Visit(this));

  if (spExpr == nullptr)
    return nullptr;

  u32 Bit = spExpr->GetSizeInBit();
  u64 Result = 0;

  switch (spUnOpExpr->GetOperation())
  {
  default:
    return nullptr;

  case OperationExpression::OpNot:
    Result = ~spExpr->GetConstant();
    break;

  case OperationExpression::OpNeg:
    Result = (~spExpr->GetConstant() + 1);
    break;

  case OperationExpression::OpSwap:
    switch (Bit)
    {
    case 8: return nullptr;
    case 16: { u16 Tmp = spExpr->GetConstant(); EndianSwap(Tmp); Result = Tmp; break; }
    case 32: { u32 Tmp = spExpr->GetConstant(); EndianSwap(Tmp); Result = Tmp; break; }
    case 64: { u64 Result = spExpr->GetConstant(); EndianSwap(Result); break; }
    }
  }

  return Expr::MakeConst(Bit, Result);
}

Expression::SPType EvaluateVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLExpr = expr_cast<ConstantExpression>(spBinOpExpr->GetLeftExpression()->Visit(this));
  auto spRExpr = expr_cast<ConstantExpression>(spBinOpExpr->GetRightExpression()->Visit(this));

  if (spLExpr == nullptr || spRExpr == nullptr)
    return nullptr;

  u32 Bit = std::max(spLExpr->GetSizeInBit(), spRExpr->GetSizeInBit());
  u64 Result = 0;

  switch (spBinOpExpr->GetOperation())
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

Expression::SPType EvaluateVisitor::VisitConstant(ConstantExpression::SPType spConstExpr)
{
  return spConstExpr;
}

Expression::SPType EvaluateVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  auto const pCpuInfo = spIdExpr->GetCpuInformation();

  auto const PcBaseId = pCpuInfo->GetRegisterByType(CpuInformation::ProgramBaseRegister, m_Mode);
  auto const PcOffId  = pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_Mode);

  if (PcBaseId != 0 && PcBaseId == spIdExpr->GetId())
  {
    m_IsRelative = true;
    return Expr::MakeConst(m_rCurAddr.GetBaseSize(), m_rCurAddr.GetBase());
  }
  if (PcOffId != 0 && PcOffId == spIdExpr->GetId())
  {
    m_IsRelative = true;
    return Expr::MakeConst(m_rCurAddr.GetOffsetSize(), m_rCurAddr.GetOffset());
  }

  m_IsSymbolic = true;
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
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

Expression::SPType EvaluateVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  u16 Base = 0;
  Expression::SPType spBaseConst;

  m_IsMemoryReference = true;

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

  // Sometimes, we don't want to evaluate memory reference
  if (!m_EvalMemRef)
  {
    return Expr::MakeMem(spMemExpr->GetAccessSizeInBit(), spBaseConst, spOffConst, spMemExpr->IsDereferencable());
  }

  Address CurAddr(Base, spOffConst->GetConstant());

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

Expression::SPType EvaluateVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

MEDUSA_NAMESPACE_END
