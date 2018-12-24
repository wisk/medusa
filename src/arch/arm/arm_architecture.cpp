#include "arm_architecture.hpp"
#include "arm_calling_convention.hpp"

#include <medusa/expression_visitor.hpp>

#include <boost/algorithm/string/join.hpp>
#include <fmt/format.h>

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
  static std::map<u32, char const*> s_IdToName
  {
    { ARM_Fl_Nf,   "nf"  },
    { ARM_Fl_Cf,   "cf"  },
    { ARM_Fl_Vf,   "vf"  },
    { ARM_Fl_Zf,   "zf"  },

    { ARM_Reg_R0,  "r0"  },
    { ARM_Reg_R1,  "r1"  },
    { ARM_Reg_R2,  "r2"  },
    { ARM_Reg_R3,  "r3"  },
    { ARM_Reg_R4,  "r4"  },
    { ARM_Reg_R5,  "r5"  },
    { ARM_Reg_R6,  "r6"  },
    { ARM_Reg_R7,  "r7"  },
    { ARM_Reg_R8,  "r8"  },
    { ARM_Reg_R9,  "r9"  },
    { ARM_Reg_R10, "r10" },
    { ARM_Reg_R11, "fp"  },
    { ARM_Reg_R12, "ip"  },
    { ARM_Reg_R13, "sp"  },
    { ARM_Reg_R14, "lr"  },
    { ARM_Reg_R15, "pc"  },
  };
  auto itResult = s_IdToName.find(Id);
  if (itResult == std::end(s_IdToName))
    return nullptr;

  return itResult->second;
}

u32 ArmArchitecture::ARMCpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::map<std::string, u32> s_NameToId
  {
    { "nf",  ARM_Fl_Nf   },
    { "cf",  ARM_Fl_Cf   },
    { "vf",  ARM_Fl_Vf   },
    { "zf",  ARM_Fl_Zf   },

    { "r0",  ARM_Reg_R0  },
    { "r1",  ARM_Reg_R1  },
    { "r2",  ARM_Reg_R2  },
    { "r3",  ARM_Reg_R3  },
    { "r4",  ARM_Reg_R4  },
    { "r5",  ARM_Reg_R5  },
    { "r6",  ARM_Reg_R6  },
    { "r7",  ARM_Reg_R7  },
    { "r8",  ARM_Reg_R8  },
    { "r9",  ARM_Reg_R9  },
    { "r10", ARM_Reg_R10 },
    { "r11", ARM_Reg_R11 },
    { "r12", ARM_Reg_R12 },
    { "r13", ARM_Reg_R13 },
    { "r14", ARM_Reg_R14 },
    { "r15", ARM_Reg_R15 },
    { "fp",  ARM_Reg_Fp  },
    { "ip",  ARM_Reg_Ip  },
    { "sp",  ARM_Reg_Sp  },
    { "lr",  ARM_Reg_Lr  },
    { "pc",  ARM_Reg_Pc  },
  };
  auto itResult = s_NameToId.find(rName);
  if (itResult == std::end(s_NameToId))
    return 0;

  return itResult->second;
}

u32 ArmArchitecture::ARMCpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  switch (RegType)
  {
  case StackPointerRegister:   return ARM_Reg_Sp;
  case ProgramPointerRegister: return ARM_Reg_Pc;
  default:                     return ARM_Id_Unknown;
  };
}

u32 ArmArchitecture::ARMCpuInformation::GetSizeOfRegisterInBit(u32 Id) const
{
  switch (Id)
  {
  case ARM_Fl_Nf: case ARM_Fl_Cf: case ARM_Fl_Vf: case ARM_Fl_Zf:
    return 1;

  case ARM_Reg_R0:  case ARM_Reg_R1: case ARM_Reg_R2:  case ARM_Reg_R3:
  case ARM_Reg_R4:  case ARM_Reg_R5: case ARM_Reg_R6:  case ARM_Reg_R7:
  case ARM_Reg_R8:  case ARM_Reg_R9: case ARM_Reg_R10: case ARM_Reg_R11:
  case ARM_Reg_R12: case ARM_Reg_R13:case ARM_Reg_R14: case ARM_Reg_R15:
    return 32;

  default:
    return 0;
  }
}

