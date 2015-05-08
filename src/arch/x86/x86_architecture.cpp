#include "x86_architecture.hpp"

X86Architecture::X86Architecture(void)
  : Architecture(MEDUSA_ARCH_TAG('x','8','6'))
  , m_CpuInfo()
{
  Configuration::Enum ArchMdl;
  ArchMdl.push_back(std::make_pair("latest", X86_Arch_Latest));
  m_CfgMdl.InsertEnum("Architecture", ArchMdl, X86_Arch_Latest);

  Configuration::Enum VendorMdl;
  VendorMdl.push_back(std::make_pair("AMD", X86_ProcType_AMD));
  VendorMdl.push_back(std::make_pair("Intel", X86_ProcType_INTEL));
  VendorMdl.push_back(std::make_pair("IA64", X86_ProcType_IA64));
  VendorMdl.push_back(std::make_pair("Cyrix", X86_ProcType_CYRIX));
  VendorMdl.push_back(std::make_pair("IIT", X86_ProcType_IIT));
  m_CfgMdl.InsertEnum("Vendor", VendorMdl, X86_ProcType_INTEL);

  /* XXX: We don't have enough information about instructions origin to offer this option. */
  //Configuration::Enum FeatInt;
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
  //m_CfgMdl.InsertEnum("Integer feature", FeatInt, X86_FeatInt_Sse4a);

  //Configuration::Enum FeatFp;
  //FeatFp.push_back(std::make_pair("8087",  X86_FeatFp_8087 ));
  //FeatFp.push_back(std::make_pair("80287", X86_FeatFp_80287));
  //FeatFp.push_back(std::make_pair("80387", X86_FeatFp_80387));
  //FeatFp.push_back(std::make_pair("80487", X86_FeatFp_80487));
  //FeatFp.push_back(std::make_pair("80587", X86_FeatFp_80587));
  //m_CfgMdl.InsertEnum("Floating-point feature", FeatFp, X86_FeatFp_80587);

  //Configuration::Enum FeatSimd;
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
  //m_CfgMdl.InsertEnum("SIMD feature", FeatSimd, X86_FeatSimd_Avx);

  //m_CfgMdl.InsertBool("undocumented instruction", true);
}


char const* X86Architecture::X86CpuInformation::ConvertIdentifierToName(u32 Id) const
{
  switch (Id)
  {
  case X86_FlCf: return "cf"; case X86_FlPf: return "pf"; case X86_FlAf: return "af"; case X86_FlZf: return "zf";
  case X86_FlSf: return "sf"; case X86_FlTf: return "tf"; case X86_FlIf: return "if"; case X86_FlDf: return "df";
  case X86_FlOf: return "of";
  }

  static const char *RegisterName[] =
  {"", "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "spl", "bpl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b",
  "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w", "ip" ,
  "es", "cs", "ss", "ds", "fs", "gs", "seg6", "seg7",
  "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d", "eip",
  "cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7", "cr8", "cr9", "cr10", "cr11", "cr12", "cr13", "cr14", "cr15",
  "dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7", "dr8", "dr9", "dr10", "dr11", "dr12", "dr13", "dr14", "dr15",
  "tr0", "tr1", "tr2", "tr3", "tr4", "tr5", "tr6", "tr7",
  "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rip",
  "st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7",
  "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
  "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15" };
  if (Id < (sizeof(RegisterName) / sizeof(*RegisterName)))
    return RegisterName[Id];

  return "";
}

