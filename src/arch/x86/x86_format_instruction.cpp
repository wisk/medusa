#include "x86.hpp"
#include "x86_architecture.hpp"

//bool X86Architecture::FormatInstruction(
//  Document      const& rDoc,
//  Address       const& rAddr,
//  Instruction   const& rInsn,
//  PrintData          & rPrintData) const
//{
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

//  return true;
//}