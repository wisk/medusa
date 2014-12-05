#include "unix.hpp"
#include <medusa/module.hpp>
#include <medusa/instruction.hpp>
#include <medusa/function.hpp>
#include <medusa/execution.hpp>
#include <medusa/expression.hpp>
#include <medusa/expression_visitor.hpp>
#include <medusa/emulation.hpp>

std::string UnixOperatingSystem::GetName(void) const
{
  // TODO: add more detail like if it's linux/*BSD/...
  return "UNIX";
}

// TODO: It's not enough to test if the loader is ElfLoader
bool UnixOperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  if (rLdr.GetName().compare(0, 3, "ELF") == 0)
    return true;

  return false;
}

bool UnixOperatingSystem::InitializeContext(
  Document const& rDoc,
  CpuContext& rCpuCtxt, MemoryContext& rMemCtxt,
  std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir) const
{
  auto const& rCpuInfo = rCpuCtxt.GetCpuInformation();
  auto const StkPtr = 0xbedb4000;
  auto const StkLen = 0x21000;

  if (rMemCtxt.AllocateMemory(StkPtr, StkLen, nullptr) == false)
    return false;

  // TODO: properly implement argc, argv, envp

  u64 StackRegisterValue = StkPtr + StkLen - 4;
  u32 StkReg = rCpuInfo.GetRegisterByType(CpuInformation::StackPointerRegister, rCpuCtxt.GetMode());
  if (StkReg == CpuInformation::InvalidRegister)
    return false;
  u32 StkRegSize = rCpuInfo.GetSizeOfRegisterInBit(StkReg);
  if (StkRegSize < 8)
    return false;
  StkRegSize /= 8;
  if (rCpuCtxt.WriteRegister(StkReg, &StackRegisterValue, StkRegSize) == false)
    return false;

  return true;
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

  auto const spAdrIpImm   = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddress + 0));
  auto const spAddIpIpImm = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddress + 4));
  auto const spLdrPcIpImm = std::dynamic_pointer_cast<Instruction const>(rDoc.GetCell(rAddress + 8));
  if (spAdrIpImm == nullptr || spAddIpIpImm == nullptr || spLdrPcIpImm == nullptr)
    return true;

  // TODO: execute this part
  auto spBase = expr_cast<ConstantExpression>(spAdrIpImm->GetOperand(1));
  auto spDisp = expr_cast<ConstantExpression>(spAddIpIpImm->GetOperand(2));
  auto spMem  = expr_cast<MemoryExpression>(spLdrPcIpImm->GetOperand(1));

  if (spBase == nullptr || spDisp == nullptr || spMem == nullptr)
    return true;

  auto spOprt = expr_cast<OperationExpression>(spMem->GetOffsetExpression());
  if (spOprt == nullptr)
    return true;
  auto spOff = expr_cast<ConstantExpression>(spOprt->GetRightExpression());

  Address DstAddr(
    Address::FlatType,
    0x0,
    static_cast<u32>(spBase->GetConstant() + spDisp->GetConstant() + spOff->GetConstant()),
    0, 32);

  EvaluateVisitor EvalVst(rDoc, spArch->CurrentAddress(rAddress, *spAdrIpImm), spAdrIpImm->GetMode(), true);

  auto DstLbl = rDoc.GetLabelFromAddress(DstAddr);
  if (DstLbl.GetType() == Label::Unknown)
    return true;

  rDoc.SetLabelToAddress(rAddress, Label("b_" + DstLbl.GetLabel(), Label::Function | Label::Global));

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

