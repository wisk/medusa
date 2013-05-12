#include "windows.hpp"
#include "stack_analyzer.hpp"

WindowsOperatingSystem::~WindowsOperatingSystem(void)
{
}

std::string WindowsOperatingSystem::GetName(void) const
{
  return "MS Windows";
}

bool WindowsOperatingSystem::InitializeCpuContext(CpuContext& rCpuCtxt) const
{
  CpuInformation const& rCpuInfo = rCpuCtxt.GetCpuInformation();
  auto IdFs = rCpuInfo.ConvertNameToIdentifier("fs");
  if (IdFs == 0)
    return false;
  u16 Fs = 0x2b;
  if (rCpuCtxt.WriteRegister(IdFs, &Fs, sizeof(Fs)) == false)
    return false;
  if (rCpuCtxt.AddMapping(Address(Fs, 0x0), 0x7fdf0000) == false)
    return false;
  return true;
}

bool WindowsOperatingSystem::InitializeMemoryContext(MemoryContext& rMemCtxt) const
{
  // TODO: create a fake _TEB/_PEB
  if (rMemCtxt.AllocateMemory(0x7fdf0000, 0x1000, nullptr) == false)
    return false;
  return true;
}

bool WindowsOperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  // At this time, only Windows on x86 is supported
  if ((rLdr.GetName() == "PE" || rLdr.GetName() == "PE+") && rArch.GetName() == "Intel x86")
    return true;

  return false;
}

void WindowsOperatingSystem::AnalyzeFunction(Address const& rFuncAddr, Analyzer& rAnlz) const
{
  auto pFunc = dynamic_cast<Function*>(rAnlz.GetMultiCell(m_rDb, m_rDb.GetFileBinaryStream(), rFuncAddr));
  if (pFunc == nullptr)
    return;
  auto spArch = rAnlz.GetArchitecture(rAnlz.GetCell(m_rDb, m_rDb.GetFileBinaryStream(), rFuncAddr)->GetArchitectureTag());
  if (spArch == nullptr)
    return;

  enum RegisterState { Ignored, NotSaved, Saved };

  struct FindSavedNonVolatileRegister : public ExpressionVisitor
  {
    FindSavedNonVolatileRegister(std::map<u32, RegisterState> const& rNonVolatileRegisters, u32 StackPointerRegister)
      : m_rNonVolatileRegisters(rNonVolatileRegisters)
      , m_SavedNonVolatileRegister(), m_RestoredNonVolatileRegister()
      , m_StackPointerRegister(StackPointerRegister)
    {
    }

    virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)
    {
      if (Type != OperationExpression::OpAff)
        return nullptr;

      auto pSavedDst    = dynamic_cast<MemoryExpression const*>(pLeftExpr);
      auto pSavedIdExpr = dynamic_cast<IdentifierExpression const*>(pRightExpr);
      if (pSavedDst && pSavedIdExpr)
      {
        ExpressionVisitor_ContainIdentifier ci(m_StackPointerRegister);
        pSavedDst->Visit(&ci);
        if (ci.GetResult())
        {
          u32 Id = pSavedIdExpr->GetId();
          auto itRegState = m_rNonVolatileRegisters.find(Id);
          if (itRegState != std::end(m_rNonVolatileRegisters) && itRegState->second == NotSaved)
            m_SavedNonVolatileRegister = Id;
        }
      }

      auto pRestoredSrc    = dynamic_cast<MemoryExpression const*>(pRightExpr);
      auto pRestoredIdExpr = dynamic_cast<IdentifierExpression const*>(pLeftExpr);
      if (pRestoredSrc && pRestoredIdExpr)
      {
        ExpressionVisitor_ContainIdentifier ci(m_StackPointerRegister);
        pRestoredSrc->Visit(&ci);
        if (ci.GetResult())
        {
          u32 Id = pRestoredIdExpr->GetId();
          auto itRegState = m_rNonVolatileRegisters.find(Id);
          if (itRegState != std::end(m_rNonVolatileRegisters) && itRegState->second == Saved)
            m_RestoredNonVolatileRegister = Id;
        }
      }
      return nullptr;
    }
    std::map<u32, RegisterState> const& m_rNonVolatileRegisters;
    u32                                 m_SavedNonVolatileRegister;
    u32                                 m_RestoredNonVolatileRegister;
    u32 const                           m_StackPointerRegister;
  };

  struct X86_FindNonVolatileRegister : public Analyzer::Tracker
  {
    X86_FindNonVolatileRegister(CpuInformation const* pCpuInfo)
      : m_pCpuInfo(pCpuInfo)
      , m_NonVolatileRegisters()
    {
      static char const *s_NonVolatileRegisters[]
      = { "ebp", "rbp", "ebx", "rbx", "edi", "rdi", "esi", "rsi", "r12d", "r12", "r13d", "r13", "r14d", "r14", "r15d", "r15", nullptr };
      for (auto pRegName = s_NonVolatileRegisters; *pRegName; ++pRegName)
      {
        u32 Id = pCpuInfo->ConvertNameToIdentifier(*pRegName);
        m_NonVolatileRegisters[Id] = NotSaved;
      }
    }
    CpuInformation const* m_pCpuInfo;
    std::map<u32, RegisterState> m_NonVolatileRegisters; /* non-volatile registers: RBP,RBX,RDI,RSI,R12,R13,R14,R15 */

    virtual bool Track(Analyzer& rAnlz, Database& rDb, Address const& rAddr)
    {
      auto pInsn = dynamic_cast<Instruction*>(rAnlz.GetCell(rDb, rDb.GetFileBinaryStream(), rAddr));
      if (pInsn == nullptr)
        return false;
      if (pInsn->GetOperationType() == Instruction::OpRet)
        return false;

      u32 StkPtrReg = m_pCpuInfo->GetRegisterByType(CpuInformation::StackPointerRegister);
      auto SemList = pInsn->GetSemantic();
      auto pVisitor = new FindSavedNonVolatileRegister(m_NonVolatileRegisters, StkPtrReg);
      for (auto itSem = std::begin(SemList); itSem != std::end(SemList); ++itSem)
      {
        pVisitor->m_SavedNonVolatileRegister = 0;
        pVisitor->m_RestoredNonVolatileRegister = 0;
        (*itSem)->Visit(pVisitor);
        if (pVisitor->m_SavedNonVolatileRegister)
        {
          pInsn->SetComment((boost::format("saved non-volatile register %s")
            % m_pCpuInfo->ConvertIdentifierToName(pVisitor->m_SavedNonVolatileRegister)).str());
          m_NonVolatileRegisters[pVisitor->m_SavedNonVolatileRegister] = Saved;
        }
        if (pVisitor->m_RestoredNonVolatileRegister)
        {
          pInsn->SetComment((boost::format("restored non-volatile register %s")
            % m_pCpuInfo->ConvertIdentifierToName(pVisitor->m_RestoredNonVolatileRegister)).str());
          m_NonVolatileRegisters[pVisitor->m_SavedNonVolatileRegister] = Ignored;
        }
      }
      delete pVisitor;

      for (auto itReg = std::begin(m_NonVolatileRegisters); itReg != std::end(m_NonVolatileRegisters); ++itReg)
        if (itReg->second == NotSaved)
          return true;

      return false;
    }
  };

  rAnlz.TrackOperand(m_rDb, rFuncAddr, X86_FindNonVolatileRegister(spArch->GetCpuInformation()));
  rAnlz.TrackOperand(m_rDb, rFuncAddr, X86StackAnalyzerTracker(spArch->GetCpuInformation()));
}