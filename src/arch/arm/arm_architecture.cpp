#include "arm_architecture.hpp"

#include <boost/algorithm/string/join.hpp>

ArmArchitecture::ArmArchitecture(void)
  : Architecture(MEDUSA_ARCH_TAG('a', 'r', 'm'))
  , m_CpuInfo()
{
  Configuration::Enum Mode;
  Mode.push_back(std::make_pair("thumb", ARM_Thumb1));
  Mode.push_back(std::make_pair("thumb2", ARM_Thumb2));
  Mode.push_back(std::make_pair("thumbEE", ARM_ThumbEE));
  m_CfgMdl.InsertEnum("Thumb feature", Mode, ARM_Thumb2);
}

char const* ArmArchitecture::ARMCpuInformation::ConvertIdentifierToName(u32 Id) const
{
  static char const* s_RegisterName[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "fp", "ip" "sp", "lr", "pc" };
  if (Id >= sizeof(s_RegisterName) / sizeof(*s_RegisterName))
    return "invalid";
  return s_RegisterName[Id];
}

u32 ArmArchitecture::ARMCpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::map<std::string, u32> s_NameToId;
  if (s_NameToId.empty())
  {
    s_NameToId["r0"]  = ARM_RegR0;
    s_NameToId["r1"]  = ARM_RegR1;
    s_NameToId["r2"]  = ARM_RegR2;
    s_NameToId["r3"]  = ARM_RegR3;
    s_NameToId["r4"]  = ARM_RegR4;
    s_NameToId["r5"]  = ARM_RegR5;
    s_NameToId["r6"]  = ARM_RegR6;
    s_NameToId["r7"]  = ARM_RegR7;
    s_NameToId["r8"]  = ARM_RegR8;
    s_NameToId["r9"]  = ARM_RegR9;
    s_NameToId["r10"] = ARM_RegR10;
    s_NameToId["r11"] = ARM_RegR11;
    s_NameToId["r12"] = ARM_RegR12;
    s_NameToId["sp"]  = ARM_RegR13;
    s_NameToId["lr"]  = ARM_RegR14;
    s_NameToId["pc"]  = ARM_RegR15;
  }
  auto itResult = s_NameToId.find(rName);
  if (itResult == std::end(s_NameToId))
    return 0;

  return itResult->second;
}

u32 ArmArchitecture::ARMCpuInformation::GetRegisterByType(CpuInformation::Type RegType) const
{
  static const u32 s_RegisterMapping[] = { ARM_RegR13, 0, ARM_RegR15, 0,ARM_RegR0, 0 };
  return (RegType < InvalidRegister) ? s_RegisterMapping[RegType] : 0;
}

bool ArmArchitecture::ARMCpuContext::ReadRegister(u32 Register, void* pValue, u32 Size) const
{
  if (Size != 32)
    return false;

#define READ_REGISTER(idx) memcpy(pValue, &m_Context.Registers[idx], 4)
  switch (Register)
  {
    case ARM_RegR0:  READ_REGISTER(0);  break;
    case ARM_RegR1:  READ_REGISTER(1);  break;
    case ARM_RegR2:  READ_REGISTER(2);  break;
    case ARM_RegR3:  READ_REGISTER(3);  break;
    case ARM_RegR4:  READ_REGISTER(4);  break;
    case ARM_RegR5:  READ_REGISTER(5);  break;
    case ARM_RegR6:  READ_REGISTER(6);  break;
    case ARM_RegR7:  READ_REGISTER(7);  break;
    case ARM_RegR8:  READ_REGISTER(8);  break;
    case ARM_RegR9:  READ_REGISTER(9);  break;
    case ARM_RegR10: READ_REGISTER(10); break;
    case ARM_RegR11: READ_REGISTER(11); break;
    case ARM_RegR12: READ_REGISTER(12); break;
    case ARM_RegR13: READ_REGISTER(13); break;
    case ARM_RegR14: READ_REGISTER(14); break;
    case ARM_RegR15: READ_REGISTER(15); break;
    default: return false;
  }
#undef READ_REGISTER
  return true;
}

bool ArmArchitecture::ARMCpuContext::WriteRegister(u32 Register, void const* pValue, u32 Size, bool SignExtend)
{
  if (Size != 32)
    return false;

#define WRITE_REGISTER(idx) memcpy(&m_Context.Registers[idx], pValue, 4)
  switch (Register)
  {
    case ARM_RegR0:  WRITE_REGISTER(0);  break;
    case ARM_RegR1:  WRITE_REGISTER(1);  break;
    case ARM_RegR2:  WRITE_REGISTER(2);  break;
    case ARM_RegR3:  WRITE_REGISTER(3);  break;
    case ARM_RegR4:  WRITE_REGISTER(4);  break;
    case ARM_RegR5:  WRITE_REGISTER(5);  break;
    case ARM_RegR6:  WRITE_REGISTER(6);  break;
    case ARM_RegR7:  WRITE_REGISTER(7);  break;
    case ARM_RegR8:  WRITE_REGISTER(8);  break;
    case ARM_RegR9:  WRITE_REGISTER(9);  break;
    case ARM_RegR10: WRITE_REGISTER(10); break;
    case ARM_RegR11: WRITE_REGISTER(11); break;
    case ARM_RegR12: WRITE_REGISTER(12); break;
    case ARM_RegR13: WRITE_REGISTER(13); break;
    case ARM_RegR14: WRITE_REGISTER(14); break;
    case ARM_RegR15: WRITE_REGISTER(15); break;
    default: return false;
  }
#undef WRITE_REGISTER
  return true;
}

