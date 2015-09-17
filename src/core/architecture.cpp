#include "medusa/architecture.hpp"
#include "medusa/module.hpp"
#include "medusa/expression_visitor.hpp"

MEDUSA_NAMESPACE_BEGIN

Architecture::Architecture(Tag ArchTag) : m_Tag(ArchTag)
{
  m_CfgMdl.InsertBoolean("Disassembly only basic block", false);
}


std::string Architecture::GetName(void) const
{
  return "";
};

bool Architecture::Translate(Address const& rVirtAddr, TOffset& rPhysOff)
{
  return false;
}

// NOTE: In most of architecture, current address is instruction address + instruction length
Address Architecture::CurrentAddress(Address const& rAddr, Instruction const& rInsn) const
{
  return rAddr + rInsn.GetLength();
}

bool Architecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  return false;
};

bool Architecture::HandleExpression(Expression::LSPType& rExprs, std::string const& rName, Instruction& rInsn, Expression::SPType spResExpr)
{
  return true;
}

bool Architecture::EmitSetExecutionAddress(Expression::VSPType& rExprs, Address const& rAddr, u8 Mode)
{
  return false;
}

u8 Architecture::GetModeByName(std::string const& rModeName) const
{
  auto const& rModes = GetModes();
  for (auto itMode = std::begin(rModes), itEnd = std::end(rModes); itMode != itEnd; ++itMode)
    if (rModeName == std::get<0>(*itMode))
      return std::get<1>(*itMode);
  return 0;
}

bool Architecture::FormatTypeDetail(TypeDetail const& rTypeDtl, PrintData& rPrintData) const
{
  switch (rTypeDtl.GetType())
  {
  case TypeDetail::ClassType:     rPrintData.AppendKeyword("class").AppendSpace(); break;
  case TypeDetail::EnumType:      rPrintData.AppendKeyword("enum").AppendSpace(); break;
  case TypeDetail::StructureType: rPrintData.AppendKeyword("struct").AppendSpace(); break;
  }

  rPrintData.AppendKeyword(rTypeDtl.GetName());

  switch (rTypeDtl.GetType())
  {
  case TypeDetail::PointerType:   rPrintData.AppendOperator("*").AppendSpace(); break;
  case TypeDetail::ReferenceType: rPrintData.AppendOperator("&").AppendSpace(); break;
  }

  return true;
}

namespace
{
  template<typename _Type>
  class ValueDetailFormatter
  {
  public:
    ValueDetailFormatter(Document const& rDoc, Address const& rAddr, ValueDetail const& rValDtl)
      : m_rDoc(rDoc), m_rAddr(rAddr), m_rValDtl(rValDtl)
    {
    }

    bool operator()(PrintData& rPrintData)
    {
      _Type Value;

      TOffset Off;
      if (!m_rDoc.ConvertAddressToFileOffset(m_rAddr, Off))
        return false;

      if (!m_rDoc.GetBinaryStream().Read(Off, Value))
        return false;

      switch (m_rValDtl.GetType() & ValueDetail::ModifierMask)
      {
      case ValueDetail::NotType:    rPrintData.AppendOperator("~"); Value = ~Value; break;
      case ValueDetail::NegateType: rPrintData.AppendOperator("-"); Value = ~Value + 1; break;
      default:                                                                      break;
      }

      switch (m_rValDtl.GetType() & ValueDetail::BaseMask)
      {
      case ValueDetail::BinaryType:      rPrintData.AppendImmediate(Value, sizeof(Value) * 8, 2); break; // 0b10
      case ValueDetail::DecimalType:     rPrintData.AppendImmediate(Value, sizeof(Value) * 8, 10); break; // 10
      case ValueDetail::HexadecimalType: rPrintData.AppendImmediate(Value, sizeof(Value) * 8, 0x10); break; // 0x10

      case ValueDetail::ReferenceType:
      {
        Address ValAddr = m_rAddr;
        ValAddr.SetOffset(Value); // FIXME: it can be wrong..
        Label Lbl = m_rDoc.GetLabelFromAddress(ValAddr);
        if (Lbl.GetType() != Label::Unknown)
          rPrintData.AppendLabel(Lbl.GetLabel());
        else
          rPrintData.AppendAddress(ValAddr);
      }

      default: return false; // TODO: handle more base type...
      }

      return true;
    }

