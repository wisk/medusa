/* This file has been automatically generated, you must _NOT_ edit it directly. (Mon Nov 14 21:29:28 2016) */
#include "st62_architecture.hpp"
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
 * test_flags: ['zf']
 * semantic: if __expr and zf.id == int1(0): program.id = op0.val;

 * operation_type: ['jmp', 'cond']
 * opcode: 00
**/
bool St62Architecture::Table_1_00(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnz);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_Z);
      /* semantic: if __expr and zf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x0),
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
 * semantic: stk5.id = stk4.id;
stk4.id = stk3.id;
stk3.id = stk2.id;
stk2.id = stk1.id;
stk1.id = stk0.id;
stk0.id = program.id;
program.id = op0.val;

**/
bool St62Architecture::Table_1_01(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Call);
    rInsn.SubType() |= Instruction::CallType;
    if (Operand__ext(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: stk5.id = stk4.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk5, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk4, &m_CpuInfo)));
      /* semantic: stk4.id = stk3.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk4, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk3, &m_CpuInfo)));
      /* semantic: stk3.id = stk2.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk3, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk2, &m_CpuInfo)));
      /* semantic: stk2.id = stk1.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk2, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk1, &m_CpuInfo)));
      /* semantic: stk1.id = stk0.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk1, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk0, &m_CpuInfo)));
      /* semantic: stk0.id = program.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk0, &m_CpuInfo),
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo)));
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
 * test_flags: ['cf']
 * semantic: if __expr and cf.id == int1(0): program.id = op0.val;

 * operation_type: ['jmp', 'cond']
 * opcode: 02
**/
bool St62Architecture::Table_1_02(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_C);
      /* semantic: if __expr and cf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x0),
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
 * update_flags: ['cf']
 * semantic: cf.id = bit_cast(op1.val >> op0.val, int1(1));
if __expr and cf.id == int1(0): program.id = op2.val;

 * alternate: jrs
 * semantic_alt: cf.id = bit_cast(op1.val >> op0.val, int1(1));
if __expr and cf.id == int1(1): program.id = op2.val;

 * operand: ['bitdirect', 'direct', 'ee']
 * test_flags: ['cf']
 * opcode: 03
 * operation_type: ['jmp', 'cond']
**/
bool St62Architecture::Table_1_03(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
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
    if (Value & 0x10)
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_C);
      rInsn.SetUpdatedFlags(ST62_Flg_C);
      /* semantic: cf.id = bit_cast(op1.val >> op0.val, int1(1)) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          rInsn.GetOperand(1),
          rInsn.GetOperand(0)), Expr::MakeBitVector(1, 0x1))));
      /* semantic: if __expr and cf.id == int1(1): program.id = op2.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x1),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(2)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    else
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_C);
      rInsn.SetUpdatedFlags(ST62_Flg_C);
      /* semantic: cf.id = bit_cast(op1.val >> op0.val, int1(1)) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          rInsn.GetOperand(1),
          rInsn.GetOperand(0)), Expr::MakeBitVector(1, 0x1))));
      /* semantic: if __expr and cf.id == int1(0): program.id = op2.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x0),
        Expr::MakeAssign(
          Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
          rInsn.GetOperand(2)), nullptr)
      );
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: nop
 * opcode: 04
 * semantic: program.id = program.id

**/
bool St62Architecture::Table_1_04(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Nop);
    {
      Expression::LSPType AllExpr;
      /* semantic: program.id = program.id
       */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo)));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 05
 * reference: table_5
