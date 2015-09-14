#include "medusa/expression_visitor.hpp"
#include "medusa/bits.hpp"

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

Expression::SPType ExpressionVisitor::VisitConstant(IntegerExpression::SPType spConstExpr)
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

Expression::SPType ExpressionVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return nullptr;
}

Expression::SPType ExpressionVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
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
    spWhileExpr->GetReferenceExpression()->Visit(this),
    spWhileExpr->GetTestExpression()->Visit(this),
    spWhileExpr->GetBodyExpression()->Visit(this));
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

Expression::SPType CloneVisitor::VisitConstant(IntegerExpression::SPType spConstExpr)
{
  return Expr::MakeConstInt(spConstExpr->GetConstant());
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

Expression::SPType CloneVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return Expr::MakeTrack(
    spTrkExpr->GetTrackedExpression(),
    spTrkExpr->GetTrackAddress(),
    spTrkExpr->GetTrackPosition()
    );
}

Expression::SPType CloneVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  return Expr::MakeVar(
    spVarExpr->GetName(),
    spVarExpr->GetAction(),
    spVarExpr->GetBitSize());
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

Expression::SPType FilterVisitor::VisitConstant(IntegerExpression::SPType spConstExpr)
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

Expression::SPType FilterVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  _Evaluate(spTrkExpr);
  return nullptr;
}

Expression::SPType FilterVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  _Evaluate(spVarExpr);
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

  return Expr::MakeTrack(spIdExpr, TrackedAddress, 0);
}

Expression::SPType TrackVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
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

Expression::SPType BackTrackVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
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
  auto spExpr = expr_cast<IntegerExpression>(spUnOpExpr->GetExpression()->Visit(this));

  if (spExpr == nullptr)
    return nullptr;

  u32 Bit = spExpr->GetBitSize();
  auto Value = spExpr->GetConstant();
  IntType Result;

  switch (spUnOpExpr->GetOperation())
  {
  case OperationExpression::OpNot:
    Result = ~Value;
    break;

  case OperationExpression::OpNeg:
    Result = -Value;
    break;

  case OperationExpression::OpSwap:
    Result = Value.Swap();
    break;

  case OperationExpression::OpBsf:
    Result = Value.Bsf();
    break;

  case OperationExpression::OpBsr:
    Result = Value.Bsr();
    break;

  default:
    return nullptr;
  }

  return Expr::MakeConstInt(Result);
}

Expression::SPType EvaluateVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLExpr = expr_cast<IntegerExpression>(spBinOpExpr->GetLeftExpression()->Visit(this));
  auto spRExpr = expr_cast<IntegerExpression>(spBinOpExpr->GetRightExpression()->Visit(this));

  if (spLExpr == nullptr || spRExpr == nullptr)
    return nullptr;

  auto Left = spLExpr->GetConstant();
  auto Right = spRExpr->GetConstant();
  IntType Result;

  switch (spBinOpExpr->GetOperation())
  {
  case OperationExpression::OpAdd:
    Result = Left + Right;
    break;

  case OperationExpression::OpSub:
    Result = Left - Right;
    break;

  case OperationExpression::OpMul:
    Result = Left * Right;
    break;

  case OperationExpression::OpUDiv:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UDiv(Right);
    break;

  case OperationExpression::OpSDiv:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SDiv(Right);
    break;

  case OperationExpression::OpUMod:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UMod(Right);
    break;

  case OperationExpression::OpSMod:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SMod(Right);
    break;

  case OperationExpression::OpAnd:
    Result = Left & Right;
    break;

  case OperationExpression::OpOr:
    Result = Left | Right;
    break;

  case OperationExpression::OpXor:
    Result = Left ^ Right;
    break;

  case OperationExpression::OpLls:
    Result = Left.Lls(Right);
    break;

  case OperationExpression::OpLrs:
    Result = Left.Lrs(Right);
    break;

  case OperationExpression::OpArs:
    Result = Left.Ars(Right);
    break;

  case OperationExpression::OpSext:
    Result = Left;
    Result.SignExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpZext:
    Result = Left;
    Result.ZeroExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpXchg:
    return nullptr;

  case OperationExpression::OpInsertBits:
    Result = ((Left << Right.Lsb()) & Right);
    break;

  case OperationExpression::OpExtractBits:
    Result = (Left & Right) >> Right.Lsb();
    break;

  case OperationExpression::OpBcast:
    Result = Left;
    Result.BitCast(Right.ConvertTo<u16>());
    break;

  default:
    return nullptr;
  }

  return Expr::MakeConstInt(Result);
}

