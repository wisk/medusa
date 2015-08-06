#include "medusa/analyzer.hpp"
#include "medusa/module.hpp"

namespace medusa
{
  bool AnalyzerDisassemble::Disassemble(void)
  {
    auto Lbl = m_rDoc.GetLabelFromAddress(m_Addr);
    if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
      return true;

    std::stack<Address> CallStack;
    Address::List FuncAddr;
    Address CurAddr = m_Addr;
    MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

    if (pMemArea == nullptr)
    {
      //Log::Write("core") << "unable to get memory area for address " << CurAddr.ToString() << LogEnd;
      return false;
    }

    // Push entry point
    CallStack.push(CurAddr);

    // Do we still have functions to disassemble?
    while (!CallStack.empty())
    {
      // Retrieve the last function
      CurAddr = CallStack.top();
      CallStack.pop();
      bool FunctionIsFinished = false;

      //Log::Write("debug") << "Analyzing address: " << CurAddr.ToString() << LogEnd;

      // Disassemble a function
      while (!m_rDoc.ContainsCode(CurAddr))
      {
        //Log::Write("debug") << "Disassembling basic block at " << CurAddr.ToString() << LogEnd;

        auto const& rLbl = m_rDoc.GetLabelFromAddress(CurAddr);
        if ((rLbl.GetType() & Label::AccessMask) == Label::Imported)
          break;

        // Let's try to disassemble a basic block
        AnalyzerDisassemble AnlzDisasm(m_rDoc, CurAddr);
        std::list<Instruction::SPType> BasicBlock;
        if (!AnlzDisasm.DisassembleBasicBlock(BasicBlock))
          break;
        if (BasicBlock.size() == 0)
          break;

        for (auto spInsn : BasicBlock)
        {
          auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
          if (spArch == nullptr)
            return false;

          if (m_rDoc.ContainsCode(CurAddr))
          {
            //Log::Write("debug") << "Instruction is already disassembled at " << CurAddr.ToString() << LogEnd;
            FunctionIsFinished = true;
            continue;
          }

          if (!m_rDoc.SetCell(CurAddr, spInsn, true))
          {
            //Log::Write("core") << "Error while inserting instruction at " << CurAddr.ToString() << LogEnd;
            FunctionIsFinished = true;
            continue;
          }

          for (u8 i = 0; i < spInsn->GetNumberOfOperand(); ++i)
          {
            Address DstAddr;
            if (spInsn->GetOperandReference(m_rDoc, i, spArch->CurrentAddress(CurAddr, *spInsn), DstAddr))
              CallStack.push(DstAddr);
          }

          AnalyzerInstruction AnlzInsn(m_rDoc, CurAddr);
          AnlzInsn.FindCrossReference();

          auto InsnType = spInsn->GetSubType();
          if (InsnType == Instruction::NoneType || InsnType == Instruction::ConditionalType)
            CurAddr += spInsn->GetLength();
        }

        if (FunctionIsFinished)
          break;

        auto spLastInsn = BasicBlock.back();
        //Log::Write("debug") << "Last insn: " << spLastInsn->ToString() << LogEnd;
        auto spArch = ModuleManager::Instance().GetArchitecture(spLastInsn->GetArchitectureTag());
        if (spArch == nullptr)
          break;

        switch (spLastInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType | Instruction::ReturnType))
        {
          // If the last instruction is a call, we follow it and save the return address
        case Instruction::CallType:
        {
          Address DstAddr;

          // Save return address
          CallStack.push(spArch->CurrentAddress(CurAddr, *spLastInsn));

          // Sometimes, we cannot determine the destination address, so we give up
          // We assume destination is hold in the first operand
          if (!spLastInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spLastInsn), DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          FuncAddr.push_back(DstAddr);
          CurAddr = DstAddr;
          break;
        } // end CallType

        // If the last instruction is a ret, we emulate its behavior
        case Instruction::ReturnType:
        {
          // We ignore conditional ret
          if (spLastInsn->GetSubType() & Instruction::ConditionalType)
          {
            CurAddr += spLastInsn->GetLength();
            continue;
          }

          // ret if reached, we try to disassemble an another function (or another part of this function)
          FunctionIsFinished = true;
          break;
        } // end ReturnType

        // Jump type could be a bit tedious to handle because of conditional jump
        // Basically we use the same policy as call instruction

        case Instruction::JumpType:
        {
          Address DstAddr;

          // Save untaken branch address
          if (spLastInsn->GetSubType() & Instruction::ConditionalType)
            CallStack.push(CurAddr + spLastInsn->GetLength());

          // Sometime, we can't determine the destination address, so we give up
          if (!spLastInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spLastInsn), DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          CurAddr = DstAddr;
          break;
        } // end JumpType

        default: break; // This case should never happen
        } // switch (spLastInsn->GetSubType())

        if (FunctionIsFinished)
          break;
      } // end while (m_Document.IsPresent(CurAddr))
    } // while (!CallStack.empty())

