#include "medusa/instruction.hpp"

MEDUSA_NAMESPACE_BEGIN

Instruction::Instruction(char const* pName, u32 Opcode, u16 Length)
  : Cell(Cell::InstructionType, NoneType)
  , m_pName(pName)
  , m_Opcd(Opcode)
  , m_Prefix()
  , m_TestedFlags()
  , m_UpdatedFlags()
  , m_ClearedFlags()
  , m_FixedFlags()
  , m_Expressions()
{
  m_spDna->Length() = Length;
}

Instruction::Instruction(CellData::SPType spDna)
  : Cell(spDna) 
  , m_pName(nullptr)
  , m_Opcd(0x0)
  , m_Prefix()
  , m_TestedFlags()
  , m_UpdatedFlags()
  , m_ClearedFlags()
  , m_FixedFlags()
  , m_Expressions()
{}

Instruction::~Instruction(void)
{
}

std::string Instruction::ToString(void) const
{
  std::string Res = (boost::format("mnem: %s(%08x), length: %d, prefix: %08x, oprd: %d")
    % m_pName % m_Opcd % m_spDna->GetLength() % m_Prefix % m_Operands.size()).str();
  if (m_Operands.empty())
    return Res;
  Res += "\n";
  for (auto const spOprd : m_Operands)
  {
    Res += spOprd->ToString();
    Res += "\n";
  }
  return Res;
}

char const* Instruction::GetName(void) const
{
  return m_pName;
}

u32 Instruction::GetOpcode(void) const
{
  return m_Opcd;
}

u32 Instruction::GetPrefix(void) const
{
  return m_Prefix;
}

u32 Instruction::GetTestedFlags(void) const
{
  return m_TestedFlags;
}

u32 Instruction::GetUpdatedFlags(void) const
{
  return m_UpdatedFlags;
}

u32 Instruction::GetClearedFlags(void) const
{
  return m_ClearedFlags;
}

u32 Instruction::GetFixedFlags(void) const
{
  return m_FixedFlags;
}

Expression::List const& Instruction::GetSemantic(void) const
{
  return m_Expressions;
}

void Instruction::AddOperand(Expression::SPType spOprdExpr)
{
  m_Operands.push_back(spOprdExpr);
}

Expression::SPType Instruction::GetOperand(u8 OprdNo) const
{
  return OprdNo < m_Operands.size() ? m_Operands[OprdNo] : nullptr;
}

void Instruction::ForEachExpression(Instruction::OperandCallback OprdCb) const
{
  for (auto const& rspOprdExpr : m_Operands)
    OprdCb(rspOprdExpr);
}

void Instruction::SetName(char const* pName)
{
  m_pName = pName;
}

void Instruction::SetOpcode(u32 Opcd)
{
  m_Opcd = Opcd;
}

void Instruction::SetTestedFlags(u32 Flags)
{
  m_TestedFlags = Flags;
}

void Instruction::SetUpdatedFlags(u32 Flags)
{
  m_UpdatedFlags = Flags;
}

void Instruction::SetClearedFlags(u32 Flags)
{
  m_ClearedFlags = Flags;
}

void Instruction::SetFixedFlags(u32 Flags)
{
  m_FixedFlags = Flags;
}

void Instruction::SetSemantic(Expression::List const& rExprList)
{
  m_Expressions = rExprList;
}

void Instruction::SetSemantic(Expression::SPType spExpr)
{
  m_Expressions.clear();
  m_Expressions.push_back(spExpr);
}

void Instruction::AddPreSemantic(Expression::SPType spExpr)
{
  m_Expressions.push_front(spExpr);
}

void Instruction::AddPostSemantic(Expression::SPType spExpr)
{
  m_Expressions.push_back(spExpr);
}

u16& Instruction::Length(void)
{
  return m_spDna->Length();
}

u32& Instruction::Prefix(void)
{
  return m_Prefix;
}

MEDUSA_NAMESPACE_END
