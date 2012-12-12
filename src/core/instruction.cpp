#include "medusa/instruction.hpp"

MEDUSA_NAMESPACE_BEGIN

Instruction::~Instruction(void)
{
  delete m_pRootExpr;
}

void Instruction::SetSemantic(Expression *pExpr)
{
  delete m_pRootExpr;
  m_pRootExpr = pExpr;
}

u8 Instruction::GetOperandOffset(u8 Oprd) const
{
  if (Oprd < OPERAND_NO)
    return m_Oprd[Oprd].GetOffset();
  return 0;
}

//TODO: Need more work
bool Instruction::GetOperandReference(Database const& rDatabase, u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const
{
  medusa::Operand const* pOprd = Operand(Oprd);
  TOffset Offset = 0x0;

  rAddrDst = rAddrSrc;

  // XXX: Should never happen
  if (pOprd == NULL) return false;

  if (pOprd->GetType() & O_NO_REF)
    return false;

  if ((pOprd->GetType() & O_REL) || ((pOprd->GetType() & O_REG_PC_REL) && (pOprd->GetType() & O_MEM)))
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

  //else if ((pOprd->GetType() & O_MEM))
  //{
  //  if (pOprd->GetType() & O_REG_PC_REL)
  //    Offset += rAddrSrc.GetOffset();

  //  switch (pOprd->GetType() & DS_MASK)
  //  {
  //    case DS_8BIT:   Offset += static_cast<s8> (pOprd->GetValue()) + GetLength(); break;
  //    case DS_16BIT:  Offset += static_cast<s16>(pOprd->GetValue()) + GetLength(); break;
  //    case DS_32BIT:  Offset += static_cast<s32>(pOprd->GetValue()) + GetLength(); break;
  //    case DS_64BIT:  Offset += static_cast<s64>(pOprd->GetValue()) + GetLength(); break;
  //    default:        Offset += pOprd->GetValue() + GetLength();
  //  }

  //  rAddrDst.SetOffset(Offset);
  //  TOffset RawOffset;
  //  MemoryArea const* pMemArea = rDatabase.GetMemoryArea(rAddrDst);
  //  if (!pMemArea->Convert(Offset, RawOffset)) return false;

  //  BinaryStream const& rBinStrm = pMemArea->GetBinaryStream();

  //  u64 ReadOffset = 0x0;
  //  try
  //  {
  //    switch (pOprd->GetType() & MS_MASK)
  //    {
  //    case MS_8BIT:  rBinStrm.Read(RawOffset, ReadOffset); ReadOffset &= 0xff;       break;
  //    case MS_16BIT: rBinStrm.Read(RawOffset, ReadOffset); ReadOffset &= 0xffff;     break;
  //    case MS_32BIT: rBinStrm.Read(RawOffset, ReadOffset); ReadOffset &= 0xffffffff; break;
  //    case MS_64BIT: rBinStrm.Read(RawOffset, ReadOffset);                           break;
  //    default: return false;
  //    }
  //  }
  //  catch(Exception&) { return false; }

  //  rAddrDst.SetOffset(ReadOffset);
  //  return true;
  //}

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

  if (pOprd == NULL) return 0;

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

MEDUSA_NAMESPACE_END
