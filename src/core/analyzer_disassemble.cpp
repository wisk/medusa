#include "medusa/analyzer.hpp"
#include "medusa/module.hpp"
#include "medusa/expression_visitor.hpp"
#include "medusa/graph.hpp"

namespace medusa
{
  bool AnalyzerDisassemble::DisassembleOneInstruction(Tag ArchTag, u8 ArchMode)
  {
    Address CurAddr = m_Addr;

    auto spInsn = std::make_shared<Instruction>();

    OffsetType PhysicalOffset;

    if (!m_rDoc.ConvertAddressToFileOffset(CurAddr, PhysicalOffset))
    {
      Log::Write("core").Level(LogWarning) << "unable to convert address: " << CurAddr << " to offset" << LogEnd;
      return false;
    }

    auto Tag = (ArchTag == MEDUSA_ARCH_UNK) ? m_rDoc.GetArchitectureTag(CurAddr) : ArchTag;
    auto spArch = ModuleManager::Instance().GetArchitecture(Tag);
    if (spArch == nullptr)
    {
      Log::Write("core").Level(LogWarning) << "unable to find architecture module for: " << CurAddr << LogEnd;
      return false;
    }

    u8 Mode = (ArchMode == 0) ? m_rDoc.GetMode(CurAddr) : ArchMode;

    // If something bad happens, we quit
    if (!spArch->Disassemble(m_rDoc.GetBinaryStream(), PhysicalOffset, *spInsn, Mode))
    {
      Log::Write("core").Level(LogWarning) << "unable to disassemble instruction at " << CurAddr << LogEnd;
      return false;
    }

    if (spInsn->GetSize() == 0)
    {
      Log::Write("core").Level(LogWarning) << "0 length instruction at " << CurAddr << LogEnd;
      return false;
    }

    if (!m_rDoc.SetCell(CurAddr, spInsn, true))
    {
      Log::Write("core").Level(LogWarning) << "failed to set cell at " << CurAddr << LogEnd;
      return false;
    }

    return true;
  }

