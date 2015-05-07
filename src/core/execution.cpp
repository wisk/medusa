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
  auto pGetEmulator = ModuleManager::Instance().GetEmulator(rEmulatorName);
  if (pGetEmulator == nullptr)
    return false;
  auto pEmul = pGetEmulator(m_spArch->GetCpuInformation(), m_pCpuCtxt, m_pMemCtxt);
  if (pEmul == nullptr)
    return false;
  m_spEmul = Emulator::SPType(pEmul);
  return true;
}

static bool DisassembleInstruction(Document& rDoc, Architecture::SPType spArch, u8 Mode, Address const& rAddr)
{
  if (std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(rAddr)) != nullptr)
    return true;
  Log::Write("exec").Level(LogDebug) << "not an instruction, try to disassemble it: " << rAddr << LogEnd;
  TOffset CurOff;
  if (!rDoc.ConvertAddressToFileOffset(rAddr, CurOff))
  {
    Log::Write("exec") << "instruction at " << rAddr.ToString() << " is not contained in file" << LogEnd;
    return false;
  }
  auto spInsn = std::make_shared<Instruction>();
  if (!spArch->Disassemble(rDoc.GetBinaryStream(), CurOff, *spInsn, Mode))
  {
    Log::Write("exec") << "unable to disassemble instruction at " << rAddr.ToString() << LogEnd;
    return false;
  }

  if (!rDoc.SetCell(rAddr, spInsn, true))
  {
    Log::Write("exec") << "unable to set a instruction at " << rAddr.ToString() << LogEnd;
    return false;
  }
  return true;
}

void Execution::Execute(Address const& rAddr)
{
  if (m_spArch == nullptr)
  {
    Log::Write("core").Level(LogError) << "architecture is null for execution" << LogEnd;
    return;
  }
  if (m_spEmul == nullptr)
  {
    Log::Write("core").Level(LogError) << "emulator is null for execution" << LogEnd;
    return;
  }

  if (!DisassembleInstruction(m_rDoc, m_spArch, m_rDoc.GetMode(rAddr), rAddr))
  {
    Log::Write("core").Level(LogError) << "unable to disassemble instruction at " << rAddr << LogEnd;
    return;
  }

  auto spCell = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(rAddr));
  if (spCell == nullptr)
  {
    Log::Write("core").Level(LogError) << "unable to retrieve cell at " << rAddr << LogEnd;
    return;
  }
  m_pCpuCtxt->SetMode(spCell->GetMode());

  Address CurAddr = rAddr;

  u32 ProgPtrReg = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
  if (ProgPtrReg == CpuInformation::InvalidRegister)
    return;
  u32 ProgPtrRegSize = m_pCpuInfo->GetSizeOfRegisterInBit(ProgPtrReg);
  if (ProgPtrRegSize < 8)
    return;

  u64 CurInsn = rAddr.GetOffset();
  if (m_pCpuCtxt->WriteRegister(ProgPtrReg, &CurInsn, ProgPtrRegSize) == false)
    return;

  static std::unordered_map<Address, Expression::LSPType> s_Cache;

  while (true)
  {
    Address BlkAddr = CurAddr;
    Expression::LSPType& Sems = s_Cache[BlkAddr];
    if (Sems.empty())
    {
      while (true)
      {

        auto spCurInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));
        if (spCurInsn == nullptr)
        {
          DisassembleInstruction(m_rDoc, m_spArch, m_pCpuCtxt->GetMode(), CurAddr);
        }

        spCurInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));
        if (spCurInsn == nullptr)
        {

          Log::Write("exec") << "unable to get instruction at " << CurAddr << LogEnd;
          Log::Write("exec") << "execution finished\n" << m_pCpuCtxt->ToString() << "\n" << /*m_pMemCtxt->ToString() <<*/ LogEnd;
          return;
        }

        Address PcAddr = m_spArch->CurrentAddress(CurAddr, *spCurInsn);

        Sems.push_back(Expr::MakeSys("dump_insn", CurAddr));

        // TODO: I'm not really satisfied with this method
        // it's not enough generic
        Sems.push_back(Expr::MakeAssign(
          Expr::MakeId(ProgPtrReg, m_pCpuInfo),
          Expr::MakeConst(IntType(PcAddr.GetOffsetSize(), PcAddr.GetOffset()))));

        CurAddr.SetOffset(CurAddr.GetOffset() + spCurInsn->GetLength());

        auto const& rCurSem = spCurInsn->GetSemantic();
        if (rCurSem.empty())
        {
          Log::Write("exec").Level(LogError) << "no semantic available: " << spCurInsn->ToString() << LogEnd;
          Sems.push_back(Expr::MakeSys("stop", CurAddr));
        }
        std::for_each(std::begin(rCurSem), std::end(rCurSem), [&](Expression::SPType spExpr)
        {
          Sems.push_back(spExpr->Clone());
        });

        Sems.push_back(Expr::MakeSys("check_exec_hook", PcAddr));

        if (spCurInsn->GetSubType() == Instruction::NoneType)
          continue;
        if (spCurInsn->GetSubType() & Instruction::JumpType
          && !(spCurInsn->GetSubType() & Instruction::ConditionalType)
          && expr_cast<ConstantExpression>(spCurInsn->GetOperand(0)) != nullptr)
          continue;
        if (spCurInsn->GetSubType() & Instruction::CallType
          && !(spCurInsn->GetSubType() & Instruction::ConditionalType)
          && expr_cast<ConstantExpression>(spCurInsn->GetOperand(0)) != nullptr)
          continue;

        break;
      };

      s_Cache[BlkAddr] = Sems;
    }

    bool Res = m_spEmul->Execute(BlkAddr, Sems);

    if (Res == false)
    {
      Log::Write("exec") << "failed to execute block " << BlkAddr << LogEnd;
      Log::Write("exec") << "execution finished\n" << m_pCpuCtxt->ToString() << /*"\n" << m_pMemCtxt->ToString() <<*/ LogEnd;
      break;
    }

    u64 NextInsn = 0;
    if (!m_pCpuCtxt->ReadRegister(ProgPtrReg, &NextInsn, ProgPtrRegSize))
      break;
    CurAddr.SetOffset(NextInsn);
  }
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