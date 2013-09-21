#include "medusa/analyzer.hpp"

#include "medusa/function.hpp"
#include "medusa/character.hpp"
#include "medusa/string.hpp"
#include "medusa/label.hpp"
#include "medusa/log.hpp"
#include "medusa/module.hpp"

#include <list>
#include <stack>

#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

// bool Analyzer::DisassembleFollowingExecutionPath(Document const& rDoc, Architecture& rArch, Address const& rAddr, std::list<Instruction*>& rBasicBlock)
void Analyzer::DisassembleFollowingExecutionPath(Document& rDoc, Address const& rEntrypoint, Architecture& rArch) const
{
  boost::lock_guard<boost::mutex> Lock(m_DisasmMutex);

  auto Lbl = rDoc.GetLabelFromAddress(rEntrypoint);
  if (Lbl.GetType() & Label::Imported)
    return;

  std::stack<Address> CallStack;
  Address::List FuncAddr;
  Address CurAddr             = rEntrypoint;
  MemoryArea const* pMemArea  = rDoc.GetMemoryArea(CurAddr);

  if (pMemArea == nullptr)
  {
    Log::Write("core") << "Unable to get memory area for address " << CurAddr.ToString() << LogEnd;
    return;
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
    while (rDoc.IsPresent(CurAddr) && !rDoc.ContainsCode(CurAddr))
    {
      //Log::Write("debug") << "Disassembling basic block at " << CurAddr.ToString() << LogEnd;

      // Let's try to disassemble a basic block
      std::list<Instruction::SPtr> BasicBlock;
      if (!DisassembleBasicBlock(rDoc, rArch, CurAddr, BasicBlock)) break;
      if (BasicBlock.size() == 0)                                  break;

      for (auto itInsn = std::begin(BasicBlock); itInsn != std::end(BasicBlock); ++itInsn)
      {
        if (rDoc.ContainsCode(CurAddr))
        {
          //Log::Write("debug") << "Instruction is already disassembled at " << CurAddr.ToString() << LogEnd;
          FunctionIsFinished = true;
          continue;
        }

        if (!rDoc.SetCell(CurAddr, *itInsn, true))
        {
          //Log::Write("core") << "Error while inserting instruction at " << CurAddr.ToString() << LogEnd;
          FunctionIsFinished = true;
          continue;
        }

        for (u8 i = 0; i < OPERAND_NO; ++i)
        {
          Address DstAddr;
          if ((*itInsn)->GetOperandReference(rDoc, i, CurAddr, DstAddr))
            CallStack.push(DstAddr);
        }

        CreateXRefs(rDoc, CurAddr);

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
          if (!pLastInsn->GetOperandReference(rDoc, 0, CurAddr, DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          FuncAddr.push_back(DstAddr);
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
          if (!pLastInsn->GetOperandReference(rDoc, 0, CurAddr, DstAddr))
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
    } // end while (m_Document.IsPresent(CurAddr))
  } // while (!CallStack.empty())

  std::for_each(std::begin(FuncAddr), std::end(FuncAddr), [&](Address const& rAddr)
  {
    CreateFunction(rDoc, rAddr);
  });
}

void Analyzer::CreateXRefs(Document& rDoc, Address const& rAddr) const
{
  auto spInsn = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddr));
  if (spInsn == nullptr)
    return;

  for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
  {
    Address DstAddr;
    if (!spInsn->GetOperandReference(rDoc, CurOp, rAddr, DstAddr))
      continue;

    rDoc.ChangeValueSize(DstAddr, spInsn->GetOperandReferenceLength(CurOp), false);

    // Check if the destination is valid and is an instruction
    auto spDstCell = rDoc.GetCell(DstAddr);
    if (spDstCell == nullptr)
      continue;

    // Add XRef
    Address OpAddr;
    if (!spInsn->GetOperandAddress(CurOp, rAddr, OpAddr))
      OpAddr = rAddr;
    rDoc.GetXRefs().AddXRef(DstAddr, OpAddr);

    // If the destination has already a label, we skip it
    if (!rDoc.GetLabelFromAddress(DstAddr).GetName().empty())
      continue;

    std::string SuffixName = DstAddr.ToString();
    std::replace(SuffixName.begin(), SuffixName.end(), ':', '_');

    switch (spInsn->GetOperationType() & (Instruction::OpCall | Instruction::OpJump))
    {
    case Instruction::OpJump:
      rDoc.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::Code | Label::Local), false);
      break;

    case Instruction::OpUnknown:
      if (rDoc.GetMemoryArea(DstAddr)->GetAccess() & MemoryArea::Execute)
        rDoc.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::Code | Label::Local), false);
      else
        rDoc.AddLabel(DstAddr, Label(m_DataPrefix + SuffixName, Label::Data | Label::Global), false);

    default: break;
    } // switch (pInsn->GetOperationType() & (Instruction::OpCall | Instruction::OpJump))
  } // for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
}

