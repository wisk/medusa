#include "medusa/analyzer.hpp"
#include "medusa/module.hpp"
#include "medusa/expression_visitor.hpp"

namespace medusa
{
  bool AnalyzerInstruction::FindCrossReference(void)
  {
    auto spArch = ModuleManager::Instance().GetArchitecture(m_rInsn.GetArchitectureTag());
    if (spArch == nullptr)
      return false;

    for (u8 CurOp = 0; CurOp < m_rInsn.GetNumberOfOperand(); ++CurOp)
    {
      auto spCurOprd = m_rInsn.GetOperand(CurOp);

      // HACK: We don't want to resolve MNEM ``ID''{, ...} which could be PC for instance
      if (CurOp == 0 && expr_cast<IdentifierExpression>(spCurOprd) != nullptr)
        continue;

      // Handle reference address (e.g. call [API])
      Address RefAddr;
      if (expr_cast<MemoryExpression>(spCurOprd) != nullptr)
        if (m_rInsn.GetOperandReference(m_rDoc, CurOp, spArch->CurrentAddress(m_Addr, m_rInsn), RefAddr, false))
        {
          m_rDoc.ChangeValueSize(RefAddr, m_rInsn.GetOperand(CurOp)->GetBitSize(), false);
          m_rDoc.AddCrossReference(RefAddr, m_Addr);
          m_rDoc.AddLabel(RefAddr, Label(RefAddr, Label::Data | Label::Global), false);
        }

      Address DstAddr;
      if (m_rInsn.GetOperandReference(m_rDoc, CurOp, spArch->CurrentAddress(m_Addr, m_rInsn), DstAddr))
      {
        // TODO(wisk): test if address is present in the document
        // and check result of AddCrossReference

        // Add cross reference if possible
        m_rDoc.AddCrossReference(DstAddr, m_Addr);
        MemoryArea MemArea;
        if (!m_rDoc.GetMemoryArea(DstAddr, MemArea))
          continue;

        u16 LblTy = Label::Unknown;

        switch (m_rInsn.GetSubType() & (Instruction::CallType | Instruction::JumpType))
        {
        case Instruction::CallType: LblTy = Label::Code | Label::Local; break;
        case Instruction::JumpType: LblTy = Label::Code | Label::Local; break;
        case Instruction::NoneType: LblTy = to_bool(MemArea.GetAccess() & MemoryArea::Access::Execute) ?
          Label::Code | Label::Local : Label::Data | Label::Global;
        default: break;
        } // switch (pInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType))

        m_rDoc.AddLabel(DstAddr, Label(DstAddr, LblTy), false);
      }



    } // for (u8 CurOp = 0; CurOp < spInsn->GetNumberOfOperand(); ++CurOp)

    return true;
  }

  bool AnalyzerInstruction::FindString(void)
  {
    return false;
  }
}