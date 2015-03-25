#include "medusa/symbolic.hpp"
#include "medusa/module.hpp"
#include "medusa/expression_simplifier.hpp"
#include "medusa/expression_visitor.hpp"

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
}

void Symbolic::Block::TrackExpression(Address const& rAddr, Track::Context& rTrackCtxt, Expression::SPType spExpr)
{
  m_Addresses.insert(rAddr);
  TrackVisitor TrackVst(rAddr, rTrackCtxt);
  AddExpression(spExpr->Visit(&TrackVst));
}

Expression::SPType Symbolic::Block::BackTrackExpression(ExpressionMatcher Matcher) const
{
  for (auto itExpr = m_TrackedExprs.rend(); itExpr != m_TrackedExprs.rbegin(); ++itExpr)
  {
    if (Matcher(*itExpr))
      return *itExpr;
  }
  return nullptr;
}

void Symbolic::Block::BackTrackId(Address const& rAddr, u32 Id, Expression::LSPType& rExprs) const
{
  auto itExpr = m_TrackedExprs.rbegin();

  Track::BackTrackContext BtCtxt;
  BtCtxt.TrackId(std::make_tuple(Id, rAddr));

  for (; itExpr != m_TrackedExprs.rend(); ++itExpr)
  {
    BackTrackVisitor BtVst(BtCtxt);
    (*itExpr)->Visit(&BtVst);
    if (BtVst.GetResult())
      rExprs.push_back(*itExpr);
  }
}

