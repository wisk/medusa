#include "medusa/execution.hpp"
#include "medusa/module.hpp"

#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

Execution::Execution(Document& rDoc)
: m_rDoc(rDoc)
, m_spArch(nullptr), m_spOs(nullptr)
, m_pCpuCtxt(nullptr), m_pMemCtxt(nullptr)
{
}

Execution::~Execution(void)
{
  delete m_pCpuCtxt;
  delete m_pMemCtxt;
}

bool Execution::SetCpuMemContext(Address const& StartAddr)
{
  m_pCpuCtxt = m_spArch->MakeCpuContext();
  m_pMemCtxt = m_spArch->MakeMemoryContext();
  if (m_pCpuCtxt == nullptr || m_pMemCtxt == nullptr)
  {
    Log::Write("core").Level(LogError) << "failed to make context for execution" << LogEnd;
    return false;
  }
    m_pCpuCtxt->SetMode(m_rDoc.GetMode(StartAddr));
  m_pCpuInfo = &m_pCpuCtxt->GetCpuInformation();
  if (m_pCpuInfo == nullptr)
  {
    Log::Write("core").Level(LogError) << "unable to get cpu information for execution" << LogEnd;
    return false;
  }

  if (!m_pMemCtxt->MapDocument(m_rDoc, m_pCpuCtxt))
  {
    Log::Write("core").Level(LogError) << "unable to map document for execution" << LogEnd;
    return false;
  }
  return true;
}

bool Execution::Initialize(std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir)
{
  delete m_pCpuCtxt;
  delete m_pMemCtxt;

  auto StartAddr = m_rDoc.GetStartAddress();
  auto const& rModMgr = ModuleManager::Instance();
  m_spArch = rModMgr.GetArchitecture(m_rDoc.GetArchitectureTag(StartAddr));
  if (m_spArch == nullptr)
  {
    Log::Write("core").Level(LogError) << "unable to get architecture module for execution" << LogEnd;
    return false;
  }
  if (SetCpuMemContext(StartAddr) == false)
    {
      Log::Write("core").Level(LogError) << "set cpu memory context failed" << LogEnd;
      return false;
    }
  m_spOs = rModMgr.GetOperatingSystem(m_rDoc.GetOperatingSystemName());
  if (m_spOs == nullptr)
    {
      Log::Write("core").Level(LogError) << "operating system was not found" << LogEnd;
      return SetStackMemory();
    }

  return m_spOs->InitializeContext(m_rDoc, *m_pCpuCtxt, *m_pMemCtxt, rArgs, rEnv, rCurWrkDir);
}

bool Execution::SetStackMemory()
{
  auto const& rCpuInfo = m_pCpuCtxt->GetCpuInformation();
  u64  const StkPtr = 0xdeadbeaf;
  u64  const StkLen = 0x1000;
  auto const ReadWrite = MemoryArea::Access::Read | MemoryArea::Access::Write;

  void* pStkMem;
  if (!m_pMemCtxt->AllocateMemory(StkPtr, StkLen, ReadWrite, &pStkMem)) {
    {
      Log::Write("core").Level(LogError) << "cannot allocate stack memory" << LogEnd;
      return false;
    }
  }

  u32 StkReg = rCpuInfo.GetRegisterByType(CpuInformation::StackPointerRegister, m_pCpuCtxt->GetMode());
  if (StkReg == CpuInformation::InvalidRegister)
    {
      Log::Write("core").Level(LogError) << "invalid stack register" << LogEnd;
      return false;
    }
  u32 StkRegSize = rCpuInfo.GetSizeOfRegisterInBit(StkReg);
  if (StkRegSize < 8)
    {
      Log::Write("core").Level(LogError) << "invalid size of stack register" << LogEnd;
      return false;
    }
  StkRegSize /= 8;

  u64 StkOff = 0;
  u64 StackRegisterValue = StkPtr + StkLen - StkOff;
  if (m_pCpuCtxt->WriteRegister(StkReg, &StackRegisterValue, StkRegSize * 8) == false)
    {
      Log::Write("core").Level(LogError) << "cannot write register in cpu context" << LogEnd;
      return false;
    }
  return true;
}

