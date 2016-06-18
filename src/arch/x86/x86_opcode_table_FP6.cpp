/* This file has been automatically generated, you must _NOT_ edit it directly. (Sat Jun 18 16:42:30 2016) */
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
 * mnemonic: ffree
 * operand: ['ST0']
 * opcode: 00
**/
bool X86Architecture::Table_fp6_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ffree
 * operand: ['ST1']
 * opcode: 01
**/
bool X86Architecture::Table_fp6_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ffree
 * operand: ['ST2']
 * opcode: 02
**/
bool X86Architecture::Table_fp6_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ffree
 * operand: ['ST3']
 * opcode: 03
**/
bool X86Architecture::Table_fp6_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ffree
 * operand: ['ST4']
 * opcode: 04
**/
bool X86Architecture::Table_fp6_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ffree
 * operand: ['ST5']
 * opcode: 05
**/
bool X86Architecture::Table_fp6_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ffree
 * operand: ['ST6']
 * opcode: 06
**/
bool X86Architecture::Table_fp6_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ffree
 * operand: ['ST7']
 * opcode: 07
**/
bool X86Architecture::Table_fp6_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ffree);
    if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0']
 * opcode: 08
**/
bool X86Architecture::Table_fp6_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST1']
 * opcode: 09
**/
bool X86Architecture::Table_fp6_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST2']
 * opcode: 0a
**/
bool X86Architecture::Table_fp6_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST3']
 * opcode: 0b
**/
bool X86Architecture::Table_fp6_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST4']
 * opcode: 0c
**/
bool X86Architecture::Table_fp6_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST5']
 * opcode: 0d
**/
bool X86Architecture::Table_fp6_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST6']
 * opcode: 0e
**/
bool X86Architecture::Table_fp6_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST7']
 * opcode: 0f
**/
bool X86Architecture::Table_fp6_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST0']
 * opcode: 10
**/
bool X86Architecture::Table_fp6_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST1']
 * opcode: 11
**/
bool X86Architecture::Table_fp6_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST2']
 * opcode: 12
**/
bool X86Architecture::Table_fp6_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST3']
 * opcode: 13
**/
bool X86Architecture::Table_fp6_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST4']
 * opcode: 14
**/
bool X86Architecture::Table_fp6_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST5']
 * opcode: 15
**/
bool X86Architecture::Table_fp6_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST6']
 * opcode: 16
**/
bool X86Architecture::Table_fp6_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fst
 * operand: ['ST7']
 * opcode: 17
**/
bool X86Architecture::Table_fp6_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fst);
    if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST0']
 * opcode: 18
**/
bool X86Architecture::Table_fp6_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST1']
 * opcode: 19
**/
bool X86Architecture::Table_fp6_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST2']
 * opcode: 1a
**/
bool X86Architecture::Table_fp6_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST3']
 * opcode: 1b
**/
bool X86Architecture::Table_fp6_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST4']
 * opcode: 1c
**/
bool X86Architecture::Table_fp6_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST5']
 * opcode: 1d
**/
bool X86Architecture::Table_fp6_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST6']
 * opcode: 1e
**/
bool X86Architecture::Table_fp6_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST7']
 * opcode: 1f
**/
bool X86Architecture::Table_fp6_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fstp);
    if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST0', 'ST0']
 * opcode: 20
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST1', 'ST0']
 * opcode: 21
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST2', 'ST0']
 * opcode: 22
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST3', 'ST0']
 * opcode: 23
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST4', 'ST0']
 * opcode: 24
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST5', 'ST0']
 * opcode: 25
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST6', 'ST0']
 * opcode: 26
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucom
 * operand: ['ST7', 'ST0']
 * opcode: 27
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucom);
      if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST0']
 * opcode: 28
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST0(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST1']
 * opcode: 29
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST1(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST2']
 * opcode: 2a
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST2(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST3']
 * opcode: 2b
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST3(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST4']
 * opcode: 2c
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST4(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST5']
 * opcode: 2d
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST5(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST6']
 * opcode: 2e
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST6(rBinStrm, Offset, rInsn, Mode) == false)
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fucomp
 * operand: ['ST7']
 * opcode: 2f
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp6_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fucomp);
      if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
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
bool X86Architecture::Table_fp6_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 31
 * invalid
**/
bool X86Architecture::Table_fp6_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 32
 * invalid
**/
bool X86Architecture::Table_fp6_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 33
 * invalid
**/
bool X86Architecture::Table_fp6_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 34
 * invalid
**/
bool X86Architecture::Table_fp6_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 35
 * invalid
**/
bool X86Architecture::Table_fp6_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 36
 * invalid
**/
bool X86Architecture::Table_fp6_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 37
 * invalid
**/
bool X86Architecture::Table_fp6_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 38
 * invalid
**/
bool X86Architecture::Table_fp6_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 39
 * invalid
**/
bool X86Architecture::Table_fp6_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3a
 * invalid
**/
bool X86Architecture::Table_fp6_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3b
 * invalid
**/
bool X86Architecture::Table_fp6_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp6_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_CYRIX)
    {
      rInsn.Length()++;
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
bool X86Architecture::Table_fp6_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3e
 * invalid
**/
bool X86Architecture::Table_fp6_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3f
 * invalid
**/
bool X86Architecture::Table_fp6_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

