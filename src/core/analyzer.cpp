#include "medusa/analyzer.hpp"

#include "medusa/function.hpp"
#include "medusa/character.hpp"
#include "medusa/string.hpp"
#include "medusa/label.hpp"
#include "medusa/log.hpp"

#include <list>
#include <stack>

#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

// bool Analyzer::DisassembleFollowingExecutionPath(Database const& rDb, Architecture& rArch, Address const& rAddr, std::list<Instruction*>& rBasicBlock)
void Analyzer::DisassembleFollowingExecutionPath(Database& rDb, Address const& rEntrypoint, Architecture& rArch) const
{
  std::stack<Address> CallStack;
  Address CurAddr             = rEntrypoint;
  MemoryArea const* pMemArea  = rDb.GetMemoryArea(CurAddr);

  if (pMemArea == NULL)
  {
    Log::Write("core") << "Unable to get memory area for address " << CurAddr.ToString() << LogEnd;
    return;
  }

  // Push entry point
  CallStack.push(CurAddr);

  // Do we still have functions to disassemble ?
  while (!CallStack.empty())
  {
    // Retrieve the last function
    CurAddr = CallStack.top();
    CallStack.pop();
    bool FunctionIsFinished = false;

    //Log::Write("debug") << "Analyzing address: " << CurAddr.ToString() << LogEnd;

    // Disassemble a function
    while (rDb.IsPresent(CurAddr) && rDb.ContainsCode(CurAddr) == false)
    {
      Log::Write("debug") << "Disassembling basic block at " << CurAddr.ToString() << LogEnd;

      // Let's try to disassemble a basic block
      std::list<Instruction*> BasicBlock;
      if (!DisassembleBasicBlock(rDb, rArch, CurAddr, BasicBlock)) break;
      if (BasicBlock.size() == 0)                                  break;

      for (auto itInsn = std::begin(BasicBlock); itInsn != std::end(BasicBlock); ++itInsn)
      {
        if (rDb.ContainsCode(CurAddr))
        {
          //Log::Write("debug") << "Instruction is already disassembled at " << CurAddr.ToString() << LogEnd;
          FunctionIsFinished = true;
          break;
        }
        if (!rDb.InsertCell(CurAddr, *itInsn, true))
        {
          //Log::Write("core") << "Error while inserting instruction at " << CurAddr.ToString() << LogEnd;
          FunctionIsFinished = true;
          break;
        }

        // LATER: Quick fix
        if ((*itInsn)->GetOperationType() == Instruction::OpUnknown)
          CurAddr += (*itInsn)->GetLength();
      }

      if (FunctionIsFinished == true) break;

      auto pLastInsn = BasicBlock.back(); // LATER: 'it' not 'p'
      //Log::Write("debug") << "Last insn: " << pLastInsn->ToString() << LogEnd;

      switch  (pLastInsn->GetOperationType())
      {
        // If the last instruction is a call, we follow it and save the return address
        case Instruction::OpCall:
          {
            Address DstAddr;

            // Save return address
            CallStack.push(CurAddr + pLastInsn->GetLength());

            // Sometimes, we cannot determine the destination address, so we give up
            // We assume destination is hold in the first operand
            if (!pLastInsn->GetOperandReference(rDb, 0, CurAddr, DstAddr))
            {
              FunctionIsFinished = true;
              break;
            }

            CurAddr = DstAddr;
            break;
          } // end OpCall

        // If the last instruction is a ret, we emulate its behavior
        case Instruction::OpRet:
          {
            // We ignore conditional ret
            if (pLastInsn->Cond() != C_NONE)
            {
              CurAddr += pLastInsn->GetLength();
              continue;
            }

            // ret if reached, we try to disassemble an another function (or another part of this function)
            FunctionIsFinished = true;
            break;
          } // end OpRet

        // Jump type could be a bit tedious to handle because of conditional jump
        // Basically we use the same policy as call instruction
        case Instruction::OpJump:
          {
            Address DstAddr;

            // Save untaken branch address
            if (pLastInsn->Cond() != C_NONE)
              CallStack.push(CurAddr + pLastInsn->GetLength());

            // Sometime, we can't determine the destination address, so we give up
            if (!pLastInsn->GetOperandReference(rDb, 0, CurAddr, DstAddr))
            {
              FunctionIsFinished = true;
              break;
            }

            CurAddr = DstAddr;
            break;
          } // end OpJump

        default: break; // This case should never happen
      } // switch (pLastInsn->GetOperationType())

      if (FunctionIsFinished == true) break;
    } // end while (m_Database.IsPresent(CurAddr))
  } // while (!CallStack.empty())
}

