#include "medusa/instruction.hpp"

MEDUSA_NAMESPACE_BEGIN

Instruction::Instruction(Instruction const& rInsn)
{
  *this = rInsn;
}

Instruction& Instruction::operator=(Instruction const& rInsn)
{
  if (this != &rInsn)
  {
    m_OperationType = rInsn.m_OperationType;
    m_pName         = rInsn.m_pName;
    for (u8 i = 0; i < OPERAND_NO; ++i)
      m_Oprd[i]     = rInsn.m_Oprd[i];
    m_Opcd          = rInsn.m_Opcd;
    m_Length        = rInsn.m_Length;
    m_Prefix        = rInsn.m_Prefix;
    m_TestedFlags   = rInsn.m_TestedFlags;
    m_UpdatedFlags  = rInsn.m_UpdatedFlags;
    m_ClearedFlags  = rInsn.m_ClearedFlags;
    m_FixedFlags    = rInsn.m_FixedFlags;

    for (auto itExpr = std::begin(m_Expressions); itExpr != std::end(m_Expressions); ++itExpr)
      delete *itExpr;
    m_Expressions.erase(std::begin(m_Expressions), std::end(m_Expressions));
    for (auto itExpr = std::begin(rInsn.m_Expressions); itExpr != std::end(rInsn.m_Expressions); ++itExpr)
      m_Expressions.push_back((*itExpr)->Clone());
  }
  return *this;
}

Instruction::~Instruction(void)
{
  for (auto itExpr = std::begin(m_Expressions); itExpr != std::end(m_Expressions); ++itExpr)
    delete *itExpr;
  m_Expressions.erase(std::begin(m_Expressions), std::end(m_Expressions));
}

void Instruction::SetSemantic(Expression::List const& rExprList)
{
  for (auto itExpr = std::begin(m_Expressions); itExpr != std::end(m_Expressions); ++itExpr)
    delete *itExpr;
  m_Expressions.erase(std::begin(m_Expressions), std::end(m_Expressions));
  m_Expressions = rExprList;
}

void Instruction::SetSemantic(Expression* pExpr)
{
  for (auto itExpr = std::begin(m_Expressions); itExpr != std::end(m_Expressions); ++itExpr)
    delete *itExpr;
  m_Expressions.erase(std::begin(m_Expressions), std::end(m_Expressions));
  m_Expressions.push_back(pExpr);
}

void Instruction::AddPreSemantic(Expression* pExpr)
{
  m_Expressions.push_front(pExpr);
}

void Instruction::AddPostSemantic(Expression* pExpr)
{
  m_Expressions.push_back(pExpr);
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

  rAddrDst = rDoc.MakeAddress(rAddrSrc.GetBase(), rAddrSrc.GetOffset());

  // XXX: Should never happen
  if (pOprd == nullptr) return false;

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

  else if ((pOprd->GetType() & O_MEM))
  {
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

    rAddrDst.SetOffset(Offset);
    TOffset RawOffset;
    MemoryArea const* pMemArea = rDoc.GetMemoryArea(rAddrDst);
    if (pMemArea == nullptr)
      return false;
    if (!pMemArea->ConvertOffsetToFileOffset(Offset, RawOffset))
      return false;

    BinaryStream const& rBinStrm = rDoc.GetFileBinaryStream();

    u64 ReadOffset = 0x0;
    try
    {
      switch (pOprd->GetType() & MS_MASK)
      {
      case MS_8BIT:  rBinStrm.Read(RawOffset, ReadOffset); ReadOffset &= 0xff;       break;
      case MS_16BIT: rBinStrm.Read(RawOffset, ReadOffset); ReadOffset &= 0xffff;     break;
      case MS_32BIT: rBinStrm.Read(RawOffset, ReadOffset); ReadOffset &= 0xffffffff; break;
      case MS_64BIT: rBinStrm.Read(RawOffset, ReadOffset);                           break;
      default: return false;
      }
    }
    catch(Exception&) { return false; }

    rAddrDst.SetOffset(ReadOffset);
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

  BinaryStream const& rBinStrm = rDoc.GetFileBinaryStream();

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
