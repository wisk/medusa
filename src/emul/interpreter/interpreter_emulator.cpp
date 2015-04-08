#include "interpreter_emulator.hpp"
#include <medusa/log.hpp>
#include <medusa/extend.hpp>
#include <medusa/bits.hpp>

MEDUSA_NAMESPACE_USE

InterpreterEmulator::InterpreterEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
: Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
{
}

InterpreterEmulator::~InterpreterEmulator(void)
{
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::SPType spExpr)
{
  if (auto spSys = expr_cast<SystemExpression>(spExpr))
  {
    if (spSys->GetName() == "dump_insn")
    {
      if (m_InsnCb)
        m_InsnCb(m_pCpuCtxt, m_pMemCtxt, spSys->GetAddress());
      return true;
    }

    if (spSys->GetName() == "check_exec_hook")
    {
      auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
      auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc);
      u64 CurPc = 0;
      m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz);
      TestHook(Address(CurPc), Emulator::HookOnExecute);
    }
  }

  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_Vars);
  auto spCurExpr = spExpr->Visit(&Visitor);

  if (spCurExpr == nullptr)
    return false;

  return true;
}

bool InterpreterEmulator::Execute(Address const& rAddress, Expression::LSPType const& rExprList)
{
  InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_Vars);

  for (Expression::SPType spExpr : rExprList)
  {
    if (auto spSys = expr_cast<SystemExpression>(spExpr))
    {
      if (spSys->GetName() == "dump_insn")
      {
        if (m_InsnCb)
          m_InsnCb(m_pCpuCtxt, m_pMemCtxt, spSys->GetAddress());
        continue;
      }

      if (spSys->GetName() == "check_exec_hook")
      {
        auto RegPc = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
        auto RegSz = m_pCpuInfo->GetSizeOfRegisterInBit(RegPc);
        u64 CurPc = 0;
        if (!m_pCpuCtxt->ReadRegister(RegPc, &CurPc, RegSz))
          return false;
        TestHook(Address(CurPc), Emulator::HookOnExecute);
        continue;
      }
    }

    auto spCurExpr = spExpr->Visit(&Visitor);
    if (spCurExpr == nullptr)
    {
      std::cout << m_pCpuCtxt->ToString() << std::endl;
      std::cout << spExpr->ToString() << std::endl;
      return false;
    }

//#ifdef _DEBUG
//    // DEBUG
//    std::cout << "cur: " << spExpr->ToString() << std::endl;
//    std::cout << "res: " << spCurExpr->ToString() << std::endl;
//    std::cout << m_pCpuCtxt->ToString() << std::endl;
//#endif
  }

  return true;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  return nullptr; // TODO
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  Expression::LSPType SmplExprList;
  for (Expression::SPType spExpr : spBindExpr->GetBoundExpressions())
    SmplExprList.push_back(spExpr->Visit(this));
  return Expr::MakeBind(SmplExprList);
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  bool Cond;

  if (!_EvaluateComparison(spTernExpr->GetType(), Cond))
    return nullptr;

  return (Cond ? spTernExpr->GetTrueExpression()->Clone() : spTernExpr->GetFalseExpression()->Clone());
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  bool Cond;

  if (!_EvaluateComparison(spIfElseExpr->GetType(), Cond))
    return nullptr;

  auto spExpr = (Cond ? spIfElseExpr->GetThenExpression()->Clone() : (spIfElseExpr->GetElseExpression() ? spIfElseExpr->GetElseExpression()->Clone() : Expr::MakeBoolean(false)));
  if (spExpr != nullptr)
    return spExpr->Visit(this);

  return spExpr;
}

