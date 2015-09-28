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

bool InterpreterEmulator::Execute(Expression::VSPType const& rExprs)
{
  for (Expression::SPType spExpr : rExprs)
  {
    if (auto spSys = expr_cast<SystemExpression>(spExpr))
    {
      if (spSys->GetName() == "call_insn_cb")
      {
        if (m_InsnCb)
          m_InsnCb(m_pCpuCtxt, m_pMemCtxt, spSys->GetAddress());
        continue;
      }

      if (spSys->GetName() == "check_exec_hook")
      {
        Address CurAddr;
        if (!m_pCpuCtxt->GetAddress(CpuContext::AddressExecution, CurAddr))
          return false;
        // HACK(KS):
        CurAddr.SetBase(0x0);
        TestHook(CurAddr, Emulator::HookOnExecute);
        continue;
      }

      if (spSys->GetName() == "stop")
      {
        return false;
      }
    }

    InterpreterExpressionVisitor Visitor(m_Hooks, m_pCpuCtxt, m_pMemCtxt, m_Vars);
    auto spCurExpr = spExpr->Visit(&Visitor);
    if (spCurExpr == nullptr)
    {
      std::cout << m_pCpuCtxt->ToString() << std::endl;
      std::cout << spExpr->ToString() << std::endl;
      return false;
    }
  }

  return true;
}

