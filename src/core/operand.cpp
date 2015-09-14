#include "medusa/operand.hpp"
#include "medusa/extend.hpp"

MEDUSA_NAMESPACE_BEGIN

u8 Operand::GetLength(void) const
{
  switch (m_Type & MS_MASK)
  {
  case MS_8BIT:  return 1;
  case MS_16BIT: return 2;
  case MS_32BIT: return 4;
  case MS_64BIT: return 8;
  }
  switch (m_Type & DS_MASK)
  {
  case DS_8BIT:  return 1;
  case DS_16BIT: return 2;
  case DS_32BIT: return 4;
  case DS_64BIT: return 8;
  }
  switch (m_Type & RS_MASK)
  {
  case RS_8BIT:  return 1;
  case RS_16BIT: return 2;
  case RS_32BIT: return 4;
  case RS_64BIT: return 8;
  }
  return 0;
}

u8 Operand::GetRawLength(void) const
{
  switch (m_Type & DS_MASK)
  {
  case DS_8BIT:  return sizeof(u8);
  case DS_16BIT: return sizeof(u16);
  case DS_32BIT: return sizeof(u32);
  case DS_64BIT: return sizeof(u64);
  default:       return 0;
  }
}

u32 Operand::GetBitSize(void) const
{
  switch (m_Type & MS_MASK)
  {
  case MS_8BIT:  return  8;
  case MS_16BIT: return 16;
  case MS_32BIT: return 32;
  case MS_64BIT: return 64;
  }
  switch (m_Type & DS_MASK)
  {
  case DS_8BIT:  return  8;
  case DS_16BIT: return 16;
  case DS_32BIT: return 32;
  case DS_64BIT: return 64;
  }
  switch (m_Type & RS_MASK)
  {
  case RS_8BIT:  return  8;
  case RS_16BIT: return 16;
  case RS_32BIT: return 32;
  case RS_64BIT: return 64;
  }
  return 0;
}

Expression::SPType Operand::GetSemantic(u8 Mode, CpuInformation const* pCpuInfo, u8 InstructionLength /*= 0*/, bool Dereference /*= true*/) const
{
  Expression::SPType spExpr;

  if (m_Type == O_NONE)
      return spExpr;

  if (m_Type & (O_ABS | O_IMM))
  {
    u32 ConstType = IntegerExpression::ConstUnknownBit;
    switch (m_Type & DS_MASK)
    {
    case DS_8BIT:  ConstType = IntegerExpression::Const8Bit; break;
    case DS_16BIT: ConstType = IntegerExpression::Const16Bit; break;
    case DS_32BIT: ConstType = IntegerExpression::Const32Bit; break;
    case DS_64BIT: ConstType = IntegerExpression::Const64Bit; break;
    default: break;
    }
    spExpr = Expr::MakeConstInt(ConstType, m_Value);
  }

  else if (m_Type & (O_REG | O_SREG))
  {
    if (m_Type & O_SREG)
    {
      spExpr = Expr::MakeId(m_SecReg, pCpuInfo);

      // FIXME: pExpr can be nullptr at this state
      if (m_Type & O_SCALE)
        spExpr = Expr::MakeBinOp(
        OperationExpression::OpMul,
        spExpr,
        Expr::MakeConstInt(IntegerExpression::Const8Bit, (m_Type >> 8) & 0xf));
    }

    if (m_Reg != 0x0)
    {
      if (spExpr == nullptr)
        spExpr = Expr::MakeId(m_Reg, pCpuInfo);
      else
        spExpr = Expr::MakeBinOp(
        OperationExpression::OpAdd,
        spExpr,
        Expr::MakeId(m_Reg, pCpuInfo));
    }


    if (m_Type & O_DISP)
    {
      u64 Disp;
      switch (m_Type & DS_MASK)
      {
      case DS_8BIT:  Disp = SignExtend<s64,  8>(m_Value); break;
      case DS_16BIT: Disp = SignExtend<s64, 16>(m_Value); break;
      case DS_32BIT: Disp = SignExtend<s64, 32>(m_Value); break;
      default:       Disp =                     m_Value ; break;
      }

      u32 ConstType = IntegerExpression::ConstUnknownBit;
      switch (m_Type & AS_MASK)
      {
      case AS_8BIT:  ConstType = IntegerExpression::Const8Bit; break;
      case AS_16BIT: ConstType = IntegerExpression::Const16Bit; break;
      case AS_32BIT: ConstType = IntegerExpression::Const32Bit; break;
      case AS_64BIT: ConstType = IntegerExpression::Const64Bit; break;
      default: break;
      }

      if (m_Reg == pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode))
        Disp += InstructionLength;

      spExpr = Expr::MakeBinOp(
        OperationExpression::OpAdd,
        spExpr,
        Expr::MakeConstInt(ConstType, Disp));
    }
  }

  else if (m_Type & O_REL)
  {
    u32 ConstType = IntegerExpression::ConstUnknownBit;
    switch (m_Type & DS_MASK)
    {
    case DS_8BIT:  ConstType = IntegerExpression::Const8Bit; break;
    case DS_16BIT: ConstType = IntegerExpression::Const16Bit; break;
    case DS_32BIT: ConstType = IntegerExpression::Const32Bit; break;
    case DS_64BIT: ConstType = IntegerExpression::Const64Bit; break;
    default: break;
    }

    spExpr = Expr::MakeBinOp(
      OperationExpression::OpAdd,
      Expr::MakeId(pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode), pCpuInfo),
      Expr::MakeConstInt(ConstType, m_Value));
  }

  if (m_Type & O_MEM)
  {
    /* TODO: Quick fix to handle x86 O* */
    if (spExpr == nullptr && m_Type & O_DISP)
    {
      u32 ConstType = IntegerExpression::ConstUnknownBit;
      switch (m_Type & DS_MASK)
      {
      case DS_8BIT:  ConstType = IntegerExpression::Const8Bit; break;
      case DS_16BIT: ConstType = IntegerExpression::Const16Bit; break;
      case DS_32BIT: ConstType = IntegerExpression::Const32Bit; break;
      case DS_64BIT: ConstType = IntegerExpression::Const64Bit; break;
      default: break;
      }
      spExpr = Expr::MakeConstInt(ConstType, m_Value);
    }

    Expression::SPType spBaseExpr;
    if (m_Type & O_SEG)
      spBaseExpr = Expr::MakeId(m_Seg, pCpuInfo);
    else if (m_Type & O_SEG_VAL)
      spBaseExpr = Expr::MakeConstInt(IntegerExpression::Const16Bit, m_SegValue);

/*
#define MS_MASK      0x0000f000
#define MS_8BIT      0x00001000
#define MS_16BIT     0x00002000
#define MS_32BIT     0x00003000
#define MS_64BIT     0x00004000
#define MS_80BIT     0x00005000
#define MS_128BIT    0x00006000
*/
    u32 MemAccessSizeInBit = 0;
    switch (m_Type & MS_MASK)
    {
    case MS_8BIT:  MemAccessSizeInBit = 8;  break;
    case MS_16BIT: MemAccessSizeInBit = 16; break;
    case MS_32BIT: MemAccessSizeInBit = 32; break;
    case MS_64BIT: MemAccessSizeInBit = 64; break;
    }

    spExpr = Expr::MakeMem(MemAccessSizeInBit, spBaseExpr, spExpr, Dereference);
  }

  assert(spExpr != nullptr);
  return spExpr;
}

MEDUSA_NAMESPACE_END