void Symbolic::Block::AddExpression(Expression::SPType spExpr)
{
  m_TrackedExprs.push_back(spExpr);
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

Expression::LSPType const& Symbolic::Block::GetConditionalExpressions(void) const
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

  FilterVisitor FltVst([&](Expression::SPType spExpr) -> Expression::SPType
  {
    auto pAssignExpr = expr_cast<AssignmentExpression>(spExpr);
    if (pAssignExpr == nullptr)
      return nullptr;

    auto spIdExpr = expr_cast<IdentifierExpression>(pAssignExpr->GetDestinationExpression());
    if (spIdExpr != nullptr && spIdExpr->GetId() == m_PcRegId)
      return spExpr;

    auto spTrkIdExpr = expr_cast<TrackedIdentifierExpression>(pAssignExpr->GetDestinationExpression());
    if (spTrkIdExpr != nullptr && spTrkIdExpr->GetId() == m_PcRegId)
      return spExpr;

    return nullptr;
  }, 1);

  // LATER: Handle branch delay slot!
  m_TrackedExprs.back()->Visit(&FltVst);
  return FltVst.GetMatchedExpressions().empty() ? false : true;
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

Expression::LSPType Symbolic::Context::BacktrackRegister(Address const& RegAddr, u32 RegId) const
{
  Address CurAddr = RegAddr;
  Expression::LSPType Exprs;

  auto itBlk = std::begin(m_Blocks);

  for (; itBlk != std::end(m_Blocks); ++itBlk)
  {
    if (itBlk->second.Contains(CurAddr))
      break;
  }

  if (itBlk == std::end(m_Blocks))
    return Exprs;

  itBlk->second.BackTrackId(CurAddr, RegId, Exprs);

  TrackedIdPropagation TrkIdProp(Exprs, RegId);
  TrkIdProp.Execute();

  for (auto spExpr : Exprs)
  {
    std::string s0 = spExpr->ToString();
    Log::Write("core").Level(LogDebug) << "expr: " << s0 << LogEnd;
    NormalizeExpression NormExpr(spExpr);
    NormExpr.Execute();
    std::string s1 = spExpr->ToString();
    Log::Write("dbg").Level(LogDebug) << "after ne: " << s1 << LogEnd;
    ConstantPropagation ConstProp(spExpr);
    ConstProp.Execute();
    std::string s2 = spExpr->ToString();
    Log::Write("dbg").Level(LogDebug) << "after cp: " << s2 << LogEnd;
  }

  // TODO: do the same thing for blocks' parent

  return Exprs;
}

Expression::VSPType Symbolic::Context::GetExpressions(void) const
{
  return Expression::VSPType();
}

std::string Symbolic::Context::ToString(void) const
{
  std::string Res = "";
  for (auto const& rBlockPair : m_Blocks)
  {
    Res += rBlockPair.first.ToString();
    Res += ":\n";
    for (auto const& rExpr : rBlockPair.second.GetTrackedExpressions())
    {
      Res += "  ";
      Res += rExpr->ToString();
      Res += "\n";
    }
    Res += "\n";
  }

  return Res;
}

Symbolic::Symbolic(Document& rDoc) : m_rDoc(rDoc), m_pCpuInfo(nullptr), m_PcRegId(), m_SpRegId(), m_FollowFunction(true)
{
}

Symbolic::~Symbolic(void)
{
}

void Symbolic::FollowFunction(bool FollowFunction)
{
  m_FollowFunction = FollowFunction;
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

      Address LastAddrInBlk = Blk.GetLastAddress();
      auto spLastInsnInBlk = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(LastAddrInBlk));
      if (spLastInsnInBlk == nullptr)
        return false;

      if (!_DetermineNextAddresses(SymCtxt, *spLastInsnInBlk.get(), LastAddrInBlk, NextAddrs))
        Log::Write("core") << "unable to determine the next addresses: " << CurAddr << LogEnd;

      // If the next address is an imported symbol or the last instruction in block is a call and the caller
      // doesn't want follow sub-function, we try to emulate the call symbolically (using parameter number,
      // calling convention...).
      if (NextAddrs.size() == 1 &&
        ((m_rDoc.GetLabelFromAddress(NextAddrs.front()).GetType() & Label::AccessMask) == Label::Imported
        || (spLastInsnInBlk->GetSubType() == Instruction::CallType && m_FollowFunction)))
      {
        if (!_ApplyCallingEffect(NextAddrs.front(), SymCtxt.GetTrackContext(), Blk))
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
  bool ModifyPc = false;

  do
  {
    auto spInsn = std::dynamic_pointer_cast<Instruction const>(m_rDoc.GetCell(CurAddr));
    if (spInsn == nullptr) // We return false if we reached an invalid instruction
      return false;

    // We need to inform the tracker that the pc register has moved to correctly backtrack pc later.
    //rBlk.TrackExpression(CurAddr, rCtxt.GetTrackContext(),
    //  Expr::MakeAssign(
    //  /**/Expr::MakeId(m_PcRegId, m_pCpuInfo),
    //  /**/Expr::MakeConst(CurAddr.GetOffsetSize(), CurAddr.GetOffset())));

    auto pInsnSem = spInsn->GetSemantic();
    for (auto spExpr : pInsnSem)
    {
      // TODO: replace PC type register in source of assignment operator to the current address

      rBlk.TrackExpression(CurAddr, rCtxt.GetTrackContext(), spExpr);

      auto spAssignExpr = expr_cast<AssignmentExpression>(spExpr);
      if (spAssignExpr == nullptr)
        continue;

      auto spIdExpr = expr_cast<IdentifierExpression>(spAssignExpr->GetDestinationExpression());
      if (spIdExpr == nullptr)
        continue;

      if (spIdExpr->GetId() != m_PcRegId)
        continue;

      ModifyPc = true;
    }

    CurAddr += spInsn->GetLength();
  } while (!ModifyPc);

  return true;
}

bool Symbolic::_DetermineNextAddresses(Symbolic::Context& rSymCtxt, Instruction const& rInsn, Address const& rCurAddr, Address::List& rNextAddresses) const
{
  auto PcExprs = rSymCtxt.BacktrackRegister(rCurAddr, m_PcRegId);
  if (PcExprs.empty())
  {
    rNextAddresses.push_back(rCurAddr + rInsn.GetLength());
    return true;
  }

  TrackedIdPropagation TrkIdProp(PcExprs, m_PcRegId);
  if (!TrkIdProp.Execute())
    return false;

  for (auto pExpr : PcExprs)
  {
    Log::Write("core").Level(LogDebug) << pExpr->ToString() << LogEnd;
  }

  // Should not happen since TIP returns only one expression
  if (PcExprs.size() != 1)
    return false;

  auto spPcExpr = PcExprs.front();
  NormalizeExpression NormExpr(spPcExpr);
  NormExpr.Execute();
  ConstantPropagation ConstProp(spPcExpr);
  ConstProp.Execute();

  Log::Write("core").Level(LogDebug) << "next addr expr " << spPcExpr->ToString() << LogEnd;
  EvaluateVisitor EvalVst(m_rDoc, rCurAddr + rInsn.GetLength(), rInsn.GetMode());
  spPcExpr->Visit(&EvalVst);

  auto spResExpr = EvalVst.GetResultExpression();

  // When the next address is a constant value
  auto spConstExpr = expr_cast<ConstantExpression>(spResExpr);
  if (spConstExpr != nullptr)
  {
    // HACK:
    Address DstAddr = rCurAddr;
    DstAddr.SetOffset(spConstExpr->GetConstant());
    rNextAddresses.push_back(DstAddr);
    return true;
  }

  // When the next address is still an address because it refers to an imported label
  auto spMemExpr = expr_cast<MemoryExpression>(spResExpr);
  if (spMemExpr != nullptr)
  {
    auto spBaseExpr = expr_cast<ConstantExpression>(spMemExpr->GetBaseExpression());
    auto spOffExpr = expr_cast<ConstantExpression>(spMemExpr->GetOffsetExpression());
    if (spOffExpr == nullptr)
      return false;
    Address DstAddr(rCurAddr.GetAddressingType(),
      spBaseExpr != nullptr ? spBaseExpr->GetConstant() : 0x0,
      spOffExpr->GetConstant(),
      spBaseExpr != nullptr ? spBaseExpr->GetSizeInBit() : 0x0,
      spOffExpr->GetSizeInBit());
    rNextAddresses.push_back(DstAddr);
    return true;
  }

  return false;
}

bool Symbolic::_ApplyCallingEffect(Address const& rImpFunc, Track::Context& rTrkCtxt, Symbolic::Block& rBlk)
{
  // TODO: store spOs in the object
  auto const OsName = m_rDoc.GetOperatingSystemName();
  auto spOs = ModuleManager::Instance().GetOperatingSystem(OsName);
  if (spOs == nullptr)
    return false;

  auto SymExprs = spOs->ExecuteSymbol(m_rDoc, rImpFunc);
  for (auto spExpr : SymExprs)
  {
    rBlk.TrackExpression(rImpFunc, rTrkCtxt, spExpr);
  }

  return true;
}