#include "z80_architecture.hpp"
#include <sstream>
#include <medusa/medusa.hpp>

char const* Z80Architecture::Z80CpuInformation::ConvertIdentifierToName(u32 Id) const
{
  switch (Id)
  {
  default: return "";

    /* Register */
  case Z80_Reg_A: return "a"; case Z80_Reg_F: return "f"; case Z80_Reg_AF: return "af";
  case Z80_Reg_B: return "b"; case Z80_Reg_C: return "c"; case Z80_Reg_BC: return "bc";
  case Z80_Reg_D: return "d"; case Z80_Reg_E: return "e"; case Z80_Reg_DE: return "de";
  case Z80_Reg_H: return "h"; case Z80_Reg_L: return "l"; case Z80_Reg_HL: return "hl";

    /* Pseudo-register */
  case Z80_Reg_Pc: return "pc"; case Z80_Reg_Sp: return "sp"; /*case Z80_Reg_Fl: return "flags";*/

    /* Flag */
  //case GB_FlCf: return "cf"; case GB_FlHf: return "hf";
  //case GB_FlNf: return "nf"; case GB_FlZf: return "zf";
  }
}

u32 Z80Architecture::Z80CpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::unordered_map<std::string, u32> s_NameToId;
  if (s_NameToId.empty())
  {
    s_NameToId["a"] = Z80_Reg_A; s_NameToId["f"] = Z80_Reg_F; s_NameToId["af"] = Z80_Reg_AF;
    s_NameToId["b"] = Z80_Reg_B; s_NameToId["c"] = Z80_Reg_C; s_NameToId["bc"] = Z80_Reg_BC;
    s_NameToId["d"] = Z80_Reg_D; s_NameToId["e"] = Z80_Reg_E; s_NameToId["de"] = Z80_Reg_DE;
    s_NameToId["h"] = Z80_Reg_H; s_NameToId["l"] = Z80_Reg_L; s_NameToId["hl"] = Z80_Reg_HL;
    s_NameToId["pc"] = Z80_Reg_Pc; s_NameToId["sp"] = Z80_Reg_Sp; /*s_NameToId["flags"] = Z80_Reg_Fl;*/
    //s_NameToId["cf"] = GB_FlCf; s_NameToId["hl"] = GB_FlHf;
    //s_NameToId["nf"] = GB_FlNf; s_NameToId["zf"] = GB_FlZf;
  }
  auto itId = s_NameToId.find(rName);
  if (itId == std::end(s_NameToId))
    return 0;

  return itId->second;
}

u32 Z80Architecture::Z80CpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  switch (RegType)
  {
  default:                     return 0;
  case StackPointerRegister:   return Z80_Reg_Sp;
  case StackFrameRegister:     return 0;
  case ProgramPointerRegister: return Z80_Reg_Pc;
  //case FlagRegister:           return Z80_Reg_Fl;
  }
}

u32 Z80Architecture::Z80CpuInformation::GetSizeOfRegisterInBit(u32 Id) const
{
  switch (Id)
  {
  default: return 0;

    /* Register */
  case Z80_Reg_A: case Z80_Reg_F:
  case Z80_Reg_B: case Z80_Reg_C:
  case Z80_Reg_D: case Z80_Reg_E:
  case Z80_Reg_H: case Z80_Reg_L:
    return 8;

  case Z80_Reg_AF: case Z80_Reg_BC:
  case Z80_Reg_DE: case Z80_Reg_HL:
    return 16;

    /* Pseudo-register */
  case Z80_Reg_Sp: case Z80_Reg_Pc:
    return 16;

  //  /* Flag */
  //case GB_FlCf: case GB_FlHf:
  //case GB_FlNf: case GB_FlZf:
  //  return 1;
  }
}

bool Z80Architecture::Z80CpuInformation::IsRegisterAliased(u32 Id0, u32 Id1) const
{
  if (Id0 == Id1)
    return true;

  if (Id0 > Id1)
    std::swap(Id0, Id1);

  switch (Id0)
  {
  case Z80_Reg_AF: return Id1 == Z80_Reg_A || Id1 == Z80_Reg_F;
  case Z80_Reg_BC: return Id1 == Z80_Reg_B || Id1 == Z80_Reg_C;
  case Z80_Reg_DE: return Id1 == Z80_Reg_D || Id1 == Z80_Reg_E;
  case Z80_Reg_HL: return Id1 == Z80_Reg_H || Id1 == Z80_Reg_L;
  }
  return false;
}

bool Z80Architecture::Translate(Address const& rVirtAddr, TOffset& rPhysOff)
{
  return false;
}
