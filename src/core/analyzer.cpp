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
  boost::lock_guard<boost::mutex> Lock(m_DisasmMutex);

  auto Lbl = rDb.GetLabelFromAddress(rEntrypoint);
  if (Lbl.GetType() & Label::LabelImported)
    return;

  std::stack<Address> CallStack;
  Address CurAddr             = rEntrypoint;
  MemoryArea const* pMemArea  = rDb.GetMemoryArea(CurAddr);

  if (pMemArea == nullptr)
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
      //Log::Write("debug") << "Disassembling basic block at " << CurAddr.ToString() << LogEnd;

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
          delete *itInsn;
          *itInsn = nullptr;
          continue;
        }
        if (!rDb.InsertCell(CurAddr, *itInsn, true))
        {
          //Log::Write("core") << "Error while inserting instruction at " << CurAddr.ToString() << LogEnd;
          FunctionIsFinished = true;
          delete *itInsn;
          *itInsn = nullptr;
          continue;
        }

        for (u8 i = 0; i < OPERAND_NO; ++i)
        {
          Address DstAddr;
          if ((*itInsn)->GetOperandReference(rDb, i, CurAddr, DstAddr))
            CallStack.push(DstAddr);
        }

        auto InsnType = (*itInsn)->GetOperationType();
        if (InsnType == Instruction::OpUnknown || InsnType == Instruction::OpCond)
          CurAddr += (*itInsn)->GetLength();
      }

      if (FunctionIsFinished == true) break;

      auto pLastInsn = BasicBlock.back();
      //Log::Write("debug") << "Last insn: " << pLastInsn->ToString() << LogEnd;

      switch  (pLastInsn->GetOperationType() & (Instruction::OpCall | Instruction::OpJump | Instruction::OpRet))
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
          if (pLastInsn->GetOperationType() & Instruction::OpCond)
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
          if (pLastInsn->GetOperationType() & Instruction::OpCond)
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
      if (itCell->second == nullptr) continue;

      if (itCell->second->GetType() == CellData::InstructionType)
      {
        Instruction* pInsn = static_cast<Instruction*>(itCell->second);

        for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
        {
          Address CurAddr = (*itMemArea)->MakeAddress(itCell->first);
          Address DstAddr;
          if (!pInsn->GetOperandReference(rDb, CurOp, CurAddr, DstAddr))
            continue;

          rDb.ChangeValueSize(DstAddr, pInsn->GetOperandReferenceLength(CurOp), false);

          // Check if the destination is valid and is an instruction
          Cell* pDstCell = rDb.RetrieveCell(DstAddr);
          if (pDstCell == nullptr) continue;

          // Add XRef
          Address OpAddr;
          if (!pInsn->GetOperandAddress(CurOp, CurAddr, OpAddr))
            OpAddr = CurAddr;
          rDb.GetXRefs().AddXRef(DstAddr, OpAddr);
          typedef View::LineInformation LineInformation;
          rDb.GetView().AddLineInformation(LineInformation(LineInformation::XrefLineType, DstAddr));

          // If the destination has already a label, we skip it
          if (!rDb.GetLabelFromAddress(DstAddr).GetName().empty())
            continue;

          std::string SuffixName = DstAddr.ToString();
          std::replace(SuffixName.begin(), SuffixName.end(), ':', '_');

          switch (pInsn->GetOperationType() & (Instruction::OpCall | Instruction::OpJump))
          {
          case Instruction::OpCall:
            {
              Address FuncEnd;
              u16 FuncLen;
              u16 InsnCnt;
              std::string FuncName = m_FunctionPrefix + SuffixName;

              if (ComputeFunctionLength(rDb, DstAddr, FuncEnd, FuncLen, InsnCnt, 0x1000) == true)
              {
                Log::Write("core")
                  << "Function found"
                  << ": address="               << DstAddr.ToString()
                  << ", length="                << FuncLen
                  << ", instruction counter: "  << InsnCnt
                  << LogEnd;

                ControlFlowGraph Cfg;
                if (BuildControlFlowGraph(rDb, DstAddr, Cfg) == false)
                {
                  Log::Write("core")
                    << "Unable to build control flow graph for " << DstAddr.ToString() << LogEnd;
                  break;
                }

                Function* pFunction = new Function(FuncLen, InsnCnt, Cfg);
                rDb.InsertMultiCell(DstAddr, pFunction, false);
              }
              else
              {
                auto spArch = GetArchitecture(pInsn->GetArchitectureTag());
                auto pFuncInsn = static_cast<Instruction const*>(GetCell(rDb, (*itMemArea)->GetBinaryStream(), DstAddr));
                if (pFuncInsn->GetOperationType() != Instruction::OpJump)
                  break;
                Address OpRefAddr;
                if (pFuncInsn->GetOperandReference(rDb, 0, DstAddr, OpRefAddr) == false)
                  break;
                auto OpLbl = rDb.GetLabelFromAddress(OpRefAddr);
                if (OpLbl.GetType() == Label::LabelUnknown)
                  break;
                FuncName = std::string(pFuncInsn->GetName()) + std::string("_") + OpLbl.GetLabel();
              }

              rDb.AddLabel(DstAddr, Label(FuncName, Label::LabelCode));
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
          } // switch (pInsn->GetOperationType() & (Instruction::OpCall | Instruction::OpJump))
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

  auto Lbl = rDb.GetLabelFromAddress(rFunctionAddress);
  if (Lbl.GetType() & Label::LabelImported)
    return false;

  if (pMemArea == nullptr)
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

      FuncLen += static_cast<u32>(rInsn.GetLength());

      VisitedInstruction[CurAddr] = true;

      rFunctionLength += static_cast<u32>(rInsn.GetLength());
      rInstructionCounter++;

      if (rInsn.GetOperationType() & Instruction::OpJump)
      {
        Address DstAddr;

        if (rInsn.GetOperationType() & Instruction::OpCond)
          CallStack.push(CurAddr + rInsn.GetLength());

        if (rInsn.Operand(0)->GetType() & O_MEM)
          break;

        if (!rInsn.GetOperandReference(rDb, 0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
        continue;
      }

      else if (rInsn.GetOperationType() & Instruction::OpRet && !(rInsn.GetOperationType() & Instruction::OpCond))
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

    MemoryArea const* pMemArea   = rDb.GetMemoryArea(It->left);
    if (pMemArea == nullptr)
      continue;

    std::string CurString        = "";
    BinaryStream const& rBinStrm = pMemArea->GetBinaryStream();
    TOffset PhysicalOffset;

    if (pMemArea->Convert(It->left.GetOffset(), PhysicalOffset) == false)
      continue;

    /* UTF-16 */
    WinString WinStr;
    WinString::CharType WinChar;
    CurString = "";

    try
    {
      while (true)
      {
        rBinStrm.Read(PhysicalOffset, WinChar);
        if (!WinStr.IsValidCharacter(WinChar))
          break;
        CurString += WinStr.ConvertToUf8(WinChar);
        PhysicalOffset += sizeof(WinChar);
      }
    }
    catch (Exception&) { CurString = ""; }

    if (WinStr.IsFinalCharacter(WinChar) && !CurString.empty())
    {
      Log::Write("core") << "Found string: " << CurString << LogEnd;
      String *pString = new String(String::Utf16Type, CurString);
      rDb.InsertCell(It->left, pString, true, true);
      rDb.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::LabelString));
      continue;
    }

    // LATER: Redo
    /* ASCII */
    AsciiString AsciiStr;
    AsciiString::CharType AsciiChar;

    try
    {
      while (true)
      {
        rBinStrm.Read(PhysicalOffset, AsciiChar);
        if (!AsciiStr.IsValidCharacter(AsciiChar))
          break;
        CurString += AsciiStr.ConvertToUf8(AsciiChar);
        PhysicalOffset += sizeof(AsciiChar);
      }
    }
    catch (Exception&) { CurString = ""; }

    if (AsciiStr.IsFinalCharacter(AsciiChar) && !CurString.empty())
    {
      Log::Write("core") << "Found string: " << CurString << LogEnd;
      String *pString = new String(String::AsciiType, CurString);
      rDb.InsertCell(It->left, pString, true, true);
      rDb.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::LabelString));
    }
  }
}

