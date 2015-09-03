/* This file has been automatically generated, you must _NOT_ edit it directly. (Sat Sep  5 13:49:08 2015) */
#include "st62_architecture.hpp"
const char *St62Architecture::m_Mnemonic[0x1e] =
{
  "unknown",
  "add",
  "addi",
  "and",
  "andi",
  "call",
  "com",
  "cp",
  "cpi",
  "dec",
  "inc",
  "jp",
  "jrc",
  "jrnc",
  "jrnz",
  "jrr",
  "jrs",
  "jrz",
  "ld",
  "ldi",
  "nop",
  "res",
  "ret",
  "reti",
  "rlc",
  "set",
  "stop",
  "sub",
  "subi",
  "wait"
};
const St62Architecture::TDisassembler St62Architecture::m_Table_1[0x10] =
{
  &St62Architecture::Table_1_00,
  &St62Architecture::Table_1_01,
  &St62Architecture::Table_1_02,
  &St62Architecture::Table_1_03,
  &St62Architecture::Table_1_04,
  &St62Architecture::Table_1_05,
  &St62Architecture::Table_1_06,
  &St62Architecture::Table_1_07,
  &St62Architecture::Table_1_08,
  &St62Architecture::Table_1_09,
  &St62Architecture::Table_1_0a,
  &St62Architecture::Table_1_0b,
  &St62Architecture::Table_1_0c,
  &St62Architecture::Table_1_0d,
  &St62Architecture::Table_1_0e,
  &St62Architecture::Table_1_0f
};

const St62Architecture::TDisassembler St62Architecture::m_Table_5[0x10] =
{
  &St62Architecture::Table_5_00,
  &St62Architecture::Table_5_01,
  &St62Architecture::Table_5_02,
  &St62Architecture::Table_5_03,
  &St62Architecture::Table_5_04,
  &St62Architecture::Table_5_05,
  &St62Architecture::Table_5_06,
  &St62Architecture::Table_5_07,
  &St62Architecture::Table_5_08,
  &St62Architecture::Table_5_09,
  &St62Architecture::Table_5_0a,
  &St62Architecture::Table_5_0b,
  &St62Architecture::Table_5_0c,
  &St62Architecture::Table_5_0d,
  &St62Architecture::Table_5_0e,
  &St62Architecture::Table_5_0f
};

const St62Architecture::TDisassembler St62Architecture::m_Table_7[0x10] =
{
  &St62Architecture::Table_7_00,
  &St62Architecture::Table_7_01,
  &St62Architecture::Table_7_02,
  &St62Architecture::Table_7_03,
  &St62Architecture::Table_7_04,
  &St62Architecture::Table_7_05,
  &St62Architecture::Table_7_06,
  &St62Architecture::Table_7_07,
  &St62Architecture::Table_7_08,
  &St62Architecture::Table_7_09,
  &St62Architecture::Table_7_0a,
  &St62Architecture::Table_7_0b,
  &St62Architecture::Table_7_0c,
  &St62Architecture::Table_7_0d,
  &St62Architecture::Table_7_0e,
  &St62Architecture::Table_7_0f
};

const St62Architecture::TDisassembler St62Architecture::m_Table_d[0x10] =
{
  &St62Architecture::Table_d_00,
  &St62Architecture::Table_d_01,
  &St62Architecture::Table_d_02,
  &St62Architecture::Table_d_03,
  &St62Architecture::Table_d_04,
  &St62Architecture::Table_d_05,
  &St62Architecture::Table_d_06,
  &St62Architecture::Table_d_07,
  &St62Architecture::Table_d_08,
  &St62Architecture::Table_d_09,
  &St62Architecture::Table_d_0a,
  &St62Architecture::Table_d_0b,
  &St62Architecture::Table_d_0c,
  &St62Architecture::Table_d_0d,
  &St62Architecture::Table_d_0e,
  &St62Architecture::Table_d_0f
};