bool ArmArchitecture::ARMCpuContext::Translate(Address const& rLogicalAddress, u64& rLinearAddress) const
{
  rLinearAddress = rLogicalAddress.GetOffset();
  return true;
}

std::string ArmArchitecture::ARMCpuContext::ToString(void) const
{
  static char const* s_RegisterName[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "fp", "ip" "sp", "lr", "pc" };
  std::ostringstream oss;
  for (u8 i = 0; i < sizeof(s_RegisterName) / sizeof(*s_RegisterName); ++i)
    oss << s_RegisterName[i] << ": " << m_Context.Registers[i] << std::endl;
  return oss.str();
}

bool ArmArchitecture::FormatInstruction(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Instruction   const& rInsn,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  static char const* Suffix[] = { "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "", "" };
  char Sep = '\0';
  std::ostringstream oss;

  oss << m_Mnemonic[rInsn.GetOpcode()];
  oss << Suffix[rInsn.GetTestedFlags() & 0xf];
  if (rInsn.GetPrefix() & ARM_Prefix_S)
    oss << "s";
  oss << " ";

  rMarks.push_back(Cell::Mark(Cell::Mark::MnemonicType, oss.str().size()));

  for (int i = 0; i < 4; ++i)
  {
    auto pOprd = rInsn.Operand(i);
    if (pOprd->GetType() == O_NONE) continue;
    if (Sep == '\0')
      Sep = ',';
    else
    {
      oss << Sep << " ";
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 2));
    }

    if ((pOprd->GetType() & O_MEM32) == O_MEM32)
    {
      oss << "[";
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));

      auto RegStr = RegisterToString(pOprd->GetReg(), rInsn.GetMode());
      oss << RegStr;
      rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, RegStr.size()));

      if (pOprd->GetType() & O_SREG)
      {
        oss << ",";
        rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));

        auto SecRegStr = RegisterToString(pOprd->GetSecReg(), rInsn.GetMode());
        oss << SecRegStr;
        rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, SecRegStr.size()));
      }
      else if (pOprd->GetType() & O_DISP)
      {
        oss << ",";
        rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));

        std::ostringstream Imm;
        Imm << "#";
        rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, 1));

        Imm << "0x" << std::setfill('0') << std::setw(8) << std::hex << pOprd->GetValue();
        oss << Imm.str();
        rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, Imm.str().size() - 1));
      }

      oss << "]";
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
    }
    else if ((pOprd->GetType() & O_REG32) == O_REG32)
    {
      auto RegStr = RegisterToString(pOprd->GetReg(), rInsn.GetMode());
      oss << RegStr;
      rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, RegStr.size()));
    }

    else if ((pOprd->GetType() & O_IMM32) == O_IMM32)
    {
      std::ostringstream ImmStrm;
      ImmStrm << "#";
      rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, 1));
      Label Lbl = rDoc.GetLabelFromAddress(pOprd->GetValue());

      if (Lbl.GetType() == Label::Unknown)
      {
        ImmStrm << "0x" << std::setfill('0') << std::setw(8) << std::hex << pOprd->GetValue();
        rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, ImmStrm.str().size() - 1));
      }

      else
      {
        ImmStrm << Lbl.GetLabel();
        rMarks.push_back(Cell::Mark(Cell::Mark::LabelType, ImmStrm.str().size() - 1));
      }

      oss << ImmStrm.str();
    }

    else if ((pOprd->GetType() & O_ABS32) == O_ABS32)
    {
      std::ostringstream ImmStrm;
      ImmStrm << "=";
      rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, 1));
      Label Lbl = rDoc.GetLabelFromAddress(pOprd->GetValue());

      if (Lbl.GetType() == Label::Unknown)
      {
        ImmStrm << "0x" << std::setfill('0') << std::setw(8) << std::hex << pOprd->GetValue();
        rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, ImmStrm.str().size() - 1));
      }

      else
      {
        ImmStrm << Lbl.GetLabel();
        rMarks.push_back(Cell::Mark(Cell::Mark::LabelType, ImmStrm.str().size() - 1));
      }

      oss << ImmStrm.str();
    }

    else if ((pOprd->GetType() & O_REL32) == O_REL32)
    {
      Address DstAddr;
      std::string OprdName = "";

      if (rInsn.GetOperandReference(rDoc, 0, rAddr, DstAddr))
      {
        Label Lbl = rDoc.GetLabelFromAddress(DstAddr);
        OprdName = Lbl.GetLabel();
        Cell::Mark::Type MarkType = Cell::Mark::LabelType;

        if (OprdName.empty())
        {
          OprdName = DstAddr.ToString();
          MarkType = Cell::Mark::ImmediateType;
        }

        oss << OprdName;
        rMarks.push_back(Cell::Mark(MarkType, OprdName.size()));
      }
      else
      {
        oss << OprdName;
        rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, OprdName.size()));
      }
    }
  }

  rStrCell = oss.str();
  return true;
}

std::string ArmArchitecture::RegisterToString(u32 Register, u8 Mode) const
{
  static char const *s_RegisterName[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "fp", "ip", "sp", "lr", "pc" };
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
