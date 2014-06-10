#include "unix.hpp"
#include <medusa/module.hpp>
#include <medusa/instruction.hpp>
#include <medusa/function.hpp>
#include <medusa/expression.hpp>
#include <medusa/emulation.hpp>

std::string UnixOperatingSystem::GetName(void) const
{
  // TODO: add more detail like if it's linux/*BSD/...
  return "UNIX";
}

bool UnixOperatingSystem::InitializeCpuContext(Document const& rDoc, CpuContext& rCpuCtxt) const
{
  return true;
}

bool UnixOperatingSystem::InitializeMemoryContext(Document const& rDoc, MemoryContext& rMemCtxt) const
{
  return true;
}

bool UnixOperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  if (rLdr.GetName().compare(0, 3, "ELF") == 0)
    return true;

  return false;
}

bool UnixOperatingSystem::ProvideDetails(Document& rDoc) const
{
  return true;
}

bool UnixOperatingSystem::AnalyzeFunction(Document& rDoc, Address const& rAddress)
{
  auto const pFunc = dynamic_cast<Function const*>(rDoc.GetMultiCell(rAddress));
  if (pFunc == nullptr)
    return false;

  if (pFunc->GetInstructionCounter() != 3)
    return false;

  auto Tag = rDoc.GetArchitectureTag(rAddress);
  auto const spArch = ModuleManager::Instance().GetArchitecture(Tag);
  if (spArch == nullptr)
    return false;

  if (spArch->GetName() != "ARM")
    return false;

  // TODO: make helper to do this...
  Expression::List FuncSem;
  Address CurAddr = rAddress;

  for (u8 i = 0; i < 3; ++i)
  {
    auto const spCurInsn = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(CurAddr));
    CurAddr += spCurInsn->GetLength();
    Expression::List CurSem = spCurInsn->GetSemantic();
    for (auto const pSem : CurSem)
      FuncSem.push_back(pSem->Clone());
  }

  auto spEmulCtor = ModuleManager::Instance().GetEmulator("interpreter");

  auto pCpuCtxt = spArch->MakeCpuContext();
  auto pMemCtxt = spArch->MakeMemoryContext();
  auto pEmul = spEmulCtor(&pCpuCtxt->GetCpuInformation(), pCpuCtxt, pMemCtxt);

  auto IdPc = pCpuCtxt->GetCpuInformation().GetRegisterByType(CpuInformation::ProgramPointerRegister, spArch->GetDefaultMode(rAddress));
  u64 CurOff = rAddress.GetOffset();
  pCpuCtxt->WriteRegister(
    IdPc,
    &CurOff, 4);
  pMemCtxt->MapDocument(rDoc, pCpuCtxt);

  for (auto const& Sem : FuncSem)
  {
    pEmul->Execute(rAddress, *Sem);
    Log::Write("os_unix") << "debug: " << Sem->ToString() << "\n" << pCpuCtxt->ToString() << LogEnd;
    pCpuCtxt->ReadRegister(
      IdPc,
      &CurOff, 4);
    CurOff += 4;
    pCpuCtxt->WriteRegister(
      IdPc,
      &CurOff, 4);
  }

  Address DstAddr = rAddress;
  DstAddr.SetOffset(CurOff);
  auto DstLbl = rDoc.GetLabelFromAddress(DstAddr);
  if (DstLbl.GetType() != Label::Unknown)
  {
    rDoc.SetLabelToAddress(rAddress, Label("b_" + DstLbl.GetName(), Label::Function));
  }

  delete pEmul;
  delete pMemCtxt;
  delete pCpuCtxt;

  return true;
}
