/* This file has been automatically generated, you must _NOT_ edit it directly. (Sun Feb 05 12:38:19 2017) */
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
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST0']
 * opcode: 00
**/
bool X86Architecture::Table_fp4_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST1']
 * opcode: 01
**/
bool X86Architecture::Table_fp4_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST2']
 * opcode: 02
**/
bool X86Architecture::Table_fp4_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST3']
 * opcode: 03
**/
bool X86Architecture::Table_fp4_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST4']
 * opcode: 04
**/
bool X86Architecture::Table_fp4_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST5']
 * opcode: 05
**/
bool X86Architecture::Table_fp4_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST6']
 * opcode: 06
**/
bool X86Architecture::Table_fp4_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnb
 * operand: ['ST0', 'ST7']
 * opcode: 07
**/
bool X86Architecture::Table_fp4_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnb);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST0']
 * opcode: 08
**/
bool X86Architecture::Table_fp4_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST1']
 * opcode: 09
**/
bool X86Architecture::Table_fp4_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST2']
 * opcode: 0a
**/
bool X86Architecture::Table_fp4_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST3']
 * opcode: 0b
**/
bool X86Architecture::Table_fp4_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST4']
 * opcode: 0c
**/
bool X86Architecture::Table_fp4_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST5']
 * opcode: 0d
**/
bool X86Architecture::Table_fp4_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST6']
 * opcode: 0e
**/
bool X86Architecture::Table_fp4_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovne
 * operand: ['ST0', 'ST7']
 * opcode: 0f
**/
bool X86Architecture::Table_fp4_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovne);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST0']
 * opcode: 10
**/
bool X86Architecture::Table_fp4_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST1']
 * opcode: 11
**/
bool X86Architecture::Table_fp4_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST2']
 * opcode: 12
**/
bool X86Architecture::Table_fp4_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST3']
 * opcode: 13
**/
bool X86Architecture::Table_fp4_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST4']
 * opcode: 14
**/
bool X86Architecture::Table_fp4_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST5']
 * opcode: 15
**/
bool X86Architecture::Table_fp4_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST6']
 * opcode: 16
**/
bool X86Architecture::Table_fp4_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnbe
 * operand: ['ST0', 'ST7']
 * opcode: 17
**/
bool X86Architecture::Table_fp4_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnbe);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST0']
 * opcode: 18
**/
bool X86Architecture::Table_fp4_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST1']
 * opcode: 19
**/
bool X86Architecture::Table_fp4_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST2']
 * opcode: 1a
**/
bool X86Architecture::Table_fp4_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST3']
 * opcode: 1b
**/
bool X86Architecture::Table_fp4_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST4']
 * opcode: 1c
**/
bool X86Architecture::Table_fp4_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST5']
 * opcode: 1d
**/
bool X86Architecture::Table_fp4_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST6']
 * opcode: 1e
**/
bool X86Architecture::Table_fp4_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovnu
 * operand: ['ST0', 'ST7']
 * opcode: 1f
**/
bool X86Architecture::Table_fp4_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovnu);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
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
 * mnemonic: fnclex
 * opcode: 22
**/
bool X86Architecture::Table_fp4_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fnclex);
    return true;
}

/** instruction
 * mnemonic: fninit
 * opcode: 23
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
      if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
}

/** instruction
 * mnemonic: fucomi
 * operand: ['ST0', 'ST1']
 * opcode: 29
**/
bool X86Architecture::Table_fp4_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
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
      if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
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
      if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
}

/** instruction
 * mnemonic: fucomi
 * operand: ['ST0', 'ST4']
 * opcode: 2c
**/
bool X86Architecture::Table_fp4_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fucomi
 * operand: ['ST0', 'ST5']
 * opcode: 2d
**/
bool X86Architecture::Table_fp4_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fucomi
 * operand: ['ST0', 'ST6']
 * opcode: 2e
**/
bool X86Architecture::Table_fp4_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fucomi
 * operand: ['ST0', 'ST7']
 * opcode: 2f
**/
bool X86Architecture::Table_fp4_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomi);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomi
 * operand: ['ST0', 'ST0']
 * opcode: 30
**/
bool X86Architecture::Table_fp4_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
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
      if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
}

/** instruction
 * mnemonic: fcomi
 * operand: ['ST0', 'ST2']
 * opcode: 32
**/
bool X86Architecture::Table_fp4_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomi
 * operand: ['ST0', 'ST3']
 * opcode: 33
**/
bool X86Architecture::Table_fp4_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomi
 * operand: ['ST0', 'ST4']
 * opcode: 34
**/
bool X86Architecture::Table_fp4_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomi
 * operand: ['ST0', 'ST5']
 * opcode: 35
**/
bool X86Architecture::Table_fp4_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomi
 * operand: ['ST0', 'ST6']
 * opcode: 36
**/
bool X86Architecture::Table_fp4_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomi
 * operand: ['ST0', 'ST7']
 * opcode: 37
**/
bool X86Architecture::Table_fp4_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomi);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
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