bool Analyzer::ComputeFunctionLength(
  Document const& rDoc,
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
  MemoryArea const* pMemArea = rDoc.GetMemoryArea(CurAddr);

  auto Lbl = rDoc.GetLabelFromAddress(rFunctionAddress);
  if (Lbl.GetType() & Label::Imported)
    return false;

  if (pMemArea == nullptr)
    return false;

  CallStack.push(CurAddr);

  while (!CallStack.empty())
  {
    CurAddr = CallStack.top();
    CallStack.pop();

    while(rDoc.ContainsCode(CurAddr))
    {
      auto spInsn = std::static_pointer_cast<Instruction>(rDoc.GetCell(CurAddr));

      if (spInsn == nullptr)
      {
        Log::Write("core") << "instruction at " << CurAddr.ToString() << " is null" << LogEnd;
        break;
      }

      if (VisitedInstruction[CurAddr])
      {
        CurAddr += spInsn->GetLength();
        continue;
      }

      FuncLen += static_cast<u32>(spInsn->GetLength());

      VisitedInstruction[CurAddr] = true;

      rFunctionLength += static_cast<u32>(spInsn->GetLength());
      rInstructionCounter++;

      if (spInsn->GetOperationType() & Instruction::OpJump)
      {
        Address DstAddr;

        if (spInsn->GetOperationType() & Instruction::OpCond)
          CallStack.push(CurAddr + spInsn->GetLength());

        if (spInsn->Operand(0)->GetType() & O_MEM)
          break;

        if (!spInsn->GetOperandReference(rDoc, 0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
        continue;
      }

      else if (spInsn->GetOperationType() & Instruction::OpRet && !(spInsn->GetOperationType() & Instruction::OpCond))
      {
        RetReached = true;
        if (EndAddr < CurAddr)
          EndAddr = CurAddr;
        break;
      }

      CurAddr += spInsn->GetLength();

      if (LengthThreshold && FuncLen > LengthThreshold)
        return false;
    } // end while (m_Document.IsPresent(CurAddr))
  } // while (!CallStack.empty())

  return RetReached;
}

void Analyzer::FindStrings(Document& rDoc, Architecture& rArch) const
{
  Document::LabelBimapType const& rLabels = rDoc.GetLabels();
  for (Document::LabelBimapType::const_iterator It = rLabels.begin();
    It != rLabels.end(); ++It)
  {
    if (It->right.GetType() != Label::Data)
      continue;

    MemoryArea const* pMemArea   = rDoc.GetMemoryArea(It->left);
    if (pMemArea == nullptr)
      continue;

    std::string CurString        = "";
    BinaryStream const& rBinStrm = rDoc.GetFileBinaryStream();
    TOffset PhysicalOffset;

    if (pMemArea->ConvertOffsetToFileOffset(It->left.GetOffset(), PhysicalOffset) == false)
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
      auto spString = std::make_shared<String>(String::Utf16Type, CurString);
      rDoc.SetCell(It->left, spString, true);
      rDoc.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::String));
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
      auto spString = std::make_shared<String>(String::AsciiType, CurString);
      rDoc.SetCell(It->left, spString, true);
      rDoc.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::String));
    }
  }
}

