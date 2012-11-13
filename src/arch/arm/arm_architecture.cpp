#include "arm_architecture.hpp"

#include <boost/algorithm/string/join.hpp>

void ArmArchitecture::FillConfigurationModel(ConfigurationModel& rCfgMdl)
{
}

void ArmArchitecture::FormatInstruction(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddress, Instruction& rInsn)
{
  char Sep = '\0';
  static char const* Suffix[] = { "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "", "" };
  std::ostringstream oss;

  oss << m_Mnemonic[rInsn.GetOpcode()];
  oss << Suffix[rInsn.GetTestedFlags() & 0xf];
  if (rInsn.GetPrefix() & ARM_Prefix_S)
    oss << "s";
  oss << " ";

  rInsn.AddMark(Cell::Mark::MnemonicType, oss.str().length());

  for (int i = 0; i < 4; ++i)
  {
    auto pOprd = rInsn.Operand(i);
    if (pOprd->GetType() == O_NONE) continue;
    if (Sep == '\0')
      Sep = ',';
    else
    {
      oss << Sep << " ";
      rInsn.AddMark(Cell::Mark::OperatorType, 2);
    }
    if (pOprd->GetType() & O_REG32)
    {
      auto RegStr = RegisterToString(pOprd->GetReg());
      oss << RegStr;
      rInsn.AddMark(Cell::Mark::RegisterType, RegStr.length());
    }

    else if (pOprd->GetType() & O_IMM32)
    {
      std::ostringstream Imm;
      Imm << "0x" << std::setfill('0') << std::setw(8) << std::hex << pOprd->Value();
      oss << Imm.str();
      rInsn.AddMark(Cell::Mark::ImmediateType, Imm.str().length());
    }
  }

  rInsn.UpdateString(oss.str());
}

std::string ArmArchitecture::RegisterToString(u32 Register) const
{
  static char const *s_RegisterName[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "lr", "pc" };
  std::vector<char const *> RegList;
  for (unsigned i = 0; i < 16; ++i)
    if (Register & (1 << i))
      return s_RegisterName[i];
      //RegList.push_back(s_RegisterName[i]);
  return "";
}
