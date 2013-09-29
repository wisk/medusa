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
      if (!DisassembleBasicBlock(rDoc, rArch, CurAddr, BasicBlock))
        break;
      if (BasicBlock.size() == 0)
        break;

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

        auto InsnType = (*itInsn)->GetSubType();
        if (InsnType == Instruction::NoneType || InsnType == Instruction::ConditionalType)
          CurAddr += (*itInsn)->GetLength();
      }

      if (FunctionIsFinished == true) break;

      auto pLastInsn = BasicBlock.back();
      //Log::Write("debug") << "Last insn: " << pLastInsn->ToString() << LogEnd;

      switch  (pLastInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType | Instruction::ReturnType))
      {
        // If the last instruction is a call, we follow it and save the return address
      case Instruction::CallType:
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
        } // end CallType

        // If the last instruction is a ret, we emulate its behavior
      case Instruction::ReturnType:
        {
          // We ignore conditional ret
          if (pLastInsn->GetSubType() & Instruction::ConditionalType)
          {
            CurAddr += pLastInsn->GetLength();
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
          if (pLastInsn->GetSubType() & Instruction::ConditionalType)
            CallStack.push(CurAddr + pLastInsn->GetLength());

          // Sometime, we can't determine the destination address, so we give up
          if (!pLastInsn->GetOperandReference(rDoc, 0, CurAddr, DstAddr))
          {
            FunctionIsFinished = true;
            break;
          }

          CurAddr = DstAddr;
          break;
        } // end JumpType

      default: break; // This case should never happen
      } // switch (pLastInsn->GetSubType())

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

    switch (spInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType))
    {
    case Instruction::JumpType:
      rDoc.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::Code | Label::Local), false);
      break;

    case Instruction::NoneType:
      if (rDoc.GetMemoryArea(DstAddr)->GetAccess() & MemoryArea::Execute)
        rDoc.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::Code | Label::Local), false);
      else
        rDoc.AddLabel(DstAddr, Label(m_DataPrefix + SuffixName, Label::Data | Label::Global), false);

    default: break;
    } // switch (pInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType))
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

      if (spInsn->GetSubType() & Instruction::JumpType)
      {
        Address DstAddr;

        if (spInsn->GetSubType() & Instruction::ConditionalType)
          CallStack.push(CurAddr + spInsn->GetLength());

        if (spInsn->Operand(0)->GetType() & O_MEM)
          break;

        if (!spInsn->GetOperandReference(rDoc, 0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
        continue;
      }

      else if (spInsn->GetSubType() & Instruction::ReturnType && !(spInsn->GetSubType() & Instruction::ConditionalType))
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
    if (!(It->right.GetType() & (Label::Data)))
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
    u16 RawLen = 0;

    while (true)
    {
      if (!rBinStrm.Read(PhysicalOffset, WinChar))
        break;
      if (!WinStr.IsValidCharacter(WinChar))
        break;
      CurString += WinStr.ConvertToUf8(WinChar);
      PhysicalOffset += sizeof(WinChar);
      RawLen += sizeof(WinChar);
    }

    if (WinStr.IsFinalCharacter(WinChar) && !CurString.empty())
    {
      RawLen += sizeof(WinChar);
      Log::Write("core") << "Found string: " << CurString << LogEnd;
      auto spString = std::make_shared<String>(String::Utf16Type, RawLen);
      rDoc.SetCell(It->left, spString, true);
      rDoc.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::String));
      continue;
    }

    // LATER: Redo
    /* ASCII */
    AsciiString AsciiStr;
    AsciiString::CharType AsciiChar;
    CurString = "";
    RawLen = 0;

    while (true)
    {
      if (!rBinStrm.Read(PhysicalOffset, AsciiChar))
        break;
      if (!AsciiStr.IsValidCharacter(AsciiChar))
        break;
      CurString += AsciiStr.ConvertToUf8(AsciiChar);
      PhysicalOffset += sizeof(AsciiChar);
      RawLen += sizeof(AsciiChar);
    }

    if (AsciiStr.IsFinalCharacter(AsciiChar) && !CurString.empty())
    {
      RawLen += sizeof(AsciiChar);
      Log::Write("core") << "Found string: " << CurString << LogEnd;
      auto spString = std::make_shared<String>(String::AsciiType, RawLen);
      rDoc.SetCell(It->left, spString, true);
      rDoc.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::String));
    }
  }
}

