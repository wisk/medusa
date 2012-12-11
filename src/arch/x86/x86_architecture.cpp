#include "x86_architecture.hpp"

char const* X86Architecture::X86CpuInformation::ConvertIdentifierToName(u32 Id) const
{
  switch (Id)
  {
  case X86_FlCf: return "cf"; case X86_FlPf: return "pf"; case X86_FlAf: return "af"; case X86_FlZf: return "zf";
  case X86_FlSf: return "sf"; case X86_FlTf: return "tf"; case X86_FlIf: return "if"; case X86_FlDf: return "df";
  case X86_FlOf: return "of";
  }

  static const char *RegisterName[] =
  {"", "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "spl", "bpl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b",
   "r13b", "r14b", "r15b", "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w",
   "r14w", "r15w", "ip" ,"es", "cs", "ss", "ds", "fs", "gs", "seg6", "seg7", "eax", "ecx", "edx", "ebx", "esp", "ebp",
   "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d", "eip", "cr0", "cr1", "cr2", "cr3", "cr4",
   "cr5", "cr6", "cr7", "cr8", "cr9", "cr10", "cr11", "cr12", "cr13", "cr14", "cr15", "dr0", "dr1", "dr2", "dr3", "dr4",
   "dr5", "dr6", "dr7", "dr8", "dr9", "dr10", "dr11", "dr12", "dr13", "dr14", "dr15", "tr0", "tr1", "tr2", "tr3", "tr4",
   "tr5", "tr6", "tr7", "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13",
   "r14", "r15", "rip", "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7", "mm0", "mm1", "mm2", "mm3", "mm4", "mm5",
   "mm6", "mm7", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12",
   "xmm13", "xmm14", "xmm15" };
  if (Id < (sizeof(RegisterName) / sizeof(*RegisterName)))
    return RegisterName[Id];

  return "";
}

u32 X86Architecture::X86CpuInformation::GetRegisterByType(CpuInformation::Type RegType) const
{
  static const u32 Register16[] = { X86_Reg_Sp,  X86_Reg_Ip,  0 };
  static const u32 Register32[] = { X86_Reg_Esp, X86_Reg_Eip, 0 };
  static const u32 Register64[] = { X86_Reg_Rsp, X86_Reg_Rip, 0 };

  if (RegType < InvalidRegister)
    switch (m_rCfg.Get("Bit"))
  {
    case X86_Bit_16: return Register16[RegType];
    case X86_Bit_32: return Register32[RegType];
    case X86_Bit_64: return Register64[RegType];
  }

  return 0;
}

u32 X86Architecture::X86CpuInformation::GetSizeOfRegisterInBit(u32 Id) const
{
  switch (Id)
  {
  default:
    return 0;

  case X86_Reg_Al:   case X86_Reg_Ah:   case X86_Reg_Bl:   case X86_Reg_Bh:
  case X86_Reg_Cl:   case X86_Reg_Ch:   case X86_Reg_Dl:   case X86_Reg_Dh:
  case X86_Reg_Spl:  case X86_Reg_Bpl:  case X86_Reg_Sil:  case X86_Reg_Dil:
  case X86_Reg_R8b:  case X86_Reg_R9b:  case X86_Reg_R10b: case X86_Reg_R11b:
  case X86_Reg_R12b: case X86_Reg_R13b: case X86_Reg_R14b: case X86_Reg_R15b:
    return 8;

  case X86_Reg_Ax:   case X86_Reg_Bx:   case X86_Reg_Cx:   case X86_Reg_Dx:
  case X86_Reg_Sp:   case X86_Reg_Bp:   case X86_Reg_Si:   case X86_Reg_Di:
  case X86_Reg_R8w:  case X86_Reg_R9w:  case X86_Reg_R10w: case X86_Reg_R11w:
  case X86_Reg_R12w: case X86_Reg_R13w: case X86_Reg_R14w: case X86_Reg_R15w:
  case X86_Reg_Ip:
  case X86_Reg_Cs:   case X86_Reg_Ds:   case X86_Reg_Es:   case X86_Reg_Ss:
  case X86_Reg_Fs:   case X86_Reg_Gs:
    return 16;

  case X86_Reg_Eax:  case X86_Reg_Ebx:  case X86_Reg_Ecx:  case X86_Reg_Edx:
  case X86_Reg_Esp:  case X86_Reg_Ebp:  case X86_Reg_Esi:  case X86_Reg_Edi:
  case X86_Reg_R8d:  case X86_Reg_R9d:  case X86_Reg_R10d: case X86_Reg_R11d:
  case X86_Reg_R12d: case X86_Reg_R13d: case X86_Reg_R14d: case X86_Reg_R15d:
  case X86_Reg_Eip:
    return 32;

  case X86_Reg_Rax:  case X86_Reg_Rbx:  case X86_Reg_Rcx:  case X86_Reg_Rdx:
  case X86_Reg_Rsp:  case X86_Reg_Rbp:  case X86_Reg_Rsi:  case X86_Reg_Rdi:
  case X86_Reg_R8:   case X86_Reg_R9:   case X86_Reg_R10:  case X86_Reg_R11:
  case X86_Reg_R12:  case X86_Reg_R13:  case X86_Reg_R14:  case X86_Reg_R15:
  case X86_Reg_Rip:
    return 64;
  }
}

