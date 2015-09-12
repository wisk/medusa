#include "st62_architecture.hpp"
#include <sstream>
#include <medusa/medusa.hpp>
#include "medusa/expression_visitor.hpp"

char const* St62Architecture::ST62CpuInformation::ConvertIdentifierToName(u32 Id) const
{
  switch (Id)
  {
  default: return "";

    /* Flag */
  case ST62_Flg_C: return "c"; case ST62_Flg_Z: return "z";

    /* Register */
  case ST62_Reg_A: return "A"; case ST62_Reg_X: return "X"; case ST62_Reg_Y: return "Y";
  case ST62_Reg_V: return "V"; case ST62_Reg_W: return "W";

    /* Pseudo-register */
  case ST62_Reg_Pc: return "pc";
  case ST62_Reg_Stk0: return "stk0";
  case ST62_Reg_Stk1: return "stk1";
  case ST62_Reg_Stk2: return "stk2";
  case ST62_Reg_Stk3: return "stk3";
  case ST62_Reg_Stk4: return "stk4";
  case ST62_Reg_Stk5: return "stk5";
  }
}

u32 St62Architecture::ST62CpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::unordered_map<std::string, u32> s_NameToId;
  if (s_NameToId.empty())
  {
    s_NameToId["c"] = ST62_Flg_C; s_NameToId["z"] = ST62_Flg_Z;
    s_NameToId["A"] = ST62_Reg_A; s_NameToId["X"] = ST62_Reg_X; s_NameToId["Y"] = ST62_Reg_Y;
    s_NameToId["V"] = ST62_Reg_V; s_NameToId["W"] = ST62_Reg_W;
    s_NameToId["pc"] = ST62_Reg_Pc;
    s_NameToId["stk0"] = ST62_Reg_Stk0;  s_NameToId["stk1"] = ST62_Reg_Stk1;
    s_NameToId["stk2"] = ST62_Reg_Stk2;  s_NameToId["stk3"] = ST62_Reg_Stk3;
    s_NameToId["stk4"] = ST62_Reg_Stk4;  s_NameToId["stk5"] = ST62_Reg_Stk5;

  }
  auto itId = s_NameToId.find(rName);
  if (itId == std::end(s_NameToId))
    return 0;

  return itId->second;
}

u32 St62Architecture::ST62CpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  switch (RegType)
  {
  default:                     return 0;
  case ProgramPointerRegister: return ST62_Reg_Pc;
  }
}

u32 St62Architecture::ST62CpuInformation::GetSizeOfRegisterInBit(u32 Id) const
{
  switch (Id)
  {
  default: return 0;

    /* Flag */
  case ST62_Flg_C: case ST62_Flg_Z:
    return 1;

    /* Register */
  case ST62_Reg_A: case ST62_Reg_X:
  case ST62_Reg_Y:  case ST62_Reg_V:
  case ST62_Reg_W:
    return 8;

    /* Pseudo-register */
  case ST62_Reg_Stk0: case ST62_Reg_Stk1:
  case ST62_Reg_Stk2: case ST62_Reg_Stk3:
  case ST62_Reg_Stk4: case ST62_Reg_Stk5:
  case ST62_Reg_Pc:
    return 16;
  }
}

bool St62Architecture::St62CpuContext::ReadRegister(u32 Reg, void* pVal, u32 BitSize) const
{
#define READ_F(flg)        if (BitSize != 1)  return false; *reinterpret_cast<bool*>(pVal) = (m_Context.flg) ? true : false;
#define READ_REGISTER(reg) if (BitSize != 8)  return false; *reinterpret_cast<u8  *>(pVal) = m_Context.reg;
#define READ_PC(reg)       if (BitSize != 16) return false; *reinterpret_cast<u16 *>(pVal) = m_Context.reg;
  switch (Reg)
  {
  case ST62_Flg_Z: READ_F(ZF); break;
  case ST62_Flg_C: READ_F(CF); break;


  case ST62_Reg_A: READ_REGISTER(A); break;
  case ST62_Reg_X: READ_REGISTER(X); break;
  case ST62_Reg_Y: READ_REGISTER(Y); break;
  case ST62_Reg_V: READ_REGISTER(V); break;
  case ST62_Reg_W: READ_REGISTER(W); break;
  case ST62_Reg_Pc:READ_PC(Pc); break;
  case ST62_Reg_Stk0:READ_PC(Stk[0]); break;
  case ST62_Reg_Stk1:READ_PC(Stk[1]); break;
  case ST62_Reg_Stk2:READ_PC(Stk[2]); break;
  case ST62_Reg_Stk3:READ_PC(Stk[3]); break;
  case ST62_Reg_Stk4:READ_PC(Stk[4]); break;
  case ST62_Reg_Stk5:READ_PC(Stk[5]); break;
  default: return false;
  }
#undef READ_REGISTER
#undef READ_F
  return true;
}

