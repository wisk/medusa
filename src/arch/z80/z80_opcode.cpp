/* This file has been automatically generated, you must _NOT_ edit it directly. (Sun Dec 28 02:05:39 2014) */
#include "z80_architecture.hpp"
bool Z80Architecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  u8 Opcode0;
  if (!rBinStrm.Read(Offset + 0, Opcode0))
    return false;

  switch (Opcode0)
  {
  case 0x00:
    rInsn.SetName("nop");
    return true;
  case 0x01:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_BC, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(16, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x02:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_BC, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x03:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_BC, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x04:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x05:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x06:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x07:
    rInsn.SetName("rlc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x08:
    rInsn.SetName("ld");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeConst(16, Imm), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x09:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_BC, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x0a:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_BC, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x0b:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_BC, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x0c:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x0d:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x0e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x0f:
    rInsn.SetName("rrc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x10:
    rInsn.SetName("stop");
    return true;
  case 0x11:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_DE, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(16, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x12:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_DE, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x13:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_DE, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x14:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x15:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x16:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x17:
    rInsn.SetName("rl");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x18:
    rInsn.SetName("jr");
    {
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeId(Z80_Reg_Pc, &m_CpuInfo),
        Expr::MakeConst(8, Imm));
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x19:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_DE, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x1a:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_DE, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x1b:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_DE, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x1c:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x1d:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x1e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x1f:
    rInsn.SetName("rr");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x20:
    rInsn.SetName("jr");
    {
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeId(Z80_Reg_Pc, &m_CpuInfo),
        Expr::MakeConst(8, Imm));
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x21:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_DE, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(16, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x22:
    rInsn.SetName("ldi");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x23:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x24:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x25:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x26:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x27:
    rInsn.SetName("daa");
    return true;
  case 0x28:
    rInsn.SetName("jr");
    {
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeId(Z80_Reg_Pc, &m_CpuInfo),
        Expr::MakeConst(8, Imm));
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x29:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x2a:
    rInsn.SetName("ldi");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x2b:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x2c:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x2d:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x2e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x2f:
    rInsn.SetName("cpl");
    return true;
  case 0x30:
    rInsn.SetName("jr");
    {
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeId(Z80_Reg_Pc, &m_CpuInfo),
        Expr::MakeConst(8, Imm));
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x31:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(16, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x32:
    rInsn.SetName("ldd");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x33:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x34:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x35:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x36:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x37:
    rInsn.SetName("scf");
    return true;
  case 0x38:
    rInsn.SetName("jr");
    {
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeId(Z80_Reg_Pc, &m_CpuInfo),
        Expr::MakeConst(8, Imm));
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x39:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x3a:
    rInsn.SetName("ldd");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x3b:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x3c:
    rInsn.SetName("inc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x3d:
    rInsn.SetName("dec");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0x3e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x3f:
    rInsn.SetName("ccf");
    return true;
  case 0x40:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x41:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x42:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x43:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x44:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x45:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x46:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x47:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x48:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x49:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x4a:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x4b:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x4c:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x4d:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x4e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x4f:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x50:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x51:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x52:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x53:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x54:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x55:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x56:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x57:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x58:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x59:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x5a:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x5b:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x5c:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x5d:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x5e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x5f:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x60:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x61:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x62:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x63:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x64:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x65:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x66:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x67:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x68:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x69:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x6a:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x6b:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x6c:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x6d:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x6e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x6f:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x70:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x71:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x72:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x73:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x74:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x75:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x76:
    rInsn.SetName("halt");
    return true;
  case 0x77:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x78:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x79:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x7a:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x7b:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x7c:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x7d:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x7e:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x7f:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x80:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x81:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x82:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x83:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x84:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x85:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x86:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x87:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x88:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x89:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x8a:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x8b:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x8c:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x8d:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x8e:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x8f:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x90:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x91:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x92:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x93:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x94:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x95:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x96:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x97:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x98:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x99:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x9a:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x9b:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x9c:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x9d:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x9e:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0x9f:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa0:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa1:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa2:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa3:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa4:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa5:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa6:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa7:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa8:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xa9:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xaa:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xab:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xac:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xad:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xae:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xaf:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb0:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb1:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb2:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb3:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb4:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb5:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb6:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb7:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb8:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xb9:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xba:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xbb:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xbc:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xbd:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xbe:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xbf:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xc0:
    rInsn.SetName("ret");
    return true;
  case 0xc1:
    rInsn.SetName("pop");
    return true;
  case 0xc2:
    rInsn.SetName("jp");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xc3:
    rInsn.SetName("jp");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xc4:
    rInsn.SetName("call");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xc5:
    rInsn.SetName("push");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_BC, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xc6:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xc7:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x0);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xc8:
    rInsn.SetName("ret");
    return true;
  case 0xc9:
    rInsn.SetName("ret");
    return true;
  case 0xca:
    rInsn.SetName("jp");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xcb:
    rInsn.Length() += 1;
    {
      u8 Opcode1;
      if (!rBinStrm.Read(Offset + 1, Opcode1))
        return false;

      switch (Opcode1)
      {
      case 0x00:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x01:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x02:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x03:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x04:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x05:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x06:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x07:
        rInsn.SetName("rlc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x08:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x09:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x0a:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x0b:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x0c:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x0d:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x0e:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x0f:
        rInsn.SetName("rrc");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x10:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x11:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x12:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x13:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x14:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x15:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x16:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x17:
        rInsn.SetName("rl");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x18:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x19:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x1a:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x1b:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x1c:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x1d:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x1e:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x1f:
        rInsn.SetName("rr");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x20:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x21:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x22:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x23:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x24:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x25:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x26:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x27:
        rInsn.SetName("sla");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x28:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x29:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x2a:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x2b:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x2c:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x2d:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x2e:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x2f:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x30:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x31:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x32:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x33:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x34:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x35:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x36:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x37:
        rInsn.SetName("swap");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x38:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x39:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x3a:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x3b:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x3c:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x3d:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x3e:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x3f:
        rInsn.SetName("sra");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x40:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x41:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x42:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x43:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x44:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x45:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x46:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x47:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x48:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x49:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x4a:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x4b:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x4c:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x4d:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x4e:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x4f:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x50:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x51:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x52:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x53:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x54:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x55:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x56:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x57:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x58:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x59:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x5a:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x5b:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x5c:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x5d:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x5e:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x5f:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x60:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x61:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x62:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x63:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x64:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x65:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x66:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x67:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x68:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x69:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x6a:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x6b:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x6c:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x6d:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x6e:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x6f:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x70:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x71:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x72:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x73:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x74:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x75:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x76:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x77:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x78:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x79:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x7a:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x7b:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x7c:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x7d:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x7e:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x7f:
        rInsn.SetName("bit");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x80:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x81:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x82:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x83:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x84:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x85:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x86:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x87:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x88:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x89:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x8a:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x8b:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x8c:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x8d:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x8e:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x8f:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x90:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x91:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x92:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x93:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x94:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x95:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x96:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x97:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x98:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0x99:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x9a:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x9b:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x9c:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x9d:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x9e:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0x9f:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa0:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa1:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa2:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa3:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa4:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa5:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa6:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa7:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xa8:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0xa9:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xaa:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xab:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xac:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xad:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xae:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xaf:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb0:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb1:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb2:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb3:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb4:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb5:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb6:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb7:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xb8:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0xb9:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xba:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xbb:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xbc:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xbd:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xbe:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xbf:
        rInsn.SetName("res");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc0:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc1:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc2:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc3:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc4:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc5:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc6:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc7:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x0);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xc8:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0xc9:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xca:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xcb:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xcc:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xcd:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xce:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xcf:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x1);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd0:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd1:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd2:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd3:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd4:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd5:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd6:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd7:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x2);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xd8:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0xd9:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xda:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xdb:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xdc:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xdd:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xde:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xdf:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x3);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe0:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe1:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe2:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe3:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe4:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe5:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe6:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe7:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x4);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xe8:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0xe9:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xea:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xeb:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xec:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xed:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xee:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xef:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x5);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf0:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf1:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf2:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf3:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf4:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf5:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf6:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf7:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x6);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xf8:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeId(Z80_Reg_B, &m_CpuInfo);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
        }
        return true;
      case 0xf9:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_C, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xfa:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_D, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xfb:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_E, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xfc:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_H, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xfd:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_L, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xfe:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      case 0xff:
        rInsn.SetName("set");
        {
          auto spOprd0 = Expr::MakeConst(8, 0x7);
          if (spOprd0 == nullptr)
            return false;
          rInsn.AddOperand(spOprd0);
          auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
          if (spOprd1 == nullptr)
            return false;
          rInsn.AddOperand(spOprd1);
        }
        return true;
      default:
        return false;}
    }
    return true;
  case 0xcc:
    rInsn.SetName("call");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xcd:
    rInsn.SetName("call");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xce:
    rInsn.SetName("adc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xcf:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x8);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xd0:
    rInsn.SetName("ret");
    return true;
  case 0xd1:
    rInsn.SetName("pop");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_DE, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xd2:
    rInsn.SetName("jp");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xd3:
    rInsn.SetName("invalid");
    return true;
  case 0xd4:
    rInsn.SetName("call");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xd5:
    rInsn.SetName("push");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_DE, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xd6:
    rInsn.SetName("sub");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xd7:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x10);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xd8:
    rInsn.SetName("ret");
    return true;
  case 0xd9:
    rInsn.SetName("reti");
    return true;
  case 0xda:
    rInsn.SetName("jp");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xdb:
    rInsn.SetName("unknown");
    return true;
  case 0xdc:
    rInsn.SetName("call");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeConst(16, Imm);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xdd:
    rInsn.SetName("unknown");
    return true;
  case 0xde:
    rInsn.SetName("sbc");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xdf:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x18);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xe0:
    rInsn.SetName("ldh");
    {
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeConst(16, 0xff00),
        Expr::MakeConst(8, Imm)), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xe1:
    rInsn.SetName("pop");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xe2:
    rInsn.SetName("ldh");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeConst(16, 0xff00),
        Expr::MakeId(Z80_Reg_C, &m_CpuInfo)), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xe3:
    rInsn.SetName("unknown");
    return true;
  case 0xe4:
    rInsn.SetName("unknown");
    return true;
  case 0xe5:
    rInsn.SetName("push");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xe6:
    rInsn.SetName("and");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xe7:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x20);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xe8:
    rInsn.SetName("add");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xe9:
    rInsn.SetName("jp");
    {
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeId(Z80_Reg_HL, &m_CpuInfo), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xea:
    rInsn.SetName("ld");
    {
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd0 = Expr::MakeMem(8, nullptr, Expr::MakeConst(16, Imm), true);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xeb:
    rInsn.SetName("unknown");
    return true;
  case 0xec:
    rInsn.SetName("unknown");
    return true;
  case 0xed:
    rInsn.SetName("unknown");
    return true;
  case 0xee:
    rInsn.SetName("xor");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xef:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x28);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xf0:
    rInsn.SetName("ldh");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeConst(16, 0xff00),
        Expr::MakeConst(8, Imm)), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xf1:
    rInsn.SetName("pop");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_AF, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xf2:
    rInsn.SetName("unknown");
    return true;
  case 0xf3:
    rInsn.SetName("di");
    return true;
  case 0xf4:
    rInsn.SetName("unknown");
    return true;
  case 0xf5:
    rInsn.SetName("push");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_AF, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xf6:
    rInsn.SetName("or");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xf7:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x30);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  case 0xf8:
    rInsn.SetName("ldhl");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeOp(OperationExpression::OpAdd,
        Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo),
        Expr::MakeConst(8, Imm));
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xf9:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_Sp, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      auto spOprd1 = Expr::MakeId(Z80_Reg_HL, &m_CpuInfo);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xfa:
    rInsn.SetName("ld");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u16 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeMem(8, nullptr, Expr::MakeConst(16, Imm), true);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xfb:
    rInsn.SetName("ei");
    return true;
  case 0xfc:
    rInsn.SetName("unknown");
    return true;
  case 0xfd:
    rInsn.SetName("unknown");
    return true;
  case 0xfe:
    rInsn.SetName("cp");
    {
      auto spOprd0 = Expr::MakeId(Z80_Reg_A, &m_CpuInfo);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
      u8 Imm;
      if (!rBinStrm.Read(Offset, Imm))
        return false;

      rInsn.Length() += sizeof(Imm);
      auto spOprd1 = Expr::MakeConst(8, Imm);
      if (spOprd1 == nullptr)
        return false;
      rInsn.AddOperand(spOprd1);
    }
    return true;
  case 0xff:
    rInsn.SetName("rst");
    {
      auto spOprd0 = Expr::MakeConst(8, 0x38);
      if (spOprd0 == nullptr)
        return false;
      rInsn.AddOperand(spOprd0);
    }
    return true;
  default:
    return false;}
}
