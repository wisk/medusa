#include "medusa/instruction.hpp"

MEDUSA_NAMESPACE_BEGIN

Instruction::~Instruction(void)
{
}

void Instruction::SetSemantic(Expression::List const& rExprList)
{
  m_Expressions = rExprList;
}

void Instruction::SetSemantic(Expression::SPtr spExpr)
{
  m_Expressions.clear();
  m_Expressions.push_back(spExpr);
}

void Instruction::AddPreSemantic(Expression::SPtr spExpr)
{
  m_Expressions.push_front(spExpr);
}

void Instruction::AddPostSemantic(Expression::SPtr spExpr)
{
  m_Expressions.push_back(spExpr);
}

u8 Instruction::GetOperandOffset(u8 Oprd) const
{
  if (Oprd < OPERAND_NO)
    return m_Oprd[Oprd].GetOffset();
  return 0;
}

//TODO: Need more work
bool Instruction::GetOperandReference(Document const& rDoc, u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const
{
  medusa::Operand const* pOprd = Operand(Oprd);
  TOffset Offset = 0x0;

  rAddrDst = rAddrSrc;

  // XXX: Should never happen
  if (pOprd == nullptr) return false;

  if (pOprd->GetType() & O_NO_REF)
    return false;

  if (pOprd->GetType() & O_REL)
  {
    switch (pOprd->GetType() & DS_MASK)
    {
      case DS_8BIT:   Offset = static_cast<s8> (pOprd->GetValue()) + GetLength(); break;
      case DS_16BIT:  Offset = static_cast<s16>(pOprd->GetValue()) + GetLength(); break;
      case DS_32BIT:  Offset = static_cast<s32>(pOprd->GetValue()) + GetLength(); break;
      case DS_64BIT:  Offset = static_cast<s64>(pOprd->GetValue()) + GetLength(); break;
      default:        Offset = pOprd->GetValue() + GetLength();
    }

    rAddrDst = rAddrSrc + Offset;
    return true;
  }

  else if ((pOprd->GetType() & O_MEM))
  {
    if (pOprd->GetType() & O_REG_PC_REL)
      Offset += rAddrSrc.GetOffset();

    switch (pOprd->GetType() & DS_MASK)
    {
      case DS_8BIT:   Offset += static_cast<s8> (pOprd->GetValue()); break;
      case DS_16BIT:  Offset += static_cast<s16>(pOprd->GetValue()); break;
      case DS_32BIT:  Offset += static_cast<s32>(pOprd->GetValue()); break;
      case DS_64BIT:  Offset += static_cast<s64>(pOprd->GetValue()); break;
      default:        Offset += pOprd->GetValue();
    }

    rAddrDst.SetOffset(Offset);
    return true;
  }

  else if ((pOprd->GetType() & O_ABS) || (pOprd->GetType() & O_IMM) || (pOprd->GetType() & O_DISP))
  {
    switch (pOprd->GetType() & DS_MASK)
    {
      case DS_8BIT:   rAddrDst.SetOffset(static_cast<s8> (pOprd->GetValue())); break;
      case DS_16BIT:  rAddrDst.SetOffset(static_cast<s16>(pOprd->GetValue())); break;
      case DS_32BIT:  rAddrDst.SetOffset(static_cast<s32>(pOprd->GetValue())); break;
      case DS_64BIT:  rAddrDst.SetOffset(static_cast<s64>(pOprd->GetValue())); break;
      default:        rAddrDst.SetOffset(pOprd->GetValue());
    }

    return true;
  }

  return false;
}

u8 Instruction::GetOperandReferenceLength(u8 Oprd) const
{
  medusa::Operand const* pOprd = Operand(Oprd);

  if (pOprd == nullptr) return 0;

  switch (pOprd->GetType() & MS_MASK)
  {
  case MS_8BIT:   return  8;
  case MS_16BIT:  return 16;
  case MS_32BIT:  return 32;
  case MS_64BIT:  return 64;
  default:        return  0;
  }
}

bool Instruction::GetOperandAddress(u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const
{
  u8 OpOff = GetOperandOffset(Oprd);
  if (OpOff == 0)
    return false;

  rAddrDst = rAddrSrc + OpOff;
  return true;
}

// UNTESTED
bool Instruction::GetIndirectReferences(Document const& rDoc, u8 Oprd, Address::List& rRefAddr) const
{
  medusa::Operand const* pOprd = Operand(Oprd);
  TOffset Offset = 0x0;

  Address RefAddr;

  // XXX: Should never happen
  if (pOprd == nullptr) return false;

  if (pOprd->GetType() & (O_NO_REF | O_REG_PC_REL))
    return false;

  if ((pOprd->GetType() & (O_MEM | O_DISP)) != (O_MEM | O_DISP))
    return false;

  switch (pOprd->GetType() & DS_MASK)
  {
  case DS_8BIT:   RefAddr.SetOffset(static_cast<s8> (pOprd->GetValue())); break;
  case DS_16BIT:  RefAddr.SetOffset(static_cast<s16>(pOprd->GetValue())); break;
  case DS_32BIT:  RefAddr.SetOffset(static_cast<s32>(pOprd->GetValue())); break;
  case DS_64BIT:  RefAddr.SetOffset(static_cast<s64>(pOprd->GetValue())); break;
  default:        RefAddr.SetOffset(pOprd->GetValue());
  }


  TOffset RawOffset;
  MemoryArea const* pMemArea = rDoc.GetMemoryArea(RefAddr);
  if (pMemArea == nullptr)
    return false;
  if (!pMemArea->ConvertOffsetToFileOffset(Offset, RawOffset))
    return false;

  BinaryStream const& rBinStrm = rDoc.GetBinaryStream();

  u8 ReadSize = GetOperandReferenceLength(Oprd);
  if (ReadSize == 0)
    return false;

  u64 ReadOffset;
  while (true)
  {
    ReadOffset = 0x0;
    try
    {
      rBinStrm.Read(RawOffset, ReadOffset);
      ReadOffset &= ((1 << ReadSize) - 1);
    }
    catch(Exception&) { return rRefAddr.empty() ? false : true; }

    //if (
    //  rDoc.ContainsCode(Address(0, ReadOffset)) == false &&
    //  rDoc.ContainsData(Address(0, ReadOffset)) == false)
    //  break;

    rRefAddr.push_back(Address(0, ReadOffset));

    RawOffset += (ReadSize / 8);
  }

  return rRefAddr.empty() ? false : true;
}

MEDUSA_NAMESPACE_END
