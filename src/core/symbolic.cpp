#include "medusa/symbolic.hpp"
#include "medusa/module.hpp"

MEDUSA_NAMESPACE_USE;

Symbolic::TaintedBlock::TaintedBlock(void)
{
}

Symbolic::TaintedBlock::~TaintedBlock(void)
{
  for (auto TaintedRegisters : m_TaintedRegisters)
    for (auto TaintedRegister : TaintedRegisters.second)
      delete TaintedRegister.second;
}

Symbolic::TaintedBlock& Symbolic::TaintedBlock::operator=(Symbolic::TaintedBlock const& rBlk)
{
  if (this == &rBlk)
    return *this;

  for (auto const& TaintedRegisters : m_TaintedRegisters)
    for (auto TaintedRegister : TaintedRegisters.second)
      delete TaintedRegister.second;

  for (auto const& TaintedRegisters : rBlk.m_TaintedRegisters)
    for (auto TaintedRegister : TaintedRegisters.second)
      TaintRegister(TaintedRegisters.first, TaintedRegister.first, TaintedRegister.second);

  m_ParentBlocks = rBlk.m_ParentBlocks;
  return *this;
}

void Symbolic::TaintedBlock::TaintRegister(Address const& rAddr, u32 RegId, Expression const* pExpr)
{
  // TODO: Check if the register is not already registered...
  m_TaintedRegisters[rAddr][RegId] = pExpr->Clone();
}

bool Symbolic::TaintedBlock::BacktrackRegister(Address const& rRegAddr, u32 RegId, std::list<Expression const*>& rExprs) const
{
  std::set<u32> BacktrackedRegister;

  BacktrackedRegister.insert(RegId);


  auto itPair = m_TaintedRegisters.rbegin();

  for (;itPair != m_TaintedRegisters.rend(); ++itPair)
  {
    if (itPair->first == rRegAddr)
      break;
  }

  if (itPair == m_TaintedRegisters.rend())
    return false;

  // For each tainted addresses...
  for (; itPair != m_TaintedRegisters.rend(); ++itPair)
  {
    // for each tainted register...
    for (auto const& rTaintedRegister : itPair->second)
    {
      // if one of register matchs with our current backtracted registers...
      if (BacktrackedRegister.find(rTaintedRegister.first) == std::end(BacktrackedRegister))
        continue;

      // we remove it from the list
      BacktrackedRegister.erase(rTaintedRegister.first);

      // add the current expression (no need to clone)
      rExprs.push_back(rTaintedRegister.second);

      // backtrack sources (TODO: use a visiter/matcher to find every possibility...)
      auto pOprtExpr = dynamic_cast<OperationExpression const*>(rTaintedRegister.second);
      if (pOprtExpr != nullptr)
      {
        auto pLeftIdExpr = dynamic_cast<IdentifierExpression const*>(pOprtExpr->GetLeftExpression());
        if (pLeftIdExpr != nullptr)
          BacktrackedRegister.insert(pLeftIdExpr->GetId());

        auto pRightIdExpr = dynamic_cast<IdentifierExpression const*>(pOprtExpr->GetRightExpression());
        if (pRightIdExpr != nullptr)
          BacktrackedRegister.insert(pRightIdExpr->GetId());
      }
    }
  }

  return true;
}

void Symbolic::TaintedBlock::AddParentBlock(Address const& rAddress)
{
  m_ParentBlocks.insert(rAddress);
}

std::set<Address> const& Symbolic::TaintedBlock::GetParentBlocks(void) const
{
  return m_ParentBlocks;
}

bool Symbolic::TaintedBlock::Contains(Address const& rAddr) const
{
  for (auto const& rPair : m_TaintedRegisters)
    if (rPair.first == rAddr)
      return true;
  return false;
}

void Symbolic::TaintedBlock::ForEachAddress(std::function<bool(Address const& rAddress)> Callback) const
{
  for (auto const& rPair : m_TaintedRegisters)
  {
    if (!Callback(rPair.first))
      return;
  }
}

Symbolic::TaintedBlock::TaintedBlock(TaintedBlock const& rBlk)
{
  *this = rBlk;
}

bool Symbolic::Context::AddBlock(Address const& rBlkAddr, Symbolic::TaintedBlock& rBlk)
{
  // TODO: check dup?
  m_TaintedBlocks[rBlkAddr] = rBlk;
  return true;
}

