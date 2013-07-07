#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

bool Architecture::FormatCell(
  Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Cell          const& rCell,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  switch (rCell.GetType())
  {
  case CellData::InstructionType: return FormatInstruction(rDatabase, rBinStrm, rAddr, static_cast<Instruction const&>(rCell), rStrCell, rMarks);
  case CellData::ValueType:       return FormatValue      (rDatabase, rBinStrm, rAddr, static_cast<Value       const&>(rCell), rStrCell, rMarks);
  case CellData::CharacterType:   return FormatCharacter  (rDatabase, rBinStrm, rAddr, static_cast<Character   const&>(rCell), rStrCell, rMarks);
  case CellData::StringType:      return FormatString     (rDatabase, rBinStrm, rAddr, static_cast<String      const&>(rCell), rStrCell, rMarks);
  default:                        return false;
  }
}

bool Architecture::FormatInstruction(
  Database      const& rDatabase,
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
    if (pOprd == NULL)
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

        if (rInsn.GetOperandReference(rDatabase, 0, rAddr, DstAddr))
        {
          Label Lbl = rDatabase.GetLabelFromAddress(DstAddr);
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
        Label Lbl = rDatabase.GetLabelFromAddress(OprdAddr);
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
  Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Character     const& rChar,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  std::ostringstream oss;
  TOffset Off;

  if (!rDatabase.Convert(rAddr, Off))
    return false;

  switch (rChar.GetCharacterType())
  {
  case Character::AsciiCharacterType: default:
    {
      s8 Char;
      rBinStrm.Read(Off, Char);
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
  Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Value         const& rVal,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
    std::ostringstream  oss;
    TOffset             Off;
    u32                 ValueType   = rVal.GetValueType();
    std::string         BasePrefix  = "";

    auto pCurMemArea = rDatabase.GetMemoryArea(rAddr);
    if (pCurMemArea == nullptr)
      return false;
    auto rCurBinStrm = pCurMemArea->GetBinaryStream();

    if (!rDatabase.Convert(rAddr, Off))
      return false;

    oss << std::setfill('0');

    // XXX: We use our custom octal prefix for esthetic reason.
    switch (ValueType & VT_MASK)
    {
    case VT_BIN:                           BasePrefix = "0b"; break; // TODO: Unimplemented
    case VT_OCT:          oss << std::oct; BasePrefix = "0o"; break;
    case VT_DEC:          oss << std::dec; BasePrefix = "0n"; break;
    case VT_HEX: default: oss << std::hex; BasePrefix = "0x"; break;
    }

    try
    {
      switch (ValueType & VS_MASK)
      {
      case VS_8BIT: default:
        {
          u8 Data;
          rCurBinStrm.Read(Off, Data);
          if ((ValueType & VT_MASK) != VT_UNK)
            oss << "db " << BasePrefix << std::setw(2) << static_cast<u16>(Data);
          else
            oss << "db (?)";
          break;
        }
      case VS_16BIT:
        {
          u16 Data;
          rCurBinStrm.Read(Off, Data);
          if ((ValueType & VT_MASK) != VT_UNK)
            oss << "dw " << BasePrefix << std::setw(4) << Data;
          else
            oss << "dw (?)";
          break;
        }
      case VS_32BIT:
        {
          u32 Data;
          rCurBinStrm.Read(Off, Data);
          if ((ValueType & VT_MASK) != VT_UNK)
            oss << "dd " << BasePrefix << std::setw(8) << Data;
          else
            oss << "dd (?)";
          break;
        }
      case VS_64BIT:
        {
          u64 Data;
          rCurBinStrm.Read(Off, Data);
          if ((ValueType & VT_MASK) != VT_UNK)
            oss << "dq " << BasePrefix << std::setw(16) << Data;
          else
            oss << "dq (?)";
          break;
        }
      }
    }
    catch (Exception&)
    {
      return "(unable to read value)";
    }

    rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, 3));
    rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, oss.str().length() - 3));
    rStrCell = oss.str();
    return true;
}

bool Architecture::FormatMultiCell(
  Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddress,
  MultiCell     const& rMultiCell,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  switch (rMultiCell.GetType())
  {
  case MultiCell::FunctionType: return FormatFunction(rDatabase, rBinStrm, rAddress, static_cast<Function const&>(rMultiCell), rStrMultiCell, rMarks);
  default:                      return false;
  }
}

bool Architecture::FormatString(
  Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  String        const& rStr,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  auto Characters = rStr.GetCharacters();
  std::string FmtStr = "";

  for (auto itChar = std::begin(Characters); itChar != std::end(Characters); ++itChar)
  {
    switch (*itChar)
    {
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
  std::string Str = "";
  if (rStr.GetStringType() == String::Utf16Type)
  {
    Str += "L";
    rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, 1));
  }
  Str += std::string("\"") + FmtStr + std::string("\", 0");
  rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
  rMarks.push_back(Cell::Mark(Cell::Mark::StringType, FmtStr.length()));
  rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 2));
  rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, 2));

  rStrMultiCell = Str;
  return true;
}

bool Architecture::FormatFunction(
  Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Function      const& rFunc,
  std::string        & rStrMultiCell,
  Cell::Mark::List   & rMarks) const
{
  std::ostringstream oss;
  oss << std::hex << std::showbase << std::left;
  Label FuncLabel = rDatabase.GetLabelFromAddress(rAddr);
  if (!(FuncLabel.GetType() & Label::LabelCode))
    return false;

  oss
    << "; " << FuncLabel.GetLabel()
    << ": size=" << rFunc.GetSize()
    << ", insn_cnt=" << rFunc.GetInstructionCounter();

  rStrMultiCell = oss.str();
  return true;
}

MEDUSA_NAMESPACE_END