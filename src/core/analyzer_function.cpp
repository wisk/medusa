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
        << ": address=" << m_Addr.ToString()
        << ", length=" << FuncLen
        << ", instruction counter: " << InsnCnt
        << LogEnd;

      Label FuncLbl(m_Addr, Label::Function | Label::Global);
      auto spFunction = std::make_shared<Function>(FuncLbl.GetLabel(), FuncLen, InsnCnt);
      m_rDoc.SetMultiCell(m_Addr, spFunction, false);
      m_rDoc.AddLabel(m_Addr, FuncLbl, false);
    }
    else
    {
      auto spInsn = std::static_pointer_cast<Instruction const>(m_rDoc.GetCell(m_Addr));
      if (spInsn == nullptr)
        return false;
      auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
      if (spArch == nullptr)
        return false;
      if (spInsn->GetSubType() != Instruction::JumpType)
        return false;
      Address OpRefAddr;
      if (spInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(m_Addr, *spInsn), OpRefAddr) == false)
        return false;
      auto OpLbl = m_rDoc.GetLabelFromAddress(OpRefAddr);
      if (OpLbl.GetType() == Label::Unknown)
        return false;

      // Set the name <mnemonic> + "_" + sym_name (The name is not refreshed if sym_name is updated)
      std::string FuncName = std::string(spInsn->GetName()) + std::string("_") + OpLbl.GetName();
      m_rDoc.AddLabel(m_Addr, Label(FuncName, Label::Function | Label::Global), false);
      auto spFunc = std::make_shared<Function>(FuncName, spInsn->GetSize(), 1);
      m_rDoc.SetMultiCell(m_Addr, spFunc, true);

      // Propagate the detail ID
      Id RefId;
      if (m_rDoc.RetrieveDetailId(OpRefAddr, 0, RefId))
        m_rDoc.BindDetailId(m_Addr, 0, RefId);
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

    spOs->AnalyzeFunction(m_rDoc, m_Addr);

    return true;
  }

  bool AnalyzerFunction::ComputeFunctionLength(u16& rFunctionLength, u16& rInstructionCounter, u32 LengthThreshold)
  {
    std::stack<Address> CallStack;
    std::map<Address, bool> VisitedInstruction;
    bool RetReached = false;

    u32 FuncSz = 0x0;
    Address CurAddr = m_Addr;
    rFunctionLength = 0x0;
    rInstructionCounter = 0x0;

    auto Lbl = m_rDoc.GetLabelFromAddress(m_Addr);
    if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
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
          auto InsnSz = spInsn->GetSize();
          if (InsnSz == 0)
          {
            Log::Write("core").Level(LogDebug) << "0 size instruction at " << CurAddr << LogEnd;
            break;
          }
          CurAddr += InsnSz;
          continue;
        }

        FuncSz += static_cast<u32>(spInsn->GetSize());

        VisitedInstruction[CurAddr] = true;

        rFunctionLength += static_cast<u32>(spInsn->GetSize());
        rInstructionCounter++;

        if (spInsn->GetSubType() & Instruction::JumpType)
        {
          Address DstAddr;

          if (spInsn->GetSubType() & Instruction::ConditionalType)
            CallStack.push(CurAddr + spInsn->GetSize());

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

        CurAddr += spInsn->GetSize();

        if (LengthThreshold && FuncSz > LengthThreshold)
          return false;
      } // end while (m_Document.IsPresent(CurAddr))
    } // while (!CallStack.empty())

    return RetReached;
  }
}