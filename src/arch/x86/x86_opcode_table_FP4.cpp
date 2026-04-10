/* This file has been automatically generated, you must _NOT_ edit it directly. (Thu Apr  9 18:19:39 2026) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp4[0x40] =
{
  &X86Architecture::Table_fp4_c0,
  &X86Architecture::Table_fp4_c1,
  &X86Architecture::Table_fp4_c2,
  &X86Architecture::Table_fp4_c3,
  &X86Architecture::Table_fp4_c4,
  &X86Architecture::Table_fp4_c5,
  &X86Architecture::Table_fp4_c6,
  &X86Architecture::Table_fp4_c7,
  &X86Architecture::Table_fp4_c8,
  &X86Architecture::Table_fp4_c9,
  &X86Architecture::Table_fp4_ca,
  &X86Architecture::Table_fp4_cb,
  &X86Architecture::Table_fp4_cc,
  &X86Architecture::Table_fp4_cd,
  &X86Architecture::Table_fp4_ce,
  &X86Architecture::Table_fp4_cf,
  &X86Architecture::Table_fp4_d0,
  &X86Architecture::Table_fp4_d1,
  &X86Architecture::Table_fp4_d2,
  &X86Architecture::Table_fp4_d3,
  &X86Architecture::Table_fp4_d4,
  &X86Architecture::Table_fp4_d5,
  &X86Architecture::Table_fp4_d6,
  &X86Architecture::Table_fp4_d7,
  &X86Architecture::Table_fp4_d8,
  &X86Architecture::Table_fp4_d9,
  &X86Architecture::Table_fp4_da,
  &X86Architecture::Table_fp4_db,
  &X86Architecture::Table_fp4_dc,
  &X86Architecture::Table_fp4_dd,
  &X86Architecture::Table_fp4_de,
  &X86Architecture::Table_fp4_df,
  &X86Architecture::Table_fp4_e0,
  &X86Architecture::Table_fp4_e1,
  &X86Architecture::Table_fp4_e2,
  &X86Architecture::Table_fp4_e3,
  &X86Architecture::Table_fp4_e4,
  &X86Architecture::Table_fp4_e5,
  &X86Architecture::Table_fp4_e6,
  &X86Architecture::Table_fp4_e7,
  &X86Architecture::Table_fp4_e8,
  &X86Architecture::Table_fp4_e9,
  &X86Architecture::Table_fp4_ea,
  &X86Architecture::Table_fp4_eb,
  &X86Architecture::Table_fp4_ec,
  &X86Architecture::Table_fp4_ed,
  &X86Architecture::Table_fp4_ee,
  &X86Architecture::Table_fp4_ef,
  &X86Architecture::Table_fp4_f0,
  &X86Architecture::Table_fp4_f1,
  &X86Architecture::Table_fp4_f2,
  &X86Architecture::Table_fp4_f3,
  &X86Architecture::Table_fp4_f4,
  &X86Architecture::Table_fp4_f5,
  &X86Architecture::Table_fp4_f6,
  &X86Architecture::Table_fp4_f7,
  &X86Architecture::Table_fp4_f8,
  &X86Architecture::Table_fp4_f9,
  &X86Architecture::Table_fp4_fa,
  &X86Architecture::Table_fp4_fb,
  &X86Architecture::Table_fp4_fc,
  &X86Architecture::Table_fp4_fd,
  &X86Architecture::Table_fp4_fe,
  &X86Architecture::Table_fp4_ff
};

/** instruction
 * opcode: 00
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp4_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 01
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp4_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp4_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 03
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp4_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp4_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 05
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp4_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp4_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp4_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 08
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp4_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 09
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp4_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0a
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp4_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0b
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp4_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0c
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp4_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0d
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp4_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0e
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp4_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0f
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp4_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 10
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp4_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 11
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp4_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 12
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp4_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 13
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp4_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 14
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp4_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 15
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp4_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 16
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp4_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 17
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp4_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 18
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp4_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 19
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp4_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1a
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp4_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1b
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp4_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1c
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp4_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1d
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp4_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1e
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp4_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1f
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp4_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instructions
 * opcode: 20
 *
 * invalid
 *
 * mnemonic: fneni
 * cpu_model: == X86_Arch_8087
 *
**/
bool X86Architecture::Table_fp4_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") == X86_Arch_8087)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fneni);
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instructions
 * opcode: 21
 *
 * invalid
 *
 * mnemonic: fndisi
 * cpu_model: == X86_Arch_8087
 *
