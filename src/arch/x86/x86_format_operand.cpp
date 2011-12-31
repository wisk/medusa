#include "x86.hpp"
#include "x86_architecture.hpp"

std::string X86Architecture::FormatOperand(TAddress Address, Instruction const& rInsn, Operand const* pOprd)
{
  std::ostringstream oss;

  oss << std::hex << std::showbase;

  if (pOprd->GetType() & O_IMM)
  {
    switch (pOprd->GetType() & DS_MASK)
    {
    case DS_8BIT:  oss << static_cast<u32>(static_cast<u8> (pOprd->GetValue())); break;
    case DS_16BIT: oss << static_cast<s16>(pOprd->GetValue()); break;
    case DS_32BIT: oss << static_cast<s32>(pOprd->GetValue()); break;
    case DS_64BIT: oss << static_cast<s64>(pOprd->GetValue()); break;
    default:       oss << pOprd->GetValue();                   break;
    }
  }

  else if (pOprd->GetType() & O_REL)
  {
    TRelative Rel;
    switch (pOprd->GetType() & DS_MASK)
    {
    case DS_8BIT:  Rel = static_cast<s8> (pOprd->GetValue()); break;
    case DS_16BIT: Rel = static_cast<s16>(pOprd->GetValue()); break;
    case DS_32BIT: Rel = static_cast<s32>(pOprd->GetValue()); break;
    case DS_64BIT: Rel = static_cast<s64>(pOprd->GetValue()); break;
    default:       Rel = pOprd->GetValue();                   break;
    }
    oss << std::hex << Address + rInsn.GetLength() + Rel;
  }

  else
  {
    if (pOprd->GetType() & O_MEM)
    {
      if ((pOprd->GetType() & MS_MASK) ==  MS_8BIT)
        oss << "byte ";
      else if ((pOprd->GetType() & MS_MASK) == MS_16BIT)
        oss << "word ";
      else if ((pOprd->GetType() & MS_MASK) == MS_32BIT)
        oss << "dword ";
      else if ((pOprd->GetType() & MS_MASK) == MS_64BIT)
        oss << "qword ";

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
        oss << std::hex << pOprd->GetValue();
      else
        oss << std::hex << " + " << pOprd->GetValue();
    }

    if (pOprd->GetType() & O_MEM)
      oss << "]";
  }

  return oss.str();
}
