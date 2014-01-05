#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

bool Architecture::FormatCell(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Cell          const& rCell,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  switch (rCell.GetType())
  {
  case Cell::InstructionType: return FormatInstruction(rDoc, rBinStrm, rAddr, static_cast<Instruction const&>(rCell), rStrCell, rMarks);
  case Cell::ValueType:       return FormatValue      (rDoc, rBinStrm, rAddr, static_cast<Value       const&>(rCell), rStrCell, rMarks);
  case Cell::CharacterType:   return FormatCharacter  (rDoc, rBinStrm, rAddr, static_cast<Character   const&>(rCell), rStrCell, rMarks);
  case Cell::StringType:      return FormatString     (rDoc, rBinStrm, rAddr, static_cast<String      const&>(rCell), rStrCell, rMarks);
  default:                    return false;
  }
}

bool Architecture::FormatInstruction(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Instruction   const& rInsn,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  char Sep = '\0';
  std::ostringstream oss;

  oss << rInsn.GetName() << " ";
  rMarks.push_back(Cell::Mark(Cell::Mark::MnemonicType, oss.str().size()));

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand const* pOprd = rInsn.Operand(i);
    if (pOprd == nullptr)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != '\0')
    {
      oss << Sep << " ";
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 2));
    }

    u32 OprdType = pOprd->GetType();
    std::string OprdName = pOprd->GetName();
    std::string MemBegChar = "[";
    std::string MemEndChar = "]";

    // NOTE: Since we have to mark all characters with good type, we handle O_MEM here.
    if (pOprd->GetType() & O_MEM)
    {
      MemBegChar = *OprdName.begin();
      MemEndChar = *OprdName.rbegin();
      OprdName   =  OprdName.substr(1, OprdName.length() - 2);
    }

    if (OprdType & O_MEM)
    {
      oss << MemBegChar;
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
    }

    do
    {
      if (OprdType & O_REL || OprdType & O_ABS)
      {
        Address DstAddr;

        if (rInsn.GetOperandReference(rDoc, 0, rAddr, DstAddr))
        {
          auto Lbl = rDoc.GetLabelFromAddress(DstAddr);
          OprdName = Lbl.GetLabel();
          Cell::Mark::Type MarkType = Cell::Mark::LabelType;

          if (OprdName.empty())  { OprdName = DstAddr.ToString(); MarkType = Cell::Mark::ImmediateType; }

          oss << OprdName;
          rMarks.push_back(Cell::Mark(MarkType, OprdName.length()));
          //if (rInsn.GetComment().empty())
          //  rInsn.SetComment(Lbl.GetName());
        }
        else
        {
          oss << OprdName;
          rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, OprdName.length()));
        }
      }
      else if (OprdType & O_DISP || OprdType & O_IMM)
      {
        if (pOprd->GetType() & O_NO_REF)
        {
          std::string ValueName = pOprd->GetName();
          oss << ValueName;
          rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, ValueName.length()));
          break;
        }

        Address OprdAddr(Address::UnknownType, pOprd->GetSegValue(), pOprd->GetValue());
        auto Lbl = rDoc.GetLabelFromAddress(OprdAddr);
        std::string LabelName = Lbl.GetLabel();

        if (LabelName.empty())
        {
          std::string ValueName = OprdName;
          oss << ValueName;
          rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, ValueName.length()));
          break;
        }

        oss << LabelName;
        rMarks.push_back(Cell::Mark(Cell::Mark::LabelType, LabelName.length()));
        //if (rInsn.GetComment().empty())
        //  rInsn.SetComment(Lbl.GetName());
      }

      else if (OprdType & O_REG)
      {
        if (OprdName.empty())
        {
          auto pCpuInfo = GetCpuInformation();
          OprdName = pCpuInfo->ConvertIdentifierToName(pOprd->GetReg());
        }
        oss << OprdName;
        rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, OprdName.length()));
      }
    } while (0);

    if (OprdType & O_MEM)
    {
      oss << MemEndChar;
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
    }

    Sep = ',';
  }

  //auto rExpr = rInsn.GetSemantic();
  //if (rExpr.empty() == false)
  //{
  //  auto BegMark = oss.str().length();
  //  oss << " [ ";
  //  auto itExpr = std::begin(rExpr);
  //  oss << (*itExpr)->ToString();
  //  ++itExpr;
  //  std::for_each(itExpr, std::end(rExpr), [&oss](Expression const* pExpr)
  //  {
  //    oss << "; " << pExpr->ToString();
  //  });
  //  oss << " ]";
  //  rMarks.push_back(Cell::Mark::KeywordType, oss.str().length() - BegMark);
  //}

  rStrCell = oss.str();
  return true;
}

