/* This file has been automatically generated, you must _NOT_ edit it directly. (Thu Apr  9 18:19:39 2026) */
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
 * opcode: 00
 * mnemonic: fld
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp2_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 01
 * mnemonic: fld
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp2_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * mnemonic: fld
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp2_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 03
 * mnemonic: fld
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp2_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * mnemonic: fld
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp2_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 05
 * mnemonic: fld
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp2_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * mnemonic: fld
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp2_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * mnemonic: fld
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp2_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 08
 * mnemonic: fxch
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp2_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 09
 * mnemonic: fxch
 * operand: ['ST0', 'ST1']
**/
bool X86Architecture::Table_fp2_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST1(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0a
 * mnemonic: fxch
 * operand: ['ST0', 'ST2']
**/
bool X86Architecture::Table_fp2_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST2(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0b
 * mnemonic: fxch
 * operand: ['ST0', 'ST3']
**/
bool X86Architecture::Table_fp2_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST3(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0c
 * mnemonic: fxch
 * operand: ['ST0', 'ST4']
**/
bool X86Architecture::Table_fp2_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST4(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0d
 * mnemonic: fxch
 * operand: ['ST0', 'ST5']
**/
bool X86Architecture::Table_fp2_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST5(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0e
 * mnemonic: fxch
 * operand: ['ST0', 'ST6']
**/
bool X86Architecture::Table_fp2_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST6(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0f
 * mnemonic: fxch
 * operand: ['ST0', 'ST7']
**/
bool X86Architecture::Table_fp2_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxch);
    if (!Operand__ST0_ST7(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 10
 * mnemonic: fnop
**/
bool X86Architecture::Table_fp2_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fnop);
    return true;
}

/** instruction
 * opcode: 11
 * invalid
**/
bool X86Architecture::Table_fp2_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 12
 * invalid
**/
bool X86Architecture::Table_fp2_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 13
 * invalid
**/
bool X86Architecture::Table_fp2_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 14
 * invalid
**/
bool X86Architecture::Table_fp2_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 15
 * invalid
**/
bool X86Architecture::Table_fp2_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 16
 * invalid
**/
bool X86Architecture::Table_fp2_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 17
 * invalid
**/
bool X86Architecture::Table_fp2_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 18
 * mnemonic: fstp
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp2_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool X86Architecture::Table_fp2_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * mnemonic: fchs
**/
bool X86Architecture::Table_fp2_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fchs);
    return true;
}

/** instruction
 * opcode: 21
 * mnemonic: fabs
**/
bool X86Architecture::Table_fp2_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fabs);
    return true;
}

/** instruction
 * opcode: 22
 * invalid
**/
bool X86Architecture::Table_fp2_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 23
 * invalid
**/
bool X86Architecture::Table_fp2_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 24
 * mnemonic: ftst
**/
bool X86Architecture::Table_fp2_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Ftst);
    return true;
}

/** instruction
 * opcode: 25
 * mnemonic: fxam
**/
bool X86Architecture::Table_fp2_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
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
bool X86Architecture::Table_fp2_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Vendor") == X86_ProcType_CYRIX)
    {
      rInsn.Size()++;
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
bool X86Architecture::Table_fp2_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 28
 * mnemonic: fld1
**/
bool X86Architecture::Table_fp2_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fld1);
    return true;
}

/** instruction
 * opcode: 29
 * mnemonic: fldl2t
**/
bool X86Architecture::Table_fp2_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fldl2t);
    return true;
}

/** instruction
 * opcode: 2a
 * mnemonic: fldl2e
**/
bool X86Architecture::Table_fp2_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fldl2e);
    return true;
}

/** instruction
 * opcode: 2b
 * mnemonic: fldpi
**/
bool X86Architecture::Table_fp2_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fldpi);
    return true;
}

/** instruction
 * opcode: 2c
 * mnemonic: fldlg2
**/
bool X86Architecture::Table_fp2_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fldlg2);
    return true;
}

/** instruction
 * opcode: 2d
 * mnemonic: fldln2
**/
bool X86Architecture::Table_fp2_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fldln2);
    return true;
}

/** instruction
 * opcode: 2e
 * mnemonic: fldz
**/
bool X86Architecture::Table_fp2_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fldz);
    return true;
}

/** instruction
 * opcode: 2f
 * invalid
**/
bool X86Architecture::Table_fp2_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 30
 * mnemonic: f2xm1
**/
bool X86Architecture::Table_fp2_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_F2xm1);
    return true;
}

/** instruction
 * opcode: 31
 * mnemonic: fyl2x
**/
bool X86Architecture::Table_fp2_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fyl2x);
    return true;
}

/** instruction
 * opcode: 32
 * mnemonic: fptan
**/
bool X86Architecture::Table_fp2_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fptan);
    return true;
}

/** instruction
 * opcode: 33
 * mnemonic: fpatan
**/
bool X86Architecture::Table_fp2_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fpatan);
    return true;
}

/** instruction
 * opcode: 34
 * mnemonic: fxtract
**/
bool X86Architecture::Table_fp2_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fxtract);
    return true;
}

/** instruction
 * opcode: 35
 * mnemonic: fprem1
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fprem1);
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 36
 * mnemonic: fdecstp
**/
bool X86Architecture::Table_fp2_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdecstp);
    return true;
}

/** instruction
 * opcode: 37
 * mnemonic: fincstp
**/
bool X86Architecture::Table_fp2_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fincstp);
    return true;
}

/** instruction
 * opcode: 38
 * mnemonic: fprem
**/
bool X86Architecture::Table_fp2_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fprem);
    return true;
}

/** instruction
 * opcode: 39
 * mnemonic: fyl2xp1
**/
bool X86Architecture::Table_fp2_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fyl2xp1);
    return true;
}

/** instruction
 * opcode: 3a
 * mnemonic: fsqrt
**/
bool X86Architecture::Table_fp2_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsqrt);
    return true;
}

/** instruction
 * opcode: 3b
 * mnemonic: fsincos
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fsincos);
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 3c
 * mnemonic: frndint
**/
bool X86Architecture::Table_fp2_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Frndint);
    return true;
}

/** instruction
 * opcode: 3d
 * mnemonic: fscale
**/
bool X86Architecture::Table_fp2_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fscale);
    return true;
}

/** instruction
 * opcode: 3e
 * mnemonic: fsin
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fsin);
      return true;
    }
    else
      return false;
}

/** instruction
 * opcode: 3f
 * mnemonic: fcos
 * cpu_model: >= X86_Arch_80387
**/
bool X86Architecture::Table_fp2_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    if (m_CfgMdl.GetEnum("Architecture") >= X86_Arch_80387)
    {
      rInsn.Size()++;
      rInsn.SetOpcode(X86_Opcode_Fcos);
      return true;
    }
    else
      return false;
}

