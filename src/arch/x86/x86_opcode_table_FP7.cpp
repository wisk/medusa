/* This file has been automatically generated, you must _NOT_ edit it directly. (Thu Apr  9 18:19:39 2026) */
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
 * opcode: 00
 * mnemonic: faddp
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp7_c0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 01
 * mnemonic: faddp
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp7_c1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * mnemonic: faddp
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp7_c2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 03
 * mnemonic: faddp
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp7_c3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * mnemonic: faddp
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp7_c4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 05
 * mnemonic: faddp
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp7_c5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * mnemonic: faddp
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp7_c6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * mnemonic: faddp
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp7_c7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Faddp);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 08
 * mnemonic: fmulp
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp7_c8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 09
 * mnemonic: fmulp
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp7_c9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0a
 * mnemonic: fmulp
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp7_ca(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0b
 * mnemonic: fmulp
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp7_cb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0c
 * mnemonic: fmulp
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp7_cc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0d
 * mnemonic: fmulp
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp7_cd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0e
 * mnemonic: fmulp
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp7_ce(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0f
 * mnemonic: fmulp
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp7_cf(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fmulp);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 10
 * mnemonic: fcomp
 * operand: ['ST0']
**/
bool X86Architecture::Table_fp7_d0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 11
 * mnemonic: fcomp
 * operand: ['ST1']
**/
bool X86Architecture::Table_fp7_d1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 12
 * mnemonic: fcomp
 * operand: ['ST2']
**/
bool X86Architecture::Table_fp7_d2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 13
 * mnemonic: fcomp
 * operand: ['ST3']
**/
bool X86Architecture::Table_fp7_d3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 14
 * mnemonic: fcomp
 * operand: ['ST4']
**/
bool X86Architecture::Table_fp7_d4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 15
 * mnemonic: fcomp
 * operand: ['ST5']
**/
bool X86Architecture::Table_fp7_d5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 16
 * mnemonic: fcomp
 * operand: ['ST6']
**/
bool X86Architecture::Table_fp7_d6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 17
 * mnemonic: fcomp
 * operand: ['ST7']
**/
bool X86Architecture::Table_fp7_d7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * opcode: 18
 * invalid
**/
bool X86Architecture::Table_fp7_d8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 19
 * mnemonic: fcompp
**/
bool X86Architecture::Table_fp7_d9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fcompp);
    return true;
}

/** instruction
 * opcode: 1a
 * invalid
**/
bool X86Architecture::Table_fp7_da(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1b
 * invalid
**/
bool X86Architecture::Table_fp7_db(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1c
 * invalid
**/
bool X86Architecture::Table_fp7_dc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1d
 * invalid
**/
bool X86Architecture::Table_fp7_dd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1e
 * invalid
**/
bool X86Architecture::Table_fp7_de(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 1f
 * invalid
**/
bool X86Architecture::Table_fp7_df(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * opcode: 20
 * mnemonic: fsubrp
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp7_e0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 21
 * mnemonic: fsubrp
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp7_e1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 22
 * mnemonic: fsubrp
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp7_e2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 23
 * mnemonic: fsubrp
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp7_e3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 24
 * mnemonic: fsubrp
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp7_e4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 25
 * mnemonic: fsubrp
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp7_e5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 26
 * mnemonic: fsubrp
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp7_e6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 27
 * mnemonic: fsubrp
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp7_e7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubrp);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 28
 * mnemonic: fsubp
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp7_e8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 29
 * mnemonic: fsubp
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp7_e9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2a
 * mnemonic: fsubp
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp7_ea(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2b
 * mnemonic: fsubp
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp7_eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2c
 * mnemonic: fsubp
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp7_ec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2d
 * mnemonic: fsubp
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp7_ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2e
 * mnemonic: fsubp
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp7_ee(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 2f
 * mnemonic: fsubp
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp7_ef(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fsubp);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 30
 * mnemonic: fdivrp
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp7_f0(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 31
 * mnemonic: fdivrp
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp7_f1(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 32
 * mnemonic: fdivrp
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp7_f2(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 33
 * mnemonic: fdivrp
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp7_f3(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 34
 * mnemonic: fdivrp
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp7_f4(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 35
 * mnemonic: fdivrp
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp7_f5(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 36
 * mnemonic: fdivrp
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp7_f6(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 37
 * mnemonic: fdivrp
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp7_f7(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivrp);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 38
 * mnemonic: fdivp
 * operand: ['ST0', 'ST0']
**/
bool X86Architecture::Table_fp7_f8(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST0_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 39
 * mnemonic: fdivp
 * operand: ['ST1', 'ST0']
**/
bool X86Architecture::Table_fp7_f9(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST1_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3a
 * mnemonic: fdivp
 * operand: ['ST2', 'ST0']
**/
bool X86Architecture::Table_fp7_fa(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST2_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3b
 * mnemonic: fdivp
 * operand: ['ST3', 'ST0']
**/
bool X86Architecture::Table_fp7_fb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST3_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3c
 * mnemonic: fdivp
 * operand: ['ST4', 'ST0']
**/
bool X86Architecture::Table_fp7_fc(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST4_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3d
 * mnemonic: fdivp
 * operand: ['ST5', 'ST0']
**/
bool X86Architecture::Table_fp7_fd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST5_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3e
 * mnemonic: fdivp
 * operand: ['ST6', 'ST0']
**/
bool X86Architecture::Table_fp7_fe(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST6_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 3f
 * mnemonic: fdivp
 * operand: ['ST7', 'ST0']
**/
bool X86Architecture::Table_fp7_ff(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(X86_Opcode_Fdivp);
    if (!Operand__ST7_ST0(rBinStrm, Offset, rInsn, Mode))
    {
      return false;
    }
    return true;
}