u32 X86Architecture::X86CpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::unordered_map<std::string, u32> s_NameToId;
  if (s_NameToId.empty())
    {
    s_NameToId["cf"] = X86_FlCf; s_NameToId["pf"] = X86_FlPf; s_NameToId["af"] = X86_FlAf; s_NameToId["zf"] = X86_FlZf;
    s_NameToId["sf"] = X86_FlSf; s_NameToId["tf"] = X86_FlTf; s_NameToId["if"] = X86_FlIf; s_NameToId["df"] = X86_FlDf;
    s_NameToId["of"] = X86_FlOf;
    s_NameToId["al"] = X86_Reg_Al; s_NameToId["cl"] = X86_Reg_Cl; s_NameToId["dl"] = X86_Reg_Dl; s_NameToId["bl"] = X86_Reg_Bl;
    s_NameToId["ah"] = X86_Reg_Ah; s_NameToId["ch"] = X86_Reg_Ch; s_NameToId["dh"] = X86_Reg_Dh; s_NameToId["bh"] = X86_Reg_Bh;
    s_NameToId["spl"] = X86_Reg_Spl; s_NameToId["bpl"] = X86_Reg_Bpl; s_NameToId["sil"] = X86_Reg_Sil; s_NameToId["dil"] = X86_Reg_Dil;
    s_NameToId["r8b"] = X86_Reg_R8b; s_NameToId["r9b"] = X86_Reg_R9b; s_NameToId["r10b"] = X86_Reg_R10b; s_NameToId["r11b"] = X86_Reg_R11b;
    s_NameToId["r12b"] = X86_Reg_R12b; s_NameToId["r13b"] = X86_Reg_R13b; s_NameToId["r14b"] = X86_Reg_R14b; s_NameToId["r15b"] = X86_Reg_R15b;
    s_NameToId["ax"] = X86_Reg_Ax; s_NameToId["cx"] = X86_Reg_Cx; s_NameToId["dx"] = X86_Reg_Dx; s_NameToId["bx"] = X86_Reg_Bx;
    s_NameToId["sp"] = X86_Reg_Sp; s_NameToId["bp"] = X86_Reg_Bp; s_NameToId["si"] = X86_Reg_Si; s_NameToId["di"] = X86_Reg_Di;
    s_NameToId["r8w"] = X86_Reg_R8w; s_NameToId["r9w"] = X86_Reg_R9w; s_NameToId["r10w"] = X86_Reg_R10w; s_NameToId["r11w"] = X86_Reg_R11w;
    s_NameToId["r12w"] = X86_Reg_R12w; s_NameToId["r13w"] = X86_Reg_R13w; s_NameToId["r14w"] = X86_Reg_R14w; s_NameToId["r15w"] = X86_Reg_R15w;
    s_NameToId["ip"] = X86_Reg_Ip; s_NameToId["es"] = X86_Reg_Es; s_NameToId["cs"] = X86_Reg_Cs; s_NameToId["ss"] = X86_Reg_Ss; s_NameToId["ds"] = X86_Reg_Ds;
    s_NameToId["fs"] = X86_Reg_Fs; s_NameToId["gs"] = X86_Reg_Gs;
    s_NameToId["seg6"] = X86_Reg_Seg6; s_NameToId["seg7"] = X86_Reg_Seg7; s_NameToId["eax"] = X86_Reg_Eax; s_NameToId["ecx"] = X86_Reg_Ecx;
    s_NameToId["edx"] = X86_Reg_Edx; s_NameToId["ebx"] = X86_Reg_Ebx; s_NameToId["esp"] = X86_Reg_Esp; s_NameToId["ebp"] = X86_Reg_Ebp;
    s_NameToId["esi"] = X86_Reg_Esi; s_NameToId["edi"] = X86_Reg_Edi; s_NameToId["r8d"] = X86_Reg_R8d; s_NameToId["r9d"] = X86_Reg_R9d;
    s_NameToId["r10d"] = X86_Reg_R10d; s_NameToId["r11d"] = X86_Reg_R11d; s_NameToId["r12d"] = X86_Reg_R12d; s_NameToId["r13d"] = X86_Reg_R13d;
    s_NameToId["r14d"] = X86_Reg_R14d; s_NameToId["r15d"] = X86_Reg_R15d; s_NameToId["eip"] = X86_Reg_Eip;
    s_NameToId["cr0"] = X86_Reg_Cr0; s_NameToId["cr1"] = X86_Reg_Cr1; s_NameToId["cr2"] = X86_Reg_Cr2; s_NameToId["cr3"] = X86_Reg_Cr3;
    s_NameToId["cr4"] = X86_Reg_Cr4; s_NameToId["cr5"] = X86_Reg_Cr5; s_NameToId["cr6"] = X86_Reg_Cr6; s_NameToId["cr7"] = X86_Reg_Cr7;
    s_NameToId["cr8"] = X86_Reg_Cr8; s_NameToId["cr9"] = X86_Reg_Cr9; s_NameToId["cr10"] = X86_Reg_Cr10; s_NameToId["cr11"] = X86_Reg_Cr11;
    s_NameToId["cr12"] = X86_Reg_Cr12; s_NameToId["cr13"] = X86_Reg_Cr13; s_NameToId["cr14"] = X86_Reg_Cr14; s_NameToId["cr15"] = X86_Reg_Cr15;
    s_NameToId["dr0"] = X86_Reg_Dr0; s_NameToId["dr1"] = X86_Reg_Dr1; s_NameToId["dr2"] = X86_Reg_Dr2; s_NameToId["dr3"] = X86_Reg_Dr3;
    s_NameToId["dr4"] = X86_Reg_Dr4; s_NameToId["dr5"] = X86_Reg_Dr5; s_NameToId["dr6"] = X86_Reg_Dr6; s_NameToId["dr7"] = X86_Reg_Dr7;
    s_NameToId["dr8"] = X86_Reg_Dr8; s_NameToId["dr9"] = X86_Reg_Dr9; s_NameToId["dr10"] = X86_Reg_Dr10; s_NameToId["dr11"] = X86_Reg_Dr11;
    s_NameToId["dr12"] = X86_Reg_Dr12; s_NameToId["dr13"] = X86_Reg_Dr13; s_NameToId["dr14"] = X86_Reg_Dr14; s_NameToId["dr15"] = X86_Reg_Dr15;
    s_NameToId["tr0"] = X86_Reg_Tr0; s_NameToId["tr1"] = X86_Reg_Tr1; s_NameToId["tr2"] = X86_Reg_Tr2; s_NameToId["tr3"] = X86_Reg_Tr3;
    s_NameToId["tr4"] = X86_Reg_Tr4; s_NameToId["tr5"] = X86_Reg_Tr5; s_NameToId["tr6"] = X86_Reg_Tr6; s_NameToId["tr7"] = X86_Reg_Tr7;
    s_NameToId["rax"] = X86_Reg_Rax; s_NameToId["rcx"] = X86_Reg_Rcx; s_NameToId["rdx"] = X86_Reg_Rdx; s_NameToId["rbx"] = X86_Reg_Rbx;
    s_NameToId["rsp"] = X86_Reg_Rsp; s_NameToId["rbp"] = X86_Reg_Rbp; s_NameToId["rsi"] = X86_Reg_Rsi; s_NameToId["rdi"] = X86_Reg_Rdi;
    s_NameToId["r8"] = X86_Reg_R8; s_NameToId["r9"] = X86_Reg_R9; s_NameToId["r10"] = X86_Reg_R10; s_NameToId["r11"] = X86_Reg_R11;
    s_NameToId["r12"] = X86_Reg_R12; s_NameToId["r13"] = X86_Reg_R13; s_NameToId["r14"] = X86_Reg_R14; s_NameToId["r15"] = X86_Reg_R15;
    s_NameToId["rip"] = X86_Reg_Rip; s_NameToId["st0"] = X86_Reg_St0; s_NameToId["st1"] = X86_Reg_St1; s_NameToId["st2"] = X86_Reg_St2;
    s_NameToId["st3"] = X86_Reg_St3; s_NameToId["st4"] = X86_Reg_St4; s_NameToId["st5"] = X86_Reg_St5; s_NameToId["st6"] = X86_Reg_St6;
    s_NameToId["st7"] = X86_Reg_St7; s_NameToId["mm0"] = X86_Reg_Mm0; s_NameToId["mm1"] = X86_Reg_Mm1; s_NameToId["mm2"] = X86_Reg_Mm2;
    s_NameToId["mm3"] = X86_Reg_Mm3; s_NameToId["mm4"] = X86_Reg_Mm4; s_NameToId["mm5"] = X86_Reg_Mm5; s_NameToId["mm6"] = X86_Reg_Mm6;
    s_NameToId["mm7"] = X86_Reg_Mm7; s_NameToId["xmm0"] = X86_Reg_Xmm0; s_NameToId["xmm1"] = X86_Reg_Xmm1; s_NameToId["xmm2"] = X86_Reg_Xmm2;
    s_NameToId["xmm3"] = X86_Reg_Xmm3; s_NameToId["xmm4"] = X86_Reg_Xmm4; s_NameToId["xmm5"] = X86_Reg_Xmm5; s_NameToId["xmm6"] = X86_Reg_Xmm6;
    s_NameToId["xmm7"] = X86_Reg_Xmm7; s_NameToId["xmm8"] = X86_Reg_Xmm8; s_NameToId["xmm9"] = X86_Reg_Xmm9; s_NameToId["xmm10"] = X86_Reg_Xmm10;
    s_NameToId["xmm11"] = X86_Reg_Xmm11; s_NameToId["xmm12"] = X86_Reg_Xmm12; s_NameToId["xmm13"] = X86_Reg_Xmm13; s_NameToId["xmm14"] = X86_Reg_Xmm14;
    s_NameToId["xmm15"] = X86_Reg_Xmm15;
    }

  auto itId = s_NameToId.find(rName);
  if (itId == std::end(s_NameToId))
    return 0;

  return itId->second;
}

