#include "medusa/execution.hpp"
#include "medusa/module.hpp"

#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

Execution::Execution(Medusa* pCore, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs)
: m_pCore(pCore)
, m_spArch(spArch), m_spOs(spOs)
, m_pCpuCtxt(nullptr), m_pMemCtxt(nullptr)
, m_pCpuInfo(spArch->GetCpuInformation())
{
}

Execution::~Execution(void)
{
}

bool Execution::Initialize(u8 Mode, u64 StackLinearAddress, u32 StackSize)
{
  delete m_pCpuCtxt;
  delete m_pMemCtxt;

  m_pCpuCtxt = m_spArch->MakeCpuContext();
  m_pMemCtxt = m_spArch->MakeMemoryContext();
  if (m_spOs != nullptr)
  {
    m_spOs->InitializeCpuContext(m_pCore->GetDocument(), *m_pCpuCtxt);
    m_spOs->InitializeMemoryContext(m_pCore->GetDocument(), *m_pMemCtxt);
  }

  m_pCpuCtxt->SetMode(Mode);

  if (m_pMemCtxt->MapDocument(m_pCore->GetDocument(), m_pCpuCtxt) == false)
    return false;

  if (m_pMemCtxt->AllocateMemory(StackLinearAddress, StackSize, nullptr) == false)
    return false;

  u64 StackRegisterValue = StackLinearAddress + StackSize;
  u32 StkReg = m_pCpuInfo->GetRegisterByType(CpuInformation::StackPointerRegister, Mode);
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
  auto pGetEmulator = ModuleManager::Instance().GetEmulator(rEmulatorName);
  if (pGetEmulator == nullptr)
    return false;
  auto pEmul = pGetEmulator(m_spArch->GetCpuInformation(), m_pCpuCtxt, m_pMemCtxt);
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

  u32 ProgPtrReg = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, m_pCpuCtxt->GetMode());
  if (ProgPtrReg == CpuInformation::InvalidRegister)
    return;
  u32 ProgPtrRegSize = m_pCpuInfo->GetSizeOfRegisterInBit(ProgPtrReg);
  if (ProgPtrRegSize < 8)
    return;
  ProgPtrRegSize /= 8;

  u64 CurInsn = rAddr.GetOffset();
  if (m_pCpuCtxt->WriteRegister(ProgPtrReg, &CurInsn, ProgPtrRegSize) == false)
    return;

  Address BlkAddr = CurAddr;
  while (true)
  {
    //std::cout << m_pCpuCtxt->ToString() << std::endl;

    Expression::List Sems;
    while (true)
    {
      auto spCurInsn = std::dynamic_pointer_cast<Instruction>(m_pCore->GetCell(CurAddr));
      if (spCurInsn == nullptr)
      {
        Log::Write("exec") << "execution finished\n" << m_pCpuCtxt->ToString() << "\n" << m_pMemCtxt->ToString() << LogEnd;
        return;
      }

      std::string StrCell;
      Cell::Mark::List Marks;
      if (m_pCore->FormatCell(CurAddr, *spCurInsn, StrCell, Marks) == false)
        break;

      Log::Write("exec") << StrCell << LogEnd;

      Sems.push_back(new OperationExpression(OperationExpression::OpAff,
        new IdentifierExpression(ProgPtrReg, m_pCpuInfo),
        new OperationExpression(OperationExpression::OpAdd,
        /**/new IdentifierExpression(ProgPtrReg, m_pCpuInfo),
        /**/new ConstantExpression(ProgPtrRegSize * 8, spCurInsn->GetLength())
        )));
      CurAddr.SetOffset(CurAddr.GetOffset() + spCurInsn->GetLength());

      auto const& rCurSem = spCurInsn->GetSemantic();
      if (rCurSem.empty())
      {
        Log::Write("exec") << "no semantic available" << LogEnd;
        break;
      }
      std::for_each(std::begin(rCurSem), std::end(rCurSem), [&](Expression const* pExpr)
      { Sems.push_back(pExpr->Clone()); });

      if (spCurInsn->GetSubType() != Instruction::NoneType)
        break;
    };

    bool Res = m_spEmul->Execute(BlkAddr, Sems);
    std::for_each(std::begin(Sems), std::end(Sems), [](Expression* pExpr)
    { delete pExpr; });

    if (Res == false)
    {
      std::cout << "Execution failed:\n" << m_pCpuCtxt->ToString() << std::endl << m_pMemCtxt->ToString() << std::endl;
      break;
    }

    u64 NextInsn = 0;
    if (m_pCpuCtxt->ReadRegister(ProgPtrReg, &NextInsn, ProgPtrRegSize) == false)
      break;
    CurAddr.SetOffset(NextInsn);
  }
}

MEDUSA_NAMESPACE_END