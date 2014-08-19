#ifndef __MEDUSA_SYMBOLIC_HPP__
#define __MEDUSA_SYMBOLIC_HPP__

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
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
    Block(u32 PcRegid = 0);
    Block(Block const& rBlk);
    Block& operator=(Block const& rBlk);

    ~Block(void);

    void SetPcRegisterId(u32 PcRegId) { m_PcRegId = PcRegId; }

    void TaintExpression(Address const& rAddr, Taint::Context& rTaintCtxt, Expression const* pExpr);
    void BackTrackId(Address const& rAddr, u32 Id, std::list<Expression const*>& rExprs) const;

    void AddParentBlock(Address const& rAddr);
    std::set<Address> const& GetParentBlocks(void) const;

    void AddConditionalExpression(Expression const* pExpr);
    Expression::List const& GetConditionalExpressions(void) const;

    bool Contains(Address const& rAddr) const;
    bool IsEndOfBlock(void) const;

    void ForEachAddress(std::function<bool(Address const& rAddress)> Callback) const;

  private:
    u32 m_PcRegId;
    std::set<Address> m_Addresses;
    std::set<Address> m_ParentBlocks;
    Expression::List  m_TaintedExprs;
    Expression::List  m_CondExprs;
  };


public:
  class Medusa_EXPORT Context
  {
  public:
    bool AddBlock(Address const& rBlkAddr, Block &rBlk);

    std::list<Expression const*> BacktrackRegister(Address const& RegAddr, u32 RegId) const;

    Taint::Context& GetTaintContext(void) { return m_TaintCtxt; }

  private:
    Taint::Context m_TaintCtxt;
    std::map<Address, Block> m_Blocks;
  };

  Symbolic(Document &rDoc);
  ~Symbolic(void);

  typedef std::function<bool (Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses)> Callback;

  bool Execute(Address const& rAddr, Callback Cb);

private:
  bool _ExecuteBlock(Symbolic::Context& rCtxt, Address const& rBlkAddr, Symbolic::Block& rBlk);

  bool _DetermineNextAddresses(Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses) const;

  Document& m_rDoc;
  u32 m_PcRegId;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SYMBOLIC_HPP__