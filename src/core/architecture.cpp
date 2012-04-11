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

    if (pOprd->GetType() & O_REL || pOprd->GetType() & O_ABS)
    {
      Address DstAddr;
      if (rInsn.GetOperandReference(rBinStrm, 0, rAddr, DstAddr))
      {
        std::string Label = "";

        Label = rDatabase.GetLabelFromAddress(DstAddr).GetLabel();

        std::string OprdName;
        Cell::Mark::Type MarkType = Cell::Mark::UnknownType;
        if (Label.empty())  { OprdName = DstAddr.ToString(); MarkType = Cell::Mark::ImmediateType; }
        else                { OprdName = Label; MarkType = Cell::Mark::LabelType; }

        pOprd->SetName(OprdName.c_str());
        oss << OprdName;
        rInsn.AddMark(MarkType, OprdName.length());
      }
      else
      {
        oss << pOprd->GetName();
        rInsn.AddMark(Cell::Mark::ImmediateType, oss.str().size());
      }
    }
    else if (pOprd->GetType() & O_DISP || pOprd->GetType() & O_IMM && !(pOprd->GetType() & O_NO_REF))
    {
      Address OprdAddr(Address::UnknownType, pOprd->GetSegValue(), pOprd->GetValue());

      std::string LabelName = rDatabase.GetLabelFromAddress(OprdAddr).GetLabel();

      if (LabelName.empty())
      {
        oss << " " << pOprd->GetName();
        Sep = ',';
        continue;
      }

      if (pOprd->GetType() & O_MEM)
      {
        oss << "[" << LabelName << "]";
        rInsn.AddMark(Cell::Mark::OperatorType, 1);
        rInsn.AddMark(Cell::Mark::LabelType, LabelName.length());
        rInsn.AddMark(Cell::Mark::OperatorType, 1);
      }
      else
      {
        oss << LabelName;
        rInsn.AddMark(Cell::Mark::LabelType, LabelName.length());
      }
    }
    else
    {
      oss << pOprd->GetName();
      rInsn.AddMark(Cell::Mark::UnknownType, pOprd->GetName().length());
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