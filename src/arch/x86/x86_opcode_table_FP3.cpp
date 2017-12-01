/* This file has been automatically generated, you must _NOT_ edit it directly. (Sun Feb 05 12:38:19 2017) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp3[0x40] =
{
  &X86Architecture::Table_fp3_c0,
  &X86Architecture::Table_fp3_c1,
  &X86Architecture::Table_fp3_c2,
  &X86Architecture::Table_fp3_c3,
  &X86Architecture::Table_fp3_c4,
  &X86Architecture::Table_fp3_c5,
  &X86Architecture::Table_fp3_c6,
  &X86Architecture::Table_fp3_c7,
  &X86Architecture::Table_fp3_c8,
  &X86Architecture::Table_fp3_c9,
  &X86Architecture::Table_fp3_ca,
  &X86Architecture::Table_fp3_cb,
  &X86Architecture::Table_fp3_cc,
  &X86Architecture::Table_fp3_cd,
  &X86Architecture::Table_fp3_ce,
  &X86Architecture::Table_fp3_cf,
  &X86Architecture::Table_fp3_d0,
  &X86Architecture::Table_fp3_d1,
  &X86Architecture::Table_fp3_d2,
  &X86Architecture::Table_fp3_d3,
  &X86Architecture::Table_fp3_d4,
  &X86Architecture::Table_fp3_d5,
  &X86Architecture::Table_fp3_d6,
  &X86Architecture::Table_fp3_d7,
  &X86Architecture::Table_fp3_d8,
  &X86Architecture::Table_fp3_d9,
  &X86Architecture::Table_fp3_da,
  &X86Architecture::Table_fp3_db,
  &X86Architecture::Table_fp3_dc,
  &X86Architecture::Table_fp3_dd,
  &X86Architecture::Table_fp3_de,
  &X86Architecture::Table_fp3_df,
  &X86Architecture::Table_fp3_e0,
  &X86Architecture::Table_fp3_e1,
  &X86Architecture::Table_fp3_e2,
  &X86Architecture::Table_fp3_e3,
  &X86Architecture::Table_fp3_e4,
  &X86Architecture::Table_fp3_e5,
  &X86Architecture::Table_fp3_e6,
  &X86Architecture::Table_fp3_e7,
  &X86Architecture::Table_fp3_e8,
  &X86Architecture::Table_fp3_e9,
  &X86Architecture::Table_fp3_ea,
  &X86Architecture::Table_fp3_eb,
  &X86Architecture::Table_fp3_ec,
  &X86Architecture::Table_fp3_ed,
  &X86Architecture::Table_fp3_ee,
  &X86Architecture::Table_fp3_ef,
  &X86Architecture::Table_fp3_f0,
  &X86Architecture::Table_fp3_f1,
  &X86Architecture::Table_fp3_f2,
  &X86Architecture::Table_fp3_f3,
  &X86Architecture::Table_fp3_f4,
  &X86Architecture::Table_fp3_f5,
  &X86Architecture::Table_fp3_f6,
  &X86Architecture::Table_fp3_f7,
  &X86Architecture::Table_fp3_f8,
  &X86Architecture::Table_fp3_f9,
  &X86Architecture::Table_fp3_fa,
  &X86Architecture::Table_fp3_fb,
  &X86Architecture::Table_fp3_fc,
  &X86Architecture::Table_fp3_fd,
  &X86Architecture::Table_fp3_fe,
  &X86Architecture::Table_fp3_ff
};

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST0']
 * opcode: 00
**/
bool X86Architecture::Table_fp3_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST1']
 * opcode: 01
**/
bool X86Architecture::Table_fp3_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST2']
 * opcode: 02
**/
bool X86Architecture::Table_fp3_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST3']
 * opcode: 03
**/
bool X86Architecture::Table_fp3_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST4']
 * opcode: 04
**/
bool X86Architecture::Table_fp3_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST5']
 * opcode: 05
**/
bool X86Architecture::Table_fp3_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST6']
 * opcode: 06
**/
bool X86Architecture::Table_fp3_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovb
 * operand: ['ST0', 'ST7']
 * opcode: 07
**/
bool X86Architecture::Table_fp3_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovb);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST0']
 * opcode: 08
**/
bool X86Architecture::Table_fp3_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST1']
 * opcode: 09
**/
bool X86Architecture::Table_fp3_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST2']
 * opcode: 0a
