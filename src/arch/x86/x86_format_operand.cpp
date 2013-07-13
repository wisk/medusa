#include "x86.hpp"
#include "x86_architecture.hpp"
#include <medusa/extend.hpp>

void X86Architecture::FormatOperand(
  std::ostringstream& rInsnBuf,
  Cell::Mark::List  & rMarks,
  Document     const& rDoc,
  TOffset             Offset,
  Instruction  const& rInsn,
  Operand      const* pOprd) const
{
  std::ostringstream ValueName;
  ValueName << std::setfill('0') << std::right << std::hex;
  s64 RelValue = static_cast<s64>(pOprd->GetValue());

  if (pOprd->GetType() & O_REG_PC_REL)
  {
    Label OprdLabel = rDoc.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), rInsn.GetLength() + pOprd->GetValue() + Offset));
    if (OprdLabel.GetType() != Label::LabelUnknown)
    {
      ValueName << "[" << OprdLabel.GetLabel() << "]";
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
      rMarks.push_back(Cell::Mark(Cell::Mark::LabelType, OprdLabel.GetLabel().length()));
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
      rInsnBuf << ValueName.str();
      //if (rInsn.GetComment().empty())
      //  rInsn.SetComment(OprdLabel.GetName());
      return;
    }
  }

  if (pOprd->GetType() & O_IMM)
  {
    Label OprdLabel = rDoc.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), pOprd->GetValue()));

    if (OprdLabel.GetType() != Label::LabelUnknown)
    {
      ValueName << OprdLabel.GetLabel();
      rMarks.push_back(Cell::Mark(Cell::Mark::LabelType, ValueName.str().length()));
      //if (rInsn.GetComment().empty())
      //  rInsn.SetComment(OprdLabel.GetName());
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
      rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, ValueName.str().length()));
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
    Label OprdLabel = rDoc.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), OprdOff));
    if (OprdLabel.GetType() != Label::LabelUnknown)
    {
      ValueName << OprdLabel.GetLabel();
      rMarks.push_back(Cell::Mark(Cell::Mark::LabelType, ValueName.str().length()));
      //if (rInsn.GetComment().empty())
      //  rInsn.SetComment(OprdLabel.GetName());
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
      rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, ValueName.str().length()));
    }

    rInsnBuf << ValueName.str();
    return;
  }

  if (pOprd->GetType() & O_MEM)
  {
    std::string AccessType;
    switch (pOprd->GetType() & MS_MASK)
    {
    case MS_8BIT:   AccessType = "byte ";  break;
    case MS_16BIT:  AccessType = "word ";  break;
    case MS_32BIT:  AccessType = "dword "; break;
    case MS_64BIT:  AccessType = "qword "; break;
    case MS_80BIT:  AccessType = "tword "; break;
    case MS_128BIT: AccessType = "oword "; break;
    default:        AccessType = "";       break;
    }
    rInsnBuf << AccessType;
    rMarks.push_back(Cell::Mark(Cell::Mark::KeywordType, AccessType.length()));

    if (pOprd->GetType() & O_SEG)
    {
      auto pRegName = m_CpuInfo.ConvertIdentifierToName(pOprd->GetSeg());
      rInsnBuf << pRegName << ":";
      rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, strlen(pRegName)));
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
    }

    if (pOprd->GetType() & O_SEG_VAL)
    {
      rInsnBuf << std::setfill('0') << std::setw(4) << std::hex << pOprd->GetSeg() << ":";
      rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, 4));
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
    }

    rInsnBuf << "[";
    rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));

    if (pOprd->GetType() & O_REG && pOprd->GetReg() != X86_Reg_Unknown)
    {
      auto pRegName = m_CpuInfo.ConvertIdentifierToName(pOprd->GetReg());
      rInsnBuf << pRegName;
      rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, strlen(pRegName)));
    }

    if (pOprd->GetType() & O_SREG && pOprd->GetSecReg() != X86_Reg_Unknown)
    {
      if (pOprd->GetReg() != X86_Reg_Unknown)
      {
        rInsnBuf << " + ";
        rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 3));
      }

      auto pRegName = m_CpuInfo.ConvertIdentifierToName(pOprd->GetSecReg());
      rInsnBuf << pRegName;
      rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, strlen(pRegName)));
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
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 3));
      rMarks.push_back(Cell::Mark(Cell::Mark::ImmediateType, 1));
    }

    if (pOprd->GetType() & O_DISP)
    {
      u64 Disp;
      switch (pOprd->GetType() & DS_MASK)
      {
      case DS_8BIT:  Disp = SignExtend<s64,  8>(pOprd->GetValue()); break;
      case DS_16BIT: Disp = SignExtend<s64, 16>(pOprd->GetValue()); break;
      case DS_32BIT: Disp = SignExtend<s64, 32>(pOprd->GetValue()); break;
      default:       Disp = pOprd->GetValue(); break;
      }
      Address AddrDst(pOprd->GetSegValue(), Disp);
      Cell::Mark::Type MarkType = Cell::Mark::UnknownType;

      Label const& Lbl = rDoc.GetLabelFromAddress(AddrDst);
      if (Lbl.GetType() != Label::LabelUnknown)
      {
        ValueName << Lbl.GetLabel();
        MarkType = Cell::Mark::LabelType;

        //if (rInsn.GetComment().empty())
        //  rInsn.SetComment(Lbl.GetName());
      }
      else
      {
        MarkType = Cell::Mark::ImmediateType;
        switch (pOprd->GetType() & AS_MASK)
        {
        case AS_8BIT:  ValueName << "0x" << std::setw(2)  << static_cast<u32>(Disp); break;
        case AS_16BIT: ValueName << "0x" << std::setw(4)  << static_cast<s16>(Disp); break;
        case AS_32BIT: ValueName << "0x" << std::setw(8)  << static_cast<s32>(Disp); break;
        case AS_64BIT: ValueName << "0x" << std::setw(16) << static_cast<s64>(Disp); break;
        default:       ValueName << "0x" <<                                   Disp ; break;
        }
      }

      if (pOprd->GetReg() == 0x0 && pOprd->GetSecReg() == 0x0)
      {
        rInsnBuf << ValueName.str();
        rMarks.push_back(Cell::Mark(MarkType, ValueName.str().length()));
      }
      else
      {
        rInsnBuf << " + " << ValueName.str();
        rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 3));
        rMarks.push_back(Cell::Mark(MarkType, ValueName.str().length()));
      }
    }

    rInsnBuf << "]";
    rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 1));
    return;
  }

  if (pOprd->GetType() & O_REG)
  {
    auto pRegName = m_CpuInfo.ConvertIdentifierToName(pOprd->GetReg());
    rInsnBuf << pRegName;
    rMarks.push_back(Cell::Mark(Cell::Mark::RegisterType, strlen(pRegName)));
  }
}