bool Analyzer::CreateFunction(Database& rDb, Address const& rAddr)
{
  std::string SuffixName = rAddr.ToString();
  std::replace(SuffixName.begin(), SuffixName.end(), ':', '_');
  Address FuncEnd;
  u16 FuncLen;
  u16 InsnCnt;
  std::string FuncName = m_FunctionPrefix + SuffixName;

  if (ComputeFunctionLength(rDb, rAddr, FuncEnd, FuncLen, InsnCnt, 0x1000) == true)
  {
    Log::Write("core")
      << "Function found"
      << ": address="               << rAddr.ToString()
      << ", length="                << FuncLen
      << ", instruction counter: "  << InsnCnt
      << LogEnd;

    ControlFlowGraph Cfg;
    if (BuildControlFlowGraph(rDb, rAddr, Cfg) == false)
    {
      Log::Write("core")
        << "Unable to build control flow graph for " << rAddr.ToString() << LogEnd;
      return false;
    }

    Function* pFunction = new Function(FuncLen, InsnCnt, Cfg);
    rDb.InsertMultiCell(rAddr, pFunction, false);
  }
  else
  {
    auto pMemArea = rDb.GetMemoryArea(rAddr);
    if (pMemArea == nullptr)
      return false;
    auto rBinStrm = pMemArea->GetBinaryStream();
    auto pInsn = GetCell(rDb, rBinStrm, rAddr);
    if (pInsn == nullptr)
      return false;
    auto spArch = GetArchitecture(pInsn->GetArchitectureTag());
    auto pFuncInsn = static_cast<Instruction const*>(GetCell(rDb, rBinStrm, rAddr));
    if (pFuncInsn->GetOperationType() != Instruction::OpJump)
      return false;
    Address OpRefAddr;
    if (pFuncInsn->GetOperandReference(rDb, 0, rAddr, OpRefAddr) == false)
      return false;
    auto OpLbl = rDb.GetLabelFromAddress(OpRefAddr);
    if (OpLbl.GetType() == Label::LabelUnknown)
      return false;
    FuncName = std::string(pFuncInsn->GetName()) + std::string("_") + OpLbl.GetLabel();
  }

  rDb.AddLabel(rAddr, Label(FuncName, Label::LabelCode));
  return true;
}