  bool AnalyzerDisassemble::Disassemble(Tag ArchTag, u8 ArchMode)
  {
    Architecture::SPType spArch;

    if (ArchTag != MEDUSA_ARCH_UNK)
    {
      spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
      if (spArch == nullptr)
        return false;
    }

    // ArchMode value is handled in DisasembleBasicBlock

    auto Lbl = m_rDoc.GetLabelFromAddress(m_Addr);
    if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
      return true;

    std::stack<Address> CallStack;
    Address::Vector FuncAddr;
    Address CurAddr = m_Addr;

    // Push entry point
    CallStack.push(CurAddr);

    // Do we still have functions to disassemble?
    while (!CallStack.empty())
    {
      auto BbAddr = CallStack.top();
      auto CurAddr = BbAddr;
      CallStack.pop();
      bool FunctionIsFinished = false;

      // Disassemble a basic block
      while (m_rDoc.ContainsUnknown(CurAddr))
      {
        auto const& rLbl = m_rDoc.GetLabelFromAddress(CurAddr);
        if ((rLbl.GetType() & Label::AccessMask) == Label::Imported)
          break;

        // Let's try to disassemble a basic block
        AnalyzerDisassemble AnlzDisasm(m_rDoc, CurAddr);
        std::list<Instruction::SPType> BasicBlock;
        if (!AnlzDisasm.DisassembleBasicBlock(BasicBlock, ArchTag, ArchMode))
          break;
        if (BasicBlock.size() == 0)
          break;

        // Insert all instructions into database
        for (auto const& spInsn : BasicBlock)
        {
          if (!m_rDoc.SetCell(CurAddr, spInsn, true))
          {
            Log::Write("core").Level(LogError) << "Error while inserting instruction at " << CurAddr.ToString() << LogEnd;
            FunctionIsFinished = true;
            continue;
          }

          auto spArch = ModuleManager::Instance().GetArchitecture(spInsn->GetArchitectureTag());
          if (spArch == nullptr)
            return false;

          for (u8 i = 0; i < spInsn->GetNumberOfOperand(); ++i)
          {
            Address DstAddr;
            if (spInsn->GetOperandReference(m_rDoc, i, spArch->CurrentAddress(CurAddr, *spInsn), DstAddr))
              CallStack.push(DstAddr);
          }

          AnalyzerInstruction AnlzInsn(m_rDoc, CurAddr, *spInsn);
          AnlzInsn.FindCrossReference();
          CurAddr += spInsn->GetSize();
        }

        if (FunctionIsFinished)
          break;

        auto spLastInsn = BasicBlock.back();
        if (ArchTag == MEDUSA_ARCH_UNK)
        {
          spArch = ModuleManager::Instance().GetArchitecture(spLastInsn->GetArchitectureTag());
          if (spArch == nullptr)
            break;
        }

        auto NextAddr = CurAddr;
        CurAddr.SetOffset(CurAddr.GetOffset() - spLastInsn->GetSize()); // TODO(wisk): something more elegant...
        switch (spLastInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType | Instruction::ReturnType))
        {
          // If the last instruction is a call, we follow it and save the return address
        case Instruction::CallType:
        {
          Address DstAddr;

          // Save return address
          CallStack.push(NextAddr);

          // Sometimes, we cannot determine the destination address, so we give up
          // We assume destination is hold in the first operand
          if (!spLastInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spLastInsn), DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          FuncAddr.push_back(DstAddr);
          BbAddr = DstAddr;
          break;
        } // end CallType

        // If the last instruction is a ret, we emulate its behavior
        case Instruction::ReturnType:
        {
          // We ignore conditional ret
          if (spLastInsn->GetSubType() & Instruction::ConditionalType)
          {
            BbAddr = NextAddr;
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
            CallStack.push(NextAddr);

          // Sometime, we can't determine the destination address, so we give up
          if (!spLastInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spLastInsn), DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          BbAddr = DstAddr;
          break;
        } // end JumpType

        default: break; // This case should never happen
        } // switch (spLastInsn->GetSubType())

        if (FunctionIsFinished)
          break;
      } // !m_rDoc.ContainsUnknown(CurAddr)


    } // !while (!CallStack.empty())

    for (auto const& rFuncAddr : FuncAddr)
    {
      AnalyzerFunction AnlzFunc(m_rDoc, rFuncAddr);
      AnlzFunc.CreateFunction();
    }

    return true;
  }

