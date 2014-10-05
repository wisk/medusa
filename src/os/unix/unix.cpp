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
  // BROKEN
  //auto const pFunc = dynamic_cast<Function const*>(rDoc.GetMultiCell(rAddress));
  //if (pFunc == nullptr)
  //  return false;

  //if (pFunc->GetInstructionCounter() != 3)
  //  return false;

  //auto Tag = rDoc.GetArchitectureTag(rAddress);
  //auto const spArch = ModuleManager::Instance().GetArchitecture(Tag);
  //if (spArch == nullptr)
  //  return false;

  //if (spArch->GetName() != "ARM")
  //  return false;

  //// TODO: make helper to do this...
  //Expression::List FuncSem;
  //Address CurAddr = rAddress;

  //Address DstAddr = rAddress + 8; // add ip, pc, #0x00000000

  //auto const spAddIpIpImm = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddress + 4));
  //auto const spLdrPcIpImm = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddress + 8));
  //if (spAddIpIpImm == nullptr || spLdrPcIpImm == nullptr)
  //  return true;
  //DstAddr += spAddIpIpImm->Operand(2)->GetValue(); // add ip, ip, #IMM
  //DstAddr += spLdrPcIpImm->Operand(1)->GetValue(); // ldr pc, [ip, #IMM]


  //auto DstLbl = rDoc.GetLabelFromAddress(DstAddr);
  //if (DstLbl.GetType() == Label::Unknown)
  //  return true;

  //rDoc.SetLabelToAddress(rAddress, Label("b_" + DstLbl.GetLabel(), Label::Function | Label::Global));

  return true;
}

Expression::List UnixOperatingSystem::ExecuteSymbol(Document& rDoc, Address const& rSymAddr)
{
  return Expression::List();
}

bool UnixOperatingSystem::GetValueDetail(Id ValueId, ValueDetail& rValDtl) const
{
  return false;
}

bool UnixOperatingSystem::GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const
{
  return false;
}

bool UnixOperatingSystem::GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const
{
  return false;
}

