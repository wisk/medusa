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

Analyzer::MakeFunctionTask::MakeFunctionTask(Document& rDoc, Address const& rFuncAddr)
  : m_rDoc(rDoc), m_Addr(rFuncAddr)
{
}

Analyzer::MakeFunctionTask::~MakeFunctionTask(void)
{
}

std::string Analyzer::MakeFunctionTask::GetName(void) const
{
  return "create a function";
}

void Analyzer::MakeFunctionTask::Run(void)
{
  CreateFunction(m_Addr);
}

bool Analyzer::MakeFunctionTask::CreateFunction(Address const& rAddr)
{
  Address FuncEnd;
  u16 FuncLen;
  u16 InsnCnt;

  if (ComputeFunctionLength(rAddr, FuncEnd, FuncLen, InsnCnt, 0x1000) == true)
  {
    Log::Write("core")
      << "Function found"
      << ": address="               << rAddr.ToString()
      << ", length="                << FuncLen
      << ", instruction counter: "  << InsnCnt
      << LogEnd;

    Label FuncLbl(rAddr, Label::Function | Label::Global);
    Function* pFunction = new Function(FuncLbl.GetLabel(), FuncLen, InsnCnt);
    m_rDoc.SetMultiCell(rAddr, pFunction, false);
    m_rDoc.AddLabel(rAddr, FuncLbl, false);
  }
  else
  {
    auto pMemArea = m_rDoc.GetMemoryArea(rAddr);
    if (pMemArea == nullptr)
      return false;
    auto pInsn = m_rDoc.GetCell(rAddr);
    if (pInsn == nullptr)
      return false;
    auto spArch = ModuleManager::Instance().GetArchitecture(pInsn->GetArchitectureTag());
    auto spFuncInsn = std::static_pointer_cast<Instruction const>(m_rDoc.GetCell(rAddr));
    if (spFuncInsn->GetSubType() != Instruction::JumpType)
      return false;
    Address OpRefAddr;
    if (spFuncInsn->GetOperandReference(m_rDoc, 0, rAddr, OpRefAddr) == false)
      return false;
    auto OpLbl = m_rDoc.GetLabelFromAddress(OpRefAddr);
    if (OpLbl.GetType() == Label::Unknown)
      return false;

    // Set the name <mnemonic> + "_" + sym_name (The name is not refreshed if sym_name is updated)
    std::string FuncName = std::string(spFuncInsn->GetName()) + std::string("_") + OpLbl.GetName();
    m_rDoc.AddLabel(rAddr, Label(FuncName, Label::Function | Label::Global), false);
    auto pFunc = new Function(FuncName, spFuncInsn->GetLength(), 1);
    m_rDoc.SetMultiCell(rAddr, pFunc, true);

    // Propagate the detail ID
    Id RefId;
    if (m_rDoc.RetrieveDetailId(OpRefAddr, 0, RefId))
      m_rDoc.BindDetailId(rAddr, 0, RefId);
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

  spOs->AnalyzeFunction(m_rDoc, rAddr);

  return true;
}

bool Analyzer::MakeFunctionTask::ComputeFunctionLength(Address const& rFuncAddr, Address& rEndAddress, u16& rFunctionLength, u16& rInstructionCounter, u32 LengthThreshold) const
{
  std::stack<Address> CallStack;
  std::map<Address, bool> VisitedInstruction;
  bool RetReached = false;

  u32 FuncLen                = 0x0;
  Address CurAddr            = rFuncAddr;
  Address EndAddr            = rFuncAddr;
  rFunctionLength            = 0x0;
  rInstructionCounter        = 0x0;
  MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

  auto Lbl = m_rDoc.GetLabelFromAddress(CurAddr);
  if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
    return false;

  if (pMemArea == nullptr)
    return false;

  CallStack.push(CurAddr);

  while (!CallStack.empty())
  {
    CurAddr = CallStack.top();
    CallStack.pop();

    while(m_rDoc.ContainsCode(CurAddr))
    {
      auto spInsn = std::static_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));

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

        if (!spInsn->GetOperandReference(m_rDoc, 0, CurAddr, DstAddr))// && !m_rDoc.IsPresent(DstAddr))
        {
          RetReached = true; // HACK: This is not really true...
          break;
        }

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


Analyzer::DisassembleTask::DisassembleTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode)
  : MakeFunctionTask(rDoc, rAddr), m_rArch(rArch), m_Mode(Mode)
{
}

Analyzer::DisassembleTask::~DisassembleTask(void)
{
}

std::string Analyzer::DisassembleTask::GetName(void) const
{
  return "disassemble";
}

void Analyzer::DisassembleTask::Run(void)
{
  Disassemble(m_Addr);
}

bool Analyzer::DisassembleTask::Disassemble(Address const& rAddr)
{
  auto Lbl = m_rDoc.GetLabelFromAddress(rAddr);
  if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
    return true;

  std::stack<Address> CallStack;
  Address::List FuncAddr;
  Address CurAddr             = rAddr;
  MemoryArea const* pMemArea  = m_rDoc.GetMemoryArea(CurAddr);

  if (pMemArea == nullptr)
  {
    Log::Write("core") << "Unable to get memory area for address " << CurAddr.ToString() << LogEnd;
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
      std::list<Instruction::SPtr> BasicBlock;
      if (!DisassembleBasicBlock(CurAddr, BasicBlock))
        break;
      if (BasicBlock.size() == 0)
        break;

      for (auto itInsn = std::begin(BasicBlock); itInsn != std::end(BasicBlock); ++itInsn)
      {
        if (m_rDoc.ContainsCode(CurAddr))
        {
          //Log::Write("debug") << "Instruction is already disassembled at " << CurAddr.ToString() << LogEnd;
          FunctionIsFinished = true;
          continue;
        }

        if (!m_rDoc.SetCell(CurAddr, *itInsn, true))
        {
          //Log::Write("core") << "Error while inserting instruction at " << CurAddr.ToString() << LogEnd;
          FunctionIsFinished = true;
          continue;
        }

        for (u8 i = 0; i < OPERAND_NO; ++i)
        {
          Address DstAddr;
          if ((*itInsn)->GetOperandReference(m_rDoc, i, CurAddr, DstAddr))
            CallStack.push(DstAddr);
        }

        CreateCrossReferences(CurAddr);

        auto InsnType = (*itInsn)->GetSubType();
        if (InsnType == Instruction::NoneType || InsnType == Instruction::ConditionalType)
          CurAddr += (*itInsn)->GetLength();
      }

      if (FunctionIsFinished == true)
        break;

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
          if (!pLastInsn->GetOperandReference(m_rDoc, 0, CurAddr, DstAddr))
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
          if (!pLastInsn->GetOperandReference(m_rDoc, 0, CurAddr, DstAddr))
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
    CreateFunction(rAddr);
  });

  return true;
}

