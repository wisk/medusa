#include "arm_architecture.hpp"

#include <medusa/expression_visitor.hpp>

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

//http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0040d/ch06s02s01.html
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
    s_IdToName[ARM_RegR11] = "fp";
    s_IdToName[ARM_RegR12] = "ip";
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
    s_NameToId["r13"] = ARM_RegR13;
    s_NameToId["r14"] = ARM_RegR14;
    s_NameToId["r15"] = ARM_RegR15;
    s_NameToId["fp"]  = ARM_RegFP;
    s_NameToId["ip"]  = ARM_RegIP;
    s_NameToId["sp"]  = ARM_RegSP;
    s_NameToId["lr"]  = ARM_RegLR;
    s_NameToId["pc"]  = ARM_RegPC;
  }
  auto itResult = s_NameToId.find(rName);
  if (itResult == std::end(s_NameToId))
    return 0;

  return itResult->second;
}

u32 ArmArchitecture::ARMCpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  switch (RegType)
  {
  case StackPointerRegister:   return ARM_RegSP;
  case StackFrameRegister:     return ARM_RegUnknown;
  case ProgramBaseRegister:    return ARM_RegUnknown;
  case ProgramPointerRegister: return ARM_RegPC;
  case AccumulatorRegister:    return ARM_RegUnknown;
  case CounterRegister:        return ARM_RegUnknown;
  case InvalidRegister:        return ARM_RegUnknown;
  default:                     return ARM_RegUnknown;
  };
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

u8 ArmArchitecture::ARMCpuContext::GetMode(void) const
{
  return (m_Context.CPSR & ARM_CSPR_T) ? ARM_ModeThumb : ARM_ModeArm;
}

void ArmArchitecture::ARMCpuContext::SetMode(u8 Mode)
{
  switch (Mode)
  {
  case ARM_ModeArm:
    m_Context.CPSR &= ~ARM_CSPR_T;
    break;

  case ARM_ModeThumb:
    m_Context.CPSR |= ARM_CSPR_T;
    break;

  default:
    break;
  }
}

std::string ArmArchitecture::ARMCpuContext::ToString(void) const
{
  std::string CPSR = "";
  CPSR += (m_Context.CPSR & ARM_CSPR_N) ? "N" : "n";
  CPSR += (m_Context.CPSR & ARM_CSPR_Z) ? "Z" : "z";
  CPSR += (m_Context.CPSR & ARM_CSPR_C) ? "C" : "c";
  CPSR += (m_Context.CPSR & ARM_CSPR_O) ? "O" : "o";
  CPSR += (m_Context.CPSR & ARM_CSPR_T) ? "T" : "t";

  return (boost::format(
      "r0:0x%08x r1:0x%08x r2: 0x%08x r3:%08x\n"
      "r4:0x%08x r5:0x%08x r6: 0x%08x r7:%08x\n"
      "r8:0x%08x r9:0x%08x r10:0x%08x fp:%08x\n"
      "ip:0x%08x sp:0x%08x lr: 0x%08x pc:%08x\n"
      "CPSR: %s\n")
    % m_Context.Registers[0] % m_Context.Registers[1] % m_Context.Registers[2] % m_Context.Registers[3]
    % m_Context.Registers[4] % m_Context.Registers[5] % m_Context.Registers[6] % m_Context.Registers[7]
    % m_Context.Registers[8] % m_Context.Registers[9] % m_Context.Registers[10] % m_Context.Registers[11]
    % m_Context.Registers[12] % m_Context.Registers[13] % m_Context.Registers[14] % m_Context.Registers[15]
    % CPSR).str();
}

void* ArmArchitecture::ARMCpuContext::GetRegisterAddress(u32 Register)
{
  switch (Register)
  {
    case ARM_RegR0:  return &m_Context.Registers[0];
    case ARM_RegR1:  return &m_Context.Registers[1];
    case ARM_RegR2:  return &m_Context.Registers[2];
    case ARM_RegR3:  return &m_Context.Registers[3];
    case ARM_RegR4:  return &m_Context.Registers[4];
    case ARM_RegR5:  return &m_Context.Registers[5];
    case ARM_RegR6:  return &m_Context.Registers[6];
    case ARM_RegR7:  return &m_Context.Registers[7];
    case ARM_RegR8:  return &m_Context.Registers[8];
    case ARM_RegR9:  return &m_Context.Registers[9];
    case ARM_RegR10: return &m_Context.Registers[10];
    case ARM_RegR11: return &m_Context.Registers[11];
    case ARM_RegR12: return &m_Context.Registers[12];
    case ARM_RegR13: return &m_Context.Registers[13];
    case ARM_RegR14: return &m_Context.Registers[14];
    case ARM_RegR15: return &m_Context.Registers[15];
    default: return nullptr;
  }
}

void* ArmArchitecture::ARMCpuContext::GetContextAddress(void)
{
  return &m_Context;
}