Expression::SPType EvaluateVisitor::VisitConstant(IntegerExpression::SPType spConstExpr)
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
    return Expr::MakeConstInt(m_rCurAddr.GetBaseSize(), m_rCurAddr.GetBase());
  }
  if (PcOffId != 0 && PcOffId == spIdExpr->GetId())
  {
    m_IsRelative = true;
    return Expr::MakeConstInt(m_rCurAddr.GetOffsetSize(), m_rCurAddr.GetOffset());
  }

  auto const itIdPair = m_Ids.find(spIdExpr->GetId());
  if (itIdPair != std::end(m_Ids))
  {
    return itIdPair->second->Visit(this);
  }

  m_IsSymbolic = true;
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

Expression::SPType EvaluateVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  Log::Write("core").Level(LogDebug) << "variable are not yet implemented in evaluate visitor" << LogEnd;
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
    spBaseConst = expr_cast<IntegerExpression>(spMemExpr->GetBaseExpression()->Visit(this));
    if (spBaseConst == nullptr)
    {
      m_IsSymbolic = true;
      return nullptr;
    }
  }

  auto spOffConst = expr_cast<IntegerExpression>(spMemExpr->GetOffsetExpression()->Visit(this));
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

  Address CurAddr(Base, spOffConst->GetConstant().ConvertTo<u64>());

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

  return Expr::MakeConstInt(spMemExpr->GetAccessSizeInBit(), Value);
}

Expression::SPType EvaluateVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

void EvaluateVisitor::SetId(u32 Id, Expression::SPType spExpr)
{
  m_Ids[Id] = spExpr;
}

SymbolicVisitor::SymbolicVisitor(Document const& rDoc, u8 Mode, bool EvalMemRef)
  : m_rDoc(rDoc), m_Mode(Mode)
  , m_IsSymbolic(false), m_IsRelative(false), m_IsMemoryReference(false), m_Update(false)
  , m_EvalMemRef(EvalMemRef)
{

}

Expression::SPType SymbolicVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  for (auto const& rspExpr : spBindExpr->GetBoundExpressions())
    rspExpr->Visit(this);
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  bool OldUpdateState = m_Update;
  m_Update = true;
  auto spRefExpr = spTernExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spTernExpr->GetTestExpression()->Visit(this);
  m_Update = OldUpdateState;

  auto spConstRefExpr = expr_cast<IntegerExpression>(spRefExpr);
  auto spConstTestExpr = expr_cast<IntegerExpression>(spTestExpr);

  if (spConstRefExpr == nullptr || spConstTestExpr == nullptr)
  {
    return Expr::MakeTernaryCond(spTernExpr->GetType(),
      spConstRefExpr != nullptr ? spConstRefExpr : spRefExpr, spConstTestExpr != nullptr ? spConstTestExpr : spTestExpr,
      spTernExpr->GetTrueExpression()->Visit(this), spTernExpr->GetFalseExpression()->Visit(this));
  }

  bool Res;
  if (!_EvaluateCondition(spTernExpr->GetType(), spConstRefExpr, spConstTestExpr, Res))
    return nullptr;

  return Res ? spTernExpr->GetTrueExpression()->Visit(this) : spTernExpr->GetFalseExpression()->Visit(this);
}

Expression::SPType SymbolicVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  bool OldUpdateState = m_Update;
  m_Update = true;
  auto spRefExpr = spIfElseExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spIfElseExpr->GetTestExpression()->Visit(this);
  m_Update = OldUpdateState;

  auto spConstRefExpr = expr_cast<IntegerExpression>(spRefExpr);
  auto spConstTestExpr = expr_cast<IntegerExpression>(spTestExpr);

  if (spConstRefExpr == nullptr || spConstTestExpr == nullptr)
  {
    OldUpdateState = m_Update;
    m_Update = false;
    auto spOldCond = m_spCond;
    m_spCond = std::dynamic_pointer_cast<IfElseConditionExpression>(Expr::MakeIfElseCond(
      spIfElseExpr->GetType(), spRefExpr, spTestExpr,
      spIfElseExpr->GetThenExpression(), spIfElseExpr->GetElseExpression()));
    auto spVstThenExpr = spIfElseExpr->GetThenExpression()->Visit(this);
    auto spVstElseExpr = spIfElseExpr->GetElseExpression() != nullptr ? spIfElseExpr->GetThenExpression()->Visit(this) : nullptr;
    m_spCond = spOldCond;
    m_Update = OldUpdateState;
    return Expr::MakeIfElseCond(spIfElseExpr->GetType(),
      spConstRefExpr != nullptr ? spConstRefExpr : spRefExpr, spConstTestExpr != nullptr ? spConstTestExpr : spTestExpr,
      spVstThenExpr, spVstElseExpr);
  }

  bool Res;
  if (!_EvaluateCondition(spIfElseExpr->GetType(), spConstRefExpr, spConstTestExpr, Res))
    return nullptr;

  return Res ? spIfElseExpr->GetThenExpression()->Visit(this) : spIfElseExpr->GetElseExpression()->Visit(this);
}