void Analyzer::CreateXRefs(Database& rDb) const
{
  for (Database::TIterator itMemArea = rDb.Begin(); itMemArea != rDb.End(); ++itMemArea)
  {
    if (!((*itMemArea)->GetAccess() & MA_EXEC))
      continue;

    for (MemoryArea::TIterator itCell = (*itMemArea)->Begin(); itCell != (*itMemArea)->End(); ++itCell)
    {
      if (itCell->second == NULL) continue;

      if (itCell->second->GetType() == CellData::InstructionType)
      {
        Instruction* pInsn = static_cast<Instruction*>(itCell->second);

        for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
        {
          Address::SharedPtr CurAddr = (*itMemArea)->MakeAddress(itCell->first);
          Address DstAddr;
          if (!pInsn->GetOperandReference(rDb, CurOp, *CurAddr, DstAddr))
            continue;

          rDb.ChangeValueSize(DstAddr, pInsn->GetOperandReferenceLength(CurOp), true);

          // Check if the destination is valid and is an instruction
          Cell* pDstCell = rDb.RetrieveCell(DstAddr);
          if (pDstCell == NULL) continue;

          // Add XRef
          Address OpAddr;
          if (!pInsn->GetOperandAddress(CurOp, *CurAddr, OpAddr))
            OpAddr = *CurAddr;
          rDb.GetXRefs().AddXRef(DstAddr, OpAddr);
          typedef Database::View::LineInformation LineInformation;
          rDb.GetView().AddLineInformation(LineInformation(LineInformation::XrefLineType, DstAddr));

          // If the destination has already a label, we skip it
          if (!rDb.GetLabelFromAddress(DstAddr).GetName().empty())
            continue;

          std::string SuffixName = DstAddr.ToString();
          std::replace(SuffixName.begin(), SuffixName.end(), ':', '_');

          switch (pInsn->GetOperationType())
          {
          case Instruction::OpCall:
            {
              rDb.AddLabel(DstAddr, Label(m_FunctionPrefix + SuffixName, Label::LabelCode));
              Address FuncEnd;
              u16 FuncLen;
              u16 InsnCnt;
              if (ComputeFunctionLength(rDb, DstAddr, FuncEnd, FuncLen, InsnCnt, 0x1000) == true)
              {
                Log::Write("core")
                  << "Function found"
                  << ": address="               << DstAddr.ToString()
                  << ", length="                << FuncLen
                  << ", instruction counter: "  << InsnCnt
                  << LogEnd;

                Function* pFunction = new Function(FuncLen, InsnCnt);
                rDb.InsertMultiCell(DstAddr, pFunction, false);
              }
              break;
            }

          case Instruction::OpJump:
            rDb.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::LabelCode));
            break;

          case Instruction::OpUnknown:
            if (rDb.GetMemoryArea(DstAddr)->GetAccess() & MA_EXEC)
              rDb.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::LabelCode));
            else
              rDb.AddLabel(DstAddr, Label(m_DataPrefix + SuffixName, Label::LabelData));

          default: break;
          } // switch (pInsn->GetOperationType())
        } // for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
      } // if (itCell->second->GetType() == Cell::InstructionType)
    } // for (MemoryArea::TIterator itCell = (*itMemArea)->Begin(); itCell != (*itMemArea)->End(); ++itCell)
  } // for (Database::TIterator itMemArea = rDb.Begin(); itMemArea != rDb.End(); ++itMemArea)
}