**/
bool X86Architecture::Table_fp3_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST3']
 * opcode: 0b
**/
bool X86Architecture::Table_fp3_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST4']
 * opcode: 0c
**/
bool X86Architecture::Table_fp3_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST5']
 * opcode: 0d
**/
bool X86Architecture::Table_fp3_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST6']
 * opcode: 0e
**/
bool X86Architecture::Table_fp3_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmove
 * operand: ['ST0', 'ST7']
 * opcode: 0f
**/
bool X86Architecture::Table_fp3_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmove);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST0']
 * opcode: 10
**/
bool X86Architecture::Table_fp3_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST1']
 * opcode: 11
**/
bool X86Architecture::Table_fp3_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST2']
 * opcode: 12
**/
bool X86Architecture::Table_fp3_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST3']
 * opcode: 13
**/
bool X86Architecture::Table_fp3_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST4']
 * opcode: 14
**/
bool X86Architecture::Table_fp3_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST5']
 * opcode: 15
**/
bool X86Architecture::Table_fp3_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST6']
 * opcode: 16
**/
bool X86Architecture::Table_fp3_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovbe
 * operand: ['ST0', 'ST7']
 * opcode: 17
**/
bool X86Architecture::Table_fp3_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovbe);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST0']
 * opcode: 18
**/
bool X86Architecture::Table_fp3_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST1']
 * opcode: 19
**/
bool X86Architecture::Table_fp3_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST2']
 * opcode: 1a
**/
bool X86Architecture::Table_fp3_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST3']
 * opcode: 1b
**/
bool X86Architecture::Table_fp3_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST4']
 * opcode: 1c
**/
bool X86Architecture::Table_fp3_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST5']
 * opcode: 1d
**/
bool X86Architecture::Table_fp3_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST6']
 * opcode: 1e
**/
bool X86Architecture::Table_fp3_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcmovu
 * operand: ['ST0', 'ST7']
 * opcode: 1f
**/
bool X86Architecture::Table_fp3_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcmovu);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 20
 * invalid
**/
bool X86Architecture::Table_fp3_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 21
 * invalid
**/
bool X86Architecture::Table_fp3_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 22
 * invalid
**/
bool X86Architecture::Table_fp3_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 23
 * invalid
**/
bool X86Architecture::Table_fp3_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 24
 * invalid
**/
bool X86Architecture::Table_fp3_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 25
 * invalid
**/
bool X86Architecture::Table_fp3_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 26
 * invalid
**/
bool X86Architecture::Table_fp3_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 27
 * invalid
**/
bool X86Architecture::Table_fp3_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 28
 * invalid
**/
bool X86Architecture::Table_fp3_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: fucompp
 * opcode: 29
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp3_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fucompp);
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 2a
 * invalid
**/
bool X86Architecture::Table_fp3_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 2b
 * invalid
**/
bool X86Architecture::Table_fp3_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 2c
 * invalid
**/
bool X86Architecture::Table_fp3_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 2d
 * invalid
**/
bool X86Architecture::Table_fp3_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 2e
 * invalid
**/
bool X86Architecture::Table_fp3_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 2f
 * invalid
**/
bool X86Architecture::Table_fp3_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 30
 * invalid
**/
bool X86Architecture::Table_fp3_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 31
 * invalid
**/
bool X86Architecture::Table_fp3_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 32
 * invalid
**/
bool X86Architecture::Table_fp3_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 33
 * invalid
**/
bool X86Architecture::Table_fp3_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 34
 * invalid
**/
bool X86Architecture::Table_fp3_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 35
 * invalid
**/
bool X86Architecture::Table_fp3_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 36
 * invalid
**/
bool X86Architecture::Table_fp3_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 37
 * invalid
**/
bool X86Architecture::Table_fp3_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 38
 * invalid
**/
bool X86Architecture::Table_fp3_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 39
 * invalid
**/
bool X86Architecture::Table_fp3_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3a
 * invalid
**/
bool X86Architecture::Table_fp3_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3b
 * invalid
**/
bool X86Architecture::Table_fp3_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3c
 * invalid
**/
bool X86Architecture::Table_fp3_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3d
 * invalid
**/
bool X86Architecture::Table_fp3_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3e
 * invalid
**/
bool X86Architecture::Table_fp3_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 3f
 * invalid
**/
bool X86Architecture::Table_fp3_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