const St62Architecture::TDisassembler St62Architecture::m_Table_f[0x10] =
{
  &St62Architecture::Table_f_00,
  &St62Architecture::Table_f_01,
  &St62Architecture::Table_f_02,
  &St62Architecture::Table_f_03,
  &St62Architecture::Table_f_04,
  &St62Architecture::Table_f_05,
  &St62Architecture::Table_f_06,
  &St62Architecture::Table_f_07,
  &St62Architecture::Table_f_08,
  &St62Architecture::Table_f_09,
  &St62Architecture::Table_f_0a,
  &St62Architecture::Table_f_0b,
  &St62Architecture::Table_f_0c,
  &St62Architecture::Table_f_0d,
  &St62Architecture::Table_f_0e,
  &St62Architecture::Table_f_0f
};

/** instruction
 * mnemonic: jrnz
 * operand: ['pcr']
 * opcode: 00
 * operation_type: ['jmp', 'cond']
 * semantic: if __expr and zf.id == int1(0): program.id = op0.val;

**/
bool St62Architecture::Table_1_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnz);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: if __expr and zf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeConst(1, 0x0),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(0)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: call
 * operand: ['ext']
 * opcode: 01
 * operation_type: ['call']
 * semantic: #stack.id -= stack.size;
#stack.mem = program.id;
program.id = op0.val;

**/
bool St62Architecture::Table_1_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Call);
    rInsn.SubType() |= Instruction::CallType;
    if (Operand__ext(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: #stack.id -= stack.size */
      /* semantic: #stack.mem = program.id */
      /* semantic: program.id = op0.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
        rInsn.GetOperand(0)));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: jrnc
 * operand: ['pcr']
 * opcode: 02
 * operation_type: ['jmp', 'cond']
 * semantic: if __expr and cf.id == int1(0): program.id = op0.val;

**/
bool St62Architecture::Table_1_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: if __expr and cf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeConst(1, 0x0),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(0)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: jrr
 * operand: ['bitdirect', 'direct', 'ee']
 * alternate: jrs
 * opcode: 03
 * operation_type: ['jmp', 'cond']
**/
bool St62Architecture::Table_1_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    u8 Value;
    if (!rBinStrm.Read(Offset, Value))
      return false;

    if (Value & 0x10) {
      rInsn.SetOpcode(ST62_Opcode_Jrs);
    } else {
      rInsn.SetOpcode(ST62_Opcode_Jrr);
    }
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__bitdirect(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__ee(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: nop
 * opcode: 04
**/
bool St62Architecture::Table_1_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Nop);
    return true;
}

/** instruction
 * opcode: 05
 * reference: table_5
**/
bool St62Architecture::Table_1_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    u8 High;
    if (!rBinStrm.Read(Offset, High))
      return false;

    High >>= 4;
    if (High + 1 > sizeof(m_Table_5))
      return false;
    return (this->*m_Table_5[High])(rBinStrm, Offset, rInsn, Mode);
}

/** instruction
 * mnemonic: jrc
 * operand: ['pcr']
 * opcode: 06
 * operation_type: ['jmp', 'cond']
 * semantic: if __expr and cf.id == int1(1): program.id = op0.val;

**/
bool St62Architecture::Table_1_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jrc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: if __expr and cf.id == int1(1): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeConst(1, 0x1),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(0)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 07
 * reference: table_7
**/
bool St62Architecture::Table_1_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    u8 High;
    if (!rBinStrm.Read(Offset, High))
      return false;

    High >>= 4;
    if (High + 1 > sizeof(m_Table_7))
      return false;
    return (this->*m_Table_7[High])(rBinStrm, Offset, rInsn, Mode);
}

/** instruction
 * mnemonic: jrnz
 * operand: ['pcr']
 * opcode: 08
 * operation_type: ['jmp', 'cond']
 * semantic: if __expr and zf.id == int1(0): program.id = op0.val;

**/
bool St62Architecture::Table_1_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnz);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: if __expr and zf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeConst(1, 0x0),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(0)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: jp
 * operand: ['ext']
 * opcode: 09
 * operation_type: ['jmp']
 * semantic: program.id = op0.val;

**/
bool St62Architecture::Table_1_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jp);
    rInsn.SubType() |= Instruction::JumpType;
    if (Operand__ext(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: program.id = op0.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
        rInsn.GetOperand(0)));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: jrnc
 * operand: ['pcr']
 * opcode: 0a
 * operation_type: ['jmp', 'cond']
 * semantic: if __expr and cf.id == int1(0): program.id = op0.val;

