/* This file has been automatically generated, you must _NOT_ edit it directly. (Mon Nov 14 21:29:28 2016) */
#include "st62_architecture.hpp"
/* operand ['op0 = decode_A'] */
bool St62Architecture::Operand__A(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_A
  auto spOprd0 = Decode_A(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_V'] */
bool St62Architecture::Operand__V(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_V
  auto spOprd0 = Decode_V(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_W'] */
bool St62Architecture::Operand__W(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_W
  auto spOprd0 = Decode_W(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_X'] */
bool St62Architecture::Operand__X(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_X
  auto spOprd0 = Decode_X(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Y'] */
bool St62Architecture::Operand__Y(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Y
  auto spOprd0 = Decode_Y(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_bitdirect'] */
bool St62Architecture::Operand__bitdirect(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_bitdirect
  auto spOprd0 = Decode_bitdirect(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* decoder ["reg('A')"] */
Expression::SPType St62Architecture::Decode_A(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('A')
  return Expr::MakeId(ST62_Reg_A, &m_CpuInfo);
}

/* decoder ["reg('V')"] */
Expression::SPType St62Architecture::Decode_V(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('V')
  return Expr::MakeId(ST62_Reg_V, &m_CpuInfo);
}

/* decoder ["reg('W')"] */
Expression::SPType St62Architecture::Decode_W(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('W')
  return Expr::MakeId(ST62_Reg_W, &m_CpuInfo);
}

/* decoder ["reg('X')"] */
Expression::SPType St62Architecture::Decode_X(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('X')
  return Expr::MakeId(ST62_Reg_X, &m_CpuInfo);
}

/* decoder ["reg('Y')"] */
Expression::SPType St62Architecture::Decode_Y(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('Y')
  return Expr::MakeId(ST62_Reg_Y, &m_CpuInfo);
}

/* decoder ["call('Decode_bitdirect')"] */
Expression::SPType St62Architecture::Decode_bitdirect(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_bitdirect')
  return __Decode_bitdirect(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_direct')"] */
Expression::SPType St62Architecture::Decode_direct(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_direct')
  return __Decode_direct(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_ee')"] */
Expression::SPType St62Architecture::Decode_ee(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_ee')
  return __Decode_ee(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_ext')"] */
Expression::SPType St62Architecture::Decode_ext(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_ext')
  return __Decode_ext(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_imm')"] */
Expression::SPType St62Architecture::Decode_imm(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_imm')
  return __Decode_imm(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_pcr')"] */
Expression::SPType St62Architecture::Decode_pcr(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_pcr')
  return __Decode_pcr(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["ireg('X')"] */
Expression::SPType St62Architecture::Decode_rX(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: ireg('X')
  return Expr::MakeMem(8, Expr::MakeBitVector(16, 0x1000), Expr::MakeId(ST62_Reg_X, &m_CpuInfo), true);
}

/* decoder ["ireg('Y')"] */
Expression::SPType St62Architecture::Decode_rY(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: ireg('Y')
  return Expr::MakeMem(8, Expr::MakeBitVector(16, 0x1000), Expr::MakeId(ST62_Reg_Y, &m_CpuInfo), true);
}

/* operand ['op0 = decode_direct'] */
bool St62Architecture::Operand__direct(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_direct
  auto spOprd0 = Decode_direct(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ee'] */
bool St62Architecture::Operand__ee(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ee
  auto spOprd0 = Decode_ee(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ext'] */
bool St62Architecture::Operand__ext(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ext
  auto spOprd0 = Decode_ext(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_imm'] */
bool St62Architecture::Operand__imm(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_imm
  auto spOprd0 = Decode_imm(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_pcr'] */
bool St62Architecture::Operand__pcr(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_pcr
  auto spOprd0 = Decode_pcr(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rX'] */
bool St62Architecture::Operand__rX(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rX
  auto spOprd0 = Decode_rX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rY'] */
bool St62Architecture::Operand__rY(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rY
  auto spOprd0 = Decode_rY(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