**/
bool St62Architecture::Table_1_05(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * test_flags: ['cf']
 * semantic: if __expr and cf.id == int1(1): program.id = op0.val;

 * operation_type: ['jmp', 'cond']
 * opcode: 06
**/
bool St62Architecture::Table_1_06(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Jrc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_C);
      /* semantic: if __expr and cf.id == int1(1): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x1),
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
bool St62Architecture::Table_1_07(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * test_flags: ['zf']
 * semantic: if __expr and zf.id == int1(0): program.id = op0.val;

 * operation_type: ['jmp', 'cond']
 * opcode: 08
**/
bool St62Architecture::Table_1_08(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnz);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_Z);
      /* semantic: if __expr and zf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x0),
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
bool St62Architecture::Table_1_09(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
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
 * test_flags: ['cf']
 * semantic: if __expr and cf.id == int1(0): program.id = op0.val;

 * operation_type: ['jmp', 'cond']
 * opcode: 0a
**/
bool St62Architecture::Table_1_0a(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Jrnc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_C);
      /* semantic: if __expr and cf.id == int1(0): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x0),
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
 * semantic: op1.val = op1.val & ~(int8(1) << op0.val)

 * operand: ['bitdirect', 'direct']
 * opcode: 0b
 * alternate: set
 * semantic_alt: op1.val = op1.val | (int8(1) << op0.val)

**/
bool St62Architecture::Table_1_0b(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
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
    if (Value & 0x10)
    {
      Expression::LSPType AllExpr;
      /* semantic: op1.val = op1.val | (int8(1) << op0.val)
       */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(1),
        Expr::MakeBinOp(
          OperationExpression::OpOr,
          rInsn.GetOperand(1),
          Expr::MakeBinOp(
            OperationExpression::OpLls,
            Expr::MakeBitVector(8, 0x1),
            rInsn.GetOperand(0)))));
      rInsn.SetSemantic(AllExpr);
    }
    else
    {
      Expression::LSPType AllExpr;
      /* semantic: op1.val = op1.val & ~(int8(1) << op0.val)
       */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(1),
        Expr::MakeBinOp(
          OperationExpression::OpAnd,
          rInsn.GetOperand(1),
          Expr::MakeUnOp(
            OperationExpression::OpNot,
            Expr::MakeBinOp(
              OperationExpression::OpLls,
              Expr::MakeBitVector(8, 0x1),
              rInsn.GetOperand(0))))));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: jrz
 * operand: ['pcr']
 * test_flags: ['zf']
 * semantic: if __expr and zf.id == int1(1): program.id = op0.val;

 * operation_type: ['jmp', 'cond']
 * opcode: 0c