bool Analyzer::BuildControlFlowGraph(Database& rDb, std::string const& rLblName, ControlFlowGraph& rCfg) const
{
  Address const& rLblAddr = rDb.GetAddressFromLabelName(rLblName);
  if (rLblAddr.GetAddressingType() == Address::UnknownType) return false;

  return BuildControlFlowGraph(rDb, rLblAddr, rCfg);
}

bool Analyzer::BuildControlFlowGraph(Database& rDb, Address const& rAddr, ControlFlowGraph& rCfg) const
{
  std::stack<Address> CallStack;
  Address::List Addresses;
  typedef std::tuple<Address, Address, BasicBlockEdgeProperties::Type> TupleEdge;
  std::list<TupleEdge> Edges;
  std::map<Address, bool> VisitedInstruction;
  bool RetReached = false;

  Address CurAddr = rAddr;

  MemoryArea const* pMemArea = rDb.GetMemoryArea(CurAddr);

  if (pMemArea == nullptr)
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

      if (rInsn.GetOperationType() & Instruction::OpJump)
      {
        Address DstAddr;

        if (rInsn.Operand(0)->GetType() & O_MEM)
          break;

        if (!rInsn.GetOperandReference(rDb, 0, CurAddr, DstAddr))
          break;

        if (rInsn.GetOperationType() & Instruction::OpCond)
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

      else if (rInsn.GetOperationType() & Instruction::OpRet && !(rInsn.GetOperationType() & Instruction::OpCond))
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
  bool Res = rArch.DisassembleBasicBlockOnly() == false ? true : false;

  auto Lbl = rDb.GetLabelFromAddress(rAddr);
  if (Lbl.GetType() & Label::LabelImported)
    return false;

  if (pMemArea == nullptr)
    goto exit;

  while (rDb.IsPresent(CurAddr))
  {
    // If we changed the current memory area, we must update it
    if (!pMemArea->IsPresent(CurAddr))
      if ((pMemArea = rDb.GetMemoryArea(CurAddr)) == nullptr)
        goto exit;

    // If the current memory area is not executable, we skip this execution flow
    if (!(pMemArea->GetAccess() & MA_EXEC))
      goto exit;

    if (rDb.RetrieveCell(CurAddr) == nullptr)
      goto exit;

    // We create a new entry and disassemble it
    Instruction* pInsn = new Instruction;

    try
    {
      TOffset PhysicalOffset;

      PhysicalOffset = CurAddr.GetOffset() - pMemArea->GetVirtualBase().GetOffset();

      // If something bad happens, we skip this instruction and go to the next function
      if (!rArch.Disassemble(pMemArea->GetBinaryStream(), PhysicalOffset, *pInsn))
        throw Exception(L"Unable to disassemble this instruction");
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

    // We try to retrieve the current instruction, if it's true we go to the next function
    for (auto InsnLen = 0; InsnLen < pInsn->GetLength(); ++InsnLen)
      if (rDb.ContainsCode(CurAddr + InsnLen))
      {
        Res = true;
        goto exit;
      }

      rBasicBlock.push_back(pInsn);

      auto OpType = pInsn->GetOperationType();
      if (
        OpType & Instruction::OpJump
        || OpType & Instruction::OpCall
        || OpType & Instruction::OpRet)
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
    rBasicBlock.erase(std::begin(rBasicBlock), std::end(rBasicBlock));
  }
  return Res;
}

bool Analyzer::RegisterArchitecture(Architecture::SharedPtr spArch)
{
  u8 Id = 0;
  bool FoundId = false;

  for (u8 i = 0; i < 32; ++i)
    if (!(m_ArchIdPool & (1 << i)))
    {
      m_ArchIdPool |= (1 << i);
      Id = i;
      FoundId = true;
      break;
    }

    if (FoundId == false) return false;

    spArch->UpdateId(Id);

    m_UsedArchitectures[spArch->GetTag()] = spArch;

    if (m_DefaultArchitectureTag == MEDUSA_ARCH_UNK)
      m_DefaultArchitectureTag = spArch->GetTag();

    return true;
}

bool Analyzer::UnregisterArchitecture(Architecture::SharedPtr spArch)
{
  return false; /* Not implemented */
}

void Analyzer::ResetArchitecture(void)
{
  m_UsedArchitectures.erase(std::begin(m_UsedArchitectures), std::end(m_UsedArchitectures));
  m_DefaultArchitectureTag = MEDUSA_ARCH_UNK;
}

Cell* Analyzer::GetCell(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr)
{
  //boost::lock_guard<MutexType> Lock(m_Mutex);
  Cell* pCell = const_cast<Cell*>(rDatabase.RetrieveCell(rAddr)); // TODO: do not use const_cast
  if (pCell == nullptr) return nullptr;

  auto spArch = GetArchitecture(pCell->GetArchitectureTag());
  if (!spArch) return nullptr;

  spArch->FormatCell(rDatabase, rBinStrm, rAddr, *pCell);
  return pCell;
}

Cell const* Analyzer::GetCell(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr) const
{
  //boost::lock_guard<MutexType> Lock(m_Mutex);
  Cell* pCell = const_cast<Cell*>(rDatabase.RetrieveCell(rAddr));
  if (pCell == nullptr) return nullptr;

  auto spArch = GetArchitecture(pCell->GetArchitectureTag());
  if (!spArch) return nullptr;

  spArch->FormatCell(rDatabase, rBinStrm, rAddr, *pCell);
  return pCell;
}

MultiCell* Analyzer::GetMultiCell(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr)
{
  MultiCell* pMultiCell = const_cast<MultiCell*>(rDatabase.RetrieveMultiCell(rAddr));
  if (pMultiCell == nullptr) return nullptr;

  auto spArch = GetArchitecture(m_DefaultArchitectureTag);
  if (!spArch) return nullptr;

  spArch->FormatMultiCell(rDatabase, rBinStrm, rAddr, *pMultiCell);
  return pMultiCell;
}

MultiCell const* Analyzer::GetMultiCell(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr) const
{
  MultiCell* pMultiCell = const_cast<MultiCell*>(rDatabase.RetrieveMultiCell(rAddr));
  if (pMultiCell == nullptr) return nullptr;

  auto spArch = GetArchitecture(m_DefaultArchitectureTag);
  if (!spArch) return nullptr;

  spArch->FormatMultiCell(rDatabase, rBinStrm, rAddr, *pMultiCell);
  return pMultiCell;
}

Architecture::SharedPtr Analyzer::GetArchitecture(Tag ArchTag) const
{
  if (ArchTag == MEDUSA_ARCH_UNK)
    ArchTag = m_DefaultArchitectureTag;

  auto itArch = m_UsedArchitectures.find(ArchTag);
  if (itArch == std::end(m_UsedArchitectures))
    return Architecture::SharedPtr();

  return itArch->second;
}

// Workaround from http://stackoverflow.com/questions/9669109/print-a-constified-subgraph-with-write-graphviz
template<typename Graph> struct PropWriter
{
  PropWriter(Graph const& rCfg, Analyzer const& rAnlz, Database const& rDatabase, BinaryStream const& rBinStrm)
    : m_rCfg(rCfg), m_rAnlz(rAnlz), m_rDb(rDatabase), m_rBinStrm(rBinStrm) {}
  template<typename Vertex> void operator()(std::ostream & out, Vertex const& v) const
  {
    out << "[shape=box] [label=\"";
    for (auto itAddr = std::begin(m_rCfg[v].GetAddresses()); itAddr != std::end(m_rCfg[v].GetAddresses()); ++itAddr)
    {
      std::string LineString = "Unknown";
      auto pCell = m_rAnlz.GetCell(m_rDb, m_rBinStrm, *itAddr);
      if (pCell != nullptr)
        LineString = pCell->ToString();
      auto Cmt = pCell->GetComment();
      if (!Cmt.empty())
      {
        LineString += std::string(" ; ");
        LineString += Cmt;
      }

      out << *itAddr << ": " << LineString << "\\n";
    }
    out << "\"]";
  }

private:
  Graph        const& m_rCfg;
  Analyzer     const& m_rAnlz;
  Database     const& m_rDb;
  BinaryStream const& m_rBinStrm;
};

void Analyzer::DumpControlFlowGraph(std::string const& rFilename, ControlFlowGraph const& rCfg, Database const& rDatabase, BinaryStream const& rBinStrm) const
{
  std::ofstream File(rFilename.c_str());
  boost::write_graphviz(File, rCfg.GetGraph(), PropWriter<ControlFlowGraph::Type>(rCfg.GetGraph(), *this, rDatabase, rBinStrm));
}

void Analyzer::TrackOperand(Database& rDb, Address const& rStartAddress, Tracker& rTracker)
{
  std::map<Address, bool> TrackedAddresses;

  Address::List FuncAddrs;
  rDb.FindFunctionAddressFromAddress(FuncAddrs, rStartAddress);

  if (!FuncAddrs.empty()) std::for_each(std::begin(FuncAddrs), std::end(FuncAddrs), [this, &rDb, &rTracker, &TrackedAddresses, &rStartAddress](Address const& rFuncAddr)
  {
    auto pFunc = dynamic_cast<Function const*>(GetMultiCell(rDb, rDb.GetFileBinaryStream(), rFuncAddr));
    if (pFunc == nullptr)
      return;

    auto rCfg = pFunc->GetControlFlowGraph();
    Address::List AllAddrs;
    AllAddrs.push_back(rStartAddress);

    while (!AllAddrs.empty())
    {
      auto Addr = AllAddrs.front();
      AllAddrs.pop_front();
      if (TrackedAddresses[Addr])
        continue;
      TrackedAddresses[Addr] = true;
      if (rTracker(*this, rDb, Addr) && !rCfg.GetNextAddress(Addr, AllAddrs))
        return;
    }
  });

  else
  {
    Address CurAddr = rStartAddress;
    while (rDb.MoveAddress(CurAddr, CurAddr, 1))
    {
      if (!rTracker(*this, rDb, CurAddr))
        break;
    }
  }
}

void Analyzer::BacktrackOperand(Database& rDb, Address const& rStartAddress, Tracker& rTracker)
{
  std::map<Address, bool> TrackedAddresses;

  Address::List FuncAddrs;
  rDb.FindFunctionAddressFromAddress(FuncAddrs, rStartAddress);

  if (!FuncAddrs.empty()) std::for_each(std::begin(FuncAddrs), std::end(FuncAddrs), [this, &rDb, &rTracker, &TrackedAddresses, &rStartAddress](Address const& rFuncAddr)
  {
    auto pFunc = dynamic_cast<Function const*>(GetMultiCell(rDb, rDb.GetFileBinaryStream(), rFuncAddr));
    if (pFunc == nullptr)
      return;

    auto rCfg = pFunc->GetControlFlowGraph();
    Address::List AllAddrs;
    AllAddrs.push_back(rStartAddress);

    while (!AllAddrs.empty())
    {
      auto Addr = AllAddrs.front();
      AllAddrs.pop_front();
      if (TrackedAddresses[Addr])
        continue;
      TrackedAddresses[Addr] = true;
      if (rTracker(*this, rDb, Addr) == false || rCfg.GetPreviousAddress(Addr, AllAddrs) == false)
        return;
    }
  });

  else
  {
    Address CurAddr = rStartAddress;
    while (rDb.MoveAddress(CurAddr, CurAddr, -1))
    {
      if (!rTracker(*this, rDb, CurAddr))
        break;
    }
  }
}

MEDUSA_NAMESPACE_END