u32 X86Architecture::X86CpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  /*
    StackPointerRegister,
    StackFrameRegister,
    ProgramBaseRegister,
    ProgramPointerRegister,
    AccumulatorRegister,
    CounterRegister,
    DivisorRegister,
    RemainderRegister,
    InvalidRegister
    */
  static const u32 Register16[] = { X86_Reg_Sp,  X86_Reg_Bp,  X86_Reg_Cs, X86_Reg_Ip,  X86_Reg_Ax,  X86_Reg_Cx,  0, X86_Reg_Ah  };
  static const u32 Register32[] = { X86_Reg_Esp, X86_Reg_Ebp, X86_Reg_Cs, X86_Reg_Eip, X86_Reg_Eax, X86_Reg_Ecx, 0, X86_Reg_Edx };
  static const u32 Register64[] = { X86_Reg_Rsp, X86_Reg_Rbp, X86_Reg_Cs, X86_Reg_Rip, X86_Reg_Rax, X86_Reg_Rcx, 0, X86_Reg_Rdx };

  if (RegType < InvalidRegister)
    switch (Mode)
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

  case X86_FlCf:      case X86_FlPf:      case X86_FlAf:      case X86_FlZf:
  case X86_FlSf:      case X86_FlTf:      case X86_FlIf:      case X86_FlDf:
  case X86_FlOf:
    return 1;

  case X86_Reg_Al:    case X86_Reg_Ah:    case X86_Reg_Bl:    case X86_Reg_Bh:
  case X86_Reg_Cl:    case X86_Reg_Ch:    case X86_Reg_Dl:    case X86_Reg_Dh:
  case X86_Reg_Spl:   case X86_Reg_Bpl:   case X86_Reg_Sil:   case X86_Reg_Dil:
  case X86_Reg_R8b:   case X86_Reg_R9b:   case X86_Reg_R10b:  case X86_Reg_R11b:
  case X86_Reg_R12b:  case X86_Reg_R13b:  case X86_Reg_R14b:  case X86_Reg_R15b:
    return 8;

  case X86_Reg_Ax:    case X86_Reg_Bx:    case X86_Reg_Cx:    case X86_Reg_Dx:
  case X86_Reg_Sp:    case X86_Reg_Bp:    case X86_Reg_Si:    case X86_Reg_Di:
  case X86_Reg_R8w:   case X86_Reg_R9w:   case X86_Reg_R10w:  case X86_Reg_R11w:
  case X86_Reg_R12w:  case X86_Reg_R13w:  case X86_Reg_R14w:  case X86_Reg_R15w:
  case X86_Reg_Ip:
  case X86_Reg_Cs:    case X86_Reg_Ds:    case X86_Reg_Es:    case X86_Reg_Ss:
  case X86_Reg_Fs:    case X86_Reg_Gs:
    return 16;
 
  case X86_Reg_Eax:   case X86_Reg_Ebx:   case X86_Reg_Ecx:   case X86_Reg_Edx:
  case X86_Reg_Esp:   case X86_Reg_Ebp:   case X86_Reg_Esi:   case X86_Reg_Edi:
  case X86_Reg_R8d:   case X86_Reg_R9d:   case X86_Reg_R10d:  case X86_Reg_R11d:
  case X86_Reg_R12d:  case X86_Reg_R13d:  case X86_Reg_R14d:  case X86_Reg_R15d:
  case X86_Reg_Eip:
    return 32;

  case X86_Reg_Rax:   case X86_Reg_Rbx:   case X86_Reg_Rcx:   case X86_Reg_Rdx:
  case X86_Reg_Rsp:   case X86_Reg_Rbp:   case X86_Reg_Rsi:   case X86_Reg_Rdi:
  case X86_Reg_R8:    case X86_Reg_R9:    case X86_Reg_R10:   case X86_Reg_R11:
  case X86_Reg_R12:   case X86_Reg_R13:   case X86_Reg_R14:   case X86_Reg_R15:
  case X86_Reg_Rip:
    return 64;

  case X86_Reg_Mm0:   case X86_Reg_Mm1:   case X86_Reg_Mm2:   case X86_Reg_Mm3:
  case X86_Reg_Mm4:   case X86_Reg_Mm5:   case X86_Reg_Mm6:   case X86_Reg_Mm7:
    return 64;

  case X86_Reg_Xmm0:  case X86_Reg_Xmm1:  case X86_Reg_Xmm2:  case X86_Reg_Xmm3:
  case X86_Reg_Xmm4:  case X86_Reg_Xmm5:  case X86_Reg_Xmm6:  case X86_Reg_Xmm7:
  case X86_Reg_Xmm8:  case X86_Reg_Xmm9:  case X86_Reg_Xmm10: case X86_Reg_Xmm11:
  case X86_Reg_Xmm12: case X86_Reg_Xmm13: case X86_Reg_Xmm14: case X86_Reg_Xmm15:
    return 128;
  }
}

