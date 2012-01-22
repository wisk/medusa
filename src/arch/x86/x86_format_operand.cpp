#include "x86.hpp"
#include "x86_architecture.hpp"

std::string X86Architecture::FormatOperand(Database const& rDb, TOffset Offset, Instruction const& rInsn, Operand const* pOprd)
{
  std::ostringstream oss;

  std::ostringstream ValueName;
  ValueName << std::setfill('0') << std::showbase << std::left << std::hex;

  Label OprdLabel = rDb.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), pOprd->GetValue()));

  if (OprdLabel.GetType() != Label::LabelUnknown)
    ValueName << OprdLabel.GetName();
  else
    switch (pOprd->GetType() & DS_MASK)
    {
    case DS_8BIT:  ValueName << std::setw(2)  << static_cast<u32>(static_cast<u8> (pOprd->GetValue())); break;
    case DS_16BIT: ValueName << std::setw(4)  << static_cast<s16>(pOprd->GetValue()); break;
    case DS_32BIT: ValueName << std::setw(8)  << static_cast<s32>(pOprd->GetValue()); break;
    case DS_64BIT: ValueName << std::setw(16) << static_cast<s64>(pOprd->GetValue()); break;
    default:       ValueName << pOprd->GetValue();                   break;
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
    OprdLabel = rDb.GetLabelFromAddress(Address(Address::FlatType, pOprd->GetSegValue(), OprdOff));
    if (OprdLabel.GetType() != Label::LabelUnknown)
      ValueName << OprdLabel.GetName();
    else
      ValueName << std::hex << OprdOff;
  }

  oss << std::hex << std::showbase;

  if ((pOprd->GetType() & O_IMM) || (pOprd->GetType() & O_REL))
  {
    oss << ValueName.str();
  }

  else
  {
    if (pOprd->GetType() & O_MEM)
    {
      switch (pOprd->GetType() & MS_MASK)
        {
        case MS_8BIT:   oss << "byte ";  break;
        case MS_16BIT:  oss << "word ";  break;
        case MS_32BIT:  oss << "dword "; break;
        case MS_64BIT:  oss << "qword "; break;
        case MS_80BIT:  oss << "tword "; break;
        case MS_128BIT: oss << "oword "; break;
        }

      if (pOprd->GetType() & O_SEG)
        oss << X86_RegName[pOprd->GetSeg()] << ":";

      if (pOprd->GetType() & O_SEG_VAL)
        oss << pOprd->GetSeg() << ":";

      oss << "[";
    }

    if (pOprd->GetType() & O_REG && pOprd->GetReg() != X86_Reg_Unknown)
      oss << X86_RegName[pOprd->GetReg()];

    if (pOprd->GetType() & O_SREG && pOprd->GetSecReg() != X86_Reg_Unknown)
    {
      if (pOprd->GetReg() != X86_Reg_Unknown)
        oss << " + ";

      oss << X86_RegName[pOprd->GetSecReg()];
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
      oss << " * " << pScaleValue;
    }

    if (pOprd->GetType() & O_DISP)
    {
      if (pOprd->GetReg() == 0x0 && pOprd->GetSecReg() == 0x0)
        oss << std::hex << ValueName.str();
      else
        oss << std::hex << " + " << ValueName.str();
    }

    if (pOprd->GetType() & O_MEM)
      oss << "]";
  }

  return oss.str();
}