bool Analyzer::MakeAsciiString(Document& rDoc, Address const& rAddr) const
{
  try
  {
    s8 CurChar;
    TOffset StrOff;
    std::string StrData     = "";
    auto pMemArea           = rDoc.GetMemoryArea(rAddr);
    auto const& rCurBinStrm = rDoc.GetFileBinaryStream();

    if (pMemArea->ConvertOffsetToFileOffset(rAddr.GetOffset(), StrOff) == false)
      return false;

    for (;;)
    {
      rCurBinStrm.Read(StrOff, CurChar);
      if (CurChar == '\0') break;

      StrData += CurChar;
      ++StrOff;
    }

    if (StrData.length() == 0) return false;

    auto spString = std::make_shared<String>(String::AsciiType, StrData);
    rDoc.SetCell(rAddr, spString, true);
    rDoc.AddLabel(rAddr, Label(m_StringPrefix + spString->GetCharacters(), Label::String | Label::Global));
  }
  catch (Exception const&)
  {
    return false;
  }

  return true;
}

bool Analyzer::MakeWindowsString(Document& rDoc, Address const& rAddr) const
{
  try
  {
    TOffset StrStartOff, StrOff;
    std::string StrData     = "";
    auto pMemArea           = rDoc.GetMemoryArea(rAddr);
    auto const& rCurBinStrm = rDoc.GetFileBinaryStream();
    WinString WinStr;
    WinString::CharType CurChar;

    if (pMemArea->ConvertOffsetToFileOffset(rAddr.GetOffset(), StrOff) == false)
      return false;

    StrStartOff = StrOff;

    bool EndReached = false;
    do
    {
      rCurBinStrm.Read(StrOff, CurChar);
      if (WinStr.IsFinalCharacter(CurChar)) EndReached = true;

      if (EndReached == false)
        StrData += WinStr.ConvertToUf8(CurChar);
      StrOff += sizeof(CurChar);
    } while (EndReached == false);

    if (StrData.length() == 0) return false;

    auto spString = std::make_shared<String>(String::Utf16Type, StrData, static_cast<u16>(StrOff - StrStartOff));
    rDoc.SetCell(rAddr, spString, true);
    rDoc.AddLabel(rAddr, Label(m_StringPrefix + spString->GetCharacters(), Label::String | Label::Global));
  }
  catch (Exception const&)
  {
    return false;
  }

  return true;
}

bool Analyzer::CreateFunction(Document& rDoc, Address const& rAddr) const
{
  std::string SuffixName = rAddr.ToString();
  std::replace(SuffixName.begin(), SuffixName.end(), ':', '_');
  Address FuncEnd;
  u16 FuncLen;
  u16 InsnCnt;
  std::string FuncName = m_FunctionPrefix + SuffixName;

  if (ComputeFunctionLength(rDoc, rAddr, FuncEnd, FuncLen, InsnCnt, 0x1000) == true)
  {
    Log::Write("core")
      << "Function found"
      << ": address="               << rAddr.ToString()
      << ", length="                << FuncLen
      << ", instruction counter: "  << InsnCnt
      << LogEnd;

    ControlFlowGraph Cfg;
    if (BuildControlFlowGraph(rDoc, rAddr, Cfg) == false)
    {
      Log::Write("core")
        << "Unable to build control flow graph for " << rAddr.ToString() << LogEnd;
      return false;
    }

    Function* pFunction = new Function(FuncLen, InsnCnt, Cfg);
    rDoc.SetMultiCell(rAddr, pFunction, false);
  }
  else
  {
    auto pMemArea = rDoc.GetMemoryArea(rAddr);
    if (pMemArea == nullptr)
      return false;
    auto pInsn = rDoc.GetCell(rAddr);
    if (pInsn == nullptr)
      return false;
    auto spArch = ModuleManager::Instance().GetArchitecture(pInsn->GetArchitectureTag());
    auto spFuncInsn = std::static_pointer_cast<Instruction const>(rDoc.GetCell(rAddr));
    if (spFuncInsn->GetOperationType() != Instruction::OpJump)
      return false;
    Address OpRefAddr;
    if (spFuncInsn->GetOperandReference(rDoc, 0, rAddr, OpRefAddr) == false)
      return false;
    auto OpLbl = rDoc.GetLabelFromAddress(OpRefAddr);
    if (OpLbl.GetType() == Label::Unknown)
      return false;
    FuncName = std::string(spFuncInsn->GetName()) + std::string("_") + OpLbl.GetLabel();
  }

  rDoc.AddLabel(rAddr, Label(FuncName, Label::Code | Label::Global), false);
  return true;
}

