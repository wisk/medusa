/* This file has been automatically generated, you must _NOT_ edit it directly. (Thu Apr  9 18:19:39 2026) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp6[0x40] =
{
  &X86Architecture::Table_fp6_c0,
  &X86Architecture::Table_fp6_c1,
  &X86Architecture::Table_fp6_c2,
  &X86Architecture::Table_fp6_c3,
  &X86Architecture::Table_fp6_c4,
  &X86Architecture::Table_fp6_c5,
  &X86Architecture::Table_fp6_c6,
  &X86Architecture::Table_fp6_c7,
  &X86Architecture::Table_fp6_c8,
  &X86Architecture::Table_fp6_c9,
  &X86Architecture::Table_fp6_ca,
  &X86Architecture::Table_fp6_cb,
  &X86Architecture::Table_fp6_cc,
  &X86Architecture::Table_fp6_cd,
  &X86Architecture::Table_fp6_ce,
  &X86Architecture::Table_fp6_cf,
  &X86Architecture::Table_fp6_d0,
  &X86Architecture::Table_fp6_d1,
  &X86Architecture::Table_fp6_d2,
  &X86Architecture::Table_fp6_d3,
  &X86Architecture::Table_fp6_d4,
  &X86Architecture::Table_fp6_d5,
  &X86Architecture::Table_fp6_d6,
  &X86Architecture::Table_fp6_d7,
  &X86Architecture::Table_fp6_d8,
  &X86Architecture::Table_fp6_d9,
  &X86Architecture::Table_fp6_da,
  &X86Architecture::Table_fp6_db,
  &X86Architecture::Table_fp6_dc,
  &X86Architecture::Table_fp6_dd,
  &X86Architecture::Table_fp6_de,
  &X86Architecture::Table_fp6_df,
  &X86Architecture::Table_fp6_e0,
  &X86Architecture::Table_fp6_e1,
  &X86Architecture::Table_fp6_e2,
  &X86Architecture::Table_fp6_e3,
  &X86Architecture::Table_fp6_e4,
  &X86Architecture::Table_fp6_e5,
  &X86Architecture::Table_fp6_e6,
  &X86Architecture::Table_fp6_e7,
  &X86Architecture::Table_fp6_e8,
  &X86Architecture::Table_fp6_e9,
  &X86Architecture::Table_fp6_ea,
  &X86Architecture::Table_fp6_eb,
  &X86Architecture::Table_fp6_ec,
  &X86Architecture::Table_fp6_ed,
  &X86Architecture::Table_fp6_ee,
  &X86Architecture::Table_fp6_ef,
  &X86Architecture::Table_fp6_f0,
  &X86Architecture::Table_fp6_f1,
  &X86Architecture::Table_fp6_f2,
  &X86Architecture::Table_fp6_f3,
  &X86Architecture::Table_fp6_f4,
  &X86Architecture::Table_fp6_f5,
  &X86Architecture::Table_fp6_f6,
  &X86Architecture::Table_fp6_f7,
  &X86Architecture::Table_fp6_f8,
  &X86Architecture::Table_fp6_f9,
  &X86Architecture::Table_fp6_fa,
  &X86Architecture::Table_fp6_fb,
  &X86Architecture::Table_fp6_fc,
  &X86Architecture::Table_fp6_fd,
  &X86Architecture::Table_fp6_fe,
  &X86Architecture::Table_fp6_ff
};

/** instruction
 * opcode: 00
 * mnemonic: ffree
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp6_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 01
 * mnemonic: ffree
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp6_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * mnemonic: ffree
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp6_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 03
 * mnemonic: ffree
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp6_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * mnemonic: ffree
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp6_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 05
 * mnemonic: ffree
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp6_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * mnemonic: ffree
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp6_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * mnemonic: ffree
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp6_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (!Operand__ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 08
 * mnemonic: fxch
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp6_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 09
 * mnemonic: fxch
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp6_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0a
 * mnemonic: fxch
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp6_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0b
 * mnemonic: fxch
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp6_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0c
 * mnemonic: fxch
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp6_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0d
 * mnemonic: fxch
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp6_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0e
 * mnemonic: fxch
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp6_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0f
 * mnemonic: fxch
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp6_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 10
 * mnemonic: fst
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp6_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 11
 * mnemonic: fst
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp6_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 12
 * mnemonic: fst
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp6_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 13
 * mnemonic: fst
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp6_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 14
 * mnemonic: fst
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp6_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 15
 * mnemonic: fst
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp6_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 16
 * mnemonic: fst
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp6_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 17
 * mnemonic: fst
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp6_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (!Operand__ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 18
 * mnemonic: fstp
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp6_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 19
 * mnemonic: fstp
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp6_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1a
 * mnemonic: fstp
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp6_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1b
 * mnemonic: fstp
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp6_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1c
 * mnemonic: fstp
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp6_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1d
 * mnemonic: fstp
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp6_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1e
 * mnemonic: fstp
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp6_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1f
 * mnemonic: fstp
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp6_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (!Operand__ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 20
 * mnemonic: fucom
 * operand: ['ST0', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 21
 * mnemonic: fucom
 * operand: ['ST1', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 22
 * mnemonic: fucom
 * operand: ['ST2', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 23
 * mnemonic: fucom
 * operand: ['ST3', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 24
 * mnemonic: fucom
 * operand: ['ST4', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 25
 * mnemonic: fucom
 * operand: ['ST5', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 26
 * mnemonic: fucom
 * operand: ['ST6', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 27
 * mnemonic: fucom
 * operand: ['ST7', 'ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 28
 * mnemonic: fucomp
 * operand: ['ST0']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 29
 * mnemonic: fucomp
 * operand: ['ST1']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 2a
 * mnemonic: fucomp
 * operand: ['ST2']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 2b
 * mnemonic: fucomp
 * operand: ['ST3']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 2c
 * mnemonic: fucomp
 * operand: ['ST4']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 2d
 * mnemonic: fucomp
 * operand: ['ST5']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 2e
 * mnemonic: fucomp
 * operand: ['ST6']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 2f
 * mnemonic: fucomp
 * operand: ['ST7']
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (!Operand__ST7(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 30
 * invalid
**/
bool X86Architecture::Table_fp6_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 31
 * invalid
**/
bool X86Architecture::Table_fp6_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 32
 * invalid
**/
bool X86Architecture::Table_fp6_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 33
 * invalid
**/
bool X86Architecture::Table_fp6_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 34
 * invalid
**/
bool X86Architecture::Table_fp6_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 35
 * invalid
**/
bool X86Architecture::Table_fp6_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 36
 * invalid
**/
bool X86Architecture::Table_fp6_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 37
 * invalid
**/
bool X86Architecture::Table_fp6_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 38
 * invalid
**/
bool X86Architecture::Table_fp6_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 39
 * invalid
**/
bool X86Architecture::Table_fp6_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3a
 * invalid
**/
bool X86Architecture::Table_fp6_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3b
 * invalid
**/
bool X86Architecture::Table_fp6_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instructions
 * opcode: 3c
 *
 * invalid
 *
 * mnemonic: frichop
 * attr: ['cyrix']
 *
**/
bool X86Architecture::Table_fp6_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_CYRIX)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Frichop);
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instruction
 * opcode: 3d
 * invalid
**/
bool X86Architecture::Table_fp6_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3e
 * invalid
**/
bool X86Architecture::Table_fp6_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3f
 * invalid
**/
bool X86Architecture::Table_fp6_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