Expression::SPType SymbolicVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  m_IsSymbolic = false;
  m_Update = true;
  auto spSrcExpr = spAssignExpr->GetSourceExpression()->Visit(this);
  m_Update = false;
  ++m_CurPos;

  auto spDstExpr = spAssignExpr->GetDestinationExpression();
  auto spDstExprVst = spDstExpr->Visit(this);

  if (spDstExpr == nullptr || spDstExprVst == nullptr)
    return nullptr;

  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spCurExpr = GetExpression(rSymPair.first);

    if ((spCurExpr->Compare(spDstExpr) == Expression::CmpIdentical) || (spCurExpr->Compare(spDstExprVst) == Expression::CmpIdentical))
    {
      m_SymCtxt.erase(rSymPair.first);
      break;
    }
  }

  // If we have a conditional expression, we need to transform the source to a ternary expression
  if (m_spCond != nullptr)
  {
    auto spTernExpr = Expr::MakeTernaryCond(
      m_spCond->GetType(), m_spCond->GetReferenceExpression(), m_spCond->GetTestExpression(),
      spSrcExpr, spDstExpr);
    spSrcExpr = spTernExpr;
  }

  m_SymCtxt[spDstExprVst] = spSrcExpr;
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExprVst = spUnOpExpr->GetExpression()->Visit(this);

  if (spExprVst == nullptr)
    return nullptr;

  auto spExpr = expr_cast<IntegerExpression>(spExprVst);

  if (spExpr == nullptr)
    return Expr::MakeUnOp(static_cast<OperationExpression::Type>(spUnOpExpr->GetOperation()), spExprVst);

  u32 Bit = spExpr->GetBitSize();
  auto Value = spExpr->GetConstant();
  IntType Result;

  switch (spUnOpExpr->GetOperation())
  {
  case OperationExpression::OpNot:
    Result = ~Value;
    break;

  case OperationExpression::OpNeg:
    Result = -Value;
    break;

  case OperationExpression::OpSwap:
    Result = Value.Swap();
    break;

  case OperationExpression::OpBsf:
    Result = Value.Bsf();
    break;

  case OperationExpression::OpBsr:
    Result = Value.Bsr();
    break;

  default:
    return nullptr;
  }

  return Expr::MakeConstInt(Result);
}

Expression::SPType SymbolicVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLExprVst = spBinOpExpr->GetLeftExpression()->Visit(this);
  auto spRExprVst = spBinOpExpr->GetRightExpression()->Visit(this);

  if (spLExprVst == nullptr || spRExprVst == nullptr)
    return nullptr;

  auto spLExpr = expr_cast<IntegerExpression>(spLExprVst);
  auto spRExpr = expr_cast<IntegerExpression>(spRExprVst);

  if (spLExpr == nullptr || spRExpr == nullptr)
    return Expr::MakeBinOp(static_cast<OperationExpression::Type>(spBinOpExpr->GetOperation()),
    /**/spLExprVst, spRExprVst);

  auto Left = spLExpr->GetConstant();
  auto Right = spRExpr->GetConstant();
  IntType Result;

  switch (spBinOpExpr->GetOperation())
  {
  case OperationExpression::OpAdd:
    Result = Left + Right;
    break;

  case OperationExpression::OpSub:
    Result = Left - Right;
    break;

  case OperationExpression::OpMul:
    Result = Left * Right;
    break;

  case OperationExpression::OpUDiv:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UDiv(Right);
    break;

  case OperationExpression::OpSDiv:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SDiv(Right);
    break;

  case OperationExpression::OpUMod:
    if (Right.GetUnsignedValue() == 0)
      return nullptr;
    Result = Left.UMod(Right);
    break;

  case OperationExpression::OpSMod:
    if (Right.GetSignedValue() == 0)
      return nullptr;
    Result = Left.SMod(Right);
    break;

  case OperationExpression::OpAnd:
    Result = Left & Right;
    break;

  case OperationExpression::OpOr:
    Result = Left | Right;
    break;

  case OperationExpression::OpXor:
    Result = Left ^ Right;
    break;

  case OperationExpression::OpLls:
    Result = Left.Lls(Right);
    break;

  case OperationExpression::OpLrs:
    Result = Left.Lrs(Right);
    break;

  case OperationExpression::OpArs:
    Result = Left.Ars(Right);
    break;

  case OperationExpression::OpSext:
    Result = Left;
    Result.SignExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpZext:
    Result = Left;
    Result.ZeroExtend(Right.ConvertTo<u16>());
    break;

  case OperationExpression::OpXchg:
    return nullptr;

  case OperationExpression::OpInsertBits:
    Result = ((Left << Right.Lsb()) & Right);
    break;

  case OperationExpression::OpExtractBits:
    Result = (Left & Right) >> Right.Lsb();
    break;

  case OperationExpression::OpBcast:
    Result = Left;
    Result.BitCast(Right.ConvertTo<u16>());
    break;

  default:
    return nullptr;
  }

  return Expr::MakeConstInt(Result);
}