bool Execution::SetEmulator(std::string const& rEmulatorName)
{
  if (m_spArch == nullptr)
    {
      Log::Write("core").Level(LogError) << "architecture was not found" << LogEnd;
      return false;
    }
  if (m_pCpuCtxt == nullptr || m_pMemCtxt == nullptr)
    {
      Log::Write("core").Level(LogError) << "cpu context or memory context is null" << LogEnd;
      return false;
    }
  auto pGetEmulator = ModuleManager::Instance().GetEmulator(rEmulatorName);
  if (pGetEmulator == nullptr)
    {
      Log::Write("core").Level(LogError) << "cannot get emulator" << LogEnd;
      return false;
    }
  auto pEmul = pGetEmulator(m_spArch->GetCpuInformation(), m_pCpuCtxt, m_pMemCtxt);
  if (pEmul == nullptr)
    {
      Log::Write("core").Level(LogError) << "cannot get emulator" << LogEnd;
      return false;
    }
  m_spEmul = Emulator::SPType(pEmul);
  return true;
}

bool Execution::Execute(Address const& rAddr)
{
  if (m_spEmul == nullptr)
  {
    Log::Write("core").Level(LogError) << "emulator is null for execution" << LogEnd;
    return false;
  }

  if (!m_pCpuCtxt->SetAddress(CpuContext::AddressExecution, rAddr))
  {
    Log::Write("core").Level(LogError) << "failed to set address " << rAddr << LogEnd;
    return false;
  }

  Address CurAddr = rAddr;
  Emulator::ReturnType Status;
  while ((Status = m_spEmul->Execute(CurAddr)) != Emulator::Stop)
  {
    if (Status == Emulator::Error)
    {
      Log::Write("core").Level(LogError) << "execution error occured at basic block: " << CurAddr << LogEnd;
      return false;
    }
    if (!m_pCpuCtxt->GetAddress(CpuContext::AddressExecution, CurAddr))
    {
      Log::Write("core").Level(LogError) << "failed to get the next address for basic block: " << CurAddr << LogEnd;
      return false;
    }
  }
  return true;
}

bool Execution::InvalidateCache(void)
{
  if (m_spEmul == nullptr)
    return false;
  return m_spEmul->InvalidateCache();
}

bool Execution::HookInstruction(Emulator::HookCallback HkCb)
{
  if (m_spEmul == nullptr)
    return false;
  m_spEmul->AddHookOnInstruction(HkCb);
  return true;
}

bool Execution::HookFunction(std::string const& rFuncName, Emulator::HookCallback HkCb)
{
  if (m_spEmul == nullptr)
    {
      Log::Write("core").Level(LogError) << "emulator is null" << LogEnd;
      return false;
    }

  static u64 s_FakeAddr = 0xdead7700; // FIXME: this is a dirty hack
  auto const& rAddr     = m_rDoc.GetAddressFromLabelName(rFuncName);
  auto const& rLbl      = m_rDoc.GetLabelFromAddress(rAddr);

  if (!rLbl.IsImported())
    {
      Log::Write("core").Level(LogError) << "function: " << rFuncName << " is not imported" << LogEnd;
      return false;
    }
  // FIXME(wisk): sometime it's almost impossible to determine if a imported symbol is a function or a variable...
  //if (!rLbl.IsFunction())
  //  return false;

  auto const* pCpuInfo = m_spArch->GetCpuInformation();
  if (pCpuInfo == nullptr)
    {
      Log::Write("core").Level(LogError) << "cpu info is null" << LogEnd;
      return false;
    }

  auto PcSize = pCpuInfo->GetSizeOfRegisterInBit(pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_rDoc.GetMode(rAddr))) / 8;
  if (PcSize == 0)
    {
      Log::Write("core").Level(LogError) << "pc size is null zero" << LogEnd;
      return false;
    }

  if (!m_spEmul->WriteMemory(rAddr, &s_FakeAddr, PcSize))
    {
      Log::Write("core").Level(LogError) << "can't write memory" << LogEnd;
      return false;
    }

  {
    std::lock_guard<std::mutex> Lock(m_HookMutex);
    m_HookName[s_FakeAddr] = rFuncName;
  }

  if (!m_spEmul->AddHook(s_FakeAddr, Emulator::HookOnExecute, HkCb))
    {
      Log::Write("core").Level(LogError) << "can't add hook" << LogEnd;
      return false;
    }

  s_FakeAddr += 4;

  return true;
}

