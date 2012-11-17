/* This file has been automatically generated, you must _NOT_ edit it directly. (Sat Nov 17 18:30:49 2012) */
enum ARM_Opcode
{
  ARM_Opcode_Unknown,
  ARM_Opcode_Adc,
  ARM_Opcode_Add,
  ARM_Opcode_And,
  ARM_Opcode_B,
  ARM_Opcode_Bic,
  ARM_Opcode_Bx,
  ARM_Opcode_Cdp,
  ARM_Opcode_Cmn,
  ARM_Opcode_Cmp,
  ARM_Opcode_Eor,
  ARM_Opcode_Ldc,
  ARM_Opcode_Ldm,
  ARM_Opcode_Ldr,
  ARM_Opcode_Mcr,
  ARM_Opcode_Mla,
  ARM_Opcode_Mlal,
  ARM_Opcode_Mov,
  ARM_Opcode_Mrc,
  ARM_Opcode_Mrs,
  ARM_Opcode_Msr,
  ARM_Opcode_Mul,
  ARM_Opcode_Mull,
  ARM_Opcode_Mvn,
  ARM_Opcode_Orr,
  ARM_Opcode_Rsb,
  ARM_Opcode_Rsc,
  ARM_Opcode_Sbc,
  ARM_Opcode_Stc,
  ARM_Opcode_Stm,
  ARM_Opcode_Str,
  ARM_Opcode_Sub,
  ARM_Opcode_Swi,
  ARM_Opcode_Teq,
  ARM_Opcode_Tst
};
static char const *m_Mnemonic[0x23];
bool InstructionAdc_0fe00000_00a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionAdc_0fe00000_02a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionAdd_0fe00000_00800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionAdd_0fe00000_02800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionAnd_0fe00000_00000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionAnd_0fe00000_02000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionB_0e000000_0a000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionBic_0fe00000_01c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionBic_0fe00000_03c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionBx_0ffffff0_012fff10(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionCdp_0f000010_0e000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionCmn_0fe00000_01600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionCmn_0fe00000_03600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionCmp_0fe00000_01400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionCmp_0fe00000_03400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionEor_0fe00000_00200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionEor_0fe00000_02200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionLdc_0e100000_0c100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionLdm_0e100000_08100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionLdr_0e100000_06100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionLdr_0e100000_04100000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionLdr_0e500f90_00500090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionLdr_0e500f90_00100090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMcr_0f100010_0e000010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMla_0fe000f0_00200090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMlal_0fa000f0_00a00090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMov_0fe00000_01a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMov_0fe00000_03a00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMrc_0f100010_0e100010(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMrs_0fbf0fff_010f0000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMsr_0db0f000_0120f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMsr_0db0f000_0160f000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMul_0fe000f0_00000090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMull_0fa000f0_00800090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMvn_0fe00000_01e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionMvn_0fe00000_03e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionOrr_0fe00000_01800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionOrr_0fe00000_03800000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionRsb_0fe00000_00600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionRsb_0fe00000_02600000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionRsc_0fe00000_00e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionRsc_0fe00000_02e00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionSbc_0fe00000_00c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionSbc_0fe00000_02c00000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionStc_0e100000_0c000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionStm_0e100000_08000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionStr_0e100000_06000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionStr_0e100000_04000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionStr_0e500f90_00400090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionStr_0e500f90_00000090(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionSub_0fe00000_00400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionSub_0fe00000_02400000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionSwi_0f000000_0f000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionTeq_0fe00000_01200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionTeq_0fe00000_03200000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionTst_0fe00000_01000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
bool InstructionTst_0fe00000_03000000(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn);
