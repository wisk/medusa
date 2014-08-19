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

  for (auto pExpr : rBlk.m_TrackedExprs)
    m_TrackedExprs.push_back(pExpr->Clone());

  for (auto pExpr : rBlk.m_CondExprs)
    m_CondExprs.push_back(pExpr->Clone());

  return *this;
}

Symbolic::Block::~Block(void)
{
  for (auto pExpr : m_TrackedExprs)
    delete pExpr;
  m_TrackedExprs.clear();

  for (auto pExpr : m_CondExprs)
    delete pExpr;
  m_CondExprs.clear();
}

void Symbolic::Block::TrackExpression(Address const& rAddr, Track::Context& rTrackCtxt, Expression const* pExpr)
{
  m_Addresses.insert(rAddr);
  TrackVisitor TrackVst(rAddr, rTrackCtxt);
  m_TrackedExprs.push_back(pExpr->Visit(&TrackVst));
}

void Symbolic::Block::BackTrackId(Address const& rAddr, u32 Id, std::list<Expression const*>& rExprs) const
{
  auto itExpr = m_TrackedExprs.rbegin();

  Track::BackTrackContext BtCtxt;

  for (; itExpr != m_TrackedExprs.rend(); ++itExpr)
  {
    BackTrackVisitor BtVst(BtCtxt, rAddr, Id);
    (*itExpr)->Visit(&BtVst);
    if (BtVst.GetResult())
      rExprs.push_back(*itExpr);
  }
}

void Symbolic::Block::AddExpression(Expression* pExpr)
{
  m_TrackedExprs.push_back(pExpr);
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

Address Symbolic::Block::GetLastAddress(void) const
{
  if (m_Addresses.empty())
    return Address();
  return *m_Addresses.rbegin();
}

bool Symbolic::Block::IsEndOfBlock(void) const
{
  if (m_PcRegId == 0)
    return false;

  if (m_TrackedExprs.empty())
    return false;

  using namespace ExprMatcher;
  ExpressionMatcher ModPcMatcher(OpLeftIs(IdIs(m_PcRegId)));

  // LATER: Handle branch delay slot!
  return ModPcMatcher.Test(m_TrackedExprs.back());
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

Symbolic::Symbolic(Document& rDoc) : m_rDoc(rDoc), m_pCpuInfo(nullptr), m_PcRegId(), m_SpRegId()
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

  m_pCpuInfo = spArch->GetCpuInformation();
  if (m_pCpuInfo == nullptr)
    return false;

  m_PcRegId = m_pCpuInfo->GetRegisterByType(CpuInformation::ProgramPointerRegister, ArchMode);
  if (m_PcRegId == 0)
    return false;

  m_SpRegId = m_pCpuInfo->GetRegisterByType(CpuInformation::StackPointerRegister, ArchMode);
  if (m_SpRegId == 0)
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

      auto PcExprs = SymCtxt.BacktrackRegister(Blk.GetLastAddress(), m_PcRegId);
      if (PcExprs.empty())
        break;

      if (!_DetermineNextAddresses(PcExprs, NextAddrs))
        Log::Write("core") << "unable to determine the next addresses: " << CurAddr << LogEnd;

      if (NextAddrs.size() == 1 && m_rDoc.GetLabelFromAddress(NextAddrs.front()).GetType() & Label::Imported)
      {
        if (!_HandleImportedFunctionExecution(NextAddrs.front(), SymCtxt.GetTrackContext(), Blk))
          Log::Write("core") << "unable to handle imported function: " << NextAddrs.front() << LogEnd;
      }

      bool TrackIsDone = false;

      // Call callback for each address contained in block
      Blk.ForEachAddress([&](Address const& rBlkAddr)
      {
        if (!Cb(SymCtxt, rBlkAddr, NextAddrs))
        {
          TrackIsDone = true;
          return false;
        }

        return true;
      });

      if (TrackIsDone)
        return true;

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
      rBlk.TrackExpression(CurAddr, rCtxt.GetTrackContext(), pExpr);
    }

    CurAddr += spInsn->GetLength();
  } while (!rBlk.IsEndOfBlock());

  return true;
}

bool Symbolic::_DetermineNextAddresses(std::list<Expression const*> const& rPcExprs, Address::List& rNextAddresses) const
{
  for (auto pExpr : rPcExprs)
  {
    //DEBUG
    std::string s = pExpr->ToString();
    Log::Write("dbg") << s << LogEnd;
  }

  return true;
}

bool Symbolic::_HandleImportedFunctionExecution(Address const& rImpFunc, Track::Context& rTrkCtxt, Symbolic::Block& rBlk)
{
  Id FuncId;
  if (!m_rDoc.RetrieveDetailId(rImpFunc, 0, FuncId))
    return false;
  FunctionDetail FuncDtl;
  if (!m_rDoc.GetFunctionDetail(FuncId, FuncDtl))
    return false;

  u32 StkBitSz = m_pCpuInfo->GetSizeOfRegisterInBit(m_SpRegId);

  // HACK: assume stdcall calling convention which could be FALSE!
  rBlk.AddExpression(Expr::MakeSym(SymbolicExpression::ReturnedValue, "eax"));
  rBlk.AddExpression(Expr::MakeSym(SymbolicExpression::Undefined, "ecx")); // volatile register
  rBlk.AddExpression(Expr::MakeSym(SymbolicExpression::Undefined, "edx")); // volatile register

  // Emulate the add esp, parm_no * stk_size
  auto const& rParms = FuncDtl.GetParameters();
  rBlk.TrackExpression(rImpFunc, rTrkCtxt, Expr::MakeOp(
    OperationExpression::OpAff,
    Expr::MakeId(m_SpRegId, m_pCpuInfo),
    Expr::MakeOp(OperationExpression::OpAdd,
    /**/Expr::MakeId(m_SpRegId, m_pCpuInfo),
    /**/Expr::MakeConst(StkBitSz, StkBitSz / 8 * rParms.size()))));

  return true;
}