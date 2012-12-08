#include "medusa/operand.hpp"

MEDUSA_NAMESPACE_BEGIN

u8 Operand::GetLength(void) const
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

Expression *Operand::GetSemantic(CpuInformation const* pCpuInfo) const
{
  Expression *pExpr = nullptr;

  if (m_Type == O_NONE)
      return pExpr;

  if (m_Type & (O_ABS | O_IMM))
  {
    u32 ConstType = ConstantExpression::ConstUnknownBit;
    switch (m_Type & DS_MASK)
    {
    case DS_8BIT:  ConstType = ConstantExpression::Const8Bit; break;
    case DS_16BIT: ConstType = ConstantExpression::Const16Bit; break;
    case DS_32BIT: ConstType = ConstantExpression::Const32Bit; break;
    case DS_64BIT: ConstType = ConstantExpression::Const64Bit; break;
    default: break;
    }
    pExpr = new ConstantExpression(ConstType, m_Value);
  }

  else if (m_Type & O_REG)
  {
    pExpr = new IdentifierExpression(m_Reg, pCpuInfo);

    if (m_Type & O_SREG)
      pExpr = new OperationExpression(
        OperationExpression::OpAdd,
        pExpr,
        new IdentifierExpression(m_SecReg, pCpuInfo));

    if (m_Type & O_SCALE)
      pExpr = new OperationExpression(
        OperationExpression::OpMul,
        pExpr,
        new ConstantExpression(ConstantExpression::Const8Bit, (m_Type >> 9) & 0x4));

    if (m_Type & O_DISP)
    {
      u32 ConstType = ConstantExpression::ConstUnknownBit;
      switch (m_Type & DS_MASK)
      {
      case DS_8BIT:  ConstType = ConstantExpression::Const8Bit; break;
      case DS_16BIT: ConstType = ConstantExpression::Const16Bit; break;
      case DS_32BIT: ConstType = ConstantExpression::Const32Bit; break;
      case DS_64BIT: ConstType = ConstantExpression::Const64Bit; break;
      default: break;
      }

      pExpr = new OperationExpression(
        OperationExpression::OpAdd,
        pExpr,
        new ConstantExpression(ConstType, m_Value));
    }
  }

  else if (m_Type & O_REL)
  {
    u32 ConstType = ConstantExpression::ConstUnknownBit;
    switch (m_Type & DS_MASK)
    {
    case DS_8BIT:  ConstType = ConstantExpression::Const8Bit; break;
    case DS_16BIT: ConstType = ConstantExpression::Const16Bit; break;
    case DS_32BIT: ConstType = ConstantExpression::Const32Bit; break;
    case DS_64BIT: ConstType = ConstantExpression::Const64Bit; break;
    default: break;
    }

    pExpr = new OperationExpression(
      OperationExpression::OpAdd,
      new IdentifierExpression(pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister), pCpuInfo),
      new ConstantExpression(ConstType, m_Value));
  }

  if (m_Type & O_MEM)
  {
    Expression *pBaseExpr = nullptr;
    if (m_Type & O_SEG)
      pBaseExpr = new IdentifierExpression(m_Seg, pCpuInfo);
    else if (m_Type & O_SEG_VAL)
      pBaseExpr = new ConstantExpression(ConstantExpression::Const16Bit, m_SegValue);

    pExpr = new MemoryExpression(pBaseExpr, pExpr);
  }

  return pExpr;
}

MEDUSA_NAMESPACE_END