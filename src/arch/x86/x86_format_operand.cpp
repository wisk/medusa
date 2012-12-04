#include "x86.hpp"
#include "x86_architecture.hpp"

void X86Architecture::FormatOperand(std::ostringstream &rInsnBuf, Database const& rDb, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  std::ostringstream ValueName;
  ValueName << std::setfill('0') << std::right << std::hex;
  s64 RelValue = static_cast<s64>(pOprd->GetValue());

  if (pOprd->GetType() & O_REG_PC_REL)
  {
    Label OprdLabel = rDb.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), rInsn.GetLength() + pOprd->GetValue() + Offset));
    if (OprdLabel.GetType() != Label::LabelUnknown)
    {
      ValueName << "[" << OprdLabel.GetLabel() << "]";
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
      rInsn.AddMark(Cell::Mark::LabelType, OprdLabel.GetLabel().length());
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
      rInsnBuf << ValueName.str();
      if (rInsn.GetComment().empty())
        rInsn.SetComment(OprdLabel.GetName());
      return;
    }
  }

  if (pOprd->GetType() & O_IMM)
  {
    Label OprdLabel = rDb.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), pOprd->GetValue()));

    if (OprdLabel.GetType() != Label::LabelUnknown)
    {
      ValueName << OprdLabel.GetLabel();
      rInsn.AddMark(Cell::Mark::LabelType, ValueName.str().length());
      if (rInsn.GetComment().empty())
        rInsn.SetComment(OprdLabel.GetName());
    }
    else
    {
      switch (pOprd->GetType() & DS_MASK)
      {
      case DS_8BIT:  ValueName << "0x" << std::setw(2)  << static_cast<u32>(static_cast<u8> (pOprd->GetValue())); break;
      case DS_16BIT: ValueName << "0x" << std::setw(4)  << static_cast<s16>(pOprd->GetValue()); break;
      case DS_32BIT: ValueName << "0x" << std::setw(8)  << static_cast<s32>(pOprd->GetValue()); break;
      case DS_64BIT: ValueName << "0x" << std::setw(16) << static_cast<s64>(pOprd->GetValue()); break;
      default:       ValueName << "0x" <<                                   pOprd->GetValue() ; break;
      }
      rInsn.AddMark(Cell::Mark::ImmediateType, ValueName.str().length());
    }

    rInsnBuf << ValueName.str();
    return;
  }

  if (pOprd->GetType() & O_REL)
  {
    TOffset OprdOff = Offset + rInsn.GetLength();
    switch (pOprd->GetType() & DS_MASK)
    {
    case DS_8BIT:  OprdOff += static_cast<s8> (pOprd->GetValue()); break;
    case DS_16BIT: OprdOff += static_cast<s16>(pOprd->GetValue()); break;
    case DS_32BIT: OprdOff += static_cast<s32>(pOprd->GetValue()); break;
    case DS_64BIT: OprdOff += static_cast<s64>(pOprd->GetValue()); break;
    default:       OprdOff += pOprd->GetValue();                   break;
    }
    Label OprdLabel = rDb.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), OprdOff));
    if (OprdLabel.GetType() != Label::LabelUnknown)
    {
      ValueName << OprdLabel.GetLabel();
      rInsn.AddMark(Cell::Mark::LabelType, ValueName.str().length());
      if (rInsn.GetComment().empty())
        rInsn.SetComment(OprdLabel.GetName());
    }
    else
    {
      switch (pOprd->GetType() & DS_MASK)
      {
      case DS_8BIT:  ValueName << "0x" << std::setw(2)  << static_cast<u32>(static_cast< u8>(OprdOff)); break;
      case DS_16BIT: ValueName << "0x" << std::setw(4)  << static_cast<s16>(OprdOff); break;
      case DS_32BIT: ValueName << "0x" << std::setw(8)  << static_cast<s32>(OprdOff); break;
      case DS_64BIT: ValueName << "0x" << std::setw(16) << static_cast<s64>(OprdOff); break;
      default:       ValueName << "0x" <<                                   OprdOff ; break;
      }
      rInsn.AddMark(Cell::Mark::ImmediateType, ValueName.str().length());
    }

    rInsnBuf << ValueName.str();
    return;
  }

  if (pOprd->GetType() & O_MEM)
  {
    std::string AccessType = "";
    switch (pOprd->GetType() & MS_MASK)
    {
    case MS_8BIT:   AccessType = "byte ";  break;
    case MS_16BIT:  AccessType = "word ";  break;
    case MS_32BIT:  AccessType = "dword "; break;
    case MS_64BIT:  AccessType = "qword "; break;
    case MS_80BIT:  AccessType = "tword "; break;
    case MS_128BIT: AccessType = "oword "; break;
    }
    rInsnBuf << AccessType;
    rInsn.AddMark(Cell::Mark::KeywordType, AccessType.length());

    if (pOprd->GetType() & O_SEG)
    {
      rInsnBuf << X86_RegName[pOprd->GetSeg()] << ":";
      rInsn.AddMark(Cell::Mark::RegisterType, strlen(X86_RegName[pOprd->GetSeg()]));
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
    }

    if (pOprd->GetType() & O_SEG_VAL)
    {
      rInsnBuf << pOprd->GetSeg() << ":";
      rInsn.AddMark(Cell::Mark::ImmediateType, 4);
      rInsn.AddMark(Cell::Mark::OperatorType, 1);
    }

    rInsnBuf << "[";
    rInsn.AddMark(Cell::Mark::OperatorType, 1);

    if (pOprd->GetType() & O_REG && pOprd->GetReg() != X86_Reg_Unknown)
    {
      rInsnBuf << X86_RegName[pOprd->GetReg()];
      rInsn.AddMark(Cell::Mark::RegisterType, strlen(X86_RegName[pOprd->GetReg()]));
    }

    if (pOprd->GetType() & O_SREG && pOprd->GetSecReg() != X86_Reg_Unknown)
    {
      if (pOprd->GetReg() != X86_Reg_Unknown)
      {
        rInsnBuf << " + ";
        rInsn.AddMark(Cell::Mark::OperatorType, 3);
      }

      rInsnBuf << X86_RegName[pOprd->GetSecReg()];
      rInsn.AddMark(Cell::Mark::RegisterType, strlen(X86_RegName[pOprd->GetSecReg()]));
    }

    if (pOprd->GetType() & O_SCALE && pOprd->GetSecReg() != X86_Reg_Unknown)
    {
      char const* pScaleValue = "1";
      switch (pOprd->GetType() & SC_MASK)
      {
      case SC_2: pScaleValue = "2"; break;
      case SC_4: pScaleValue = "4"; break;
      case SC_8: pScaleValue = "8"; break;
      }
      rInsnBuf << " * " << pScaleValue;
      rInsn.AddMark(Cell::Mark::OperatorType, 3);
      rInsn.AddMark(Cell::Mark::ImmediateType, 1);
    }

    if (pOprd->GetType() & O_DISP)
    {
      Address AddrDst(pOprd->GetSegValue(), pOprd->GetValue());
      Cell::Mark::Type MarkType = Cell::Mark::UnknownType;

      Label const& Lbl = rDb.GetLabelFromAddress(AddrDst);
      if (Lbl.GetType() != Label::LabelUnknown)
      {
        ValueName << Lbl.GetLabel();
        MarkType = Cell::Mark::LabelType;

        if (rInsn.GetComment().empty())
          rInsn.SetComment(Lbl.GetName());
      }
      else
      {
        MarkType = Cell::Mark::ImmediateType;
        switch (pOprd->GetType() & DS_MASK)
        {
        case DS_8BIT:  ValueName << "0x" << std::setw(2)  << static_cast<u32>(static_cast< u8>(pOprd->GetValue())); break;
        case DS_16BIT: ValueName << "0x" << std::setw(4)  << static_cast<s16>(pOprd->GetValue()); break;
        case DS_32BIT: ValueName << "0x" << std::setw(8)  << static_cast<s32>(pOprd->GetValue()); break;
        case DS_64BIT: ValueName << "0x" << std::setw(16) << static_cast<s64>(pOprd->GetValue()); break;
        default:       ValueName << "0x" <<                                   pOprd->GetValue() ; break;
        }
      }

      if (pOprd->GetReg() == 0x0 && pOprd->GetSecReg() == 0x0)
      {
        rInsnBuf << ValueName.str();
        rInsn.AddMark(MarkType, ValueName.str().length());
      }
      else
      {
        rInsnBuf << " + " << ValueName.str();
        rInsn.AddMark(Cell::Mark::OperatorType, 3);
        rInsn.AddMark(MarkType, ValueName.str().length());
      }
    }

    rInsnBuf << "]";
    rInsn.AddMark(Cell::Mark::OperatorType, 1);
    return;
  }

  if (pOprd->GetType() & O_REG)
  {
    rInsnBuf << X86_RegName[pOprd->GetReg()];
    rInsn.AddMark(Cell::Mark::RegisterType, strlen(X86_RegName[pOprd->GetReg()]));
  }
}