bool X86Architecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);
  bool Res = (this->*m_Table_1[Opcode])(rBinStrm, Offset + 1, rInsn);
  ApplySegmentOverridePrefix(rInsn);
  return Res;
}

void X86Architecture::FillConfigurationModel(ConfigurationModel& rCfgMdl)
{
  Architecture::FillConfigurationModel(rCfgMdl);

  ConfigurationModel::Enum Bit;
  Bit.push_back(std::make_pair("16-bit", X86_Bit_16));
  Bit.push_back(std::make_pair("32-bit", X86_Bit_32));
  Bit.push_back(std::make_pair("64-bit", X86_Bit_64));
  rCfgMdl.Set("Bit", Bit, X86_Bit_32);

  /* XXX: We don't have enough information about instructions origin to offer this option. */
  //ConfigurationModel::Enum FeatInt;
  //FeatInt.push_back(std::make_pair("8086",  X86_FeatInt_8086 ));
  //FeatInt.push_back(std::make_pair("80186", X86_FeatInt_80186));
  //FeatInt.push_back(std::make_pair("80286", X86_FeatInt_80286));
  //FeatInt.push_back(std::make_pair("80386", X86_FeatInt_80386));
  //FeatInt.push_back(std::make_pair("80486", X86_FeatInt_80486));
  //FeatInt.push_back(std::make_pair("80586", X86_FeatInt_80586));
  //FeatInt.push_back(std::make_pair("80686", X86_FeatInt_80686));
  //FeatInt.push_back(std::make_pair("80786", X86_FeatInt_80786));
  //FeatInt.push_back(std::make_pair("sse",   X86_FeatInt_Sse  ));
  //FeatInt.push_back(std::make_pair("sse2",  X86_FeatInt_Sse2 ));
  //FeatInt.push_back(std::make_pair("sse3",  X86_FeatInt_Sse3 ));
  //FeatInt.push_back(std::make_pair("svm",   X86_FeatInt_Svm  ));
  //FeatInt.push_back(std::make_pair("vmx",   X86_FeatInt_Vmx  ));
  //FeatInt.push_back(std::make_pair("sse4a", X86_FeatInt_Sse4a));
  //rCfgMdl.Set("Integer feature", FeatInt, X86_FeatInt_Sse4a);

  //ConfigurationModel::Enum FeatFp;
  //FeatFp.push_back(std::make_pair("8087",  X86_FeatFp_8087 ));
  //FeatFp.push_back(std::make_pair("80287", X86_FeatFp_80287));
  //FeatFp.push_back(std::make_pair("80387", X86_FeatFp_80387));
  //FeatFp.push_back(std::make_pair("80487", X86_FeatFp_80487));
  //FeatFp.push_back(std::make_pair("80587", X86_FeatFp_80587));
  //rCfgMdl.Set("Floating-point feature", FeatFp, X86_FeatFp_80587);

  //ConfigurationModel::Enum FeatSimd;
  //FeatSimd.push_back(std::make_pair("mmx",     X86_FeatSimd_Mmx    ));
  //FeatSimd.push_back(std::make_pair("mmx+",    X86_FeatSimd_MmxPlus));
  //FeatSimd.push_back(std::make_pair("emmx",    X86_FeatSimd_Emmx   ));
  //FeatSimd.push_back(std::make_pair("3dnow!",  X86_FeatSimd_3dNow  ));
  //FeatSimd.push_back(std::make_pair("sse",     X86_FeatSimd_Sse    ));
  //FeatSimd.push_back(std::make_pair("sse 2",   X86_FeatSimd_Sse2   ));
  //FeatSimd.push_back(std::make_pair("sse 3",   X86_FeatSimd_Sse3   ));
  //FeatSimd.push_back(std::make_pair("ssse 3",  X86_FeatSimd_Ssse3  ));
  //FeatSimd.push_back(std::make_pair("sse 4.1", X86_FeatSimd_Sse41  ));
  //FeatSimd.push_back(std::make_pair("sse 4a",  X86_FeatSimd_Sse4a  ));
  //FeatSimd.push_back(std::make_pair("sse 4.2", X86_FeatSimd_Sse42  ));
  //FeatSimd.push_back(std::make_pair("aes",     X86_FeatSimd_Aes    ));
  //FeatSimd.push_back(std::make_pair("avx",     X86_FeatSimd_Avx    ));
  //rCfgMdl.Set("SIMD feature", FeatSimd, X86_FeatSimd_Avx);

  //rCfgMdl.Set("undocumented instruction", true);
}