bool Analyzer::DisassembleTask::DisassembleBasicBlock(Address const& rAddr, std::list<Instruction::SPtr>& rBasicBlock)
{
  Address CurAddr = rAddr;
  MemoryArea const* pMemArea = m_rDoc.GetMemoryArea(CurAddr);

  try
  {
    auto Lbl = m_rDoc.GetLabelFromAddress(CurAddr);
    if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
      throw std::string("Label \"") + Lbl.GetName() + std::string("\" ") + Lbl.GetLabel() + std::string(" is imported");

    if (pMemArea == nullptr)
      throw std::string("Unable to get memory area for address: ") + CurAddr.ToString();

    while (/*m_rDoc.IsPresent(CurAddr)*/ true)
    {
      // If we changed the current memory area, we must update it
      if (!pMemArea->IsCellPresent(CurAddr))
        if ((pMemArea = m_rDoc.GetMemoryArea(CurAddr)) == nullptr)
          throw std::string("Unable to get memory area for address: ") + CurAddr.ToString();

      // If the current memory area is not executable, we skip this execution flow
      if (!(pMemArea->GetAccess() & MemoryArea::Execute))
        throw std::string("Memory access \"") + pMemArea->GetName() + std::string("\" is not executable");

      if (m_rDoc.ContainsCode(CurAddr))
        return true;

      if (!m_rDoc.ContainsUnknown(CurAddr))
        throw std::string("Cell at \"") + CurAddr.ToString() + std::string("\" is not unknown");

      // We create a new entry and disassemble it
      auto spInsn = std::make_shared<Instruction>();

      TOffset PhysicalOffset;

      if (pMemArea->ConvertOffsetToFileOffset(CurAddr.GetOffset(), PhysicalOffset) == false)
        throw std::string("Unable to convert address ") + CurAddr.ToString() + std::string(" to offset");

      u8 Mode = m_Mode != 0 ? m_Mode : m_rDoc.GetMode(CurAddr);

      spInsn->GetData()->ArchitectureTag() = m_rArch.GetTag();
      spInsn->GetData()->Mode() = Mode;

      // If something bad happens, we skip this instruction and go to the next function
      if (!m_rArch.Disassemble(m_rDoc.GetBinaryStream(), PhysicalOffset, *spInsn, Mode))
        throw std::string("Unable to disassemble instruction at ") + CurAddr.ToString();

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

  catch(std::string const& rExcpMsg)
  {
    rBasicBlock.clear();
    Log::Write("core") << rExcpMsg << LogEnd;
    return false;
  }

  // At this point, we reach neither an basic block exit (jump, call, return) nor code,
  // so if we must disassemble basic block only: we have to return false, otherwise it's safe
  // to return true.
  return m_rArch.DisassembleBasicBlockOnly() == false ? true : false;
}

bool Analyzer::DisassembleTask::CreateCrossReferences(Address const& rAddr)
{
  auto spInsn = std::dynamic_pointer_cast<Instruction const>(m_rDoc.GetCell(rAddr));
  if (spInsn == nullptr)
    return false;

  for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)
  {
    Address DstAddr;
    if (!spInsn->GetOperandReference(m_rDoc, CurOp, rAddr, DstAddr))
      continue;

    //if (!m_rDoc.IsPresent(DstAddr))
    //  continue;

    m_rDoc.ChangeValueSize(DstAddr, spInsn->GetOperandReferenceLength(CurOp), false);

    // Check if the destination is valid and is an instruction
    auto spDstCell = m_rDoc.GetCell(DstAddr);
    if (spDstCell == nullptr)
      continue;

    // Add XRef
    Address OpAddr;
    if (!spInsn->GetOperandAddress(CurOp, rAddr, OpAddr))
      OpAddr = rAddr;
    m_rDoc.AddCrossReference(DstAddr, OpAddr);

    u16 LblTy = Label::Unknown;

    switch (spInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType))
    {
    case Instruction::CallType: LblTy = Label::Code | Label::Local; break;
    case Instruction::JumpType: LblTy = Label::Code | Label::Local; break;
    case Instruction::NoneType: LblTy = (m_rDoc.GetMemoryArea(DstAddr)->GetAccess() & MemoryArea::Execute) ?
                                  Label::Code | Label::Local : Label::Data | Label::Global;
    default: break;
    } // switch (pInsn->GetSubType() & (Instruction::CallType | Instruction::JumpType))

    m_rDoc.AddLabel(DstAddr, Label(DstAddr, LblTy), false);
  } // for (u8 CurOp = 0; CurOp < OPERAND_NO; ++CurOp)

  return true;
}

