#include "x86.hpp"
#include "x86_architecture.hpp"

bool X86Architecture::FormatInstruction(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  PrintData          & rPrintData) const
{
  /* ref: http://www.intel.com/content/www/us/en/processors/architectures-software-developer-manuals.html
  The REP prefix can be added to the INS, OUTS, MOVS, LODS, and STOS instructions,
  and the REPE, REPNE, REPZ, and REPNZ prefixes can be added to the CMPS and SCAS 
  instructions. (The REPZ and REPNZ prefixes are synonymous forms of the REPE and
  REPNE prefixes, respectively.) The behavior of the REP prefix is undefined when
  used with non-string instructions.
  */
  if (rInsn.GetPrefix())
  {
    std::string Prefix;
    if (rInsn.GetPrefix() & X86_Prefix_Lock)
    {
      switch (rInsn.GetOpcode())
      {
      case X86_Opcode_Inc: case X86_Opcode_Add: case X86_Opcode_Adc: case X86_Opcode_Dec: case X86_Opcode_Sub: case X86_Opcode_Sbb:
      case X86_Opcode_Neg: case X86_Opcode_Xadd:
      case X86_Opcode_Xchg: case X86_Opcode_Cmpxchg: case X86_Opcode_Cmpxchg8b: case X86_Opcode_Cmpxchg16b:
      case X86_Opcode_And:  case X86_Opcode_Xor: case X86_Opcode_Not: case X86_Opcode_Or:
      case X86_Opcode_Btc: case X86_Opcode_Btr: case X86_Opcode_Bts:
      Prefix = "lock";
        break;
      default:
        break;
      }
    }
    else if (rInsn.GetPrefix() & X86_Prefix_Rep)
    {
      switch (rInsn.GetOpcode())
      {
      case X86_Opcode_Ins: case X86_Opcode_Outs: case X86_Opcode_Movs:
      case X86_Opcode_Lods: case X86_Opcode_Stos:
        Prefix = "rep";
        break;
      case X86_Opcode_Cmps: case X86_Opcode_Scas:
        Prefix = "repz";
        break;
      }
    }
    else if (rInsn.GetPrefix() & X86_Prefix_RepNz)
    {
      switch (rInsn.GetOpcode())
      {
        case X86_Opcode_Cmps: case X86_Opcode_Scas:
          Prefix = "repnz";
        default:
          break;
      }
    }
    if (!Prefix.empty())
      rPrintData.AppendMnemonic(Prefix).AppendSpace();
  }

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
    if (!FormatOperand(rDoc, rAddr, rInsn, OprdIdx, rPrintData))
      return false;
  }

  return true;
}