  private:
    Document const&    m_rDoc;
    Address const&     m_rAddr;
    ValueDetail const& m_rValDtl;
  };
}

bool Architecture::FormatValueDetail(Document const& rDoc, Address const& rAddr, u8 ValueBits, ValueDetail const& rValDtl, PrintData& rPrintData) const
{
  switch (ValueBits)
  {
  case 8:
    {
      ValueDetailFormatter<u8> VDF8(rDoc, rAddr, rValDtl);
      return VDF8(rPrintData);
    }

  case 16:
    {
      ValueDetailFormatter<u16> VDF16(rDoc, rAddr, rValDtl);
      return VDF16(rPrintData);
    }

  case 32:
    {
      ValueDetailFormatter<u32> VDF32(rDoc, rAddr, rValDtl);
      return VDF32(rPrintData);
    }

  case 64:
    {
      ValueDetailFormatter<u64> VDF64(rDoc, rAddr, rValDtl);
      return VDF64(rPrintData);
    }
  }

  return false;
}

bool Architecture::FormatCell(
  Document      const& rDoc,
  Address       const& rAddr,
  Cell          const& rCell,
  PrintData          & rPrintData) const
{
  switch (rCell.GetType())
  {
  case Cell::InstructionType: return FormatInstruction(rDoc, rAddr, static_cast<Instruction const&>(rCell), rPrintData);
  case Cell::ValueType:       return FormatValue      (rDoc, rAddr, static_cast<Value       const&>(rCell), rPrintData);
  case Cell::CharacterType:   return FormatCharacter  (rDoc, rAddr, static_cast<Character   const&>(rCell), rPrintData);
  case Cell::StringType:      return FormatString     (rDoc, rAddr, static_cast<String      const&>(rCell), rPrintData);
  default:                    return false;
  }
}

