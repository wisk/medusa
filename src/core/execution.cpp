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

  m_spOs = rModMgr.GetOperatingSystem(m_rDoc.GetOperatingSystemName());
  if (m_spOs == nullptr)
    return true;

  return m_spOs->InitializeContext(m_rDoc, *m_pCpuCtxt, *m_pMemCtxt, rArgs, rEnv, rCurWrkDir);
}

bool Execution::SetEmulator(std::string const& rEmulatorName)
{
  if (m_spArch == nullptr)
    return false;
  if (m_pCpuCtxt == nullptr || m_pMemCtxt == nullptr)
    return false;
  auto pGetEmulator = ModuleManager::Instance().GetEmulator(rEmulatorName);
  if (pGetEmulator == nullptr)
    return false;
  auto pEmul = pGetEmulator(m_spArch->GetCpuInformation(), m_pCpuCtxt, m_pMemCtxt);
  if (pEmul == nullptr)
    return false;
  m_spEmul = Emulator::SPType(pEmul);
  return true;
}

void Execution::Execute(Address const& rAddr)
{
  if (m_spEmul == nullptr)
  {
    Log::Write("core").Level(LogError) << "emulator is null for execution" << LogEnd;
    return;
  }

  if (!m_pCpuCtxt->SetAddress(CpuContext::AddressExecution, rAddr))
  {
    Log::Write("core").Level(LogError) << "failed to set address " << rAddr << LogEnd;
    return;
  }

  Address CurAddr = rAddr;
  while (m_spEmul->Execute(CurAddr))
    if (!m_pCpuCtxt->GetAddress(CpuContext::AddressExecution, CurAddr))
      break;
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
    return false;

  static u64 s_FakeAddr = 0xdead7700; // FIXME: this is a dirty hack
  auto const& rAddr   = m_rDoc.GetAddressFromLabelName(rFuncName);
  auto const& rLbl    = m_rDoc.GetLabelFromAddress(rAddr);

  if (rLbl.GetType() & (Label::Imported | Label::Function))
  {
    auto const* pCpuInfo = m_spArch->GetCpuInformation();
    if (pCpuInfo == nullptr)
      return false;

    auto PcSize = pCpuInfo->GetSizeOfRegisterInBit(pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_rDoc.GetMode(rAddr))) / 8;
    if (PcSize == 0)
      return false;

    if (!m_spEmul->WriteMemory(rAddr, &s_FakeAddr, PcSize))
      return false;
  }

  {
    std::lock_guard<std::mutex> Lock(m_HookMutex);
    m_HookName[s_FakeAddr] = rFuncName;
  }

  if (!m_spEmul->AddHook(s_FakeAddr, Emulator::HookOnExecute, HkCb))
    return false;

  s_FakeAddr += 4;

  return true;
}

bool Execution::Hook(Address const& rAddress, u32 Type, Emulator::HookCallback Callback)
{
  if (m_spEmul == nullptr)
    return false;
  m_spEmul->AddHook(rAddress, Type, Callback);
  return true;
}

bool Execution::Hook(std::string const& rLabelName, u32 Type, Emulator::HookCallback Callback)
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

MEDUSA_NAMESPACE_END
