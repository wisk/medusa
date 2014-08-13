#include "medusa/symbolic.hpp"
#include "medusa/module.hpp"

MEDUSA_NAMESPACE_USE;

Symbolic::TaintedBlock::~TaintedBlock(void)
{
  for (auto TaintedRegisters : m_TaintedRegisters)
    for (auto TaintedRegister : TaintedRegisters.second)
      delete TaintedRegister.second;
}

void Symbolic::TaintedBlock::TaintRegister(Address const& rAddr, u32 RegId, Expression const* pExpr)
{
  // TODO: Check if the register is not already registered...
  m_TaintedRegisters[rAddr][RegId] = pExpr->Clone();
}

Symbolic::Symbolic(Document& rDoc)
  : m_rDoc(rDoc)
{
}

Symbolic::~Symbolic(void)
{
}

bool Symbolic::TaintRegister(u32 RegId, Address const& rAddr, Symbolic::Callback Cb)
{
  auto ArchTag = m_rDoc.GetArchitectureTag(rAddr);
  auto spArch  = ModuleManager::Instance().GetArchitecture(ArchTag);
  if (spArch == nullptr)
    return false;



  Address LastAddr, CurAddr;
  Context SymCtxt;
  std::unordered_map<Address, bool> VstAddrs;
  std::stack<Address> StkAddrs;

  StkAddrs.push(rAddr);
  while (!StkAddrs.empty())
  {
    CurAddr = StkAddrs.top();
    StkAddrs.pop();

    while (true)
    {
      auto spInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));
      if (spInsn == nullptr)
        break;

      Address::List NextAddrs;

      TaintedBlock Blk;
      _TaintBlock(CurAddr, Blk);

      // Call callback for each address contained in block

      if (!Cb(SymCtxt, CurAddr, NextAddrs))
        break;

      if (NextAddrs.empty())
        break;

      while (NextAddrs.size() != 1)
      {
        StkAddrs.push(NextAddrs.front());
        NextAddrs.pop_front();
      }

      LastAddr = CurAddr;
      CurAddr  = NextAddrs.front();
    }
  };

  return true;
}

bool Symbolic::TaintRegister(CpuInformation::Type RegisterType, Address const& rAddress, Symbolic::Callback Cb)
{
  auto ArchTag  = m_rDoc.GetArchitectureTag(rAddress);
  auto ArchMode = m_rDoc.GetMode(rAddress);
  auto spArch   = ModuleManager::Instance().GetArchitecture(ArchTag);
  if (spArch == nullptr)
    return false;

  auto const pCpuInfo = spArch->GetCpuInformation();
  if (pCpuInfo == nullptr)
    return false;

  u32 RegId = pCpuInfo->GetRegisterByType(RegisterType, ArchMode);
  if (RegId == 0)
    return false;

  return TaintRegister(RegId, rAddress, Cb);
}

bool Symbolic::_TaintBlock(Address const& rBlkAddr, Symbolic::TaintedBlock& rBlk)
{
  Address CurAddr = rBlkAddr;
  bool EndIsReached = false;

  // TODO: code is duplicated
  auto ArchTag  = m_rDoc.GetArchitectureTag(rBlkAddr);
  auto ArchMode = m_rDoc.GetMode(rBlkAddr);
  auto spArch   = ModuleManager::Instance().GetArchitecture(ArchTag);
  if (spArch == nullptr)
    return false;

  auto const pCpuInfo = spArch->GetCpuInformation();
  if (pCpuInfo == nullptr)
    return false;

  u32 PcRegId = pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, ArchMode);
  if (PcRegId == 0)
    return false;

  do
  {
    auto spInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(rBlkAddr));
    if (spInsn == nullptr)
      return false;
    auto pInsnSems = spInsn->GetSemantic();
    if (pInsnSems.empty())
      return false;

    for (auto const pExpr : pInsnSems)
    {
      // First, we're looking for an affectation operation...
      auto pOprtExpr = dynamic_cast<OperationExpression const*>(pExpr);
      if (pOprtExpr == nullptr)
        continue;
      if (pOprtExpr->GetOperation() != OperationExpression::OpAff)
        continue;

      // ...next, the left must be an identifier
      auto pIdExpr = dynamic_cast<IdentifierExpression const*>(pOprtExpr->GetLeftExpression());
      if (pIdExpr == nullptr)
        continue;

      u32 RegId = pIdExpr->GetId();
      rBlk.TaintRegister(CurAddr, RegId, pOprtExpr->GetRightExpression());

      if (RegId == PcRegId)
        EndIsReached = true;

      delete pExpr;
    }

  } while (!EndIsReached);
}