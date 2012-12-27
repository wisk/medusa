#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

void Architecture::FormatCell(
    Database const& rDatabase,
    BinaryStream const& rBinStrm,
    Address const& rAddr,
    Cell &rCell) const
{
  rCell.ResetMarks();
  switch (rCell.GetType())
  {
  case CellData::InstructionType: FormatInstruction(rDatabase, rBinStrm, rAddr, static_cast<Instruction&>(rCell)); break;
  case CellData::ValueType:       FormatValue      (rDatabase, rBinStrm, rAddr, static_cast<Value&>(rCell));       break;
  case CellData::CharacterType:   FormatCharacter  (rDatabase, rBinStrm, rAddr, static_cast<Character&>(rCell));   break;
  case CellData::StringType:      FormatString     (rDatabase, rBinStrm, rAddr, static_cast<String&>(rCell));      break;
  default:                        rCell.UpdateString      ("unknown_cell");                                        break;
  }
}

void Architecture::FormatInstruction(Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Instruction        & rInsn) const
{
  char Sep = '\0';
  std::ostringstream oss;

  rInsn.ResetMarks();

  oss << rInsn.GetName() << " ";
  rInsn.AddMark(Cell::Mark::MnemonicType, oss.str().size());

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand* pOprd = rInsn.Operand(i);
    if (pOprd == NULL)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != '\0')
    {
      oss << Sep << " ";
      rInsn.AddMark(Cell::Mark::OperatorType, 2);
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
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
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
          rInsn.AddMark(MarkType, OprdName.length());
          if (rInsn.GetComment().empty())
            rInsn.SetComment(Lbl.GetName());
        }
        else
        {
          oss << OprdName;
          rInsn.AddMark(Cell::Mark::ImmediateType, OprdName.length());
        }
      }
      else if (OprdType & O_DISP || OprdType & O_IMM)
      {
        if (pOprd->GetType() & O_NO_REF)
        {
          std::string ValueName = pOprd->GetName();
          oss << ValueName;
          rInsn.AddMark(Cell::Mark::ImmediateType, ValueName.length());
          break;
        }

        Address OprdAddr(Address::UnknownType, pOprd->GetSegValue(), pOprd->GetValue());
        Label Lbl = rDatabase.GetLabelFromAddress(OprdAddr);
        std::string LabelName = Lbl.GetLabel();

        if (LabelName.empty())
        {
          std::string ValueName = OprdName;
          oss << ValueName;
          rInsn.AddMark(Cell::Mark::ImmediateType, ValueName.length());
          break;
        }

        oss << LabelName;
        rInsn.AddMark(Cell::Mark::LabelType, LabelName.length());
        if (rInsn.GetComment().empty())
          rInsn.SetComment(Lbl.GetName());
      }

      else if (OprdType & O_REG)
      {
        oss << OprdName;
        rInsn.AddMark(Cell::Mark::RegisterType, OprdName.length());
      }
    } while (0);

    if (OprdType & O_MEM)
    {
      oss << MemEndChar;
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
    }

    Sep = ',';
  }
  auto rExpr = rInsn.GetSemantic();
  if (rExpr.empty() == false)
  {
    auto BegMark = oss.str().length();
    oss << " [ ";
    auto itExpr = std::begin(rExpr);
    oss << (*itExpr)->ToString();
    ++itExpr;
    std::for_each(itExpr, std::end(rExpr), [&oss](Expression const* pExpr)
    {
      oss << "; " << pExpr->ToString();
    });
    oss << " ]";
    rInsn.AddMark(Cell::Mark::KeywordType, oss.str().length() - BegMark);
  }

  rInsn.UpdateString(oss.str());
}

void Architecture::FormatCharacter(
    Database      const& rDatabase,
    BinaryStream  const& rBinStrm,
    Address       const& rAddr,
    Character          & rChar) const
{
  std::ostringstream oss;
  TOffset Off;

  rChar.ResetMarks();

  if (!rDatabase.Convert(rAddr, Off)) return;

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
  rChar.UpdateString(oss.str());
  rChar.AddMark(Cell::Mark::StringType, 1);
}

void Architecture::FormatValue(
    Database      const& rDatabase,
    BinaryStream  const& rBinStrm,
    Address       const& rAddr,
    Value              & rVal) const
{
    std::ostringstream  oss;
    TOffset             Off;
    u32                 ValueType   = rVal.GetValueType();
    std::string         BasePrefix  = "";

    rVal.ResetMarks();

    auto pCurMemArea = rDatabase.GetMemoryArea(rAddr);
    if (pCurMemArea == nullptr) return;
    auto rCurBinStrm = pCurMemArea->GetBinaryStream();

    if (!rDatabase.Convert(rAddr, Off)) return;

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
      rVal.UpdateString("(unable to read value)");
      return;
    }

    rVal.UpdateString(oss.str());
    rVal.AddMark(Cell::Mark::KeywordType, 3);
    rVal.AddMark(Cell::Mark::ImmediateType, oss.str().length() - 3);
}

void Architecture::FormatMultiCell(
  Database     const& rDatabase,
  BinaryStream const& rBinStrm,
  Address      const& rAddress,
  MultiCell         & rMultiCell) const
{
  switch (rMultiCell.GetType())
  {
  case MultiCell::FunctionType: FormatFunction(rDatabase, rBinStrm, rAddress, static_cast<Function&>(rMultiCell)); break;
  default:                      rMultiCell.UpdateString("unknown multicell");                                      break;
  }
}

void Architecture::FormatString(
  Database     const& rDatabase,
  BinaryStream const& rBinStrm,
  Address      const& rAddr,
  String            & rStr) const
{
  rStr.ResetMarks();

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

  rStr.UpdateString(std::string("\"") + FmtStr + std::string("\", 0"));
  rStr.AddMark(Cell::Mark::OperatorType, 1);
  rStr.AddMark(Cell::Mark::StringType, FmtStr.length());
  rStr.AddMark(Cell::Mark::OperatorType, 2);
  rStr.AddMark(Cell::Mark::ImmediateType, 2);
}

void Architecture::FormatFunction(
  Database     const& rDatabase,
  BinaryStream const& rBinStrm,
  Address      const& rAddr,
  Function          & rFunc) const
{
  std::ostringstream oss;
  oss << std::hex << std::showbase << std::left;
  Label FuncLabel = rDatabase.GetLabelFromAddress(rAddr);
  if (!(FuncLabel.GetType() & Label::LabelCode)) return;

  oss
    << "; " << FuncLabel.GetLabel()
    << ": size=" << rFunc.GetSize()
    << ", insn_cnt=" << rFunc.GetInstructionCounter();

  rFunc.UpdateString(oss.str());
}

MEDUSA_NAMESPACE_END