bool Analyzer::ComputeFunctionLength(
    Database const& rDb,
    Address const& rFunctionAddress,
    Address& EndAddress,
    u16& rFunctionLength,
    u16& rInstructionCounter,
    u32 LengthThreshold) const
{
  std::stack<Address> CallStack;
  std::map<Address, bool> VisitedInstruction;
  bool RetReached = false;

  u32 FuncLen                = 0x0;
  Address CurAddr            = rFunctionAddress;
  Address EndAddr            = rFunctionAddress;
  rFunctionLength            = 0x0;
  rInstructionCounter        = 0x0;
  MemoryArea const* pMemArea = rDb.GetMemoryArea(CurAddr);

  if (pMemArea == NULL)
    return false;

  CallStack.push(CurAddr);

  while (!CallStack.empty())
  {
    CurAddr = CallStack.top();
    CallStack.pop();

    while (rDb.ContainsCode(CurAddr))
    {
      Instruction const& rInsn = *static_cast<Instruction const*>(rDb.RetrieveCell(CurAddr));

      if (VisitedInstruction[CurAddr])
      {
        CurAddr += rInsn.GetLength();
        continue;
      }

      VisitedInstruction[CurAddr] = true;

      rFunctionLength += static_cast<u32>(rInsn.GetLength());
      rInstructionCounter++;

      if (rInsn.GetOperationType() == Instruction::OpJump)
      {
        Address DstAddr;

        if (rInsn.GetCond() != C_NONE)
          CallStack.push(CurAddr + rInsn.GetLength());

        if (rInsn.Operand(0)->GetType() & O_MEM)
          break;

        if (!rInsn.GetOperandReference(rDb, 0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
        continue;
      }

      else if (rInsn.GetOperationType() == Instruction::OpRet && rInsn.GetCond() == C_NONE)
      {
        RetReached = true;
        if (EndAddr < CurAddr)
          EndAddr = CurAddr;
        break;
      }

      CurAddr += rInsn.GetLength();

      if (LengthThreshold && FuncLen > LengthThreshold)
        return false;
    } // end while (m_Database.IsPresent(CurAddr))
  } // while (!CallStack.empty())

  return RetReached;
}

void Analyzer::FindStrings(Database& rDb, Architecture& rArch) const
{
  Database::TLabelMap const& rLabels = rDb.GetLabels();
  for (Database::TLabelMap::const_iterator It = rLabels.begin();
      It != rLabels.end(); ++It)
  {
    if (It->right.GetType() != Label::LabelData)
      continue;

    u8 CurChar;
    std::string CurString        = "";
    MemoryArea const* pMemArea   = rDb.GetMemoryArea(It->left);
    BinaryStream const& rBinStrm = pMemArea->GetBinaryStream();
    TOffset PhysicalOffset;

    if (pMemArea == NULL)
      continue;

    if (pMemArea->Convert(It->left.GetOffset(), PhysicalOffset) == false)
      continue;

    /* ATM we look only for ASCII strings */
    AsciiString AsciiStr;
    u16 StrLen = 0;

    try
    {
      while (true)
      {
        rBinStrm.Read(PhysicalOffset, CurChar);
        if (!AsciiStr.IsValidCharacter(CurChar))
          break;
        CurString += CurChar;
        PhysicalOffset++;
        StrLen++;
      }
    }
    catch (Exception&) { continue; }

    if (AsciiStr.IsFinalCharacter(CurChar) && !CurString.empty())
    {
      Log::Write("core") << "Found string: " << CurString << LogEnd;
      String *pString = new String(CurString);
      rDb.InsertCell(It->left, pString, true, true);
      rDb.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::LabelString));
    }
  }
}

bool Analyzer::BuildControlFlowGraph(Database& rDb, std::string const& rLblName, ControlFlowGraph& rCfg)
{
  Address const& rLblAddr = rDb.GetAddressFromLabelName(rLblName);
  if (rLblAddr.GetAddressingType() == Address::UnknownType) return false;

  return BuildControlFlowGraph(rDb, rLblAddr, rCfg);
}

