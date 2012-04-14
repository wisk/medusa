#include "medusa/instruction.hpp"

MEDUSA_NAMESPACE_BEGIN

//void                  Instruction::Load(SerializeEntity::SPtr spSrlzEtt)
//{
//  spSrlzEtt->GetField("type",     m_OperationType );
//  spSrlzEtt->GetField("comment",  m_Comment       );
//  spSrlzEtt->GetField("op_type",  m_OperationType );
//  spSrlzEtt->GetField("name",     m_pName         );
//  spSrlzEtt->GetField("opcd",     m_Opcd          );
//  spSrlzEtt->GetField("length",   m_Length        );
//  spSrlzEtt->GetField("cond",     m_Cond          );
//  spSrlzEtt->GetField("prefix",   m_Prefix        );
//
//  size_t OprdCount = 0;
//  for (SerializeEntity::SPtrList::const_iterator It = spSrlzEtt->BeginSubEntities();
//    It != spSrlzEtt->EndSubEntities(); ++It)
//  {
//    if (OprdCount >= OPERAND_NO)
//      throw Exception(L"Database is corrupted (too much operands)");
//
//    m_Oprd[OprdCount].Load(*It);
//    OprdCount++;
//  }
//}
//
//SerializeEntity::SPtr Instruction::Save(void)
//{
//  SerializeEntity::SPtr Instruction(new SerializeEntity("instruction"));
//  Instruction->AddField("type",     m_Type          );
//  Instruction->AddField("comment",  m_Comment       );
//  Instruction->AddField("op_type",  m_OperationType );
//  Instruction->AddField("name",     m_pName         );
//  Instruction->AddField("opcd",     m_Opcd          );
//  Instruction->AddField("length",   m_Length        );
//  Instruction->AddField("cond",     m_Cond          );
//  Instruction->AddField("prefix",   m_Prefix        );
//
//  for (size_t i = 0; i < OPERAND_NO; ++i)
//    Instruction->AddSubEntity(m_Oprd[i].Save());
//
//  return Instruction;
//}

u8 Instruction::GetOperandOffset(u8 Oprd) const
{
  if (Oprd < OPERAND_NO)
    return m_Oprd[Oprd].GetOffset();
  return 0;
}

//TODO: Need more work
bool Instruction::GetOperandReference(BinaryStream const& rBinStrm, u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const
{
  medusa::Operand const* pOprd = Operand(Oprd);
  TOffset Offset = 0x0;

  rAddrDst = rAddrSrc;

  // XXX: Should never happen
  if (pOprd == NULL) return false;

  if (pOprd->GetType() & O_NO_REF)
    return false;

  if ((pOprd->GetType() & O_REL) || ((pOprd->GetType() & O_REG_PC_REL) && !(pOprd->GetType() & O_MEM)))
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
    return false;
    if (pOprd->GetType() & O_REG_PC_REL)
      Offset += rAddrSrc.GetOffset();

    switch (pOprd->GetType() & DS_MASK)
    {
      case DS_8BIT:   Offset += static_cast<s8> (pOprd->GetValue()) + GetLength(); break;
      case DS_16BIT:  Offset += static_cast<s16>(pOprd->GetValue()) + GetLength(); break;
      case DS_32BIT:  Offset += static_cast<s32>(pOprd->GetValue()) + GetLength(); break;
      case DS_64BIT:  Offset += static_cast<s64>(pOprd->GetValue()) + GetLength(); break;
      default:        Offset += pOprd->GetValue() + GetLength();
    }

    u64 ReadOffset = 0x0;
    try
    {
      switch (pOprd->GetType() & MS_MASK)
      {
      case MS_8BIT:  rBinStrm.Read(Offset, ReadOffset); ReadOffset &= 0xff;       break;
      case MS_16BIT: rBinStrm.Read(Offset, ReadOffset); ReadOffset &= 0xffff;     break;
      case MS_32BIT: rBinStrm.Read(Offset, ReadOffset); ReadOffset &= 0xffffffff; break;
      case MS_64BIT: rBinStrm.Read(Offset, ReadOffset);                           break;
      default: return false;
      }
    }
    catch(Exception&) { return false; }

    rAddrDst.SetOffset(ReadOffset);
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