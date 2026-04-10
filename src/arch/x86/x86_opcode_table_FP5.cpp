/* This file has been automatically generated, you must _NOT_ edit it directly. (Thu Apr  9 18:19:39 2026) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp5[0x40] =
{
  &X86Architecture::Table_fp5_c0,
  &X86Architecture::Table_fp5_c1,
  &X86Architecture::Table_fp5_c2,
  &X86Architecture::Table_fp5_c3,
  &X86Architecture::Table_fp5_c4,
  &X86Architecture::Table_fp5_c5,
  &X86Architecture::Table_fp5_c6,
  &X86Architecture::Table_fp5_c7,
  &X86Architecture::Table_fp5_c8,
  &X86Architecture::Table_fp5_c9,
  &X86Architecture::Table_fp5_ca,
  &X86Architecture::Table_fp5_cb,
  &X86Architecture::Table_fp5_cc,
  &X86Architecture::Table_fp5_cd,
  &X86Architecture::Table_fp5_ce,
  &X86Architecture::Table_fp5_cf,
  &X86Architecture::Table_fp5_d0,
  &X86Architecture::Table_fp5_d1,
  &X86Architecture::Table_fp5_d2,
  &X86Architecture::Table_fp5_d3,
  &X86Architecture::Table_fp5_d4,
  &X86Architecture::Table_fp5_d5,
  &X86Architecture::Table_fp5_d6,
  &X86Architecture::Table_fp5_d7,
  &X86Architecture::Table_fp5_d8,
  &X86Architecture::Table_fp5_d9,
  &X86Architecture::Table_fp5_da,
  &X86Architecture::Table_fp5_db,
  &X86Architecture::Table_fp5_dc,
  &X86Architecture::Table_fp5_dd,
  &X86Architecture::Table_fp5_de,
  &X86Architecture::Table_fp5_df,
  &X86Architecture::Table_fp5_e0,
  &X86Architecture::Table_fp5_e1,
  &X86Architecture::Table_fp5_e2,
  &X86Architecture::Table_fp5_e3,
  &X86Architecture::Table_fp5_e4,
  &X86Architecture::Table_fp5_e5,
  &X86Architecture::Table_fp5_e6,
  &X86Architecture::Table_fp5_e7,
  &X86Architecture::Table_fp5_e8,
  &X86Architecture::Table_fp5_e9,
  &X86Architecture::Table_fp5_ea,
  &X86Architecture::Table_fp5_eb,
  &X86Architecture::Table_fp5_ec,
  &X86Architecture::Table_fp5_ed,
  &X86Architecture::Table_fp5_ee,
  &X86Architecture::Table_fp5_ef,
  &X86Architecture::Table_fp5_f0,
  &X86Architecture::Table_fp5_f1,
  &X86Architecture::Table_fp5_f2,
  &X86Architecture::Table_fp5_f3,
  &X86Architecture::Table_fp5_f4,
  &X86Architecture::Table_fp5_f5,
  &X86Architecture::Table_fp5_f6,
  &X86Architecture::Table_fp5_f7,
  &X86Architecture::Table_fp5_f8,
  &X86Architecture::Table_fp5_f9,
  &X86Architecture::Table_fp5_fa,
  &X86Architecture::Table_fp5_fb,
  &X86Architecture::Table_fp5_fc,
  &X86Architecture::Table_fp5_fd,
  &X86Architecture::Table_fp5_fe,
  &X86Architecture::Table_fp5_ff
};

/** instruction
 * opcode: 00
 * mnemonic: fadd
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp5_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 01
 * mnemonic: fadd
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp5_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * mnemonic: fadd
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp5_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 03
 * mnemonic: fadd
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp5_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * mnemonic: fadd
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp5_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 05
 * mnemonic: fadd
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp5_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * mnemonic: fadd
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp5_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * mnemonic: fadd
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp5_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 08
 * mnemonic: fmul
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp5_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 09
 * mnemonic: fmul
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp5_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0a
 * mnemonic: fmul
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp5_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0b
 * mnemonic: fmul
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp5_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0c
 * mnemonic: fmul
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp5_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0d
 * mnemonic: fmul
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp5_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0e
 * mnemonic: fmul
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp5_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0f
 * mnemonic: fmul
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp5_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 10
 * mnemonic: fcom
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp5_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 11
 * mnemonic: fcom
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp5_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 12
 * mnemonic: fcom
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp5_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 13
 * mnemonic: fcom
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp5_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 14
 * mnemonic: fcom
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp5_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 15
 * mnemonic: fcom
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp5_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 16
 * mnemonic: fcom
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp5_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 17
 * mnemonic: fcom
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp5_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 18
 * mnemonic: fcomp
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp5_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 19
 * mnemonic: fcomp
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp5_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1a
 * mnemonic: fcomp
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp5_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1b
 * mnemonic: fcomp
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp5_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1c
 * mnemonic: fcomp
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp5_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1d
 * mnemonic: fcomp
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp5_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1e
 * mnemonic: fcomp
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp5_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1f
 * mnemonic: fcomp
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp5_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 20
 * mnemonic: fsubr
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp5_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 21
 * mnemonic: fsubr
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp5_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 22
 * mnemonic: fsubr
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp5_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 23
 * mnemonic: fsubr
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp5_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 24
 * mnemonic: fsubr
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp5_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 25
 * mnemonic: fsubr
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp5_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 26
 * mnemonic: fsubr
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp5_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 27
 * mnemonic: fsubr
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp5_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 28
 * mnemonic: fsub
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp5_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 29
 * mnemonic: fsub
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp5_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2a
 * mnemonic: fsub
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp5_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2b
 * mnemonic: fsub
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp5_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2c
 * mnemonic: fsub
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp5_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2d
 * mnemonic: fsub
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp5_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2e
 * mnemonic: fsub
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp5_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2f
 * mnemonic: fsub
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp5_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 30
 * mnemonic: fdivr
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp5_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 31
 * mnemonic: fdivr
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp5_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 32
 * mnemonic: fdivr
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp5_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 33
 * mnemonic: fdivr
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp5_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 34
 * mnemonic: fdivr
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp5_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 35
 * mnemonic: fdivr
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp5_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 36
 * mnemonic: fdivr
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp5_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 37
 * mnemonic: fdivr
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp5_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 38
 * mnemonic: fdiv
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp5_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 39
 * mnemonic: fdiv
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp5_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3a
 * mnemonic: fdiv
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp5_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3b
 * mnemonic: fdiv
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp5_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3c
 * mnemonic: fdiv
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp5_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3d
 * mnemonic: fdiv
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp5_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3e
 * mnemonic: fdiv
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp5_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3f
 * mnemonic: fdiv
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp5_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