Analyzer::DisassembleFunctionTask::DisassembleFunctionTask(Document& rDoc, Address const& rAddr, Architecture& rArch, u8 Mode)
  : DisassembleTask(rDoc, rAddr, rArch, Mode)
{
}

Analyzer::DisassembleFunctionTask::~DisassembleFunctionTask(void)
{
}

std::string Analyzer::DisassembleFunctionTask::GetName(void) const
{
  return "disassemble a function";
}

void Analyzer::DisassembleFunctionTask::Run(void)
{
  DisassembleTask::Run();
  CreateFunction(m_Addr);
}

Analyzer::DisassembleAllFunctionsTask::DisassembleAllFunctionsTask(Document& rDoc)
  : m_rDoc(rDoc)
{
}

Analyzer::DisassembleAllFunctionsTask::~DisassembleAllFunctionsTask(void)
{
}

std::string Analyzer::DisassembleAllFunctionsTask::GetName(void) const
{
  return "disassemble all functions";
}

void Analyzer::DisassembleAllFunctionsTask::Run(void)
{
  /* Disassemble all symbols if possible */
  m_rDoc.ForEachLabel([&](Address const& rAddress, Label const& rLabel)
  {
    u16 LblType = rLabel.GetType() & Label::CellMask;
    bool IsExported = ((rLabel.GetType() & Label::AccessMask) == Label::Exported) ? true : false;

    if (!(LblType == Label::Function || ((LblType == Label::Code) && IsExported)))
      return;

    Log::Write("core") << "disassembling function " << rAddress << LogEnd;

    auto spArch = ModuleManager::Instance().GetArchitecture(m_rDoc.GetArchitectureTag(rAddress));
    if (spArch == nullptr)
    {
      Log::Write("core") << "there's no architecture for " << rAddress << LogEnd;
      return;
    }

    u8 Mode = m_rDoc.GetMode(rAddress);

    DisassembleFunctionTask DisasmFuncTask(m_rDoc, rAddress, *spArch, Mode);
    DisasmFuncTask.Run();
  });
}