Expression::SPType SymbolicVisitor::VisitConstant(IntegerExpression::SPType spConstExpr)
{
  return spConstExpr;
}

Expression::SPType SymbolicVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spCurExpr = GetExpression(rSymPair.first);

    if (auto spSymIdExpr = expr_cast<IdentifierExpression>(spCurExpr))
    {
      if (spSymIdExpr->GetId() == spIdExpr->GetId())
        if (m_Update)
          return rSymPair.second;
        else
          return rSymPair.first;
    }
  }

  if (!m_Update)
    return spIdExpr;

  m_IsSymbolic = true;
  auto spTrkId = Expr::MakeTrack(spIdExpr, m_CurAddr, m_CurPos);
  auto spSymId = Expr::MakeSym(SymbolicExpression::Undefined, "sym_vst", m_CurAddr, spIdExpr);
  m_SymCtxt[spTrkId] = spSymId;
  return spSymId;
}

Expression::SPType SymbolicVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
}

Expression::SPType SymbolicVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  auto itVar = m_VarPool.find(spVarExpr->GetName());
  if (itVar == std::end(m_VarPool))
  {
    if (spVarExpr->GetAction() != VariableExpression::Alloc)
    {
      Log::Write("core").Level(LogError) << "invalid var expr action while evaluation" << LogEnd;
      return nullptr;
    }
    m_VarPool.insert(spVarExpr->GetName());
  }
  else
  {
    switch (spVarExpr->GetAction())
    {
    default:
      break;

    case VariableExpression::Alloc:
      Log::Write("core").Level(LogError) << "try to allocate an already allocated var while evaluation" << LogEnd;
      break;

    case VariableExpression::Use:
    {
      if (!m_Update)
        return spVarExpr;

      for (auto const& rSymPair : m_SymCtxt)
      {
        auto spCurExpr = GetExpression(rSymPair.first);
        if (auto spSymVarExpr = expr_cast<VariableExpression>(spCurExpr))
        {
          if (spSymVarExpr->GetName() == spVarExpr->GetName())
            return rSymPair.second;
        }
      }
    }

    case VariableExpression::Free:
      m_VarPool.erase(spVarExpr->GetName());
      for (auto const& rSymPair : m_SymCtxt)
      {
        auto spCurExpr = GetExpression(rSymPair.first);
        if (auto spSymVarExpr = expr_cast<VariableExpression>(spCurExpr))
        {
          if (spSymVarExpr->GetName() == spVarExpr->GetName())
          {
            m_SymCtxt.erase(rSymPair.first);
            break;
          }
        }
      }
      break;
    }
  }
  return nullptr;
}

