#include "medusa/instruction.hpp"
#include "medusa/expression_visitor.hpp"
#include <fmt/format.h>

MEDUSA_NAMESPACE_BEGIN

Instruction::Instruction(char const* pName, u32 Opcode, u16 Size)
  : Cell(Cell::InstructionType, NoneType, Size)
  , m_pFormat(nullptr)
  , m_pName(pName)
  , m_Opcode(Opcode)
  , m_Prefix()
  , m_TestedFlags()
  , m_UpdatedFlags()
  , m_ClearedFlags()
  , m_FixedFlags()
  , m_Expressions()
{
}

Instruction::Instruction(CellData::SPType spDna)
  : Cell(spDna)
  , m_pFormat(nullptr)
  , m_pName(nullptr)
  , m_Opcode(0x0)
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
  std::string Res = fmt::format("mnem: %s(%08x), length: %d, prefix: %08x, oprd: %d",
    m_pName, m_Opcode, m_spDna->GetSize(), m_Prefix, m_Operands.size());
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

char const* Instruction::GetFormat(void) const
{
  return m_pFormat;
}

char const* Instruction::GetName(void) const
{
  return m_pName;
}

u32 Instruction::GetOpcode(void) const
{
  return m_Opcode;
}

u32 Instruction::GetPrefix(void) const
{
  return m_Prefix;
}

u32 Instruction::GetAttributes(void) const
{
  return m_Attributes;
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

Expression::LSPType  const& Instruction::GetSemantic(void) const
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

bool Instruction::GetOperandReference(Document const& rDoc, u8 OprdNo, Address const& rCurAddr, Address& rDstAddr, bool EvalMemRef) const
{
  auto spOprd = GetOperand(OprdNo);
  if (spOprd == nullptr)
    return false;

  EvaluateVisitor EvalVst(rDoc, rCurAddr, GetMode(), EvalMemRef);
  auto spResExpr = spOprd->Visit(&EvalVst);
  if (spResExpr == nullptr)
    return false;
  rDstAddr = rCurAddr;

  auto spMemExpr = expr_cast<MemoryExpression>(spResExpr);
  if (spMemExpr != nullptr)
  {
    auto spBaseExpr = expr_cast<BitVectorExpression>(spMemExpr->GetBaseExpression());
    if (spBaseExpr != nullptr)
      rDstAddr.SetBase(spBaseExpr->GetInt().ConvertTo<BaseType>());

    auto spOffExpr = expr_cast<BitVectorExpression>(spMemExpr->GetOffsetExpression());
    if (spOffExpr == nullptr)
      return false;
    rDstAddr.SetOffset(spOffExpr->GetInt().ConvertTo<OffsetType>());
    return true;
  }

  // HACK: We ignore 8-bit const since it's probably not a reference
  auto spConstExpr = expr_cast<BitVectorExpression>(spResExpr);
  if (spConstExpr != nullptr && spConstExpr->GetBitSize() > 8)
  {
    rDstAddr.SetOffset(spConstExpr->GetInt().ConvertTo<OffsetType>());
    return true;
  }

  return false;
}

u8 Instruction::GetNumberOfOperand(void) const
{
  return static_cast<u8>(m_Operands.size());
}

void Instruction::ForEachOperand(Instruction::OperandCallback OprdCb) const
{
  for (auto const& rspOprdExpr : m_Operands)
    OprdCb(rspOprdExpr);
}

void Instruction::SetFormat(char const* pFormat)
{
  m_pFormat = pFormat;
}

void Instruction::SetName(char const* pName)
{
  m_pName = pName;
}

void Instruction::SetMnemonic(char const* pMnem)
{
  m_pName = pMnem;
}

void Instruction::AddMnemonicPrefix(char const* pPrefix)
{
  m_MnemonicPrefix += pPrefix;
}

void Instruction::AddMnemonicSuffix(char const* pSuffix)
{
  m_MnemonicSuffix += pSuffix;
}

void Instruction::AddAttribute(u32 Attr)
{
  m_Attributes |= Attr;
}

void Instruction::SetOpcode(u32 Opcd)
{
  m_Opcode = Opcd;
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

void Instruction::SetSemantic(Expression::LSPType  const& rExprList)
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

u16& Instruction::Size(void)
{
  return m_spDna->Size();
}

u32& Instruction::Prefix(void)
{
  return m_Prefix;
}

MEDUSA_NAMESPACE_END