bool St62Architecture::St62CpuContext::WriteRegister(u32 Reg, void const* pVal, u32 BitSize)
{
#define WRITE_F(flg)        if (BitSize != 1)  return false; m_Context.flg = *reinterpret_cast<u8 const*>(pVal) ? true : false;
#define WRITE_REGISTER(reg) if (BitSize != 8)  return false; m_Context.reg = *reinterpret_cast<u8 const*>(pVal);
#define WRITE_PC(reg)       if (BitSize != 16) return false; m_Context.reg = *reinterpret_cast<u16 const*>(pVal);
  switch (Reg)
  {
  case ST62_Flg_Z: WRITE_F(ZF); break;
  case ST62_Flg_C: WRITE_F(CF); break;


  case ST62_Reg_A: WRITE_REGISTER(A); break;
  case ST62_Reg_X: WRITE_REGISTER(X); break;
  case ST62_Reg_Y: WRITE_REGISTER(Y); break;
  case ST62_Reg_V: WRITE_REGISTER(V); break;
  case ST62_Reg_W: WRITE_REGISTER(W); break;
  case ST62_Reg_Pc:WRITE_PC(Pc); break;
  case ST62_Reg_Stk0:WRITE_PC(Stk[0]); break;
  case ST62_Reg_Stk1:WRITE_PC(Stk[1]); break;
  case ST62_Reg_Stk2:WRITE_PC(Stk[2]); break;
  case ST62_Reg_Stk3:WRITE_PC(Stk[3]); break;
  case ST62_Reg_Stk4:WRITE_PC(Stk[4]); break;
  case ST62_Reg_Stk5:WRITE_PC(Stk[5]); break;
  default: return false;
  }
#undef WRITE_REGISTER
#undef WRITE_F
#undef WRITE_PC
  return true;
}

bool St62Architecture::St62CpuContext::Translate(Address const& rLogicalAddress, u64& rLinearAddress) const
{
  rLinearAddress = rLogicalAddress.GetBase() + rLogicalAddress.GetOffset();
  return true;
}

u8 St62Architecture::St62CpuContext::GetMode(void) const
{
  return ST62_Mode_Original;
}

void St62Architecture::St62CpuContext::SetMode(u8 Mode)
{
}

bool St62Architecture::St62CpuContext::GetAddress(CpuContext::AddressKind AddrKind, Address& rAddr) const
{
  switch (AddrKind)
  {
    case AddressExecution:
      rAddr = Address(Address::BankType, 0, m_Context.Pc, 8, 16);
      return true;

    default:
      return false;
  }
}

bool St62Architecture::St62CpuContext::SetAddress(CpuContext::AddressKind AddrKind, Address const& rAddr)
{
  switch (AddrKind)
  {
  case AddressExecution:
    m_Context.Pc = static_cast<u16>(rAddr.GetOffset());
    return true;

  default:
    return false;
  }
}

std::string St62Architecture::St62CpuContext::ToString(void) const
{
  return (boost::format(
      "A:0x%02x X:0x%02x Y: 0x%02x V:0x%02x W:0x%02x\n"
      "pc:%03x flags: %s%s  stack:%03x %03x %03x %03x %03x %03x")
    % (u16)m_Context.A % (u16)m_Context.X % (u16)m_Context.Y
    % (u16)m_Context.V % (u16)m_Context.W
    % m_Context.Pc
    % (m_Context.CF ? "C" : "c")
    % (m_Context.ZF ? "Z" : "z")
    % m_Context.Stk[0] % m_Context.Stk[1]
    % m_Context.Stk[2] % m_Context.Stk[2]
    % m_Context.Stk[4] % m_Context.Stk[3]
    ).str();
}

void* St62Architecture::St62CpuContext::GetRegisterAddress(u32 Register)
{
  switch (Register)
  {
    case ST62_Reg_A:  return &m_Context.A;
    case ST62_Reg_X:  return &m_Context.X;
    case ST62_Reg_Y:  return &m_Context.Y;
    case ST62_Reg_V:  return &m_Context.V;
    case ST62_Reg_W:  return &m_Context.W;
    case ST62_Reg_Pc: return &m_Context.Pc;
    case ST62_Reg_Stk0: return &m_Context.Stk[0];
    case ST62_Reg_Stk1: return &m_Context.Stk[1];
    case ST62_Reg_Stk2: return &m_Context.Stk[2];
    case ST62_Reg_Stk3: return &m_Context.Stk[3];
    case ST62_Reg_Stk4: return &m_Context.Stk[4];
    case ST62_Reg_Stk5: return &m_Context.Stk[5];
    default: return nullptr;
  }
}

void* St62Architecture::St62CpuContext::GetContextAddress(void)
{
  return &m_Context;
}

u16 St62Architecture::St62CpuContext::GetRegisterOffset(u32 Register)
{
#define OFF(r,reg)   case r: return static_cast<u16>(offsetof(Context, reg));

  switch (Register)
  {
  OFF(ST62_Flg_Z,  ZF);
  OFF(ST62_Flg_C,  CF);
  OFF(ST62_Reg_A,  A);
  OFF(ST62_Reg_X,  X);
  OFF(ST62_Reg_Y,  Y);
  OFF(ST62_Reg_V,  V);
  OFF(ST62_Reg_W,  W);
  OFF(ST62_Reg_Pc, Pc);
  OFF(ST62_Reg_Stk0, Stk[0]);
  OFF(ST62_Reg_Stk1, Stk[1]);
  OFF(ST62_Reg_Stk2, Stk[2]);
  OFF(ST62_Reg_Stk3, Stk[3]);
  OFF(ST62_Reg_Stk4, Stk[4]);
  OFF(ST62_Reg_Stk5, Stk[5]);
  default:           break;
  }
#undef OFF
  return -1;
}