Expression::SPType SymbolicVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  Expression::SPType spBaseExpr;
  Expression::SPType spOffExpr;

  m_IsMemoryReference = true;

  bool OldUpdateState = m_Update;
  m_Update = true;
  if (spMemExpr->GetBaseExpression() != nullptr)
  {
    spBaseExpr = spMemExpr->GetBaseExpression()->Visit(this);
  }

  spOffExpr = spMemExpr->GetOffsetExpression()->Visit(this);
  m_Update = OldUpdateState;

  // Sometimes, we don't want to evaluate memory reference or we can't
  auto spOffConstExpr = expr_cast<IntegerExpression>(spOffExpr);
  if (!m_EvalMemRef || spOffConstExpr == nullptr)
  {
    auto spMemExprVst = Expr::MakeMem(spMemExpr->GetAccessSizeInBit(), spBaseExpr, spOffExpr, spMemExpr->IsDereferencable());
    if (!m_Update)
      return spMemExprVst;

    auto spFoundExpr = FindExpression(spMemExprVst);
    if (spFoundExpr == nullptr)
      return spMemExprVst;
    return spFoundExpr;
  }

  auto Offset = spOffConstExpr->GetConstant().ConvertTo<TOffset>();

  TBase Base = 0;
  if (auto spBaseConstExpr = expr_cast<IntegerExpression>(spBaseExpr))
    Base = spBaseConstExpr->GetConstant().ConvertTo<TBase>();

  Address CurAddr(Base, Offset);

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

  // LATER(wisk): rely on IntType to support any read size

  default:
    return nullptr;
  }

  return Expr::MakeConstInt(spMemExpr->GetAccessSizeInBit(), Value);
}

Expression::SPType SymbolicVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  m_IsSymbolic = true;
  return nullptr;
}

Expression::VSPType SymbolicVisitor::GetExpressions(void) const
{
  Expression::VSPType Res;
  for (auto const& rSymPair : m_SymCtxt)
    Res.push_back(rSymPair.second);
  return Res;
}

std::string SymbolicVisitor::ToString(void) const
{
  static const std::string s_Sep(80, '_');
  std::ostringstream oss;
  for (auto const& rSymPair : m_SymCtxt)
  {
    oss << rSymPair.first->ToString() << " = ";
    if (rSymPair.second == nullptr)
      oss << "<null>";
    else
      oss << rSymPair.second->ToString();
    oss << std::endl << s_Sep << std::endl;
  }
  return oss.str();
}

void SymbolicVisitor::BindExpression(Expression::SPType spKeyExpr, Expression::SPType spValueExpr)
{
  m_SymCtxt[spKeyExpr] = spValueExpr;
}

bool SymbolicVisitor::UpdateAddress(Architecture& rArch, Address const& rAddr)
{
  m_CurAddr = rAddr;
  m_CurPos = 0;

  auto spInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(rAddr));
  if (spInsn == nullptr)
  {
    return false;
  }
  auto ExecAddr = rArch.CurrentAddress(rAddr, *spInsn);
  Expression::VSPType SetCurAddrExprs;
  if (!rArch.EmitSetExecutionAddress(SetCurAddrExprs, ExecAddr, m_Mode))
  {
    return false;
  }

  for (auto const& rspExpr : SetCurAddrExprs)
    rspExpr->Visit(this);

  return true;
}

bool SymbolicVisitor::UpdateExpression(Expression::SPType spKeyExpr, SymbolicVisitor::Updater updt)
{
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spCurExpr = GetExpression(rSymPair.first);
    if (spKeyExpr->Compare(spCurExpr) == Expression::CmpIdentical)
    {
      auto spClonedExpr = rSymPair.second->Clone();
      if (!updt(spClonedExpr))
        return false;
      m_SymCtxt[rSymPair.first] = spClonedExpr;
      return true;
    }
  }
  return false;
}

Expression::SPType SymbolicVisitor::GetExpression(Expression::SPType spExpr)
{
  if (auto spTrkExpr = expr_cast<TrackExpression>(spExpr))
    return spTrkExpr->GetTrackedExpression();
  if (auto spSymExpr = expr_cast<SymbolicExpression>(spExpr))
    return spSymExpr->GetExpression();
  return spExpr;
}

Expression::SPType SymbolicVisitor::FindExpression(Expression::SPType spExpr)
{
  for (auto const& rSymPair : m_SymCtxt)
  {
    auto spCurExpr = GetExpression(rSymPair.first);
    if (spExpr->Compare(spCurExpr) == Expression::CmpIdentical)
      return rSymPair.second;
  }
  return nullptr;
}

