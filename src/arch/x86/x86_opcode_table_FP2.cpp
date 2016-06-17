/* This file has been automatically generated, you must _NOT_ edit it directly. (Fri Jun 17 18:00:43 2016) */
#include "x86_architecture.hpp"
const X86Architecture::TDisassembler X86Architecture::m_Table_fp2[0x40] =
{
  &X86Architecture::Table_fp2_c0,
  &X86Architecture::Table_fp2_c1,
  &X86Architecture::Table_fp2_c2,
  &X86Architecture::Table_fp2_c3,
  &X86Architecture::Table_fp2_c4,
  &X86Architecture::Table_fp2_c5,
  &X86Architecture::Table_fp2_c6,
  &X86Architecture::Table_fp2_c7,
  &X86Architecture::Table_fp2_c8,
  &X86Architecture::Table_fp2_c9,
  &X86Architecture::Table_fp2_ca,
  &X86Architecture::Table_fp2_cb,
  &X86Architecture::Table_fp2_cc,
  &X86Architecture::Table_fp2_cd,
  &X86Architecture::Table_fp2_ce,
  &X86Architecture::Table_fp2_cf,
  &X86Architecture::Table_fp2_d0,
  &X86Architecture::Table_fp2_d1,
  &X86Architecture::Table_fp2_d2,
  &X86Architecture::Table_fp2_d3,
  &X86Architecture::Table_fp2_d4,
  &X86Architecture::Table_fp2_d5,
  &X86Architecture::Table_fp2_d6,
  &X86Architecture::Table_fp2_d7,
  &X86Architecture::Table_fp2_d8,
  &X86Architecture::Table_fp2_d9,
  &X86Architecture::Table_fp2_da,
  &X86Architecture::Table_fp2_db,
  &X86Architecture::Table_fp2_dc,
  &X86Architecture::Table_fp2_dd,
  &X86Architecture::Table_fp2_de,
  &X86Architecture::Table_fp2_df,
  &X86Architecture::Table_fp2_e0,
  &X86Architecture::Table_fp2_e1,
  &X86Architecture::Table_fp2_e2,
  &X86Architecture::Table_fp2_e3,
  &X86Architecture::Table_fp2_e4,
  &X86Architecture::Table_fp2_e5,
  &X86Architecture::Table_fp2_e6,
  &X86Architecture::Table_fp2_e7,
  &X86Architecture::Table_fp2_e8,
  &X86Architecture::Table_fp2_e9,
  &X86Architecture::Table_fp2_ea,
  &X86Architecture::Table_fp2_eb,
  &X86Architecture::Table_fp2_ec,
  &X86Architecture::Table_fp2_ed,
  &X86Architecture::Table_fp2_ee,
  &X86Architecture::Table_fp2_ef,
  &X86Architecture::Table_fp2_f0,
  &X86Architecture::Table_fp2_f1,
  &X86Architecture::Table_fp2_f2,
  &X86Architecture::Table_fp2_f3,
  &X86Architecture::Table_fp2_f4,
  &X86Architecture::Table_fp2_f5,
  &X86Architecture::Table_fp2_f6,
  &X86Architecture::Table_fp2_f7,
  &X86Architecture::Table_fp2_f8,
  &X86Architecture::Table_fp2_f9,
  &X86Architecture::Table_fp2_fa,
  &X86Architecture::Table_fp2_fb,
  &X86Architecture::Table_fp2_fc,
  &X86Architecture::Table_fp2_fd,
  &X86Architecture::Table_fp2_fe,
  &X86Architecture::Table_fp2_ff
};

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST0']
 * opcode: 00
**/
bool X86Architecture::Table_fp2_c0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST1']
 * opcode: 01
**/
bool X86Architecture::Table_fp2_c1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST2']
 * opcode: 02
**/
bool X86Architecture::Table_fp2_c2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST3']
 * opcode: 03
**/
bool X86Architecture::Table_fp2_c3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST4']
 * opcode: 04
**/
bool X86Architecture::Table_fp2_c4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST5']
 * opcode: 05
**/
bool X86Architecture::Table_fp2_c5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST6']
 * opcode: 06
**/
bool X86Architecture::Table_fp2_c6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fld
 * operand: ['ST0', 'ST7']
 * opcode: 07
**/
bool X86Architecture::Table_fp2_c7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST0']
 * opcode: 08
**/
bool X86Architecture::Table_fp2_c8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST1']
 * opcode: 09
**/
bool X86Architecture::Table_fp2_c9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST2']
 * opcode: 0a
**/
bool X86Architecture::Table_fp2_ca(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST3']
 * opcode: 0b
**/
bool X86Architecture::Table_fp2_cb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST4']
 * opcode: 0c
**/
bool X86Architecture::Table_fp2_cc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST5']
 * opcode: 0d
**/
bool X86Architecture::Table_fp2_cd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST6']
 * opcode: 0e
**/
bool X86Architecture::Table_fp2_ce(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fxch
 * operand: ['ST0', 'ST7']
 * opcode: 0f
**/
bool X86Architecture::Table_fp2_cf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: fnop
 * opcode: 10
**/
bool X86Architecture::Table_fp2_d0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fnop);
    return true;
}

/** instruction
 * opcode: 11
 * invalid
**/
bool X86Architecture::Table_fp2_d1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 12
 * invalid
