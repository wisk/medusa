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
  char Sep = '\0';
  std::ostringstream oss;

  rPrintData.AppendMnemonic(rInsn.GetName());

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand const* pOprd = rInsn.Operand(i);
    if (pOprd == nullptr)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != '\0')
      rPrintData.AppendOperator(",").AppendSpace();

    if (!FormatOperand(rDoc, rAddr, rInsn, *pOprd, i, rPrintData))
      return false;
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
  auto const& rBinStrm = rDoc.GetBinaryStream();
  return false;
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
  std::ostringstream  oss;
  TOffset             Off;
  u8                  ValueType   = rVal.GetSubType();
  std::string         BasePrefix  = "";
  bool                IsUnk = false;

  auto const& rCurBinStrm = rDoc.GetBinaryStream();

  if (!rDoc.ConvertAddressToFileOffset(rAddr, Off))
    IsUnk = true;

  oss << std::setfill('0');

  switch (ValueType)
  {
  case Value::BinaryType:                                BasePrefix = "0b"; break; // TODO: Unimplemented
  case Value::DecimalType:              oss << std::dec; BasePrefix = "0n"; break;
  case Value::HexadecimalType: default: oss << std::hex; BasePrefix = "0x"; break;
  }

  switch (rVal.GetLength())
  {
  case 1: default:
    {
      rPrintData.AppendKeyword("db").AppendSpace();
      if (IsUnk)
        rPrintData.AppendOperator("(?)");
      else
      {
        u8 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;
        oss << BasePrefix << std::setw(2) << static_cast<u16>(Data); //  u8 type would be printed as char by ostream
        rPrintData.AppendImmediate(oss.str());
      }
      break;
    }
  case 2:
    {
      rPrintData.AppendKeyword("dw").AppendSpace();
      if (IsUnk)
        rPrintData.AppendOperator("(?)");
      else
      {
        u16 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;

        Label Lbl = rDoc.GetLabelFromAddress(Data);
        if (Lbl.GetType() == Label::Unknown)
        {
          oss << BasePrefix << std::setw(4) << static_cast<u16>(Data);
          rPrintData.AppendImmediate(oss.str());
        }

        else
          rPrintData.AppendLabel(Lbl.GetLabel());
      }
      break;
    }
  case 4:
    {
      rPrintData.AppendKeyword("dd").AppendSpace();
      if (IsUnk)
        rPrintData.AppendOperator("(?)");
      else
      {
        u32 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;

        Label Lbl = rDoc.GetLabelFromAddress(Data);
        if (Lbl.GetType() == Label::Unknown)
        {
          oss << BasePrefix << std::setw(8) << static_cast<u32>(Data);
          rPrintData.AppendImmediate(oss.str());
        }

        else
          rPrintData.AppendLabel(Lbl.GetLabel());
      }
      break;
    }
  case 8:
    {
      rPrintData.AppendKeyword("dq").AppendSpace();
      if (IsUnk)
        rPrintData.AppendOperator("(?)");
      else
      {
        u64 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;

        Label Lbl = rDoc.GetLabelFromAddress(Data);
        if (Lbl.GetType() == Label::Unknown)
        {
          oss << BasePrefix << std::setw(16) << static_cast<u64>(Data);
          rPrintData.AppendImmediate(oss.str());
        }

        else
          rPrintData.AppendLabel(Lbl.GetLabel());
      }
      break;
    }
  }

  return true;
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
  std::ostringstream oss;
  oss << std::hex << std::showbase << std::left;
  auto FuncLabel = rDoc.GetLabelFromAddress(rAddr);

  oss
    << "; " << FuncLabel.GetLabel()
    << ": size=" << rFunc.GetSize()
    << ", insn_cnt=" << rFunc.GetInstructionCounter();

  rPrintData.AppendComment(oss.str());
  return true;
}

MEDUSA_NAMESPACE_END
