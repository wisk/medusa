/* This file has been automatically generated, you must _NOT_ edit it directly. (Fri Jun 17 18:00:43 2016) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp7[0x40] =
{
  &X86Architecture::Table_fp7_c0,
  &X86Architecture::Table_fp7_c1,
  &X86Architecture::Table_fp7_c2,
  &X86Architecture::Table_fp7_c3,
  &X86Architecture::Table_fp7_c4,
  &X86Architecture::Table_fp7_c5,
  &X86Architecture::Table_fp7_c6,
  &X86Architecture::Table_fp7_c7,
  &X86Architecture::Table_fp7_c8,
  &X86Architecture::Table_fp7_c9,
  &X86Architecture::Table_fp7_ca,
  &X86Architecture::Table_fp7_cb,
  &X86Architecture::Table_fp7_cc,
  &X86Architecture::Table_fp7_cd,
  &X86Architecture::Table_fp7_ce,
  &X86Architecture::Table_fp7_cf,
  &X86Architecture::Table_fp7_d0,
  &X86Architecture::Table_fp7_d1,
  &X86Architecture::Table_fp7_d2,
  &X86Architecture::Table_fp7_d3,
  &X86Architecture::Table_fp7_d4,
  &X86Architecture::Table_fp7_d5,
  &X86Architecture::Table_fp7_d6,
  &X86Architecture::Table_fp7_d7,
  &X86Architecture::Table_fp7_d8,
  &X86Architecture::Table_fp7_d9,
  &X86Architecture::Table_fp7_da,
  &X86Architecture::Table_fp7_db,
  &X86Architecture::Table_fp7_dc,
  &X86Architecture::Table_fp7_dd,
  &X86Architecture::Table_fp7_de,
  &X86Architecture::Table_fp7_df,
  &X86Architecture::Table_fp7_e0,
  &X86Architecture::Table_fp7_e1,
  &X86Architecture::Table_fp7_e2,
  &X86Architecture::Table_fp7_e3,
  &X86Architecture::Table_fp7_e4,
  &X86Architecture::Table_fp7_e5,
  &X86Architecture::Table_fp7_e6,
  &X86Architecture::Table_fp7_e7,
  &X86Architecture::Table_fp7_e8,
  &X86Architecture::Table_fp7_e9,
  &X86Architecture::Table_fp7_ea,
  &X86Architecture::Table_fp7_eb,
  &X86Architecture::Table_fp7_ec,
  &X86Architecture::Table_fp7_ed,
  &X86Architecture::Table_fp7_ee,
  &X86Architecture::Table_fp7_ef,
  &X86Architecture::Table_fp7_f0,
  &X86Architecture::Table_fp7_f1,
  &X86Architecture::Table_fp7_f2,
  &X86Architecture::Table_fp7_f3,
  &X86Architecture::Table_fp7_f4,
  &X86Architecture::Table_fp7_f5,
  &X86Architecture::Table_fp7_f6,
  &X86Architecture::Table_fp7_f7,
  &X86Architecture::Table_fp7_f8,
  &X86Architecture::Table_fp7_f9,
  &X86Architecture::Table_fp7_fa,
  &X86Architecture::Table_fp7_fb,
  &X86Architecture::Table_fp7_fc,
  &X86Architecture::Table_fp7_fd,
  &X86Architecture::Table_fp7_fe,
  &X86Architecture::Table_fp7_ff
};

/** instruction
 * mnemonic: faddp
 * operand: ['ST0', 'ST0']
 * opcode: 00
**/
bool X86Architecture::Table_fp7_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: faddp
 * operand: ['ST1', 'ST0']
 * opcode: 01
**/
bool X86Architecture::Table_fp7_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: faddp
 * operand: ['ST2', 'ST0']
 * opcode: 02
**/
bool X86Architecture::Table_fp7_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: faddp
 * operand: ['ST3', 'ST0']
 * opcode: 03
**/
bool X86Architecture::Table_fp7_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: faddp
 * operand: ['ST4', 'ST0']
 * opcode: 04
**/
bool X86Architecture::Table_fp7_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: faddp
 * operand: ['ST5', 'ST0']
 * opcode: 05
**/
bool X86Architecture::Table_fp7_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: faddp
 * operand: ['ST6', 'ST0']
 * opcode: 06
**/
bool X86Architecture::Table_fp7_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: faddp
 * operand: ['ST7', 'ST0']
 * opcode: 07
**/
bool X86Architecture::Table_fp7_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST0', 'ST0']
 * opcode: 08
**/
bool X86Architecture::Table_fp7_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST1', 'ST0']
 * opcode: 09
**/
bool X86Architecture::Table_fp7_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST2', 'ST0']
 * opcode: 0a
**/
bool X86Architecture::Table_fp7_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST3', 'ST0']
 * opcode: 0b
**/
bool X86Architecture::Table_fp7_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST4', 'ST0']
 * opcode: 0c
**/
bool X86Architecture::Table_fp7_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST5', 'ST0']
 * opcode: 0d
