/* This file has been automatically generated, you must _NOT_ edit it directly. (Sat Jun 18 16:42:30 2016) */
#include "x86_architecture.hpp"
/* operand ['op0 = decode_3'] */
bool X86Architecture::Operand__3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_3
  auto spOprd0 = Decode_3(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_AH', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__AH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_AH
  auto spOprd0 = Decode_AH(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_AL', 'op1 = decode_DX'] */
bool X86Architecture::Operand__AL_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_AL
  auto spOprd0 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_DX
  auto spOprd1 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_AL', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__AL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_AL
  auto spOprd0 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_AL', 'op1 = decode_Ob'] */
bool X86Architecture::Operand__AL_Ob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_AL
  auto spOprd0 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ob
  auto spOprd1 = Decode_Ob(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_AL', 'op1 = decode_Xb'] */
bool X86Architecture::Operand__AL_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_AL
  auto spOprd0 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Xb
  auto spOprd1 = Decode_Xb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_AL', 'op1 = decode_Yb'] */
bool X86Architecture::Operand__AL_Yb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_AL
  auto spOprd0 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Yb
  auto spOprd1 = Decode_Yb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_AX'] */
bool X86Architecture::Operand__AX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_AX
  auto spOprd0 = Decode_AX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Ap'] */
bool X86Architecture::Operand__Ap(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ap
  auto spOprd0 = Decode_Ap(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_BH', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__BH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_BH
  auto spOprd0 = Decode_BH(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_BL', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__BL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_BL
  auto spOprd0 = Decode_BL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_By', 'op1 = decode_Ey'] */
bool X86Architecture::Operand__By_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_By
  auto spOprd0 = Decode_By(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ey
  auto spOprd1 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_By', 'op1 = decode_Ey', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__By_Ey_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_By
  auto spOprd0 = Decode_By(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ey
  auto spOprd1 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_CH', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__CH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_CH
  auto spOprd0 = Decode_CH(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_CL', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__CL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_CL
  auto spOprd0 = Decode_CL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_CS'] */
bool X86Architecture::Operand__CS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_CS
  auto spOprd0 = Decode_CS(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_DH', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__DH_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_DH
  auto spOprd0 = Decode_DH(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_DL', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__DL_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_DL
  auto spOprd0 = Decode_DL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_DS'] */
bool X86Architecture::Operand__DS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_DS
  auto spOprd0 = Decode_DS(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_DX', 'op1 = decode_AL'] */
bool X86Architecture::Operand__DX_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_DX
  auto spOprd0 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_AL
  auto spOprd1 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_DX', 'op1 = decode_Xb'] */
bool X86Architecture::Operand__DX_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_DX
  auto spOprd0 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Xb
  auto spOprd1 = Decode_Xb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_DX', 'op1 = decode_Xz'] */
bool X86Architecture::Operand__DX_Xz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_DX
  auto spOprd0 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Xz
  auto spOprd1 = Decode_Xz(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_DX', 'op1 = decode_eAX'] */
bool X86Architecture::Operand__DX_eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_DX
  auto spOprd0 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_eAX
  auto spOprd1 = Decode_eAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Dy', 'op1 = decode_Ry'] */
bool X86Architecture::Operand__Dy_Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Dy
  auto spOprd0 = Decode_Dy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ry
  auto spOprd1 = Decode_Ry(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ES'] */
bool X86Architecture::Operand__ES(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ES
  auto spOprd0 = Decode_ES(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Eb'] */
bool X86Architecture::Operand__Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Eb
  auto spOprd0 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Eb', 'op1 = decode_1'] */
bool X86Architecture::Operand__Eb_1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Eb
  auto spOprd0 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_1
  auto spOprd1 = Decode_1(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Eb', 'op1 = decode_CL'] */
bool X86Architecture::Operand__Eb_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Eb
  auto spOprd0 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_CL
  auto spOprd1 = Decode_CL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Eb', 'op1 = decode_Gb'] */
bool X86Architecture::Operand__Eb_Gb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Eb
  auto spOprd0 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gb
  auto spOprd1 = Decode_Gb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Eb', 'move_decode_offset(next_operand)', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__Eb_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Eb
  auto spOprd0 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ed'] */
bool X86Architecture::Operand__Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ed
  auto spOprd0 = Decode_Ed(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Ed', 'op1 = decode_Vo'] */
bool X86Architecture::Operand__Ed_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ed
  auto spOprd0 = Decode_Ed(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ev'] */
bool X86Architecture::Operand__Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Ev', 'op1 = decode_1'] */
bool X86Architecture::Operand__Ev_1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_1
  auto spOprd1 = Decode_1(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ev', 'op1 = decode_CL'] */
bool X86Architecture::Operand__Ev_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_CL
  auto spOprd1 = Decode_CL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ev', 'op1 = decode_Gv'] */
bool X86Architecture::Operand__Ev_Gv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gv
  auto spOprd1 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ev', 'op1 = decode_Gv', 'op2 = decode_CL'] */
bool X86Architecture::Operand__Ev_Gv_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gv
  auto spOprd1 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_CL
  auto spOprd2 = Decode_CL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Ev', 'op1 = decode_Gv', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Ev_Gv_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gv
  auto spOprd1 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Ev', 'move_decode_offset(next_operand)', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__Ev_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Ev', 'move_decode_offset(next_operand)', 'op1 = decode_Ibs'] */
bool X86Architecture::Operand__Ev_Ibs(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand1: op1 = decode_Ibs
  auto spOprd1 = Decode_Ibs(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Ev', 'move_decode_offset(next_operand)', 'op1 = decode_Iz'] */
bool X86Architecture::Operand__Ev_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Ev
  auto spOprd0 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand1: op1 = decode_Iz
  auto spOprd1 = Decode_Iz(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ew', 'op1 = decode_Gw'] */
bool X86Architecture::Operand__Ew_Gw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ew
  auto spOprd0 = Decode_Ew(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gw
  auto spOprd1 = Decode_Gw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ew', 'op1 = decode_Sw'] */
bool X86Architecture::Operand__Ew_Sw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ew
  auto spOprd0 = Decode_Ew(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Sw
  auto spOprd1 = Decode_Sw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ey', 'op1 = decode_Gy'] */
bool X86Architecture::Operand__Ey_Gy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ey
  auto spOprd0 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gy
  auto spOprd1 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ey', 'op1 = decode_Pq'] */
bool X86Architecture::Operand__Ey_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ey
  auto spOprd0 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Pq
  auto spOprd1 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ey', 'op1 = decode_Vo'] */
bool X86Architecture::Operand__Ey_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ey
  auto spOprd0 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ey', 'op1 = decode_Vo', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Ey_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ey
  auto spOprd0 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Eyb', 'op1 = decode_Vo', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Eyb_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Eyb
  auto spOprd0 = Decode_Eyb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Eyw', 'op1 = decode_Vo', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Eyw_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Eyw
  auto spOprd0 = Decode_Eyw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_FS'] */
bool X86Architecture::Operand__FS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_FS
  auto spOprd0 = Decode_FS(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_GS'] */
bool X86Architecture::Operand__GS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_GS
  auto spOprd0 = Decode_GS(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Gb', 'op1 = decode_Eb'] */
bool X86Architecture::Operand__Gb_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gb
  auto spOprd0 = Decode_Gb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Eb
  auto spOprd1 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gb', 'op1 = decode_Ev'] */
bool X86Architecture::Operand__Gb_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gb
  auto spOprd0 = Decode_Gb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ev
  auto spOprd1 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_Eb'] */
bool X86Architecture::Operand__Gv_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Eb
  auto spOprd1 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_Ed'] */
bool X86Architecture::Operand__Gv_Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ed
  auto spOprd1 = Decode_Ed(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_Ev'] */
bool X86Architecture::Operand__Gv_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ev
  auto spOprd1 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Gv', 'op1 = decode_Ev', 'move_decode_offset(next_operand)', 'op2 = decode_Ibs'] */
bool X86Architecture::Operand__Gv_Ev_Ibs(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ev
  auto spOprd1 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ibs
  auto spOprd2 = Decode_Ibs(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Gv', 'op1 = decode_Ev', 'move_decode_offset(next_operand)', 'op2 = decode_Iz'] */
bool X86Architecture::Operand__Gv_Ev_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ev
  auto spOprd1 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Iz
  auto spOprd2 = Decode_Iz(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_Ew'] */
bool X86Architecture::Operand__Gv_Ew(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ew
  auto spOprd1 = Decode_Ew(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_M'] */
bool X86Architecture::Operand__Gv_M(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_M
  auto spOprd1 = Decode_M(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_Ma'] */
bool X86Architecture::Operand__Gv_Ma(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ma
  auto spOprd1 = Decode_Ma(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_Mp'] */
bool X86Architecture::Operand__Gv_Mp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mp
  auto spOprd1 = Decode_Mp(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gv', 'op1 = decode_Mv'] */
bool X86Architecture::Operand__Gv_Mv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gv
  auto spOprd0 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mv
  auto spOprd1 = Decode_Mv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_By', 'op2 = decode_Ey'] */
bool X86Architecture::Operand__Gy_By_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_By
  auto spOprd1 = Decode_By(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ey
  auto spOprd2 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Eb'] */
bool X86Architecture::Operand__Gy_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Eb
  auto spOprd1 = Decode_Eb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Ev'] */
bool X86Architecture::Operand__Gy_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ev
  auto spOprd1 = Decode_Ev(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Ey'] */
bool X86Architecture::Operand__Gy_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ey
  auto spOprd1 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Ey', 'op2 = decode_By'] */
bool X86Architecture::Operand__Gy_Ey_By(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ey
  auto spOprd1 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_By
  auto spOprd2 = Decode_By(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Nq'] */
bool X86Architecture::Operand__Gy_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Nq
  auto spOprd1 = Decode_Nq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Nq', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Gy_Nq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Nq
  auto spOprd1 = Decode_Nq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Uo'] */
bool X86Architecture::Operand__Gy_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uo
  auto spOprd1 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Uo', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Gy_Uo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uo
  auto spOprd1 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Ux'] */
bool X86Architecture::Operand__Gy_Ux(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ux
  auto spOprd1 = Decode_Ux(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Wod'] */
bool X86Architecture::Operand__Gy_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wod
  auto spOprd1 = Decode_Wod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Gy', 'op1 = decode_Woq'] */
bool X86Architecture::Operand__Gy_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Gy
  auto spOprd0 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Woq
  auto spOprd1 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Hx', 'op1 = decode_Hx', 'op2 = decode_Wx'] */
bool X86Architecture::Operand__Hx_Hx_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Hx
  auto spOprd0 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hx
  auto spOprd1 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wx
  auto spOprd2 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Hx', 'op1 = decode_Ux', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Hx_Ux_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Hx
  auto spOprd0 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ux
  auto spOprd1 = Decode_Ux(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Ib'] */
bool X86Architecture::Operand__Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ib
  auto spOprd0 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Ib', 'op1 = decode_AL'] */
bool X86Architecture::Operand__Ib_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ib
  auto spOprd0 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_AL
  auto spOprd1 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ib', 'op1 = decode_eAX'] */
bool X86Architecture::Operand__Ib_eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ib
  auto spOprd0 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_eAX
  auto spOprd1 = Decode_eAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Iw'] */
bool X86Architecture::Operand__Iw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Iw
  auto spOprd0 = Decode_Iw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Iw', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__Iw_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Iw
  auto spOprd0 = Decode_Iw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Iz'] */
bool X86Architecture::Operand__Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Iz
  auto spOprd0 = Decode_Iz(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Jb'] */
bool X86Architecture::Operand__Jb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Jb
  auto spOprd0 = Decode_Jb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Jz'] */
bool X86Architecture::Operand__Jz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Jz
  auto spOprd0 = Decode_Jz(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_M'] */
bool X86Architecture::Operand__M(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_M
  auto spOprd0 = Decode_M(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Md'] */
bool X86Architecture::Operand__Md(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Md
  auto spOprd0 = Decode_Md(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Md', 'op1 = decode_Vo'] */
bool X86Architecture::Operand__Md_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Md
  auto spOprd0 = Decode_Md(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Mp'] */
bool X86Architecture::Operand__Mp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mp
  auto spOprd0 = Decode_Mp(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Mq'] */
bool X86Architecture::Operand__Mq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mq
  auto spOprd0 = Decode_Mq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Mq', 'op1 = decode_Pq'] */
bool X86Architecture::Operand__Mq_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mq
  auto spOprd0 = Decode_Mq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Pq
  auto spOprd1 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Mq', 'op1 = decode_Vo'] */
bool X86Architecture::Operand__Mq_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mq
  auto spOprd0 = Decode_Mq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Mv', 'op1 = decode_Gv'] */
bool X86Architecture::Operand__Mv_Gv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mv
  auto spOprd0 = Decode_Mv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gv
  auto spOprd1 = Decode_Gv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Mw'] */
bool X86Architecture::Operand__Mw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mw
  auto spOprd0 = Decode_Mw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Mx', 'op1 = decode_Hx', 'op2 = decode_Vx'] */
bool X86Architecture::Operand__Mx_Hx_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mx
  auto spOprd0 = Decode_Mx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hx
  auto spOprd1 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Vx
  auto spOprd2 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Mx', 'op1 = decode_Vx'] */
bool X86Architecture::Operand__Mx_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Mx
  auto spOprd0 = Decode_Mx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vx
  auto spOprd1 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_My', 'op1 = decode_Gy'] */
bool X86Architecture::Operand__My_Gy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_My
  auto spOprd0 = Decode_My(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Gy
  auto spOprd1 = Decode_Gy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Nq', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__Nq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Nq
  auto spOprd0 = Decode_Nq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ob', 'op1 = decode_AL'] */
bool X86Architecture::Operand__Ob_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ob
  auto spOprd0 = Decode_Ob(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_AL
  auto spOprd1 = Decode_AL(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ov', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__Ov_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ov
  auto spOprd0 = Decode_Ov(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Ey'] */
bool X86Architecture::Operand__Pq_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ey
  auto spOprd1 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Mw', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Pq_Mw_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mw
  auto spOprd1 = Decode_Mw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Nq'] */
bool X86Architecture::Operand__Pq_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Nq
  auto spOprd1 = Decode_Nq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Qd'] */
bool X86Architecture::Operand__Pq_Qd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Qd
  auto spOprd1 = Decode_Qd(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Qq'] */
bool X86Architecture::Operand__Pq_Qq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Qq
  auto spOprd1 = Decode_Qq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Qq', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Pq_Qq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Qq
  auto spOprd1 = Decode_Qq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Uq'] */
bool X86Architecture::Operand__Pq_Uq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uq
  auto spOprd1 = Decode_Uq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Wo'] */
bool X86Architecture::Operand__Pq_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wo
  auto spOprd1 = Decode_Wo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Pq', 'op1 = decode_Woq'] */
bool X86Architecture::Operand__Pq_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Pq
  auto spOprd0 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Woq
  auto spOprd1 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Qq', 'op1 = decode_Pq'] */
bool X86Architecture::Operand__Qq_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Qq
  auto spOprd0 = Decode_Qq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Pq
  auto spOprd1 = Decode_Pq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Rv'] */
bool X86Architecture::Operand__Rv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Rv
  auto spOprd0 = Decode_Rv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Ry'] */
bool X86Architecture::Operand__Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ry
  auto spOprd0 = Decode_Ry(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Ry', 'op1 = decode_Cy'] */
bool X86Architecture::Operand__Ry_Cy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ry
  auto spOprd0 = Decode_Ry(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Cy
  auto spOprd1 = Decode_Cy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ry', 'op1 = decode_Dy'] */
bool X86Architecture::Operand__Ry_Dy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ry
  auto spOprd0 = Decode_Ry(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Dy
  auto spOprd1 = Decode_Dy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ry', 'op1 = decode_Ty'] */
bool X86Architecture::Operand__Ry_Ty(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ry
  auto spOprd0 = Decode_Ry(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ty
  auto spOprd1 = Decode_Ty(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_SS'] */
bool X86Architecture::Operand__SS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_SS
  auto spOprd0 = Decode_SS(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST0'] */
bool X86Architecture::Operand__ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST0_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST1'] */
bool X86Architecture::Operand__ST0_ST1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST1
  auto spOprd1 = Decode_ST1(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST2'] */
bool X86Architecture::Operand__ST0_ST2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST2
  auto spOprd1 = Decode_ST2(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST3'] */
bool X86Architecture::Operand__ST0_ST3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST3
  auto spOprd1 = Decode_ST3(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST4'] */
bool X86Architecture::Operand__ST0_ST4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST4
  auto spOprd1 = Decode_ST4(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST5'] */
bool X86Architecture::Operand__ST0_ST5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST5
  auto spOprd1 = Decode_ST5(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST6'] */
bool X86Architecture::Operand__ST0_ST6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST6
  auto spOprd1 = Decode_ST6(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST0', 'op1 = decode_ST7'] */
bool X86Architecture::Operand__ST0_ST7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST0
  auto spOprd0 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST7
  auto spOprd1 = Decode_ST7(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST1'] */
bool X86Architecture::Operand__ST1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST1
  auto spOprd0 = Decode_ST1(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST1', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST1_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST1
  auto spOprd0 = Decode_ST1(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST2'] */
bool X86Architecture::Operand__ST2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST2
  auto spOprd0 = Decode_ST2(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST2', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST2_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST2
  auto spOprd0 = Decode_ST2(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST3'] */
bool X86Architecture::Operand__ST3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST3
  auto spOprd0 = Decode_ST3(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST3', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST3_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST3
  auto spOprd0 = Decode_ST3(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST4'] */
bool X86Architecture::Operand__ST4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST4
  auto spOprd0 = Decode_ST4(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST4', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST4_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST4
  auto spOprd0 = Decode_ST4(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST5'] */
bool X86Architecture::Operand__ST5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST5
  auto spOprd0 = Decode_ST5(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST5', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST5_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST5
  auto spOprd0 = Decode_ST5(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST6'] */
bool X86Architecture::Operand__ST6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST6
  auto spOprd0 = Decode_ST6(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST6', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST6_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST6
  auto spOprd0 = Decode_ST6(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_ST7'] */
bool X86Architecture::Operand__ST7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST7
  auto spOprd0 = Decode_ST7(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ST7', 'op1 = decode_ST0'] */
bool X86Architecture::Operand__ST7_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ST7
  auto spOprd0 = Decode_ST7(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_ST0
  auto spOprd1 = Decode_ST0(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Sw', 'op1 = decode_Ew'] */
bool X86Architecture::Operand__Sw_Ew(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Sw
  auto spOprd0 = Decode_Sw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ew
  auto spOprd1 = Decode_Ew(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Ty', 'op1 = decode_Ry'] */
bool X86Architecture::Operand__Ty_Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Ty
  auto spOprd0 = Decode_Ty(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ry
  auto spOprd1 = Decode_Ry(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Uo'] */
bool X86Architecture::Operand__Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Uo
  auto spOprd0 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Uo', 'op1 = decode_Ib', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Uo_Ib_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Uo
  auto spOprd0 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Uo', 'op1 = decode_Vod'] */
bool X86Architecture::Operand__Uo_Vod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Uo
  auto spOprd0 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vod
  auto spOprd1 = Decode_Vod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Uo', 'op1 = decode_Voq'] */
bool X86Architecture::Operand__Uo_Voq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Uo
  auto spOprd0 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Voq
  auto spOprd1 = Decode_Voq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Uoq', 'op1 = decode_Vo'] */
bool X86Architecture::Operand__Uoq_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Uoq
  auto spOprd0 = Decode_Uoq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Ux', 'move_decode_offset(next_operand)', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__Ux_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Ux
  auto spOprd0 = Decode_Ux(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ed'] */
bool X86Architecture::Operand__Vo_Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ed
  auto spOprd1 = Decode_Ed(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Edb', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Edb_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Edb
  auto spOprd1 = Decode_Edb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ey'] */
bool X86Architecture::Operand__Vo_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ey
  auto spOprd1 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ey', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Ey_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ey
  auto spOprd1 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Edb', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Ho_Edb_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Edb
  auto spOprd2 = Decode_Edb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Ey'] */
bool X86Architecture::Operand__Vo_Ho_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ey
  auto spOprd2 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Ey', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Ho_Ey_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ey
  auto spOprd2 = Decode_Ey(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Mw', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Ho_Mw_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Mw
  auto spOprd2 = Decode_Mw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Uod', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Ho_Uod_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Uod
  auto spOprd2 = Decode_Uod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Uoq'] */
bool X86Architecture::Operand__Vo_Ho_Uoq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Uoq
  auto spOprd2 = Decode_Uoq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Wd'] */
bool X86Architecture::Operand__Vo_Ho_Wd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wd
  auto spOprd2 = Decode_Wd(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Wo'] */
bool X86Architecture::Operand__Vo_Ho_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wo
  auto spOprd2 = Decode_Wo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Wo', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Ho_Wo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wo
  auto spOprd2 = Decode_Wo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Wod'] */
bool X86Architecture::Operand__Vo_Ho_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wod
  auto spOprd2 = Decode_Wod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Woq'] */
bool X86Architecture::Operand__Vo_Ho_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Woq
  auto spOprd2 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Woq', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Ho_Woq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Woq
  auto spOprd2 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Ho', 'op2 = decode_Wq'] */
bool X86Architecture::Operand__Vo_Ho_Wq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ho
  auto spOprd1 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wq
  auto spOprd2 = Decode_Wq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Md', 'op2 = decode_x', 'op3 = decode_Ho'] */
bool X86Architecture::Operand__Vo_Md_x_Ho(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Md
  auto spOprd1 = Decode_Md(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_x
  auto spOprd2 = Decode_x(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Ho
  auto spOprd3 = Decode_Ho(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Mq'] */
bool X86Architecture::Operand__Vo_Mq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mq
  auto spOprd1 = Decode_Mq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Mw', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Mw_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mw
  auto spOprd1 = Decode_Mw(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Nq'] */
bool X86Architecture::Operand__Vo_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Nq
  auto spOprd1 = Decode_Nq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Uo'] */
bool X86Architecture::Operand__Vo_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uo
  auto spOprd1 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Uod'] */
bool X86Architecture::Operand__Vo_Uod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uod
  auto spOprd1 = Decode_Uod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Uod', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Uod_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uod
  auto spOprd1 = Decode_Uod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Uoq'] */
bool X86Architecture::Operand__Vo_Uoq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uoq
  auto spOprd1 = Decode_Uoq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Wo'] */
bool X86Architecture::Operand__Vo_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wo
  auto spOprd1 = Decode_Wo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Wo', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Wo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wo
  auto spOprd1 = Decode_Wo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Wod'] */
bool X86Architecture::Operand__Vo_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wod
  auto spOprd1 = Decode_Wod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Woq'] */
bool X86Architecture::Operand__Vo_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Woq
  auto spOprd1 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Woq', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vo_Woq_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Woq
  auto spOprd1 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Wow'] */
bool X86Architecture::Operand__Vo_Wow(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wow
  auto spOprd1 = Decode_Wow(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vo', 'op1 = decode_Wx'] */
bool X86Architecture::Operand__Vo_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vo
  auto spOprd0 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wx
  auto spOprd1 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Hx', 'op2 = decode_Mx'] */
bool X86Architecture::Operand__Vx_Hx_Mx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hx
  auto spOprd1 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Mx
  auto spOprd2 = Decode_Mx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Hx', 'op2 = decode_Wx'] */
bool X86Architecture::Operand__Vx_Hx_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hx
  auto spOprd1 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wx
  auto spOprd2 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Vx', 'op1 = decode_Hx', 'op2 = decode_Wx', 'move_decode_offset(next_operand)', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vx_Hx_Wx_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hx
  auto spOprd1 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wx
  auto spOprd2 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Hx', 'op2 = decode_Wx', 'op3 = decode_Lx'] */
bool X86Architecture::Operand__Vx_Hx_Wx_Lx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hx
  auto spOprd1 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wx
  auto spOprd2 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Lx
  auto spOprd3 = Decode_Lx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Md', 'op2 = decode_x', 'op3 = decode_Hx'] */
bool X86Architecture::Operand__Vx_Md_x_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Md
  auto spOprd1 = Decode_Md(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_x
  auto spOprd2 = Decode_x(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Hx
  auto spOprd3 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Mq', 'op2 = decode_x', 'op3 = decode_Hx'] */
bool X86Architecture::Operand__Vx_Mq_x_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mq
  auto spOprd1 = Decode_Mq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_x
  auto spOprd2 = Decode_x(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: op3 = decode_Hx
  auto spOprd3 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Mqo', 'op2 = decode_Hx'] */
bool X86Architecture::Operand__Vx_Mqo_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mqo
  auto spOprd1 = Decode_Mqo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Hx
  auto spOprd2 = Decode_Hx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Mx'] */
bool X86Architecture::Operand__Vx_Mx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Mx
  auto spOprd1 = Decode_Mx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Uo'] */
bool X86Architecture::Operand__Vx_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uo
  auto spOprd1 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Uo', 'op2 = decode_b'] */
bool X86Architecture::Operand__Vx_Uo_b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uo
  auto spOprd1 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_b
  auto spOprd2 = Decode_b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Uo', 'op2 = decode_w'] */
bool X86Architecture::Operand__Vx_Uo_w(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uo
  auto spOprd1 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_w
  auto spOprd2 = Decode_w(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Uod'] */
bool X86Architecture::Operand__Vx_Uod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uod
  auto spOprd1 = Decode_Uod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Uoq'] */
bool X86Architecture::Operand__Vx_Uoq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uoq
  auto spOprd1 = Decode_Uoq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Woq'] */
bool X86Architecture::Operand__Vx_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Woq
  auto spOprd1 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vx', 'op1 = decode_Wx'] */
bool X86Architecture::Operand__Vx_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wx
  auto spOprd1 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Vx', 'op1 = decode_Wx', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vx_Wx_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Vx
  auto spOprd0 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wx
  auto spOprd1 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Vy', 'op1 = decode_Hy', 'op2 = decode_Wo', 'move_decode_offset(next_operand)', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vy_Hy_Wo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hy
  auto spOprd1 = Decode_Hy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wo
  auto spOprd2 = Decode_Wo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vy', 'op1 = decode_Hy', 'op2 = decode_Wy'] */
bool X86Architecture::Operand__Vy_Hy_Wy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hy
  auto spOprd1 = Decode_Hy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wy
  auto spOprd2 = Decode_Wy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Vy', 'op1 = decode_Hy', 'op2 = decode_Wy', 'move_decode_offset(next_operand)', 'op3 = decode_Ib'] */
bool X86Architecture::Operand__Vy_Hy_Wy_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Hy
  auto spOprd1 = Decode_Hy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: op2 = decode_Wy
  auto spOprd2 = Decode_Wy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  // operand3: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand3: op3 = decode_Ib
  auto spOprd3 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd3 == nullptr)
    return false;
  rInsn.AddOperand(spOprd3);

  return true;
}

/* operand ['op0 = decode_Vy', 'op1 = decode_Uo'] */
bool X86Architecture::Operand__Vy_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uo
  auto spOprd1 = Decode_Uo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vy', 'op1 = decode_Uoq'] */
bool X86Architecture::Operand__Vy_Uoq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Uoq
  auto spOprd1 = Decode_Uoq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vy', 'op1 = decode_Wod'] */
bool X86Architecture::Operand__Vy_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wod
  auto spOprd1 = Decode_Wod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vy', 'op1 = decode_Woo'] */
bool X86Architecture::Operand__Vy_Woo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Woo
  auto spOprd1 = Decode_Woo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Vy', 'op1 = decode_Woq'] */
bool X86Architecture::Operand__Vy_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Woq
  auto spOprd1 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Vy', 'op1 = decode_Wy', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Vy_Wy_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Vy
  auto spOprd0 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Wy
  auto spOprd1 = Decode_Wy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Wo', 'op1 = decode_Vy', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Wo_Vy_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Wo
  auto spOprd0 = Decode_Wo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vy
  auto spOprd1 = Decode_Vy(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Wob', 'op1 = decode_Vo', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Wob_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Wob
  auto spOprd0 = Decode_Wob(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Wod', 'op1 = decode_Vo'] */
bool X86Architecture::Operand__Wod_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Wod
  auto spOprd0 = Decode_Wod(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Woq', 'op1 = decode_Vo'] */
bool X86Architecture::Operand__Woq_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Woq
  auto spOprd0 = Decode_Woq(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['last_len', 'op0 = decode_Woqo', 'op1 = decode_Vx', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Woqo_Vx_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Woqo
  auto spOprd0 = Decode_Woqo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vx
  auto spOprd1 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['last_len', 'op0 = decode_Wow', 'op1 = decode_Vo', 'move_decode_offset(next_operand)', 'op2 = decode_Ib'] */
bool X86Architecture::Operand__Wow_Vo_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: last_len
  auto LastLen = rInsn.GetLength();
  // operand0: op0 = decode_Wow
  auto spOprd0 = Decode_Wow(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vo
  auto spOprd1 = Decode_Vo(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  // operand2: move_decode_offset(next_operand)
  Offset += (rInsn.GetLength() - LastLen);
  // operand2: op2 = decode_Ib
  auto spOprd2 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd2 == nullptr)
    return false;
  rInsn.AddOperand(spOprd2);

  return true;
}

/* operand ['op0 = decode_Wx', 'op1 = decode_Vx'] */
bool X86Architecture::Operand__Wx_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Wx
  auto spOprd0 = Decode_Wx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Vx
  auto spOprd1 = Decode_Vx(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Xb', 'op1 = decode_Yb'] */
bool X86Architecture::Operand__Xb_Yb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Xb
  auto spOprd0 = Decode_Xb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Yb
  auto spOprd1 = Decode_Yb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Xv', 'op1 = decode_Yv'] */
bool X86Architecture::Operand__Xv_Yv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Xv
  auto spOprd0 = Decode_Xv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Yv
  auto spOprd1 = Decode_Yv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Yb', 'op1 = decode_DX'] */
bool X86Architecture::Operand__Yb_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Yb
  auto spOprd0 = Decode_Yb(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_DX
  auto spOprd1 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_Yz', 'op1 = decode_DX'] */
bool X86Architecture::Operand__Yz_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Yz
  auto spOprd0 = Decode_Yz(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_DX
  auto spOprd1 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_d64_r10'] */
bool X86Architecture::Operand__d64_r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r10
  auto spOprd0 = Decode_d64_r10(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_r11'] */
bool X86Architecture::Operand__d64_r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r11
  auto spOprd0 = Decode_d64_r11(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_r12'] */
bool X86Architecture::Operand__d64_r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r12
  auto spOprd0 = Decode_d64_r12(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_r13'] */
bool X86Architecture::Operand__d64_r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r13
  auto spOprd0 = Decode_d64_r13(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_r14'] */
bool X86Architecture::Operand__d64_r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r14
  auto spOprd0 = Decode_d64_r14(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_r15'] */
bool X86Architecture::Operand__d64_r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r15
  auto spOprd0 = Decode_d64_r15(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_r8'] */
bool X86Architecture::Operand__d64_r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r8
  auto spOprd0 = Decode_d64_r8(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_r9'] */
bool X86Architecture::Operand__d64_r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_r9
  auto spOprd0 = Decode_d64_r9(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rAX'] */
bool X86Architecture::Operand__d64_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rAX
  auto spOprd0 = Decode_d64_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rBP'] */
bool X86Architecture::Operand__d64_rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rBP
  auto spOprd0 = Decode_d64_rBP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rBX'] */
bool X86Architecture::Operand__d64_rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rBX
  auto spOprd0 = Decode_d64_rBX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rCX'] */
bool X86Architecture::Operand__d64_rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rCX
  auto spOprd0 = Decode_d64_rCX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rDI'] */
bool X86Architecture::Operand__d64_rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rDI
  auto spOprd0 = Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rDX'] */
bool X86Architecture::Operand__d64_rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rDX
  auto spOprd0 = Decode_d64_rDX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rSI'] */
bool X86Architecture::Operand__d64_rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rSI
  auto spOprd0 = Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_d64_rSP'] */
bool X86Architecture::Operand__d64_rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_d64_rSP
  auto spOprd0 = Decode_d64_rSP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* decoder ['const(8, 1)'] */
Expression::SPType X86Architecture::Decode_1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: const(8, 1)
  return Expr::MakeBitVector(8, 1);
}

/* decoder ['const(8, 3)'] */
Expression::SPType X86Architecture::Decode_3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: const(8, 3)
  return Expr::MakeBitVector(8, 3);
}

/* decoder ["reg('ah')"] */
Expression::SPType X86Architecture::Decode_AH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('ah')
  return Expr::MakeId(X86_Reg_Ah, &m_CpuInfo);
}

/* decoder ["reg('al')"] */
Expression::SPType X86Architecture::Decode_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('al')
  return Expr::MakeId(X86_Reg_Al, &m_CpuInfo);
}

/* decoder ["reg('ax')"] */
Expression::SPType X86Architecture::Decode_AX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('ax')
  return Expr::MakeId(X86_Reg_Ax, &m_CpuInfo);
}

/* decoder ["call('Decode_Ap')"] */
Expression::SPType X86Architecture::Decode_Ap(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ap')
  return __Decode_Ap(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg('bh')"] */
Expression::SPType X86Architecture::Decode_BH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('bh')
  return Expr::MakeId(X86_Reg_Bh, &m_CpuInfo);
}

/* decoder ["reg('bl')"] */
Expression::SPType X86Architecture::Decode_BL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('bl')
  return Expr::MakeId(X86_Reg_Bl, &m_CpuInfo);
}

/* decoder ["call('Decode_By')"] */
Expression::SPType X86Architecture::Decode_By(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_By')
  return __Decode_By(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg('ch')"] */
Expression::SPType X86Architecture::Decode_CH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('ch')
  return Expr::MakeId(X86_Reg_Ch, &m_CpuInfo);
}

/* decoder ["reg('cl')"] */
Expression::SPType X86Architecture::Decode_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('cl')
  return Expr::MakeId(X86_Reg_Cl, &m_CpuInfo);
}

/* decoder ["reg('cs')"] */
Expression::SPType X86Architecture::Decode_CS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('cs')
  return Expr::MakeId(X86_Reg_Cs, &m_CpuInfo);
}

/* decoder ["call('Decode_Cy')"] */
Expression::SPType X86Architecture::Decode_Cy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Cy')
  return __Decode_Cy(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg('dh')"] */
Expression::SPType X86Architecture::Decode_DH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('dh')
  return Expr::MakeId(X86_Reg_Dh, &m_CpuInfo);
}

/* decoder ["reg('dl')"] */
Expression::SPType X86Architecture::Decode_DL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('dl')
  return Expr::MakeId(X86_Reg_Dl, &m_CpuInfo);
}

/* decoder ["reg('ds')"] */
Expression::SPType X86Architecture::Decode_DS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('ds')
  return Expr::MakeId(X86_Reg_Ds, &m_CpuInfo);
}

/* decoder ["reg('dx')"] */
Expression::SPType X86Architecture::Decode_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('dx')
  return Expr::MakeId(X86_Reg_Dx, &m_CpuInfo);
}

/* decoder ["call('Decode_Dy')"] */
Expression::SPType X86Architecture::Decode_Dy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Dy')
  return __Decode_Dy(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg('es')"] */
Expression::SPType X86Architecture::Decode_ES(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('es')
  return Expr::MakeId(X86_Reg_Es, &m_CpuInfo);
}

/* decoder ["call('Decode_Eb')"] */
Expression::SPType X86Architecture::Decode_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Eb')
  return __Decode_Eb(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ed')"] */
Expression::SPType X86Architecture::Decode_Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ed')
  return __Decode_Ed(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Edb')"] */
Expression::SPType X86Architecture::Decode_Edb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Edb')
  return __Decode_Edb(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ev')"] */
Expression::SPType X86Architecture::Decode_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ev')
  return __Decode_Ev(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ew')"] */
Expression::SPType X86Architecture::Decode_Ew(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ew')
  return __Decode_Ew(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ey')"] */
Expression::SPType X86Architecture::Decode_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ey')
  return __Decode_Ey(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Eyb')"] */
Expression::SPType X86Architecture::Decode_Eyb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Eyb')
  return __Decode_Eyb(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Eyw')"] */
Expression::SPType X86Architecture::Decode_Eyw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Eyw')
  return __Decode_Eyw(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg('fs')"] */
Expression::SPType X86Architecture::Decode_FS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('fs')
  return Expr::MakeId(X86_Reg_Fs, &m_CpuInfo);
}

/* decoder ["reg('gs')"] */
Expression::SPType X86Architecture::Decode_GS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('gs')
  return Expr::MakeId(X86_Reg_Gs, &m_CpuInfo);
}

/* decoder ["call('Decode_Gb')"] */
Expression::SPType X86Architecture::Decode_Gb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Gb')
  return __Decode_Gb(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Gv')"] */
Expression::SPType X86Architecture::Decode_Gv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Gv')
  return __Decode_Gv(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Gw')"] */
Expression::SPType X86Architecture::Decode_Gw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Gw')
  return __Decode_Gw(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Gy')"] */
Expression::SPType X86Architecture::Decode_Gy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Gy')
  return __Decode_Gy(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ho')"] */
Expression::SPType X86Architecture::Decode_Ho(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ho')
  return __Decode_Ho(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Hx')"] */
Expression::SPType X86Architecture::Decode_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Hx')
  return __Decode_Hx(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Hy')"] */
Expression::SPType X86Architecture::Decode_Hy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Hy')
  return __Decode_Hy(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ['read_b(off)'] */
Expression::SPType X86Architecture::Decode_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: read_b(off)
  u8 Value;
  if (!rBinStrm.Read(Offset, Value))
    return nullptr;
  Offset += sizeof(Value);
  rInsn.Length() += sizeof(Value);
  return Expr::MakeBitVector(8, Value);

}

/* decoder ["call('Decode_Ibs')"] */
Expression::SPType X86Architecture::Decode_Ibs(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ibs')
  return __Decode_Ibs(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ['read_bsq(off)'] */
Expression::SPType X86Architecture::Decode_Ibsq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: read_bsq(off)
  u8 Value;
  if (!rBinStrm.Read(Offset, Value))
    return nullptr;
  Offset += sizeof(Value);
  rInsn.Length() += sizeof(Value);
  return Expr::MakeBitVector(8, Value);

}

/* decoder ['read_v(off)'] */
Expression::SPType X86Architecture::Decode_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: read_v(off)
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(32, Value);
    }
    else
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(16, Value);
    }
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
    {
      u64 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(64, Value);
    }
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(16, Value);
    }
    else
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(32, Value);
    }
  default:
    return nullptr;
  }

}

/* decoder ['read_w(off)'] */
Expression::SPType X86Architecture::Decode_Iw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: read_w(off)
  u16 Value;
  if (!rBinStrm.Read(Offset, Value))
    return nullptr;
  Offset += sizeof(Value);
  rInsn.Length() += sizeof(Value);
  return Expr::MakeBitVector(16, Value);

}

/* decoder ['read_z(off)'] */
Expression::SPType X86Architecture::Decode_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: read_z(off)
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(32, Value);
    }
    else
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(16, Value);
    }
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(64, SignExtend<s64, 32>(Value));
    }
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(16, Value);
    }
    else
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(32, Value);
    }
  default:
    return nullptr;
  }

}

/* decoder ['read_zsq(off)'] */
Expression::SPType X86Architecture::Decode_Izsq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: read_zsq(off)
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(32, Value);
    }
    else
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(16, Value);
    }
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(64, SignExtend<s64, 32>(Value));
    }
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(16, Value);
    }
    else
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      Offset += sizeof(Value);
      rInsn.Length() += sizeof(Value);
      return Expr::MakeBitVector(32, Value);
    }
  default:
    return nullptr;
  }

}

/* decoder ["call('Decode_Jb')"] */
Expression::SPType X86Architecture::Decode_Jb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Jb')
  return __Decode_Jb(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Jz')"] */
Expression::SPType X86Architecture::Decode_Jz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Jz')
  return __Decode_Jz(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Lx')"] */
Expression::SPType X86Architecture::Decode_Lx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Lx')
  return __Decode_Lx(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_M')"] */
Expression::SPType X86Architecture::Decode_M(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_M')
  return __Decode_M(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ma')"] */
Expression::SPType X86Architecture::Decode_Ma(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ma')
  return __Decode_Ma(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Md')"] */
Expression::SPType X86Architecture::Decode_Md(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Md')
  return __Decode_Md(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Mp')"] */
Expression::SPType X86Architecture::Decode_Mp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Mp')
  return __Decode_Mp(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Mq')"] */
Expression::SPType X86Architecture::Decode_Mq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Mq')
  return __Decode_Mq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Mqo')"] */
Expression::SPType X86Architecture::Decode_Mqo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Mqo')
  return __Decode_Mqo(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Mv')"] */
Expression::SPType X86Architecture::Decode_Mv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Mv')
  return __Decode_Mv(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Mw')"] */
Expression::SPType X86Architecture::Decode_Mw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Mw')
  return __Decode_Mw(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Mx')"] */
Expression::SPType X86Architecture::Decode_Mx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Mx')
  return __Decode_Mx(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_My')"] */
Expression::SPType X86Architecture::Decode_My(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_My')
  return __Decode_My(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Nq')"] */
Expression::SPType X86Architecture::Decode_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Nq')
  return __Decode_Nq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ob')"] */
Expression::SPType X86Architecture::Decode_Ob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ob')
  return __Decode_Ob(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ov')"] */
Expression::SPType X86Architecture::Decode_Ov(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ov')
  return __Decode_Ov(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Pq')"] */
Expression::SPType X86Architecture::Decode_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Pq')
  return __Decode_Pq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Qd')"] */
Expression::SPType X86Architecture::Decode_Qd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Qd')
  return __Decode_Qd(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Qq')"] */
Expression::SPType X86Architecture::Decode_Qq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Qq')
  return __Decode_Qq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Rv')"] */
Expression::SPType X86Architecture::Decode_Rv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Rv')
  return __Decode_Rv(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ry')"] */
Expression::SPType X86Architecture::Decode_Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ry')
  return __Decode_Ry(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg('ss')"] */
Expression::SPType X86Architecture::Decode_SS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('ss')
  return Expr::MakeId(X86_Reg_Ss, &m_CpuInfo);
}

/* decoder ["reg('st0')"] */
Expression::SPType X86Architecture::Decode_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st0')
  return Expr::MakeId(X86_Reg_St0, &m_CpuInfo);
}

/* decoder ["reg('st1')"] */
Expression::SPType X86Architecture::Decode_ST1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st1')
  return Expr::MakeId(X86_Reg_St1, &m_CpuInfo);
}

/* decoder ["reg('st2')"] */
Expression::SPType X86Architecture::Decode_ST2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st2')
  return Expr::MakeId(X86_Reg_St2, &m_CpuInfo);
}

/* decoder ["reg('st3')"] */
Expression::SPType X86Architecture::Decode_ST3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st3')
  return Expr::MakeId(X86_Reg_St3, &m_CpuInfo);
}

/* decoder ["reg('st4')"] */
Expression::SPType X86Architecture::Decode_ST4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st4')
  return Expr::MakeId(X86_Reg_St4, &m_CpuInfo);
}

/* decoder ["reg('st5')"] */
Expression::SPType X86Architecture::Decode_ST5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st5')
  return Expr::MakeId(X86_Reg_St5, &m_CpuInfo);
}

/* decoder ["reg('st6')"] */
Expression::SPType X86Architecture::Decode_ST6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st6')
  return Expr::MakeId(X86_Reg_St6, &m_CpuInfo);
}

/* decoder ["reg('st7')"] */
Expression::SPType X86Architecture::Decode_ST7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('st7')
  return Expr::MakeId(X86_Reg_St7, &m_CpuInfo);
}

/* decoder ["call('Decode_Sw')"] */
Expression::SPType X86Architecture::Decode_Sw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Sw')
  return __Decode_Sw(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ty')"] */
Expression::SPType X86Architecture::Decode_Ty(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ty')
  return __Decode_Ty(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Uo')"] */
Expression::SPType X86Architecture::Decode_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Uo')
  return __Decode_Uo(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Uod')"] */
Expression::SPType X86Architecture::Decode_Uod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Uod')
  return __Decode_Uod(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Uoq')"] */
Expression::SPType X86Architecture::Decode_Uoq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Uoq')
  return __Decode_Uoq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Uq')"] */
Expression::SPType X86Architecture::Decode_Uq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Uq')
  return __Decode_Uq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Ux')"] */
Expression::SPType X86Architecture::Decode_Ux(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Ux')
  return __Decode_Ux(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Vo')"] */
Expression::SPType X86Architecture::Decode_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Vo')
  return __Decode_Vo(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Vod')"] */
Expression::SPType X86Architecture::Decode_Vod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Vod')
  return __Decode_Vod(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Voq')"] */
Expression::SPType X86Architecture::Decode_Voq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Voq')
  return __Decode_Voq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Vx')"] */
Expression::SPType X86Architecture::Decode_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Vx')
  return __Decode_Vx(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Vy')"] */
Expression::SPType X86Architecture::Decode_Vy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Vy')
  return __Decode_Vy(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wd')"] */
Expression::SPType X86Architecture::Decode_Wd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wd')
  return __Decode_Wd(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wo')"] */
Expression::SPType X86Architecture::Decode_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wo')
  return __Decode_Wo(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wob')"] */
Expression::SPType X86Architecture::Decode_Wob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wob')
  return __Decode_Wob(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wod')"] */
Expression::SPType X86Architecture::Decode_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wod')
  return __Decode_Wod(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Woo')"] */
Expression::SPType X86Architecture::Decode_Woo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Woo')
  return __Decode_Woo(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Woq')"] */
Expression::SPType X86Architecture::Decode_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Woq')
  return __Decode_Woq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Woqo')"] */
Expression::SPType X86Architecture::Decode_Woqo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Woqo')
  return __Decode_Woqo(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wow')"] */
Expression::SPType X86Architecture::Decode_Wow(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wow')
  return __Decode_Wow(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wq')"] */
Expression::SPType X86Architecture::Decode_Wq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wq')
  return __Decode_Wq(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wx')"] */
Expression::SPType X86Architecture::Decode_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wx')
  return __Decode_Wx(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_Wy')"] */
Expression::SPType X86Architecture::Decode_Wy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_Wy')
  return __Decode_Wy(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ['addr_b(decode_DS, decode_d64_rSI)'] */
Expression::SPType X86Architecture::Decode_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: addr_b(decode_DS, decode_d64_rSI)
  return Expr::MakeMem(8, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
}

/* decoder ['addr_v(decode_DS, decode_d64_rSI)'] */
Expression::SPType X86Architecture::Decode_Xv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: addr_v(decode_DS, decode_d64_rSI)
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(32, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(16, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeMem(64, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(16, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(32, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
  default:
    return nullptr;
  }

}

/* decoder ['addr_z(decode_DS, decode_d64_rSI)'] */
Expression::SPType X86Architecture::Decode_Xz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: addr_z(decode_DS, decode_d64_rSI)
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(32, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(16, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(16, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(32, Decode_DS(rBinStrm, Offset, rInsn, Mode), Decode_d64_rSI(rBinStrm, Offset, rInsn, Mode), true);
  default:
    return nullptr;
  }

}

/* decoder ['addr_b(decode_ES, decode_d64_rDI)'] */
Expression::SPType X86Architecture::Decode_Yb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: addr_b(decode_ES, decode_d64_rDI)
  return Expr::MakeMem(8, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
}

/* decoder ['addr_v(decode_ES, decode_d64_rDI)'] */
Expression::SPType X86Architecture::Decode_Yv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: addr_v(decode_ES, decode_d64_rDI)
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(32, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(16, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeMem(64, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(16, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(32, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
  default:
    return nullptr;
  }

}

/* decoder ['addr_z(decode_ES, decode_d64_rDI)'] */
Expression::SPType X86Architecture::Decode_Yz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: addr_z(decode_ES, decode_d64_rDI)
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(32, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(16, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeMem(16, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
    else
      return Expr::MakeMem(32, Decode_ES(rBinStrm, Offset, rInsn, Mode), Decode_d64_rDI(rBinStrm, Offset, rInsn, Mode), true);
  default:
    return nullptr;
  }

}

/* decoder ["call('Decode_b')"] */
Expression::SPType X86Architecture::Decode_b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_b')
  return __Decode_b(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg_d64_r('r10w', 'r10d', 'r10')"] */
Expression::SPType X86Architecture::Decode_d64_r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r10w', 'r10d', 'r10')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R10d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R10w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R10, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R10w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R10d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('r11w', 'r11d', 'r11')"] */
Expression::SPType X86Architecture::Decode_d64_r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r11w', 'r11d', 'r11')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R11d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R11w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R11, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R11w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R11d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('r12w', 'r12d', 'r12')"] */
Expression::SPType X86Architecture::Decode_d64_r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r12w', 'r12d', 'r12')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R12d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R12w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R12, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R12w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R12d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('r13w', 'r13d', 'r13')"] */
Expression::SPType X86Architecture::Decode_d64_r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r13w', 'r13d', 'r13')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R13d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R13w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R13, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R13w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R13d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('r14w', 'r14d', 'r14')"] */
Expression::SPType X86Architecture::Decode_d64_r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r14w', 'r14d', 'r14')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R14d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R14w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R14, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R14w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R14d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('r15w', 'r15d', 'r15')"] */
Expression::SPType X86Architecture::Decode_d64_r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r15w', 'r15d', 'r15')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R15d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R15w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R15, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R15w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R15d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('r8w', 'r8d', 'r8')"] */
Expression::SPType X86Architecture::Decode_d64_r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r8w', 'r8d', 'r8')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R8d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R8w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R8, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R8w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R8d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('r9w', 'r9d', 'r9')"] */
Expression::SPType X86Architecture::Decode_d64_r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('r9w', 'r9d', 'r9')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R9d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R9w, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_R9, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R9w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R9d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('ax', 'eax', 'rax')"] */
Expression::SPType X86Architecture::Decode_d64_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('ax', 'eax', 'rax')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Eax, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ax, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rax, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ax, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Eax, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('bp', 'ebp', 'rbp')"] */
Expression::SPType X86Architecture::Decode_d64_rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('bp', 'ebp', 'rbp')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ebp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Bp, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rbp, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Bp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ebp, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('bx', 'ebx', 'rbx')"] */
Expression::SPType X86Architecture::Decode_d64_rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('bx', 'ebx', 'rbx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ebx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Bx, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rbx, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Bx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ebx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('cx', 'ecx', 'rcx')"] */
Expression::SPType X86Architecture::Decode_d64_rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('cx', 'ecx', 'rcx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ecx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Cx, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rcx, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Cx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ecx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('di', 'edi', 'rdi')"] */
Expression::SPType X86Architecture::Decode_d64_rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('di', 'edi', 'rdi')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Edi, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Di, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rdi, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Di, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Edi, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('dx', 'edx', 'rdx')"] */
Expression::SPType X86Architecture::Decode_d64_rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('dx', 'edx', 'rdx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Edx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Dx, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rdx, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Dx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Edx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('si', 'esi', 'rsi')"] */
Expression::SPType X86Architecture::Decode_d64_rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('si', 'esi', 'rsi')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Esi, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Si, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rsi, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Si, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Esi, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_d64_r('sp', 'esp', 'rsp')"] */
Expression::SPType X86Architecture::Decode_d64_rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_d64_r('sp', 'esp', 'rsp')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Esp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Sp, &m_CpuInfo);
  case X86_Bit_64:
    return Expr::MakeId(X86_Reg_Rsp, &m_CpuInfo);case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Sp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Esp, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('ax', 'eax')"] */
Expression::SPType X86Architecture::Decode_eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('ax', 'eax')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Eax, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ax, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ax, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Eax, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('bp', 'ebp')"] */
Expression::SPType X86Architecture::Decode_eBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('bp', 'ebp')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ebp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Bp, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Bp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ebp, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('bx', 'ebx')"] */
Expression::SPType X86Architecture::Decode_eBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('bx', 'ebx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ebx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Bx, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Bx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ebx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('cx', 'ecx')"] */
Expression::SPType X86Architecture::Decode_eCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('cx', 'ecx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ecx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Cx, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Cx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ecx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('di', 'edi')"] */
Expression::SPType X86Architecture::Decode_eDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('di', 'edi')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Edi, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Di, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Di, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Edi, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('dx', 'edx')"] */
Expression::SPType X86Architecture::Decode_eDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('dx', 'edx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Edx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Dx, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Dx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Edx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('si', 'esi')"] */
Expression::SPType X86Architecture::Decode_eSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('si', 'esi')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Esi, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Si, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Si, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Esi, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_z('sp', 'esp')"] */
Expression::SPType X86Architecture::Decode_eSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_z('sp', 'esp')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Esp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Sp, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Sp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Esp, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["call('Decode_m16int')"] */
Expression::SPType X86Architecture::Decode_m16int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m16int')
  return __Decode_m16int(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_m32fp')"] */
Expression::SPType X86Architecture::Decode_m32fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m32fp')
  return __Decode_m32fp(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_m32int')"] */
Expression::SPType X86Architecture::Decode_m32int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m32int')
  return __Decode_m32int(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_m64fp')"] */
Expression::SPType X86Architecture::Decode_m64fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m64fp')
  return __Decode_m64fp(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_m64int')"] */
Expression::SPType X86Architecture::Decode_m64int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m64int')
  return __Decode_m64int(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_m80bcd')"] */
Expression::SPType X86Architecture::Decode_m80bcd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m80bcd')
  return __Decode_m80bcd(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_m80dec')"] */
Expression::SPType X86Architecture::Decode_m80dec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m80dec')
  return __Decode_m80dec(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_m80fp')"] */
Expression::SPType X86Architecture::Decode_m80fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_m80fp')
  return __Decode_m80fp(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["reg_v('r10w', 'r10d', 'r10')"] */
Expression::SPType X86Architecture::Decode_r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r10w', 'r10d', 'r10')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R10d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R10w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R10, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R10w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R10d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r10b')"] */
Expression::SPType X86Architecture::Decode_r10b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r10b')
  return Expr::MakeId(X86_Reg_R10b, &m_CpuInfo);
}

/* decoder ["reg_v('r11w', 'r11d', 'r11')"] */
Expression::SPType X86Architecture::Decode_r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r11w', 'r11d', 'r11')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R11d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R11w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R11, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R11w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R11d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r11b')"] */
Expression::SPType X86Architecture::Decode_r11b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r11b')
  return Expr::MakeId(X86_Reg_R11b, &m_CpuInfo);
}

/* decoder ["reg_v('r12w', 'r12d', 'r12')"] */
Expression::SPType X86Architecture::Decode_r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r12w', 'r12d', 'r12')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R12d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R12w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R12, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R12w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R12d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r12b')"] */
Expression::SPType X86Architecture::Decode_r12b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r12b')
  return Expr::MakeId(X86_Reg_R12b, &m_CpuInfo);
}

/* decoder ["reg_v('r13w', 'r13d', 'r13')"] */
Expression::SPType X86Architecture::Decode_r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r13w', 'r13d', 'r13')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R13d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R13w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R13, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R13w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R13d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r13b')"] */
Expression::SPType X86Architecture::Decode_r13b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r13b')
  return Expr::MakeId(X86_Reg_R13b, &m_CpuInfo);
}

/* decoder ["reg_v('r14w', 'r14d', 'r14')"] */
Expression::SPType X86Architecture::Decode_r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r14w', 'r14d', 'r14')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R14d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R14w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R14, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R14w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R14d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r14b')"] */
Expression::SPType X86Architecture::Decode_r14b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r14b')
  return Expr::MakeId(X86_Reg_R14b, &m_CpuInfo);
}

/* decoder ["reg_v('r15w', 'r15d', 'r15')"] */
Expression::SPType X86Architecture::Decode_r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r15w', 'r15d', 'r15')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R15d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R15w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R15, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R15w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R15d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r15b')"] */
Expression::SPType X86Architecture::Decode_r15b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r15b')
  return Expr::MakeId(X86_Reg_R15b, &m_CpuInfo);
}

/* decoder ["reg_v('r8w', 'r8d', 'r8')"] */
Expression::SPType X86Architecture::Decode_r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r8w', 'r8d', 'r8')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R8d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R8w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R8, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R8w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R8d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r8b')"] */
Expression::SPType X86Architecture::Decode_r8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r8b')
  return Expr::MakeId(X86_Reg_R8b, &m_CpuInfo);
}

/* decoder ["reg_v('r9w', 'r9d', 'r9')"] */
Expression::SPType X86Architecture::Decode_r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('r9w', 'r9d', 'r9')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R9d, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R9w, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_R9, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_R9w, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_R9d, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg('r9b')"] */
Expression::SPType X86Architecture::Decode_r9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('r9b')
  return Expr::MakeId(X86_Reg_R9b, &m_CpuInfo);
}

/* decoder ["reg_v('ax', 'eax', 'rax')"] */
Expression::SPType X86Architecture::Decode_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('ax', 'eax', 'rax')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Eax, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ax, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rax, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ax, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Eax, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_v('bp', 'ebp', 'rbp')"] */
Expression::SPType X86Architecture::Decode_rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('bp', 'ebp', 'rbp')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ebp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Bp, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rbp, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Bp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ebp, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_v('bx', 'ebx', 'rbx')"] */
Expression::SPType X86Architecture::Decode_rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('bx', 'ebx', 'rbx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ebx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Bx, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rbx, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Bx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ebx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_v('cx', 'ecx', 'rcx')"] */
Expression::SPType X86Architecture::Decode_rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('cx', 'ecx', 'rcx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Ecx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Cx, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rcx, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Cx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Ecx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_v('di', 'edi', 'rdi')"] */
Expression::SPType X86Architecture::Decode_rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('di', 'edi', 'rdi')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Edi, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Di, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rdi, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Di, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Edi, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_v('dx', 'edx', 'rdx')"] */
Expression::SPType X86Architecture::Decode_rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('dx', 'edx', 'rdx')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Edx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Dx, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rdx, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Dx, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Edx, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_v('si', 'esi', 'rsi')"] */
Expression::SPType X86Architecture::Decode_rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('si', 'esi', 'rsi')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Esi, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Si, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rsi, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Si, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Esi, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["reg_v('sp', 'esp', 'rsp')"] */
Expression::SPType X86Architecture::Decode_rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg_v('sp', 'esp', 'rsp')
  switch (Mode)
  {
  case X86_Bit_16:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Esp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Sp, &m_CpuInfo);
  case X86_Bit_64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return Expr::MakeId(X86_Reg_Rsp, &m_CpuInfo);
  case X86_Bit_32:
    if ((rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize)
      return Expr::MakeId(X86_Reg_Sp, &m_CpuInfo);
    else
      return Expr::MakeId(X86_Reg_Esp, &m_CpuInfo);
  default:
    return nullptr;
  }

}

/* decoder ["call('Decode_w')"] */
Expression::SPType X86Architecture::Decode_w(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_w')
  return __Decode_w(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_x')"] */
Expression::SPType X86Architecture::Decode_x(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_x')
  return __Decode_x(rBinStrm, Offset, rInsn, Mode);
}

/* operand ['op0 = decode_eAX'] */
bool X86Architecture::Operand__eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eAX
  auto spOprd0 = Decode_eAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_eAX', 'op1 = decode_DX'] */
bool X86Architecture::Operand__eAX_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eAX
  auto spOprd0 = Decode_eAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_DX
  auto spOprd1 = Decode_DX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_eAX', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__eAX_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eAX
  auto spOprd0 = Decode_eAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_eBP'] */
bool X86Architecture::Operand__eBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eBP
  auto spOprd0 = Decode_eBP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_eBX'] */
bool X86Architecture::Operand__eBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eBX
  auto spOprd0 = Decode_eBX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_eCX'] */
bool X86Architecture::Operand__eCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eCX
  auto spOprd0 = Decode_eCX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_eDI'] */
bool X86Architecture::Operand__eDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eDI
  auto spOprd0 = Decode_eDI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_eDX'] */
bool X86Architecture::Operand__eDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eDX
  auto spOprd0 = Decode_eDX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_eSI'] */
bool X86Architecture::Operand__eSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eSI
  auto spOprd0 = Decode_eSI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_eSP'] */
bool X86Architecture::Operand__eSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_eSP
  auto spOprd0 = Decode_eSP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m16int'] */
bool X86Architecture::Operand__m16int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m16int
  auto spOprd0 = Decode_m16int(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m32fp'] */
bool X86Architecture::Operand__m32fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m32fp
  auto spOprd0 = Decode_m32fp(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m32int'] */
bool X86Architecture::Operand__m32int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m32int
  auto spOprd0 = Decode_m32int(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m64fp'] */
bool X86Architecture::Operand__m64fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m64fp
  auto spOprd0 = Decode_m64fp(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m64int'] */
bool X86Architecture::Operand__m64int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m64int
  auto spOprd0 = Decode_m64int(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m80bcd'] */
bool X86Architecture::Operand__m80bcd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m80bcd
  auto spOprd0 = Decode_m80bcd(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m80dec'] */
bool X86Architecture::Operand__m80dec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m80dec
  auto spOprd0 = Decode_m80dec(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_m80fp'] */
bool X86Architecture::Operand__m80fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_m80fp
  auto spOprd0 = Decode_m80fp(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r10'] */
bool X86Architecture::Operand__r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r10
  auto spOprd0 = Decode_r10(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r10', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r10_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r10
  auto spOprd0 = Decode_r10(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r10', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r10_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r10
  auto spOprd0 = Decode_r10(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r10b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r10b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r10b
  auto spOprd0 = Decode_r10b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r11'] */
bool X86Architecture::Operand__r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r11
  auto spOprd0 = Decode_r11(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r11', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r11_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r11
  auto spOprd0 = Decode_r11(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r11', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r11_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r11
  auto spOprd0 = Decode_r11(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r11b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r11b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r11b
  auto spOprd0 = Decode_r11b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r12'] */
bool X86Architecture::Operand__r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r12
  auto spOprd0 = Decode_r12(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r12', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r12_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r12
  auto spOprd0 = Decode_r12(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r12', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r12_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r12
  auto spOprd0 = Decode_r12(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r12b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r12b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r12b
  auto spOprd0 = Decode_r12b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r13'] */
bool X86Architecture::Operand__r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r13
  auto spOprd0 = Decode_r13(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r13', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r13_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r13
  auto spOprd0 = Decode_r13(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r13', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r13_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r13
  auto spOprd0 = Decode_r13(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r13b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r13b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r13b
  auto spOprd0 = Decode_r13b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r14'] */
bool X86Architecture::Operand__r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r14
  auto spOprd0 = Decode_r14(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r14', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r14_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r14
  auto spOprd0 = Decode_r14(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r14', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r14_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r14
  auto spOprd0 = Decode_r14(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r14b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r14b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r14b
  auto spOprd0 = Decode_r14b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r15'] */
bool X86Architecture::Operand__r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r15
  auto spOprd0 = Decode_r15(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r15', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r15_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r15
  auto spOprd0 = Decode_r15(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r15', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r15_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r15
  auto spOprd0 = Decode_r15(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r15b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r15b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r15b
  auto spOprd0 = Decode_r15b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r8'] */
bool X86Architecture::Operand__r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r8
  auto spOprd0 = Decode_r8(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r8', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r8_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r8
  auto spOprd0 = Decode_r8(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r8', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r8_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r8
  auto spOprd0 = Decode_r8(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r8b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r8b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r8b
  auto spOprd0 = Decode_r8b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r9'] */
bool X86Architecture::Operand__r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r9
  auto spOprd0 = Decode_r9(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_r9', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__r9_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r9
  auto spOprd0 = Decode_r9(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r9', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__r9_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r9
  auto spOprd0 = Decode_r9(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_r9b', 'op1 = decode_Ib'] */
bool X86Architecture::Operand__r9b_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_r9b
  auto spOprd0 = Decode_r9b(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ib
  auto spOprd1 = Decode_Ib(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rAX'] */
bool X86Architecture::Operand__rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rAX
  auto spOprd0 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rAX', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rAX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rAX
  auto spOprd0 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rAX', 'op1 = decode_Iz'] */
bool X86Architecture::Operand__rAX_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rAX
  auto spOprd0 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iz
  auto spOprd1 = Decode_Iz(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rAX', 'op1 = decode_Ov'] */
bool X86Architecture::Operand__rAX_Ov(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rAX
  auto spOprd0 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Ov
  auto spOprd1 = Decode_Ov(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rAX', 'op1 = decode_Xv'] */
bool X86Architecture::Operand__rAX_Xv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rAX
  auto spOprd0 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Xv
  auto spOprd1 = Decode_Xv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rAX', 'op1 = decode_Yv'] */
bool X86Architecture::Operand__rAX_Yv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rAX
  auto spOprd0 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Yv
  auto spOprd1 = Decode_Yv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rBP'] */
bool X86Architecture::Operand__rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rBP
  auto spOprd0 = Decode_rBP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rBP', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rBP_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rBP
  auto spOprd0 = Decode_rBP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rBP', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__rBP_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rBP
  auto spOprd0 = Decode_rBP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rBX'] */
bool X86Architecture::Operand__rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rBX
  auto spOprd0 = Decode_rBX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rBX', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rBX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rBX
  auto spOprd0 = Decode_rBX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rBX', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__rBX_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rBX
  auto spOprd0 = Decode_rBX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rCX'] */
bool X86Architecture::Operand__rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rCX
  auto spOprd0 = Decode_rCX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rCX', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rCX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rCX
  auto spOprd0 = Decode_rCX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rCX', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__rCX_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rCX
  auto spOprd0 = Decode_rCX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rDI'] */
bool X86Architecture::Operand__rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rDI
  auto spOprd0 = Decode_rDI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rDI', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rDI_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rDI
  auto spOprd0 = Decode_rDI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rDI', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__rDI_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rDI
  auto spOprd0 = Decode_rDI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rDX'] */
bool X86Architecture::Operand__rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rDX
  auto spOprd0 = Decode_rDX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rDX', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rDX_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rDX
  auto spOprd0 = Decode_rDX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rDX', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__rDX_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rDX
  auto spOprd0 = Decode_rDX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rSI'] */
bool X86Architecture::Operand__rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rSI
  auto spOprd0 = Decode_rSI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rSI', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rSI_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rSI
  auto spOprd0 = Decode_rSI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rSI', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__rSI_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rSI
  auto spOprd0 = Decode_rSI(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rSP'] */
bool X86Architecture::Operand__rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rSP
  auto spOprd0 = Decode_rSP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rSP', 'op1 = decode_Iv'] */
bool X86Architecture::Operand__rSP_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rSP
  auto spOprd0 = Decode_rSP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_Iv
  auto spOprd1 = Decode_Iv(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

/* operand ['op0 = decode_rSP', 'op1 = decode_rAX'] */
bool X86Architecture::Operand__rSP_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rSP
  auto spOprd0 = Decode_rSP(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  // operand1: op1 = decode_rAX
  auto spOprd1 = Decode_rAX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd1 == nullptr)
    return false;
  rInsn.AddOperand(spOprd1);

  return true;
}

