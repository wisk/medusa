#include "medusa/disassembler.hpp"

#include "medusa/function.hpp"
#include "medusa/character.hpp"
#include "medusa/string.hpp"
#include "medusa/label.hpp"

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
        std::wostringstream woss;
        woss
          << "Exception while disassemble instruction at " << CurAddr.ToWString()
          << ", reason: " << e.What()
          << std::endl;
        break;
      }

      rArch.FormatInstruction(rDatabase, CurAddr, *pInsn);

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
        if (!pInsn->GetOperandReference(0, CurAddr, DstAddr))
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

        // Sometime, we can determine the destination address, so we give up
        if (!pInsn->GetOperandReference(0, CurAddr, DstAddr))
          break;

        CurAddr = DstAddr;
      } // end OpJump

      else if (pInsn->GetOperationType() == Instruction::OpUnknown)
      {
        for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
        {
          Address RefAddr;
          if (pInsn->GetOperandReference(CurOp, CurAddr, RefAddr))
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
          TAddressPtr CurAddr = (*itMemArea)->MakeAddress(itCell->first);
          Address DstAddr;
          if (!pInsn->GetOperandReference(CurOp, *CurAddr, DstAddr))
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
                // TMP: Remove this
                std::cout
                  << "New function: address=" << DstAddr
                  << ", length=" << FuncLen
                  << ", instruction counter: " << InsnCnt
                  << std::endl;

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

void Disassembler::FormatInstructions(Database& rDatabase, Architecture &rArch) const
{
  u32 FuncLenThreshold = -1;
  for (Database::TIterator itMemArea = rDatabase.Begin(); itMemArea != rDatabase.End(); ++itMemArea)
  {
    if (!((*itMemArea)->GetAccess() & MA_EXEC))
      continue;

    for (MemoryArea::TIterator itCell = (*itMemArea)->Begin(); itCell != (*itMemArea)->End(); ++itCell)
    {
      if (itCell->second == NULL) continue;

      if (itCell->second->GetType() != Cell::InstructionType)
        continue;

      Instruction& rInsn = *static_cast<Instruction*>(itCell->second);
      TAddressPtr CurAddr = (*itMemArea)->MakeAddress(itCell->first);

      rArch.FormatInstruction(rDatabase, *CurAddr, rInsn);
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

        if (!rInsn.GetOperandReference(0, CurAddr, DstAddr))
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

void Disassembler::FindStrings(Database& rDatabase) const
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
      std::cout << "Found string: " << CurString << std::endl;
      String *pString = new String(static_cast<u16>(CurString.length()));
      rDatabase.InsertMultiCell(It->left, pString);
      rDatabase.SetLabelToAddress(It->left, Label(m_StringPrefix + CurString, Label::LabelString));

      u16 Index = 0;
      BOOST_FOREACH(char CurChar, CurString)
      {
        AsciiCharacter *pChar = new AsciiCharacter(CurChar, AsciiCharacter::AsciiCharacterType);
        rDatabase.InsertCell(It->left + Index, pChar, true);
        Index++;
      }
      AsciiCharacter *pChar = new AsciiCharacter('\0', AsciiCharacter::AsciiCharacterType);
      rDatabase.InsertCell(It->left + Index, pChar, true);
    }
  }

  for (Database::TLabelMap::const_iterator It = rLabels.begin();
      It != rLabels.end(); ++It)
  {
    std::cout << It->left.ToString() << " " << It->right.GetName() << std::endl;
  }
}

MEDUSA_NAMESPACE_END