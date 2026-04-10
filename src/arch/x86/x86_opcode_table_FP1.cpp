/* This file has been automatically generated, you must _NOT_ edit it directly. (Thu Apr  9 18:19:39 2026) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp1[0x40] =
{
  &X86Architecture::Table_fp1_c0,
  &X86Architecture::Table_fp1_c1,
  &X86Architecture::Table_fp1_c2,
  &X86Architecture::Table_fp1_c3,
  &X86Architecture::Table_fp1_c4,
  &X86Architecture::Table_fp1_c5,
  &X86Architecture::Table_fp1_c6,
  &X86Architecture::Table_fp1_c7,
  &X86Architecture::Table_fp1_c8,
  &X86Architecture::Table_fp1_c9,
  &X86Architecture::Table_fp1_ca,
  &X86Architecture::Table_fp1_cb,
  &X86Architecture::Table_fp1_cc,
  &X86Architecture::Table_fp1_cd,
  &X86Architecture::Table_fp1_ce,
  &X86Architecture::Table_fp1_cf,
  &X86Architecture::Table_fp1_d0,
  &X86Architecture::Table_fp1_d1,
  &X86Architecture::Table_fp1_d2,
  &X86Architecture::Table_fp1_d3,
  &X86Architecture::Table_fp1_d4,
  &X86Architecture::Table_fp1_d5,
  &X86Architecture::Table_fp1_d6,
  &X86Architecture::Table_fp1_d7,
  &X86Architecture::Table_fp1_d8,
  &X86Architecture::Table_fp1_d9,
  &X86Architecture::Table_fp1_da,
  &X86Architecture::Table_fp1_db,
  &X86Architecture::Table_fp1_dc,
  &X86Architecture::Table_fp1_dd,
  &X86Architecture::Table_fp1_de,
  &X86Architecture::Table_fp1_df,
  &X86Architecture::Table_fp1_e0,
  &X86Architecture::Table_fp1_e1,
  &X86Architecture::Table_fp1_e2,
  &X86Architecture::Table_fp1_e3,
  &X86Architecture::Table_fp1_e4,
  &X86Architecture::Table_fp1_e5,
  &X86Architecture::Table_fp1_e6,
  &X86Architecture::Table_fp1_e7,
  &X86Architecture::Table_fp1_e8,
  &X86Architecture::Table_fp1_e9,
  &X86Architecture::Table_fp1_ea,
  &X86Architecture::Table_fp1_eb,
  &X86Architecture::Table_fp1_ec,
  &X86Architecture::Table_fp1_ed,
  &X86Architecture::Table_fp1_ee,
  &X86Architecture::Table_fp1_ef,
  &X86Architecture::Table_fp1_f0,
  &X86Architecture::Table_fp1_f1,
  &X86Architecture::Table_fp1_f2,
  &X86Architecture::Table_fp1_f3,
  &X86Architecture::Table_fp1_f4,
  &X86Architecture::Table_fp1_f5,
  &X86Architecture::Table_fp1_f6,
  &X86Architecture::Table_fp1_f7,
  &X86Architecture::Table_fp1_f8,
  &X86Architecture::Table_fp1_f9,
  &X86Architecture::Table_fp1_fa,
  &X86Architecture::Table_fp1_fb,
  &X86Architecture::Table_fp1_fc,
  &X86Architecture::Table_fp1_fd,
  &X86Architecture::Table_fp1_fe,
  &X86Architecture::Table_fp1_ff
};

/** instruction
 * opcode: 00
 * mnemonic: fadd
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp1_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * mnemonic: fadd
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 03
 * mnemonic: fadd
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * mnemonic: fadd
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 05
 * mnemonic: fadd
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * mnemonic: fadd
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * mnemonic: fadd
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
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
bool X86Architecture::Table_fp1_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0a
 * mnemonic: fmul
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0b
 * mnemonic: fmul
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0c
 * mnemonic: fmul
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0d
 * mnemonic: fmul
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0e
 * mnemonic: fmul
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0f
 * mnemonic: fmul
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 10
 * mnemonic: fcom
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp1_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 11
 * mnemonic: fcom
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 12
 * mnemonic: fcom
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 13
 * mnemonic: fcom
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 14
 * mnemonic: fcom
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 15
 * mnemonic: fcom
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 16
 * mnemonic: fcom
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 17
 * mnemonic: fcom
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 18
 * mnemonic: fcomp
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp1_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 19
 * mnemonic: fcomp
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1a
 * mnemonic: fcomp
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1b
 * mnemonic: fcomp
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1c
 * mnemonic: fcomp
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1d
 * mnemonic: fcomp
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1e
 * mnemonic: fcomp
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 1f
 * mnemonic: fcomp
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 20
 * mnemonic: fsub
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp1_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 21
 * mnemonic: fsub
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 22
 * mnemonic: fsub
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 23
 * mnemonic: fsub
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 24
 * mnemonic: fsub
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 25
 * mnemonic: fsub
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 26
 * mnemonic: fsub
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 27
 * mnemonic: fsub
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 28
 * mnemonic: fsubr
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp1_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 29
 * mnemonic: fsubr
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2a
 * mnemonic: fsubr
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2b
 * mnemonic: fsubr
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2c
 * mnemonic: fsubr
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2d
 * mnemonic: fsubr
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2e
 * mnemonic: fsubr
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2f
 * mnemonic: fsubr
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 30
 * mnemonic: fdiv
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp1_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 31
 * mnemonic: fdiv
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 32
 * mnemonic: fdiv
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 33
 * mnemonic: fdiv
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 34
 * mnemonic: fdiv
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 35
 * mnemonic: fdiv
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 36
 * mnemonic: fdiv
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 37
 * mnemonic: fdiv
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 38
 * mnemonic: fdivr
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp1_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 39
 * mnemonic: fdivr
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp1_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3a
 * mnemonic: fdivr
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp1_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3b
 * mnemonic: fdivr
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp1_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3c
 * mnemonic: fdivr
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp1_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3d
 * mnemonic: fdivr
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp1_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3e
 * mnemonic: fdivr
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp1_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3f
 * mnemonic: fdivr
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp1_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