InterpreterEmulator::InterpreterExpressionVisitor::~InterpreterExpressionVisitor(void)
{
  if (!m_Values.empty())
  {
    Log::Write("emul_interpreter") << "unconsumed value" << LogEnd;
    for (auto const& rValue : m_Values)
    {
      Log::Write("emul_interpreter") << "leaked value: " << rValue.ToString() << LogEnd;
    }
  }
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  Log::Write("emul_interpreter").Level(LogError) << "not yet implemented system called" << LogEnd;
  return nullptr; // TODO
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  Expression::LSPType SmplExprList;
  for (Expression::SPType spExpr : spBindExpr->GetBoundExpressions())
    if (spExpr->Visit(this) == nullptr)
      return nullptr;
  return spBindExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spTernExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spTernExpr->GetTestExpression()->Visit(this);
  m_State = m_State;

  if (spRefExpr == nullptr || spTestExpr == nullptr)
    return nullptr;

  bool Cond;
  if (!_EvaluateComparison(spTernExpr->GetType(), Cond))
    return nullptr;

  if (Cond)
  {
    auto spTrueExpr = spTernExpr->GetTrueExpression()->Visit(this);
    if (spTrueExpr == nullptr)
      return nullptr;
  }
  else
  {
    auto spFalseExpr = spTernExpr->GetFalseExpression()->Visit(this);
    if (spFalseExpr == nullptr)
      return nullptr;
  }

  return spTernExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spIfElseExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spIfElseExpr->GetTestExpression()->Visit(this);
  m_State = OldState;

  if (spRefExpr == nullptr || spTestExpr == nullptr)
    return nullptr;

  bool Cond;
  if (!_EvaluateComparison(spIfElseExpr->GetType(), Cond))
    return nullptr;

  if (Cond)
  {
    auto spThenExpr = spIfElseExpr->GetThenExpression()->Visit(this);
    if (spThenExpr == nullptr)
      return nullptr;
  }
  else if (spIfElseExpr->GetElseExpression() != nullptr)
  {
    auto spElseExpr = spIfElseExpr->GetElseExpression()->Visit(this);
    if (spElseExpr == nullptr)
      return nullptr;
  }

  return spIfElseExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  while (true)
  {
    State OldState = m_State;
    m_State = Read;
    auto spRefExpr = spWhileExpr->GetReferenceExpression()->Visit(this);
    auto spTestExpr = spWhileExpr->GetTestExpression()->Visit(this);
    m_State = OldState;

    if (spRefExpr == nullptr || spTestExpr == nullptr)
      return nullptr;

    bool Cond;
    if (!_EvaluateComparison(spWhileExpr->GetType(), Cond))
      return nullptr;

    if (!Cond)
      break;

    auto spBodyExpr = spWhileExpr->GetBodyExpression()->Visit(this);
    if (spBodyExpr == nullptr)
      return nullptr;
  }

  return spWhileExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  if (auto spDstVecId = expr_cast<VectorIdentifierExpression>(spAssignExpr->GetDestinationExpression()))
    m_NrOfValueToRead = spDstVecId->GetVector().size();
  else
    m_NrOfValueToRead = 0;

  State OldState = m_State;

  m_State = Read;
  auto spSrc = spAssignExpr->GetSourceExpression()->Visit(this);
  if (spSrc == nullptr)
  {
    m_State = OldState;
    Log::Write("emul_interpreter").Level(LogError) << "failed to execute source of expression: " << spAssignExpr->ToString() << LogEnd;
    return nullptr;
  }
  m_State = Write;
  auto spDst = spAssignExpr->GetDestinationExpression()->Visit(this);
  if (spDst == nullptr)
  {
    m_State = OldState;
    Log::Write("emul_interpreter").Level(LogError) << "failed to execute destination of expression: " << spAssignExpr->ToString() << LogEnd;
    return nullptr;
  }
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
    Log::Write("emul_interpreter").Level(LogError) << "unknown unary operator" << LogEnd;
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

    case OperationExpression::OpRol:
      m_Values.push_back(LeftVal.Rol(RightVal));
      break;

    case OperationExpression::OpRor:
      m_Values.push_back(LeftVal.Ror(RightVal));
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

    case OperationExpression::OpClearBits:
      m_Values.push_back(LeftVal & ~RightVal);
      break;

    case OperationExpression::OpBcast:
    {
      auto Result = LeftVal;
      Result.BitCast(RightVal.ConvertTo<u16>());
      m_Values.push_back(Result);
      break;
    }

    case OperationExpression::OpFAdd:
      m_Values.push_back(LeftVal.FAdd(RightVal));
      break;

    case OperationExpression::OpFSub:
      m_Values.push_back(LeftVal.FSub(RightVal));
      break;

    case OperationExpression::OpFMul:
      m_Values.push_back(LeftVal.FMul(RightVal));
      break;

    case OperationExpression::OpFDiv:
      m_Values.push_back(LeftVal.FDiv(RightVal));
      break;

    case OperationExpression::OpFMod:
      m_Values.push_back(LeftVal.FMod(RightVal));
      break;

    default:
      Log::Write("emul_interpreter").Level(LogError) << "unknown binary operator" << LogEnd;
      return nullptr;
  }

  return spBinOpExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitBitVector(BitVectorExpression::SPType spConstExpr)
{
  if (m_State != Read)
  {
    Log::Write("emul_interpreter").Level(LogError) << "constant can only be read" << LogEnd;
    return nullptr;
  }
  m_Values.push_back(spConstExpr->GetInt());
  return spConstExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  switch (m_State)
  {
  case Read:
  {
    BitVector RegVal(spIdExpr->GetBitSize(), 0);
    if (!m_pCpuCtxt->ReadRegister(spIdExpr->GetId(), RegVal))
    {
      Log::Write("emul_interpreter").Level(LogError) << "unable to read register " << m_pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(spIdExpr->GetId()) << LogEnd;
      return nullptr;
    }
    m_Values.push_back(RegVal);
    break;
  }

  case Write:
  {
    if (m_Values.empty())
      return nullptr;
    BitVector RegVal = m_Values.back();
    if (!m_pCpuCtxt->WriteRegister(spIdExpr->GetId(), RegVal))
    {
      Log::Write("emul_interpreter").Level(LogError) << "unable to write register " << m_pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(spIdExpr->GetId()) << LogEnd;
      return nullptr;
    }
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
      BitVector RegVal(pCpuInfo->GetSizeOfRegisterInBit(Id), 0);
      if (!m_pCpuCtxt->ReadRegister(Id, RegVal))
      {
        Log::Write("emul_interpreter").Level(LogError) << "unable to read register" << LogEnd;
        return nullptr;
      }
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
      {
        Log::Write("emul_interpreter").Level(LogError) << "no value to write into register" << LogEnd;
        return nullptr;
      }
      if (!m_pCpuCtxt->WriteRegister(Id, m_Values.back()))
      {
        Log::Write("emul_interpreter").Level(LogError) << "unable to write register" << LogEnd;
        return nullptr;
      }
      m_Values.pop_back();
    }
    break;
  }

  default:
    return nullptr;
  }

  return spVecIdExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
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
      m_rVars[spVarExpr->GetName()] = BitVector();
      break;

    case VariableExpression::Free:
      m_rVars.erase(spVarExpr->GetName());
      break;

    default:
      Log::Write("emul_interpreter").Level(LogError) << "unknown variable action" << LogEnd;
      return nullptr;
    }
    break;
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
    {
      Log::Write("emul_interpreter").Level(LogError) << "invalid state for variable reading" << LogEnd;
      return nullptr;
    }

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
    {
      Log::Write("emul_interpreter").Level(LogError) << "invalid state for variable writing" << LogEnd;
      return nullptr;
    }

  default:
    return nullptr;
  }

  return spVarExpr;
}

