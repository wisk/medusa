#include "x86.hpp"
#include "x86_architecture.hpp"
#include <medusa/extend.hpp>

bool X86Architecture::FormatOperand(
  Document      const& rDoc,
  Address       const& rAddress,
  Instruction   const& rInstruction,
  u8                   OperandNo,
  PrintData          & rPrintData) const
{
  // BROKEN
  //rPrintData.MarkOffset();

  //s64 RelValue = static_cast<s64>(rOperand.GetValue());

  //Address RefAddr;
  //bool OprdHasRef = rInstruction.GetOperandReference(rDoc, OperandNo, rAddress, RefAddr);
  //if (rOperand.GetType() & O_REG_PC_REL && OprdHasRef)
  //{
  //  if (rOperand.GetType() & O_MEM)
  //  {
  //    std::string AccessType;
  //    switch (rOperand.GetType() & MS_MASK)
  //    {
  //    case MS_8BIT:   AccessType = "byte ";  break;
  //    case MS_16BIT:  AccessType = "word ";  break;
  //    case MS_32BIT:  AccessType = "dword "; break;
  //    case MS_64BIT:  AccessType = "qword "; break;
  //    case MS_80BIT:  AccessType = "tword "; break;
  //    case MS_128BIT: AccessType = "oword "; break;
  //    default:        AccessType = "";       break;
  //    }
  //    rPrintData
  //      .AppendKeyword(AccessType)
  //      .AppendSpace().AppendOperator("[");
  //  }

  //  rPrintData.AppendKeyword("rel").AppendSpace();

  //  Label OprdLabel = rDoc.GetLabelFromAddress(RefAddr);
  //  if (OprdLabel.GetType() != Label::Unknown)
  //    rPrintData.AppendLabel(OprdLabel.GetLabel());
  //  else
  //    rPrintData.AppendAddress(RefAddr);

  //  if (rOperand.GetType() & O_MEM)
  //    rPrintData.AppendOperator("]");

  //  return true;
  //}

  //if (rOperand.GetType() & O_IMM)
  //{
  //  Id BindId;
  //  ValueDetail ValDtl;
  //  if (rDoc.RetrieveDetailId(rAddress, OperandNo, BindId) && rDoc.GetValueDetail(BindId, ValDtl))
  //  {
  //    FormatValueDetail(rDoc, rAddress, rOperand.GetSizeInBit(), ValDtl, rPrintData);
  //  }
  //  else
  //  {
  //    Label OprdLabel = rDoc.GetLabelFromAddress(Address(Address::FlatType, rOperand.GetSegValue(), rOperand.GetValue()));

  //    if (OprdLabel.GetType() != Label::Unknown)
  //      rPrintData.AppendLabel(OprdLabel.GetLabel());
  //    else
  //    {
  //      switch (rOperand.GetType() & DS_MASK)
  //      {
  //      case DS_8BIT:  rPrintData.AppendImmediate(rOperand.GetValue(),  8); break;
  //      case DS_16BIT: rPrintData.AppendImmediate(rOperand.GetValue(), 16); break;
  //      case DS_32BIT: rPrintData.AppendImmediate(rOperand.GetValue(), 32); break;
  //      case DS_64BIT:
  //      default:       rPrintData.AppendImmediate(rOperand.GetValue(), 64); break;
  //      }
  //    }
  //  }
  //  return true;
  //}

  //if (rOperand.GetType() & O_REL)
  //{
  //  s64 OprdVal;
  //  switch (rOperand.GetType() & DS_MASK)
  //  {
  //  case DS_8BIT:  OprdVal = static_cast<s8> (rOperand.GetValue()); break;
  //  case DS_16BIT: OprdVal = static_cast<s16>(rOperand.GetValue()); break;
  //  case DS_32BIT: OprdVal = static_cast<s32>(rOperand.GetValue()); break;
  //  case DS_64BIT: OprdVal = static_cast<s64>(rOperand.GetValue()); break;
  //  default:       OprdVal = rOperand.GetValue();                   break;
  //  }
  //  Address DstAddr = rAddress + rInstruction.GetLength() + OprdVal;
  //  Label OprdLabel = rDoc.GetLabelFromAddress(DstAddr);
  //  if (OprdLabel.GetType() != Label::Unknown)
  //    rPrintData.AppendLabel(OprdLabel.GetLabel());
  //  else
  //    rPrintData.AppendAddress(DstAddr + rOperand.GetValue());
  //  return true;
  //}

  //if (rOperand.GetType() & O_MEM)
  //{
  //  std::string AccessType;
  //  switch (rOperand.GetType() & MS_MASK)
  //  {
  //  case MS_8BIT:   AccessType = "byte ";  break;
  //  case MS_16BIT:  AccessType = "word ";  break;
  //  case MS_32BIT:  AccessType = "dword "; break;
  //  case MS_64BIT:  AccessType = "qword "; break;
  //  case MS_80BIT:  AccessType = "tword "; break;
  //  case MS_128BIT: AccessType = "oword "; break;
  //  default:        AccessType = "";       break;
  //  }
  //  rPrintData.AppendKeyword(AccessType);

  //  if (rOperand.GetType() & O_SEG)
  //  {
  //    auto pRegName = m_CpuInfo.ConvertIdentifierToName(rOperand.GetSeg());
  //    rPrintData.AppendRegister(pRegName).AppendOperator(":");
  //  }

  //  else if (rOperand.GetType() & O_SEG_VAL)
  //    rPrintData.AppendImmediate(rOperand.GetSeg(), 0x10).AppendOperator(":");

  //  rPrintData.AppendOperator("[");

  //  if (rOperand.GetType() & O_REG && rOperand.GetReg() != X86_Reg_Unknown)
  //  {
  //    auto pRegName = m_CpuInfo.ConvertIdentifierToName(rOperand.GetReg());
  //    rPrintData.AppendRegister(pRegName);
  //  }

  //  if (rOperand.GetType() & O_SREG && rOperand.GetSecReg() != X86_Reg_Unknown)
  //  {
  //    if (rOperand.GetReg() != X86_Reg_Unknown)
  //      rPrintData.AppendSpace().AppendOperator("+").AppendSpace();

  //    auto pRegName = m_CpuInfo.ConvertIdentifierToName(rOperand.GetSecReg());
  //    rPrintData.AppendRegister(pRegName);
  //  }

  //  if (rOperand.GetType() & O_SCALE && rOperand.GetSecReg() != X86_Reg_Unknown)
  //  {
  //    char const* pScaleValue = "1";
  //    switch (rOperand.GetType() & SC_MASK)
  //    {
  //    case SC_2: pScaleValue = "2"; break;
  //    case SC_4: pScaleValue = "4"; break;
  //    case SC_8: pScaleValue = "8"; break;
  //    }
  //    rPrintData.AppendSpace().AppendOperator("*").AppendSpace().AppendImmediate(pScaleValue);
  //  }

  //  if (rOperand.GetType() & O_DISP)
  //  {
  //    if (rOperand.GetReg() != 0x0 || rOperand.GetSecReg() != 0x0)
  //      rPrintData.AppendSpace().AppendOperator("+").AppendSpace();

  //    Id BindId;
  //    ValueDetail ValDtl;
  //    if (rDoc.RetrieveDetailId(rAddress, OperandNo, BindId) && rDoc.GetValueDetail(BindId, ValDtl))
  //    {
  //      FormatValueDetail(rDoc, rAddress, rOperand.GetSizeInBit(), ValDtl, rPrintData);
  //    }
  //    else
  //    {
  //      u64 Disp;
  //      switch (rOperand.GetType() & DS_MASK)
  //      {
  //      case DS_8BIT:  Disp = SignExtend<s64,  8>(rOperand.GetValue()); break;
  //      case DS_16BIT: Disp = SignExtend<s64, 16>(rOperand.GetValue()); break;
  //      case DS_32BIT: Disp = SignExtend<s64, 32>(rOperand.GetValue()); break;
  //      default:       Disp = rOperand.GetValue(); break;
  //      }
  //      Address AddrDst(rOperand.GetSegValue(), Disp);

  //      Label const& Lbl = rDoc.GetLabelFromAddress(AddrDst);
  //      if (Lbl.GetType() != Label::Unknown)
  //        rPrintData.AppendLabel(Lbl.GetLabel());
  //      else
  //      {
  //        switch (rOperand.GetType() & AS_MASK)
  //        {
  //        case AS_8BIT:  rPrintData.AppendImmediate(Disp,  8); break;
  //        case AS_16BIT: rPrintData.AppendImmediate(Disp,  16); break;
  //        case AS_32BIT: rPrintData.AppendImmediate(Disp,  32); break;
  //        case AS_64BIT:
  //        default:       rPrintData.AppendImmediate(Disp,  64); break;
  //        }
  //      }
  //    }
  //  }

  //  rPrintData.AppendOperator("]");
  //  return true;
  //}

  //if (rOperand.GetType() & O_REG)
  //{
  //  auto pRegName = m_CpuInfo.ConvertIdentifierToName(rOperand.GetReg());
  //  rPrintData.AppendRegister(pRegName);
  //}

  return true;
}