    return true;
  }

  bool AnalyzerDisassemble::DisassembleWith(Architecture& rArch, u8 Mode)
  {
    auto Lbl = m_rDoc.GetLabelFromAddress(m_Addr);
    if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
      return true;

    std::stack<Address> CallStack;
    Address::List FuncAddr;
    Address CurAddr = m_Addr;
    MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

    if (pMemArea == nullptr)
    {
      //Log::Write("core") << "unable to get memory area for address " << CurAddr.ToString() << LogEnd;
      return false;
    }

    // Push entry point
    CallStack.push(CurAddr);

    // Do we still have functions to disassemble?
    while (!CallStack.empty())
    {
      // Retrieve the last function
      CurAddr = CallStack.top();
      CallStack.pop();
      bool FunctionIsFinished = false;

      //Log::Write("debug") << "Analyzing address: " << CurAddr.ToString() << LogEnd;

      // Disassemble a function
      while (!m_rDoc.ContainsCode(CurAddr))
      {
        //Log::Write("debug") << "Disassembling basic block at " << CurAddr.ToString() << LogEnd;

        auto const& rLbl = m_rDoc.GetLabelFromAddress(CurAddr);
        if ((rLbl.GetType() & Label::AccessMask) == Label::Imported)
          break;

        // Let's try to disassemble a basic block
        AnalyzerDisassemble AnlzDisasm(m_rDoc, CurAddr);
        std::list<Instruction::SPType> BasicBlock;
        if (!AnlzDisasm.DisassembleBasicBlockWith(rArch, Mode, BasicBlock))
          break;
        if (BasicBlock.size() == 0)
          break;

        for (auto spInsn : BasicBlock)
        {
          auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
          if (spArch == nullptr)
            return false;

          if (m_rDoc.ContainsCode(CurAddr))
          {
            //Log::Write("debug") << "Instruction is already disassembled at " << CurAddr.ToString() << LogEnd;
            FunctionIsFinished = true;
            continue;
          }

          if (!m_rDoc.SetCell(CurAddr, spInsn, true))
          {
            //Log::Write("core") << "Error while inserting instruction at " << CurAddr.ToString() << LogEnd;
            FunctionIsFinished = true;
            continue;
          }

          for (u8 i = 0; i < spInsn->GetNumberOfOperand(); ++i)
          {
            Address DstAddr;
            if (spInsn->GetOperandReference(m_rDoc, i, spArch->CurrentAddress(CurAddr, *spInsn), DstAddr))
              CallStack.push(DstAddr);
          }

          AnalyzerInstruction AnlzInsn(m_rDoc, CurAddr);
          AnlzInsn.FindCrossReference();

          auto InsnType = spInsn->GetSubType();
          if (InsnType == Instruction::NoneType || InsnType == Instruction::ConditionalType)
            CurAddr += spInsn->GetLength();
        }

        if (FunctionIsFinished)
          break;

        auto spLastInsn = BasicBlock.back();
        //Log::Write("debug") << "Last insn: " << spLastInsn->ToString() << LogEnd;
        auto spArch = ModuleManager::Instance().GetArchitecture(spLastInsn->GetArchitectureTag());
        if (spArch == nullptr)
          break;

        switch (spLastInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType | Instruction::ReturnType))
        {
          // If the last instruction is a call, we follow it and save the return address
        case Instruction::CallType:
        {
          Address DstAddr;

          // Save return address
          CallStack.push(spArch->CurrentAddress(CurAddr, *spLastInsn));

          // Sometimes, we cannot determine the destination address, so we give up
          // We assume destination is hold in the first operand
          if (!spLastInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spLastInsn), DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          FuncAddr.push_back(DstAddr);
          CurAddr = DstAddr;
          break;
        } // end CallType

        // If the last instruction is a ret, we emulate its behavior
        case Instruction::ReturnType:
        {
          // We ignore conditional ret
          if (spLastInsn->GetSubType() & Instruction::ConditionalType)
          {
            CurAddr += spLastInsn->GetLength();
            continue;
          }

          // ret if reached, we try to disassemble an another function (or another part of this function)
          FunctionIsFinished = true;
          break;
        } // end ReturnType

        // Jump type could be a bit tedious to handle because of conditional jump
        // Basically we use the same policy as call instruction

        case Instruction::JumpType:
        {
          Address DstAddr;

          // Save untaken branch address
          if (spLastInsn->GetSubType() & Instruction::ConditionalType)
            CallStack.push(CurAddr + spLastInsn->GetLength());

          // Sometime, we can't determine the destination address, so we give up
          if (!spLastInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spLastInsn), DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          CurAddr = DstAddr;
          break;
        } // end JumpType

        default: break; // This case should never happen
        } // switch (spLastInsn->GetSubType())

        if (FunctionIsFinished)
          break;
      } // end while (m_Document.IsPresent(CurAddr))
    } // while (!CallStack.empty())

    return true;
  }

  bool AnalyzerDisassemble::DisassembleBasicBlock(std::list<Instruction::SPType>& rBasicBlock)
  {
    Address CurAddr = m_Addr;
    MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

    try
    {
      auto Lbl = m_rDoc.GetLabelFromAddress(CurAddr);
      if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
        throw std::string("Label \"") + Lbl.GetName() + std::string("\" ") + Lbl.GetLabel() + std::string(" is imported");

      if (pMemArea == nullptr)
        throw std::string("unable to get memory area for address: ") + CurAddr.ToString();

      while (/*m_rDoc.IsPresent(CurAddr)*/ true)
      {
        // If we changed the current memory area, we must update it
        if (!pMemArea->IsCellPresent(CurAddr))
          if ((pMemArea = m_rDoc.GetMemoryArea(CurAddr)) == nullptr)
            throw std::string("unable to get memory area for address: ") + CurAddr.ToString();

        // If the current memory area is not executable, we skip this execution flow
        if (!(pMemArea->GetAccess() & MemoryArea::Execute))
          throw std::string("memory access \"") + pMemArea->GetName() + std::string("\" is not executable");

        if (m_rDoc.ContainsCode(CurAddr))
          return true;

        if (!m_rDoc.ContainsUnknown(CurAddr))
          throw std::string("cell at \"") + CurAddr.ToString() + std::string("\" is not unknown");

        // We create a new entry and disassemble it
        auto spInsn = std::make_shared<Instruction>();

        TOffset PhysicalOffset;

        if (!pMemArea->ConvertOffsetToFileOffset(CurAddr.GetOffset(), PhysicalOffset))
          throw std::string("unable to convert address ") + CurAddr.ToString() + std::string(" to offset");

        auto Tag = m_rDoc.GetArchitectureTag(CurAddr);
        auto spArch = ModuleManager::Instance().GetArchitecture(Tag);
        if (spArch == nullptr)
          throw std::string("unable to find architecture module for: ") + CurAddr.ToString();
        u8 Mode = m_rDoc.GetMode(CurAddr);

        // If something bad happens, we quit
        if (!spArch->Disassemble(m_rDoc.GetBinaryStream(), PhysicalOffset, *spInsn, Mode))
          throw std::string("unable to disassemble instruction at ") + CurAddr.ToString();

        if (spInsn->GetLength() == 0)
          throw std::string("0 length instruction at ") + CurAddr.ToString();

        // We try to retrieve the current instruction, if it's true we go to the next function
        for (size_t InsnLen = 0; InsnLen < spInsn->GetLength(); ++InsnLen)
          if (m_rDoc.ContainsCode(CurAddr + InsnLen))
            return true;

        rBasicBlock.push_back(spInsn);

        auto OpType = spInsn->GetSubType();
        if (
          OpType & Instruction::JumpType
          || OpType & Instruction::CallType
          || OpType & Instruction::ReturnType)
        {
          return true;
        }

        CurAddr += spInsn->GetLength();
      } // !while (m_rDoc.IsPresent(CurAddr))
    }

    catch (std::string const& rExcpMsg)
    {
      rBasicBlock.clear();
      Log::Write("core").Level(LogDebug) << rExcpMsg << LogEnd;
      return false;
    }

    return false;

    //// At this point, we reach neither an basic block exit (jump, call, return) nor code,
    //// so if we must disassemble basic block only: we have to return false, otherwise it's safe
    //// to return true.
    //return spArch->DisassembleBasicBlockOnly() == false ? true : false;
  }

  bool AnalyzerDisassemble::DisassembleBasicBlockWith(Architecture& rArch, u8 Mode, std::list<Instruction::SPType>& rBasicBlock)
  {
    Address CurAddr = m_Addr;
    MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

    try
    {
      auto Lbl = m_rDoc.GetLabelFromAddress(CurAddr);
      if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
        throw std::string("Label \"") + Lbl.GetName() + std::string("\" ") + Lbl.GetLabel() + std::string(" is imported");

      if (pMemArea == nullptr)
        throw std::string("unable to get memory area for address: ") + CurAddr.ToString();

      while (/*m_rDoc.IsPresent(CurAddr)*/ true)
      {
        // If we changed the current memory area, we must update it
        if (!pMemArea->IsCellPresent(CurAddr))
          if ((pMemArea = m_rDoc.GetMemoryArea(CurAddr)) == nullptr)
            throw std::string("unable to get memory area for address: ") + CurAddr.ToString();

        // If the current memory area is not executable, we skip this execution flow
        if (!(pMemArea->GetAccess() & MemoryArea::Execute))
          throw std::string("memory access \"") + pMemArea->GetName() + std::string("\" is not executable");

        if (m_rDoc.ContainsCode(CurAddr))
          return true;

        if (!m_rDoc.ContainsUnknown(CurAddr))
          throw std::string("cell at \"") + CurAddr.ToString() + std::string("\" is not unknown");

        // We create a new entry and disassemble it
        auto spInsn = std::make_shared<Instruction>();

        TOffset PhysicalOffset;

        if (!pMemArea->ConvertOffsetToFileOffset(CurAddr.GetOffset(), PhysicalOffset))
          throw std::string("unable to convert address ") + CurAddr.ToString() + std::string(" to offset");

        // If something bad happens, we quit
        if (!rArch.Disassemble(m_rDoc.GetBinaryStream(), PhysicalOffset, *spInsn, Mode))
          throw std::string("unable to disassemble instruction at ") + CurAddr.ToString();

        if (spInsn->GetLength() == 0)
          throw std::string("0 length instruction at ") + CurAddr.ToString();

        // We try to retrieve the current instruction, if it's true we go to the next function
        for (size_t InsnLen = 0; InsnLen < spInsn->GetLength(); ++InsnLen)
          if (m_rDoc.ContainsCode(CurAddr + InsnLen))
            return true;

        rBasicBlock.push_back(spInsn);

        auto OpType = spInsn->GetSubType();
        if (
          OpType & Instruction::JumpType
          || OpType & Instruction::CallType
          || OpType & Instruction::ReturnType)
        {
          return true;
        }

        CurAddr += spInsn->GetLength();
      } // !while (m_rDoc.IsPresent(CurAddr))
    }

    catch (std::string const& rExcpMsg)
    {
      rBasicBlock.clear();
      Log::Write("core").Level(LogDebug) << rExcpMsg << LogEnd;
      return false;
    }

    return false;

    //// At this point, we reach neither an basic block exit (jump, call, return) nor code,
    //// so if we must disassemble basic block only: we have to return false, otherwise it's safe
    //// to return true.
    //return spArch->DisassembleBasicBlockOnly() == false ? true : false;
  }

  bool AnalyzerDisassemble::BuildControlFlowGraph(ControlFlowGraph& rCfg)
  {
    std::stack<Address> CallStack;
    Address::List Addresses;
    typedef std::tuple<Address, Address, BasicBlockEdgeProperties::Type> TupleEdge;
    std::list<TupleEdge> Edges;
    std::map<Address, bool> VisitedInstruction;
    bool RetReached = false;

    Address CurAddr = m_Addr;

    MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

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
          return false;

        auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
        if (spArch == nullptr)
          return false;

        // If the current address is already visited
        if (VisitedInstruction[CurAddr])
        {
          // ... and if the current instruction is the end of the function, we take another address from the callstack
          if (spInsn->GetSubType() & Instruction::ReturnType && !(spInsn->GetSubType() & Instruction::ConditionalType))
            break;

          // if not, we try with the next address.
          CurAddr += spInsn->GetLength();
          continue;
        }

        Addresses.push_back(CurAddr);
        VisitedInstruction[CurAddr] = true;

        if (spInsn->GetSubType() & Instruction::JumpType)
        {
          Address DstAddr;


          if (expr_cast<MemoryExpression>(spInsn->GetOperand(0)) != nullptr)
            break;

          if (!spInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spInsn), DstAddr))
            break;

          if (spInsn->GetSubType() & Instruction::ConditionalType)
          {
            Address NextAddr = CurAddr + spInsn->GetLength();
            Edges.push_back(TupleEdge(DstAddr, CurAddr, BasicBlockEdgeProperties::True));
            Edges.push_back(TupleEdge(NextAddr, CurAddr, BasicBlockEdgeProperties::False));
            CallStack.push(NextAddr);
          }
          else
          {
            Edges.push_back(TupleEdge(DstAddr, CurAddr, BasicBlockEdgeProperties::Unconditional));
          }

          CurAddr = DstAddr;
          continue;
        }

        else if (spInsn->GetSubType() & Instruction::ReturnType && !(spInsn->GetSubType() & Instruction::ConditionalType))
        {
          RetReached = true;
          break;
        }

        CurAddr += spInsn->GetLength();
      } // end while (m_Document.IsPresent(CurAddr))
    } // while (!CallStack.empty())

    BasicBlockVertexProperties FirstBasicBlock(m_rDoc, Addresses);

    // This case is required because a jmp type insn can be located just before the entry point of a function,
    // thus nothing can split this part (which normally is done by the call <func>)
    Address::List FirstSplitAddrs;
    if (FirstBasicBlock.Split(m_Addr, FirstSplitAddrs))
      rCfg.AddBasicBlockVertex(BasicBlockVertexProperties(m_rDoc, FirstSplitAddrs));

    rCfg.AddBasicBlockVertex(FirstBasicBlock);

    for (auto itEdge = std::begin(Edges); itEdge != std::end(Edges); ++itEdge)
    {
      static const char *TypeStr[] =
      {
        "Unknown",
        "Unconditional",
        "True",
        "False"
      };
      bool Res = rCfg.SplitBasicBlock(std::get<0>(*itEdge), std::get<1>(*itEdge), std::get<2>(*itEdge));
      Log::Write("core") << "dst: " << std::get<0>(*itEdge) << ", src: " << std::get<1>(*itEdge) << ", type: " << TypeStr[std::get<2>(*itEdge)] << (Res ? ", succeed" : ", failed") << LogEnd;
    }

    for (auto itEdge = std::begin(Edges); itEdge != std::end(Edges); ++itEdge)
      rCfg.AddBasicBlockEdge(BasicBlockEdgeProperties(std::get<2>(*itEdge)), std::get<1>(*itEdge), std::get<0>(*itEdge));

    rCfg.Finalize(m_rDoc);

    return RetReached;
  }
}