Expression::SPType InterpreterEmulator::InterpreterExpressionVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  State OldState = m_State;
  m_State = Read;
  auto spOffsetExpr = spMemExpr->GetOffsetExpression()->Visit(this);
  auto spBaseExpr = spMemExpr->GetBaseExpression() ? spMemExpr->GetBaseExpression()->Visit(this) : nullptr;
  m_State = OldState;
  if (spOffsetExpr == nullptr)
  {
    Log::Write("emul_interpreter").Level(LogError) << "invalid offset" << LogEnd;
    return nullptr;
  }

  TBase Base = 0;
  if (spBaseExpr != nullptr)
  {
    if (m_Values.size() < 2)
    {
      Log::Write("emul_interpreter").Level(LogError) << "no value for address base" << LogEnd;
      return nullptr;
    }
    Base = m_Values.back().ConvertTo<u16>();
    m_Values.pop_back();
  }

  if (m_Values.size() < 1)
  {
    Log::Write("emul_interpreter").Level(LogError) << "no value for address offset" << LogEnd;
    return nullptr;
  }
  TOffset Offset = m_Values.back().ConvertTo<TOffset>();
  m_Values.pop_back();

  Address Addr(Base, Offset);

  u64 LinAddr = 0;
  if (!m_pCpuCtxt->Translate(Addr, LinAddr))
    LinAddr = Offset;

  switch (m_State)
  {
  default:
    Log::Write("emul_interpreter").Level(LogError) << "unknown state for address" << LogEnd;
    return nullptr;

  case Read:
  {
    if (spMemExpr->IsDereferencable())
    {
      if (m_NrOfValueToRead == 0)
      {
        BitVector MemVal(spMemExpr->GetAccessSizeInBit(), 0);
        if (!m_pMemCtxt->ReadMemory(LinAddr, MemVal))
        {
          Log::Write("emul_interpreter").Level(LogError) << "unable to read memory at address: " << LinAddr << LogEnd;
          return nullptr;
        }
        m_Values.push_back(MemVal);
      }
      while (m_NrOfValueToRead != 0)
      {
        BitVector MemVal(spMemExpr->GetAccessSizeInBit(), 0);
        if (!m_pMemCtxt->ReadMemory(LinAddr, MemVal))
        {
          Log::Write("emul_interpreter").Level(LogError) << "unable to read memory at address: " << LinAddr << LogEnd;
          return nullptr;
        }
        LinAddr += MemVal.GetBitSize() / 8;
        m_Values.push_back(MemVal);
        --m_NrOfValueToRead;
      }
    }
    else
    {
      m_Values.push_back(BitVector(spMemExpr->GetAccessSizeInBit(), LinAddr));
    }
    break;
  }

  case Write:
  {
    if (m_Values.empty())
    {
      Log::Write("emul_interpreter").Level(LogError) << "unable to write memory at address: " << LinAddr << LogEnd;
      return nullptr;
    }

    // NOTE: Trying to write an non-deferencable address is like
    // changing its offset.
    if (!spMemExpr->IsDereferencable())
    {
      auto spOffsetExpr = spMemExpr->GetOffsetExpression()->Visit(this);
      if (spOffsetExpr == nullptr)
        return nullptr;
      break;
    }

    do
    {
      auto MemVal = m_Values.back();
      if (!m_pMemCtxt->WriteMemory(LinAddr, MemVal))
      {
        Log::Write("emul_interpreter").Level(LogError) << "unable to write memory at address: " << LinAddr << LogEnd;
        return nullptr;
      }
      m_Values.pop_back();
      LinAddr += MemVal.GetBitSize() / 8;
    } while (!m_Values.empty());

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
  {
    Log::Write("emul_interpreter").Level(LogError) << "no enough values to do comparison" << LogEnd;
    return false;
  }

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
    Log::Write("emul_interpreter") << "unknown comparison" << LogEnd;
    return false;
  }

  return true;
}