bool X86Architecture::X86CpuInformation::IsRegisterAliased(u32 Id0, u32 Id1) const
{
  if (Id0 == Id1)
    return true;

  if (Id0 > Id1)
    std::swap(Id0, Id1);

  if (Id0 == X86_Reg_Al && Id1 == X86_Reg_Ah) return false;
  if (Id0 == X86_Reg_Bl && Id1 == X86_Reg_Bh) return false;
  if (Id0 == X86_Reg_Cl && Id1 == X86_Reg_Ch) return false;
  if (Id0 == X86_Reg_Dl && Id1 == X86_Reg_Dh) return false;

  switch (Id0)
  {
  case X86_Reg_Al: case X86_Reg_Ah: case X86_Reg_Ax: case X86_Reg_Eax: case X86_Reg_Rax:
    return Id1 == X86_Reg_Al || Id1 == X86_Reg_Ah || Id1 == X86_Reg_Ax || Id1 == X86_Reg_Eax || Id1 == X86_Reg_Rax;
  case X86_Reg_Bl: case X86_Reg_Bh: case X86_Reg_Bx: case X86_Reg_Ebx: case X86_Reg_Rbx:
    return Id1 == X86_Reg_Bl || Id1 == X86_Reg_Bh || Id1 == X86_Reg_Bx || Id1 == X86_Reg_Ebx || Id1 == X86_Reg_Rbx;
  case X86_Reg_Cl: case X86_Reg_Ch: case X86_Reg_Cx: case X86_Reg_Ecx: case X86_Reg_Rcx:
    return Id1 == X86_Reg_Cl || Id1 == X86_Reg_Ch || Id1 == X86_Reg_Cx || Id1 == X86_Reg_Ecx || Id1 == X86_Reg_Rcx;
  case X86_Reg_Dl: case X86_Reg_Dh: case X86_Reg_Dx: case X86_Reg_Edx: case X86_Reg_Rdx:
    return Id1 == X86_Reg_Dl || Id1 == X86_Reg_Dh || Id1 == X86_Reg_Dx || Id1 == X86_Reg_Edx || Id1 == X86_Reg_Rdx;
  case X86_Reg_Sil: case X86_Reg_Si: case X86_Reg_Esi: case X86_Reg_Rsi:
    return Id1 == X86_Reg_Sil || Id1 == X86_Reg_Si || Id1 == X86_Reg_Esi || Id1 == X86_Reg_Rsi;
  case X86_Reg_Dil: case X86_Reg_Di: case X86_Reg_Edi: case X86_Reg_Rdi:
    return Id1 == X86_Reg_Dil || Id1 == X86_Reg_Di || Id1 == X86_Reg_Edi || Id1 == X86_Reg_Rdi;
  case X86_Reg_Spl: case X86_Reg_Sp: case X86_Reg_Esp: case X86_Reg_Rsp:
    return Id1 == X86_Reg_Spl || Id1 == X86_Reg_Sp || Id1 == X86_Reg_Esp || Id1 == X86_Reg_Rsp;
  case X86_Reg_Bpl: case X86_Reg_Bp: case X86_Reg_Ebp: case X86_Reg_Rbp:
    return Id1 == X86_Reg_Bpl || Id1 == X86_Reg_Bp || Id1 == X86_Reg_Ebp || Id1 == X86_Reg_Rbp;
  case X86_Reg_R8b: case X86_Reg_R8w: case X86_Reg_R8d: case X86_Reg_R8:
    return Id1 == X86_Reg_R8b || Id1 == X86_Reg_R8w || Id1 == X86_Reg_R8d || Id1 == X86_Reg_R8;
  case X86_Reg_R9b: case X86_Reg_R9w: case X86_Reg_R9d: case X86_Reg_R9:
    return Id1 == X86_Reg_R9b || Id1 == X86_Reg_R9w || Id1 == X86_Reg_R9d || Id1 == X86_Reg_R9;
  case X86_Reg_R10b: case X86_Reg_R10w: case X86_Reg_R10d: case X86_Reg_R10:
    return Id1 == X86_Reg_R10b || Id1 == X86_Reg_R10w || Id1 == X86_Reg_R10d || Id1 == X86_Reg_R10;
  case X86_Reg_R11b: case X86_Reg_R11w: case X86_Reg_R11d: case X86_Reg_R11:
    return Id1 == X86_Reg_R11b || Id1 == X86_Reg_R11w || Id1 == X86_Reg_R11d || Id1 == X86_Reg_R11;
  case X86_Reg_R12b: case X86_Reg_R12w: case X86_Reg_R12d: case X86_Reg_R12:
    return Id1 == X86_Reg_R12b || Id1 == X86_Reg_R12w || Id1 == X86_Reg_R12d || Id1 == X86_Reg_R12;
  case X86_Reg_R13b: case X86_Reg_R13w: case X86_Reg_R13d: case X86_Reg_R13:
    return Id1 == X86_Reg_R13b || Id1 == X86_Reg_R13w || Id1 == X86_Reg_R13d || Id1 == X86_Reg_R13;
  case X86_Reg_R14b: case X86_Reg_R14w: case X86_Reg_R14d: case X86_Reg_R14:
    return Id1 == X86_Reg_R14b || Id1 == X86_Reg_R14w || Id1 == X86_Reg_R14d || Id1 == X86_Reg_R14;
  case X86_Reg_R15b: case X86_Reg_R15w: case X86_Reg_R15d: case X86_Reg_R15:
    return Id1 == X86_Reg_R15b || Id1 == X86_Reg_R15w || Id1 == X86_Reg_R15d || Id1 == X86_Reg_R15;
  case X86_Reg_Ip: case X86_Reg_Eip: case X86_Reg_Rip:
    return Id1 == X86_Reg_Ip || Id1 == X86_Reg_Eip || Id1 == X86_Reg_Rip;
  }
  return false;
}