std::list<Expression const*> Symbolic::Context::BacktrackRegister(Address const& RegAddr, u32 RegId) const
{
  std::list<Expression const*> Exprs;

  for (auto const& rBlk : m_TaintedBlocks)
  {
    if (!rBlk.second.Contains(RegAddr))
      continue;
    if (!rBlk.second.BacktrackRegister(RegAddr, RegId, Exprs))
      return Exprs;
    // TODO: do the same thing for blocks' parent
  }

  return Exprs;
}

Symbolic::Symbolic(Document& rDoc)
  : m_rDoc(rDoc), m_PcRegId()
{
}

Symbolic::~Symbolic(void)
{
}

bool Symbolic::TaintRegister(u32 RegId, Address const& rAddr, Symbolic::Callback Cb)
{
  auto ArchTag = m_rDoc.GetArchitectureTag(rAddr);
  auto ArchMode = m_rDoc.GetMode(rAddr);
  auto spArch = ModuleManager::Instance().GetArchitecture(ArchTag);
  if (spArch == nullptr)
    return false;

  auto const pCpuInfo = spArch->GetCpuInformation();
  if (pCpuInfo == nullptr)
    return false;

  m_PcRegId = pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, ArchMode);
  if (m_PcRegId == 0)
    return false;

  Address LastAddr, CurAddr;
  Context SymCtxt;
  std::unordered_map<Address, bool> VstAddrs;
  std::stack<Address> StkAddrs;
  bool FirstBlock = true;

  StkAddrs.push(rAddr);
  while (!StkAddrs.empty())
  {
    CurAddr = StkAddrs.top();
    StkAddrs.pop();

    while (true)
    {
      Address::List NextAddrs;

      TaintedBlock Blk;
      if (!_TaintBlock(CurAddr, Blk))
        return false;

      if (!FirstBlock)
        Blk.AddParentBlock(LastAddr);
      else
        FirstBlock = false;

      SymCtxt.AddBlock(CurAddr, Blk);

      bool TaintIsDone = false;

      // Call callback for each address contained in block
      Blk.ForEachAddress([&](Address const& rBlkAddr)
      {
        if (!Cb(SymCtxt, rBlkAddr, NextAddrs))
        {
          TaintIsDone = true;
          return false;
        }

        return true;
      });

      if (TaintIsDone)
        return true;

      // TODO: Warn the user
      if (!_DetermineNextAddresses(SymCtxt, CurAddr, NextAddrs))
      { }

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

bool Symbolic::_TaintBlock(Address const& rBlkAddr, Symbolic::TaintedBlock& rBlk)
{
  Address CurAddr = rBlkAddr;
  bool EndIsReached = false;

  do
  {
    auto spInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));
    if (spInsn == nullptr)
      return false;
    auto pInsnSems = spInsn->GetSemantic();
    if (pInsnSems.empty())
      return false;

    for (auto const pExpr : pInsnSems)
    {
      // DEBUG
      std::string SemStr = pExpr->ToString();

      // First, we're looking for an affectation operation...
      auto pOprtExpr = dynamic_cast<OperationExpression const*>(pExpr);

      // ... we also check for conditional operation
      if (pOprtExpr == nullptr)
      {
        auto pIfExpr = dynamic_cast<IfConditionExpression const*>(pExpr);
        if (pIfExpr == nullptr)
          continue;
        pOprtExpr = dynamic_cast<OperationExpression const*>(pIfExpr->GetThenExpression());
        if (pOprtExpr == nullptr)
          continue;
      }


      if (pOprtExpr->GetOperation() != OperationExpression::OpAff)
        continue;

      // ...next, the left must be an identifier
      auto pIdExpr = dynamic_cast<IdentifierExpression const*>(pOprtExpr->GetLeftExpression());
      if (pIdExpr == nullptr)
        continue;

      u32 RegId = pIdExpr->GetId();
      rBlk.TaintRegister(CurAddr, RegId, pOprtExpr->GetRightExpression());

      if (RegId == m_PcRegId)
        EndIsReached = true;

    }

    CurAddr += spInsn->GetLength();

  } while (!EndIsReached);

  return true;
}

bool Symbolic::_DetermineNextAddresses(Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses) const
{
  // TODO: implement it
  return true;
}