**/
bool X86Architecture::Table_fp2_d2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 13
 * invalid
**/
bool X86Architecture::Table_fp2_d3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 14
 * invalid
**/
bool X86Architecture::Table_fp2_d4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 15
 * invalid
**/
bool X86Architecture::Table_fp2_d5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 16
 * invalid
**/
bool X86Architecture::Table_fp2_d6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 17
 * invalid
**/
bool X86Architecture::Table_fp2_d7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: fstp
 * operand: ['ST0']
 * opcode: 18
**/
bool X86Architecture::Table_fp2_d8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_d9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_da(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_db(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_dc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_dd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_de(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_df(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
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
 * mnemonic: fchs
 * opcode: 20
**/
bool X86Architecture::Table_fp2_e0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fchs);
    return true;
}

/** instruction
 * mnemonic: fabs
 * opcode: 21
**/
bool X86Architecture::Table_fp2_e1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fabs);
    return true;
}

/** instruction
 * opcode: 22
 * invalid
**/
bool X86Architecture::Table_fp2_e2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 23
 * invalid
**/
bool X86Architecture::Table_fp2_e3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ftst
 * opcode: 24
**/
bool X86Architecture::Table_fp2_e4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Ftst);
    return true;
}

/** instruction
 * mnemonic: fxam
 * opcode: 25
**/
bool X86Architecture::Table_fp2_e5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxam);
    return true;
}

/** instructions
 * opcode: 26
 *
 * invalid
 *
 * mnemonic: ftstp
 * attr: ['cyrix']
 *
**/
bool X86Architecture::Table_fp2_e6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_CYRIX)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Ftstp);
      return true;
    }
    else
    {
      return false; /* INVALID */
    }
}

/** instruction
 * opcode: 27
 * invalid
**/
bool X86Architecture::Table_fp2_e7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: fld1
 * opcode: 28
**/
bool X86Architecture::Table_fp2_e8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fld1);
    return true;
}

/** instruction
 * mnemonic: fldl2t
 * opcode: 29
**/
bool X86Architecture::Table_fp2_e9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fldl2t);
    return true;
}

/** instruction
 * mnemonic: fldl2e
 * opcode: 2a
**/
bool X86Architecture::Table_fp2_ea(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fldl2e);
    return true;
}

/** instruction
 * mnemonic: fldpi
 * opcode: 2b
**/
bool X86Architecture::Table_fp2_eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fldpi);
    return true;
}

/** instruction
 * mnemonic: fldlg2
 * opcode: 2c
**/
bool X86Architecture::Table_fp2_ec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fldlg2);
    return true;
}

/** instruction
 * mnemonic: fldln2
 * opcode: 2d
**/
bool X86Architecture::Table_fp2_ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fldln2);
    return true;
}

/** instruction
 * mnemonic: fldz
 * opcode: 2e
**/
bool X86Architecture::Table_fp2_ee(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fldz);
    return true;
}

/** instruction
 * opcode: 2f
 * invalid
**/
bool X86Architecture::Table_fp2_ef(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: f2xm1
 * opcode: 30
**/
bool X86Architecture::Table_fp2_f0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_F2xm1);
    return true;
}

/** instruction
 * mnemonic: fyl2x
 * opcode: 31
**/
bool X86Architecture::Table_fp2_f1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fyl2x);
    return true;
}

/** instruction
 * mnemonic: fptan
 * opcode: 32
**/
bool X86Architecture::Table_fp2_f2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fptan);
    return true;
}

/** instruction
 * mnemonic: fpatan
 * opcode: 33
**/
bool X86Architecture::Table_fp2_f3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fpatan);
    return true;
}

/** instruction
 * mnemonic: fxtract
 * opcode: 34
**/
bool X86Architecture::Table_fp2_f4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fxtract);
    return true;
}

/** instruction
 * mnemonic: fprem1
 * opcode: 35
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_f5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fprem1);
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fdecstp
 * opcode: 36
**/
bool X86Architecture::Table_fp2_f6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fdecstp);
    return true;
}

/** instruction
 * mnemonic: fincstp
 * opcode: 37
**/
bool X86Architecture::Table_fp2_f7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fincstp);
    return true;
}

/** instruction
 * mnemonic: fprem
 * opcode: 38
**/
bool X86Architecture::Table_fp2_f8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fprem);
    return true;
}

/** instruction
 * mnemonic: fyl2xp1
 * opcode: 39
**/
bool X86Architecture::Table_fp2_f9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fyl2xp1);
    return true;
}

/** instruction
 * mnemonic: fsqrt
 * opcode: 3a
**/
bool X86Architecture::Table_fp2_fa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fsqrt);
    return true;
}

/** instruction
 * mnemonic: fsincos
 * opcode: 3b
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_fb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fsincos);
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: frndint
 * opcode: 3c
**/
bool X86Architecture::Table_fp2_fc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Frndint);
    return true;
}

/** instruction
 * mnemonic: fscale
 * opcode: 3d
**/
bool X86Architecture::Table_fp2_fd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(X86_Opcode_Fscale);
    return true;
}

/** instruction
 * mnemonic: fsin
 * opcode: 3e
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_fe(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fsin);
      return true;
    }
    else
      return false;
}

/** instruction
 * mnemonic: fcos
 * opcode: 3f
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_ff(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Length()++;
      rInsn.SetOpcode(X86_Opcode_Fcos);
      return true;
    }
    else
      return false;
}