**/
bool X86Architecture::Table_fp7_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST6', 'ST0']
 * opcode: 0e
**/
bool X86Architecture::Table_fp7_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fmulp
 * operand: ['ST7', 'ST0']
 * opcode: 0f
**/
bool X86Architecture::Table_fp7_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fcomp
 * operand: ['ST0']
 * opcode: 10
**/
bool X86Architecture::Table_fp7_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
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
 * opcode: 11
**/
bool X86Architecture::Table_fp7_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
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
 * opcode: 12
**/
bool X86Architecture::Table_fp7_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
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
 * opcode: 13
**/
bool X86Architecture::Table_fp7_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
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
 * opcode: 14
**/
bool X86Architecture::Table_fp7_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
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
 * opcode: 15
**/
bool X86Architecture::Table_fp7_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
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
 * opcode: 16
**/
bool X86Architecture::Table_fp7_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
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
 * opcode: 17
**/
bool X86Architecture::Table_fp7_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcomp);
    if (Operand__ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 18
 * invalid
**/
bool X86Architecture::Table_fp7_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: fcompp
 * opcode: 19
**/
bool X86Architecture::Table_fp7_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fcompp);
    return true;
}

/** instruction
 * opcode: 1a
 * invalid
**/
bool X86Architecture::Table_fp7_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1b
 * invalid
**/
bool X86Architecture::Table_fp7_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1c
 * invalid
**/
bool X86Architecture::Table_fp7_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1d
 * invalid
**/
bool X86Architecture::Table_fp7_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1e
 * invalid
**/
bool X86Architecture::Table_fp7_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1f
 * invalid
**/
bool X86Architecture::Table_fp7_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST0', 'ST0']
 * opcode: 20
**/
bool X86Architecture::Table_fp7_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST1', 'ST0']
 * opcode: 21
**/
bool X86Architecture::Table_fp7_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST2', 'ST0']
 * opcode: 22
**/
bool X86Architecture::Table_fp7_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST3', 'ST0']
 * opcode: 23
**/
bool X86Architecture::Table_fp7_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST4', 'ST0']
 * opcode: 24
**/
bool X86Architecture::Table_fp7_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST5', 'ST0']
 * opcode: 25
**/
bool X86Architecture::Table_fp7_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST6', 'ST0']
 * opcode: 26
**/
bool X86Architecture::Table_fp7_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubrp
 * operand: ['ST7', 'ST0']
 * opcode: 27
**/
bool X86Architecture::Table_fp7_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST0', 'ST0']
 * opcode: 28
**/
bool X86Architecture::Table_fp7_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST1', 'ST0']
 * opcode: 29
**/
bool X86Architecture::Table_fp7_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST2', 'ST0']
 * opcode: 2a
**/
bool X86Architecture::Table_fp7_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST3', 'ST0']
 * opcode: 2b
**/
bool X86Architecture::Table_fp7_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST4', 'ST0']
 * opcode: 2c
**/
bool X86Architecture::Table_fp7_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST5', 'ST0']
 * opcode: 2d
**/
bool X86Architecture::Table_fp7_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST6', 'ST0']
 * opcode: 2e
**/
bool X86Architecture::Table_fp7_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fsubp
 * operand: ['ST7', 'ST0']
 * opcode: 2f
**/
bool X86Architecture::Table_fp7_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST0', 'ST0']
 * opcode: 30
**/
bool X86Architecture::Table_fp7_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST1', 'ST0']
 * opcode: 31
**/
bool X86Architecture::Table_fp7_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST2', 'ST0']
 * opcode: 32
**/
bool X86Architecture::Table_fp7_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST3', 'ST0']
 * opcode: 33
**/
bool X86Architecture::Table_fp7_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST4', 'ST0']
 * opcode: 34
**/
bool X86Architecture::Table_fp7_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST5', 'ST0']
 * opcode: 35
**/
bool X86Architecture::Table_fp7_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST6', 'ST0']
 * opcode: 36
**/
bool X86Architecture::Table_fp7_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivrp
 * operand: ['ST7', 'ST0']
 * opcode: 37
**/
bool X86Architecture::Table_fp7_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST0', 'ST0']
 * opcode: 38
**/
bool X86Architecture::Table_fp7_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST1', 'ST0']
 * opcode: 39
**/
bool X86Architecture::Table_fp7_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST2', 'ST0']
 * opcode: 3a
**/
bool X86Architecture::Table_fp7_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST3', 'ST0']
 * opcode: 3b
**/
bool X86Architecture::Table_fp7_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST4', 'ST0']
 * opcode: 3c
**/
bool X86Architecture::Table_fp7_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST5', 'ST0']
 * opcode: 3d
**/
bool X86Architecture::Table_fp7_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST6', 'ST0']
 * opcode: 3e
**/
bool X86Architecture::Table_fp7_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fdivp
 * operand: ['ST7', 'ST0']
 * opcode: 3f
**/
bool X86Architecture::Table_fp7_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

