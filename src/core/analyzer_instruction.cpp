#include "medusa/analyzer.hpp"
#include "medusa/module.hpp"

namespace medusa
{
  bool AnalyzerInstruction::FindCrossReference(void)
  {
    auto spInsn = std::dynamic_pointer_cast<Instruction const>(m_rDoc.GetCell(m_Addr));
    if (spInsn == nullptr)
      return false;

    auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
    if (spArch == nullptr)
      return false;

    for (u8 CurOp = 0; CurOp < spInsn->GetNumberOfOperand(); ++CurOp)
    {
      auto spCurOprd = spInsn->GetOperand(CurOp);

      // HACK: We don't want to resolve MNEM ``ID''{, ...} which could be PC for instance
      if (CurOp == 0 && expr_cast<IdentifierExpression>(spCurOprd) != nullptr)
        continue;
      if (expr_cast<BindExpression>(spCurOprd) != nullptr)
        continue;

      Address DstAddr;
      if (!spInsn->GetOperandReference(m_rDoc, CurOp, spArch->CurrentAddress(m_Addr, *spInsn), DstAddr))
        continue;

      auto spMemExpr = expr_cast<MemoryExpression>(spCurOprd);
      if (spMemExpr != nullptr && spMemExpr->IsDereferencable())
      {
        Address RefAddr;
        if (spInsn->GetOperandReference(m_rDoc, CurOp, spArch->CurrentAddress(m_Addr, *spInsn), RefAddr, false))
          m_rDoc.ChangeValueSize(RefAddr, spInsn->GetOperand(CurOp)->GetBitSize(), false);
      }

      // Check if the destination is valid and is an instruction
      auto spDstCell = m_rDoc.GetCell(DstAddr);
      if (spDstCell == nullptr)
        continue;

      // Add XRef
      if (!m_rDoc.AddCrossReference(DstAddr, m_Addr))
        return false;
      MemoryArea MemArea;
      if (!m_rDoc.GetMemoryArea(DstAddr, MemArea))
        return false;

      u16 LblTy = Label::Unknown;

      switch (spInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType))
      {
      case Instruction::CallType: LblTy = Label::Code | Label::Local; break;
      case Instruction::JumpType: LblTy = Label::Code | Label::Local; break;
      case Instruction::NoneType: LblTy = (MemArea.GetAccess() & MemoryArea::Execute) ?
        Label::Code | Label::Local : Label::Data | Label::Global;
      default: break;
      } // switch (pInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType))

      m_rDoc.AddLabel(DstAddr, Label(DstAddr, LblTy), false);
    } // for (u8 CurOp = 0; CurOp < spInsn->GetNumberOfOperand(); ++CurOp)

    return true;
  }

  bool AnalyzerInstruction::FindString(void)
  {
    return false;
  }
}