**/
bool St62Architecture::Table_1_0c(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Jrz);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_Z);
      /* semantic: if __expr and zf.id == int1(1): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x1),
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
bool St62Architecture::Table_1_0d(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
 * test_flags: ['cf']
 * semantic: if __expr and cf.id == int1(1): program.id = op0.val;

 * operation_type: ['jmp', 'cond']
 * opcode: 0e
**/
bool St62Architecture::Table_1_0e(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Jrc);
    rInsn.SubType() |= Instruction::JumpType | Instruction::ConditionalType;
    if (Operand__pcr(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_C);
      /* semantic: if __expr and cf.id == int1(1): program.id = op0.val */
      AllExpr.push_back(Expr::MakeIfElseCond(
        ConditionExpression::CondEq,
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBitVector(1, 0x1),
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
bool St62Architecture::Table_1_0f(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
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
bool St62Architecture::Table_5_00(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['X']
 * update_flags: ['zf']
 * opcode: 01
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val + op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_5_01(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__X(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val + op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAdd,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 02
 * invalid: True
**/
bool St62Architecture::Table_5_02(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'X']
 * update_flags: ['zf']
 * opcode: 03
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_5_03(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__X(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 04
 * invalid: True
**/
bool St62Architecture::Table_5_04(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['Y']
 * update_flags: ['zf']
 * opcode: 05
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val + op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_5_05(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val + op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAdd,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 06
 * invalid: True
**/
bool St62Architecture::Table_5_06(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'Y']
 * update_flags: ['zf']
 * opcode: 07
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_5_07(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 08
 * invalid: True
**/
bool St62Architecture::Table_5_08(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['V']
 * update_flags: ['zf']
 * opcode: 09
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val + op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_5_09(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val + op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAdd,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 0a
 * invalid: True
**/
bool St62Architecture::Table_5_0a(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'V']
 * update_flags: ['zf']
 * opcode: 0b
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_5_0b(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 0c
 * invalid: True
**/
bool St62Architecture::Table_5_0c(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: inc
 * operand: ['W']
 * update_flags: ['zf']
 * opcode: 0d
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val + op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_5_0d(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val + op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAdd,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 0e
 * invalid: True
**/
bool St62Architecture::Table_5_0e(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'W']
 * update_flags: ['zf']
 * opcode: 0f
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_5_0f(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'rX']
 * update_flags: ['zf']
 * opcode: 00
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_7_00(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ldi
 * operand: ['A', 'imm']
 * update_flags: ['zf']
 * opcode: 01
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_7_01(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ldi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: cp
 * operand: ['A', 'rX']
 * update_flags: ['zf', 'cf']
 * opcode: 02
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
free_var('res');

**/
bool St62Architecture::Table_7_02(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Cp);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      free_var('res') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: cpi
 * operand: ['A', 'imm']
 * update_flags: ['zf', 'cf']
 * opcode: 03
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
free_var('res');

**/
bool St62Architecture::Table_7_03(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Cpi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      free_var('res') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: add
 * operand: ['A', 'rX']
 * update_flags: ['zf', 'cf']
 * opcode: 04
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
call('carry_flag_add');
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_7_04(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Add);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: call('carry_flag_add') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          Expr::MakeBinOp(
            OperationExpression::OpXor,
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              rInsn.GetOperand(0),
              rInsn.GetOperand(1)),
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                Expr::MakeBinOp(
                  OperationExpression::OpXor,
                  rInsn.GetOperand(0),
                  rInsn.GetOperand(1)),
                Expr::MakeVar("res", VariableExpression::Use)),
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                rInsn.GetOperand(0),
                rInsn.GetOperand(1)))),
          Expr::MakeBinOp(
            OperationExpression::OpSub,
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), rInsn.GetOperand(0)->GetBitSize()),
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1))), Expr::MakeBitVector(1, 0x1))));
      /* semantic: op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: addi
 * operand: ['A', 'imm']
 * update_flags: ['zf', 'cf']
 * opcode: 05
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
call('carry_flag_add');
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_7_05(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Addi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: call('carry_flag_add') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          Expr::MakeBinOp(
            OperationExpression::OpXor,
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              rInsn.GetOperand(0),
              rInsn.GetOperand(1)),
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                Expr::MakeBinOp(
                  OperationExpression::OpXor,
                  rInsn.GetOperand(0),
                  rInsn.GetOperand(1)),
                Expr::MakeVar("res", VariableExpression::Use)),
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                rInsn.GetOperand(0),
                rInsn.GetOperand(1)))),
          Expr::MakeBinOp(
            OperationExpression::OpSub,
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), rInsn.GetOperand(0)->GetBitSize()),
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1))), Expr::MakeBitVector(1, 0x1))));
      /* semantic: op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: inc
 * operand: ['rX']
 * update_flags: ['zf']
 * opcode: 06
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val + op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_7_06(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val + op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAdd,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 07
 * invalid: True
**/
bool St62Architecture::Table_7_07(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ld
 * operand: ['rX', 'A']
 * update_flags: ['zf']
 * opcode: 08
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_7_08(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 09
 * invalid: True
**/
bool St62Architecture::Table_7_09(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: and
 * operand: ['A', 'rX']
 * update_flags: ['zf']
 * opcode: 0a
 * semantic: alloc_var('res', op0.bit);
res = op0.val & op1.val;
call('zero_flag')
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_7_0a(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_And);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val & op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAnd,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag')
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: andi
 * operand: ['A', 'imm']
 * update_flags: ['zf']
 * opcode: 0b
 * semantic: alloc_var('res', op0.bit);
res = op0.val & op1.val;
call('zero_flag')
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_7_0b(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Andi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val & op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAnd,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag')
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: sub
 * operand: ['A', 'rX']
 * update_flags: ['zf', 'cf']
 * opcode: 0c
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_7_0c(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Sub);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: subi
 * operand: ['A', 'imm']
 * update_flags: ['zf', 'cf']
 * opcode: 0d
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_7_0d(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Subi);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['rX']
 * update_flags: ['zf']
 * opcode: 0e
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val - op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_7_0e(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__rX(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val - op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 0f
 * invalid: True
**/
bool St62Architecture::Table_7_0f(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: ldi
 * operand: ['direct', 'imm']
 * opcode: 00
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_d_00(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ldi);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__imm(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: set
 * operand: ['bitdirect', 'direct']
 * opcode: 01
**/
bool St62Architecture::Table_d_01(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
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
 * semantic: alloc_var('res', op0.bit);
cf.id = bit_cast(op0.val >> (int(op0.bit, op0.bit) - int(op0.bit, 1)), int1(1));
res = ~op0.val;
op0.val = res;
call('zero_flag')
free_var('res');

**/
bool St62Architecture::Table_d_02(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Com);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: cf.id = bit_cast(op0.val >> (int(op0.bit, op0.bit) - int(op0.bit, 1)), int1(1)) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          rInsn.GetOperand(0),
          Expr::MakeBinOp(
            OperationExpression::OpSub,
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), rInsn.GetOperand(0)->GetBitSize()),
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1))), Expr::MakeBitVector(1, 0x1))));
      /* semantic: res = ~op0.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeUnOp(
          OperationExpression::OpNot,
          rInsn.GetOperand(0))));
      /* semantic: op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: call('zero_flag')
      free_var('res') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['X', 'A']
 * update_flags: ['zf']
 * opcode: 03
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_d_03(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__X(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: reti
 * operation_type: ['ret']
 * update_flags: ['zf', 'cf']
 * opcode: 04
 * semantic: program.id = stk0.id;
stk0.id = stk1.id;
stk1.id = stk2.id;
stk2.id = stk3.id;
stk3.id = stk4.id;
stk4.id = stk5.id;

**/
bool St62Architecture::Table_d_04(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Reti);
    rInsn.SubType() |= Instruction::ReturnType;
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: program.id = stk0.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk0, &m_CpuInfo)));
      /* semantic: stk0.id = stk1.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk0, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk1, &m_CpuInfo)));
      /* semantic: stk1.id = stk2.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk1, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk2, &m_CpuInfo)));
      /* semantic: stk2.id = stk3.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk2, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk3, &m_CpuInfo)));
      /* semantic: stk3.id = stk4.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk3, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk4, &m_CpuInfo)));
      /* semantic: stk4.id = stk5.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk4, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk5, &m_CpuInfo)));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['Y']
 * update_flags: ['zf']
 * opcode: 05
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val - op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_d_05(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val - op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: stop
 * opcode: 06
**/
bool St62Architecture::Table_d_06(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Stop);
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['Y', 'A']
 * update_flags: ['zf']
 * opcode: 07
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_d_07(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__Y(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * opcode: 08
 * invalid: True
**/
bool St62Architecture::Table_d_08(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    return false; /* INVALID */
}

/** instruction
 * mnemonic: dec
 * operand: ['V']
 * update_flags: ['zf']
 * opcode: 09
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val - op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_d_09(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val - op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: rlc
 * test_flags: ['cf']
 * semantic: alloc_var('res', op0.bit);
cf.id = bit_cast(op0.val >> (int(op0.bit, op0.bit) - int(op0.bit, 1)), int1(1));
res.val = op0.val << int(op0.bit, 1) + bit_cast(cf.id, int(op0.bit, op0.bit));
op0.val = res.val;
free_var('res');

 * operand: ['A']
 * opcode: 0a
 * update_flags: ['zf', 'cf']
**/
bool St62Architecture::Table_d_0a(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Rlc);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetTestedFlags(ST62_Flg_C);
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: cf.id = bit_cast(op0.val >> (int(op0.bit, op0.bit) - int(op0.bit, 1)), int1(1)) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          rInsn.GetOperand(0),
          Expr::MakeBinOp(
            OperationExpression::OpSub,
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), rInsn.GetOperand(0)->GetBitSize()),
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1))), Expr::MakeBitVector(1, 0x1))));
      /* semantic: res.val = op0.val << int(op0.bit, 1) + bit_cast(cf.id, int(op0.bit, op0.bit)) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpLls,
          rInsn.GetOperand(0),
          Expr::MakeBinOp(
            OperationExpression::OpAdd,
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1),
            Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeId(ST62_Flg_C, &m_CpuInfo), Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), rInsn.GetOperand(0)->GetBitSize()))))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['V', 'A']
 * update_flags: ['zf']
 * opcode: 0b
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_d_0b(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__V(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ret
 * operation_type: ['ret']
 * opcode: 0c
 * semantic: program.id = stk0.id;
stk0.id = stk1.id;
stk1.id = stk2.id;
stk2.id = stk3.id;
stk3.id = stk4.id;
stk4.id = stk5.id;

**/
bool St62Architecture::Table_d_0c(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ret);
    rInsn.SubType() |= Instruction::ReturnType;
    {
      Expression::LSPType AllExpr;
      /* semantic: program.id = stk0.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()), &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk0, &m_CpuInfo)));
      /* semantic: stk0.id = stk1.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk0, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk1, &m_CpuInfo)));
      /* semantic: stk1.id = stk2.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk1, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk2, &m_CpuInfo)));
      /* semantic: stk2.id = stk3.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk2, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk3, &m_CpuInfo)));
      /* semantic: stk3.id = stk4.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk3, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk4, &m_CpuInfo)));
      /* semantic: stk4.id = stk5.id */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Reg_Stk4, &m_CpuInfo),
        Expr::MakeId(ST62_Reg_Stk5, &m_CpuInfo)));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['W']
 * update_flags: ['zf']
 * opcode: 0d
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val - op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_d_0d(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val - op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: wait
 * opcode: 0e
**/
bool St62Architecture::Table_d_0e(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Wait);
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['W', 'A']
 * update_flags: ['zf']
 * opcode: 0f
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_d_0f(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__W(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'rY']
 * update_flags: ['zf']
 * opcode: 00
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_f_00(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['A', 'direct']
 * update_flags: ['zf']
 * opcode: 01
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_f_01(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: cp
 * operand: ['A', 'rY']
 * update_flags: ['zf', 'cf']
 * opcode: 02
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
free_var('res');

**/
bool St62Architecture::Table_f_02(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Cp);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      free_var('res') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: cp
 * operand: ['A', 'direct']
 * update_flags: ['zf', 'cf']
 * opcode: 03
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
free_var('res');

**/
bool St62Architecture::Table_f_03(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Cp);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      free_var('res') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: add
 * operand: ['A', 'rY']
 * update_flags: ['zf', 'cf']
 * opcode: 04
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
call('carry_flag_add');
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_f_04(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Add);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: call('carry_flag_add') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          Expr::MakeBinOp(
            OperationExpression::OpXor,
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              rInsn.GetOperand(0),
              rInsn.GetOperand(1)),
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                Expr::MakeBinOp(
                  OperationExpression::OpXor,
                  rInsn.GetOperand(0),
                  rInsn.GetOperand(1)),
                Expr::MakeVar("res", VariableExpression::Use)),
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                rInsn.GetOperand(0),
                rInsn.GetOperand(1)))),
          Expr::MakeBinOp(
            OperationExpression::OpSub,
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), rInsn.GetOperand(0)->GetBitSize()),
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1))), Expr::MakeBitVector(1, 0x1))));
      /* semantic: op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: add
 * operand: ['A', 'direct']
 * update_flags: ['zf', 'cf']
 * opcode: 05
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
call('carry_flag_add');
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_f_05(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Add);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: call('carry_flag_add') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeBinOp(OperationExpression::OpBcast, Expr::MakeBinOp(
          OperationExpression::OpLrs,
          Expr::MakeBinOp(
            OperationExpression::OpXor,
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              rInsn.GetOperand(0),
              rInsn.GetOperand(1)),
            Expr::MakeBinOp(
              OperationExpression::OpAnd,
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                Expr::MakeBinOp(
                  OperationExpression::OpXor,
                  rInsn.GetOperand(0),
                  rInsn.GetOperand(1)),
                Expr::MakeVar("res", VariableExpression::Use)),
              Expr::MakeBinOp(
                OperationExpression::OpXor,
                rInsn.GetOperand(0),
                rInsn.GetOperand(1)))),
          Expr::MakeBinOp(
            OperationExpression::OpSub,
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), rInsn.GetOperand(0)->GetBitSize()),
            Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1))), Expr::MakeBitVector(1, 0x1))));
      /* semantic: op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: inc
 * operand: ['rY']
 * update_flags: ['zf']
 * opcode: 06
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val + op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_f_06(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val + op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAdd,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: inc
 * operand: ['direct']
 * update_flags: ['zf']
 * opcode: 07
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val + op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_f_07(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Inc);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val + op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAdd,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['rY', 'A']
 * update_flags: ['zf']
 * opcode: 08
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_f_08(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: ld
 * operand: ['direct', 'A']
 * update_flags: ['zf']
 * opcode: 09
 * semantic: alloc_var('res', op0.bit);
res.val = op1.val;
call('zero_flag');
op0.val = res.val;
free_var('res');

**/
bool St62Architecture::Table_f_09(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Ld);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res.val = op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        rInsn.GetOperand(1)));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: and
 * operand: ['A', 'rY']
 * update_flags: ['zf']
 * opcode: 0a
 * semantic: alloc_var('res', op0.bit);