bool ArmArchitecture::ARMCpuContext::ReadRegister(u32 Reg, void* pVal, u32 BitSize) const
{
#define READ_FLAG(fl) if (BitSize != 1) return false; *reinterpret_cast<u8*>(pVal) = (m_Context.CPSR & fl ? 1 : 0);
#define READ_REGISTER(idx) if (BitSize != 32) return false; memcpy(pVal, &m_Context.Registers[idx], 4)
  switch (Reg)
  {
  case ARM_Fl_Nf: READ_FLAG(ARM_CSPR_N); break;
  case ARM_Fl_Cf: READ_FLAG(ARM_CSPR_C); break;
  case ARM_Fl_Vf: READ_FLAG(ARM_CSPR_V); break;
  case ARM_Fl_Zf: READ_FLAG(ARM_CSPR_Z); break;

  case ARM_Reg_R0:  READ_REGISTER(0);  break;
  case ARM_Reg_R1:  READ_REGISTER(1);  break;
  case ARM_Reg_R2:  READ_REGISTER(2);  break;
  case ARM_Reg_R3:  READ_REGISTER(3);  break;
  case ARM_Reg_R4:  READ_REGISTER(4);  break;
  case ARM_Reg_R5:  READ_REGISTER(5);  break;
  case ARM_Reg_R6:  READ_REGISTER(6);  break;
  case ARM_Reg_R7:  READ_REGISTER(7);  break;
  case ARM_Reg_R8:  READ_REGISTER(8);  break;
  case ARM_Reg_R9:  READ_REGISTER(9);  break;
  case ARM_Reg_R10: READ_REGISTER(10); break;
  case ARM_Reg_R11: READ_REGISTER(11); break;
  case ARM_Reg_R12: READ_REGISTER(12); break;
  case ARM_Reg_R13: READ_REGISTER(13); break;
  case ARM_Reg_R14: READ_REGISTER(14); break;
  case ARM_Reg_R15: READ_REGISTER(15); break;
  default: return false;
  }
#undef READ_REGISTER
#undef READ_FLAG
  return true;
}

