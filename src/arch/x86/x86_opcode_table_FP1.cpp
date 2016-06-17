/* This file has been automatically generated, you must _NOT_ edit it directly. (Fri Jun 17 18:00:43 2016) */
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
 * mnemonic: fadd
 * operand: ['ST0', 'ST0']
 * opcode: 00
**/
bool X86Architecture::Table_fp1_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST0', 'ST1']
 * opcode: 01
**/
bool X86Architecture::Table_fp1_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST0', 'ST2']
 * opcode: 02
**/
bool X86Architecture::Table_fp1_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST0', 'ST3']
 * opcode: 03
**/
bool X86Architecture::Table_fp1_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST0', 'ST4']
 * opcode: 04
**/
bool X86Architecture::Table_fp1_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST0', 'ST5']
 * opcode: 05
**/
bool X86Architecture::Table_fp1_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST0', 'ST6']
 * opcode: 06
**/
bool X86Architecture::Table_fp1_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fadd
 * operand: ['ST0', 'ST7']
 * opcode: 07
**/
bool X86Architecture::Table_fp1_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fadd);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
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
bool X86Architecture::Table_fp1_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST1']
 * opcode: 09
**/
bool X86Architecture::Table_fp1_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST2']
 * opcode: 0a
**/
bool X86Architecture::Table_fp1_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST3']
 * opcode: 0b
**/
bool X86Architecture::Table_fp1_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST4']
 * opcode: 0c
**/
bool X86Architecture::Table_fp1_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST5']
 * opcode: 0d
**/
bool X86Architecture::Table_fp1_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST6']
 * opcode: 0e
**/
bool X86Architecture::Table_fp1_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmul
 * operand: ['ST0', 'ST7']
 * opcode: 0f
**/
bool X86Architecture::Table_fp1_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmul);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST0']
 * opcode: 10
**/
bool X86Architecture::Table_fp1_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST1']
 * opcode: 11
**/
bool X86Architecture::Table_fp1_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST2']
 * opcode: 12
**/
bool X86Architecture::Table_fp1_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST3']
 * opcode: 13
**/
bool X86Architecture::Table_fp1_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST4']
 * opcode: 14
**/
bool X86Architecture::Table_fp1_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST5']
 * opcode: 15
**/
bool X86Architecture::Table_fp1_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST6']
 * opcode: 16
**/
bool X86Architecture::Table_fp1_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcom
 * operand: ['ST0', 'ST7']
 * opcode: 17
**/
bool X86Architecture::Table_fp1_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcom);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST0']
 * opcode: 18
**/
bool X86Architecture::Table_fp1_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST1']
 * opcode: 19
**/
bool X86Architecture::Table_fp1_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST2']
 * opcode: 1a
**/
bool X86Architecture::Table_fp1_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST3']
 * opcode: 1b
**/
bool X86Architecture::Table_fp1_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST4']
 * opcode: 1c
**/
bool X86Architecture::Table_fp1_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST5']
 * opcode: 1d
**/
bool X86Architecture::Table_fp1_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST6']
 * opcode: 1e
**/
bool X86Architecture::Table_fp1_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0', 'ST7']
 * opcode: 1f
**/
bool X86Architecture::Table_fp1_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST0']
 * opcode: 20
**/
bool X86Architecture::Table_fp1_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST1']
 * opcode: 21
**/
bool X86Architecture::Table_fp1_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST2']
 * opcode: 22
**/
bool X86Architecture::Table_fp1_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST3']
 * opcode: 23
**/
bool X86Architecture::Table_fp1_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST4']
 * opcode: 24
**/
bool X86Architecture::Table_fp1_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST5']
 * opcode: 25
**/
bool X86Architecture::Table_fp1_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST6']
 * opcode: 26
**/
bool X86Architecture::Table_fp1_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsub
 * operand: ['ST0', 'ST7']
 * opcode: 27
**/
bool X86Architecture::Table_fp1_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsub);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST0']
 * opcode: 28
**/
bool X86Architecture::Table_fp1_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST1']
 * opcode: 29
**/
bool X86Architecture::Table_fp1_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST2']
 * opcode: 2a
**/
bool X86Architecture::Table_fp1_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST3']
 * opcode: 2b
**/
bool X86Architecture::Table_fp1_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST4']
 * opcode: 2c
**/
bool X86Architecture::Table_fp1_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST5']
 * opcode: 2d
**/
bool X86Architecture::Table_fp1_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST6']
 * opcode: 2e
**/
bool X86Architecture::Table_fp1_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubr
 * operand: ['ST0', 'ST7']
 * opcode: 2f
**/
bool X86Architecture::Table_fp1_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubr);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST0']
 * opcode: 30
**/
bool X86Architecture::Table_fp1_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST1']
 * opcode: 31
**/
bool X86Architecture::Table_fp1_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST2']
 * opcode: 32
**/
bool X86Architecture::Table_fp1_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST3']
 * opcode: 33
**/
bool X86Architecture::Table_fp1_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST4']
 * opcode: 34
**/
bool X86Architecture::Table_fp1_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST5']
 * opcode: 35
**/
bool X86Architecture::Table_fp1_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST6']
 * opcode: 36
**/
bool X86Architecture::Table_fp1_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdiv
 * operand: ['ST0', 'ST7']
 * opcode: 37
**/
bool X86Architecture::Table_fp1_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdiv);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST0']
 * opcode: 38
**/
bool X86Architecture::Table_fp1_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST1']
 * opcode: 39
**/
bool X86Architecture::Table_fp1_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST2']
 * opcode: 3a
**/
bool X86Architecture::Table_fp1_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST3']
 * opcode: 3b
**/
bool X86Architecture::Table_fp1_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST4']
 * opcode: 3c
**/
bool X86Architecture::Table_fp1_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST5']
 * opcode: 3d
**/
bool X86Architecture::Table_fp1_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST6']
 * opcode: 3e
**/
bool X86Architecture::Table_fp1_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivr
 * operand: ['ST0', 'ST7']
 * opcode: 3f
**/
bool X86Architecture::Table_fp1_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivr);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

