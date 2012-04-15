#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

void Architecture::FormatCell(
    Database const& rDatabase,
    BinaryStream const& rBinStrm,
    Address const& rAddr,
    Cell &rCell)
{
  switch (rCell.GetType())
  {
  case Cell::InstructionType: DefaultFormatInstruction(rDatabase, rBinStrm, rAddr, static_cast<Instruction&>(rCell)); break;
  case Cell::ValueType:       DefaultFormatValue      (rDatabase, rBinStrm, rAddr, static_cast<Value&>(rCell));       break;
  case Cell::CharacterType:   DefaultFormatCharacter  (rDatabase, rBinStrm, rAddr, static_cast<Character&>(rCell));   break;
  case Cell::StringType:      DefaultFormatString     (rDatabase, rBinStrm, rAddr, static_cast<String&>(rCell));      break;
  default:                    rCell.UpdateString      ("unknown_cell");                                               break;
  }
}

void Architecture::DefaultFormatInstruction(Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Instruction        & rInsn)
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
          OprdName = rDatabase.GetLabelFromAddress(DstAddr).GetLabel();
          Cell::Mark::Type MarkType = Cell::Mark::LabelType;

          if (OprdName.empty())  { OprdName = DstAddr.ToString(); MarkType = Cell::Mark::ImmediateType; }

          oss << OprdName;
          rInsn.AddMark(MarkType, OprdName.length());
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
        std::string LabelName = rDatabase.GetLabelFromAddress(OprdAddr).GetLabel();

        if (LabelName.empty())
        {
          std::string ValueName = OprdName;
          oss << ValueName;
          rInsn.AddMark(Cell::Mark::ImmediateType, ValueName.length());
          break;
        }

        oss << LabelName;
        rInsn.AddMark(Cell::Mark::LabelType, LabelName.length());
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
  rInsn.UpdateString(oss.str());
}

void Architecture::DefaultFormatCharacter(
    Database      const& rDatabase,
    BinaryStream  const& rBinStrm,
    Address       const& rAddr,
    Character          & rChar)
{
  std::ostringstream oss;
  TOffset Off;

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

void Architecture::DefaultFormatValue(
    Database      const& rDatabase,
    BinaryStream  const& rBinStrm,
    Address       const& rAddr,
    Value              & rVal)
{
    std::ostringstream  oss;
    TOffset             Off;
    u32                 ValueType   = rVal.GetValueType();
    std::string         BasePrefix  = "";

    if (!rDatabase.Convert(rAddr, Off)) return;

    oss << std::setfill('0');

    // XXX: We use our custom octal prefix for esthetic reason.
    switch (ValueType & VT_MASK)
    {
    case VT_BIN:                           BasePrefix = "0b"; break; // TODO: Unimplemented
    case VT_OCT:          oss << std::oct; BasePrefix = "0o"; break;
    case VT_DEC:                                              break;
    case VT_HEX: default: oss << std::hex; BasePrefix = "0x"; break;
    }

    try
    {
      switch (ValueType & VS_MASK)
      {
      case VS_8BIT: default:
        {
          u8 Data;
          rBinStrm.Read(Off, Data);
          oss << "db " << BasePrefix << std::setw(2) << static_cast<u16>(Data);
          break;
        }
      case VS_16BIT:
        {
          u16 Data;
          rBinStrm.Read(Off, Data);
          oss << "dw " << BasePrefix << std::setw(4) << Data;
          break;
        }
      case VS_32BIT:
        {
          u32 Data;
          rBinStrm.Read(Off, Data);
          oss << "dd " << BasePrefix << std::setw(8) << Data;
          break;
        }
      case VS_64BIT:
        {
          u64 Data;
          rBinStrm.Read(Off, Data);
          oss << "dq " << BasePrefix << std::setw(16) << Data;
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
  MultiCell         & rMultiCell)
{
  switch (rMultiCell.GetType())
  {
  case MultiCell::FunctionType: DefaultFormatFunction(rDatabase, rBinStrm, rAddress, static_cast<Function&>(rMultiCell)); break;
  default:                      rMultiCell.UpdateString("unknown multicell");                                             break;
  }
}

void Architecture::DefaultFormatString(
  Database     const& rDatabase,
  BinaryStream const& rBinStrm,
  Address      const& rAddr,
  String            & rStr)
{
  rStr.UpdateString(std::string("\"") + rStr.GetCharacters() + std::string("\", 0"));
  rStr.AddMark(Cell::Mark::OperatorType, 1);
  rStr.AddMark(Cell::Mark::StringType, rStr.GetCharacters().length());
  rStr.AddMark(Cell::Mark::OperatorType, 1);
}

void Architecture::DefaultFormatFunction(
  Database     const& rDatabase,
  BinaryStream const& rBinStrm,
  Address      const& rAddr,
  Function          & rFunc)
{
  std::ostringstream oss;
  oss << std::hex << std::showbase << std::left;
  Label FuncLabel = rDatabase.GetLabelFromAddress(rAddr);
  if (!(FuncLabel.GetType() & Label::LabelCode)) return;

  oss
    << "; " << FuncLabel.GetLabel()
    << ": size=" << rFunc.GetSize()
    << ", insn_cnt=" << rFunc.GetInstructionCounter()
    << "\n";

  rFunc.UpdateString(oss.str());
}

MEDUSA_NAMESPACE_END