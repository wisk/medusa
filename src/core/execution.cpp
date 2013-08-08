#include "medusa/execution.hpp"

#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

Execution::Execution(Medusa* pCore, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs)
: m_pCore(pCore)
, m_spArch(spArch), m_spOs(spOs)
, m_pCpuCtxt(nullptr), m_pMemCtxt(nullptr), m_pVarCtxt(new VariableContext)
, m_pCpuInfo(spArch->GetCpuInformation())
{
}

Execution::~Execution(void)
{
}

bool Execution::Initialize(u64 StackLinearAddress, u32 StackSize)
{
  delete m_pCpuCtxt;
  delete m_pMemCtxt;

  m_pCpuCtxt = m_spArch->MakeCpuContext();
  m_pMemCtxt = m_spArch->MakeMemoryContext();
  if (m_spOs != nullptr)
  {
    m_spOs->InitializeCpuContext(*m_pCpuCtxt);
    m_spOs->InitializeMemoryContext(*m_pMemCtxt);
  }

  if (m_pMemCtxt->MapDocument(m_pCore->GetDocument(), m_pCpuCtxt) == false)
    return false;

  if (m_pMemCtxt->AllocateMemory(StackLinearAddress, StackSize, nullptr) == false)
    return false;

  u64 StackRegisterValue = StackLinearAddress + StackSize;
  u32 StkReg = m_pCpuInfo->GetRegisterByType(CpuInformation::StackPointerRegister);
  if (StkReg == CpuInformation::InvalidRegister)
    return false;
  u32 StkRegSize = m_pCpuInfo->GetSizeOfRegisterInBit(StkReg);
  if (StkRegSize < 8)
    return false;
  StkRegSize /= 8;
  if (m_pCpuCtxt->WriteRegister(StkReg, &StackRegisterValue, StkRegSize) == false)
    return false;

  return true;
}

bool Execution::SetEmulator(std::string const& rEmulatorName)
{
  auto const& Emulators = m_pCore->GetEmulators();
  auto pGetEmulator = Emulators.find(rEmulatorName)->second;
  if (pGetEmulator == nullptr)
    return false;
  auto pEmul = pGetEmulator(m_spArch->GetCpuInformation(), m_pCpuCtxt, m_pMemCtxt, m_pVarCtxt);
  if (pEmul == nullptr)
    return false;
  m_spEmul = Emulator::SharedPtr(pEmul);
  return true;
}

void Execution::Execute(Address const& rAddr)
{
  if (m_spEmul == nullptr)
    return;

  Address CurAddr = rAddr;
  Instruction* pCurInsn = nullptr;

  u32 ProgPtrReg = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister);
  if (ProgPtrReg == CpuInformation::InvalidRegister)
    return;
  u32 ProgPtrRegSize = m_pCpuInfo->GetSizeOfRegisterInBit(ProgPtrReg);
  if (ProgPtrRegSize < 8)
    return;
  ProgPtrRegSize /= 8;

  u64 CurInsn = rAddr.GetOffset();
  if (m_pCpuCtxt->WriteRegister(ProgPtrReg, &CurInsn, ProgPtrRegSize) == false)
    return;

  while (true)
  {
    Log::Write("exec") << m_pCpuCtxt->ToString() << LogEnd;

    Expression::List Sems;
    while (true)
    {
      pCurInsn = dynamic_cast<Instruction*>(m_pCore->GetCell(CurAddr));
      if (pCurInsn == nullptr)
        break;

      auto const& rCurSem = pCurInsn->GetSemantic();
      std::for_each(std::begin(rCurSem), std::end(rCurSem), [&](Expression const* pExpr)
      { Sems.push_back(pExpr->Clone()); });

      if (pCurInsn->GetOperationType() != Instruction::OpUnknown)
        break;

      Sems.push_back(new OperationExpression(OperationExpression::OpAff,
        new IdentifierExpression(ProgPtrReg, m_pCpuInfo),
        new OperationExpression(OperationExpression::OpAdd,
        /**/new IdentifierExpression(ProgPtrReg, m_pCpuInfo),
        /**/new ConstantExpression(ProgPtrRegSize * 8, pCurInsn->GetLength())
        )));
      CurAddr.SetOffset(CurAddr.GetOffset() + pCurInsn->GetLength());
    };

    bool Res = m_spEmul->Execute(Sems);
    std::for_each(std::begin(Sems), std::end(Sems), [](Expression* pExpr)
    { delete pExpr; });

    if (Res == false)
      break;

    u64 NextInsn = 0;
    if (m_pCpuCtxt->ReadRegister(ProgPtrReg, &NextInsn, ProgPtrRegSize) == false)
      break;
    CurAddr.SetOffset(NextInsn);
  }
}

MEDUSA_NAMESPACE_END