bool ArmArchitecture::ARMCpuContext::WriteRegister(u32 Reg, void const* pVal, u32 BitSize)
{
#define WRITE_FLAG(fl) if (BitSize != 1) return false; if (reinterpret_cast<u8 const*>(pVal)) m_Context.CPSR |= fl; else m_Context.CPSR &= ~fl;
#define WRITE_REGISTER(idx) if (BitSize != 32) return false; memcpy(&m_Context.Registers[idx], pVal, 4)
  switch (Reg)
  {
  case ARM_Fl_Nf: WRITE_FLAG(ARM_CSPR_N); break;
  case ARM_Fl_Cf: WRITE_FLAG(ARM_CSPR_C); break;
  case ARM_Fl_Vf: WRITE_FLAG(ARM_CSPR_V); break;
  case ARM_Fl_Zf: WRITE_FLAG(ARM_CSPR_Z); break;

  case ARM_Reg_R0:  WRITE_REGISTER(0);  break;
  case ARM_Reg_R1:  WRITE_REGISTER(1);  break;
  case ARM_Reg_R2:  WRITE_REGISTER(2);  break;
  case ARM_Reg_R3:  WRITE_REGISTER(3);  break;
  case ARM_Reg_R4:  WRITE_REGISTER(4);  break;
  case ARM_Reg_R5:  WRITE_REGISTER(5);  break;
  case ARM_Reg_R6:  WRITE_REGISTER(6);  break;
  case ARM_Reg_R7:  WRITE_REGISTER(7);  break;
  case ARM_Reg_R8:  WRITE_REGISTER(8);  break;
  case ARM_Reg_R9:  WRITE_REGISTER(9);  break;
  case ARM_Reg_R10: WRITE_REGISTER(10); break;
  case ARM_Reg_R11: WRITE_REGISTER(11); break;
  case ARM_Reg_R12: WRITE_REGISTER(12); break;
  case ARM_Reg_R13: WRITE_REGISTER(13); break;
  case ARM_Reg_R14: WRITE_REGISTER(14); break;
  case ARM_Reg_R15: WRITE_REGISTER(15); break;
  default: return false;
  }
#undef WRITE_REGISTER
#undef READ_REGISTER
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

bool ArmArchitecture::ARMCpuContext::GetAddress(CpuContext::AddressKind AddrKind, Address& rAddr) const
{
  switch (AddrKind)
  {
    case AddressExecution:
      rAddr = Address(Address::LinearType, 0, m_Context.Registers[15], 0, 32);
      return true;

    default:
      return false;
  }
}

bool ArmArchitecture::ARMCpuContext::SetAddress(CpuContext::AddressKind AddrKind, Address const& rAddr)
{
  switch (AddrKind)
  {
  case AddressExecution:
    m_Context.Registers[15] = static_cast<u32>(rAddr.GetOffset());
    return true;

  default:
    return false;
  }
}

std::string ArmArchitecture::ARMCpuContext::ToString(void) const
{
  std::string CPSR = "";
  CPSR += (m_Context.CPSR & ARM_CSPR_N) ? "N" : "n";
  CPSR += (m_Context.CPSR & ARM_CSPR_Z) ? "Z" : "z";
  CPSR += (m_Context.CPSR & ARM_CSPR_C) ? "C" : "c";
  CPSR += (m_Context.CPSR & ARM_CSPR_V) ? "V" : "v";
  CPSR += (m_Context.CPSR & ARM_CSPR_T) ? "T" : "t";

  return fmt::format(
      "r0:0x%08x r1:0x%08x r2: 0x%08x r3:%08x\n"
      "r4:0x%08x r5:0x%08x r6: 0x%08x r7:%08x\n"
      "r8:0x%08x r9:0x%08x r10:0x%08x fp:%08x\n"
      "ip:0x%08x sp:0x%08x lr: 0x%08x pc:%08x\n"
      "CPSR: %s\n"
    , m_Context.Registers[0],  m_Context.Registers[1],  m_Context.Registers[2],  m_Context.Registers[3]
    , m_Context.Registers[4],  m_Context.Registers[5],  m_Context.Registers[6],  m_Context.Registers[7]
    , m_Context.Registers[8],  m_Context.Registers[9],  m_Context.Registers[10], m_Context.Registers[11]
    , m_Context.Registers[12], m_Context.Registers[13], m_Context.Registers[14], m_Context.Registers[15]
    , CPSR);
}

void* ArmArchitecture::ARMCpuContext::GetRegisterAddress(u32 Register)
{
  switch (Register)
  {
    case ARM_Reg_R0:  return &m_Context.Registers[0];
    case ARM_Reg_R1:  return &m_Context.Registers[1];
    case ARM_Reg_R2:  return &m_Context.Registers[2];
    case ARM_Reg_R3:  return &m_Context.Registers[3];
    case ARM_Reg_R4:  return &m_Context.Registers[4];
    case ARM_Reg_R5:  return &m_Context.Registers[5];
    case ARM_Reg_R6:  return &m_Context.Registers[6];
    case ARM_Reg_R7:  return &m_Context.Registers[7];
    case ARM_Reg_R8:  return &m_Context.Registers[8];
    case ARM_Reg_R9:  return &m_Context.Registers[9];
    case ARM_Reg_R10: return &m_Context.Registers[10];
    case ARM_Reg_R11: return &m_Context.Registers[11];
    case ARM_Reg_R12: return &m_Context.Registers[12];
    case ARM_Reg_R13: return &m_Context.Registers[13];
    case ARM_Reg_R14: return &m_Context.Registers[14];
    case ARM_Reg_R15: return &m_Context.Registers[15];
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
  RegList.push_back(ARM_Fl_Nf);
  RegList.push_back(ARM_Fl_Cf);
  RegList.push_back(ARM_Fl_Vf);
  RegList.push_back(ARM_Fl_Zf);

  RegList.push_back(ARM_Reg_R0);
  RegList.push_back(ARM_Reg_R1);
  RegList.push_back(ARM_Reg_R2);
  RegList.push_back(ARM_Reg_R3);
  RegList.push_back(ARM_Reg_R4);
  RegList.push_back(ARM_Reg_R5);
  RegList.push_back(ARM_Reg_R6);
  RegList.push_back(ARM_Reg_R7);
  RegList.push_back(ARM_Reg_R8);
  RegList.push_back(ARM_Reg_R9);
  RegList.push_back(ARM_Reg_R10);
  RegList.push_back(ARM_Reg_R11);
  RegList.push_back(ARM_Reg_R12);
  RegList.push_back(ARM_Reg_R13);
  RegList.push_back(ARM_Reg_R14);
  RegList.push_back(ARM_Reg_R15);
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
      bool CommaInserted = false;

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
        {
          m_rPrintData.AppendOperator(",").AppendSpace();
          CommaInserted = true;
        }
      }
      else
        IncReg = true;
      if (itReg == itEnd)
        break;
      if (!CommaInserted && itReg + 1 != itEnd)
        m_rPrintData.AppendOperator(",").AppendSpace();

      if (IncReg)
        ++itReg;

      CommaInserted = false;
    }

    m_rPrintData.AppendSpace().AppendOperator("}");

    return spVecIdExpr;
  }

  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
  {
    if (spBinOpExpr->GetLeftExpression()->Visit(this) == nullptr)
      return nullptr;
    char const* pOpTok = "???";
    switch (spBinOpExpr->GetOperation())
    {
    default: break;
    case OperationExpression::OpAnd:  pOpTok = "&";   break;
    case OperationExpression::OpOr:   pOpTok = "|";   break;
    case OperationExpression::OpXor:  pOpTok = "^";   break;
    case OperationExpression::OpLls:  pOpTok = "LSL"; break;
    case OperationExpression::OpLrs:  pOpTok = "LSR"; break;
    case OperationExpression::OpArs:  pOpTok = "ASR"; break;
    case OperationExpression::OpRol:  pOpTok = "ROL"; break;
    case OperationExpression::OpRor:  pOpTok = "ROR"; break;
    case OperationExpression::OpAdd:  pOpTok = "+";   break;
    case OperationExpression::OpSub:  pOpTok = "-";   break;
    case OperationExpression::OpMul:  pOpTok = "*";   break;
    case OperationExpression::OpSDiv:
    case OperationExpression::OpUDiv: pOpTok = "/";   break;
    case OperationExpression::OpSMod:
    case OperationExpression::OpUMod: pOpTok = "%";   break;
    }
    m_rPrintData.AppendSpace().AppendOperator(pOpTok).AppendSpace();
    if (spBinOpExpr->GetRightExpression()->Visit(this) == nullptr)
      return nullptr;
    return spBinOpExpr;
  }

  virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spConstExpr)
  {
    Address const OprdAddr(spConstExpr->GetInt().ConvertTo<OffsetType>());
    auto OprdLbl = m_rDoc.GetLabelFromAddress(OprdAddr);
    if (OprdLbl.GetType() != Label::Unknown)
    {
      m_rPrintData.AppendLabel(OprdLbl.GetLabel());
      return spConstExpr;
    }

    m_rPrintData.AppendImmediate(spConstExpr->GetInt());
    return spConstExpr;
  }

  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr)
  {
    auto const pCpuInfo = spIdExpr->GetCpuInformation();
    auto Id = spIdExpr->GetId();
    auto IdName = pCpuInfo->ConvertIdentifierToName(Id);
    if (IdName == nullptr)
      return nullptr;
    m_rPrintData.AppendRegister(IdName);
    return spIdExpr;
  }

  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr)
  {
    m_rPrintData.AppendOperator("[");
    auto spOff = spMemExpr->GetOffsetExpression();
    spOff->Visit(this);
    m_rPrintData.AppendOperator("]");
    return spMemExpr;
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

  if (spIdOprd == nullptr || spIdOprd->GetId() != ARM_Reg_Pc || OperandNo != 0)
  {
    u64 PcOff = rInsn.GetMode() == ARM_ModeThumb ? 4 : 8;
    EvaluateVisitor EvalVst(rDoc, rAddr + PcOff, rInsn.GetMode(), true);
    auto spEvalRes = spCurOprd->Visit(&EvalVst);
    if (spEvalRes != nullptr)
      spCurOprd = spEvalRes;

    if (spEvalRes != nullptr && EvalVst.IsRelative() && EvalVst.IsMemoryReference())
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

  char const* Sep = nullptr;

  // TODO(wisk): mnemonic prefix

  std::string Mnem = m_Mnemonic[rInsn.GetOpcode()];
  rPrintData.AppendMnemonic(Mnem);

  // TODO(wisk): mnemonic suffix
  if (rInsn.GetPrefix() & ARM_Prefix_S)
    rPrintData.AppendMnemonicSuffix("s");
  std::string CondSuffix = Suffix[rInsn.GetTestedFlags() & 0xf];
  if (!CondSuffix.empty())
    rPrintData.AppendMnemonicSuffix(CondSuffix);

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

    if (rInsn.GetAttributes() & ARM_Attribute_WriteBack)
    {
      auto spMemOprd = expr_cast<MemoryExpression>(rInsn.GetOperand(OprdIdx));
      if (spMemOprd != nullptr)
        rPrintData.AppendOperator("!");
    }
  }

  return true;
}

CallingConvention const* ArmArchitecture::GetCallingConvention(std::string const& rCallConvName, u8 Mode) const
{
  if (rCallConvName == "aapcs")
  {
    static AapcsCallingConvention s_AapcsCallConv;
    return &s_AapcsCallConv;
  }

  return nullptr;
}

bool ArmArchitecture::HandleExpression(Expression::LSPType & rExprs, std::string const& rName, Instruction& rInsn, Expression::SPType spResExpr)
{
  return false;
}

bool ArmArchitecture::EmitSetExecutionAddress(Expression::VSPType& rExprs, Address const& rAddr, u8 Mode)
{
  u32 Id = m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode);
  if (Id == 0)
    return false;
  u32 IdSz = m_CpuInfo.GetSizeOfRegisterInBit(Id);
  if (IdSz == 0)
    return false;
  rExprs.push_back(Expr::MakeAssign(Expr::MakeId(Id, &m_CpuInfo), Expr::MakeBitVector(IdSz, rAddr.GetOffset())));
  return true;
}