// FIXME:
Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  return nullptr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  State OldState = m_State;

  m_State = Read;
  auto spSrc = spAssignExpr->GetSourceExpression()->Visit(this);
  m_State = Write;
  auto spDst = spAssignExpr->GetDestinationExpression()->Visit(this);
  m_State = OldState;

  if (spDst == nullptr || spSrc == nullptr)
    return nullptr;

  return spSrc;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExpr = spUnOpExpr->GetExpression()->Visit(this);

  if (spExpr == nullptr)
    return nullptr;

  if (m_Values.size() < 1)
    return nullptr;
  auto Val = m_Values.back();
  m_Values.pop_back();

  switch (spUnOpExpr->GetOperation())
  {
  case OperationExpression::OpNot:
    m_Values.push_back(Val.Not());
    break;

  case OperationExpression::OpNeg:
    m_Values.push_back(Val.Neg());
    break;

  case OperationExpression::OpSwap:
    m_Values.push_back(Val.Swap());
    break;

  case OperationExpression::OpBsf:
    m_Values.push_back(Val.Bsf());
    break;

  case OperationExpression::OpBsr:
    m_Values.push_back(Val.Bsr());
    break;

  default:
    return nullptr;
  }

  return spUnOpExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLeft = spBinOpExpr->GetLeftExpression()->Visit(this);
  auto spRight = spBinOpExpr->GetRightExpression()->Visit(this);

  if (spLeft == nullptr || spRight == nullptr)
    return nullptr;

  if (m_Values.size() < 2)
    return nullptr;

  auto RightVal = m_Values.back();
  m_Values.pop_back();
  auto LeftVal = m_Values.back();
  m_Values.pop_back();

  switch (spBinOpExpr->GetOperation())
  {
    case OperationExpression::OpXchg:
      break;

    case OperationExpression::OpAnd:
      m_Values.push_back(LeftVal.And(RightVal));
      break;

    case OperationExpression::OpOr:
      m_Values.push_back(LeftVal.Or(RightVal));
      break;

    case OperationExpression::OpXor:
      m_Values.push_back(LeftVal.Xor(RightVal));
      break;

    case OperationExpression::OpLls:
      m_Values.push_back(LeftVal.Lls(RightVal));
      break;

    case OperationExpression::OpLrs:
      m_Values.push_back(LeftVal.Lrs(RightVal));
      break;

    case OperationExpression::OpArs:
      m_Values.push_back(LeftVal.Ars(RightVal));
      break;

    case OperationExpression::OpAdd:
      m_Values.push_back(LeftVal.Add(RightVal));
      break;

    case OperationExpression::OpSub:
      m_Values.push_back(LeftVal.Sub(RightVal));
      break;

    case OperationExpression::OpMul:
      m_Values.push_back(LeftVal.Mul(RightVal));
      break;

    case OperationExpression::OpSDiv:
      m_Values.push_back(LeftVal.SDiv(RightVal));
      break;

    case OperationExpression::OpUDiv:
      m_Values.push_back(LeftVal.UDiv(RightVal));
      break;

    case OperationExpression::OpSMod:
      m_Values.push_back(LeftVal.SMod(RightVal));
      break;

    case OperationExpression::OpUMod:
      m_Values.push_back(LeftVal.UMod(RightVal));
      break;

    case OperationExpression::OpSext:
    {
      auto Result = LeftVal;
      Result.SignExtend(RightVal.ConvertTo<u16>());
      m_Values.push_back(Result);
      break;
    }

    case OperationExpression::OpZext:
    {
      auto Result = LeftVal;
      Result.ZeroExtend(RightVal.ConvertTo<u16>());
      m_Values.push_back(Result);
      break;
    }

    case OperationExpression::OpInsertBits:
      m_Values.push_back((LeftVal << RightVal.Lsb()) & RightVal);
      break;

    case OperationExpression::OpExtractBits:
      m_Values.push_back((LeftVal & RightVal) >> RightVal.Lsb());
      break;

    default:
      return nullptr;
  }

  return spBinOpExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitConstant(ConstantExpression::SPType spConstExpr)
{
  if (m_State != Read)
    return nullptr;
  m_Values.push_back(spConstExpr->GetConstant());
  return spConstExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  switch (m_State)
  {
  case Read:
  {
    IntType RegVal(spIdExpr->GetBitSize(), 0);
    if (!m_pCpuCtxt->ReadRegister(spIdExpr->GetId(), RegVal))
      return nullptr;
    m_Values.push_back(RegVal);
    break;
  }

  case Write:
  {
    if (m_Values.empty())
      return nullptr;
    IntType RegVal = m_Values.back();
    if (!m_pCpuCtxt->WriteRegister(spIdExpr->GetId(), RegVal))
      return nullptr;
    m_Values.pop_back();
    break;
  }

  default:
    return nullptr;
  }
  return spIdExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  auto const* pCpuInfo = spVecIdExpr->GetCpuInformation();
  switch (m_State)
  {
  case Read:
  {
    auto VecId = spVecIdExpr->GetVector();
    for (auto Id : VecId)
    {
      IntType RegVal(pCpuInfo->GetSizeOfRegisterInBit(Id), 0);
      if (!m_pCpuCtxt->ReadRegister(Id, RegVal))
        return nullptr;
      m_Values.push_back(RegVal);
    }
    break;
  }

  case Write:
  {
    auto VecId = spVecIdExpr->GetVector();
    for (auto Id : VecId)
    {
      if (m_Values.empty())
        return nullptr;
      if (!m_pCpuCtxt->WriteRegister(Id, m_Values.back()))
        return nullptr;
      m_Values.pop_back();
    }
    break;
  }

  default:
    return nullptr;
  }

  return spVecIdExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr)
{
  switch (m_State)
  {
  case Read:
  {
    IntType RegVal(spTrkIdExpr->GetBitSize(), 0);
    if (!m_pCpuCtxt->ReadRegister(spTrkIdExpr->GetId(), RegVal))
      return nullptr;
    m_Values.push_back(RegVal);
    break;
  }

  case Write:
    if (m_Values.empty())
      return nullptr;
    if (!m_pCpuCtxt->WriteRegister(spTrkIdExpr->GetId(), m_Values.back()))
      return nullptr;
    m_Values.pop_back();
    break;

  default:
    return nullptr;
  }
  return spTrkIdExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  switch (m_State)
  {
  case Unknown:
  {
    switch (spVarExpr->GetAction())
    {
    case VariableExpression::Alloc:
      m_rVars[spVarExpr->GetName()] = IntType();
      break;

    case VariableExpression::Free:
      m_rVars.erase(spVarExpr->GetName());
      break;

    default:
      return nullptr;
    }
  }

  case Read:
    if (spVarExpr->GetAction() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      m_Values.push_back(itVar->second);
      break;
    }
    else
      return nullptr;

  case Write:
    if (spVarExpr->GetAction() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      itVar->second = m_Values.back();
      m_Values.pop_back();
      break;
    }
    else
      return nullptr;

  default:
    return nullptr;
  }

  return spVarExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  State OldState = m_State;
  m_State = Read;
  auto spBaseExpr = spMemExpr->GetBaseExpression() ? spMemExpr->GetBaseExpression()->Visit(this) : nullptr;
  auto spOffsetExpr = spMemExpr->GetOffsetExpression()->Visit(this);
  m_State = OldState;
  if (spOffsetExpr == nullptr)
    return nullptr;

  TBase Base = 0;
  if (spBaseExpr != nullptr)
  {
    if (m_Values.size() < 2)
      return nullptr;
    Base = m_Values.back().ConvertTo<u16>();
    m_Values.pop_back();
  }

  if (m_Values.size() < 1)
    return nullptr;
  TOffset Offset = m_Values.back().ConvertTo<TOffset>();
  m_Values.pop_back();

  Address Addr(Base, Offset);

  u64 LinAddr = 0;
  if (m_pCpuCtxt->Translate(Addr, LinAddr))
    LinAddr = Offset;

  switch (m_State)
  {
  default: return nullptr;

  case Read:
  {
    if (spMemExpr->IsDereferencable())
    {
      IntType MemVal(spMemExpr->GetAccessSizeInBit(), 0);
      if (!m_pMemCtxt->ReadMemory(LinAddr, MemVal))
        return nullptr;
      m_Values.push_back(MemVal);
    }
    else
    {
      m_Values.push_back(IntType(spMemExpr->GetAccessSizeInBit(), LinAddr));
    }
    break;
  }

  case Write:
  {
    if (!spMemExpr->IsDereferencable())
      return nullptr;
    if (m_Values.empty())
      return nullptr;
    if (!m_pMemCtxt->WriteMemory(LinAddr, m_Values.back()))
      return nullptr;
    m_Values.pop_back();
    break;
  }
  }

  return spMemExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  // TODO:
  return nullptr;
}

bool InterpreterEmulator::InterpreterExpressionVisitor::_EvaluateComparison(u8 CondOp, bool& rRes)
{
  if (m_Values.size() < 2)
    return false;

  auto TestVal = m_Values.back();
  m_Values.pop_back();
  auto RefVal = m_Values.back();
  m_Values.pop_back();

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
    return false;
  }

  return false;
}