res = op0.val & op1.val;
call('zero_flag')
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_f_0a(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_And);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val & op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAnd,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag')
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: and
 * operand: ['A', 'direct']
 * update_flags: ['zf']
 * opcode: 0b
 * semantic: alloc_var('res', op0.bit);
res = op0.val & op1.val;
call('zero_flag')
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_f_0b(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_And);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val & op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpAnd,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag')
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: sub
 * operand: ['A', 'rY']
 * update_flags: ['zf', 'cf']
 * opcode: 0c
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_f_0c(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Sub);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: sub
 * operand: ['A', 'direct']
 * update_flags: ['zf', 'cf']
 * opcode: 0d
 * semantic: alloc_var('res', op0.bit);
res = op0.val - op1.val;
call('zero_flag');
cf.id = ite(op0.val < op1.val, int1(1), int1(0))
op0.val = res;
free_var('res');

**/
bool St62Architecture::Table_f_0d(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Sub);
    if (Operand__A(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z | ST62_Flg_C);
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: res = op0.val - op1.val */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          rInsn.GetOperand(1))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: cf.id = ite(op0.val < op1.val, int1(1), int1(0))
      op0.val = res */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_C, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        rInsn.GetOperand(0),
        rInsn.GetOperand(1),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['rY']
 * update_flags: ['zf']
 * opcode: 0e
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val - op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_f_0e(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__rY(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val - op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

/** instruction
 * mnemonic: dec
 * operand: ['direct']
 * update_flags: ['zf']
 * opcode: 0f
 * semantic: alloc_var('op1', op0.bit);
alloc_var('res', op0.bit);
op1 = int(op0.bit, 1);
res = op0.val - op1;
call('zero_flag');
op0.val = res.val;
free_var('op1');
free_var('res');

**/
bool St62Architecture::Table_f_0f(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
    rInsn.Size()++;
    rInsn.SetOpcode(ST62_Opcode_Dec);
    if (Operand__direct(rBinStrm, Offset, rInsn, Mode) == false)
    {
      return false;
    }
    {
      Expression::LSPType AllExpr;
      rInsn.SetUpdatedFlags(ST62_Flg_Z);
      /* semantic: alloc_var('op1', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: alloc_var('res', op0.bit) */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Alloc, rInsn.GetOperand(0)->GetBitSize()));
      /* semantic: op1 = int(op0.bit, 1) */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("op1", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x1)));
      /* semantic: res = op0.val - op1 */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBinOp(
          OperationExpression::OpSub,
          rInsn.GetOperand(0),
          Expr::MakeVar("op1", VariableExpression::Use))));
      /* semantic: call('zero_flag') */
      AllExpr.push_back(Expr::MakeAssign(
        Expr::MakeId(ST62_Flg_Z, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        Expr::MakeVar("res", VariableExpression::Use),
        Expr::MakeBitVector(rInsn.GetOperand(0)->GetBitSize(), 0x0),
        Expr::MakeBitVector(1, 0x1), Expr::MakeBitVector(1, 0x0))));
      /* semantic: op0.val = res.val */
      AllExpr.push_back(Expr::MakeAssign(
        rInsn.GetOperand(0),
        Expr::MakeVar("res", VariableExpression::Use)));
      /* semantic: free_var('op1') */
      AllExpr.push_back(Expr::MakeVar("op1", VariableExpression::Free));
      /* semantic: free_var('res') */
      AllExpr.push_back(Expr::MakeVar("res", VariableExpression::Free));
      rInsn.SetSemantic(AllExpr);
    }
    return true;
}