**/
bool X86Architecture::Table_fp4_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") == X86_Arch_8087)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fndisi);
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instruction
 * opcode: 22
 * mnemonic: fnclex
**/
bool X86Architecture::Table_fp4_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fnclex);
    return true;
}

/** instruction
 * opcode: 23
 * mnemonic: fninit
 * semantic: ['program.id = program.id']
**/
bool X86Architecture::Table_fp4_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fninit);
    {
      Expression::LSPType AllExpr;
      /* semantic: program.id = program.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo)));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instructions
 * opcode: 24
 *
 * invalid
 *
 * mnemonic: fnsetpm
 * cpu_model: == X86_Arch_80287
 *
**/
bool X86Architecture::Table_fp4_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") == X86_Arch_80287)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fnsetpm);
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instructions
 * opcode: 25
 *
 * invalid
 *
 * mnemonic: frstpm
 * cpu_model: == X86_Arch_80287xl
 *
**/
bool X86Architecture::Table_fp4_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") == X86_Arch_80287xl)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Frstpm);
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instruction
 * opcode: 26
 * invalid
**/
bool X86Architecture::Table_fp4_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 27
 * invalid
**/
bool X86Architecture::Table_fp4_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instructions
 * opcode: 28
 *
 * mnemonic: fucomi
 * operand: ['ST0', 'ST0']
 *
 * mnemonic: fsbp0
 * attr: ['iit']
 *
**/
bool X86Architecture::Table_fp4_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_IIT)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fsbp0);
      return true;
    }
    else
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomi);
      if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
}

/** instruction
 * opcode: 29
 * mnemonic: fucomi
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp4_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instructions
 * opcode: 2a
 *
 * mnemonic: fucomi
 * operand: ['ST0', 'ST2']
 *
 * mnemonic: fsbp2
 * attr: ['iit']
 *
**/
bool X86Architecture::Table_fp4_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_IIT)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fsbp2);
      return true;
    }
    else
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomi);
      if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
}

/** instructions
 * opcode: 2b
 *
 * mnemonic: fucomi
 * operand: ['ST0', 'ST3']
 *
 * mnemonic: fsbp1
 * attr: ['iit']
 *
**/
bool X86Architecture::Table_fp4_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_IIT)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fsbp1);
      return true;
    }
    else
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomi);
      if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
}

/** instruction
 * opcode: 2c
 * mnemonic: fucomi
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp4_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2d
 * mnemonic: fucomi
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp4_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2e
 * mnemonic: fucomi
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp4_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2f
 * mnemonic: fucomi
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp4_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 30
 * mnemonic: fcomi
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp4_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instructions
 * opcode: 31
 *
 * mnemonic: fcomi
 * operand: ['ST0', 'ST1']
 *
 * mnemonic: f4x4
 * attr: ['iit']
 *
**/
bool X86Architecture::Table_fp4_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_IIT)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_F4x4);
      return true;
    }
    else
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fcomi);
      if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
}

/** instruction
 * opcode: 32
 * mnemonic: fcomi
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp4_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 33
 * mnemonic: fcomi
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp4_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 34
 * mnemonic: fcomi
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp4_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 35
 * mnemonic: fcomi
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp4_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 36
 * mnemonic: fcomi
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp4_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 37
 * mnemonic: fcomi
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp4_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instructions
 * opcode: 38
 *
 * invalid
 *
 * mnemonic: frint2
 * attr: ['cyrix']
 *
**/
bool X86Architecture::Table_fp4_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_CYRIX)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Frint2);
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instruction
 * opcode: 39
 * invalid
**/
bool X86Architecture::Table_fp4_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3a
 * invalid
**/
bool X86Architecture::Table_fp4_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3b
 * invalid
**/
bool X86Architecture::Table_fp4_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3c
 * invalid
**/
bool X86Architecture::Table_fp4_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3d
 * invalid
**/
bool X86Architecture::Table_fp4_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3e
 * invalid
**/
bool X86Architecture::Table_fp4_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3f
 * invalid
**/
bool X86Architecture::Table_fp4_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

