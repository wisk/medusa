#include "medusa/analyzer.hpp"
#include "medusa/module.hpp"

namespace medusa
{
  bool AnalyzerFunction::CreateFunction(void)
  {
    Address FuncEnd;
    u16 FuncLen;
    u16 InsnCnt;

    if (ComputeFunctionLength(FuncLen, InsnCnt) == true)
    {
      Log::Write("core")
        << "Function found"
        << ": address=" << m_rAddr.ToString()
        << ", length=" << FuncLen
        << ", instruction counter: " << InsnCnt
        << LogEnd;

      Label FuncLbl(m_rAddr, Label::Function | Label::Global);
      Function* pFunction = new Function(FuncLbl.GetLabel(), FuncLen, InsnCnt);
      m_rDoc.SetMultiCell(m_rAddr, pFunction, false);
      m_rDoc.AddLabel(m_rAddr, FuncLbl, false);
    }
    else
    {
      auto pMemArea = m_rDoc.GetMemoryArea(m_rAddr);
      if (pMemArea == nullptr)
        return false;
      auto spInsn = std::static_pointer_cast<Instruction const>(m_rDoc.GetCell(m_rAddr));
      if (spInsn == nullptr)
        return false;
      auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
      if (spArch == nullptr)
        return false;
      if (spInsn->GetSubType() != Instruction::JumpType)
        return false;
      Address OpRefAddr;
      if (spInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(m_rAddr, *spInsn), OpRefAddr) == false)
        return false;
      auto OpLbl = m_rDoc.GetLabelFromAddress(OpRefAddr);
      if (OpLbl.GetType() == Label::Unknown)
        return false;

      // Set the name <mnemonic> + "_" + sym_name (The name is not refreshed if sym_name is updated)
      std::string FuncName = std::string(spInsn->GetName()) + std::string("_") + OpLbl.GetName();
      m_rDoc.AddLabel(m_rAddr, Label(FuncName, Label::Function | Label::Global), false);
      auto pFunc = new Function(FuncName, spInsn->GetLength(), 1);
      m_rDoc.SetMultiCell(m_rAddr, pFunc, true);

      // Propagate the detail ID
      Id RefId;
      if (m_rDoc.RetrieveDetailId(OpRefAddr, 0, RefId))
        m_rDoc.BindDetailId(m_rAddr, 0, RefId);
    }

    auto OsName = m_rDoc.GetOperatingSystemName();
    if (OsName.empty())
      return true;

    auto spOs = ModuleManager::Instance().GetOperatingSystem(OsName);
    if (spOs == nullptr)
    {
      Log::Write("core") << "unable to get operating system module " << OsName << LogEnd;
      return true;
    }

    spOs->AnalyzeFunction(m_rDoc, m_rAddr);

    return true;
  }

  bool AnalyzerFunction::ComputeFunctionLength(u16& rFunctionLength, u16& rInstructionCounter, u32 LengthThreshold)
  {
    std::stack<Address> CallStack;
    std::map<Address, bool> VisitedInstruction;
    bool RetReached = false;

    u32 FuncLen = 0x0;
    Address CurAddr = m_rAddr;
    rFunctionLength = 0x0;
    rInstructionCounter = 0x0;
    MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

    auto Lbl = m_rDoc.GetLabelFromAddress(m_rAddr);
    if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
      return false;

    if (pMemArea == nullptr)
      return false;

    CallStack.push(CurAddr);

    while (!CallStack.empty())
    {
      CurAddr = CallStack.top();
      CallStack.pop();

      while (m_rDoc.ContainsCode(CurAddr))
      {
        auto spInsn = std::static_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));

        if (spInsn == nullptr)
        {
          Log::Write("core") << "instruction at " << CurAddr.ToString() << " is null" << LogEnd;
          break;
        }

        auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
        if (spArch == nullptr)
          return false;

        if (VisitedInstruction[CurAddr])
        {
          CurAddr += spInsn->GetLength();
          continue;
        }

        FuncLen += static_cast<u32>(spInsn->GetLength());

        VisitedInstruction[CurAddr] = true;

        rFunctionLength += static_cast<u32>(spInsn->GetLength());
        rInstructionCounter++;

        if (spInsn->GetSubType() & Instruction::JumpType)
        {
          Address DstAddr;

          if (spInsn->GetSubType() & Instruction::ConditionalType)
            CallStack.push(CurAddr + spInsn->GetLength());

          if (expr_cast<MemoryExpression>(spInsn->GetOperand(0)) != nullptr)
            break;

          if (!spInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spInsn), DstAddr))
            break;

          CurAddr = DstAddr;
          continue;
        }

        else if (spInsn->GetSubType() & Instruction::ReturnType && !(spInsn->GetSubType() & Instruction::ConditionalType))
        {
          RetReached = true;
          break;
        }

        CurAddr += spInsn->GetLength();

        if (LengthThreshold && FuncLen > LengthThreshold)
          return false;
      } // end while (m_Document.IsPresent(CurAddr))
    } // while (!CallStack.empty())

    return RetReached;
  }
}