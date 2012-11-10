/* This file has been automatically generated, you must _NOT_ edit it directly. (Sun Nov 11 00:03:13 2012) */
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

  if ((Opcode & 0x0de00000) == 0x00a00000)
    return InstructionAdc_0de00000_00a00000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x00800000)
    return InstructionAdd_0de00000_00800000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x00000000)
    return InstructionAnd_0de00000_00000000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e000000) == 0x0a000000)
    return InstructionB_0e000000_0a000000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01c00000)
    return InstructionBic_0de00000_01c00000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0ffffff0) == 0x012fff10)
    return InstructionBx_0ffffff0_012fff10(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0f000010) == 0x0e000000)
    return InstructionCdp_0f000010_0e000000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01600000)
    return InstructionCmn_0de00000_01600000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01400000)
    return InstructionCmp_0de00000_01400000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x00200000)
    return InstructionEor_0de00000_00200000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e100000) == 0x0c100000)
    return InstructionLdc_0e100000_0c100000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e100000) == 0x08100000)
    return InstructionLdm_0e100000_08100000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e100090) == 0x00100090)
    return InstructionLdr_0e100090_00100090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00100090)
    return InstructionLdr_0e500f90_00100090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00500090)
    return InstructionLdr_0e500f90_00500090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0f100010) == 0x0e000010)
    return InstructionMcr_0f100010_0e000010(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0fe000f0) == 0x00200090)
    return InstructionMla_0fe000f0_00200090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0fa000f0) == 0x00a00090)
    return InstructionMlal_0fa000f0_00a00090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01a00000)
    return InstructionMov_0de00000_01a00000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0f100010) == 0x0e100010)
    return InstructionMrc_0f100010_0e100010(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0fbf0fff) == 0x010f0000)
    return InstructionMrs_0fbf0fff_010f0000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0db0f000) == 0x0120f000)
    return InstructionMsr_0db0f000_0120f000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0fe000f0) == 0x00000090)
    return InstructionMul_0fe000f0_00000090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0fa000f0) == 0x00800090)
    return InstructionMull_0fa000f0_00800090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01e00000)
    return InstructionMvn_0de00000_01e00000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01800000)
    return InstructionOrr_0de00000_01800000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x00600000)
    return InstructionRsb_0de00000_00600000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x00e00000)
    return InstructionRsc_0de00000_00e00000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x00c00000)
    return InstructionSbc_0de00000_00c00000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e100000) == 0x0c000000)
    return InstructionStc_0e100000_0c000000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e100000) == 0x08000000)
    return InstructionStm_0e100000_08000000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e100090) == 0x00000090)
    return InstructionStr_0e100090_00000090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00000090)
    return InstructionStr_0e500f90_00000090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0e500f90) == 0x00400090)
    return InstructionStr_0e500f90_00400090(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x00400000)
    return InstructionSub_0de00000_00400000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0f000000) == 0x0f000000)
    return InstructionSwi_0f000000_0f000000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01200000)
    return InstructionTeq_0de00000_01200000(rBinStrm, Offset, rInsn);
  else if ((Opcode & 0x0de00000) == 0x01000000)
    return InstructionTst_0de00000_01000000(rBinStrm, Offset, rInsn);
  return false;
}
bool ArmArchitecture::InstructionBx_0ffffff0_012fff10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Bx);
  rInsn.SetName("bx");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionB_0e000000_0a000000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_B);
  rInsn.SetName("b");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionAnd_0de00000_00000000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_And);
  rInsn.SetName("and");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionEor_0de00000_00200000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Eor);
  rInsn.SetName("eor");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionSub_0de00000_00400000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Sub);
  rInsn.SetName("sub");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionRsb_0de00000_00600000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Rsb);
  rInsn.SetName("rsb");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionAdd_0de00000_00800000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Add);
  rInsn.SetName("add");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionAdc_0de00000_00a00000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Adc);
  rInsn.SetName("adc");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionSbc_0de00000_00c00000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Sbc);
  rInsn.SetName("sbc");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionRsc_0de00000_00e00000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Rsc);
  rInsn.SetName("rsc");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionTst_0de00000_01000000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Tst);
  rInsn.SetName("tst");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionTeq_0de00000_01200000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Teq);
  rInsn.SetName("teq");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionCmp_0de00000_01400000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cmp);
  rInsn.SetName("cmp");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionCmn_0de00000_01600000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cmn);
  rInsn.SetName("cmn");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionOrr_0de00000_01800000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Orr);
  rInsn.SetName("orr");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMov_0de00000_01a00000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mov);
  rInsn.SetName("mov");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionBic_0de00000_01c00000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Bic);
  rInsn.SetName("bic");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMvn_0de00000_01e00000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mvn);
  rInsn.SetName("mvn");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMrs_0fbf0fff_010f0000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mrs);
  rInsn.SetName("mrs");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMsr_0db0f000_0120f000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Msr);
  rInsn.SetName("msr");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMul_0fe000f0_00000090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mul);
  rInsn.SetName("mul");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMla_0fe000f0_00200090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mla);
  rInsn.SetName("mla");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMull_0fa000f0_00800090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mull);
  rInsn.SetName("mull");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMlal_0fa000f0_00a00090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mlal);
  rInsn.SetName("mlal");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionStr_0e100090_00000090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Str);
  rInsn.SetName("str");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionLdr_0e100090_00100090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldr);
  rInsn.SetName("ldr");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionStr_0e500f90_00000090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Str);
  rInsn.SetName("str");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionLdr_0e500f90_00100090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldr);
  rInsn.SetName("ldr");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionStr_0e500f90_00400090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Str);
  rInsn.SetName("str");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionLdr_0e500f90_00500090(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldr);
  rInsn.SetName("ldr");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionStm_0e100000_08000000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Stm);
  rInsn.SetName("stm");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionLdm_0e100000_08100000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldm);
  rInsn.SetName("ldm");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionSwi_0f000000_0f000000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Swi);
  rInsn.SetName("swi");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionCdp_0f000010_0e000000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Cdp);
  rInsn.SetName("cdp");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionStc_0e100000_0c000000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Stc);
  rInsn.SetName("stc");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionLdc_0e100000_0c100000(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Ldc);
  rInsn.SetName("ldc");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMcr_0f100010_0e000010(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mcr);
  rInsn.SetName("mcr");
  rInsn.Length() += 4;
  return true;
}
bool ArmArchitecture::InstructionMrc_0f100010_0e100010(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetOpcode(ARM_Opcode_Mrc);
  rInsn.SetName("mrc");
  rInsn.Length() += 4;
  return true;
}
