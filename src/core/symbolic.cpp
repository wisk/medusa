#include "medusa/symbolic.hpp"
#include "medusa/module.hpp"

MEDUSA_NAMESPACE_USE;

Symbolic::TaintedBlock::TaintedBlock(void)
{
}

Symbolic::TaintedBlock::~TaintedBlock(void)
{
  for (auto pExpr : m_TaintedExprs)
    delete pExpr;
  m_TaintedExprs.clear();

  for (auto pExpr : m_CondExprs)
    delete pExpr;
  m_CondExprs.clear();
}

Symbolic::TaintedBlock& Symbolic::TaintedBlock::operator=(Symbolic::TaintedBlock const& rBlk)
{
  if (this == &rBlk)
    return *this;

  for (auto pExpr : m_TaintedExprs)
    m_TaintedExprs.push_back(pExpr->Clone());

  for (auto pExpr : m_CondExprs)
    m_CondExprs.push_back(pExpr->Clone());

  m_ParentBlocks = rBlk.m_ParentBlocks;
  return *this;
}

void Symbolic::TaintedBlock::TaintExpression(Address const& rAddr, Taint::Context& rTaintCtxt, Expression const* pExpr)
{
  m_Addresses.insert(rAddr);
  TaintVisitor TaintVst(rAddr, rTaintCtxt);
  m_TaintedExprs.push_back(pExpr->Visit(&TaintVst));
}

void Symbolic::TaintedBlock::AddParentBlock(Address const& rAddress)
{
  m_ParentBlocks.insert(rAddress);
}

std::set<Address> const& Symbolic::TaintedBlock::GetParentBlocks(void) const
{
  return m_ParentBlocks;
}

void Symbolic::TaintedBlock::AddConditionalExpression(Expression const* pExpr)
{
  m_CondExprs.push_back(pExpr->Clone());
}

Expression::List const& Symbolic::TaintedBlock::GetConditionalExpressions(void) const
{
  return m_CondExprs;
}

bool Symbolic::TaintedBlock::Contains(Address const& rAddr) const
{
  for (auto const& rBlkAddr : m_Addresses)
    if (rBlkAddr == rAddr)
      return true;
  return false;
}

void Symbolic::TaintedBlock::ForEachAddress(std::function<bool(Address const& rAddress)> Callback) const
{
  for (auto const& rBlkAddr: m_Addresses)
  {
    if (!Callback(rBlkAddr))
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
  Address CurAddr = RegAddr;
  std::list<Expression const*> Exprs;

  auto itBlk = std::begin(m_TaintedBlocks);

  for (; itBlk != std::end(m_TaintedBlocks); ++itBlk)
  {
    if (itBlk->second.Contains(CurAddr))
      break;
  }

  if (itBlk == std::end(m_TaintedBlocks))
    return Exprs;

  std::set<u32> BtRegs;
  BtRegs.insert(RegId);
  if (!itBlk->second.BacktrackRegister(CurAddr, BtRegs, Exprs))
    return Exprs;

  // TODO: do the same thing for blocks' parent


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

      // TODO: handle jump/call to imported function
      // We have to set retval, undef volatile registers, adjust stack if needed (stdcall)

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

}

bool Symbolic::_DetermineNextAddresses(Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses) const
{
  // TODO: implement it
  return true;
}