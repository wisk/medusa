#include "medusa/emulation.hpp"
#include "medusa/module.hpp"

MEDUSA_NAMESPACE_BEGIN

Emulator::Emulator(void) : m_pCpuInfo(nullptr), m_pCpuCtxt(nullptr), m_pMemCtxt(nullptr)
{
}

Emulator::~Emulator(void)
{
}

void Emulator::Initialize(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
{
  m_pCpuInfo = pCpuInfo;
  m_pCpuCtxt = pCpuCtxt;
  m_pMemCtxt = pMemCtxt;
}

bool Emulator::ReadRegister(u32 Reg, void* pVal, u32 BitSize) const
{
  return m_pCpuCtxt->ReadRegister(Reg, pVal, BitSize);
}

template<>
bool Emulator::ReadRegister<bool>(u32 Reg, bool& rRegVal) const
{
  return ReadRegister(Reg, &rRegVal, 1);
}

bool Emulator::WriteRegister(u32 Reg, void const* pVal, u32 BitSize)
{
  return m_pCpuCtxt->WriteRegister(Reg, pVal, BitSize);
}

template<>
bool Emulator::WriteRegister<bool>(u32 Reg, bool RegVal)
{
  return WriteRegister(Reg, &RegVal, 1);
}

bool Emulator::ReadMemory(Address const& rAddr, void* pVal, u32 Size) const
{
  u64 LinAddr;
  if (m_pCpuCtxt->Translate(rAddr, LinAddr) == false)
    LinAddr = rAddr.GetOffset();
  return m_pMemCtxt->ReadMemory(LinAddr, pVal, Size);
}

bool Emulator::WriteMemory(Address const& rAddr, void const* pVal, u32 Size)
{
  u64 LinAddr;
  if (m_pCpuCtxt->Translate(rAddr, LinAddr) == false)
    LinAddr = rAddr.GetOffset();
  return m_pMemCtxt->WriteMemory(LinAddr, pVal, Size);
}

Emulator::ReturnType Emulator::Execute(Expression::SPType spExpr)
{
  return Execute(Expression::VSPType({ spExpr }));
}

Emulator::ReturnType Emulator::Execute(Address const& rAddress)
{
  if (_IsSemanticCached(rAddress))
  {
    auto const& rExprs = m_SemCache[rAddress];
    return Execute(rExprs);
  }

  Expression::VSPType Exprs;
  _Disassemble(rAddress, [&](Address const& rInsnAddr, Instruction& rCurInsn, Architecture& rCurArch, u8 CurMode) -> bool
  {
    auto CurAddr = rCurArch.CurrentAddress(rInsnAddr, rCurInsn);
    Exprs.push_back(Expr::MakeSys("call_insn_cb", rInsnAddr));

    // Set the current IP/PC address
    if (!rCurArch.EmitSetExecutionAddress(Exprs, CurAddr, CurMode))
    {
      Log::Write("core").Level(LogError) << "unable to emit set execution address to " << CurAddr << LogEnd;
      return false;
    }

    for (auto spExpr : rCurInsn.GetSemantic())
      Exprs.push_back(spExpr);

    Exprs.push_back(Expr::MakeSys("check_exec_hook", rInsnAddr));

    // Jump, Call, Return types finish the block
    if (rCurInsn.GetSubType() & (Instruction::JumpType | Instruction::CallType | Instruction::ReturnType))
      return false;

    return true;
  });

  if (!_CacheSemantic(rAddress, Exprs))
    return Error;

  return Execute(Exprs);
}

bool Emulator::AddHook(Address const& rAddress, Emulator::HookType Type, HookCallback Callback)
{
  auto itHook = m_Hooks.find(rAddress);
  if (itHook != std::end(m_Hooks))
  {
    itHook->second.m_Callback = Callback;
    itHook->second.m_Type |= Type;
    return true;
  }

  m_Hooks[rAddress] = HookInformation(Type, Callback);
  return true;
}

bool Emulator::AddHookOnInstruction(HookCallback InsnCb)
{
  m_InsnCb = InsnCb;
  return true;
}

bool Emulator::AddHook(Document const& rDoc, std::string const& rLabelName, Emulator::HookType Type, HookCallback Callback)
{
  auto Addr = rDoc.GetAddressFromLabelName(rLabelName);
  return AddHook(Addr, Type, Callback);
}

bool Emulator::RemoveHook(Address const& rAddress)
{
  auto itHook = m_Hooks.find(rAddress);
  if (itHook == std::end(m_Hooks))
    return false;
  m_Hooks.erase(itHook);
  return true;
}

Emulator::ReturnType Emulator::CallInstructionHook(void)
{
  if (!m_InsnCb)
    return Continue;
  Address CurAddr;
  if (!m_pCpuCtxt->GetAddress(CpuContext::AddressExecution, CurAddr))
  {
    Log::Write("core").Level(LogError) << "failed to get execution address" << LogEnd;
    return Error;
  }
  return m_InsnCb(m_pCpuCtxt, m_pMemCtxt, CurAddr);
}

Emulator::ReturnType Emulator::CallHookOnExecutionIfNeeded(Address const& rAddress) const
{
  auto itHook = m_Hooks.find(rAddress);
  // Requested address is not hooked, continue the emulation
  if (itHook == std::end(m_Hooks))
    return Continue;

  // We only want hook on execution
  if (!(itHook->second.m_Type & HookOnExecute))
    return Continue;

  return itHook->second.m_Callback(m_pCpuCtxt, m_pMemCtxt, rAddress);
}

bool Emulator::InvalidateCache(void)
{
  return false;
}

bool Emulator::_Disassemble(Address const& rAddress, DisasmCbType Cb)
{
  Address InsnAddr = rAddress;
  while (true)
  {
    // Try to translate logical address to linear address
    u64 LinAddr;
    if (!m_pCpuCtxt->Translate(InsnAddr, LinAddr))
    {
      Log::Write("core").Level(LogError) << "unable to translate address: " << InsnAddr << LogEnd;
      return false;
    }

    // To disassemble instruction, we need both a binary stream and offset
    BinaryStream::SPType spBinStrm;
    u32 Off;
    MemoryArea::Access Flags;
    if (!m_pMemCtxt->FindMemory(LinAddr, spBinStrm, Off, Flags))
    {
      Log::Write("core").Level(LogError) << "unable to find memory for: " << InsnAddr << ", linear address: " << LinAddr << LogEnd;
      return false;
    }

    // Retrieve the current mode and architecture module
    auto ArchTag = m_pCpuCtxt->GetCpuInformation().GetArchitectureTag();
    // FIXME(KS): it may not work with thumb mode...
    auto ArchMode = m_pCpuCtxt->GetMode();
    auto& rModMgr = ModuleManager::Instance();
    auto spArch = rModMgr.GetArchitecture(ArchTag);
    if (spArch == nullptr)
    {
      Log::Write("core").Level(LogError) << "unable to find architecture module for: " << InsnAddr << ", linear address: " << LinAddr << LogEnd;
      return false;
    }

    // Disassemble the current instruction and store the result
    auto spCurInsn = std::make_shared<Instruction>();
    if (!spArch->Disassemble(*spBinStrm, Off, *spCurInsn, ArchMode))
    {
      Log::Write("core").Level(LogError) << "unable to disassemble instruction at: " << InsnAddr << ", linear address: " << LinAddr << LogEnd;
      return false;
    }

    if (!Cb(InsnAddr, *spCurInsn, *spArch, ArchMode))
      break;

    // Go to the next instruction
    InsnAddr += spCurInsn->GetSize();
  }

  return true;
}

bool Emulator::_IsSemanticCached(Address const& rAddress) const
{
  return m_SemCache.find(rAddress) != std::end(m_SemCache);
}

bool Emulator::_CacheSemantic(Address const& rAddress, Expression::VSPType& rExprs)
{
  if (rExprs.empty())
    return false;
  if (_IsSemanticCached(rAddress))
    return false;
  m_SemCache[rAddress] = rExprs;
  return true;
}

bool Emulator::_InvalidSemantic(Address const& rAddress)
{
  auto itCache = m_SemCache.find(rAddress);
  if (itCache == std::end(m_SemCache))
    return false;
  m_SemCache.erase(itCache);
  return true;
}

MEDUSA_NAMESPACE_END
