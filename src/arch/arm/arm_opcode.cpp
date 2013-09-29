/* This file has been automatically generated, you must _NOT_ edit it directly. (Sun Sep 29 19:23:04 2013) */
#include "arm_architecture.hpp"
const char *ArmArchitecture::m_Mnemonic[0x23] =
{
  "unknown",
  "adc",
  "add",
  "and",
  "b",
  "bic",
  "bx",
  "cdp",
  "cmn",
  "cmp",
  "eor",
  "ldc",
  "ldm",
  "ldr",
  "mcr",
  "mla",
  "mlal",
  "mov",
  "mrc",
  "mrs",
  "msr",
  "mul",
  "mull",
  "mvn",
  "orr",
  "rsb",
  "rsc",
  "sbc",
  "stc",
  "stm",
  "str",
  "sub",
  "swi",
  "teq",
  "tst"
};
bool ArmArchitecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u32 Opcode;
  rBinStrm.Read(Offset, Opcode);

  if ((Opcode & 0x0fe00000) == 0x00a00000)
    return InstructionAdc_0fe00000_00a00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02a00000)
    return InstructionAdc_0fe00000_02a00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x00800000)
    return InstructionAdd_0fe00000_00800000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02800000)
    return InstructionAdd_0fe00000_02800000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x00000000)
    return InstructionAnd_0fe00000_00000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02000000)
    return InstructionAnd_0fe00000_02000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e000000) == 0x0a000000)
    return InstructionB_0e000000_0a000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01c00000)
    return InstructionBic_0fe00000_01c00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03c00000)
    return InstructionBic_0fe00000_03c00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0ffffff0) == 0x012fff10)
    return InstructionBx_0ffffff0_012fff10(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0f000010) == 0x0e000000)
    return InstructionCdp_0f000010_0e000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01600000)
    return InstructionCmn_0fe00000_01600000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03600000)
    return InstructionCmn_0fe00000_03600000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01400000)
    return InstructionCmp_0fe00000_01400000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03400000)
    return InstructionCmp_0fe00000_03400000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x00200000)
    return InstructionEor_0fe00000_00200000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02200000)
    return InstructionEor_0fe00000_02200000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x0c100000)
    return InstructionLdc_0e100000_0c100000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x08100000)
    return InstructionLdm_0e100000_08100000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x06100000)
    return InstructionLdr_0e100000_06100000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x04100000)
    return InstructionLdr_0e100000_04100000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00500090)
    return InstructionLdr_0e500f90_00500090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00100090)
    return InstructionLdr_0e500f90_00100090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0f100010) == 0x0e000010)
    return InstructionMcr_0f100010_0e000010(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe000f0) == 0x00200090)
    return InstructionMla_0fe000f0_00200090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fa000f0) == 0x00a00090)
    return InstructionMlal_0fa000f0_00a00090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01a00000)
    return InstructionMov_0fe00000_01a00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03a00000)
    return InstructionMov_0fe00000_03a00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0f100010) == 0x0e100010)
    return InstructionMrc_0f100010_0e100010(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fbf0fff) == 0x010f0000)
    return InstructionMrs_0fbf0fff_010f0000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0db0f000) == 0x0120f000)
    return InstructionMsr_0db0f000_0120f000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0db0f000) == 0x0160f000)
    return InstructionMsr_0db0f000_0160f000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe000f0) == 0x00000090)
    return InstructionMul_0fe000f0_00000090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fa000f0) == 0x00800090)
    return InstructionMull_0fa000f0_00800090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01e00000)
    return InstructionMvn_0fe00000_01e00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03e00000)
    return InstructionMvn_0fe00000_03e00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01800000)
    return InstructionOrr_0fe00000_01800000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03800000)
    return InstructionOrr_0fe00000_03800000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x00600000)
    return InstructionRsb_0fe00000_00600000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02600000)
    return InstructionRsb_0fe00000_02600000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x00e00000)
    return InstructionRsc_0fe00000_00e00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02e00000)
    return InstructionRsc_0fe00000_02e00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x00c00000)
    return InstructionSbc_0fe00000_00c00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02c00000)
    return InstructionSbc_0fe00000_02c00000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x0c000000)
    return InstructionStc_0e100000_0c000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x08000000)
    return InstructionStm_0e100000_08000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x06000000)
    return InstructionStr_0e100000_06000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e100000) == 0x04000000)
    return InstructionStr_0e100000_04000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00400090)
    return InstructionStr_0e500f90_00400090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00000090)
    return InstructionStr_0e500f90_00000090(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x00400000)
    return InstructionSub_0fe00000_00400000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x02400000)
    return InstructionSub_0fe00000_02400000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0f000000) == 0x0f000000)
    return InstructionSwi_0f000000_0f000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01200000)
    return InstructionTeq_0fe00000_01200000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03200000)
    return InstructionTeq_0fe00000_03200000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x01000000)
    return InstructionTst_0fe00000_01000000(rBinStrm, Offset, Opcode, rInsn);
  else if ((Opcode & 0x0fe00000) == 0x03000000)
    return InstructionTst_0fe00000_03000000(rBinStrm, Offset, Opcode, rInsn);
  return false;
}
bool ArmArchitecture::InstructionBx_0ffffff0_012fff10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Bx);
  rInsn.Length() += 4;
  rInsn.SubType() |= Instruction::CallType;
  u32 RnField = ExtractBits<0, 3>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: program.id = op0.val */
    new OperationExpression(OperationExpression::OpAff,
      new IdentifierExpression(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister), &m_CpuInfo),
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionB_0e000000_0a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_B);
  rInsn.Length() += 4;
  rInsn.SubType() |= Instruction::JumpType;
  u32 OffField = ExtractBits<0, 23>(Opcode);
  u32 LField = ExtractBit<24>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REL32);
  rInsn.Operand(0)->SetValue(SignExtend<s64, 25>((OffField << 2) + 4)); /* NOTE: +8 for prefetch -4 */
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: program.id = program.id + op0.val */
    new OperationExpression(OperationExpression::OpAff,
      new IdentifierExpression(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister), &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpAdd,
        new IdentifierExpression(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister), &m_CpuInfo),
        rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionAnd_0fe00000_00000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_And);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val & op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAnd,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionEor_0fe00000_00200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Eor);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val ^ op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpXor,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionSub_0fe00000_00400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Sub);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val - op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpSub,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionRsb_0fe00000_00600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Rsb);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op2.val - op1.val - int(op0.bit, 1) + cf.id */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpSub,
          new OperationExpression(
            OperationExpression::OpSub,
            rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
            rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
          new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0x1)),
        new IdentifierExpression(ARM_FlCf, &m_CpuInfo)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionAdd_0fe00000_00800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Add);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val + op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionAdc_0fe00000_00a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Adc);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val + op2.val + cf.id */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpAdd,
          rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
          rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
        new IdentifierExpression(ARM_FlCf, &m_CpuInfo)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionSbc_0fe00000_00c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Sbc);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val - op2.val - int(op0.bit, 1) + cf.id */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpSub,
          new OperationExpression(
            OperationExpression::OpSub,
            rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
            rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
          new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0x1)),
        new IdentifierExpression(ARM_FlCf, &m_CpuInfo)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionRsc_0fe00000_00e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Rsc);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op2.val - op1.val - int(op0.bit, 1) + cf.id */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpSub,
          new OperationExpression(
            OperationExpression::OpSub,
            rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
            rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
          new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0x1)),
        new IdentifierExpression(ARM_FlCf, &m_CpuInfo)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionTst_0fe00000_01000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Tst);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  return true;
}
bool ArmArchitecture::InstructionTeq_0fe00000_01200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Teq);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  return true;
}
bool ArmArchitecture::InstructionCmp_0fe00000_01400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cmp);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  return true;
}
bool ArmArchitecture::InstructionCmn_0fe00000_01600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cmn);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  return true;
}
bool ArmArchitecture::InstructionOrr_0fe00000_01800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Orr);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val | op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpOr,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionMov_0fe00000_01a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mov);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  if (RdField & ARM_RegPC)
  {
    rInsn.SubType() |= Instruction::ReturnType;
  }
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionBic_0fe00000_01c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Bic);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op2.val ^ int(op0.bit, 0xffffffff) & op1.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpXor,
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        new OperationExpression(
          OperationExpression::OpAnd,
          new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0xffffffff),
          rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true))));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionMvn_0fe00000_01e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mvn);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = int(op0.bit, 0xffffffff) ^ op1.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpXor,
        new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0xffffffff),
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionAnd_0fe00000_02000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_And);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val & op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAnd,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionEor_0fe00000_02200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Eor);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val ^ op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpXor,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionSub_0fe00000_02400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Sub);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val - op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpSub,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionRsb_0fe00000_02600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Rsb);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op2.val - op1.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpSub,
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionAdd_0fe00000_02800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Add);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val + op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionAdc_0fe00000_02a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Adc);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val + op2.val + cf.id */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpAdd,
          rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
          rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
        new IdentifierExpression(ARM_FlCf, &m_CpuInfo)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionSbc_0fe00000_02c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Sbc);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val - op2.val - int(op0.bit, 1) + cf.id */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpSub,
          new OperationExpression(
            OperationExpression::OpSub,
            rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
            rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
          new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0x1)),
        new IdentifierExpression(ARM_FlCf, &m_CpuInfo)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionRsc_0fe00000_02e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Rsc);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op2.val - op1.val - int(op0.bit, 1) + cf.id */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpSub,
          new OperationExpression(
            OperationExpression::OpSub,
            rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
            rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
          new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0x1)),
        new IdentifierExpression(ARM_FlCf, &m_CpuInfo)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionTst_0fe00000_03000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Tst);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_IMM32);
  rInsn.Operand(1)->SetValue(ImmField);
  return true;
}
bool ArmArchitecture::InstructionTeq_0fe00000_03200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Teq);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_IMM32);
  rInsn.Operand(1)->SetValue(ImmField);
  return true;
}
bool ArmArchitecture::InstructionCmp_0fe00000_03400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cmp);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_IMM32);
  rInsn.Operand(1)->SetValue(ImmField);
  return true;
}
bool ArmArchitecture::InstructionCmn_0fe00000_03600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cmn);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_IMM32);
  rInsn.Operand(1)->SetValue(ImmField);
  return true;
}
bool ArmArchitecture::InstructionOrr_0fe00000_03800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Orr);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val | op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpOr,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionMov_0fe00000_03a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mov);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  if (RdField & ARM_RegPC)
  {
    rInsn.SubType() |= Instruction::ReturnType;
  }
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_IMM32);
  rInsn.Operand(1)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionBic_0fe00000_03c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Bic);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op2.val ^ int(op0.bit, 0xffffffff) & op1.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpXor,
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        new OperationExpression(
          OperationExpression::OpAnd,
          new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0xffffffff),
          rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true))));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionMvn_0fe00000_03e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mvn);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_IMM32);
  rInsn.Operand(1)->SetValue(ImmField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = int(op0.bit, 0xffffffff) ^ op1.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpXor,
        new ConstantExpression(rInsn.Operand(0)->GetLength() * 8, 0xffffffff),
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionMrs_0fbf0fff_010f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mrs);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  u32 RnField = ExtractBits<16, 19>(Opcode);
  return true;
}
bool ArmArchitecture::InstructionMsr_0db0f000_0120f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Msr);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  u32 RnField = ExtractBits<16, 19>(Opcode);
  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RmField);
  return true;
}
bool ArmArchitecture::InstructionMsr_0db0f000_0160f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Msr);
  rInsn.Length() += 4;
  u32 ImmField = ExtractBits<0, 7>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 OpcdField = ExtractBits<21, 24>(Opcode);
  u32 RotField = ExtractBits<8, 11>(Opcode);
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RnField);
  rInsn.Operand(2)->SetType(O_IMM32);
  rInsn.Operand(2)->SetValue(ImmField);
  return true;
}
bool ArmArchitecture::InstructionMul_0fe000f0_00000090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mul);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 RnField = ExtractBits<12, 15>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 CondField = ExtractBits<29, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  u32 RsField = ExtractBits<8, 11>(Opcode);
  u32 RdField = ExtractBits<16, 19>(Opcode);
  u32 AField = ExtractBit<21>(Opcode);
  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RsField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val * op2.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpMul,
        rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
        rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionMla_0fe000f0_00200090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mla);
  rInsn.Length() += 4;
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 RnField = ExtractBits<12, 15>(Opcode);
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 CondField = ExtractBits<29, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  u32 RsField = ExtractBits<8, 11>(Opcode);
  u32 RdField = ExtractBits<16, 19>(Opcode);
  u32 AField = ExtractBit<21>(Opcode);
  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(1 << RmField);
  rInsn.Operand(2)->SetType(O_REG32);
  rInsn.Operand(2)->SetReg(1 << RsField);
  rInsn.Operand(3)->SetType(O_REG32);
  rInsn.Operand(3)->SetReg(1 << RnField);
  {
    Expression::List AllExpr;
    auto pExpr0 = /* Semantic: op0.val = op1.val * op2.val + op3.val */
    new OperationExpression(OperationExpression::OpAff,
      rInsn.Operand(0)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
      new OperationExpression(
        OperationExpression::OpAdd,
        new OperationExpression(
          OperationExpression::OpMul,
          rInsn.Operand(1)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true),
          rInsn.Operand(2)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)),
        rInsn.Operand(3)->GetSemantic(&m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)));
    AllExpr.push_back(pExpr0);
    rInsn.SetSemantic(AllExpr);
  }
  return true;
}
bool ArmArchitecture::InstructionMull_0fa000f0_00800090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mull);
  rInsn.Length() += 4;
  u32 RdloField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<8, 11>(Opcode);
  u32 RdhiField = ExtractBits<16, 19>(Opcode);
  u32 AField = ExtractBit<21>(Opcode);
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 UField = ExtractBit<22>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 CondField = ExtractBits<29, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionMlal_0fa000f0_00a00090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mlal);
  rInsn.Length() += 4;
  u32 RdloField = ExtractBits<12, 15>(Opcode);
  u32 RnField = ExtractBits<8, 11>(Opcode);
  u32 RdhiField = ExtractBits<16, 19>(Opcode);
  u32 AField = ExtractBit<21>(Opcode);
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 UField = ExtractBit<22>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 SField = ExtractBit<20>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 CondField = ExtractBits<29, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionStr_0e100000_06000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Str);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 IField = ExtractBit<25>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  if (PField)
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32 | O_SREG);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(1)->SetSecReg(1 << RmField);
  }
  else
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(2)->SetType(O_REG32);
    rInsn.Operand(2)->SetReg(1 << RmField);
  }
  return true;
}
bool ArmArchitecture::InstructionStr_0e100000_04000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Str);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 IField = ExtractBit<25>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 ImmField = ExtractBits<0, 11>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  if (PField)
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32 | O_DISP32);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(1)->SetValue(ImmField);
  }
  else
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(2)->SetType(O_IMM32);
    rInsn.Operand(2)->SetValue(ImmField);
  }
  if ((1 << RnField) & ARM_RegPC)
  {
    u32 Imm;
    rBinStrm.Read(Offset + ImmField + 8, Imm);

    rInsn.Operand(1)->SetValue(Imm);
    rInsn.Operand(1)->Type() &= ~O_REG_PC_REL; // Since we resolve the PC ref manually, we tell it's not relative
  }
  return true;
}
bool ArmArchitecture::InstructionLdr_0e100000_06100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldr);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 IField = ExtractBit<25>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 RdField = ExtractBits<12, 15>(Opcode);
  if (RdField & ARM_RegPC)
  {
    rInsn.SubType() |= Instruction::ReturnType;
  }
  u32 ShfField = ExtractBits<4, 11>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  if (PField)
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32 | O_SREG);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(1)->SetSecReg(1 << RmField);
  }
  else
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(2)->SetType(O_REG32);
    rInsn.Operand(2)->SetReg(1 << RmField);
  }
  return true;
}
bool ArmArchitecture::InstructionLdr_0e100000_04100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldr);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 IField = ExtractBit<25>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 ImmField = ExtractBits<0, 11>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  if (RdField & ARM_RegPC)
  {
    rInsn.SubType() |= Instruction::ReturnType;
  }
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RdField);
  if (PField)
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32 | O_DISP32);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(1)->SetValue(ImmField);
  }
  else
  {
    rInsn.Operand(1)->SetType(O_MEM32 | O_REG32);
    rInsn.Operand(1)->SetReg(1 << RnField);
    rInsn.Operand(2)->SetType(O_IMM32);
    rInsn.Operand(2)->SetValue(ImmField);
  }
  if ((1 << RnField) & ARM_RegPC)
  {
    u32 Imm;
    rBinStrm.Read(Offset + ImmField + 8, Imm);

    rInsn.Operand(1)->SetValue(Imm);
    rInsn.Operand(1)->Type() &= ~O_REG_PC_REL; // Since we resolve the PC ref manually, we tell it's not relative
  }
  return true;
}
bool ArmArchitecture::InstructionStr_0e500f90_00400090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Str);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 HField = ExtractBit<5>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 SField = ExtractBit<6>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  return true;
}
bool ArmArchitecture::InstructionLdr_0e500f90_00500090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldr);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 RmField = ExtractBits<0, 3>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 HField = ExtractBit<5>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  u32 RdField = ExtractBits<12, 15>(Opcode);
  if (RdField & ARM_RegPC)
  {
    rInsn.SubType() |= Instruction::ReturnType;
  }
  u32 SField = ExtractBit<6>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  return true;
}
bool ArmArchitecture::InstructionStr_0e500f90_00000090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Str);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 Off0Field = ExtractBits<0, 3>(Opcode);
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 SField = ExtractBit<6>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 HField = ExtractBit<5>(Opcode);
  u32 Off1Field = ExtractBits<8, 11>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionLdr_0e500f90_00100090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldr);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 Off0Field = ExtractBits<0, 3>(Opcode);
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 SField = ExtractBit<6>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 HField = ExtractBit<5>(Opcode);
  u32 Off1Field = ExtractBits<8, 11>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  if (RdField & ARM_RegPC)
  {
    rInsn.SubType() |= Instruction::ReturnType;
  }
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionStm_0e100000_08000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Stm);
  rInsn.Length() += 4;
  u32 SField = ExtractBit<22>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 RlField = ExtractBits<0, 15>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(RlField);
  return true;
}
bool ArmArchitecture::InstructionLdm_0e100000_08100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldm);
  rInsn.Length() += 4;
  u32 SField = ExtractBit<22>(Opcode);
  if (SField)
  {
    rInsn.Prefix() |= ARM_Prefix_S;
    rInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);
  }
  u32 RlField = ExtractBits<0, 15>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  rInsn.Operand(0)->SetType(O_REG32);
  rInsn.Operand(0)->SetReg(1 << RnField);
  rInsn.Operand(1)->SetType(O_REG32);
  rInsn.Operand(1)->SetReg(RlField);
  return true;
}
bool ArmArchitecture::InstructionSwi_0f000000_0f000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Swi);
  rInsn.Length() += 4;
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionCdp_0f000010_0e000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cdp);
  rInsn.Length() += 4;
  u32 CrmField = ExtractBits<0, 3>(Opcode);
  u32 CpOpcField = ExtractBits<20, 23>(Opcode);
  u32 CrdField = ExtractBits<12, 15>(Opcode);
  u32 CpField = ExtractBits<5, 7>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  u32 CpnField = ExtractBits<8, 11>(Opcode);
  u32 CrnField = ExtractBits<16, 19>(Opcode);
  return true;
}
bool ArmArchitecture::InstructionStc_0e100000_0c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Stc);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 NField = ExtractBit<22>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 OffField = ExtractBits<0, 7>(Opcode);
  u32 CpField = ExtractBits<8, 11>(Opcode);
  u32 CrdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionLdc_0e100000_0c100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldc);
  rInsn.Length() += 4;
  u32 RnField = ExtractBits<16, 19>(Opcode);
  u32 WField = ExtractBit<21>(Opcode);
  if (WField)
  {
    rInsn.Prefix() |= ARM_Prefix_W;
  }
  u32 NField = ExtractBit<22>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 PField = ExtractBit<24>(Opcode);
  if (PField)
  {
    rInsn.Prefix() |= ARM_Prefix_P;
  }
  u32 UField = ExtractBit<23>(Opcode);
  if (UField)
  {
    rInsn.Prefix() |= ARM_Prefix_U;
  }
  u32 OffField = ExtractBits<0, 7>(Opcode);
  u32 CpField = ExtractBits<8, 11>(Opcode);
  u32 CrdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionMcr_0f100010_0e000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mcr);
  rInsn.Length() += 4;
  u32 CpOpcField = ExtractBits<21, 23>(Opcode);
  u32 CpnField = ExtractBits<8, 11>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 CrnField = ExtractBits<16, 19>(Opcode);
  u32 CrmField = ExtractBits<0, 3>(Opcode);
  u32 CpField = ExtractBits<5, 7>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
bool ArmArchitecture::InstructionMrc_0f100010_0e100010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mrc);
  rInsn.Length() += 4;
  u32 CpOpcField = ExtractBits<21, 23>(Opcode);
  u32 CpnField = ExtractBits<8, 11>(Opcode);
  u32 LField = ExtractBit<20>(Opcode);
  if (LField)
  {
    rInsn.Prefix() |= ARM_Prefix_L;
  }
  u32 CrnField = ExtractBits<16, 19>(Opcode);
  u32 CrmField = ExtractBits<0, 3>(Opcode);
  u32 CpField = ExtractBits<5, 7>(Opcode);
  u32 RdField = ExtractBits<12, 15>(Opcode);
  u32 CondField = ExtractBits<28, 31>(Opcode);
  rInsn.SetTestedFlags(CondField);

  return true;
}