bool Execution::Hook(Address const& rAddress, Emulator::HookType Type, Emulator::HookCallback Callback)
{
  if (m_spEmul == nullptr)
    return false;
  m_spEmul->AddHook(rAddress, Type, Callback);
  return true;
}

bool Execution::Hook(std::string const& rLabelName, Emulator::HookType Type, Emulator::HookCallback Callback)
{
  if (m_spEmul == nullptr)
    return false;
  m_spEmul->AddHook(m_rDoc, rLabelName, Type, Callback);
  return true;
}

std::string Execution::GetHookName(void) const
{
  u32 PrgReg = m_pCpuCtxt->GetCpuInformation().GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
  if (PrgReg == 0)
    return "";
  auto PrgRegSize = m_pCpuCtxt->GetCpuInformation().GetSizeOfRegisterInBit(PrgReg);
  if (PrgRegSize == 0)
    return "";
  u64 PrgRegVal = 0;
  if (!m_pCpuCtxt->ReadRegister(PrgReg, &PrgRegVal, PrgRegSize))
    return "";

  std::lock_guard<std::mutex> Lock(m_HookMutex);
  auto itHookPair = m_HookName.find(PrgRegVal);
  if (itHookPair == std::end(m_HookName))
    return "";
  return itHookPair->second;
}

Address Execution::GetHookAddress(std::string const& rHkFuncName) const
{
  std::lock_guard<std::mutex> Lock(m_HookMutex);
  for (auto const& rHkPr : m_HookName)
    if (rHkPr.second == rHkFuncName)
      return rHkPr.first;
  return Address();
}

bool Execution::GetCallingConvention(std::string& rCallingConvention, Address const& rAddress) const
{
  // TODO(wisk): try to retrieve function detail first
  if (m_spOs == nullptr)
    return false;
  return m_spOs->GetDefaultCallingConvention(m_rDoc, rCallingConvention, rAddress);
}

bool Execution::GetFunctionParameter(std::string const& rCallConv, u16 ParamNo, BitVector& rParamValue) const
{
  auto pCallConv = m_spArch->GetCallingConvention(rCallConv, m_pCpuCtxt->GetMode());
  if (pCallConv == nullptr)
  {
    Log::Write("core").Level(LogError) << "unable to find calling convention: " << rCallConv << LogEnd;
    return false;
  }

  return pCallConv->GetParameter(m_pCpuCtxt, m_pMemCtxt, ParamNo, rParamValue);
}

bool Execution::ReturnFromFunction(std::string const& rCallConv, u16 ParamNo) const
{
  auto pCallConv = m_spArch->GetCallingConvention(rCallConv, m_pCpuCtxt->GetMode());
  if (pCallConv == nullptr)
  {
    Log::Write("core").Level(LogError) << "unable to find calling convention: " << rCallConv << LogEnd;
    return false;
  }

  return pCallConv->ReturnFromFunction(m_pCpuCtxt, m_pMemCtxt, ParamNo);
}

bool Execution::ReturnValueFromFunction(std::string const& rCallConv, u16 ParamNo, BitVector const& rRetVal) const
{
  auto pCallConv = m_spArch->GetCallingConvention(rCallConv, m_pCpuCtxt->GetMode());
  if (pCallConv == nullptr)
  {
    Log::Write("core").Level(LogError) << "unable to find calling convention: " << rCallConv << LogEnd;
    return false;
  }

  return pCallConv->ReturnValueFromFunction(m_pCpuCtxt, m_pMemCtxt, ParamNo, rRetVal);
}

MEDUSA_NAMESPACE_END