  bool AnalyzerDisassemble::DisassembleBasicBlock(std::list<Instruction::SPType>& rBasicBlock, Tag ArchTag, u8 ArchMode)
  {
    // If the caller doesn't provide an architecture, we use the default one

    if (ArchTag == MEDUSA_ARCH_UNK)
      ArchTag = m_rDoc.GetArchitectureTag(m_Addr);

    auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
    if (spArch == nullptr)
    {
      Log::Write("core").Level(LogWarning) << "unable to load architecture module for " << m_Addr << LogEnd;
      return false;
    }

    // If the caller doesn't provide a mode, we use the default one
    if (ArchMode == 0)
      ArchMode = m_rDoc.GetMode(m_Addr);

    Address CurAddr = m_Addr;
    bool DisasmBscBlkOnly = true;

    // This variable contains the current error message
    std::string ErrMsg;

    auto const& rBinStrm = m_rDoc.GetBinaryStream();

    // Check if the current address is not an exported symbol
    auto Lbl = m_rDoc.GetLabelFromAddress(CurAddr);
    if (Lbl.IsImported())
    {
      Log::Write("core").Level(LogWarning) << "address " << CurAddr << " is imported code" << LogEnd;
      return false;
    }

    Instruction::SPType spInsn;

    do
    {
      // Allocate a new instruction
      spInsn = std::make_shared<Instruction>();

      // Convert address to binary stream offset
      OffsetType InsnOff;
      if (!m_rDoc.ConvertAddressToFileOffset(CurAddr, InsnOff))
      {
        ErrMsg = "failed to convert address " + CurAddr.ToString() + " to offset";
        break;
      }

      // Try to disassemble the current instruction
      if (!spArch->Disassemble(rBinStrm, InsnOff, *spInsn, ArchMode))
      {
        ErrMsg = "failed to disassemble instruction at " + CurAddr.ToString();
        break;
      }

      rBasicBlock.push_back(spInsn);

      CurAddr += spInsn->GetSize();


      // We continue to disassemble code until we already disassembled it
    } while (!(spInsn->GetSubType() & (Instruction::JumpType | Instruction::CallType | Instruction::ReturnType)));

    // If this variable is not empty, we had an error during the disassembling
    if (!ErrMsg.empty())
    {
      Log::Write("core").Level(LogError) << ErrMsg << LogEnd;
      return false;
    }

    return true;
  }

bool AnalyzerDisassemble::BuildControlFlowGraph(Graph& rCfg)
{
  std::map<Address, bool> VstAddrs;
  std::set<Address> SplitAddrs;
  std::stack<Address> CallStack;
  std::vector<std::tuple<Address /* source */, Address /* destination */, Graph::EdgeProperties::Type>> Edges;

  auto ArchTag = m_rDoc.GetArchitectureTag(m_Addr);
  auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
  if (spArch == nullptr)
  {
    Log::Write("core").Level(LogError) << "no architecture at: " << m_Addr << LogEnd;
    return false;
  }

  // Convert instruction sub type to graph steps
  CallStack.push(m_Addr);
  while (!CallStack.empty())
  {
    auto CurAddr = CallStack.top();
    CallStack.pop();

    bool EndOfBasicBlock = false;
    do
    {
      // Job is already done
      auto itVstAddr = VstAddrs.find(CurAddr);
      // Not yet visited
      if (itVstAddr != std::end(VstAddrs) && itVstAddr->second)
        break;

      // Try to retrieve the current instruction
      auto spInsn = std::static_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));
      if (spInsn == nullptr)
      {
        Log::Write("core").Level(LogWarning) << "no instruction at: " << CurAddr << LogEnd;
        continue;
      }

      // This lambda handle references during a call or jump
      auto HandleInstructionReference = [&](void) -> bool
      {
        // Try to get the destination
        Address DstAddr;
        if (!spInsn->GetOperandReference(m_rDoc, 0, spArch->CurrentAddress(CurAddr, *spInsn), DstAddr))
          return false;

        // We have to save the destination for later if required
        CallStack.push(DstAddr);
        if (spInsn->GetSubType() & Instruction::ConditionalType)
        {
          auto NxtAddr = CurAddr + spInsn->GetSize();
          Edges.emplace_back(CurAddr, DstAddr, Graph::EdgeProperties::True);
          Edges.emplace_back(CurAddr, NxtAddr, Graph::EdgeProperties::False);
          SplitAddrs.insert(NxtAddr);
        }
        else
          Edges.emplace_back(CurAddr, DstAddr, Graph::EdgeProperties::Unconditional);

        // Store split address
        SplitAddrs.insert(DstAddr);

        return true;
      };

      VstAddrs[CurAddr] = true;

      // We handle the instruction condition type seperatly
      switch (spInsn->GetSubType() & ~Instruction::ConditionalType)
      {
      default:
        break;

        // We don't follow the path for call
      case Instruction::CallType:
        break;

        // Jump / Conditional instruction types
      case Instruction::JumpType:
        if (!HandleInstructionReference())
          // Exit the basic block if we can't reach the destination statically
          EndOfBasicBlock = true;
        break;

        // Return / Conditional instruction types
      case Instruction::ReturnType:
        // TODO(wisk): handle conditional return
        EndOfBasicBlock = true;
        SplitAddrs.insert(CurAddr + spInsn->GetSize());
        break;

      } // !switch (spInsn->GetSubType() & ~Instruction::ConditionalType)

      // Go to the next instruction address
      CurAddr += spInsn->GetSize();

      // Until the end of the current basic block
    } while (!EndOfBasicBlock);


  } // !while (!CallStack.empty())

  auto itSplitAddr = std::begin(SplitAddrs);
  // If the first split and the first visited address are the same, we can safely skip it
  if (itSplitAddr != std::end(SplitAddrs) && *itSplitAddr == std::begin(VstAddrs)->first)
    ++itSplitAddr;
  std::vector<Address> CurBscBlk;
  Address PrevAddr;
  for (auto const& rVstAddr : VstAddrs)
  {
    if (!rVstAddr.second)
      continue;

    if (itSplitAddr != std::end(SplitAddrs) && *itSplitAddr == rVstAddr.first && !CurBscBlk.empty())
    {
      rCfg.AddVertex(Graph::VertexProperties(CurBscBlk));
      CurBscBlk.clear();
      ++itSplitAddr;
      Edges.emplace_back(PrevAddr, rVstAddr.first, Graph::EdgeProperties::Next);
    }

    CurBscBlk.push_back(rVstAddr.first);
    PrevAddr = rVstAddr.first;
  }

  if (!CurBscBlk.empty())
  {
    rCfg.AddVertex(Graph::VertexProperties(CurBscBlk));
    CurBscBlk.clear();
  }

  for (auto const& rEdge : Edges)
  {
    rCfg.AddEdge(std::get<2>(rEdge), std::get<0>(rEdge), std::get<1>(rEdge));
  }

  return true;
}

  typedef std::map<Expression::SPType, Expression::SPType> SymbolicContextType;
  typedef std::unordered_map<Address, SymbolicContextType> SymbolicContextMapType;

  bool AnalyzerDisassemble::DisassembleUsingSymbolicExecution(void)
  {
    auto Lbl = m_rDoc.GetLabelFromAddress(m_Addr);
    if (Lbl.IsImported())
      return true;

    Address::Vector Addresses;
    typedef std::tuple<Address, Address, Graph::EdgeProperties::Type> TupleEdge;
    std::vector<TupleEdge> Branches, AllBranches;
    std::unordered_map<Address, bool> VisitedAddresses;
    Address::Vector ExitAddrs;
    std::stack<std::tuple<SymbolicVisitor, Address>> CallStack;
    Address::Vector FuncAddr;
    Address CurAddr = m_Addr;
    std::map<Address, bool> VisitedInstruction;

    // Push entry point
    CallStack.push(std::make_tuple(SymbolicVisitor(m_rDoc, m_rDoc.GetMode(CurAddr)), CurAddr));

    while (!CallStack.empty())
    {
      auto SymVst = std::get<0>(CallStack.top());
      CurAddr = std::get<1>(CallStack.top());
      CallStack.pop();

      auto ArchTag = m_rDoc.GetArchitectureTag(CurAddr);
      auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
      if (spArch == nullptr)
      {
        Log::Write("core") << "no architecture for: " << CurAddr << LogEnd;
        continue;
      }
      auto Mode = m_rDoc.GetMode(CurAddr);
      if (Mode == 0)
      {
        Log::Write("core") << "no mode for: " << CurAddr << LogEnd;
        continue;
      }

      int NumOfPathFound;

      while (true)
      {
        if (VisitedAddresses[CurAddr])
          break;
        Addresses.push_back(CurAddr);
        VisitedAddresses[CurAddr] = true;

        OffsetType Off;
        m_rDoc.ConvertAddressToFileOffset(CurAddr, Off);
        auto spInsn = std::make_shared<Instruction>();

        if (!spArch->Disassemble(m_rDoc.GetBinaryStream(), Off, *spInsn, Mode))
        {
          Log::Write("core").Level(LogError) << "failed to disassemble instruction at: " << CurAddr << LogEnd;
          break;
        }

        if (!m_rDoc.SetCell(CurAddr, spInsn, true))
        {
          Log::Write("core").Level(LogError) << "failed to set instruction at: " << CurAddr << LogEnd;
          break;
        }

        auto InsnExprs = spInsn->GetSemantic();

        // If we don't have semantic, we're gonna ignore this instruction
        if (InsnExprs.empty())
        {
          Log::Write("core").Level(LogWarning) << "no semantic for instruction: " << spInsn->GetName() << LogEnd;
          CurAddr += spInsn->GetSize();
          continue;
        }

        if (!SymVst.UpdateAddress(*spArch, CurAddr))
        {
          return false;
        }
        for (auto Expr : InsnExprs)
        {
          Expr->Visit(&SymVst);
        }

        //Log::Write("dbg") << SymVst.ToString() << LogEnd;

        Address DstAddr;
        bool EndOfBlk = false;

        if (!SymVst.FindAllPaths(NumOfPathFound, *spArch, [&](Expression::SPType spDstExpr, Expression::VSPType spCondExprs)
        {
          //Log::Write("dbg") << "DST: " << spDstExpr->ToString();
          std::string CondExprStr;
          for (auto spCondExpr : spCondExprs)
          {
            //Log::Write("dbg") << ", COND: " << spCondExpr->ToString();
            CondExprStr += spCondExpr->ToString();
            CondExprStr += " ; ";
          }
          //Log::Write("dbg") << LogEnd;

          // NOTE(wisk): Ignore symbolic branches
          // LATER(wisk): We may want to undefine some registers according the calling convention and fix the stack if needed
          // FIXME(wisk): It won't work with code like: cond_branch <extfunc>
          if (auto spSymExpr = expr_cast<SymbolicExpression>(spDstExpr))
          {
            m_rDoc.SetComment(CurAddr, spDstExpr->ToString());
            DstAddr = CurAddr + spInsn->GetSize();
            //CallStack.push(std::make_tuple(SymVst.Fork(), DstAddr));
            //Branches.push_back(std::make_tuple(CurAddr, DstAddr, Graph::EdgeProperties::Next));
          }

          else if (auto spBvExpr = expr_cast<BitVectorExpression>(spDstExpr))
          {
            // LATER(wisk): The way this address is made is not fully generic
            DstAddr = CurAddr;
            DstAddr.SetOffset(spBvExpr->GetInt().ConvertTo<OffsetType>());
            m_rDoc.SetComment(DstAddr, CondExprStr);

            // FIXME(wisk): we have to cheat here because there's not way to distinct next insn from direct uncond jump
            if (spInsn->GetSubType() & Instruction::JumpType)
            {
              CallStack.push(std::make_tuple(SymVst.Fork(), DstAddr));
              Branches.push_back(std::make_tuple(DstAddr, CurAddr, Graph::EdgeProperties::Unknown));
              EndOfBlk = true;
            }
          }

          else
          {
            Log::Write("core").Level(LogWarning) << "unknown destination type: " << CurAddr << LogEnd;
          }
        }))
        {
          Log::Write("core").Level(LogWarning) << "unknown destination: " << CurAddr << LogEnd;
          ExitAddrs.push_back(CurAddr);
          break; // We need to exit the loop to find another branches
        }

        if (!EndOfBlk)
        {
          CurAddr = DstAddr;
          continue;
        }

        break;
      }

      switch (Branches.size())
      {
      case 0:
        ExitAddrs.push_back(CurAddr);
        break;

      case 1: // 1 path means it's a simple branch
        //Log::Write("dbg") << "it's a simple branch" << LogEnd;
        std::get<2>(Branches[0]) = Graph::EdgeProperties::Unconditional;
        break;

      case 2: // 2 paths mean it's a conditional branch
        //Log::Write("dbg") << "it's a conditional branch" << LogEnd;
        std::get<2>(Branches[0]) = Graph::EdgeProperties::True;
        std::get<2>(Branches[1]) = Graph::EdgeProperties::False;
        break;

      default: // default means it's a multiple branch (branch table)
        //Log::Write("dbg") << "it's a multiple branch" << LogEnd;
        for (auto i = 0UL; i < Branches.size(); ++i)
          std::get<2>(Branches[i]) = Graph::EdgeProperties::Multiple;
        break;
      }

      AllBranches.insert(std::end(AllBranches), std::begin(Branches), std::end(Branches));
      Branches.clear();

      // NOTE(wisk): If we find new paths, we have to exit this loop so we
      // can use multiple symbolic context
      //if (NumOfPathFound != 0)
      //  break;
    }

    // Now we can build the control flow graph
    Graph::SPType spCfg = std::make_shared<Graph>();
    Graph::VertexProperties FirstBasicBlock(Addresses);

    // This case is required because a jmp type insn can be located just before the entry point of a function,
    // thus nothing can split this part (which normally is done by the call <func>)
    Address::Vector FirstSplitAddrs;
    if (FirstBasicBlock.Split(m_Addr, FirstSplitAddrs))
    {
      Address PrevAddr;
      FirstBasicBlock.GetLastAddress(PrevAddr);
      spCfg->AddEdge(Graph::EdgeProperties(Graph::EdgeProperties::Next), PrevAddr, FirstSplitAddrs.front());
      spCfg->AddVertex(Graph::VertexProperties(FirstSplitAddrs));
    }

    spCfg->AddVertex(FirstBasicBlock);

    decltype(AllBranches) NextBranches;
    for (auto const& rBranch : AllBranches)
    {
      static const char *TypeStr[] =
      {
        "Unknown",
        "Unconditional",
        "True",
        "False",
        "Next",
        "Multiple",
      };
      auto const& rDstAddr = std::get<0>(rBranch);
      auto const& rSrcAddr = std::get<1>(rBranch);
      Address     PrevAddr;
      auto Edge            = std::get<2>(rBranch);
      bool SameVertex = false;
      Graph::VertexDescriptor DstVtxDesc;
      Graph::VertexDescriptor SrcVtxDesc;
      if (spCfg->FindVertex(rDstAddr, DstVtxDesc) && spCfg->FindVertex(rSrcAddr, SrcVtxDesc))
        if (DstVtxDesc == SrcVtxDesc)
          SameVertex = true;
      bool Res = spCfg->SplitVertex(rDstAddr, rSrcAddr, Edge, &PrevAddr);
      Log::Write("core") << "dst: " << rDstAddr << ", src: " << rSrcAddr << ", type: " << TypeStr[Edge] << (Res ? ", succeed" : ", failed") << LogEnd;
      if (SameVertex)
      {
        NextBranches.push_back(std::make_tuple(PrevAddr, rDstAddr, Graph::EdgeProperties::Next));
      }
    }

    auto& rBoostGraph = (*spCfg)();

    for (auto const& rBranch : AllBranches)
      spCfg->AddEdge(Graph::EdgeProperties(std::get<2>(rBranch)), std::get<1>(rBranch), std::get<0>(rBranch));
    for (auto const& rBranch : NextBranches)
    {
      Graph::VertexDescriptor PrevVtxDesc;

      if (!spCfg->FindVertex(std::get<0>(rBranch), PrevVtxDesc))
        continue;
      auto& rPrevVtxProp = rBoostGraph[PrevVtxDesc];
      Address LastAddr;
      if (!rPrevVtxProp.GetLastAddress(LastAddr))
        continue;
      if (std::find(std::begin(ExitAddrs), std::end(ExitAddrs), LastAddr) != std::end(ExitAddrs))
        continue;
      spCfg->AddEdge(Graph::EdgeProperties(std::get<2>(rBranch)), std::get<1>(rBranch), std::get<0>(rBranch));
    }

    Graph::VertexDescriptor VtxHead;
    if (spCfg->FindVertex(m_Addr, VtxHead))
    {
      auto& rVtxProp = rBoostGraph[VtxHead];
      rVtxProp.MarkAsHead();
    }

    for (auto const& rAddr : ExitAddrs)
    {
      Graph::VertexDescriptor VtxExit;
      if (!spCfg->FindVertex(rAddr, VtxExit))
        continue;
      auto& rVtxProp = rBoostGraph[VtxExit];
      rVtxProp.MarkAsEnd();
    }

    auto spFunc = m_rDoc.GetMultiCell(m_Addr);
    if (spFunc == nullptr)
    {
      Label FuncLbl(m_Addr, Label::Function | Label::Global);
      spFunc = std::make_shared<Function>(0, 0); // TODO(wisk): set the good size and instruction count
    }

    spFunc->SetGraph(spCfg);
    m_rDoc.SetMultiCell(m_Addr, spFunc);

    return true;
  }
}