Analyzer::FindAllStringTask::FindAllStringTask(Document& rDoc) : m_rDoc(rDoc)
{
}

Analyzer::FindAllStringTask::~FindAllStringTask(void)
{
}

std::string Analyzer::FindAllStringTask::GetName(void) const
{
  return "find all strings";
}

void Analyzer::FindAllStringTask::Run(void)
{
  m_rDoc.ForEachLabel([this](Address const& rAddress, Label const& rLabel)
  {
    if ((rLabel.GetType() & Label::AccessMask) == Label::Imported)
      return;
    if ((rLabel.GetType() & Label::CellMask) != Label::Data)
      return;

    BinaryStream const& rBinStrm = m_rDoc.GetBinaryStream();
    TOffset StrOff;

    if (!m_rDoc.ConvertAddressToFileOffset(rAddress, StrOff))
      return;

    /* UTF-16 */
    static Utf16StringTrait Utf16Str;
    Utf16StringTrait::CharType Utf16Char;
    u16 RawLen = 0;

    while (true)
    {
      if (!rBinStrm.Read(StrOff + RawLen, Utf16Char))
      {
        Log::Write("core") << "Unable to read utf-16 string at " << rAddress << LogEnd;
        return;
      }

      if (!Utf16Str.IsValidCharacter(Utf16Char))
        break;

      RawLen += sizeof(Utf16Char);
    }

    if (Utf16Str.IsFinalCharacter(Utf16Char) && RawLen != 0x0)
    {
      RawLen += sizeof(Utf16Char);
      auto pStrBuf = new u8[RawLen];
      if (!rBinStrm.Read(StrOff, pStrBuf, RawLen))
      {
        Log::Write("core") << "Unable to read utf-16 string at " << rAddress << LogEnd;
        delete [] pStrBuf;
        return;
      }
      std::string CvtStr = Utf16Str.ConvertToUtf8(pStrBuf, RawLen);
      delete [] pStrBuf;
      if (CvtStr.empty())
      {
        Log::Write("core") << "Unable to convert utf-16 string at " << rAddress << LogEnd;
        return;
      }
      auto spString = std::make_shared<String>(String::Utf16Type, RawLen);
      m_rDoc.SetCellWithLabel(rAddress, spString, Label(CvtStr, Label::String | Label::Global), true);
      return;
    }

    /* UTF-8 */
    static Utf8StringTrait Utf8Str;
    Utf8StringTrait::CharType Utf8Char;
    RawLen = 0;
    std::string CurStr;

    while (true)
    {
      if (!rBinStrm.Read(StrOff + RawLen, Utf8Char))
      {
        Log::Write("core") << "Unable to read utf-8 string at " << rAddress << LogEnd;
        return;
      }

      if (!Utf8Str.IsValidCharacter(Utf8Char))
        break;

      RawLen += sizeof(Utf8Char);
      CurStr += Utf8Char;
    }

    if (Utf8Str.IsFinalCharacter(Utf8Char) && RawLen != 0x0)
    {
      RawLen += sizeof(Utf8Char);
      auto spString = std::make_shared<String>(String::Utf8Type, RawLen);
      m_rDoc.SetCellWithLabel(rAddress, spString, Label(CurStr, Label::String | Label::Global), true);
    }
  });
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
  if ((Lbl.GetType() & Label::AccessMask) == Label::Imported)
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

bool Analyzer::MakeAsciiString(Document& rDoc, Address const& rAddr) const
{
  static Utf8StringTrait Utf8Str;
  s8 CurChar;
  TOffset Offset;
  u16 RawLen = 0;
  std::string StrData;
  auto const& rBinStrm = rDoc.GetBinaryStream();

  if (rDoc.ConvertAddressToFileOffset(rAddr.GetOffset(), Offset) == false)
    return false;

  for (;;)
  {
    if (!rBinStrm.Read(Offset + RawLen, CurChar))
    {
      Log::Write("core") << "Unable to read utf-8 string at " << rAddr << LogEnd;
      return false;
    }

    if (CurChar == '\0')
      break;

    StrData += CurChar;
    ++RawLen;
  }

  if (RawLen == 0)
    return false;

  ++RawLen;

  auto spString = std::make_shared<String>(String::Utf8Type, RawLen);
  rDoc.SetCellWithLabel(rAddr, spString, Label(StrData, Label::String | Label::Global), true);

  return true;
}

bool Analyzer::MakeWindowsString(Document& rDoc, Address const& rAddr) const
{
  static Utf16StringTrait Utf16Str;
  Utf16StringTrait::CharType Utf16Char;
  TOffset StrOff;
  u16 RawLen = 0;
  auto const& rBinStrm = rDoc.GetBinaryStream();

  if (rDoc.ConvertAddressToFileOffset(rAddr, StrOff) == false)
    return false;

  for (;;)
  {
    if (!rBinStrm.Read(StrOff + RawLen, Utf16Char))
    {
      Log::Write("core") << "Unable to read utf-16 string at " << rAddr << LogEnd;
      return false;
    }

    if (Utf16Str.IsFinalCharacter(Utf16Char))
      break;

    RawLen += sizeof(Utf16Char);
  }

  if (RawLen == 0x0)
    return false;

  RawLen += sizeof(Utf16Char); // include L'\0'

  auto pStrBuf = new u8[RawLen];
  if (!rBinStrm.Read(StrOff, pStrBuf, RawLen))
  {
    Log::Write("core") << "Unable to read utf-16 string at " << rAddr << LogEnd;
    delete [] pStrBuf;
    return false;
  }
  std::string CvtStr = Utf16Str.ConvertToUtf8(pStrBuf, RawLen);
  delete [] pStrBuf;
  if (CvtStr.empty())
  {
    Log::Write("core") << "Unable to convert utf-16 string at " << rAddr << LogEnd;
    delete [] pStrBuf;
    return false;
  }

  auto spString = std::make_shared<String>(String::Utf16Type, RawLen);
  rDoc.SetCellWithLabel(rAddr, spString, Label(CvtStr, Label::String | Label::Global), true);

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

bool Analyzer::FormatCell(Document const& rDoc, Address const& rAddress, Cell const& rCell, PrintData &rPrintData) const
{
  auto spArch = ModuleManager::Instance().GetArchitecture(rCell.GetArchitectureTag());
  if (spArch == nullptr)
    return false;
  return spArch->FormatCell(rDoc, rAddress, rCell, rPrintData);
}

bool Analyzer::FormatMultiCell(Document const& rDoc, Address const& rAddress, MultiCell const& rMultiCell, PrintData& rPrintData) const
{
  auto spCell = rDoc.GetCell(rAddress);
  if (spCell == nullptr)
    return false;
  auto spArch = ModuleManager::Instance().GetArchitecture(spCell->GetArchitectureTag());
  if (spArch == nullptr)
    return false;
  return spArch->FormatMultiCell(rDoc, rAddress, rMultiCell, rPrintData);
}

void Analyzer::TrackOperand(Document& rDoc, Address const& rStartAddress, Tracker& rTracker)
{
}

void Analyzer::BacktrackOperand(Document& rDoc, Address const& rStartAddress, Tracker& rTracker)
{
}

MEDUSA_NAMESPACE_END
