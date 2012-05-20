#include "medusa/disassembler.hpp"

#include "medusa/function.hpp"
#include "medusa/character.hpp"
#include "medusa/string.hpp"
#include "medusa/label.hpp"
#include "medusa/log.hpp"

#include <list>
#include <stack>

#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

void Disassembler::Load(SerializeEntity::SPtr spSrlzEtt)
{
  spSrlzEtt->GetField("func_pref", m_FunctionPrefix);
  spSrlzEtt->GetField("lbl_pref",  m_LabelPrefix);
  spSrlzEtt->GetField("dat_pref",  m_DataPrefix);
  spSrlzEtt->GetField("str_pref",  m_StringPrefix);
}

SerializeEntity::SPtr Disassembler::Save(void)
{
  SerializeEntity::SPtr spDisasm(new SerializeEntity("disasm"));
  spDisasm->AddField("func_pref", m_FunctionPrefix);
  spDisasm->AddField("lbl_pref",  m_LabelPrefix);
  spDisasm->AddField("dat_pref",  m_DataPrefix);
  spDisasm->AddField("str_pref",  m_StringPrefix);
  return spDisasm;
}

void Disassembler::FollowExecutionPath(Database& rDatabase, Address const& rEntrypoint, Architecture& rArch) const
{
  std::stack<Address> CallStack;
  Address CurAddr = rEntrypoint;
  MemoryArea const* pMemArea = rDatabase.GetMemoryArea(CurAddr);

  if (pMemArea == NULL)
    return;

  // Push entry point
  CallStack.push(CurAddr);

  // Do we still have functions to disassemble ?
  while (!CallStack.empty())
  {
    // Retrieve the last function
    CurAddr = CallStack.top();
    CallStack.pop();

    while (rDatabase.IsPresent(CurAddr))
    {
      // If we changed the current memory area, we must update it
      if (!pMemArea->IsPresent(CurAddr))
        if ((pMemArea = rDatabase.GetMemoryArea(CurAddr)) == NULL)
          break;

      // If the current memory area is not executable, we skip this execution flow
      if (!(pMemArea->GetAccess() & MA_EXEC))
        break;

      if (rDatabase.RetrieveCell(CurAddr) == NULL)
        break;

      // We try to retrieve the current instruction, if it's true we go to the next function
      if (rDatabase.ContainsCode(CurAddr))
        break;

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
          break;
        }
      }
      catch (Exception const& e)
      {
        Log::Write("core")
          << "Exception while disassemble instruction at " << CurAddr.ToString()
          << ", reason: " << e.What()
          << LogEnd;
        delete pInsn;
        break;
      }

      rArch.FormatCell(rDatabase, pMemArea->GetBinaryStream(), CurAddr, *pInsn);

      if (!rDatabase.InsertCell(CurAddr, pInsn, true))
      {
        delete pInsn;
        break;
      }

      if (pInsn->GetOperationType() == Instruction::OpCall)
      {
        Address DstAddr;

        // Save return address
        CallStack.push(CurAddr + pInsn->GetLength());

        // Sometimes, we cannot determine the destination address, so we give up
        // We assume destination is hold in the first operand
        if (!pInsn->GetOperandReference(rDatabase, 0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
      } // end OpCall

      // We emulate the pop
      else if (pInsn->GetOperationType() == Instruction::OpRet)
      {
        if (pInsn->Cond() != C_NONE)
        {
          CurAddr += pInsn->GetLength();
          continue;
        }

        // If there is not address in CallStack, it means the code tries to screw us
        if (CallStack.empty()) break;
        CurAddr = CallStack.top();
        CallStack.pop();
      } // end OpRet

      // Jump type could be a bit tedious to handle
      // because of conditional jump
      else if (pInsn->GetOperationType() == Instruction::OpJump)
      {
        Address DstAddr;

        // Save untaken branch address
        if (pInsn->Cond() != C_NONE)
          CallStack.push(CurAddr + pInsn->GetLength());

        // Sometime, we can't determine the destination address, so we give up
        if (!pInsn->GetOperandReference(rDatabase, 0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
      } // end OpJump

      else if (pInsn->GetOperationType() == Instruction::OpUnknown)
      {
        for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
        {
          Address RefAddr;
          if (pInsn->GetOperandReference(rDatabase, CurOp, CurAddr, RefAddr))
            CallStack.push(RefAddr);
        }

        size_t InsnSz = pInsn->GetLength();
        if (InsnSz == 0) break;
        CurAddr += InsnSz;
      } // end OpUnknown

      else break;
    } // end while (m_Database.IsPresent(CurAddr))
  } // while (!CallStack.empty())
}

void Disassembler::CreateXRefs(Database& rDatabase) const
{
  for (Database::TIterator itMemArea = rDatabase.Begin(); itMemArea != rDatabase.End(); ++itMemArea)
  {
    if (!((*itMemArea)->GetAccess() & MA_EXEC))
      continue;

    for (MemoryArea::TIterator itCell = (*itMemArea)->Begin(); itCell != (*itMemArea)->End(); ++itCell)
    {
      if (itCell->second == NULL) continue;

      if (itCell->second->GetType() == Cell::InstructionType)
      {
        Instruction* pInsn = static_cast<Instruction*>(itCell->second);

        for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
        {
          Address::SPtr CurAddr = (*itMemArea)->MakeAddress(itCell->first);
          Address DstAddr;
          if (!pInsn->GetOperandReference(rDatabase, CurOp, *CurAddr, DstAddr))
            continue;

          rDatabase.ChangeValueSize(DstAddr, pInsn->GetOperandReferenceLength(CurOp), true);

          // Check if the destination is valid and is an instruction
          Cell* pDstCell = rDatabase.RetrieveCell(DstAddr);
          if (pDstCell == NULL) continue;

          // Add XRef
          Address OpAddr;
          if (!pInsn->GetOperandAddress(CurOp, *CurAddr, OpAddr))
            OpAddr = *CurAddr;
          rDatabase.GetXRefs().AddXRef(DstAddr, OpAddr);
          typedef Database::View::LineInformation LineInformation;
          rDatabase.GetView().AddLineInformation(LineInformation(LineInformation::XrefLineType, DstAddr));

          // If the destination has already a label, we skip it
          if (!rDatabase.GetLabelFromAddress(DstAddr).GetName().empty())
            continue;

          std::string SuffixName = DstAddr.ToString();
          std::replace(SuffixName.begin(), SuffixName.end(), ':', '_');

          switch (pInsn->GetOperationType())
          {
          case Instruction::OpCall:
            {
              rDatabase.AddLabel(DstAddr, Label(m_FunctionPrefix + SuffixName, Label::LabelCode));
              Address FuncEnd;
              u16 FuncLen;
              u16 InsnCnt;
              if (ComputeFunctionLength(rDatabase, DstAddr, FuncEnd, FuncLen, InsnCnt, 0x1000) == true)
              {
                Log::Write("core")
                  << "Function found"
                  << ": address="               << DstAddr.ToString()
                  << ", length="                << FuncLen
                  << ", instruction counter: "  << InsnCnt
                  << LogEnd;

                Function* pFunction = new Function(FuncLen, InsnCnt);
                rDatabase.InsertMultiCell(DstAddr, pFunction, false);
              }
              break;
            }

          case Instruction::OpJump:
            rDatabase.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::LabelCode));
            break;

          case Instruction::OpUnknown:
            if (rDatabase.GetMemoryArea(DstAddr)->GetAccess() & MA_EXEC)
              rDatabase.AddLabel(DstAddr, Label(m_LabelPrefix + SuffixName, Label::LabelCode));
            else
              rDatabase.AddLabel(DstAddr, Label(m_DataPrefix + SuffixName, Label::LabelData));

          default: break;
          } // switch (pInsn->GetOperationType())
        } // for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
      } // if (itCell->second->GetType() == Cell::InstructionType)
    } // for (MemoryArea::TIterator itCell = (*itMemArea)->Begin(); itCell != (*itMemArea)->End(); ++itCell)
  } // for (Database::TIterator itMemArea = rDatabase.Begin(); itMemArea != rDatabase.End(); ++itMemArea)
}

