#include "unix.hpp"
#include <medusa/module.hpp>
#include <medusa/instruction.hpp>
#include <medusa/function.hpp>
#include <medusa/execution.hpp>
#include <medusa/expression.hpp>
#include <medusa/expression_visitor.hpp>
#include <medusa/emulation.hpp>

#include <boost/range/adaptor/reversed.hpp>

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

//http://asm.sourceforge.net/articles/startup.html
//https://grugq.github.io/docs/ul_exec.txt
// TODO: Make sure StkLen is large enough...
bool UnixOperatingSystem::InitializeContext(
  Document const& rDoc,
  CpuContext& rCpuCtxt, MemoryContext& rMemCtxt,
  std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir) const
{
  auto const& rCpuInfo = rCpuCtxt.GetCpuInformation();
  u64 const StkPtr = 0xbedb4000;
  u64 const StkLen = 0x21000;

  void* pStkMem;
  if (!rMemCtxt.AllocateMemory(StkPtr, StkLen, &pStkMem))
    return false;

  u32 StkReg = rCpuInfo.GetRegisterByType(CpuInformation::StackPointerRegister, rCpuCtxt.GetMode());
  if (StkReg == CpuInformation::InvalidRegister)
    return false;
  u32 StkRegSize = rCpuInfo.GetSizeOfRegisterInBit(StkReg);
  if (StkRegSize < 8)
    return false;

  u64 StkOff = 0;
  u64 NullPtr = 0x0;

  /* Write environ data */
  std::vector<u64> EnvPtr;
  EnvPtr.reserve(rEnv.size());
  for (auto const& rEnvVar : boost::adaptors::reverse(rEnv))
  {
    auto EnvVarLen = rEnvVar.length() + 1;
    StkOff += EnvVarLen;
    ::memcpy(reinterpret_cast<u8*>(pStkMem) + StkLen - StkOff, rEnvVar.c_str(), EnvVarLen);
    EnvPtr.push_back(StkPtr + StkLen - StkOff);
  }
  /* Write arguments data */
  std::vector<u64> ArgsPtr;
  ArgsPtr.reserve(rArgs.size());
  for (auto const& rArgVar : boost::adaptors::reverse(rArgs))
  {
    auto ArgVarLen = rArgVar.length() + 1;
    StkOff += ArgVarLen;
    ::memcpy(reinterpret_cast<u8*>(pStkMem) + StkLen - StkOff, rArgVar.c_str(), ArgVarLen);
    ArgsPtr.push_back(StkPtr + StkLen - StkOff);
  }
  /* Write environ pointers (envp) */
  StkOff += StkRegSize;
  ::memcpy(reinterpret_cast<u8*>(pStkMem) + StkLen - StkOff, &NullPtr, StkRegSize);
  for (auto CurEnvPtr : EnvPtr)
  {
    StkOff += StkRegSize;
    ::memcpy(reinterpret_cast<u8*>(pStkMem) + StkLen - StkOff, &CurEnvPtr, StkRegSize);
  }
  /* Write argument pointers (argv) */
  StkOff += StkRegSize;
  ::memcpy(reinterpret_cast<u8*>(pStkMem) + StkLen - StkOff, &NullPtr, StkRegSize);
  for (auto CurArgPtr : ArgsPtr)
  {
    StkOff += StkRegSize;
    ::memcpy(reinterpret_cast<u8*>(pStkMem) + StkLen - StkOff, &CurArgPtr, StkRegSize);
  }
  /* Write arguments count (argc) */
  StkOff += StkRegSize;
  u64 ArgCnt = ArgsPtr.size();
  ::memcpy(reinterpret_cast<u8*>(pStkMem) + StkLen - StkOff, &ArgCnt, StkRegSize);

  u64 StackRegisterValue = StkPtr + StkLen - StkOff;

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

  auto spBinOp = expr_cast<BinaryOperationExpression>(spMem->GetOffsetExpression());
  if (spBinOp == nullptr)
    return true;
  auto spOff = expr_cast<ConstantExpression>(spBinOp->GetRightExpression());

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

Expression::LSPType UnixOperatingSystem::ExecuteSymbol(Document& rDoc, Address const& rSymAddr)
{
  return Expression::LSPType();
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

