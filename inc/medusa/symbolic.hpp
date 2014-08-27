#ifndef __MEDUSA_SYMBOLIC_HPP__
#define __MEDUSA_SYMBOLIC_HPP__

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/instruction.hpp"
#include "medusa/export.hpp"
#include "medusa/document.hpp"
#include "medusa/information.hpp"
#include "medusa/expression.hpp"

#include <tuple>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Symbolic
{
private:

  class Block
  {
  public:
    typedef std::function<bool(Expression::SPtr)> ExpressionMatcher;

    Block(u32 PcRegid = 0);
    Block(Block const& rBlk);
    Block& operator=(Block const& rBlk);

    ~Block(void);

    void SetPcRegisterId(u32 PcRegId) { m_PcRegId = PcRegId; }

    void TrackExpression(Address const& rAddr, Track::Context& rTrackCtxt, Expression::SPtr spExpr);
    void BackTrackId(Address const& rAddr, u32 Id, Expression::List& rExprs) const;

    void AddExpression(Expression::SPtr spExpr);
    Expression::SPtr BackTrackExpression(ExpressionMatcher Matcher) const;

    void AddParentBlock(Address const& rAddr);
    std::set<Address> const& GetParentBlocks(void) const;

    void AddConditionalExpression(Expression const* pExpr);
    Expression::List const& GetConditionalExpressions(void) const;

    bool Contains(Address const& rAddr) const;
    Address GetLastAddress(void) const;
    bool IsEndOfBlock(void) const;

    void ForEachAddress(std::function<bool(Address const& rAddress)> Callback) const;

  private:
    u32 m_PcRegId;
    std::set<Address> m_Addresses;
    std::set<Address> m_ParentBlocks;
    Expression::List  m_TrackedExprs;
    Expression::List  m_CondExprs;
  };


public:
  class Medusa_EXPORT Context
  {
  public:
    bool AddBlock(Address const& rBlkAddr, Block &rBlk);

    Expression::List BacktrackRegister(Address const& RegAddr, u32 RegId) const;

    Track::Context& GetTrackContext(void) { return m_TrackCtxt; }

  private:
    Track::Context m_TrackCtxt;
    std::map<Address, Block> m_Blocks;
  };

  Symbolic(Document &rDoc);
  ~Symbolic(void);

  typedef std::function<bool (Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses)> Callback;

  void FollowFunction(bool FollowFunction);

  bool Execute(Address const& rAddr, Callback Cb);

private:
  bool _ExecuteBlock(Symbolic::Context& rCtxt, Address const& rBlkAddr, Symbolic::Block& rBlk);

  bool _DetermineNextAddresses(Symbolic::Context& rSymCtxt, Instruction const& rInsn, Address const& rCurAddr, Address::List& rNextAddresses) const;
  bool _ApplyCallingEffect(Address const& rImpFunc, Track::Context& rTrkCtxt, Symbolic::Block& rBlk);

  Document& m_rDoc;
  CpuInformation const* m_pCpuInfo;
  u32 m_PcRegId;
  u32 m_SpRegId;
  bool m_FollowFunction;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SYMBOLIC_HPP__