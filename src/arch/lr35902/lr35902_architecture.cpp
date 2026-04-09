#include "lr35902_architecture.hpp"
#include <medusa/medusa.hpp>

char const* LR35902Architecture::LR35902CpuInformation::ConvertIdentifierToName(u32 Id) const
{
  switch (Id)
  {
  default: return "";

    /* Flag */
  case LR35902_Flg_C: return "cf"; case LR35902_Flg_H: return "hf";
  case LR35902_Flg_N: return "nf"; case LR35902_Flg_Z: return "zf";

    /* Register */
  case LR35902_Reg_A: return "a"; case LR35902_Reg_F: return "f"; case LR35902_Reg_AF: return "af";
  case LR35902_Reg_B: return "b"; case LR35902_Reg_C: return "c"; case LR35902_Reg_BC: return "bc";
  case LR35902_Reg_D: return "d"; case LR35902_Reg_E: return "e"; case LR35902_Reg_DE: return "de";
  case LR35902_Reg_H: return "h"; case LR35902_Reg_L: return "l"; case LR35902_Reg_HL: return "hl";

    /* Pseudo-register */
  case LR35902_Reg_Pc: return "pc"; case LR35902_Reg_Sp: return "sp";
  }
}

u32 LR35902Architecture::LR35902CpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::unordered_map<std::string, u32> s_NameToId;
  if (s_NameToId.empty())
  {
    s_NameToId["cf"] = LR35902_Flg_C; s_NameToId["hf"] = LR35902_Flg_H;
    s_NameToId["nf"] = LR35902_Flg_N; s_NameToId["zf"] = LR35902_Flg_Z;
    s_NameToId["a"] = LR35902_Reg_A; s_NameToId["f"] = LR35902_Reg_F; s_NameToId["af"] = LR35902_Reg_AF;
    s_NameToId["b"] = LR35902_Reg_B; s_NameToId["c"] = LR35902_Reg_C; s_NameToId["bc"] = LR35902_Reg_BC;
    s_NameToId["d"] = LR35902_Reg_D; s_NameToId["e"] = LR35902_Reg_E; s_NameToId["de"] = LR35902_Reg_DE;
    s_NameToId["h"] = LR35902_Reg_H; s_NameToId["l"] = LR35902_Reg_L; s_NameToId["hl"] = LR35902_Reg_HL;
    s_NameToId["pc"] = LR35902_Reg_Pc; s_NameToId["sp"] = LR35902_Reg_Sp;
  }
  auto itId = s_NameToId.find(rName);
  if (itId == std::end(s_NameToId))
    return 0;

  return itId->second;
}

u32 LR35902Architecture::LR35902CpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  switch (RegType)
  {
  default:                     return 0;
  case StackPointerRegister:   return LR35902_Reg_Sp;
  case StackFrameRegister:     return 0;
  case ProgramPointerRegister: return LR35902_Reg_Pc;
  }
}

u32 LR35902Architecture::LR35902CpuInformation::GetSizeOfRegisterInBit(u32 Id) const
{
  switch (Id)
  {
  default: return 0;

    /* Flag */
  case LR35902_Flg_C: case LR35902_Flg_H:
  case LR35902_Flg_N: case LR35902_Flg_Z:
    return 1;

    /* Register */
  case LR35902_Reg_A: case LR35902_Reg_F:
  case LR35902_Reg_B: case LR35902_Reg_C:
  case LR35902_Reg_D: case LR35902_Reg_E:
  case LR35902_Reg_H: case LR35902_Reg_L:
    return 8;

  case LR35902_Reg_AF: case LR35902_Reg_BC:
  case LR35902_Reg_DE: case LR35902_Reg_HL:
    return 16;

    /* Pseudo-register */
  case LR35902_Reg_Sp: case LR35902_Reg_Pc:
    return 16;
  }
}

bool LR35902Architecture::LR35902CpuInformation::IsRegisterAliased(u32 Id0, u32 Id1) const
{
  if (Id0 == Id1)
    return true;

  // Ensure Id1 is the larger value (pair registers have higher enum values)
  if (Id0 > Id1)
    std::swap(Id0, Id1);

  switch (Id1)
  {
  case LR35902_Reg_AF: return Id0 == LR35902_Reg_A || Id0 == LR35902_Reg_F;
  case LR35902_Reg_BC: return Id0 == LR35902_Reg_B || Id0 == LR35902_Reg_C;
  case LR35902_Reg_DE: return Id0 == LR35902_Reg_D || Id0 == LR35902_Reg_E;
  case LR35902_Reg_HL: return Id0 == LR35902_Reg_H || Id0 == LR35902_Reg_L;
  }
  return false;
}

bool LR35902Architecture::Translate(Address const& rVirtAddr, OffsetType& rPhysOff)
{
  return false;
}

Architecture::NamedModeVector LR35902Architecture::GetModes() const
{
  NamedModeVector Modes;
  Modes.push_back(NamedMode("LR35902", LR35902_Mode_Default));
  return Modes;
}

bool LR35902Architecture::FormatInstruction(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  PrintData          & rPrintData) const
{
  rPrintData.AppendMnemonic(rInsn.GetName()).AppendSpace();

  char const* Sep = nullptr;
  auto const OprdNo = rInsn.GetNumberOfOperand();
  for (u8 OprdIdx = 0; OprdIdx < OprdNo; ++OprdIdx)
  {
    if (Sep != nullptr)
      rPrintData.AppendOperator(Sep).AppendSpace();
    else
      Sep = ",";

    rPrintData.MarkOffset();

    // HACK: handle not flag now
    if (OprdIdx == 0 && rInsn.GetPrefix() == LR35902_Insn_Prefix_NotFlag)
    {
      auto spFirstRegister = expr_cast<IdentifierExpression>(rInsn.GetOperand(0));
      if (spFirstRegister != nullptr)
      {
        switch (spFirstRegister->GetId())
        {
        case LR35902_Flg_C: case LR35902_Flg_H:
        case LR35902_Flg_N: case LR35902_Flg_Z:
          rPrintData.AppendOperator("!");
          break;
        default:
          break;
        }
      }
    }

    if (!FormatOperand(rDoc, rAddr, rInsn, OprdIdx, rPrintData))
      return false;
  }

  return true;
}