**/
bool St62Architecture::Table_1_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: if __expr and cf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeConst(1, 0x0),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(0)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: res
 * operand: ['bitdirect', 'direct']
 * alternate: set
 * opcode: 0b
**/
bool St62Architecture::Table_1_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    u8 Value;
    if (!rBinStrm.Read(Offset, Value))
      return false;

    if (Value & 0x10) {
      rInsn.SetOpcode(ST62_Opcode_Set);
    } else {
      rInsn.SetOpcode(ST62_Opcode_Res);
    }
    if (Operand__bitdirect(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: jrz
 * operand: ['pcr']
 * opcode: 0c
 * operation_type: ['jmp', 'cond']
 * semantic: if __expr and zf.id == int1(1): program.id = op0.val;

**/
bool St62Architecture::Table_1_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jrz);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: if __expr and zf.id == int1(1): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeConst(1, 0x1),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(0)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 0d
 * reference: table_D
**/
bool St62Architecture::Table_1_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    u8 High;
    if (!rBinStrm.Read(Offset, High))
      return false;

    High >>= 4;
    if (High + 1 > sizeof(m_Table_d))
      return false;
    return (this->*m_Table_d[High])(rBinStrm, Offset, rInsn, Mode);
}

/** instruction
 * mnemonic: jrc
 * operand: ['pcr']
 * opcode: 0e
 * operation_type: ['jmp', 'cond']
 * semantic: if __expr and cf.id == int1(1): program.id = op0.val;

**/
bool St62Architecture::Table_1_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Jrc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: if __expr and cf.id == int1(1): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeConst(1, 0x1),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(0)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 0f
 * reference: table_F
**/
bool St62Architecture::Table_1_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    u8 High;
    if (!rBinStrm.Read(Offset, High))
      return false;

    High >>= 4;
    if (High + 1 > sizeof(m_Table_f))
      return false;
    return (this->*m_Table_f[High])(rBinStrm, Offset, rInsn, Mode);
}

/** instruction
 * opcode: 00
 * invalid: True
**/
bool St62Architecture::Table_5_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['X']
 * opcode: 01
**/
bool St62Architecture::Table_5_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__X(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 02
 * invalid: True
**/
bool St62Architecture::Table_5_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'X']
 * opcode: 03
**/
bool St62Architecture::Table_5_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__X(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 04
 * invalid: True
**/
bool St62Architecture::Table_5_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['Y']
 * opcode: 05
**/
bool St62Architecture::Table_5_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 06
 * invalid: True
**/
bool St62Architecture::Table_5_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'Y']
 * opcode: 07
**/
bool St62Architecture::Table_5_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 08
 * invalid: True
**/
bool St62Architecture::Table_5_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['V']
 * opcode: 09
**/
bool St62Architecture::Table_5_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0a
 * invalid: True
**/
bool St62Architecture::Table_5_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'V']
 * opcode: 0b
**/
bool St62Architecture::Table_5_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0c
 * invalid: True
**/
bool St62Architecture::Table_5_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['W']
 * opcode: 0d
**/
bool St62Architecture::Table_5_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0e
 * invalid: True
**/
bool St62Architecture::Table_5_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'W']
 * opcode: 0f
**/
bool St62Architecture::Table_5_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'rX']
 * opcode: 00
**/
bool St62Architecture::Table_7_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ldi
 * operand: ['A', 'imm']
 * opcode: 01
**/
bool St62Architecture::Table_7_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ldi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: cp
 * operand: ['A', 'rX']
 * opcode: 02
**/
bool St62Architecture::Table_7_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Cp);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: cpi
 * operand: ['A', 'imm']
 * opcode: 03
**/
bool St62Architecture::Table_7_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Cpi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: add
 * operand: ['A', 'rX']
 * opcode: 04
**/
bool St62Architecture::Table_7_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Add);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: addi
 * operand: ['A', 'imm']
 * opcode: 05
**/
bool St62Architecture::Table_7_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Addi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: inc
 * operand: ['rX']
 * opcode: 06
**/
bool St62Architecture::Table_7_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 07
 * invalid: True
**/
bool St62Architecture::Table_7_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['rX', 'A']
 * opcode: 08
**/
bool St62Architecture::Table_7_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 09
 * invalid: True
**/
bool St62Architecture::Table_7_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: and
 * operand: ['rX', 'A']
 * opcode: 0a