bool Analyzer::BuildControlFlowGraph(Document& rDoc, std::string const& rLblName, ControlFlowGraph& rCfg) const
{
  Address const& rLblAddr = rDoc.GetAddressFromLabelName(rLblName);
  if (rLblAddr.GetAddressingType() == Address::UnknownType) return false;

  return BuildControlFlowGraph(rDoc, rLblAddr, rCfg);
}

bool Analyzer::BuildControlFlowGraph(Document& rDoc, Address const& rAddr, ControlFlowGraph& rCfg) const
{
  std::stack<Address> CallStack;
  Address::List Addresses;
  typedef std::tuple<Address, Address, BasicBlockEdgeProperties::Type> TupleEdge;
  std::list<TupleEdge> Edges;
  std::map<Address, bool> VisitedInstruction;
  bool RetReached = false;

  Address CurAddr = rAddr;

  MemoryArea const* pMemArea = rDoc.GetMemoryArea(CurAddr);

  if (pMemArea == nullptr)
    return false;

  CallStack.push(CurAddr);

  while (!CallStack.empty())
  {
    CurAddr = CallStack.top();
    CallStack.pop();

    while (rDoc.ContainsCode(CurAddr))
    {
      auto spInsn = std::static_pointer_cast<Instruction>(rDoc.GetCell(CurAddr));

      // If the current address is already visited
      if (VisitedInstruction[CurAddr])
      {
        // ... and if the current instruction is the end of the function, we take another address from the callstack
        if (spInsn->GetOperationType() & Instruction::OpRet && !(spInsn->GetOperationType() & Instruction::OpCond))
          break;

        // if not, we try with the next address.
        CurAddr += spInsn->GetLength();
        continue;
      }

      Addresses.push_back(CurAddr);
      VisitedInstruction[CurAddr] = true;

      if (spInsn->GetOperationType() & Instruction::OpJump)
      {
        Address DstAddr;

        if (spInsn->Operand(0)->GetType() & O_MEM)
          break;

         if (!spInsn->GetOperandReference(rDoc, 0, CurAddr, DstAddr))
          break;

        if (spInsn->GetOperationType() & Instruction::OpCond)
        {
          Address NextAddr = CurAddr + spInsn->GetLength();
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

      else if (spInsn->GetOperationType() & Instruction::OpRet && !(spInsn->GetOperationType() & Instruction::OpCond))
      {
        RetReached = true;
        break;
      }

      CurAddr += spInsn->GetLength();
    } // end while (m_Document.IsPresent(CurAddr))
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

  rCfg.Finalize(rDoc);

  return RetReached;
}

bool Analyzer::FormatCell(Document const& rDoc, BinaryStream const& rBinStrm, Address const& rAddress, Cell const& rCell, std::string & rStrCell, Cell::Mark::List & rMarks) const
{
  auto spArch = ModuleManager::Instance().GetArchitecture(rCell.GetArchitectureTag());
  if (spArch == nullptr)
    return false;
  return spArch->FormatCell(rDoc, rBinStrm, rAddress, rCell, rStrCell, rMarks);
}

bool Analyzer::FormatMultiCell(Document const& rDoc,BinaryStream const& rBinStrm,Address const& rAddress,MultiCell const& rMultiCell,std::string & rStrMultiCell,Cell::Mark::List & rMarks) const
{
  auto spCell = rDoc.GetCell(rAddress);
  if (spCell == nullptr)
    return false;
  auto spArch = ModuleManager::Instance().GetArchitecture(spCell->GetArchitectureTag());
  if (spArch == nullptr)
    return false;
  return spArch->FormatMultiCell(rDoc, rBinStrm, rAddress, rMultiCell, rStrMultiCell, rMarks);
}

bool Analyzer::DisassembleBasicBlock(Document const& rDoc, Architecture& rArch, Address const& rAddr, std::list<Instruction::SPtr>& rBasicBlock)
{
  Address CurAddr = rAddr;
  MemoryArea const* pMemArea = rDoc.GetMemoryArea(CurAddr);
  bool Res = rArch.DisassembleBasicBlockOnly() == false ? true : false;

  auto Lbl = rDoc.GetLabelFromAddress(rAddr);
  if (Lbl.GetType() & Label::Imported)
    return false;

  if (pMemArea == nullptr)
    goto exit;

  while (rDoc.IsPresent(CurAddr))
  {
    // If we changed the current memory area, we must update it
    if (!pMemArea->IsCellPresent(CurAddr.GetOffset()))
      if ((pMemArea = rDoc.GetMemoryArea(CurAddr)) == nullptr)
        goto exit;

    // If the current memory area is not executable, we skip this execution flow
    if (!(pMemArea->GetAccess() & MemoryArea::Execute))
      goto exit;

    auto spCurCell = rDoc.GetCell(CurAddr);

    if (spCurCell == nullptr)
      goto exit;

    if (spCurCell->GetType() != CellData::ValueType || spCurCell->GetLength() != 1)
      goto exit;

    // We create a new entry and disassemble it
    auto spInsn = std::make_shared<Instruction>();

    try
    {
      TOffset PhysicalOffset;

      if (pMemArea->ConvertOffsetToFileOffset(CurAddr.GetOffset(), PhysicalOffset) == false)
        throw Exception(L"Invalid memory area");

      // If something bad happens, we skip this instruction and go to the next function
      if (!rArch.Disassemble(rDoc.GetFileBinaryStream(), PhysicalOffset, *spInsn))
        throw Exception(L"Unable to disassemble this instruction");
    }
    catch (Exception const& e)
    {
      Log::Write("core")
        << "Exception while disassemble instruction at " << CurAddr.ToString()
        << ", reason: " << e.What()
        << LogEnd;
      goto exit;
    }

    // We try to retrieve the current instruction, if it's true we go to the next function
    for (size_t InsnLen = 0; InsnLen < spInsn->GetLength(); ++InsnLen)
      if (rDoc.ContainsCode(CurAddr + InsnLen))
      {
        Res = true;
        goto exit;
      }

      rBasicBlock.push_back(spInsn);

      auto OpType = spInsn->GetOperationType();
      if (
        OpType & Instruction::OpJump
        || OpType & Instruction::OpCall
        || OpType & Instruction::OpRet)
      {
        Res = true;
        goto exit;
      }

      CurAddr += spInsn->GetLength();
  } // !while (rDoc.IsPresent(CurAddr))

exit:
  if (Res == false)
  {
    rBasicBlock.erase(std::begin(rBasicBlock), std::end(rBasicBlock));
  }
  return Res;
}

void Analyzer::TrackOperand(Document& rDoc, Address const& rStartAddress, Tracker& rTracker)
{
  std::map<Address, bool> TrackedAddresses;

  Address::List FuncAddrs;
  rDoc.FindFunctionAddressFromAddress(FuncAddrs, rStartAddress);

  if (!FuncAddrs.empty()) std::for_each(std::begin(FuncAddrs), std::end(FuncAddrs), [this, &rDoc, &rTracker, &TrackedAddresses, &rStartAddress](Address const& rFuncAddr)
  {
    auto pFunc = dynamic_cast<Function const*>(rDoc.GetMultiCell(rFuncAddr));
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
      if (rTracker(*this, rDoc, Addr) && !rCfg.GetNextAddress(Addr, AllAddrs))
        return;
    }
  });

  else
  {
    Address CurAddr = rStartAddress;
    while (rDoc.MoveAddress(CurAddr, CurAddr, 1))
    {
      if (!rTracker(*this, rDoc, CurAddr))
        break;
    }
  }
}

void Analyzer::BacktrackOperand(Document& rDoc, Address const& rStartAddress, Tracker& rTracker)
{
  std::map<Address, bool> TrackedAddresses;

  Address::List FuncAddrs;
  rDoc.FindFunctionAddressFromAddress(FuncAddrs, rStartAddress);

  if (!FuncAddrs.empty()) std::for_each(std::begin(FuncAddrs), std::end(FuncAddrs), [this, &rDoc, &rTracker, &TrackedAddresses, &rStartAddress](Address const& rFuncAddr)
  {
    auto pFunc = dynamic_cast<Function const*>(rDoc.GetMultiCell(rFuncAddr));
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
      if (rTracker(*this, rDoc, Addr) == false || rCfg.GetPreviousAddress(Addr, AllAddrs) == false)
        return;
    }
  });

  else
  {
    Address CurAddr = rStartAddress;
    while (rDoc.MoveAddress(CurAddr, CurAddr, -1))
    {
      if (!rTracker(*this, rDoc, CurAddr))
        break;
    }
  }
}

MEDUSA_NAMESPACE_END
