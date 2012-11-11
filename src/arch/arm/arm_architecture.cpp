#include "arm_architecture.hpp"

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

  rInsn.UpdateString(oss.str());
}