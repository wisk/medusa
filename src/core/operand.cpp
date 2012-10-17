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
    pExpr = new ConstantExpression(0, m_Value);

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
        new ConstantExpression(0, (m_Type >> 9) & 0x4));

    if (m_Type & O_DISP)
      pExpr = new OperationExpression(
        OperationExpression::OpAdd,
        pExpr,
        new ConstantExpression(0, m_Value));
  }

  else if (m_Type & O_REL)
    pExpr = new OperationExpression(
      OperationExpression::OpAdd,
      new IdentifierExpression(pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister), pCpuInfo),
      new ConstantExpression(0, m_Value));

  if (m_Type & O_MEM)
  {
    Expression *pBaseExpr = nullptr;
    if (m_Type & O_SEG)
      pBaseExpr = new IdentifierExpression(m_Seg, pCpuInfo);
    else if (m_Type & O_SEG_VAL)
      pBaseExpr = new ConstantExpression(0, m_SegValue);

    pExpr = new MemoryExpression(pBaseExpr, pExpr);
  }

  return pExpr;
}

MEDUSA_NAMESPACE_END