bool X86Architecture::X86CpuInformation::NormalizeRegister(u32 Id, u8 Mode, u32& rExtId, u64& rMask) const
{
  auto CurRegSize = GetSizeOfRegisterInBit(Id);
  if (CurRegSize == 0)
    return false;

  // KS: Flag registers are already normalized since they can't be extended.
  if (CurRegSize == 1)
    return false;
  // KS: We ignore segment registers
  switch (Id)
  {
  case X86_Reg_Cs: case X86_Reg_Ds: case X86_Reg_Es: case X86_Reg_Ss: case X86_Reg_Fs: case X86_Reg_Gs:
    return false;
  }
  // KS: For now, we assume that SIMD registers can't be normalized (which is false).
  if (CurRegSize > 64)
    return false;


  switch (Mode)
  {
    // KS: This case is tedious because X86_Bit_16 could actually allow 32-bit register using the op_size prefix.
    // The question is: wouldn't be better to assume that this mode uses 32-bit by default?
  case X86_Bit_16:
  case X86_Bit_32:
  {
    if (CurRegSize > 32)
      return false;
    if (CurRegSize == 32)
      return false;

    break;
  }

  case X86_Bit_64:
  {
    if (CurRegSize == 64)
      return false;

    break;
  }

  default:
    return false;
  }

  switch (Id)
  {
  case X86_Reg_Al: case X86_Reg_Ah: case X86_Reg_Ax:   case X86_Reg_Eax:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rax : X86_Reg_Eax;  break;
  case X86_Reg_Bl: case X86_Reg_Bh: case X86_Reg_Bx:   case X86_Reg_Ebx:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rbx : X86_Reg_Ebx;  break;
  case X86_Reg_Cl: case X86_Reg_Ch: case X86_Reg_Cx:   case X86_Reg_Ecx:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rcx : X86_Reg_Ecx;  break;
  case X86_Reg_Dl: case X86_Reg_Dh: case X86_Reg_Dx:   case X86_Reg_Edx:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rdx : X86_Reg_Edx;  break;
  case X86_Reg_Sil:                 case X86_Reg_Si:   case X86_Reg_Esi:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rsi : X86_Reg_Esi;  break;
  case X86_Reg_Dil:                 case X86_Reg_Di:   case X86_Reg_Edi:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rdi : X86_Reg_Edi;  break;
  case X86_Reg_Spl:                 case X86_Reg_Sp:   case X86_Reg_Esp:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rsp : X86_Reg_Esp;  break;
  case X86_Reg_Bpl:                 case X86_Reg_Bp:   case X86_Reg_Ebp:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_Rbp : X86_Reg_Ebp;  break;
  case X86_Reg_R8b:                 case X86_Reg_R8w:  case X86_Reg_R8d:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_R8  : X86_Reg_R8d;  break;
  case X86_Reg_R9b:                 case X86_Reg_R9w:  case X86_Reg_R9d:  rExtId = (Mode == X86_Bit_64) ? X86_Reg_R9  : X86_Reg_R9d;  break;
  case X86_Reg_R10b:                case X86_Reg_R10w: case X86_Reg_R10d: rExtId = (Mode == X86_Bit_64) ? X86_Reg_R10 : X86_Reg_R10d; break;
  case X86_Reg_R11b:                case X86_Reg_R11w: case X86_Reg_R11d: rExtId = (Mode == X86_Bit_64) ? X86_Reg_R11 : X86_Reg_R11d; break;
  case X86_Reg_R12b:                case X86_Reg_R12w: case X86_Reg_R12d: rExtId = (Mode == X86_Bit_64) ? X86_Reg_R12 : X86_Reg_R12d; break;
  case X86_Reg_R13b:                case X86_Reg_R13w: case X86_Reg_R13d: rExtId = (Mode == X86_Bit_64) ? X86_Reg_R13 : X86_Reg_R13d; break;
  case X86_Reg_R14b:                case X86_Reg_R14w: case X86_Reg_R14d: rExtId = (Mode == X86_Bit_64) ? X86_Reg_R14 : X86_Reg_R14d; break;
  case X86_Reg_R15b:                case X86_Reg_R15w: case X86_Reg_R15d: rExtId = (Mode == X86_Bit_64) ? X86_Reg_R15 : X86_Reg_R15d; break;
  }

  switch (Id)
  {
  case X86_Reg_Ah: case X86_Reg_Bh: case X86_Reg_Ch: case X86_Reg_Dh: rMask = 0x000000000000ff00ULL; break;
  default: rMask = (1ULL << CurRegSize) - 1; break;
  }

  // In AMD64, if a reg32 is written, it clears the 32-bit MSB of the corresponding register.
  if (Mode == X86_Bit_64 && CurRegSize == 32)
    rMask = 0xffffffffffffffffULL;

  return true;
}

