/* This file has been automatically generated, you must _NOT_ edit it directly. (Wed Oct  5 22:56:59 2016) */
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
 * mnemonic: fadd
 * operand: ['ST0', 'ST0']
 * opcode: 00
**/
bool X86Architecture::Table_fp5_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST1', 'ST0']
 * opcode: 01
**/
bool X86Architecture::Table_fp5_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST2', 'ST0']
 * opcode: 02
**/
bool X86Architecture::Table_fp5_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST3', 'ST0']
 * opcode: 03
**/
bool X86Architecture::Table_fp5_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST4', 'ST0']
 * opcode: 04
**/
bool X86Architecture::Table_fp5_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST5', 'ST0']
 * opcode: 05
**/
bool X86Architecture::Table_fp5_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST6', 'ST0']
 * opcode: 06
**/
bool X86Architecture::Table_fp5_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST7', 'ST0']
 * opcode: 07
**/
bool X86Architecture::Table_fp5_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST0']
 * opcode: 08
**/
bool X86Architecture::Table_fp5_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST1', 'ST0']
 * opcode: 09
**/
bool X86Architecture::Table_fp5_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST2', 'ST0']
 * opcode: 0a
**/
bool X86Architecture::Table_fp5_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST3', 'ST0']
 * opcode: 0b
**/
bool X86Architecture::Table_fp5_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST4', 'ST0']
 * opcode: 0c
**/
bool X86Architecture::Table_fp5_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST5', 'ST0']
 * opcode: 0d
**/
bool X86Architecture::Table_fp5_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST6', 'ST0']
 * opcode: 0e
**/
bool X86Architecture::Table_fp5_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST7', 'ST0']
 * opcode: 0f
**/
bool X86Architecture::Table_fp5_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0']
 * opcode: 10
**/
bool X86Architecture::Table_fp5_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST1']
 * opcode: 11
**/
bool X86Architecture::Table_fp5_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST2']
 * opcode: 12
**/
bool X86Architecture::Table_fp5_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST3']
 * opcode: 13
**/
bool X86Architecture::Table_fp5_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST4']
 * opcode: 14
**/
bool X86Architecture::Table_fp5_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST5']
 * opcode: 15
**/
bool X86Architecture::Table_fp5_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST6']
 * opcode: 16
**/
bool X86Architecture::Table_fp5_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST7']
 * opcode: 17
**/
bool X86Architecture::Table_fp5_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0']
 * opcode: 18
**/
bool X86Architecture::Table_fp5_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST1']
 * opcode: 19
**/
bool X86Architecture::Table_fp5_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST2']
 * opcode: 1a
**/
bool X86Architecture::Table_fp5_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST3']
 * opcode: 1b
**/
bool X86Architecture::Table_fp5_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST4']
 * opcode: 1c
**/
bool X86Architecture::Table_fp5_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST5']
 * opcode: 1d
**/
bool X86Architecture::Table_fp5_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST6']
 * opcode: 1e
**/
bool X86Architecture::Table_fp5_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST7']
 * opcode: 1f
**/
bool X86Architecture::Table_fp5_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST0']
 * opcode: 20
**/
bool X86Architecture::Table_fp5_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST1', 'ST0']
 * opcode: 21
**/
bool X86Architecture::Table_fp5_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST2', 'ST0']
 * opcode: 22
**/
bool X86Architecture::Table_fp5_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST3', 'ST0']
 * opcode: 23
**/
bool X86Architecture::Table_fp5_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST4', 'ST0']
 * opcode: 24
**/
bool X86Architecture::Table_fp5_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST5', 'ST0']
 * opcode: 25
**/
bool X86Architecture::Table_fp5_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST6', 'ST0']
 * opcode: 26
**/
bool X86Architecture::Table_fp5_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST7', 'ST0']
 * opcode: 27
**/
bool X86Architecture::Table_fp5_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST0']
 * opcode: 28
**/
bool X86Architecture::Table_fp5_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST1', 'ST0']
 * opcode: 29
**/
bool X86Architecture::Table_fp5_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST2', 'ST0']
 * opcode: 2a
**/
bool X86Architecture::Table_fp5_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST3', 'ST0']
 * opcode: 2b
**/
bool X86Architecture::Table_fp5_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST4', 'ST0']
 * opcode: 2c
**/
bool X86Architecture::Table_fp5_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST5', 'ST0']
 * opcode: 2d
**/
bool X86Architecture::Table_fp5_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST6', 'ST0']
 * opcode: 2e
**/
bool X86Architecture::Table_fp5_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST7', 'ST0']
 * opcode: 2f
**/
bool X86Architecture::Table_fp5_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST0']
 * opcode: 30
**/
bool X86Architecture::Table_fp5_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST1', 'ST0']
 * opcode: 31
**/
bool X86Architecture::Table_fp5_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST2', 'ST0']
 * opcode: 32
**/
bool X86Architecture::Table_fp5_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST3', 'ST0']
 * opcode: 33
**/
bool X86Architecture::Table_fp5_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST4', 'ST0']
 * opcode: 34
**/
bool X86Architecture::Table_fp5_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST5', 'ST0']
 * opcode: 35
**/
bool X86Architecture::Table_fp5_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST6', 'ST0']
 * opcode: 36
**/
bool X86Architecture::Table_fp5_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST7', 'ST0']
 * opcode: 37
**/
bool X86Architecture::Table_fp5_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST0']
 * opcode: 38
**/
bool X86Architecture::Table_fp5_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST1', 'ST0']
 * opcode: 39
**/
bool X86Architecture::Table_fp5_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST2', 'ST0']
 * opcode: 3a
**/
bool X86Architecture::Table_fp5_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST3', 'ST0']
 * opcode: 3b
**/
bool X86Architecture::Table_fp5_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST4', 'ST0']
 * opcode: 3c
**/
bool X86Architecture::Table_fp5_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST5', 'ST0']
 * opcode: 3d
**/
bool X86Architecture::Table_fp5_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST6', 'ST0']
 * opcode: 3e
**/
bool X86Architecture::Table_fp5_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST7', 'ST0']
 * opcode: 3f
**/
bool X86Architecture::Table_fp5_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

