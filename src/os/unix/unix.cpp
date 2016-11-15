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
// TODO(wisk): handle stack cookie for linux
// - https://stackoverflow.com/questions/10325713/why-does-this-memory-address-have-a-random-value
// - https://fossies.org/dox/glibc-2.22/structtcbhead__t.html
bool UnixOperatingSystem::InitializeContext(
  Document const& rDoc,
  CpuContext& rCpuCtxt, MemoryContext& rMemCtxt,
  std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir) const
{
  auto const& rCpuInfo = rCpuCtxt.GetCpuInformation();
  u64  const StkPtr = 0xbedb4000;
  u64  const StkLen = 0x21000;
  auto const ReadWrite = MemoryArea::Access::Read | MemoryArea::Access::Write;

  void* pStkMem;
  if (!rMemCtxt.AllocateMemory(StkPtr, StkLen, ReadWrite, &pStkMem))
    return false;

  u32 StkReg = rCpuInfo.GetRegisterByType(CpuInformation::StackPointerRegister, rCpuCtxt.GetMode());
  if (StkReg == CpuInformation::InvalidRegister)
    return false;
  u32 StkRegSize = rCpuInfo.GetSizeOfRegisterInBit(StkReg);
  if (StkRegSize < 8)
    return false;
  StkRegSize /= 8; // Set stack register size in byte

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
  /* Align the stack */
  StkOff = (StkOff + StkRegSize) & ~(StkRegSize - 1);
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

  if (rCpuCtxt.WriteRegister(StkReg, &StackRegisterValue, StkRegSize * 8) == false)
    return false;

  return true;
}

bool UnixOperatingSystem::ProvideDetails(Document& rDoc) const
{
  return true;
}

bool UnixOperatingSystem::AnalyzeFunction(Document& rDoc, Address const& rAddress)
{
  return false;
}

Expression::VSPType UnixOperatingSystem::ExecuteSymbol(Document& rDoc, Address const& rSymAddr)
{
  return {};
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

bool UnixOperatingSystem::GetDefaultCallingConvention(Document const& rDoc, std::string& rCallingConvention, Address const& rAddress) const
{
  auto spInsn = rDoc.GetCell(rAddress); // TODO(wisk): make sure this is an instruction
  if (spInsn == nullptr)
    return false;
  auto ArchTag = spInsn->GetArchitectureTag();
  auto Mode = spInsn->GetMode();

  auto const& rModMgr = ModuleManager::Instance();
  auto spArch = rModMgr.GetArchitecture(ArchTag);

  if (spArch->GetName() == "x86")
  {
    switch (Mode)
    {
    default:
      return false;
    case 16:
    case 32:
      rCallingConvention = "cdecl";
      return true;
    case 64:
      rCallingConvention = "system_v";
      return true;
    }
  }

  return false;
}