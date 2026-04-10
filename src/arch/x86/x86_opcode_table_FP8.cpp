/* This file has been automatically generated, you must _NOT_ edit it directly. (Thu Apr  9 18:19:39 2026) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp8[0x40] =
{
  &X86Architecture::Table_fp8_c0,
  &X86Architecture::Table_fp8_c1,
  &X86Architecture::Table_fp8_c2,
  &X86Architecture::Table_fp8_c3,
  &X86Architecture::Table_fp8_c4,
  &X86Architecture::Table_fp8_c5,
  &X86Architecture::Table_fp8_c6,
  &X86Architecture::Table_fp8_c7,
  &X86Architecture::Table_fp8_c8,
  &X86Architecture::Table_fp8_c9,
  &X86Architecture::Table_fp8_ca,
  &X86Architecture::Table_fp8_cb,
  &X86Architecture::Table_fp8_cc,
  &X86Architecture::Table_fp8_cd,
  &X86Architecture::Table_fp8_ce,
  &X86Architecture::Table_fp8_cf,
  &X86Architecture::Table_fp8_d0,
  &X86Architecture::Table_fp8_d1,
  &X86Architecture::Table_fp8_d2,
  &X86Architecture::Table_fp8_d3,
  &X86Architecture::Table_fp8_d4,
  &X86Architecture::Table_fp8_d5,
  &X86Architecture::Table_fp8_d6,
  &X86Architecture::Table_fp8_d7,
  &X86Architecture::Table_fp8_d8,
  &X86Architecture::Table_fp8_d9,
  &X86Architecture::Table_fp8_da,
  &X86Architecture::Table_fp8_db,
  &X86Architecture::Table_fp8_dc,
  &X86Architecture::Table_fp8_dd,
  &X86Architecture::Table_fp8_de,
  &X86Architecture::Table_fp8_df,
  &X86Architecture::Table_fp8_e0,
  &X86Architecture::Table_fp8_e1,
  &X86Architecture::Table_fp8_e2,
  &X86Architecture::Table_fp8_e3,
  &X86Architecture::Table_fp8_e4,
  &X86Architecture::Table_fp8_e5,
  &X86Architecture::Table_fp8_e6,
  &X86Architecture::Table_fp8_e7,
  &X86Architecture::Table_fp8_e8,
  &X86Architecture::Table_fp8_e9,
  &X86Architecture::Table_fp8_ea,
  &X86Architecture::Table_fp8_eb,
  &X86Architecture::Table_fp8_ec,
  &X86Architecture::Table_fp8_ed,
  &X86Architecture::Table_fp8_ee,
  &X86Architecture::Table_fp8_ef,
  &X86Architecture::Table_fp8_f0,
  &X86Architecture::Table_fp8_f1,
  &X86Architecture::Table_fp8_f2,
  &X86Architecture::Table_fp8_f3,
  &X86Architecture::Table_fp8_f4,
  &X86Architecture::Table_fp8_f5,
  &X86Architecture::Table_fp8_f6,
  &X86Architecture::Table_fp8_f7,
  &X86Architecture::Table_fp8_f8,
  &X86Architecture::Table_fp8_f9,
  &X86Architecture::Table_fp8_fa,
  &X86Architecture::Table_fp8_fb,
  &X86Architecture::Table_fp8_fc,
  &X86Architecture::Table_fp8_fd,
  &X86Architecture::Table_fp8_fe,
  &X86Architecture::Table_fp8_ff
};

/** instruction
 * opcode: 00
 * mnemonic: ffreep
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp8_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
    if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 01
 * mnemonic: ffreep
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp8_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
    if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * mnemonic: ffreep
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp8_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
    if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 03
 * mnemonic: ffreep
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp8_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
    if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * mnemonic: ffreep
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp8_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
    if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 05
 * mnemonic: ffreep
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp8_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
    if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * mnemonic: ffreep
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp8_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
    if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * mnemonic: ffreep
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp8_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ffreep);
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
bool X86Architecture::Table_fp8_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * mnemonic: fstp
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp8_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 11
 * mnemonic: fstp
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp8_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 12
 * mnemonic: fstp
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp8_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 13
 * mnemonic: fstp
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp8_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 14
 * mnemonic: fstp
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp8_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 15
 * mnemonic: fstp
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp8_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 16
 * mnemonic: fstp
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp8_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 17
 * mnemonic: fstp
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp8_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 18
 * mnemonic: fstp
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp8_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp8_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * mnemonic: fnstsw
 * operand: ['AX']
 * cpu_model: >= X86_Arch_80287
**/
bool X86Architecture::Table_fp8_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80287)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fnstsw);
      if (!Operand__AX(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
      return false;
}

/** instructions
 * opcode: 21
 *
 * invalid
 *
 * mnemonic: fstdw
 * operand: ['AX']
 * cpu_model: == X86_Arch_80387sl
 *
**/
bool X86Architecture::Table_fp8_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") == X86_Arch_80387sl)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fstdw);
      if (!Operand__AX(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instructions
 * opcode: 22
 *
 * invalid
 *
 * mnemonic: fstsg
 * operand: ['AX']
 * cpu_model: == X86_Arch_80387sl
 *
**/
bool X86Architecture::Table_fp8_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") == X86_Arch_80387sl)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fstsg);
      if (!Operand__AX(rBinStrm, Offset, rInsn, Mode))
      {
        return false;
      }
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instruction
 * opcode: 23
 * invalid
**/
bool X86Architecture::Table_fp8_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 24
 * invalid
**/
bool X86Architecture::Table_fp8_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 25
 * invalid
**/
bool X86Architecture::Table_fp8_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 26
 * invalid
**/
bool X86Architecture::Table_fp8_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 27
 * invalid
**/
bool X86Architecture::Table_fp8_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 28
 * mnemonic: fucomip
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp8_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 29
 * mnemonic: fucomip
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp8_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2a
 * mnemonic: fucomip
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp8_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2b
 * mnemonic: fucomip
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp8_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2c
 * mnemonic: fucomip
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp8_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2d
 * mnemonic: fucomip
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp8_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2e
 * mnemonic: fucomip
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp8_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2f
 * mnemonic: fucomip
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp8_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fucomip);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 30
 * mnemonic: fcomip
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp8_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 31
 * mnemonic: fcomip
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp8_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 32
 * mnemonic: fcomip
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp8_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 33
 * mnemonic: fcomip
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp8_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 34
 * mnemonic: fcomip
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp8_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 35
 * mnemonic: fcomip
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp8_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 36
 * mnemonic: fcomip
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp8_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 37
 * mnemonic: fcomip
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp8_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomip);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 38
 * invalid
**/
bool X86Architecture::Table_fp8_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 39
 * invalid
**/
bool X86Architecture::Table_fp8_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3a
 * invalid
**/
bool X86Architecture::Table_fp8_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3b
 * invalid
**/
bool X86Architecture::Table_fp8_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instructions
 * opcode: 3c
 *
 * invalid
 *
 * mnemonic: frinear
 * attr: ['cyrix']
 *
**/
bool X86Architecture::Table_fp8_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_CYRIX)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Frinear);
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
bool X86Architecture::Table_fp8_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3e
 * invalid
**/
bool X86Architecture::Table_fp8_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3f
 * invalid
**/
bool X86Architecture::Table_fp8_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

