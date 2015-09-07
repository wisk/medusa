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
  case ST62_Reg_Pc:
    return 16;
  }
}

bool St62Architecture::ST62CpuInformation::IsRegisterAliased(u32 Id0, u32 Id1) const
{
  if (Id0 == Id1)
    return true;

  if (Id0 > Id1)
    std::swap(Id0, Id1);

  return false;
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