bool X86Architecture::HandleExpression(Expression::LSPType & rExprs, std::string const& rName, Instruction& rInsn, Expression::SPType spResExpr)
{
  // TODO: use unordered_map
  // TODO: base code on https://github.com/qemu/qemu/blob/f368c33d5ab09dd5656924185cd975b11838cd25/target-i386/cc_helper_template.h

  if (rName == "begin_update_flags")
  {

  }

  else if (rName == "end_update_flags")
  {
    if (rInsn.GetNumberOfOperand() == 0)
      return false;

    if (spResExpr == nullptr)
      return false;

    u32 Bit = rInsn.GetOperand(0)->GetBitSize();
    if (Bit == 0)
      return false;
    auto InsnLen = static_cast<u8>(rInsn.GetLength());
    if (InsnLen == 0)
      return false;

    auto UpdatedFlags = rInsn.GetUpdatedFlags();

    if (UpdatedFlags & X86_FlZf)
    {
      // zf = op0 == 0 ? true : false
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlZf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        /**/spResExpr->Clone(),
        /**/Expr::MakeConst(Bit, 0x0),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));
    }

    if (UpdatedFlags & X86_FlSf)
    {
      // sf = (op0 & (1 << (op0.bit - 1)) == (1 << (op0.bit - 1)) ? true : false
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlSf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        /**/Expr::MakeBinOp(OperationExpression::OpAnd,
        /****/spResExpr->Clone(),
        /****/Expr::MakeConst(Bit, 1ULL << (Bit - 1))),
        /**/Expr::MakeConst(Bit, 1ULL << (Bit - 1)),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));
    }

    if (UpdatedFlags & X86_FlPf)
    {
      auto MaskLsb = [](Expression::SPType spExpr) -> Expression::SPType
      {
        return Expr::MakeBinOp(OperationExpression::OpAnd, spExpr, Expr::MakeConst(spExpr->GetBitSize(), 1));
      };

      auto spBit0 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 0)));
      auto spBit1 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 1)));
      auto spBit2 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 2)));
      auto spBit3 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 3)));
      auto spBit4 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 4)));
      auto spBit5 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 5)));
      auto spBit6 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 6)));
      auto spBit7 = MaskLsb(Expr::MakeBinOp(OperationExpression::OpLrs, spResExpr, Expr::MakeConst(spResExpr->GetBitSize(), 7)));

      auto spPart0 = Expr::MakeBinOp(OperationExpression::OpXor, spBit0, spBit1);
      auto spPart1 = Expr::MakeBinOp(OperationExpression::OpXor, spBit2, spBit3);
      auto spPart2 = Expr::MakeBinOp(OperationExpression::OpXor, spBit4, spBit5);
      auto spPart3 = Expr::MakeBinOp(OperationExpression::OpXor, spBit6, spBit7);

      auto spPart4 = Expr::MakeBinOp(OperationExpression::OpXor, spPart0, spPart1);
      auto spPart5 = Expr::MakeBinOp(OperationExpression::OpXor, spPart2, spPart3);

      auto spPart6 = Expr::MakeBinOp(OperationExpression::OpXor, spPart4, spPart5);

      auto spCond = Expr::MakeTernaryCond(ConditionExpression::CondEq, spPart6, Expr::MakeConst(spResExpr->GetBitSize(), 0), Expr::MakeBoolean(true), Expr::MakeBoolean(false));
      rExprs.push_back(Expr::MakeAssign(Expr::MakeId(X86_FlPf, &m_CpuInfo), spCond));
    }

    if (UpdatedFlags & X86_FlAf)
    {
      // TODO:
    }

    switch (rInsn.GetOpcode())
    {
    case X86_Opcode_Inc:
      break;

    case X86_Opcode_Add: case X86_Opcode_Xadd:
      if (spResExpr == nullptr)
        return false;

      // cf = (res < op0) ? true : false (unsigned)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlCf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        /**/spResExpr->Clone(),
        /**/rInsn.GetOperand(0),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));

      {
        // of = ((op0 ^ res) & (~(op0 ^ op1))) & (int(op0.bit, 1 << (op0.bit - 1))
        auto spXor0 = Expr::MakeBinOp(OperationExpression::OpXor, rInsn.GetOperand(0), spResExpr);
        auto spXor1 = Expr::MakeBinOp(OperationExpression::OpXor, rInsn.GetOperand(0), rInsn.GetOperand(1));
        auto spNot = Expr::MakeUnOp(OperationExpression::OpNot, spXor1);
        auto spAnd = Expr::MakeBinOp(OperationExpression::OpAnd, spXor0, spNot);
        auto spMsb = Expr::MakeConst(rInsn.GetOperand(0)->GetBitSize(), 1 << (rInsn.GetOperand(0)->GetBitSize() - 1));
        auto spMask = Expr::MakeBinOp(OperationExpression::OpAnd, spAnd, spMsb);
        auto spCheckOf = Expr::MakeTernaryCond(ConditionExpression::CondEq, spMask, spMsb, Expr::MakeBoolean(true), Expr::MakeBoolean(false));
        rExprs.push_back(Expr::MakeAssign(Expr::MakeId(X86_FlOf, &m_CpuInfo), spCheckOf));
      }
      break;

    case X86_Opcode_Adc:
      if (spResExpr == nullptr)
        return false;

      // cf = (res < op0) ? true : false (unsigned) (cf is already included in res)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlCf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        /**/spResExpr->Clone(),
        /**/rInsn.GetOperand(0),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));

      // of = (((op0 ^ op1) ^ res) ^ ((op0 ^ res) & (op0 ^ op1))) & int(op0.bit, 1 << (op0.bit - 1))
      rExprs.push_back(
        Expr::MakeAssign(Expr::MakeId(X86_FlOf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondNe,
        Expr::MakeBinOp(OperationExpression::OpAnd,
        /**/Expr::MakeBinOp(OperationExpression::OpAnd,
        /****/Expr::MakeBinOp(OperationExpression::OpXor, rInsn.GetOperand(0), spResExpr),
        /****/Expr::MakeUnOp(OperationExpression::OpNot,
        /******/Expr::MakeBinOp(OperationExpression::OpXor, rInsn.GetOperand(0), rInsn.GetOperand(1)))),
        /**/Expr::MakeConst(rInsn.GetOperand(0)->GetBitSize(), 1 << (rInsn.GetOperand(0)->GetBitSize() - 1))),
        Expr::MakeConst(rInsn.GetOperand(0)->GetBitSize(), 1 << (rInsn.GetOperand(0)->GetBitSize() - 1)),
        Expr::MakeBoolean(false), Expr::MakeBoolean(true))));


      // of = (res < op0) ? true : false (signed) (cf is already included in res)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlOf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondSlt,
        /**/spResExpr->Clone(),
        /**/rInsn.GetOperand(0),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));
      break;

    case X86_Opcode_Dec:
      break;

    case X86_Opcode_Sub: case X86_Opcode_Cmp:
      if (spResExpr == nullptr)
        return false;

      // cf = (op0 < op1) ? true : false (unsigned)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlCf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        /**/rInsn.GetOperand(0),
        /**/rInsn.GetOperand(1),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));

      // of = (op0 < op1) ? true : false (signed)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlOf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondSlt,
        /**/rInsn.GetOperand(0),
        /**/rInsn.GetOperand(1),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));
      break;

    case X86_Opcode_Sbb:
      if (spResExpr == nullptr)
        return false;

      // FIXME(KS): cf is modified before the actual operation
      // cf = (op0 < op1) ? true : false (unsigned) (-cf is already included)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlCf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondUlt,
        /**/rInsn.GetOperand(0),
        /**/rInsn.GetOperand(1),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));

      // of = (op0 < op1) ? true : false (signed) (-cf is already included)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlOf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondSlt,
        /**/rInsn.GetOperand(0),
        /**/rInsn.GetOperand(1),
        /**/Expr::MakeBoolean(true), Expr::MakeBoolean(false))));
      break;

    // TODO: handle of
    case X86_Opcode_Rol:
    {
      if (spResExpr == nullptr)
        return false;

      auto spLsb = Expr::MakeBinOp(OperationExpression::OpAnd, spResExpr, Expr::MakeConst(Bit, 1));

      // cf = lsb(res)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlCf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        spLsb, Expr::MakeConst(Bit, 1),
        Expr::MakeBoolean(true), Expr::MakeBoolean(false))));
      break;
    }

    case X86_Opcode_Ror:
    {
      if (spResExpr == nullptr)
        return false;

      auto spMsb = Expr::MakeBinOp(OperationExpression::OpAnd, spResExpr, Expr::MakeConst(Bit, 1 << (Bit - 1)));

      // cf = msb(res)
      rExprs.push_back(Expr::MakeAssign(
        Expr::MakeId(X86_FlCf, &m_CpuInfo),
        Expr::MakeTernaryCond(ConditionExpression::CondEq,
        spMsb, Expr::MakeConst(Bit, 1 << (Bit - 1)),
        Expr::MakeBoolean(true), Expr::MakeBoolean(false))));
      break;
    }

    default:
      return false;
    }

  }

  return true;
}

bool X86Architecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  rInsn.GetData()->ArchitectureTag() = GetTag();
  rInsn.Mode() = Mode;

  u8 Opcode;
  if (!rBinStrm.Read(Offset, Opcode))
    return false;
  bool Res = (this->*m_Table_1[Opcode])(rBinStrm, Offset + 1, rInsn, Mode);
  rInsn.SetName(m_Mnemonic[rInsn.GetOpcode()]);
  return Res;
}