bool Analyzer::BuildControlFlowGraph(Database& rDb, Address const& rAddr, ControlFlowGraph& rCfg)
{
  std::stack<Address> CallStack;
  Address::List Addresses;
  typedef std::tuple<Address, Address, BasicBlockEdgeProperties::Type> TupleEdge;
  std::list<TupleEdge> Edges;
  std::map<Address, bool> VisitedInstruction;
  bool RetReached = false;

  Address CurAddr = rAddr;

  MemoryArea const* pMemArea = rDb.GetMemoryArea(CurAddr);

  if (pMemArea == NULL)
    return false;

  CallStack.push(CurAddr);

  while (!CallStack.empty())
  {
    CurAddr = CallStack.top();
    CallStack.pop();

    while (rDb.ContainsCode(CurAddr))
    {
      Instruction const& rInsn = *static_cast<Instruction const*>(rDb.RetrieveCell(CurAddr));

      if (VisitedInstruction[CurAddr])
      {
        CurAddr += rInsn.GetLength();
        continue;
      }

      Addresses.push_back(CurAddr);
      VisitedInstruction[CurAddr] = true;

      if (rInsn.GetOperationType() == Instruction::OpJump)
      {
        Address DstAddr;

        if (rInsn.Operand(0)->GetType() & O_MEM)
          break;

        if (!rInsn.GetOperandReference(rDb, 0, CurAddr, DstAddr))
          break;

        if (rInsn.GetCond() != C_NONE)
        {
          Address NextAddr = CurAddr + rInsn.GetLength();
          Edges.push_back(TupleEdge(DstAddr, CurAddr,  BasicBlockEdgeProperties::True ));
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

      else if (rInsn.GetOperationType() == Instruction::OpRet && rInsn.GetCond() == C_NONE)
      {
        RetReached = true;
        break;
      }

      CurAddr += rInsn.GetLength();
    } // end while (m_Database.IsPresent(CurAddr))
  } // while (!CallStack.empty())

  BasicBlockVertexProperties FirstBasicBlock(Addresses);
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

  rCfg.Finalize(rDb);

  return RetReached;
}

bool Analyzer::DisassembleBasicBlock(Database const& rDb, Architecture& rArch, Address const& rAddr, std::list<Instruction*>& rBasicBlock)
{
  Address CurAddr = rAddr;
  MemoryArea const* pMemArea = rDb.GetMemoryArea(CurAddr);
  bool Res = false;

  if (pMemArea == NULL)
    goto exit;

  while (rDb.IsPresent(CurAddr))
  {
    // If we changed the current memory area, we must update it
    if (!pMemArea->IsPresent(CurAddr))
      if ((pMemArea = rDb.GetMemoryArea(CurAddr)) == NULL)
        goto exit;

    // If the current memory area is not executable, we skip this execution flow
    if (!(pMemArea->GetAccess() & MA_EXEC))
      goto exit;

    if (rDb.RetrieveCell(CurAddr) == NULL)
      goto exit;

    // We try to retrieve the current instruction, if it's true we go to the next function
    if (rDb.ContainsCode(CurAddr))
    {
      Res = true;
      goto exit;
    }

    // We create a new entry and disassemble it
    Instruction* pInsn = new Instruction;

    try
    {
      TOffset PhysicalOffset;

      PhysicalOffset = CurAddr.GetOffset() - pMemArea->GetVirtualBase().GetOffset();

      // If something bad happens, we skip this instruction and go to the next function
      if (!rArch.Disassemble(pMemArea->GetBinaryStream(), PhysicalOffset, *pInsn))
      {
        delete pInsn;
        goto exit;
      }
    }
    catch (Exception const& e)
    {
      Log::Write("core")
        << "Exception while disassemble instruction at " << CurAddr.ToString()
        << ", reason: " << e.What()
        << LogEnd;
      delete pInsn;
      goto exit;
    }

    rBasicBlock.push_back(pInsn);

    auto OpType = pInsn->GetOperationType();
    if (
        OpType == Instruction::OpJump
        || OpType == Instruction::OpCall
        || OpType == Instruction::OpRet)
    {
      Res = true;
      goto exit;
    }

    CurAddr += pInsn->GetLength();
  } // !while (rDb.IsPresent(CurAddr))

exit:
  if (Res == false)
  {
    for (auto itInsn = std::begin(rBasicBlock); itInsn != std::end(rBasicBlock); ++itInsn)
      delete *itInsn;
    // LATER: empty rBasicBlock?
  }
  return Res;
}

MEDUSA_NAMESPACE_END