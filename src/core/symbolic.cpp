#include "medusa/symbolic.hpp"
#include "medusa/module.hpp"

MEDUSA_NAMESPACE_USE;

Symbolic::Block::Block(u32 PcRegId) : m_PcRegId(PcRegId)
{
}

Symbolic::Block::Block(Block const& rBlk)
{
  *this = rBlk;
}

Symbolic::Block& Symbolic::Block::operator=(Symbolic::Block const& rBlk)
{
  if (this == &rBlk)
    return *this;

  m_PcRegId = rBlk.m_PcRegId;

  m_Addresses = rBlk.m_Addresses;
  m_ParentBlocks = rBlk.m_ParentBlocks;

  for (auto pExpr : rBlk.m_TaintedExprs)
    m_TaintedExprs.push_back(pExpr->Clone());

  for (auto pExpr : rBlk.m_CondExprs)
    m_CondExprs.push_back(pExpr->Clone());

  return *this;
}

Symbolic::Block::~Block(void)
{
  for (auto pExpr : m_TaintedExprs)
    delete pExpr;
  m_TaintedExprs.clear();

  for (auto pExpr : m_CondExprs)
    delete pExpr;
  m_CondExprs.clear();
}

void Symbolic::Block::TaintExpression(Address const& rAddr, Taint::Context& rTaintCtxt, Expression const* pExpr)
{
  m_Addresses.insert(rAddr);
  TaintVisitor TaintVst(rAddr, rTaintCtxt);
  m_TaintedExprs.push_back(pExpr->Visit(&TaintVst));
}

void Symbolic::Block::BackTrackId(Address const& rAddr, u32 Id, std::list<Expression const*>& rExprs) const
{
  auto itExpr = m_TaintedExprs.rbegin();

  Taint::BackTrackContext BtCtxt;

  for (; itExpr != m_TaintedExprs.rend(); ++itExpr)
  {
    BackTrackVisitor BtVst(BtCtxt, rAddr, Id);
    (*itExpr)->Visit(&BtVst);
    if (BtVst.GetResult())
      rExprs.push_back(*itExpr);
  }
}

void Symbolic::Block::AddParentBlock(Address const& rAddress)
{
  m_ParentBlocks.insert(rAddress);
}

std::set<Address> const& Symbolic::Block::GetParentBlocks(void) const
{
  return m_ParentBlocks;
}

void Symbolic::Block::AddConditionalExpression(Expression const* pExpr)
{
  m_CondExprs.push_back(pExpr->Clone());
}

Expression::List const& Symbolic::Block::GetConditionalExpressions(void) const
{
  return m_CondExprs;
}

bool Symbolic::Block::Contains(Address const& rAddr) const
{
  for (auto const& rBlkAddr : m_Addresses)
    if (rBlkAddr == rAddr)
      return true;
  return false;
}

bool Symbolic::Block::IsEndOfBlock(void) const
{
  if (m_PcRegId == 0)
    return false;

  if (m_TaintedExprs.empty())
    return false;

  ModifyIdVisitor ModPcVst(m_PcRegId);
  // LATER: Handle branch delay slot!
  m_TaintedExprs.back()->Visit(&ModPcVst);
  return ModPcVst.GetResult();
}

void Symbolic::Block::ForEachAddress(std::function<bool(Address const& rAddress)> Callback) const
{
  for (auto const& rBlkAddr: m_Addresses)
  {
    if (!Callback(rBlkAddr))
      return;
  }
}

bool Symbolic::Context::AddBlock(Address const& rBlkAddr, Symbolic::Block& rBlk)
{
  // TODO: check dup?
  m_Blocks[rBlkAddr] = rBlk;
  return true;
}

std::list<Expression const*> Symbolic::Context::BacktrackRegister(Address const& RegAddr, u32 RegId) const
{
  Address CurAddr = RegAddr;
  std::list<Expression const*> Exprs;

  auto itBlk = std::begin(m_Blocks);

  for (; itBlk != std::end(m_Blocks); ++itBlk)
  {
    if (itBlk->second.Contains(CurAddr))
      break;
  }

  if (itBlk == std::end(m_Blocks))
    return Exprs;

  itBlk->second.BackTrackId(CurAddr, RegId, Exprs);

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

bool Symbolic::Execute(Address const& rAddr, Symbolic::Callback Cb)
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

      Block Blk(m_PcRegId);
      if (!_ExecuteBlock(SymCtxt, CurAddr, Blk))
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

bool Symbolic::_ExecuteBlock(Symbolic::Context& rCtxt, Address const& rBlkAddr, Symbolic::Block& rBlk)
{
  Address CurAddr = rBlkAddr;

  do
  {
    auto spInsn = std::dynamic_pointer_cast<Instruction const>(m_rDoc.GetCell(CurAddr));
    if (spInsn == nullptr) // We return false if we reached an invalid instruction
      return false;

    auto pInsnSem = spInsn->GetSemantic();
    for (auto pExpr : pInsnSem)
    {
      rBlk.TaintExpression(CurAddr, rCtxt.GetTaintContext(), pExpr);
    }

    CurAddr += spInsn->GetLength();
  } while (!rBlk.IsEndOfBlock());

  return true;
}

bool Symbolic::_DetermineNextAddresses(Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses) const
{
  // TODO: implement it
  return true;
}