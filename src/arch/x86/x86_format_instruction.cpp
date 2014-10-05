#include "x86.hpp"
#include "x86_architecture.hpp"

bool X86Architecture::FormatInstruction(
  Document      const& rDoc,
  Address       const& rAddr,
  Instruction   const& rInsn,
  PrintData          & rPrintData) const
{
  // BROKEN
  //char Sep = '\0';
  //std::string Mnem;

  //std::string OpRefCmt;
  //rDoc.GetComment(rAddr, OpRefCmt);

  //if (rInsn.GetPrefix())
  //{
  //  if (rInsn.GetPrefix() & X86_Prefix_Lock)
  //    Mnem = "lock ";
  //  else if (rInsn.GetPrefix() & X86_Prefix_RepNz)
  //    Mnem = "repnz ";
  //  else if (rInsn.GetPrefix() & X86_Prefix_Rep)
  //  {
  //    // 0xF3 is only used as REPZ prefix for cmps and scas instructions.
  //    if (rInsn.GetOpcode() == X86_Opcode_Cmps || rInsn.GetOpcode() == X86_Opcode_Scas)
  //      Mnem = "repz ";
  //    else
  //      Mnem = "rep ";
  //  }
  //}

  //Mnem += m_Mnemonic[rInsn.GetOpcode()];
  //rPrintData.AppendMnemonic(Mnem).AppendSpace();

  //for (unsigned int i = 0; i < OPERAND_NO; ++i)
  //{
  //  Operand const* pOprd = rInsn.Operand(i);
  //  if (pOprd == nullptr)
  //    break;
  //  if (pOprd->GetType() == O_NONE)
  //    break;

  //  if (Sep != '\0')
  //    rPrintData.AppendOperator(",").AppendSpace();

  //  FormatOperand(rDoc, rAddr, rInsn, *pOprd, i, rPrintData);

  //  Sep = ',';

  //  Address OpRefAddr;
  //  if (OpRefCmt.empty() && rInsn.GetOperandReference(rDoc, i, rAddr, OpRefAddr))
  //  {
  //    Id OpId;
  //    if (rDoc.RetrieveDetailId(OpRefAddr, 0, OpId))
  //    {
  //      FunctionDetail FuncDtl;
  //      if (rDoc.GetFunctionDetail(OpId, FuncDtl))
  //      {
  //        // TODO: provide helper to avoid this...
  //        u16 CmtOff = static_cast<u16>(rPrintData.GetCurrentText().length()) - 6 - 1 - rAddr.ToString().length();

  //        rPrintData.AppendSpace().AppendComment(";").AppendSpace();
  //        FormatTypeDetail(FuncDtl.GetReturnType(), rPrintData);
  //        rPrintData.AppendSpace().AppendLabel(FuncDtl.GetName()).AppendOperator("(");

  //        if (!FuncDtl.GetParameters().empty())
  //          rPrintData.AppendNewLine().AppendSpace(CmtOff).AppendComment(";").AppendSpace(3);

  //        bool FirstParam = true;
  //        for (auto const& rParam : FuncDtl.GetParameters())
  //        {
  //          if (FirstParam)
  //            FirstParam = false;
  //          else
  //            rPrintData.AppendOperator(",").AppendNewLine().AppendSpace(CmtOff).AppendComment(";").AppendSpace(3);
  //          FormatTypeDetail(*rParam.GetType().get(), rPrintData);
  //          rPrintData.AppendSpace().AppendLabel(rParam.GetValue().GetName());
  //        }
  //        rPrintData.AppendOperator(");");
  //      }
  //    }
  //  }

  //}

  return true;
}

void X86Architecture::ApplySegmentOverridePrefix(Instruction& rInsn, Operand* pOprd)
{
  if (rInsn.GetPrefix() && pOprd->GetType() & O_MEM)
  {
    // For instructions with two memory operands (cmps/movs),
    // the segment override is only applied on the source operand (i == 1).
    if (rInsn.Opcode() == X86_Opcode_Cmps || rInsn.Opcode() == X86_Opcode_Movs)
      return;

    // TODO: What do we do for multiple segment override prefix?
    // Intel Vol2A 2.1.1: For each instruction, it is only useful to include
    // up to one prefix code from each of the four groups (Groups 1, 2, 3, 4).
    if (rInsn.GetPrefix() & X86_Prefix_CS || rInsn.GetPrefix() & X86_Prefix_HintNotTaken)
    {
      pOprd->Type() |= O_SEG;
      pOprd->Seg() = X86_Reg_Cs;
    }
    else if (rInsn.GetPrefix() & X86_Prefix_DS || rInsn.GetPrefix() & X86_Prefix_HintTaken)
    {
      pOprd->Type() |= O_SEG;
      pOprd->Seg() = X86_Reg_Ds;
    }
    else if (rInsn.GetPrefix() & X86_Prefix_ES)
    {
      pOprd->Type() |= O_SEG;
      pOprd->Seg() = X86_Reg_Es;
    }
    else if (rInsn.GetPrefix() & X86_Prefix_FS || rInsn.GetPrefix() & X86_Prefix_HintAltTaken)
    {
      pOprd->Type() |= O_SEG;
      pOprd->Seg() = X86_Reg_Fs;
    }
    else if (rInsn.GetPrefix() & X86_Prefix_GS)
    {
      pOprd->Type() |= O_SEG;
      pOprd->Seg() = X86_Reg_Gs;
    }
    else if (rInsn.GetPrefix() & X86_Prefix_SS)
    {
      pOprd->Type() |= O_SEG;
      pOprd->Seg() = X86_Reg_Ss;
    }
  }
}