**/
bool St62Architecture::Table_7_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_And);
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: andi
 * operand: ['A', 'imm']
 * opcode: 0b
**/
bool St62Architecture::Table_7_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Andi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: sub
 * operand: ['A', 'rX']
 * opcode: 0c
**/
bool St62Architecture::Table_7_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Sub);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: subi
 * operand: ['A', 'imm']
 * opcode: 0d
**/
bool St62Architecture::Table_7_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Subi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['rX']
 * opcode: 0e
**/
bool St62Architecture::Table_7_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 0f
 * invalid: True
**/
bool St62Architecture::Table_7_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ldi
 * operand: ['direct', 'imm']
 * opcode: 00
**/
bool St62Architecture::Table_d_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ldi);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: set
 * operand: ['bitdirect', 'direct']
 * opcode: 01
**/
bool St62Architecture::Table_d_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Set);
    if (Operand__bitdirect(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: com
 * operand: ['A']
 * opcode: 02
**/
bool St62Architecture::Table_d_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Com);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['X', 'A']
 * opcode: 03
**/
bool St62Architecture::Table_d_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__X(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: reti
 * operation_type: ['ret']
 * opcode: 04
**/
bool St62Architecture::Table_d_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Reti);
    rInsn.SubType() |= Instruction::ReturnType;
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['Y']
 * opcode: 05
**/
bool St62Architecture::Table_d_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: stop
 * opcode: 06
**/
bool St62Architecture::Table_d_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Stop);
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['Y', 'A']
 * opcode: 07
**/
bool St62Architecture::Table_d_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * opcode: 08
 * invalid: True
**/
bool St62Architecture::Table_d_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: dec
 * operand: ['V']
 * opcode: 09
**/
bool St62Architecture::Table_d_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: rlc
 * operand: ['A']
 * opcode: 0a
**/
bool St62Architecture::Table_d_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Rlc);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['V', 'A']
 * opcode: 0b
**/
bool St62Architecture::Table_d_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ret
 * operation_type: ['ret']
 * opcode: 0c
**/
bool St62Architecture::Table_d_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ret);
    rInsn.SubType() |= Instruction::ReturnType;
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['W']
 * opcode: 0d
**/
bool St62Architecture::Table_d_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: wait
 * opcode: 0e
**/
bool St62Architecture::Table_d_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Wait);
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['W', 'A']
 * opcode: 0f
**/
bool St62Architecture::Table_d_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'rY']
 * opcode: 00
**/
bool St62Architecture::Table_f_00(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'direct']
 * opcode: 01
