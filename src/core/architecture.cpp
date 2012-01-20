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
  case Cell::InstructionType: DefaultFormatInstruction(rDatabase, rAddr, static_cast<Instruction&>(rCell));       break;
  case Cell::CharacterType:   DefaultFormatCharacter(rDatabase, rBinStrm, rAddr, static_cast<Character&>(rCell)); break;
  case Cell::ValueType:       DefaultFormatValue(rDatabase, rBinStrm, rAddr, static_cast<Value&>(rCell));         break;
  default:                    rCell.UpdateString("unknown_cell");                                                 break;
  }
}

void Architecture::DefaultFormatInstruction(Database      const& rDatabase,
                                            Address       const& rAddr,
                                            Instruction        & rInsn)
{
  char Sep = '\0';
  std::ostringstream oss;

  oss << rInsn.GetName() << " ";

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand* pOprd = rInsn.Operand(i);
    if (pOprd == NULL)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != '\0')
      oss << Sep << " ";

    if (pOprd->GetType() & O_REL || pOprd->GetType() & O_ABS)
    {
      Address DstAddr;
      if (rInsn.GetOperandReference(0, rAddr, DstAddr))
      {
        std::string Label = "";

        //if (pCell != NULL) Label = pCell->GetLabel();
        Label = rDatabase.GetLabelFromAddress(DstAddr).GetName();

        std::string OprdName;
        if (Label.empty())  OprdName = DstAddr.ToString();
        else                OprdName = Label;

        pOprd->SetName(OprdName.c_str());
        oss << OprdName;
      }
      else
        oss << pOprd->GetName();
    }
    else if (pOprd->GetType() & O_DISP || pOprd->GetType() & O_IMM && !(pOprd->GetType() & O_NO_REF))
    {
      Address OprdAddr(Address::UnknownType, pOprd->GetSegValue(), pOprd->GetValue());

      std::string MultiCellName = rDatabase.GetLabelFromAddress(OprdAddr).GetName();

      if (MultiCellName.empty())
      {
        oss << " " << pOprd->GetName();
        Sep = ',';
        continue;
      }

      rInsn.SetComment(MultiCellName);
      oss << pOprd->GetName();
    }
    else
      oss << pOprd->GetName();

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
      if (Char == '\0')
        oss << "'\\0'";
      else
        oss << "'" << Char << "'";
      break;
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

    rVal.UpdateString(oss.str());
}

MEDUSA_NAMESPACE_END