bool SymbolicVisitor::_EvaluateCondition(u8 CondOp, IntegerExpression::SPType spConstRefExpr, IntegerExpression::SPType spConstTestExpr, bool& rRes) const
{
  auto RefVal = spConstRefExpr->GetConstant();
  auto TestVal = spConstTestExpr->GetConstant();

  switch (CondOp)
  {
  case ConditionExpression::CondEq:
    rRes = RefVal.GetUnsignedValue() == TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondNe:
    rRes = RefVal.GetUnsignedValue() != TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUgt:
    rRes = RefVal.GetUnsignedValue() > TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUge:
    rRes = RefVal.GetUnsignedValue() >= TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUlt:
    rRes = RefVal.GetUnsignedValue() < TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondUle:
    rRes = RefVal.GetUnsignedValue() <= TestVal.GetUnsignedValue();
    break;

  case ConditionExpression::CondSgt:
    rRes = RefVal.GetSignedValue() > TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSge:
    rRes = RefVal.GetSignedValue() >= TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSlt:
    rRes = RefVal.GetSignedValue() < TestVal.GetSignedValue();
    break;

  case ConditionExpression::CondSle:
    rRes = RefVal.GetSignedValue() <= TestVal.GetSignedValue();
    break;

  default:
    Log::Write("core") << "unknown comparison" << LogEnd;
    return false;
  }

  return true;
}


NormalizeIdentifier::NormalizeIdentifier(CpuInformation const& rCpuInfo, u8 Mode)
: m_rCpuInfo(rCpuInfo), m_Mode(Mode)
{
}

Expression::SPType NormalizeIdentifier::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  auto spNrmSrc = spAssignExpr->GetSourceExpression()->Visit(this);

  if (auto spDstId = expr_cast<IdentifierExpression>(spAssignExpr->GetDestinationExpression()))
  {
    u32 Id = spDstId->GetId();
    u32 NrmId = 0;
    u64 Mask = 0x0;

    if (m_rCpuInfo.NormalizeRegister(Id, m_Mode, NrmId, Mask))
    {
      auto const* pCpuInfo = spDstId->GetCpuInformation();
      u32 NrmIdBitSize = pCpuInfo->GetSizeOfRegisterInBit(NrmId);

      // ExtDstId = (ExtDstId & ~Mask) | InsertBits(bit_cast(ExtSrc, NrmIdBitSize))
      auto spExtDstId = Expr::MakeId(NrmId, pCpuInfo);
      auto spMask = Expr::MakeConstInt(NrmIdBitSize, Mask);
      auto spNotMask = Expr::MakeConstInt(NrmIdBitSize, ~Mask);
      auto spBitCastSrc = Expr::MakeBinOp(OperationExpression::OpBcast, spNrmSrc, Expr::MakeConstInt(NrmIdBitSize, NrmIdBitSize));
      auto spInsertBits = Expr::MakeBinOp(OperationExpression::OpInsertBits, spBitCastSrc, spMask);
      auto spClearBits = Expr::MakeBinOp(OperationExpression::OpAnd, spExtDstId, spNotMask);
      auto spExtSrc = Expr::MakeBinOp(OperationExpression::OpOr, spClearBits, spInsertBits);

      return Expr::MakeAssign(spExtDstId, spExtSrc);
    }
  }

  return Expr::MakeAssign(spAssignExpr->GetDestinationExpression(), spNrmSrc);
}

Expression::SPType NormalizeIdentifier::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  u32 Id = spIdExpr->GetId();
  u32 NrmId = 0;
  u64 Mask = 0x0;

  if (!m_rCpuInfo.NormalizeRegister(Id, m_Mode, NrmId, Mask))
    return spIdExpr;
  u32 IdBitSize = m_rCpuInfo.GetSizeOfRegisterInBit(Id);
  u32 NrmIdBitSize = m_rCpuInfo.GetSizeOfRegisterInBit(NrmId);

  auto spExtractBits = Expr::MakeBinOp(
    OperationExpression::OpExtractBits,
    Expr::MakeId(NrmId, spIdExpr->GetCpuInformation()),
    Expr::MakeConstInt(NrmIdBitSize, Mask));
  return Expr::MakeBinOp(OperationExpression::OpBcast,
    spExtractBits, Expr::MakeConstInt(IdBitSize, IdBitSize));
}

Expression::SPType IdentifierToVariable::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  auto pCpuInfo = spIdExpr->GetCpuInformation();
  auto Id = spIdExpr->GetId();
  m_UsedId.insert(Id);
  return Expr::MakeVar(pCpuInfo->ConvertIdentifierToName(Id), VariableExpression::Use);
}

Expression::SPType SimplifyVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  return nullptr;
}

Expression::SPType SimplifyVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  return nullptr;
}

MEDUSA_NAMESPACE_END