bool Architecture::FormatCharacter(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Character     const& rChar,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
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
  rMarks.push_back(Cell::Mark(Cell::Mark::StringType, 1));
  rStrCell = oss.str();
  return true;
}

bool Architecture::FormatValue(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Value         const& rVal,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
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
      if (IsUnk)
        oss << "db (?)";
      else
      {
        u8 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;
        oss << "db " << BasePrefix << std::setw(2) << static_cast<u16>(Data); //  u8 type would be printed as char by ostream
      }
      break;
    }
  case 2:
    {
      if (IsUnk)
        oss << "dw (?)";
      else
      {
        u16 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;
        oss << "dw " << BasePrefix << std::setw(4) << static_cast<u16>(Data);
      }
      break;
    }
  case 4:
    {
      if (IsUnk)
        oss << "dd (?)";
      else
      {
        u32 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;
        oss << "dd " << BasePrefix << std::setw(8) << static_cast<u32>(Data);
      }
      break;
    }
  case 8:
    {
      if (IsUnk)
        oss << "dq (?)";
      else
      {
        u64 Data;
        if (!rCurBinStrm.Read(Off, Data))
          return false;
        oss << "dq " << BasePrefix << std::setw(16) << static_cast<u64>(Data);
      }
      break;
    }
  }

  rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, 3));
  rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, oss.str().length() - 3));
  rStrCell = oss.str();
  return true;
}

bool Architecture::FormatMultiCell(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddress,
  MultiCell     const& rMultiCell,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  switch (rMultiCell.GetType())
  {
  case MultiCell::FunctionType: return FormatFunction(rDoc, rBinStrm, rAddress, static_cast<Function const&>(rMultiCell), rStrMultiCell, rMarks);
  default:                      return false;
  }
}

bool Architecture::FormatString(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  String        const& rStr,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  std::string FmtStr = "";

  if (rStr.GetType() == String::Utf16Type)
    return false;


  TOffset FileOff;

  if (rDoc.ConvertAddressToFileOffset(rAddr, FileOff) == false)
    return false;

  if (rStr.GetLength() <= 1)
    return false;

  size_t StrLen = rStr.GetLength() - 1;

  char* pStrBuf = new char[StrLen];
  if (rDoc.GetBinaryStream().Read(FileOff, pStrBuf, StrLen) == false)
  {
    delete[] pStrBuf;
    return false;
  }

  for (size_t i = 0; i < StrLen; ++i)
  {
    switch (pStrBuf[i])
    {
    case '\a': FmtStr += "\\a";      break;
    case '\b': FmtStr += "\\b";      break;
    case '\t': FmtStr += "\\t";      break;
    case '\n': FmtStr += "\\n";      break;
    case '\v': FmtStr += "\\v";      break;
    case '\f': FmtStr += "\\f";      break;
    case '\r': FmtStr += "\\r";      break;
    default:   FmtStr += pStrBuf[i]; break;
    }
  }
  std::string Str = "";
  if (rStr.GetSubType() == String::Utf16Type)
  {
    Str += "L";
    rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, 1));
  }
  Str += std::string("\"") + FmtStr + std::string("\", 0");
  delete[] pStrBuf;
  rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
  rMarks.push_back(Cell::Mark(Cell::Mark::StringType, FmtStr.length()));
  rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 2));
  rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, 2));

  rStrMultiCell = Str;
  return true;
}

bool Architecture::FormatFunction(
  Document      const& rDoc,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Function      const& rFunc,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  std::ostringstream oss;
  oss << std::hex << std::showbase << std::left;
  auto FuncLabel = rDoc.GetLabelFromAddress(rAddr);
  if (!(FuncLabel.GetType() & Label::Code))
    return false;

  oss
    << "; " << FuncLabel.GetLabel()
    << ": size=" << rFunc.GetSize()
    << ", insn_cnt=" << rFunc.GetInstructionCounter();

  rStrMultiCell = oss.str();
  return true;
}

MEDUSA_NAMESPACE_END
