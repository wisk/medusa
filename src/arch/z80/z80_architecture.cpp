#include "z80_architecture.hpp"
#include <sstream>
#include <medusa/medusa.hpp>

char const* Z80Architecture::Z80CpuInformation::ConvertIdentifierToName(u32 Id) const
{
  switch (Id)
  {
  default: return "";

    /* Flag */
  case Z80_Flg_C: return "c"; case Z80_Flg_H: return "h";
  case Z80_Flg_N: return "n"; case Z80_Flg_Z: return "z";

    /* Register */
  case Z80_Reg_A: return "a"; case Z80_Reg_F: return "f"; case Z80_Reg_AF: return "af";
  case Z80_Reg_B: return "b"; case Z80_Reg_C: return "c"; case Z80_Reg_BC: return "bc";
  case Z80_Reg_D: return "d"; case Z80_Reg_E: return "e"; case Z80_Reg_DE: return "de";
  case Z80_Reg_H: return "h"; case Z80_Reg_L: return "l"; case Z80_Reg_HL: return "hl";

    /* Pseudo-register */
  case Z80_Reg_Pc: return "pc"; case Z80_Reg_Sp: return "sp";
  }
}

u32 Z80Architecture::Z80CpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::unordered_map<std::string, u32> s_NameToId;
  if (s_NameToId.empty())
  {
    s_NameToId["c"] = Z80_Flg_C; s_NameToId["h"] = Z80_Flg_H;
    s_NameToId["n"] = Z80_Flg_N; s_NameToId["z"] = Z80_Flg_Z;
    s_NameToId["a"] = Z80_Reg_A; s_NameToId["f"] = Z80_Reg_F; s_NameToId["af"] = Z80_Reg_AF;
    s_NameToId["b"] = Z80_Reg_B; s_NameToId["c"] = Z80_Reg_C; s_NameToId["bc"] = Z80_Reg_BC;
    s_NameToId["d"] = Z80_Reg_D; s_NameToId["e"] = Z80_Reg_E; s_NameToId["de"] = Z80_Reg_DE;
    s_NameToId["h"] = Z80_Reg_H; s_NameToId["l"] = Z80_Reg_L; s_NameToId["hl"] = Z80_Reg_HL;
    s_NameToId["pc"] = Z80_Reg_Pc; s_NameToId["sp"] = Z80_Reg_Sp;
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
  }
}

u32 Z80Architecture::Z80CpuInformation::GetSizeOfRegisterInBit(u32 Id) const
{
  switch (Id)
  {
  default: return 0;

    /* Flag */
  case Z80_Flg_C: case Z80_Flg_H:
  case Z80_Flg_N: case Z80_Flg_Z:
    return 1;

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

Architecture::NamedModeVector Z80Architecture::GetModes(void) const
{
  NamedModeVector Modes;
  //GbModes.push_back(NamedMode("original", Z80_Mode_Original));
  Modes.push_back(NamedMode("LR35902", Z80_Mode_LR35902));
  return Modes;
}

bool Z80Architecture::FormatInstruction(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  PrintData          & rPrintData) const
{
  rPrintData.AppendMnemonic(rInsn.GetName()).AppendSpace();

  char const* Sep = nullptr;

  auto pSep = nullptr;
  auto const OprdNo = rInsn.GetNumberOfOperand();
  for (u8 OprdIdx = 0; OprdIdx < OprdNo; ++OprdIdx)
  {
    if (Sep != nullptr)
      rPrintData.AppendOperator(Sep).AppendSpace();
    else
      Sep = ",";

    rPrintData.MarkOffset();

    // HACK: handle not flag now
    if (OprdIdx == 0 && rInsn.GetPrefix() == Z80_Insn_Prefix_NotFlag)
    {
      auto spFirstRegister = expr_cast<IdentifierExpression>(rInsn.GetOperand(0));
      if (spFirstRegister != nullptr)
      {
        switch (spFirstRegister->GetId())
        {
        case Z80_Flg_C: case Z80_Flg_H:
        case Z80_Flg_N: case Z80_Flg_Z:
          rPrintData.AppendOperator("!");
        }
      }
    }

    if (!FormatOperand(rDoc, rAddr, rInsn, OprdIdx, rPrintData))
      return false;
  }

  return true;
}