bool Architecture::FormatInstruction(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  PrintData          & rPrintData) const
{
  char const* Sep = nullptr;

  rPrintData.AppendMnemonic(rInsn.GetName());

  std::string OpRefCmt;
  rDoc.GetComment(rAddr, OpRefCmt);

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

namespace
{
  class OperandFormatter : public ExpressionVisitor
{
public:
  OperandFormatter(Document const& rDoc, PrintData& rPrintData, u8 Mode)
    : m_rDoc(rDoc), m_rPrintData(rPrintData), m_Mode(Mode) {}

  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
  {
    std::string OpTok = "";
    switch (spBinOpExpr->GetOperation())
    {
      case OperationExpression::OpAdd:  OpTok = "+";  break;
      case OperationExpression::OpSub:  OpTok = "-";  break;
      case OperationExpression::OpMul:  OpTok = "*";  break;
      case OperationExpression::OpSDiv:
      case OperationExpression::OpUDiv: OpTok = "/";  break;
      case OperationExpression::OpLls:  OpTok = "<<"; break;
      case OperationExpression::OpLrs:
      case OperationExpression::OpArs:  OpTok = ">>"; break;
    }
    spBinOpExpr->GetLeftExpression()->Visit(this);
    m_rPrintData.AppendSpace().AppendOperator(OpTok).AppendSpace();
    spBinOpExpr->GetRightExpression()->Visit(this);
    return nullptr;
  }
  virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spConstExpr)
  {
    Address const OprdAddr(spConstExpr->GetInt().ConvertTo<TOffset>());
    auto OprdLbl = m_rDoc.GetLabelFromAddress(OprdAddr);
    if (OprdLbl.GetType() != Label::Unknown)
    {
      m_rPrintData.AppendLabel(OprdLbl.GetLabel());
      return nullptr;
    }

    m_rPrintData.AppendImmediate(spConstExpr->GetInt(), 16);
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
    //ref: https://stackoverflow.com/questions/12063840/what-are-the-sizes-of-tword-oword-and-yword-operands
    switch (spMemExpr->GetAccessSizeInBit())
    {
    case 8:   m_rPrintData.AppendKeyword("byte").AppendSpace();  break;
    case 16:  m_rPrintData.AppendKeyword("word").AppendSpace();  break;
    case 32:  m_rPrintData.AppendKeyword("dword").AppendSpace(); break;
    case 64:  m_rPrintData.AppendKeyword("qword").AppendSpace(); break;
    case 80:  m_rPrintData.AppendKeyword("tword").AppendSpace(); break;
    case 128: m_rPrintData.AppendKeyword("oword").AppendSpace(); break;
    case 256: m_rPrintData.AppendKeyword("yword").AppendSpace(); break;
    case 512: m_rPrintData.AppendKeyword("zword").AppendSpace(); break;
    }
    m_rPrintData.AppendOperator("[");
    auto spBase = spMemExpr->GetBaseExpression();
    if (spBase != nullptr)
    {
      spBase->Visit(this);
      m_rPrintData.AppendOperator(":");
    }
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

// TODO: improve how we evaluate/simplify operand expression
bool Architecture::FormatOperand(
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

bool Architecture::FormatCharacter(
  Document      const& rDoc,
  Address       const& rAddr,
  Character     const& rChar,
  PrintData          & rPrintData) const
{
  auto const& rBinStrm = rDoc.GetBinaryStream();
  std::ostringstream oss;
  TOffset Off;

  if (!rDoc.ConvertAddressToFileOffset(rAddr, Off))
    return false;

  switch (rChar.GetSubType())
  {
  case Character::AsciiCharacterType: default:
    {
      s8 Char;
      if (!rBinStrm.Read(Off, Char))
        return false;

      switch (Char)
      {
      case '\0': oss << "\\0"; break;
      case '\a': oss << "\\a"; break;
      case '\b': oss << "\\b"; break;
      case '\t': oss << "\\t"; break;
      case '\n': oss << "\\n"; break;
      case '\v': oss << "\\v"; break;
      case '\f': oss << "\\f"; break;
      case '\r': oss << "\\r"; break;
      default:   oss << Char;  break;
      }
    }
  }
  rPrintData.AppendCharacter(oss.str());
  return true;
}

bool Architecture::FormatString(
  Document      const& rDoc,
  Address       const& rAddr,
  String        const& rStr,
  PrintData          & rPrintData) const
{
  auto const&        rBinStrm  = rDoc.GetBinaryStream();
  TOffset            FileOff;
  size_t             StrLen    = rStr.GetLength();
  StringTrait const* pStrTrait = rStr.GetStringTrait();

  if (pStrTrait == nullptr)
    return false;

  if (rDoc.ConvertAddressToFileOffset(rAddr, FileOff) == false)
    return false;

  char* pStrBuf = new char[StrLen];
  if (rDoc.GetBinaryStream().Read(FileOff, pStrBuf, StrLen) == false)
  {
    delete[] pStrBuf;
    return false;
  }

  std::string OrgStr = pStrTrait->ConvertToUtf8(pStrBuf, StrLen);
  delete[] pStrBuf;
  if (OrgStr.empty())
    return false;
  std::string FmtStr;

  if (rStr.GetSubType() == String::Utf16Type)
    rPrintData.AppendKeyword("L");

  rPrintData.AppendOperator("\"");

  auto itCharEnd = std::end(OrgStr);
  size_t FmtStrBeg = FmtStr.length();
  for (auto itChar = std::begin(OrgStr); itChar != itCharEnd; ++itChar)
  {
    switch (*itChar)
    {
    case '\0': FmtStr += "\\0";   break;
    case '\\': FmtStr += "\\\\";  break;
    case '\a': FmtStr += "\\a";   break;
    case '\b': FmtStr += "\\b";   break;
    case '\t': FmtStr += "\\t";   break;
    case '\n': FmtStr += "\\n";   break;
    case '\v': FmtStr += "\\v";   break;
    case '\f': FmtStr += "\\f";   break;
    case '\r': FmtStr += "\\r";   break;
    default:   FmtStr += *itChar; break;
    }
  }

  rPrintData.AppendString(FmtStr);
  rPrintData.AppendOperator("\"");
  return true;
}

bool Architecture::FormatValue(
  Document      const& rDoc,
  Address       const& rAddr,
  Value         const& rVal,
  PrintData          & rPrintData) const
{
  auto const&         rBinStrm = rDoc.GetBinaryStream();
  TOffset             Off;
  u8                  ValueModifier = rVal.GetSubType() & ValueDetail::ModifierMask;
  u8                  ValueType     = rVal.GetSubType() & ValueDetail::BaseMask;
  std::string         BasePrefix    = "";
  bool                IsUnk = false;

  switch (rVal.GetLength())
  {
  case 1:  rPrintData.AppendKeyword("db").AppendSpace(); break;
  case 2:  rPrintData.AppendKeyword("dw").AppendSpace(); break;
  case 4:  rPrintData.AppendKeyword("dd").AppendSpace(); break;
  case 8:  rPrintData.AppendKeyword("dq").AppendSpace(); break;
  default: rPrintData.AppendKeyword("d?").AppendSpace(); break;
  }

  if (!rDoc.ConvertAddressToFileOffset(rAddr, Off))
  {
    rPrintData.AppendOperator("(?)");
    return true;
  }

  Id BindId;
  ValueDetail ValDtl("", static_cast<ValueDetail::Type>(rVal.GetSubType()), Id());

  if (rDoc.RetrieveDetailId(rAddr, 0, BindId))
    rDoc.GetValueDetail(BindId, ValDtl);

  return FormatValueDetail(rDoc, rAddr, static_cast<u8>(rVal.GetLength() * 8), ValDtl, rPrintData);
}

bool Architecture::FormatMultiCell(
  Document      const& rDoc,
  Address       const& rAddress,
  MultiCell     const& rMultiCell,
  PrintData          & rPrintData) const
{
  switch (rMultiCell.GetType())
  {
  case MultiCell::FunctionType: return FormatFunction(rDoc, rAddress, static_cast<Function const&>(rMultiCell), rPrintData);
  default:                      return false;
  }
}

bool Architecture::FormatFunction(
  Document      const& rDoc,
  Address       const& rAddr,
  Function      const& rFunc,
  PrintData          & rPrintData) const
{
  auto FuncLabel = rDoc.GetLabelFromAddress(rAddr);

  if (rFunc.GetSize() != 0 && rFunc.GetInstructionCounter() != 0)
  {
    std::ostringstream oss;
    oss
      << std::hex << std::showbase << std::left
      << "; size=" << rFunc.GetSize()
      << ", insn_cnt=" << rFunc.GetInstructionCounter();

    rPrintData.AppendComment(oss.str());
  }
  else
    rPrintData.AppendComment("; imported");

  FunctionDetail FuncDtl;
  Id CurId;
  if (!rDoc.RetrieveDetailId(rAddr, 0, CurId))
    return true;

  auto spOs = ModuleManager::Instance().GetOperatingSystem(rDoc.GetOperatingSystemName());
  if (spOs == nullptr || spOs->GetFunctionDetail(CurId, FuncDtl) == false)
    if (!rDoc.GetFunctionDetail(CurId, FuncDtl))
      return true;

  rPrintData.AppendNewLine().AppendSpace(2).AppendComment(";").AppendSpace();

  auto const& RetType = FuncDtl.GetReturnType();

  std::string FuncName;
  Label CurLbl = rDoc.GetLabelFromAddress(rAddr);
  if (CurLbl.GetType() == Label::Unknown)
    FuncName = FuncDtl.GetName();
  else
    FuncName = CurLbl.GetName();

  FormatTypeDetail(RetType, rPrintData);
  rPrintData.AppendSpace().AppendLabel(FuncName).AppendOperator("(");

  bool FirstParam = true;
  auto const& Params = FuncDtl.GetParameters();
  for (auto const& Param : Params)
  {
    if (FirstParam)
      FirstParam = false;
    else
      rPrintData.AppendOperator(",").AppendSpace();

    FormatTypeDetail(*Param.GetType().get(), rPrintData);
    rPrintData.AppendSpace().AppendLabel(Param.GetValue().GetName());
  }

  rPrintData.AppendOperator(");");

  return true;
}

bool Architecture::FormatStructure(
    Document      const& rDoc,
    Address       const& rAddr,
    Structure     const& rStruct,
    PrintData          & rPrintData) const
{
  std::ostringstream oss;
  oss << std::hex << std::showbase << std::left;

  oss
    << "; " << "<structure_name>"
    << ": size=" << rStruct.GetSize();

  rPrintData.AppendComment(oss.str());
  return true;
}

MEDUSA_NAMESPACE_END