u16 ArmArchitecture::ARMCpuContext::GetRegisterOffset(u32 Register)
{
  return static_cast<u16>(Register + sizeof(u32) * Register);
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

Address ArmArchitecture::CurrentAddress(Address const& rAddr, Instruction const& rInsn) const
{
  u64 PcOff = (rInsn.GetMode() == ARM_ModeThumb) ? 4 : 8;
  return rAddr + PcOff;
}

namespace
{
  class OperandFormatter : public ExpressionVisitor
{
public:
  OperandFormatter(Document const& rDoc, PrintData& rPrintData, u8 Mode)
    : m_rDoc(rDoc), m_rPrintData(rPrintData), m_Mode(Mode) {}

  virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
  {
    auto const& rRegs = spVecIdExpr->GetVector();
    u32 LastId = 0;
    CpuInformation const* pCpuInfo = spVecIdExpr->GetCpuInformation();

    m_rPrintData.AppendOperator("{").AppendSpace();

    auto itReg = std::begin(rRegs);
    auto const itEnd = std::end(rRegs);
    while (itReg < itEnd)
    {
      char const* pRegName = pCpuInfo->ConvertIdentifierToName(*itReg);
      assert(pRegName != nullptr);
      m_rPrintData.AppendRegister(pRegName);

      bool IncReg = false;

      if ((itReg + 1) != std::end(rRegs) && *itReg + 1 == *(itReg + 1))
      {
        do
        {
          ++itReg;
        }
        while (itReg + 1 < itEnd && *itReg + 1 == *(itReg + 1));
        pRegName = pCpuInfo->ConvertIdentifierToName(*itReg);
        assert(pRegName != nullptr);
        m_rPrintData.AppendOperator("-").AppendRegister(pRegName);
        ++itReg;
        if (itReg != itEnd)
          m_rPrintData.AppendOperator(",").AppendSpace();
      }
      else
        IncReg = true;
      if (itReg == itEnd)
        break;
      if (itReg + 1 != itEnd)
        m_rPrintData.AppendOperator(",").AppendSpace();

      if (IncReg)
        ++itReg;
    }

    m_rPrintData.AppendSpace().AppendOperator("}");

    return nullptr;
  }

  virtual Expression::SPType VisitOperation(OperationExpression::SPType spOpExpr)
  {
    static const char *s_StrOp[] = { "???", "↔", "&", "|", "^", "LSL", "LSR", "ASR", "+", "-", "*", "/" };
    spOpExpr->GetLeftExpression()->Visit(this);
    auto Op = spOpExpr->GetOperation();
    if (Op >= (sizeof(s_StrOp) / sizeof(*s_StrOp)))
      return nullptr;
    m_rPrintData.AppendSpace().AppendOperator(s_StrOp[Op]).AppendSpace();
    spOpExpr->GetRightExpression()->Visit(this);
    return nullptr;
  }
  virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr)
  {
    Address const OprdAddr(spConstExpr->GetConstant());
    auto OprdLbl = m_rDoc.GetLabelFromAddress(OprdAddr);
    if (OprdLbl.GetType() != Label::Unknown)
    {
      m_rPrintData.AppendLabel(OprdLbl.GetLabel());
      return nullptr;
    }

    m_rPrintData.AppendImmediate(spConstExpr->GetConstant(), spConstExpr->GetSizeInBit());
    return nullptr;
  }

  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr)
  {
    auto const pCpuInfo = spIdExpr->GetCpuInformation();
    auto Id = spIdExpr->GetId();
    auto IdName = pCpuInfo->ConvertIdentifierToName(Id);
    if (IdName == nullptr)
      return nullptr;
    m_rPrintData.AppendRegister(IdName);
    return nullptr;
  }

  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr)
  {
    m_rPrintData.AppendOperator("[");
    auto spOff = spMemExpr->GetOffsetExpression();
    spOff->Visit(this);
    m_rPrintData.AppendOperator("]");
    return nullptr;
  }

private:
  Document const& m_rDoc;
  PrintData& m_rPrintData;
  u8 m_Mode;
};
}

bool ArmArchitecture::FormatOperand(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  u8                   OperandNo,
  PrintData          & rPrintData) const
{
  auto spCurOprd = rInsn.GetOperand(OperandNo);
  if (spCurOprd == nullptr)
    return false;

  // HACK: We don't want to first PC register to be resolved (e.g. LDR PC, =XXXXX)
  auto spIdOprd = expr_cast<IdentifierExpression>(spCurOprd);

  if (spIdOprd == nullptr || spIdOprd->GetId() != ARM_RegPC || OperandNo != 0)
  {
    u64 PcOff = rInsn.GetMode() == ARM_ModeThumb ? 4 : 8;
    EvaluateVisitor EvalVst(rDoc, rAddr + PcOff, rInsn.GetMode(), true);
    auto spEvalRes = spCurOprd->Visit(&EvalVst);
    if (spEvalRes != nullptr)
      spCurOprd = spEvalRes;

    if (EvalVst.IsRelative() && EvalVst.IsMemoryReference())
      rPrintData.AppendOperator("=");
  }

  OperandFormatter OF(rDoc, rPrintData, rInsn.GetMode());
  spCurOprd->Visit(&OF);

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

  char const* Sep = nullptr;

  rPrintData.AppendMnemonic(Mnem);

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

    if (rInsn.GetPrefix() & ARM_Prefix_W)
    {
      auto spMemOprd = expr_cast<MemoryExpression>(rInsn.GetOperand(OprdIdx));
      if (spMemOprd != nullptr)
        rPrintData.AppendOperator("!");
    }
  }

  return true;
}