void Disassembler::FormatsAllCells(Database& rDatabase, Architecture& rArch) const
{
  u32 FuncLenThreshold = -1;
  for (Database::TIterator itMemArea = rDatabase.Begin(); itMemArea != rDatabase.End(); ++itMemArea)
  {
    for (MemoryArea::TIterator itCell = (*itMemArea)->Begin(); itCell != (*itMemArea)->End(); ++itCell)
    {
      if (itCell->second == NULL) continue;

      MultiCell *pMc = rDatabase.RetrieveMultiCell(itCell->first);
      if (pMc)
        rArch.FormatMultiCell(rDatabase, (*itMemArea)->GetBinaryStream(), itCell->first, *pMc);

      Address::SPtr CurAddr = (*itMemArea)->MakeAddress(itCell->first);

      rArch.FormatCell(rDatabase, (*itMemArea)->GetBinaryStream(), *CurAddr, *itCell->second);
    }
  }
}

bool Disassembler::ComputeFunctionLength(
    Database const& rDatabase,
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
  MemoryArea const* pMemArea = rDatabase.GetMemoryArea(CurAddr);

  if (pMemArea == NULL)
    return false;

  CallStack.push(CurAddr);

  while (!CallStack.empty())
  {
    CurAddr = CallStack.top();
    CallStack.pop();

    while (rDatabase.ContainsCode(CurAddr))
    {
      Instruction const& rInsn = *static_cast<Instruction const*>(rDatabase.RetrieveCell(CurAddr));

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

        if (!rInsn.GetOperandReference(rDatabase, 0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
        continue;
      }

      else if (rInsn.GetOperationType() == Instruction::OpRet)
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

void Disassembler::FindStrings(Database& rDatabase, Architecture& rArch) const
{
  Database::TLabelMap const& rLabels = rDatabase.GetLabels();
  for (Database::TLabelMap::const_iterator It = rLabels.begin();
      It != rLabels.end(); ++It)
  {
    if (It->right.GetType() != Label::LabelData)
      continue;

    u8 CurChar;
    BinaryStream const& rBinStrm = rDatabase.GetFileBinaryStream();
    std::string CurString        = "";
    MemoryArea const* pMemArea   = rDatabase.GetMemoryArea(It->left);
    TOffset PhysicalOffset;

    if (pMemArea == NULL)
      continue;

    if (pMemArea->Translate(It->left.GetOffset(), PhysicalOffset) == false)
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
      rDatabase.InsertCell(It->left, pString, true, true);
      rDatabase.SetLabelToAddress(It->left, Label(CurString, m_StringPrefix, Label::LabelString));
    }
  }
}

bool Disassembler::BuildControlFlowGraph(Database& rDatabase, std::string const& rLblName, ControlFlowGraph& rCfg)
{
  Address const& rLblAddr = rDatabase.GetAddressFromLabelName(rLblName);
  if (rLblAddr.GetAddressingType() == Address::UnknownType) return false;

  return BuildControlFlowGraph(rDatabase, rLblAddr, rCfg);
}

bool Disassembler::BuildControlFlowGraph(Database& rDatabase, Address const& rAddr, ControlFlowGraph& rCfg)
{
  std::stack<Address> AddressStack;
  std::map<Address, bool> VisitedInstruction;
  Address CurAddr = rAddr;
  Address BasicBlockAddr;
  std::list<Address> BasicBlocks;
  typedef std::tuple<Address, Address, BasicBlockEdgeProperties::Type> EdgeTuple;
  std::list<EdgeTuple> Edges;

  MemoryArea const* pMemArea = rDatabase.GetMemoryArea(CurAddr);

  if (pMemArea == NULL)
    return false;

  AddressStack.push(CurAddr);

  while (!AddressStack.empty())
  {
    CurAddr = AddressStack.top();
    BasicBlockAddr = CurAddr;
    AddressStack.pop();

    while (rDatabase.ContainsCode(CurAddr))
    {
      Instruction const& rInsn = *static_cast<Instruction const*>(rDatabase.RetrieveCell(CurAddr));

      if (VisitedInstruction[CurAddr])
      {
        CurAddr += rInsn.GetLength();
        continue;
      }

      VisitedInstruction[CurAddr] = true;

      if (rInsn.GetOperationType() == Instruction::OpJump)
      {
        Address DstAddr;

        if (rInsn.Operand(0)->GetType() & O_MEM)
          break;

        if (!rInsn.GetOperandReference(rDatabase, 0, CurAddr, DstAddr))
          break;

        if (rInsn.GetCond() != C_NONE)
        {
          Address NextAddr = CurAddr;
          NextAddr += rInsn.GetLength();
          AddressStack.push(NextAddr);
          Edges.push_back(EdgeTuple(BasicBlockAddr, DstAddr,  BasicBlockEdgeProperties::True ));
          Edges.push_back(EdgeTuple(BasicBlockAddr, NextAddr, BasicBlockEdgeProperties::False));
          Log::Write("core") << "dst addr: " << DstAddr << ", next addr: " << NextAddr << LogEnd;
        }
        else
          Edges.push_back(EdgeTuple(BasicBlockAddr, DstAddr, BasicBlockEdgeProperties::Unconditional));

        CurAddr = DstAddr;
        BasicBlockAddr = DstAddr;
        continue;
      }

      else if (rInsn.GetOperationType() == Instruction::OpRet)
        break;

      CurAddr += rInsn.GetLength();
    } // end while (m_Database.IsPresent(CurAddr))
  } // while (!CallStack.empty())

  Edges.sort([](EdgeTuple const& lhs, EdgeTuple const& rhs) -> bool
  {
    if (std::get<0>(lhs) == std::get<0>(rhs))
    {
      if (std::get<1>(lhs) == std::get<1>(rhs))
        return std::get<2>(lhs) < std::get<2>(rhs);
      return std::get<1>(lhs) < std::get<1>(rhs);
    }
    return std::get<0>(lhs) < std::get<0>(rhs);
  });
  Edges.unique([](EdgeTuple const& lhs, EdgeTuple const& rhs) -> bool
  {
    return
         std::get<0>(lhs) == std::get<0>(rhs)
      && std::get<1>(lhs) == std::get<1>(rhs)
      && std::get<2>(lhs) == std::get<2>(rhs);
  });

  std::for_each(std::begin(Edges), std::end(Edges), [&rCfg](EdgeTuple const& rEdge)
  {
    static const char *TypeStr[] = { "Unknown", "Unconditional", "True", "False" };
    std::cout << "Edge: " << std::get<0>(rEdge) << " - " << std::get<1>(rEdge) << " - " << TypeStr[std::get<2>(rEdge)] << std::endl;
  });

  std::for_each(std::begin(Edges), std::end(Edges), [&rCfg](EdgeTuple const& rEdge)
  {
    rCfg.AddBasicBlockVertex(BasicBlockVertexProperties(std::get<0>(rEdge)));
    rCfg.AddBasicBlockVertex(BasicBlockVertexProperties(std::get<1>(rEdge)));
  });

  std::for_each(std::begin(Edges), std::end(Edges), [&rCfg](EdgeTuple const& rEdge)
  {
    if (rCfg.AddBasicBlockEdge(BasicBlockEdgeProperties(std::get<2>(rEdge)), std::get<0>(rEdge), std::get<1>(rEdge)))
      std::cout << "Valid Edge: " << std::get<0>(rEdge) << " - " << std::get<1>(rEdge) << std::endl;
    else
      std::cout << "Invalid Edge: " << std::get<0>(rEdge) << " - " << std::get<1>(rEdge) << std::endl;
  });

  return true;
}

MEDUSA_NAMESPACE_END