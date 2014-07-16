#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

  Architecture::Architecture(Tag ArchTag) : m_Tag(ArchTag)
{
  m_CfgMdl.InsertBoolean("Disassembly only basic block", false);
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
      case ValueDetail::NegateType: rPrintData.AppendOperator("-"); Value = -Value; break;
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

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand const* pOprd = rInsn.Operand(i);
    if (pOprd == nullptr)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != nullptr)
      rPrintData.AppendOperator(Sep).AppendSpace();
    else
      Sep = ",";

    if (!FormatOperand(rDoc, rAddr, rInsn, *pOprd, i, rPrintData))
      return false;

    Address OpRefAddr;
    if (OpRefCmt.empty() && rInsn.GetOperandReference(rDoc, i, rAddr, OpRefAddr))
    {
      Id OpId;
      if (rDoc.RetrieveDetailId(OpRefAddr, 0, OpId))
      {
        FunctionDetail FuncDtl;
        if (rDoc.GetFunctionDetail(OpId, FuncDtl))
        {
          // TODO: provide helper to avoid this...
          u16 CmtOff = static_cast<u16>(rPrintData.GetCurrentText().length()) - 6 - 1 - rAddr.ToString().length();

          rPrintData.AppendSpace().AppendComment(";").AppendSpace();
          FormatTypeDetail(FuncDtl.GetReturnType(), rPrintData);
          rPrintData.AppendSpace().AppendLabel(FuncDtl.GetName()).AppendOperator("(");

          if (!FuncDtl.GetParameters().empty())
            rPrintData.AppendNewLine().AppendSpace(CmtOff).AppendComment(";").AppendSpace(3);

          bool FirstParam = true;
          for (auto const& rParam : FuncDtl.GetParameters())
          {
            if (FirstParam)
              FirstParam = false;
            else
              rPrintData.AppendOperator(",").AppendNewLine().AppendSpace(CmtOff).AppendComment(";").AppendSpace(3);
            FormatTypeDetail(rParam.GetType(), rPrintData);
            rPrintData.AppendSpace().AppendLabel(rParam.GetValue().GetName());
          }
          rPrintData.AppendOperator(");");
        }
      }
    }
  }

  return true;
}

bool Architecture::FormatOperand(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  Operand       const& rOprd,
  u8                   OperandNo,
  PrintData          & rPrintData) const
{
  rPrintData.MarkOffset();

  auto const& rBinStrm = rDoc.GetBinaryStream();

  if (rOprd.GetType() == O_NONE)
    return true;

  u32 OprdType = rOprd.GetType();
  auto const* pCpuInfo = GetCpuInformation();
  std::string MemBegChar = "[";
  std::string MemEndChar = "]";

  if (OprdType & O_MEM)
    rPrintData.AppendOperator("[");

  if (OprdType & O_REL || OprdType & O_ABS)
  {
    Address DstAddr;

    if (rInsn.GetOperandReference(rDoc, 0, rAddr, DstAddr))
    {
      auto Lbl = rDoc.GetLabelFromAddress(DstAddr);
      if (Lbl.GetType() != Label::Unknown)
        rPrintData.AppendLabel(Lbl.GetLabel());
      else
        rPrintData.AppendAddress(rAddr);
    }
    else
      rPrintData.AppendImmediate(rOprd.GetValue(), rAddr.GetOffsetSize());
  }
  else if (OprdType & O_DISP || OprdType & O_IMM)
  {
    if (rOprd.GetType() & O_NO_REF)
    {
      rPrintData.AppendImmediate(rOprd.GetValue(), rAddr.GetOffsetSize());
      return true;
    }

    Id BindId;
    ValueDetail ValDtl;
    if (rDoc.RetrieveDetailId(rAddr, OperandNo, BindId) && rDoc.GetValueDetail(BindId, ValDtl))
    {
      FormatValueDetail(rDoc, rAddr, rOprd.GetSizeInBit(), ValDtl, rPrintData);
    }
    else
    {
      Address OprdAddr = rDoc.MakeAddress(rOprd.GetSegValue(), rOprd.GetValue());
      auto Lbl = rDoc.GetLabelFromAddress(OprdAddr);
      if (Lbl.GetType() != Label::Unknown)
        rPrintData.AppendLabel(Lbl.GetLabel());
      else
        rPrintData.AppendAddress(OprdAddr);
    }
  }

  else if (OprdType & O_REG)
  {
    if (pCpuInfo == nullptr)
      return false;
    auto pRegName = pCpuInfo->ConvertIdentifierToName(rOprd.GetReg());
    if (pRegName == nullptr)
      return false;
    rPrintData.AppendRegister(pRegName);
  }

  if (OprdType & O_MEM)
    rPrintData.AppendOperator("]");

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
  ValueDetail ValDtl("", Id(), static_cast<ValueDetail::Type>(rVal.GetSubType()), Id());

  if (rDoc.RetrieveDetailId(rAddr, 0, BindId))
    rDoc.GetValueDetail(BindId, ValDtl);

  return FormatValueDetail(rDoc, rAddr, rVal.GetLength() * 8, ValDtl, rPrintData);
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

  Id CurId;
  if (!rDoc.RetrieveDetailId(rAddr, 0, CurId))
    return true;

  FunctionDetail FuncDtl;
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

    FormatTypeDetail(Param.GetType(), rPrintData);
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
