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
  static std::map<u32, char const*> s_IdToName;
  if (s_IdToName.empty())
  {
    s_IdToName[ARM_RegR0 ] = "r0";
    s_IdToName[ARM_RegR1 ] = "r1";
    s_IdToName[ARM_RegR2 ] = "r2";
    s_IdToName[ARM_RegR3 ] = "r3";
    s_IdToName[ARM_RegR4 ] = "r4";
    s_IdToName[ARM_RegR5 ] = "r5";
    s_IdToName[ARM_RegR6 ] = "r6";
    s_IdToName[ARM_RegR7 ] = "r7";
    s_IdToName[ARM_RegR8 ] = "r8";
    s_IdToName[ARM_RegR9 ] = "r9";
    s_IdToName[ARM_RegR10] = "r10";
    s_IdToName[ARM_RegR11] = "r11";
    s_IdToName[ARM_RegR12] = "r12";
    s_IdToName[ARM_RegR13] = "sp";
    s_IdToName[ARM_RegR14] = "lr";
    s_IdToName[ARM_RegR15] = "pc";
  }
  auto itResult = s_IdToName.find(Id);
  if (itResult == std::end(s_IdToName))
    return nullptr;

  return itResult->second;
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

u32 ArmArchitecture::ARMCpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  static const u32 s_RegisterMapping[] = { ARM_RegR13, 0, ARM_RegR15, 0,ARM_RegR0, 0 };
  return (RegType < InvalidRegister) ? s_RegisterMapping[RegType] : 0;
}

bool ArmArchitecture::ARMCpuContext::ReadRegister(u32 Register, void* pValue, u32 Size) const
{
  Size *= 8;
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
  Size *= 8;
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
  return (boost::format(
      "r0:0x%08x r1:0x%08x r2: 0x%08x r3:%08x\n"
      "r4:0x%08x r5:0x%08x r6: 0x%08x r7:%08x\n"
      "r8:0x%08x r9:0x%08x r10:0x%08x fp:%08x\n"
      "ip:0x%08x sp:0x%08x lr: 0x%08x pc:%08x\n")
    % m_Context.Registers[0] % m_Context.Registers[1] % m_Context.Registers[2] % m_Context.Registers[3]
    % m_Context.Registers[4] % m_Context.Registers[5] % m_Context.Registers[6] % m_Context.Registers[7]
    % m_Context.Registers[8] % m_Context.Registers[9] % m_Context.Registers[10] % m_Context.Registers[11]
    % m_Context.Registers[12] % m_Context.Registers[13] % m_Context.Registers[14] % m_Context.Registers[15]).str();
}

// TODO: improve this shit
void* ArmArchitecture::ARMCpuContext::GetRegisterAddress(u32 Register)
{
  return &m_Context.Registers[Register];
}

void* ArmArchitecture::ARMCpuContext::GetContextAddress(void)
{
  return &m_Context;
}

u16 ArmArchitecture::ARMCpuContext::GetRegisterOffset(u32 Register)
{
  return Register + sizeof(u32) * Register;
}

void ArmArchitecture::ARMCpuContext::GetRegisters(CpuContext::RegisterList& RegList) const
{
  RegList.push_back(ARM_RegR0);
  RegList.push_back(ARM_RegR1);
  RegList.push_back(ARM_RegR2);
  RegList.push_back(ARM_RegR3);
  RegList.push_back(ARM_RegR4);
  RegList.push_back(ARM_RegR5);
  RegList.push_back(ARM_RegR6);
  RegList.push_back(ARM_RegR7);
  RegList.push_back(ARM_RegR8);
  RegList.push_back(ARM_RegR9);
  RegList.push_back(ARM_RegR10);
  RegList.push_back(ARM_RegR11);
  RegList.push_back(ARM_RegR12);
  RegList.push_back(ARM_RegR13);
  RegList.push_back(ARM_RegR14);
  RegList.push_back(ARM_RegR15);
}

bool ArmArchitecture::FormatOperand(
  Document      const& rDoc,
  Address       const& rAddress,
  Instruction   const& rInstruction,
  Operand       const& rOperand,
  u8                   OperandNo,
  PrintData          & rPrintData) const
{
  if ((rOperand.GetType() & O_MEM32) == O_MEM32)
  {
    rPrintData
      .AppendOperator("[")
      .AppendRegister(RegisterToString(rOperand.GetReg(), rInstruction.GetMode()));

    if (rOperand.GetType() & O_SREG)
    {
      rPrintData
        .AppendOperator(",").AppendSpace()
        .AppendRegister(RegisterToString(rOperand.GetSecReg(), rInstruction.GetMode()));
    }
    else if (rOperand.GetType() & O_DISP)
    {
      rPrintData
        .AppendOperator(",").AppendSpace()
        .AppendOperator("#").AppendImmediate(rOperand.GetValue(), 32);
    }

    rPrintData.AppendOperator("]");
  }
  else if ((rOperand.GetType() & O_REG32) == O_REG32)
  {
    rPrintData.AppendRegister(RegisterToString(rOperand.GetReg(), rInstruction.GetMode()));
  }

  else if ((rOperand.GetType() & O_IMM32) == O_IMM32)
  {
    Label Lbl = rDoc.GetLabelFromAddress(rOperand.GetValue());

    rPrintData.AppendOperator("#");

    if (Lbl.GetType() == Label::Unknown)
      rPrintData.AppendImmediate(rOperand.GetValue(), 32);

    else
      rPrintData.AppendLabel(Lbl.GetLabel());
  }

  else if ((rOperand.GetType() & O_ABS32) == O_ABS32)
  {
    Label Lbl = rDoc.GetLabelFromAddress(rOperand.GetValue());

    rPrintData.AppendOperator("=");

    if (Lbl.GetType() == Label::Unknown)
      rPrintData.AppendImmediate(rOperand.GetValue(), 32);

    else
      rPrintData.AppendLabel(Lbl.GetLabel());
  }

  else if ((rOperand.GetType() & O_REL32) == O_REL32)
  {
    Address DstAddr;
    std::string OprdName = "";

    if (rInstruction.GetOperandReference(rDoc, 0, rAddress, DstAddr))
    {
      Label Lbl = rDoc.GetLabelFromAddress(DstAddr);
      OprdName = Lbl.GetLabel();

      if (Lbl.GetType() == Label::Unknown)
        rPrintData.AppendAddress(DstAddr);

      else
        rPrintData.AppendLabel(Lbl.GetLabel());
    }
    else
      rPrintData.AppendImmediate(rOperand.GetValue(), 32);
  }

  return true;
}

bool ArmArchitecture::FormatInstruction(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  PrintData          & rPrintData) const
{
  static char const* Suffix[] = { "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "", "" };
  std::string Mnem;

  Mnem =  m_Mnemonic[rInsn.GetOpcode()];
  Mnem += Suffix[rInsn.GetTestedFlags() & 0xf];
  if (rInsn.GetPrefix() & ARM_Prefix_S)
    Mnem += "s";

  char const* Sep = "\0";

  rPrintData.AppendMnemonic(Mnem);

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand const* pOprd = rInsn.Operand(i);
    if (pOprd == nullptr)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (*Sep != '\0')
      rPrintData.AppendOperator(Sep).AppendSpace();

    if (!FormatOperand(rDoc, rAddr, rInsn, *pOprd, i, rPrintData))
      return false;

    Sep = ",";
  }

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