bool Analyzer::MakeAsciiString(Document& rDoc, Address const& rAddr) const
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
    if (!rCurBinStrm.Read(StrOff, CurChar))
      return false;
    if (CurChar == '\0')
      break;

    StrData += CurChar;
    ++StrOff;
  }

  if (StrData.length() == 0)
    return false;

  auto spString = std::make_shared<String>(String::AsciiType, static_cast<u16>(StrData.length() + 1));
  rDoc.SetCell(rAddr, spString, true);
  rDoc.AddLabel(rAddr, Label(m_StringPrefix + StrData, Label::String | Label::Global));

  return true;
}

bool Analyzer::MakeWindowsString(Document& rDoc, Address const& rAddr) const
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
    if (!rCurBinStrm.Read(StrOff, CurChar))
      return false;
    if (WinStr.IsFinalCharacter(CurChar))
      EndReached = true;

    if (EndReached == false)
      StrData += WinStr.ConvertToUf8(CurChar);
    StrOff += sizeof(CurChar);
  } while (EndReached == false);

  if (StrData.length() == 0)
    return false;

  StrOff += sizeof(CurChar);

  auto spString = std::make_shared<String>(String::Utf16Type, static_cast<u16>(StrOff - StrStartOff));
  rDoc.SetCell(rAddr, spString, true);
  rDoc.AddLabel(rAddr, Label(m_StringPrefix + StrData, Label::String | Label::Global));

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

    Function* pFunction = new Function(FuncLen, InsnCnt);
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
    if (spFuncInsn->GetSubType() != Instruction::JumpType)
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

bool Analyzer::BuildControlFlowGraph(Document const& rDoc, std::string const& rLblName, ControlFlowGraph& rCfg) const
{
  Address const& rLblAddr = rDoc.GetAddressFromLabelName(rLblName);
  if (rLblAddr.GetAddressingType() == Address::UnknownType)
    return false;

  return BuildControlFlowGraph(rDoc, rLblAddr, rCfg);
}

bool Analyzer::BuildControlFlowGraph(Document const& rDoc, Address const& rAddr, ControlFlowGraph& rCfg) const
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

        if (spInsn->Operand(0)->GetType() & O_MEM)
          break;

         if (!spInsn->GetOperandReference(rDoc, 0, CurAddr, DstAddr))
          break;

        if (spInsn->GetSubType() & Instruction::ConditionalType)
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

      else if (spInsn->GetSubType() & Instruction::ReturnType && !(spInsn->GetSubType() & Instruction::ConditionalType))
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


    if (!rDoc.ContainsUnknown(CurAddr))
      goto exit;

    // We create a new entry and disassemble it
    auto spInsn = std::make_shared<Instruction>();

    TOffset PhysicalOffset;

    if (pMemArea->ConvertOffsetToFileOffset(CurAddr.GetOffset(), PhysicalOffset) == false)
    {
      Log::Write("core") << "Invalid memory area at " << CurAddr.ToString() << LogEnd;
      goto exit;
    }

    // If something bad happens, we skip this instruction and go to the next function
    if (!rArch.Disassemble(rDoc.GetFileBinaryStream(), PhysicalOffset, *spInsn))
    {
      Log::Write("core") << "Unable to disassemble instruction at " << CurAddr.ToString() << LogEnd;
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

      auto OpType = spInsn->GetSubType();
      if (
        OpType & Instruction::JumpType
        || OpType & Instruction::CallType
        || OpType & Instruction::ReturnType)
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
}

void Analyzer::BacktrackOperand(Document& rDoc, Address const& rStartAddress, Tracker& rTracker)
{
}

MEDUSA_NAMESPACE_END