**/
bool St62Architecture::Table_f_01(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: cp
 * operand: ['A', 'rY']
 * opcode: 02
**/
bool St62Architecture::Table_f_02(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Cp);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: cp
 * operand: ['A', 'direct']
 * opcode: 03
**/
bool St62Architecture::Table_f_03(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Cp);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: add
 * operand: ['A', 'rY']
 * opcode: 04
**/
bool St62Architecture::Table_f_04(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Add);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: add
 * operand: ['A', 'direct']
 * opcode: 05
**/
bool St62Architecture::Table_f_05(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Add);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: inc
 * operand: ['rY']
 * opcode: 06
**/
bool St62Architecture::Table_f_06(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: inc
 * operand: ['direct']
 * opcode: 07
**/
bool St62Architecture::Table_f_07(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['rY', 'A']
 * opcode: 08
**/
bool St62Architecture::Table_f_08(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['direct', 'A']
 * opcode: 09
**/
bool St62Architecture::Table_f_09(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: and
 * operand: ['A', 'rY']
 * opcode: 0a
**/
bool St62Architecture::Table_f_0a(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_And);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: and
 * operand: ['A', 'direct']
 * opcode: 0b
**/
bool St62Architecture::Table_f_0b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_And);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: sub
 * operand: ['A', 'rY']
 * opcode: 0c
**/
bool St62Architecture::Table_f_0c(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Sub);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: sub
 * operand: ['A', 'direct']
 * opcode: 0d
**/
bool St62Architecture::Table_f_0d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Sub);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['rY']
 * opcode: 0e
**/
bool St62Architecture::Table_f_0e(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['direct']
 * opcode: 0f
**/
bool St62Architecture::Table_f_0f(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Length()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    return true;
}

/* operand ['op0 = decode_A'] */
bool St62Architecture::Operand__A(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_A
  auto spOprd0 = Decode_A(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_V'] */
bool St62Architecture::Operand__V(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_V
  auto spOprd0 = Decode_V(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_W'] */
bool St62Architecture::Operand__W(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_W
  auto spOprd0 = Decode_W(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_X'] */
bool St62Architecture::Operand__X(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_X
  auto spOprd0 = Decode_X(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_Y'] */
bool St62Architecture::Operand__Y(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_Y
  auto spOprd0 = Decode_Y(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_bitdirect'] */
bool St62Architecture::Operand__bitdirect(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_bitdirect
  auto spOprd0 = Decode_bitdirect(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* decoder ["reg('A')"] */
Expression::SPType St62Architecture::Decode_A(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('A')
  return Expr::MakeId(ST62_Reg_A, &m_CpuInfo);
}

/* decoder ["reg('V')"] */
Expression::SPType St62Architecture::Decode_V(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('V')
  return Expr::MakeId(ST62_Reg_V, &m_CpuInfo);
}

/* decoder ["reg('W')"] */
Expression::SPType St62Architecture::Decode_W(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('W')
  return Expr::MakeId(ST62_Reg_W, &m_CpuInfo);
}

/* decoder ["reg('X')"] */
Expression::SPType St62Architecture::Decode_X(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('X')
  return Expr::MakeId(ST62_Reg_X, &m_CpuInfo);
}

/* decoder ["reg('Y')"] */
Expression::SPType St62Architecture::Decode_Y(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: reg('Y')
  return Expr::MakeId(ST62_Reg_Y, &m_CpuInfo);
}

/* decoder ["call('Decode_bitdirect')"] */
Expression::SPType St62Architecture::Decode_bitdirect(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_bitdirect')
  return __Decode_bitdirect(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_direct')"] */
Expression::SPType St62Architecture::Decode_direct(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_direct')
  return __Decode_direct(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_ee')"] */
Expression::SPType St62Architecture::Decode_ee(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_ee')
  return __Decode_ee(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_ext')"] */
Expression::SPType St62Architecture::Decode_ext(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_ext')
  return __Decode_ext(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_imm')"] */
Expression::SPType St62Architecture::Decode_imm(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_imm')
  return __Decode_imm(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["call('Decode_pcr')"] */
Expression::SPType St62Architecture::Decode_pcr(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: call('Decode_pcr')
  return __Decode_pcr(rBinStrm, Offset, rInsn, Mode);
}

/* decoder ["ireg('X')"] */
Expression::SPType St62Architecture::Decode_rX(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: ireg('X')
  return Expr::MakeMem(8, Expr::MakeConst(16, 0x1000), Expr::MakeId(ST62_Reg_X, &m_CpuInfo), true);
}

/* decoder ["ireg('Y')"] */
Expression::SPType St62Architecture::Decode_rY(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: ireg('Y')
  return Expr::MakeMem(8, Expr::MakeConst(16, 0x1000), Expr::MakeId(ST62_Reg_Y, &m_CpuInfo), true);
}

/* operand ['op0 = decode_direct'] */
bool St62Architecture::Operand__direct(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_direct
  auto spOprd0 = Decode_direct(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ee'] */
bool St62Architecture::Operand__ee(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ee
  auto spOprd0 = Decode_ee(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_ext'] */
bool St62Architecture::Operand__ext(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_ext
  auto spOprd0 = Decode_ext(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_imm'] */
bool St62Architecture::Operand__imm(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_imm
  auto spOprd0 = Decode_imm(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_pcr'] */
bool St62Architecture::Operand__pcr(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_pcr
  auto spOprd0 = Decode_pcr(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rX'] */
bool St62Architecture::Operand__rX(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rX
  auto spOprd0 = Decode_rX(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

/* operand ['op0 = decode_rY'] */
bool St62Architecture::Operand__rY(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  // operand0: op0 = decode_rY
  auto spOprd0 = Decode_rY(rBinStrm, Offset, rInsn, Mode);
  if (spOprd0 == nullptr)
    return false;
  rInsn.AddOperand(spOprd0);

  return true;
}