void St62Architecture::St62CpuContext::GetRegisters(CpuContext::RegisterList& RegList) const
{
  RegList.push_back(ST62_Flg_Z);
  RegList.push_back(ST62_Flg_C);

  RegList.push_back(ST62_Reg_A);
  RegList.push_back(ST62_Reg_X);
  RegList.push_back(ST62_Reg_Y);
  RegList.push_back(ST62_Reg_V);
  RegList.push_back(ST62_Reg_W);
  RegList.push_back(ST62_Reg_Pc);

  RegList.push_back(ST62_Reg_Stk0);
  RegList.push_back(ST62_Reg_Stk1);
  RegList.push_back(ST62_Reg_Stk2);
  RegList.push_back(ST62_Reg_Stk3);
  RegList.push_back(ST62_Reg_Stk4);
  RegList.push_back(ST62_Reg_Stk5);
}


bool St62Architecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  rInsn.GetData()->ArchitectureTag() = GetTag();
  rInsn.Mode() = Mode;

  u8 Opcode;
  if (!rBinStrm.Read(Offset, Opcode))
    return false;
  bool Res = (this->*m_Table_1[Opcode & 0xF])(rBinStrm, Offset, rInsn, Mode);
  rInsn.SetName(m_Mnemonic[rInsn.GetOpcode()]);
  return Res;
}

namespace
{
  class OperandFormatter : public ExpressionVisitor
{
public:
  OperandFormatter(Document const& rDoc, PrintData& rPrintData, u8 Mode)
    : m_rDoc(rDoc), m_rPrintData(rPrintData), m_Mode(Mode) {}

  virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr)
  {
    Address const OprdAddr(spConstExpr->GetConstant().ConvertTo<TOffset>());
    auto OprdLbl = m_rDoc.GetLabelFromAddress(OprdAddr);
    if (OprdLbl.GetType() != Label::Unknown)
    {
      m_rPrintData.AppendLabel(OprdLbl.GetLabel());
      return nullptr;
    }

    m_rPrintData.AppendImmediate(spConstExpr->GetConstant(), 16);
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
    auto spBaseConst = expr_cast<ConstantExpression>(spMemExpr->GetBaseExpression());
    auto spOffConst = expr_cast<ConstantExpression>(spMemExpr->GetOffsetExpression());

    // offset isn't a constant, so it's a indirect reference
    if (spOffConst == nullptr) {
      m_rPrintData.AppendOperator("(");
      spMemExpr->GetOffsetExpression()->Visit(this);
      m_rPrintData.AppendOperator(")");
      return nullptr;
    }

    Address rAddress = m_rDoc.MakeAddress(spBaseConst->GetConstant().ConvertTo<TBase>(),
                                          spOffConst->GetConstant().ConvertTo<TOffset>());

    auto OprdLbl = m_rDoc.GetLabelFromAddress(rAddress);
    if (OprdLbl.GetType() != Label::Unknown)
    {
      m_rPrintData.AppendLabel(OprdLbl.GetLabel());
      return nullptr;
    }

    auto const pMemArea = m_rDoc.GetMemoryArea(rAddress);
    if (pMemArea != nullptr)
    {
      m_rPrintData.AppendOperator(pMemArea->GetName());
      m_rPrintData.AppendOperator(":");
    }
    spMemExpr->GetOffsetExpression()->Visit(this);
    return spMemExpr;
  }

private:
  Document const& m_rDoc;
  PrintData& m_rPrintData;
  u8 m_Mode;
};
}

bool St62Architecture::FormatOperand(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  u8                   OperandNo,
  PrintData          & rPrintData) const
{
  auto spCurOprd = rInsn.GetOperand(OperandNo);
  if (spCurOprd == nullptr)
    return false;

  // TODO: rAddr+InsnLen is not always equivalent to PC!
  EvaluateVisitor EvalVst(rDoc, rAddr + rInsn.GetLength(), rInsn.GetMode(), false);
  auto spEvalRes = spCurOprd->Visit(&EvalVst);
  if (spEvalRes != nullptr)
    spCurOprd = spEvalRes;

  OperandFormatter OF(rDoc, rPrintData, rInsn.GetMode());
  spCurOprd->Visit(&OF);

  return true;
}

bool St62Architecture::EmitSetExecutionAddress(Expression::VSPType& rExprs, Address const& rAddr, u8 Mode)
{
  u32 Id = m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode);
  if (Id == 0)
    return false;
  u32 IdSz = m_CpuInfo.GetSizeOfRegisterInBit(Id);
  if (IdSz == 0)
    return false;
  rExprs.push_back(Expr::MakeAssign(Expr::MakeId(Id, &m_CpuInfo), Expr::MakeConst(IdSz, rAddr.GetOffset())));
  return true;
}
