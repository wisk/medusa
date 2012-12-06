#include "arm_architecture.hpp"

#include <boost/algorithm/string/join.hpp>

void ArmArchitecture::FillConfigurationModel(ConfigurationModel& rCfgMdl)
{
}

void ArmArchitecture::FormatInstruction(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddress, Instruction& rInsn) const
{
  char Sep = '\0';
  static char const* Suffix[] = { "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "", "" };
  std::ostringstream oss;

  oss << m_Mnemonic[rInsn.GetOpcode()];
  oss << Suffix[rInsn.GetTestedFlags() & 0xf];
  if (rInsn.GetPrefix() & ARM_Prefix_S)
    oss << "s";
  oss << " ";

  rInsn.AddMark(Cell::Mark::MnemonicType, oss.str().size());

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

    if (((pOprd->GetType() & (O_MEM32 | O_REG_PC_REL)) == (O_MEM32 | O_REG_PC_REL)) && pOprd->GetValue() != 0x0)
    {
      oss << "=";
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
      std::ostringstream ImmOss;
      ImmOss << "0x" << std::hex << std::setw(8) << std::setfill('0') << pOprd->GetValue();
      oss << ImmOss.str();
      rInsn.AddMark(Cell::Mark::ImmediateType, ImmOss.str().length());
    }

    else if ((pOprd->GetType() & O_MEM32) == O_MEM32)
    {
      oss << "[";
      rInsn.AddMark(Cell::Mark::OperatorType, 1);

      auto RegStr = RegisterToString(pOprd->GetReg());
      oss << RegStr;
      rInsn.AddMark(Cell::Mark::RegisterType, RegStr.size());

      if (pOprd->GetType() & O_SREG)
      {
        oss << ",";
        rInsn.AddMark(Cell::Mark::OperatorType, 1);

        auto SecRegStr = RegisterToString(pOprd->GetSecReg());
        oss << SecRegStr;
        rInsn.AddMark(Cell::Mark::RegisterType, SecRegStr.size());
      }
      else if (pOprd->GetType() & O_DISP)
      {
        oss << ",";
        rInsn.AddMark(Cell::Mark::OperatorType, 1);

        std::ostringstream Imm;
        Imm << "#";
        rInsn.AddMark(Cell::Mark::KeywordType, 1);

        Imm << "0x" << std::setfill('0') << std::setw(8) << std::hex << pOprd->Value();
        oss << Imm.str();
        rInsn.AddMark(Cell::Mark::ImmediateType, Imm.str().size() - 1);
      }

      oss << "]";
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
    }
    else if ((pOprd->GetType() & O_REG32) == O_REG32)
    {
      auto RegStr = RegisterToString(pOprd->GetReg());
      oss << RegStr;
      rInsn.AddMark(Cell::Mark::RegisterType, RegStr.size());
    }

    else if ((pOprd->GetType() & O_IMM32) == O_IMM32)
    {
      std::ostringstream Imm;
      Imm << "#";
      rInsn.AddMark(Cell::Mark::KeywordType, 1);

      Imm << "0x" << std::setfill('0') << std::setw(8) << std::hex << pOprd->Value();
      oss << Imm.str();
      rInsn.AddMark(Cell::Mark::ImmediateType, Imm.str().size());
    }

    else if ((pOprd->GetType() & O_REL32) == O_REL32)
    {
      Address DstAddr;
      std::string OprdName = "";

      if (rInsn.GetOperandReference(rDatabase, 0, rAddress, DstAddr))
      {
        Label Lbl = rDatabase.GetLabelFromAddress(DstAddr);
        OprdName = Lbl.GetLabel();
        Cell::Mark::Type MarkType = Cell::Mark::LabelType;

        if (OprdName.empty()) { OprdName = DstAddr.ToString(); MarkType = Cell::Mark::ImmediateType; }

        oss << OprdName;
        rInsn.AddMark(MarkType, OprdName.size());
        if (rInsn.GetComment().empty())
          rInsn.SetComment(Lbl.GetName());
      }
      else
      {
        oss << OprdName;
        rInsn.AddMark(Cell::Mark::ImmediateType, OprdName.size());
      }
    }
  }

  rInsn.UpdateString(oss.str());
}

std::string ArmArchitecture::RegisterToString(u32 Register) const
{
  static char const *s_RegisterName[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc" };
  std::list<std::string> RegsStr;

  for (unsigned i = 0; i < 16; ++i)
  {
    if (Register & (1 << i))
    {
      if (((Register >> i) & 3) == 3)
      {
        std::string Res;
        Res += s_RegisterName[i];
        Res += "-";
        while (i < 15 && (Register & (1 << (i + 1))))
          ++i;
        Res += s_RegisterName[i];
        RegsStr.push_back(Res);
      }
      else
        RegsStr.push_back(s_RegisterName[i]);
    }
  }


  if (RegsStr.size() > 1)
    return std::string("{") + boost::join(RegsStr, ",") + std::string("}");
  return *std::begin(RegsStr);
}
